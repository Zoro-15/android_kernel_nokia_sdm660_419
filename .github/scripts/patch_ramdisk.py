#!/usr/bin/env python3
import os
import sys

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
            # If target not found and it's a prop setting, append it
            if "=" in rep and "\n" not in target:
                content += f"\n{rep}\n"
                modified = True
                print(f"[+] Appended '{rep}' to {filepath}")

    if modified:
        with open(filepath, "w", encoding="utf-8") as f:
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

    # 3. Patch system/etc/init/hw/init.rc to auto-start adbd without waiting for recovery
    init_rc = os.path.join(rd_dir, "system/etc/init/hw/init.rc")
    if os.path.exists(init_rc):
        with open(init_rc, "r", encoding="utf-8", errors="ignore") as f:
            lines = f.readlines()

        new_lines = []
        for line in lines:
            # Comment out 'disabled' in adbd service definition
            if line.strip() == "disabled" and len(new_lines) > 0 and "service adbd" in new_lines[-1]:
                new_lines.append("# disabled (force early start)\n")
                continue
            new_lines.append(line)

        content = "".join(new_lines)
        # Add early ADB trigger on post-fs and boot
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
        with open(init_rc, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"[+] Patched {init_rc} with early adbd triggers")

    # 4. Create an early USB gadget fallback script
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
    with open(early_rc_path, "w", encoding="utf-8") as f:
        f.write(early_rc_content)
    print(f"[+] Created {early_rc_path}")

    print("[*] Ramdisk patching complete!")

if __name__ == "__main__":
    main()
