#!/usr/bin/env python3
import os
import sys
import shutil

def patch_file(filepath, replacements):
    if not os.path.exists(filepath):
        print(f"[!] Warning: {filepath} not found")
        return
    with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()

    modified = False
    for target, rep in replacements.items():
        if target in content:
            content = content.replace(target, rep)
            modified = True
            print(f"[+] Replaced '{target[:30]}...' in {filepath}")
        else:
            if "=" in rep and "\n" not in target:
                content += f"\n{rep}\n"
                modified = True
                print(f"[+] Appended '{rep}' to {filepath}")

    if modified:
        with open(filepath, "w", encoding="utf-8", newline="\n") as f:
            f.write(content)
        print(f"[+] Patched {filepath}")

def main():
    rd_dir = sys.argv[1] if len(sys.argv) > 1 else "."
    print(f"[*] Patching ramdisk in {rd_dir}")

    # 1. Patch prop.default
    patch_file(os.path.join(rd_dir, "prop.default"), {
        "ro.adb.secure=1": "ro.adb.secure=0",
        "ro.debuggable=0": "ro.debuggable=1",
        "persist.sys.usb.config=none": "persist.sys.usb.config=adb",
        "sys.usb.config=none": "sys.usb.config=adb",
    })

    # 2. Patch first_stage_ramdisk build.prop if present
    patch_file(os.path.join(rd_dir, "first_stage_ramdisk/system/etc/ramdisk/build.prop"), {
        "ro.adb.secure=1": "ro.adb.secure=0",
        "ro.debuggable=0": "ro.debuggable=1",
    })

    # 3. Patch system/etc/init/hw/init.rc
    init_rc = os.path.join(rd_dir, "system/etc/init/hw/init.rc")
    if os.path.exists(init_rc):
        with open(init_rc, "r", encoding="utf-8", errors="ignore") as f:
            lines = f.readlines()

        new_lines = []
        in_adbd = False
        in_console = False

        for line in lines:
            # Prevent panic on oops
            if "panic_on_oops 1" in line:
                new_lines.append("    write /proc/sys/kernel/panic_on_oops 0\n")
                continue

            # Track services
            if "service adbd" in line:
                in_adbd = True
            elif line.startswith("service "):
                in_adbd = False

            if "service console" in line:
                in_console = True
            elif line.startswith("service ") and not "service console" in line:
                in_console = False

            # Enable console service
            if in_console and line.strip() == "disabled":
                new_lines.append("    # disabled (enabled for root shell)\n")
                continue
            if in_console and "user shell" in line:
                new_lines.append("    user root\n")
                continue
            if in_console and "group shell" in line:
                new_lines.append("    group root log readproc\n")
                continue

            # Enable adbd service
            if in_adbd and line.strip() == "disabled":
                new_lines.append("    # disabled (force early start)\n")
                continue

            new_lines.append(line)

        content = "".join(new_lines)

        # Append early ADB triggers
        early_adb_trigger = """
on early-init
    setprop sys.usb.configfs 1
    setprop sys.usb.config adb

on post-fs
    setprop sys.usb.configfs 1
    setprop sys.usb.config adb
    start adbd

on boot
    setprop sys.usb.configfs 1
    setprop sys.usb.config adb
    start adbd
"""
        content += early_adb_trigger
        with open(init_rc, "w", encoding="utf-8", newline="\n") as f:
            f.write(content)
        print(f"[+] Patched {init_rc}")

    # 4. Wrap /system/bin/recovery to prevent init from rebooting when recovery crashes
    rec_bin = os.path.join(rd_dir, "system/bin/recovery")
    rec_real = os.path.join(rd_dir, "system/bin/recovery.real")
    if os.path.exists(rec_bin) and not os.path.exists(rec_real):
        shutil.move(rec_bin, rec_real)
        os.chmod(rec_real, 0o755)
        print(f"[+] Moved {rec_bin} -> {rec_real}")

        wrapper_script = """#!/system/bin/sh
# Recovery Keep-Alive Wrapper for Debugging
echo "=== Recovery Wrapper Started ===" > /tmp/recovery_wrapper.log
dmesg > /tmp/dmesg_boot.log 2>&1

# Run real recovery in background or foreground
/system/bin/recovery.real "$@" >> /tmp/recovery_wrapper.log 2>&1
RET=$?
echo "Real recovery exited with $RET. HOLDING SYSTEM ALIVE FOR ADB..." >> /tmp/recovery_wrapper.log
dmesg > /tmp/dmesg_after_recovery.log 2>&1

# NEVER exit! Keep init running so it does not reboot!
while true; do
    dmesg > /tmp/dmesg_latest.log 2>&1
    sleep 3
done
"""
        with open(rec_bin, "w", encoding="utf-8", newline="\n") as f:
            f.write(wrapper_script)
        os.chmod(rec_bin, 0o755)
        print(f"[+] Created keep-alive wrapper at {rec_bin}")

    # 5. Neutralize /system/bin/reboot to block userspace reboot triggers
    reboot_bin = os.path.join(rd_dir, "system/bin/reboot")
    reboot_real = os.path.join(rd_dir, "system/bin/reboot.real")
    if os.path.exists(reboot_bin) and not os.path.exists(reboot_real):
        shutil.move(reboot_bin, reboot_real)
        os.chmod(reboot_real, 0o755)
        print(f"[+] Moved {reboot_bin} -> {reboot_real}")

        block_reboot = """#!/system/bin/sh
echo "BLOCKED reboot command: $@" >> /tmp/reboot_blocked.log
dmesg > /tmp/dmesg_at_reboot_call.log 2>&1
# Do nothing - stay alive!
exit 0
"""
        with open(reboot_bin, "w", encoding="utf-8", newline="\n") as f:
            f.write(block_reboot)
        os.chmod(reboot_bin, 0o755)
        print(f"[+] Neutralized reboot binary at {reboot_bin}")

    # 6. Early USB gadget fallback script
    early_rc_path = os.path.join(rd_dir, "system/etc/init/early_adbd.rc")
    early_rc_content = """# Early ADB gadget fallback
on post-fs
    setprop sys.usb.configfs 1
    setprop sys.usb.config adb
    start adbd

on property:sys.usb.ffs.ready=1
    write /config/usb_gadget/g1/idProduct 0xd001
    write /config/usb_gadget/g1/configs/b.1/strings/0x409/configuration "adb"
    symlink /config/usb_gadget/g1/functions/ffs.adb /config/usb_gadget/g1/configs/b.1/f1
    write /config/usb_gadget/g1/UDC "a800000.dwc3"
    setprop sys.usb.state adb
"""
    os.makedirs(os.path.dirname(early_rc_path), exist_ok=True)
    with open(early_rc_path, "w", encoding="utf-8", newline="\n") as f:
        f.write(early_rc_content)
    print(f"[+] Created {early_rc_path}")

    print("[*] Ramdisk patching complete!")

if __name__ == "__main__":
    main()
