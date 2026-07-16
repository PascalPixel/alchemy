# Raw

Local reconstruction of Golden Sun from one private ROM.

- `baserom.gba` is the sole game-specific input.
- `toolchain/gcc296/` is the native code-generation oracle.
- Host Python and ARM binutils provide generic analysis.
- `tools/` contains independent analysis and matching code.
- `src/` contains only byte-verified reconstructed C.
- `assets/` contains only source assets with exact ROM ranges and encoders.
- `python3 tools/build_claimed.py` links and verifies every claimed source region together.
- `python3 tools/build_assets.py` encodes and verifies every claimed asset.
- `python3 tools/build_full.py` verifies the combined byte-identical private rebuild.

Private inputs and generated artifacts are ignored; pushing is disabled.
Complete means these tracked sources and generic tools, given only the private ROM and approved compiler, independently build a byte-identical full ROM; every decompiled region comes from compiled reconstructed source.
