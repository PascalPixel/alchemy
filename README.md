# Raw

Local reconstruction of Golden Sun from one private ROM.

- `baserom.gba` is the sole game-specific input.
- `toolchain/gcc296/` is the native code-generation oracle.
- Host Python and ARM binutils provide generic analysis.
- `tools/` contains independent analysis and matching code.
- `src/` contains only byte-verified reconstructed C.

Private inputs and generated artifacts are ignored; pushing is disabled.
