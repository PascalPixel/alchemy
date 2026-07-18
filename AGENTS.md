# Rules

1. Treat the explicitly supplied private ROM set as the complete game-evidence boundary: `gs1-{en,ja,de,es,fr,it}.gba` and `gs2-{en,ja,de,es,fr,it}.gba`. `gs1-en.gba` is the canonical byte-identical build target. The other approved ROMs may be compared locally to distinguish shared engine code and data from localization, graphics, and story content. Never inspect or import any other file from their containing checkout, or any other Golden Sun checkout, history, source, symbols, labels, pseudocode, scripts, outputs, or notes. Public decompilation projects such as `pret/pokeemerald` may be consulted only for generic repository, build, and publication conventions; never copy their game code, labels, assets, or game knowledge.
2. Generic tools and public architecture/compiler documentation are allowed; derive all Golden Sun game knowledge anew from the approved private ROM set. A cross-ROM match is evidence for shared layout or behavior, not evidence of authorship or an original name.
3. Track independent tools, byte-verified C, and byte-verified reconstruction assembly under `asm/` for regions the approved compiler cannot emit (entry stubs, runtime-library thunks, BIOS-call wrappers) or that lack an asm-free C match; keep the ROM, toolchains, dumps, generated analysis, compiler/disassembler output, and build products ignored. Inline `asm` inside C is disallowed: a function that only matches through register pins or asm barriers belongs in `asm/` as reconstructed disassembly until an asm-free C match is found.
4. Never transmit, paste, upload, or expose any approved ROM, raw ROM excerpt, copied ROM gap, cross-ROM binary diff, unconverted extraction dump, private analysis output, toolchain, or build product; never give private ROM content or private generated artifacts to network tools. Canonical reconstructed source is not prohibited merely because it was derived from the approved private ROMs.
5. Inspect staged names before every commit and apply the `pret/pokeemerald` source-tree boundary described in [`PUBLICATION.md`](PUBLICATION.md). Reconstructed C and assembly, scripts and text, semantic metadata, source PNGs and palettes, tilemaps and metatiles, and deterministic encoder or compression recipes are publishable when they are canonical, scoped build inputs. ROM images, copied ROM spans, opaque bulk extracts used as ROM substitutes, toolchains, dumps, generated previews or analysis, compiler/disassembler output, and build products are not publishable.
6. Claim completion only when this branch, given canonical `gs1-en.gba` and the approved compiler, independently builds a byte-identical full ROM; every claimed region must be emitted from reconstructed source, compiled C or assembled `asm/`, never copied ROM bytes. Comparative ROMs are analysis evidence only and never become fallback build inputs. Assembly regions are reconstructed but not yet decompiled: a region is "decompiled" only once its C builds byte-identically, so full decompilation is reached when no `asm/` region remains except the genuinely compiler-unproducible stubs.

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
here; re-verify with a full `bun tools/build_full.ts` before adopting any other
assembler version, since GAS canonicalization differences do change encodings
for some instructions.
