# Rules

1. Treat `baserom.gba` as the sole game evidence; never inspect or import another checkout, history, source, symbols, labels, pseudocode, scripts, outputs, or notes.
2. Generic tools and public architecture/compiler documentation are allowed; derive all game knowledge anew.
3. Track independent tools, byte-verified C, and byte-verified reconstruction assembly under `asm/` for regions the approved compiler cannot emit (entry stubs, runtime-library thunks, BIOS-call wrappers) or that lack an asm-free C match; keep the ROM, toolchains, dumps, generated analysis, compiler/disassembler output, and build products ignored. Inline `asm` inside C is disallowed: a function that only matches through register pins or asm barriers belongs in `asm/` as reconstructed disassembly until an asm-free C match is found.
4. Never transmit, paste, upload, or expose ROM bytes or anything derived from them; never give local content to network tools.
5. Inspect staged names before every commit and keep the tree source-only (no ROM bytes, dumps, toolchains, or build products). The repository is publishable as source under this invariant; publish or push only reconstructed source and generic tooling, never anything derived from the private ROM as bulk bytes.
6. Claim completion only when this branch, given the private ROM and approved compiler, independently builds a byte-identical full ROM; every claimed region must be emitted from reconstructed source, compiled C or assembled `asm/`, never copied ROM bytes. Assembly regions are reconstructed but not yet decompiled: a region is "decompiled" only once its C builds byte-identically, so full decompilation is reached when no `asm/` region remains except the genuinely compiler-unproducible stubs.

# Build environment

On Apple Silicon, put a native arm64 `arm-none-eabi-binutils` ahead of any
x86_64 one in `PATH`. The build shells out to `arm-none-eabi-as`, `ld`, `nm`,
`objcopy`, and `objdump` per region, so an x86_64 build of those tools makes
every region pay Rosetta translation plus Gatekeeper verification: measured
~21 ms per `as` spawn versus ~6 ms native, a 3.4x difference that dominates
any build, verify, or permuter loop (the top CPU consumers become `oahd` and
`taskgated`, not the compiler). `brew install arm-none-eabi-binutils` under
`/opt/homebrew` supplies a native build. Binutils **2.46.1 was verified
byte-identical to the previously pinned 2.33.1** across all claimed regions
(same `identical=True`, same region and byte counts), so the upgrade is safe
here; re-verify with a full `tools/build_full.py` before adopting any other
assembler version, since GAS canonicalization differences do change encodings
for some instructions.
