# Rules

1. Treat the explicitly supplied private ROM set as the complete game-evidence boundary: `gs1-{en,ja,de,es,fr,it}.gba` and `gs2-{en,ja,de,es,fr,it}.gba`. `gs1-en.gba` is the canonical byte-identical build target. The other approved ROMs may be compared locally to distinguish shared engine code and data from localization, graphics, and story content. Never inspect or import any other file from their containing checkout, or any other Golden Sun checkout, history, source, symbols, labels, pseudocode, scripts, outputs, or notes. Public decompilation projects such as `pret/pokeemerald` may be consulted only for generic repository, build, and publication conventions; never copy their game code, labels, assets, or game knowledge.
2. Generic tools and public architecture/compiler documentation are allowed; derive all Golden Sun game knowledge anew from the approved private ROM set. A cross-ROM match is evidence for shared layout or behavior, not evidence of authorship or an original name.
3. Track independent tools, byte-verified C, and byte-verified reconstruction assembly under `asm/` for regions the approved compiler cannot emit (entry stubs, runtime-library thunks, BIOS-call wrappers) or that lack an asm-free C match; keep the ROM, toolchains, dumps, generated analysis, compiler/disassembler output, and build products ignored. Inline `asm` inside C is disallowed: a function that only matches through register pins or asm barriers belongs in `asm/` as reconstructed disassembly until an asm-free C match is found.
4. Never transmit, paste, upload, or expose any approved ROM, raw ROM excerpt, copied ROM gap, cross-ROM binary diff, unconverted extraction dump, private analysis output, toolchain, or build product; never give private ROM content or private generated artifacts to network tools. Canonical reconstructed source is not prohibited merely because it was derived from the approved private ROMs.
5. Inspect staged names before every commit and apply the `pret/pokeemerald` source-tree boundary described in [`AGENTS.md`](AGENTS.md). Reconstructed C and assembly, scripts and text, semantic metadata, source PNGs and palettes, tilemaps and metatiles, and deterministic encoder or compression recipes are publishable when they are canonical, scoped build inputs. ROM images, copied ROM spans, opaque bulk extracts used as ROM substitutes, toolchains, dumps, generated previews or analysis, compiler/disassembler output, and build products are not publishable.
6. Claim the immediate Golden Sun English byte-closure milestone only when this branch, given canonical `gs1-en.gba` and the approved compiler, independently builds a byte-identical full ROM; every claimed region must be emitted from reconstructed source, compiled C or assembled `asm/`, never copied ROM bytes. Comparative ROMs are analysis evidence only and never become fallback build inputs. Claim project completion only when this one source tree likewise rebuilds all twelve approved ROMs and satisfies the semantic-asset and repository-quality gates in [`STATUS.md`](STATUS.md). Assembly regions are reconstructed but not yet decompiled: a region is "decompiled" only once its C builds byte-identically, so full decompilation is reached when no `asm/` region remains except the genuinely compiler-unproducible stubs.

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

**Pinned Bun version: 1.3.14** (`engines.bun` in `package.json`). This is
load-bearing, not advisory: `tools/zlib.ts` compresses every tracked PNG's
IDAT chunk, and DEFLATE has no single canonical encoding for a given
input — a different compressor build can produce different, equally valid,
equally decompressible bytes for the same pixel data. `tools/zlib.ts`
therefore uses `node:zlib` (Bun's Node-compatibility shim, backed by classic
zlib) rather than `Bun.deflateSync`/`Bun.inflateSync`/`Bun.hash`, which are
backed by libdeflate — a different, from-scratch compressor whose output
was confirmed build-configuration-dependent (an independent build of the
exact pinned libdeflate commit did not reproduce Bun's own output on the
same host). Classic zlib is deterministic for a given input, level, and
strategy regardless of CPU architecture, which is why it is the standard
choice for reproducible builds generally; verified directly, not assumed:
a fresh Bun 1.3.14 running as genuine linux/amd64 in Docker
(`--platform linux/amd64`, `oven/bun:1.3.14`) reproduces all 8,108 tracked
PNGs' IDAT bytes exactly against this host's macOS/arm64 output
(`checked=8108 mismatches=0`, 2026-07-24). Re-run that check before bumping
the pinned Bun version, since a future Bun could change its bundled
`node:zlib` backend the same way it already diverged from classic zlib for
its native `Bun.*` compression APIs.

