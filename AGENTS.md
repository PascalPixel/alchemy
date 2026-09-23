# Alchemy

Alchemy is a decompilation of Golden Sun: **The Broken Seal (TBS)** ☀️ and
**The Lost Age (TLA)** ⚓️. It rebuilds each game byte for byte from readable C,
assembly and editable assets. Japanese releases are the source editions;
localizations are measured differences. Build IDs are `tbs` and `tla`.

This file is the only working guide. `README.md` is for fans. Read this file
once; do not copy it into prompts or notes.

## Our model: pret's pokeemerald

We work the way pret's pokeemerald did from 2015 to 2021, when it reached 100%
(a local checkout lives at `~/Developer/pret/pokeemerald`). When in doubt about
method, tooling, what to commit or how to measure, do what pokeemerald did.

The one deliberate difference is presentation: our tree looks like the project
Camelot most plausibly had on disk in 2001, not like pret's layout.

| pokeemerald | Alchemy |
| --- | --- |
| `src/`, `include/`, `asm/`, `data/`, `graphics/`, `sound/` | `games/<GAME>/SRC`, `INCLUDE`, `SOUND`, `TEXT`, assets beside their module; uppercase 8.3-style names such as `BATTLE/EFFECT/PARTICLE.C`, `FIELD_EVENT.H`, `CHAR_ISAAC.PNG` |
| snake_case files, CamelCase functions | uppercase files; `Subsystem_VerbObject` functions; short `pos`, `cnt`, `tbl`, `buf`, `work` locals; C89 |
| English map names | romaji place prefixes plus one area word from the Japanese ROM: `RUNPA_DOU`, `HAIDIA_MURA` |
| scaffolding in the same tree | our scaffolding lives apart in lowercase `recon/tbs` and `recon/tla` and shrinks to nothing at 100% |

## How we work

1. **Match first, polish later.** A function counts when its C compiles to the
   exact bytes. Names, types and comments improve over time, as pret's did; a
   placeholder name is fine until evidence gives a better one.
2. **Never throw work away.** A function that does not match yet keeps its C
   beside the assembly the build still links, pret's `NONMATCHING` pattern:
   the C lives in the module under `#ifdef NONMATCHING` (or in its
   `recon/<game>` draft) with its score and remaining difference named. It
   counts when it matches. A readable rewrite of a matching function is kept
   the same way until it matches too. Every attempt ends in a commit.
3. **Fake matches are allowed and tagged.** C that matches only through an
   odd construct (a reordered statement, a temporary that means nothing, a
   `register` hint) counts, carries a `/* FAKEMATCH */` comment saying what is
   odd, and is cleaned up later. What never counts: patched compiler output,
   bytes copied from the ROM into source, and changing the test or the
   measurement.
4. **Largest functions first.** Rank the unmatched functions by size, take the
   largest with the best evidence, and reuse what exact neighbours, callees and
   shared headers already prove. After three attempts without a new idea,
   commit the best draft and take the next one.
5. **Stop researching when it stops paying.** Work that cannot raise the
   percentage (compressor tails, packing, provenance archaeology) is timeboxed
   and never blocks a commit.

## Compiler and build

Game code uses the approved agscc bundle (GCC 2.96 based) through
`tools/alchemy/src/compiler/routing.rs`. As in pokeemerald, a whole source file
may use its own flags when the original evidently differed (pret builds its
library and flash files that way); record the reason beside the route. TLA
enables the reconstructed `-mgs2` lowering in agscc. Code the ROM copies into
RAM and runs in ARM state uses `-marm -mno-apcs-frame` (Pascal, 2026-09-23).
Compiler source changes, new binaries and new digests need Pascal's approval.

The build must always reproduce both English ROMs byte for byte:

```sh
./alchemy build full                 # TBS English
./alchemy build full --target tla-en # TLA English
make verify                          # the commit gate
```

Other editions compile but do not rebuild yet.

## Assets and compression

Assets are editable files the build converts, as in pokeemerald: PNG graphics,
JSON tables, MIDI sequences, WAV samples and PO text. Compressed data is
regenerated from them by our encoders. Where the general encoder does not
reproduce Camelot's stream, a per-file option (a window, a search limit, a
read-ahead) is recorded beside that file, as pret records `-num_tiles`; that
is a build setting, not cheating. Stored raw streams from the frozen
compression answers (TBS, commit `d08ee3a2`) and the six stored TLA streams
(Pascal, 2026-09-23) remain until an option or the encoder replaces them.

