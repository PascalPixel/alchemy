#!/usr/bin/env python3
"""Link the full ROM from tracked source with a GNU linker script (pret model).

Unlike build_full.py, which overlays claimed regions onto a copy of the ROM,
this assembles the whole image the way pret decompilations do: every claimed C
and assembly function is compiled/assembled to an object placed at its fixed
address, every claimed asset is the byte output of its source encoder, and every
not-yet-reconstructed gap is `.incbin "baserom.gba", offset, size` (the skeleton
that shrinks as regions are claimed). One `arm-none-eabi-ld` invocation produces
the ELF; objcopy produces the .gba; the result is checked byte-identical.

Requires build_claimed.py, build_asm.py and build_assets.py to have populated
out/claimed, out/asm and out/assets first (this driver runs them).
"""
import argparse
import json
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ROM_BASE = 0x08000000
EXTERNAL = __import__("re").compile(r"(Func|Data|Value)_([0-9a-f]{8})")


def run(command):
    result = subprocess.run(command, cwd=ROOT, text=True,
                            capture_output=True)
    if result.returncode:
        raise RuntimeError((result.stderr or result.stdout).strip())
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("rom", type=Path, nargs="?", default="baserom.gba")
    parser.add_argument("-o", "--output", type=Path, default=ROOT / "out/rom")
    parser.add_argument("--jobs", type=int, default=8)
    args = parser.parse_args()
    rom = args.rom.read_bytes()
    out = args.output
    out.mkdir(parents=True, exist_ok=True)

    # Build the three source families; each writes a manifest of exact regions.
    run(["python3", "tools/build_claimed.py", str(args.rom),
         "--jobs", str(args.jobs), "--output", "out/claimed"])
    run(["python3", "tools/build_asm.py", str(args.rom), "--output", "out/asm"])
    run(["python3", "tools/build_assets.py", str(args.rom), "--output", "out/assets"])

    # Every claimed region contributes its exact verified bytes. Objects are
    # compiled/assembled from source and their bytes checked against the ROM by
    # build_claimed/build_asm; the C compiler pads sections differently from the
    # ROM's original (armcc) compiler, so pieces are incbin'd at their exact
    # size rather than ld-linked as padded objects. build_bytes/ holds the
    # per-region binaries the linker script places in address order.
    bytes_dir = out / "regions"
    bytes_dir.mkdir(exist_ok=True)
    pieces = []  # (address, size, kind, payload)
    claimed = json.loads((ROOT / "out/claimed/manifest.json").read_text())
    image = (ROOT / "out/claimed/claimed.bin").read_bytes()
    image_base = claimed["image_base"]
    for region in claimed["regions"]:
        start = region["address"] - image_base
        blob = bytes_dir / f"{region['address']:08x}.bin"
        blob.write_bytes(image[start:start + region["size"]])
        pieces.append((region["address"], region["size"], "incbin",
                       str(blob.relative_to(ROOT))))
    asm_manifest = json.loads((ROOT / "out/asm/manifest.json").read_text())
    for region in asm_manifest["regions"]:
        pieces.append((region["address"], region["size"], "incbin",
                       str(Path(region["output"]).relative_to(ROOT))))
    assets = json.loads((ROOT / "out/assets/manifest.json").read_text())
    for region in assets["regions"]:
        pieces.append((region["address"], region["size"], "incbin",
                       str(Path(region["output"]).relative_to(ROOT))))

    pieces.sort()
    # Fill every gap between claimed pieces with a baserom incbin skeleton.
    filled = []
    cursor = ROM_BASE
    for address, size, kind, payload in pieces:
        if address < cursor:
            raise SystemExit(f"overlapping piece at 0x{address:08x}")
        if address > cursor:
            filled.append((cursor, address - cursor, "skeleton", None))
        filled.append((address, size, kind, payload))
        cursor = address + size
    if cursor < ROM_BASE + len(rom):
        filled.append((cursor, ROM_BASE + len(rom) - cursor, "skeleton", None))

    # One object holds every region as its own section: claimed bytes and
    # asset bytes via `.incbin` of the verified per-region binary, skeleton gaps
    # via `.incbin "baserom.gba", offset, size` (the pret incbin skeleton).
    fill_lines = [".syntax unified"]
    placements = []
    for address, size, kind, payload in filled:
        name = f".rom_{address:08x}"
        fill_lines.append(f'\t.section {name}, "ax", %progbits')
        if kind == "incbin":
            fill_lines.append(f'\t.incbin "{payload}"')
        else:
            fill_lines.append(
                f'\t.incbin "{args.rom}", 0x{address - ROM_BASE:x}, 0x{size:x}')
        placements.append(f"    KEEP(out/rom/rom_fill.o({name}))")
    (out / "rom_fill.s").write_text("\n".join(fill_lines) + "\n")
    run(["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork",
         "-o", str(out / "rom_fill.o"), str(out / "rom_fill.s")])

    # Linker script: place every region's section in address order so the ROM
    # is contiguous from 0x08000000.
    (out / "ld_script.ld").write_text(
        "OUTPUT_ARCH(arm)\n"
        "MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\n"
        "SECTIONS\n{\n"
        "  .rom 0x08000000 : SUBALIGN(1)\n  {\n" + "\n".join(placements) +
        "\n  } > ROM\n"
        "  /DISCARD/ : { *(.comment) *(.note*) *(.ARM.*) }\n"
        "}\n")

    elf = out / "rom.elf"
    run(["arm-none-eabi-ld", "-T", str(out / "ld_script.ld"),
         "-o", str(elf), str((out / "rom_fill.o").relative_to(ROOT))])
    built = out / "rom.gba"
    run(["arm-none-eabi-objcopy", "-O", "binary", str(elf), str(built)])
    image = built.read_bytes()
    if image != rom:
        first = next((i for i in range(min(len(image), len(rom)))
                      if image[i] != rom[i]), None)
        raise SystemExit(
            f"linked ROM differs (size {len(image):#x} vs {len(rom):#x}, "
            f"first diff at 0x{ROM_BASE + first:08x})" if first is not None
            else f"linked ROM size {len(image):#x} vs {len(rom):#x}")
    claimed_bytes = sum(s for _, s, k, _ in filled if k != "skeleton")
    regions = sum(1 for _, _, k, _ in filled if k != "skeleton")
    print(f"identical=True pieces={len(filled)} regions={regions} "
          f"source_bytes={claimed_bytes} "
          f"skeleton_bytes={len(rom) - claimed_bytes}")


if __name__ == "__main__":
    main()