# Publication boundary


This repository follows the practical source-tree boundary established by
[`pret/pokeemerald`](https://github.com/pret/pokeemerald). This is an
operational publication rule, not a claim that precedent creates a licence or
guarantees that a host or rights holder will agree with every publication.

The boundary is based on a file's role, not on the fact that reconstruction
began with private ROM evidence. This project may compare only the twelve
approved Golden Sun and Golden Sun: The Lost Age ROMs locally. Those comparisons
may guide one edition-aware reconstruction, but they do not make ROMs, binary
diffs, or private analysis into publishable source. `pokeemerald` publicly
tracks reconstructed C and assembly, map JSON, source PNG graphics and
palettes, and canonical binary map sources such as metatiles. It ignores the
built ROM, object and ELF files, generated hardware graphics encodings, dumps,
and build directories.

## Publishable source

- Reconstructed C and assembly, including byte-verified code that has not yet
  been decompiled to C.
- Scripts, text, tables, symbols, addresses, ranges, checksums, manifests, and
  semantic JSON required to understand or rebuild the game.
- Canonical source assets such as PNGs, palettes, audio sources, maps,
  tilemaps, metatiles, and animation data.
- Edition- and language-aware source metadata that lets one reconstruction
  express proven differences among the twelve approved build targets.
- Deterministic encoder descriptions and payload-free compression plans that
  turn those source forms back into the verified ROM representation.
- A narrowly scoped binary source format when it is the canonical named input
  for a reconstructed subsystem, is actively consumed by the build, and is
  not merely an opaque slice copied from the ROM.
- A bounded, canonical JSON byte-value region when it is an actively verified
  source input, names every reconstructed range, and is not a ROM, binary
  patch, or fallback artifact. It establishes byte closure only; a semantic
  producer should replace it when the data is understood.

Tracked assets must be authoritative build inputs, not disposable extraction
output. When a coherent semantic source replaces a provisional atlas or raw
representation, the superseded file is removed.

## Never publish

- Any of `gs1-{en,ja,de,es,fr,it}.gba` or
  `gs2-{en,ja,de,es,fr,it}.gba`, a built `.gba`, or any other playable ROM
  image.
- Binary patches, cross-ROM difference spans, or private comparison reports
  that reproduce or expose ROM content.
- Copied ROM gaps, committed `.incbin` payloads, or raw ROM ranges kept only to
  make an incomplete reconstruction link.
- Opaque bulk extraction dumps whose only function is to substitute for ROM
  bytes and which have not been promoted to a canonical source format.
- The private compiler bundle, toolchains, native dependencies, or other
  redistributable packages not authored as part of this repository.
- Object files, archives, ELFs, logs, generated linker output, analysis dumps,
  disassembler/compiler output, work directories, caches, and generated
  previews.
- Private ROM content or generated private artifacts sent to network tools.

## Commit gate

Before every commit, inspect all staged paths. A staged file passes only if it
is a canonical source input or independently authored generic tooling and is
needed by the tracked build or documentation. Verify that no forbidden file
above is staged, then run the relevant byte-identical build checks. Repository
size alone is not the boundary, but redundant and superseded assets must not
accumulate.

Commit subjects and public status text must name the demonstrated milestone:
use **byte closure** or **byte-identical, zero-fallback rebuild** for complete
address coverage. Do not say that all bytes are “identified,” “understood,” or
“decompiled” unless the relevant semantic or C-decompilation gate is also met.

The immediate verification target is `gs1-en.gba`. The other eleven approved
ROMs are local comparison evidence and future independent build targets; none
may supply fallback bytes to another target. Adding a target means expressing
its differences as canonical source or configuration and verifying its whole
output, not publishing a patch or a copied ROM span.

Useful upstream examples are `pokeemerald`'s
[`data/tilesets/primary/general`](https://github.com/pret/pokeemerald/tree/master/data/tilesets/primary/general),
which tracks `tiles.png`, palettes, `metatiles.bin`, and
`metatile_attributes.bin`; its semantic
[`LittlerootTown/map.json`](https://github.com/pret/pokeemerald/blob/master/data/maps/LittlerootTown/map.json);
and its [`.gitignore`](https://github.com/pret/pokeemerald/blob/master/.gitignore),
which excludes ROMs, compiled graphics encodings, object and ELF files, dumps,
and build output.