Game assets are tracked in the repository exactly as pokeemerald tracks them,
in the same shape: one indexed PNG per asset with its real palette (a
character, a portrait, a tileset), one identified BIN per tilemap or table,
and WAV, MIDI, JSON and PO. Never move a tracked asset out of the tree or
replace it with ROM extraction. What pret would not commit stays private: the
ROMs, the cartridge logo, and raw dumps. Today's giant grey sheets
(`CHAR_COMMON.PNG`, `TILE_BANK.PNG`), whole-area BIN bundles and the
unidentified `DATA.BIN` are dumps, restored from your own ROM
(`recon/<game>/private-inputs.json`, `--extract-sources`) until each is split
into pret-shaped assets and moved into its module.

## What may be committed

Code, tooling, our own documentation and the editable inputs the build
consumes. Never ROMs, never another project's Golden Sun work, never SDK or
leaked code. Compiler and binutils sources stay in their own licensed
repositories. Admissible evidence: your own ROMs, this repository, and public
language, hardware and compiler documentation. Methods may come from any
unrelated project, pokeemerald first.

## Measuring progress

**DONE = matching C + proven library or handwritten assembly**, over each
game's executable bytes, reported as ☀️ and ⚓️. `make progress` prints it and
`make progress-subject` gives the commit prefix. Report bytes, not rounded
percentages. Non-matching and fake-matching counts are tracked beside DONE, as
pret tracked its `NONMATCHING` list.

## Source tree

| SRC directory | Responsibility |
| --- | --- |
| SYSTEM | Startup, BIOS calls, IWRAM runtime, input, overlays (`OVERLAY.INC`); SCHEDULER, MEMORY, SAVE, LINK, RESOURCE |
| LIB | Game support and math; compiler runtime comes from its licensed container |
| GRAPHICS | DISPLAY, PALETTE, RENDER, TILE, TEXT, WINDOW; FONT, CHARACTER and COMMON hold assets |
| SOUND | Audio runtime; sound assets live in `games/<GAME>/SOUND` |
| GAME | Characters, party, items, inventory, Djinn and summons, flags |
| FIELD/COMMON | Shared field engine, maps, camera, objects, events, scripts |
| FIELD/<place> | One separately loaded area and its exclusive assets |
| BATTLE, MENU, DEBUG | Their game responsibilities |

A C file is one translation unit. Keep modules shallow and grouped by
responsibility, fold tiny folders into their parent, and give shared
declarations one header. Code shared by both games lives once in
`games/COMMON` after each game proves it exact. TBS editions share source and
differ only where measured, selected in `INCLUDE/VERSION.H`; each game keeps
its own compiler route and ROM. `SRC/FIELD/COMMON/KUUPUAPPU_RUNPA` is the
finished-module example: enums for scene IDs, named engine calls, typed tables.

Scaffolding in `recon/<game>`: drafts (`<edition>/main`, `<edition>/overlays`,
`en/units`), retained listings (`raw`), registries (`source-paths.json`,
`translation-units.json`, `source-bindings.json`, `assets.json`, `text.json`,
`locations.tsv`, `private-inputs.json`, `machine.json`). Generated output goes
to ignored `out/`.

## Assembly credit

Assembly counts toward DONE only when it is proven library code or
hand-written: a maintained `.S` module says so in its header
(`@ credit: library|handwritten — <object>`), and the build compares its bytes.
Unmatched compiler output is not handwritten, however stubborn. Overlay entry
trampolines built with `SRC/SYSTEM/OVERLAY.INC` are credited as reconstructed
veneers. Only Pascal changes credit standards.

## Working with agents

- Workflows need Pascal's approval before launch; the large-functions
  closers run without a separate verify stage and the lead verifies them.
- At most three agents at a time outside approved workflows. Each works on its
  own branch in a worktree **outside the checkout** (for example
  `../alchemy-worktrees/<name>`), commits a salvage commit before it stops, and
  is deleted when its work lands.
- The lead lands verified work on `main` at least daily as one squash commit
  through `make verify`, then removes landed branches. `main` is the only
  long-lived branch.
- Any tooling or input change invalidates both games' build proofs. Before a
  commit: re-extract private inputs that moved, then per game `build full`,
  `coverage audit --target <target> --inventory`, `build full`, then
  `make coverage`.
- Commit subjects start with `make progress-subject`'s prefix; agent commits
  end with their Co-Authored-By trailer. Push only when Pascal asks, only
  `main`.
- Scripts are TypeScript on Bun or Rust, never Python or shell scripts. The
  only prose files are `AGENTS.md` and `README.md`; no other notes anywhere.

## Tooling index

