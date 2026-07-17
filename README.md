# goldensun-decomp-pure

An all-AI, for-fun attempt at decompiling Golden Sun (GBA). Every line of the
reconstruction and tooling in this repository was written by an AI (Anthropic's
Claude, via Claude Code) as an experiment in how far a language model can get at
a clean-room decompilation on its own. It is a hobby/research project, not a
serious or community-driven decomp, and it is nowhere near complete.

A clean-room, in-progress decompilation of Golden Sun (GBA), reconstructed
entirely from a single privately-supplied ROM using only generic tools.

This repository is **source only**. It ships no ROM, no game binaries, and no
playable output. To build or verify anything you must supply your own legally
obtained Golden Sun ROM as `baserom.gba`; it must match the checksum in
`rom.sha1`. Every not-yet-reconstructed region is filled from that private ROM
at build time (the pret-style incbin skeleton), so nothing here reproduces a
playable game on its own. Not affiliated with or endorsed by Nintendo or
Camelot. Non-commercial.

- `baserom.gba` (your own dump, git-ignored) is the sole game-specific input.
- `toolchain/gcc296/` is the native code-generation oracle.
- Host Python and ARM binutils provide generic analysis.
- `tools/` contains independent analysis and matching code.
- `src/` contains only byte-verified reconstructed C, with no inline `asm`.
- `asm/` contains byte-verified reconstruction assembly for regions the approved
  compiler cannot emit (the ROM-start dispatch stubs, the runtime-library
  interworking thunks, the BIOS-call wrappers) and for functions that do not yet
  have an asm-free C match. These regions are reconstructed but not yet
  decompiled; each becomes a `src/` C file once its C builds byte-identically.
- `assets/` contains only source assets with exact ROM ranges and encoders.
- `assets/code/` holds the EWRAM code overlays that ship compressed in the ROM,
  reconstructed as Thumb assembly (control-flow-walked disassembly with the
  pointer tables kept as data) plus the exact compression plan; the build
  assembles the overlay, re-compresses it, and checks the result byte-for-byte.
- Map tile PNGs preserve sequential 4bpp tiles and virtual IDs; they are not
  presented as composed artwork. They are stored 8bpp so each tile can carry
  the palette bank the map assigns it (pixel = bank * 16 + index), which shows
  true colors while the low nibble still round-trips to the exact 4bpp indices;
  index 0 is marked transparent, as the hardware treats it.
- `python3 tools/build_claimed.py` links and verifies every claimed C region together.
- `python3 tools/build_asm.py` assembles and verifies every claimed `asm/` region.
- `python3 tools/build_assets.py` encodes and verifies every claimed asset.
- `python3 tools/build_full.py` verifies the combined byte-identical private rebuild.
- `python3 tools/build_rom.py` assembles the ROM the pret way: a generated GNU
  linker script (`ld_script.ld`) lays out every claimed region in address order
  and fills each not-yet-reconstructed gap with `.incbin "baserom.gba", offset,
  size` (the skeleton that shrinks as regions are claimed), linked with
  `arm-none-eabi-ld` to a byte-identical ELF. A pure link of compiled objects at
  fixed addresses needs the ROM's original compiler (gcc pads sections
  differently), so each region contributes its verified bytes; the linker-script
  and incbin-skeleton structure follows pret/pokeemerald, the golden reference.

The private ROM, toolchains, and generated artifacts are git-ignored; only
reconstructed source and generic tools are tracked. Complete means these tracked
sources and generic tools, given only your ROM and the approved compiler,
independently build a byte-identical full ROM; every claimed region comes from
reconstructed C or assembly, never copied ROM bytes. Full decompilation is the
further goal of retiring every `asm/` region except the genuinely
compiler-unproducible stubs.