Prefer existing commands. There are two hosts and two crates; every immediate
tool directory must appear here, enforced by `make tooling-index-check`.
**Alchemy builds, Psynergy reads:** Alchemy owns game policy, paths, state,
compilation, encoding, linking and verification. Psynergy owns portable reading,
decoding, analysis and comparison over explicit input. No game-default ROMs,
owners or compiler routes in Psynergy, no aliases exposing an operation in both.

| Tool | Responsibility |
| --- | --- |
| [alchemy](tools/alchemy/) | Golden Sun command dispatch, twelve-target registry, owner lookup and extraction, source adoption, scene integration, compiler routes and provenance, candidate compilation, bindings, translation units, residual classification, matching catalog, overlay loading, serialization, assembly and audits, ROM stages, asset manifests, map networks, coverage, publication checks and dashboard. Its `compiler`, `recovery`, `score`, `matching`, `overlay`, `coverage` and asset and build modules are project integration. |
| [psynergy](tools/psynergy/) | Portable ARMv4T function, pool and jump-table discovery; Thumb decoding and assembly reconstruction; lifetime analysis, C recovery, normalization and alignment, structural and byte comparison, relocation-masked twin search, bounded C repair enumeration, GCC allocation-dump reading, format conversion, explicit subprocess execution, atomic writes, transactional cache storage, and image, MIDI, WAV, text, pixel, Huffman and LZ codecs. Callers supply images, addresses, symbols, paths, keys, formats and layouts; no Golden Sun owners, default ROMs or compiler routes. |

| Portable command | Responsibility |
| --- | --- |
| `psynergy decompile` | Recover draft C from an image with explicit base, entry and span; optional name and output. |
| `psynergy disassemble` | Read reachable Thumb instructions in the same explicit image window; no owner lookup or game symbol annotations. |
| `psynergy discover` | Walk an explicit GBA image to a fixed point across ARM and Thumb flow, pointers, literal pools and jump tables; optionally write its canonical machine report. |
| `psynergy reconstruct-asm` | Emit standalone ARMv4T assembly for an explicit complete Thumb extent, preserving reached instructions and in-extent data. |
| `psynergy diff` | Compare two supplied binary files, including length differences; `--width 1\|2\|4` sets the comparison unit. Exit 0 means equal bytes, 1 differences, 2 invalid input. No compilation or relocation. |
| `psynergy repair` | Enumerate one or two caller-named, guarded source repairs. Report the finite space; `--choice N` emits one alternative, optionally to `--out FILE`. No scoring, compiler selection or adoption. |
| `psynergy inspect allocator` | Read existing `.rtl`, `.lreg` and `.greg` GCC dumps from an explicit directory; no compiler invocation. |
| `psynergy convert` | Convert files using the directional formats below. No ROM offsets, engine headers or asset manifests. |

| Golden Sun command | Responsibility |
| --- | --- |
| `alchemy extract` | Resolve an owner and extract its reference bytes under ignored `out/`. |
| `alchemy inspect` | Resolve project call sites and symbols; `--asm` adds annotated owner disassembly; `--siblings` lists relocation-masked twins across images with status and binding equivalence. |
| `alchemy score` | Compile a candidate or whole declared unit with the approved route and compare its complete owner, including bindings and overlay serialization; `--unit ID --instance IMAGE \| --all-instances` scores unit instances, including explicitly declared main-image placements. It prints the scored owner's twin count and aligned halfword binary similarity (see [Completion](#completion-and-measurement)). |
| `alchemy match` | Resolve an owner, obtain a decoder-named repair, then compile and score bounded Psynergy alternatives under project policy. `--acceptance-test` checks the five catalog fixtures. |
| `alchemy adopt` | Verify and install a standalone overlay candidate; main integration uses `alchemy check integrate`. |
| `alchemy unit` | `scaffold` declares a main unit; `flatten` consolidates a verified overlay under project ownership. |
| `alchemy bootstrap` | Build and install a missing compiler toolchain from pinned sources. `--check` validates without building; `--build` rebuilds; `--from BUNDLE` imports an admitted distribution. |
| `alchemy build` | `compilers`, `asm`, `claimed`, `full`/`rom`, `assets` and `allocator`. Compiler source builds do not install a distribution. The allocator stage generates canonical GCC dumps for Psynergy inspection. `assets --network` draws map networks and assembles worlds ([Assets](#assets-and-local-viewers)). |
| `alchemy verify` | Run the staged repository's verification contract. |
| `alchemy coverage` | Rebuild and publish project coverage. `audit --target TARGET` inventories every ROM resource-directory pointer, physical spans only for byte-reproduced compressed streams, candidate executable overlay spans from canonical streams by ARMv4T control flow alone (loader veneers, owner-register entries, framed word-aligned Thumb function pointers, calls, branches and register-tracked GCC switch tables; word-aligned prologues, loaded Thumb pointers and gaps between proved functions only as candidates whose complete walks must end in control flow, discarded whole otherwise; listings are not evidence), and the bounded main image as the exact complement of ROM-verified asset regions. Raw pointers are hierarchical and never treated as file extents. The candidate goes to `out/<target>/reports/executable-audit-candidate.json`, never to the inventory path, and each run prints the digest of its overlay intervals. `--calibrate --expected LEDGER` lists every range differing from a ledger, as a diagnostic only. `--inventory` writes `out/<target>/reports/executable.json`: complete only when the overlay intervals hash to the game's verification record and a byte-identical full ROM build proves the main complement, otherwise pending, so progress reports `?`; it is pending while a run is in progress and after one fails. It never edits a committed file. |
| `alchemy check` | `publication`, `commit-progress`, `source-tracking`, `owners`, `tla-owners`, `coverage`, `integrate`, `no-asm`, `progress`, `routes` and `siblings`: repository contracts, not portable file operations. `progress` combines the generated executable inventory with the current verified build receipt ([Completion](#completion-and-measurement)); `--json` reports DONE and exact C separately, and `--write-report` writes that same result under `out/`. |
| `alchemy cross-edition` | Compare reviewed owner correspondence across Golden Sun editions. |
| `alchemy overlay` | `adopt`, `park`, `audit` and `export`: Golden Sun loader, resource integration and byte-identical retained-source export. |
| `alchemy dashboard` | Serve Files, ROM coverage, Music, Maps and six-edition Text debugging tabs locally from its `out/dashboard` cache. |
| `alchemy format` | Format native JSON; `--check` gates formatting and uppercase names. |

Retired entry points are rejected, not forwarded. Use Psynergy for `decompile`,
`discover`, `reconstruct-asm`, `disassemble`, `diff`, `repair` and `convert`; annotated owner disassembly is
`alchemy inspect OWNER --asm`. Old commands and logs are disposable diagnostics,
not instructions to resurrect aliases. `alchemy score --target tla` selects
TLA explicitly; arbitrary ROM overrides cannot substitute a reference. Scoring
one exact unit member still verifies the unit. Default work lives in out/score.

New tooling must fix a demonstrated recurring blocker, reuse or replace existing
code, and prove its behavior with regression coverage. Remove superseded
machinery instead of controlling complexity with a historical line-count quota.
`alchemy format` preserves JSON values, field order and boundaries using the
native two-space/120-column style and packed short tuples.

`psynergy convert FORMAT INPUT OUTPUT` supports decode-lz, words2bin, pairs2bin,
tilemap2bin, png2bpp4, bpp42png, png2bpp8, bpp82png, png2bgr555, wav2pcm8 and
pcm82wav. Bpp is tile-major, palettes little-endian BGR555, PCM8 signed; reverse
tiles need palette/tiles-wide, PCM-to-WAV needs rate, WAV input is mono 8-bit.
Invalid ranges, transparency and overwrites refuse. Engine headers stay in game
manifests. Encoding/repair operations still crossing the builds/reads boundary
are open work below, not permission to invent a third host.


## Build, verify and commit

```sh
git submodule update --init
git config core.hooksPath .hooks
make bootstrap           # pinned agscc/agbcc and binutils under tools/out
make verify              # staged tree: both ROMs, owners, publication, documents
make test                # tooling
make coverage            # progress figure and README status
```

Stage explicit paths. `make verify` rejects unstaged tracked changes and
untracked files.

## Open work

- ☀️ 55.87% → 75% needs 263,241 more DONE bytes in TBS: unregistered main-image
  code (295 KB), registered functions still in assembly (185 KB) and
  unregistered overlay code (68 KB), largest first.
- ⚓️ 2.14%: most TLA code is shared with TBS. Prove twins of exact TBS modules
  under the TLA route and move them into `games/COMMON`.
- Rewrite `BATTLE/ACTION_RESOLVE_TARGET_ACTION.C` as one readable function
  without per-edition knobs, and humanize the shared battle-effect and
  character interfaces the largest open functions depend on.
- Replace the stored compression streams with per-file encoder options.
- Map viewer: group each place with its rooms by world-map position; show
  story variants one at a time ("Vale: Stormy"). Music: restore the synth
  voices of the player retired on September 10.
- Later, as pokeemerald did: a `MODERN` build of the same source with a
  current compiler, and `BUGFIX` switches, for the recompilation.
