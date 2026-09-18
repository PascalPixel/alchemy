# Tooling

Prefer using the tools to adding tools. Read a command's `--help` before
scripting around it, and scripts are TypeScript on Bun or Rust only.

## Tooling index

There are two public commands and two crates in one Cargo workspace: `alchemy`
for Golden Sun integration and `psynergy` for portable machinery. Every
immediate tool directory, including internal or game-specific libraries, must
be listed here, whatever its build dependency status. `make tooling-index-check`
enforces this. Never remove this inventory requirement or hide tool growth.

| Tool | Responsibility |
| --- | --- |
| [alchemy](../tools/alchemy/) | Golden Sun command dispatch, twelve-target registry, owner lookup and extraction, source adoption, scene integration, compiler routes and provenance, candidate compilation, bindings, translation units, residual classification, matching catalog, overlay loading, serialization, assembly and audits, ROM stages, asset manifests, map networks, coverage, publication checks and dashboard. Its `compiler`, `recovery`, `score`, `matching`, `overlay`, `coverage` and asset and build modules are project integration. |
| [psynergy](../tools/psynergy/) | Portable Thumb and objdump decoding, lifetime analysis, C recovery, normalization and alignment, structural and byte comparison, relocation-masked twin search, bounded C repair enumeration, GCC allocation-dump reading, format conversion, explicit subprocess execution, atomic writes, transactional cache storage, and image, MIDI, WAV, text, pixel, Huffman and LZ codecs. Callers supply addresses, symbols, paths, keys, formats and layouts; no Golden Sun owners, default ROMs or compiler routes. |

**Alchemy builds, Psynergy reads.** That is the law of the boundary. Alchemy
builds Golden Sun: it owns the project's state and policy, resolves owners, and
compiles, encodes, links and verifies. Its owner-aware commands that look at
the ROM, such as `extract` and `inspect`, serve that build and stay in Alchemy.
Psynergy reads a game: portable decoding, disassembly, decompilation and
comparison over explicit files and parameters, without a checkout and with no
Golden Sun defaults. The boundary is direction and authority, not
implementation language. A reusable reading operation belongs in Psynergy even
if its first caller is Alchemy. Do not move game defaults into Psynergy or
expose one operation through both hosts as aliases. Where a tool still crosses
the law, `TODO.md` tracks the move.

Psynergy's commands use the same library as Alchemy's internal integration,
sharing decoding and comparison across recovery, scoring and overlays; BL-shaped
data is not automatically executable code. Its dependencies include `regex`,
`rusqlite`, `png`, and test-only `tempfile`. Alchemy owns cache identity,
project paths and compiler fingerprints. Do not duplicate these
responsibilities in another wrapper or registry.

| Portable command | Responsibility |
| --- | --- |
| `psynergy decompile` | Recover draft C from an image with explicit base, entry and span; optional name and output. |
| `psynergy disassemble` | Read reachable Thumb instructions in the same explicit image window; no owner lookup or game symbol annotations. |
| `psynergy diff` | Compare two supplied binary files, including length differences; `--width 1\|2\|4` sets the comparison unit. Exit 0 means equal bytes, 1 differences, 2 invalid input. No compilation or relocation. |
| `psynergy repair` | Enumerate one or two caller-named, guarded source repairs. Report the finite space; `--choice N` emits one alternative, optionally to `--out FILE`. No scoring, compiler selection or adoption. |
| `psynergy inspect allocator` | Read existing `.rtl`, `.lreg` and `.greg` GCC dumps from an explicit directory; no compiler invocation. |
| `psynergy convert` | Convert files using the directional formats below. No ROM offsets, engine headers or asset manifests. |

| Golden Sun command | Responsibility |
| --- | --- |
| `alchemy extract` | Resolve an owner and extract its reference bytes under ignored `out/`. |
| `alchemy inspect` | Resolve project call sites and symbols; `--asm` adds annotated owner disassembly; `--siblings` lists relocation-masked twins across images with status and binding equivalence. |
| `alchemy score` | Compile a candidate or whole declared unit with the approved route and compare its complete owner, including bindings and overlay serialization; `--unit ID --instance IMAGE \| --all-instances` scores unit instances. It prints the scored owner's twin count. |
| `alchemy match` | Resolve an owner, obtain a decoder-named repair, then compile and score bounded Psynergy alternatives under project policy. `--acceptance-test` checks the five catalog fixtures. |
| `alchemy adopt` | Verify and install a standalone overlay candidate; main integration uses `alchemy check integrate`. |
| `alchemy unit` | `scaffold` declares a main unit; `flatten` consolidates a verified overlay under project ownership. |
| `alchemy bootstrap` | Build and install a missing compiler toolchain from pinned sources. `--check` validates without building; `--build` rebuilds; `--from BUNDLE` imports an admitted distribution. |
| `alchemy build` | `compilers`, `asm`, `claimed`, `full`/`rom`, `assets` and `allocator`. Compiler source builds do not install a distribution. The allocator stage generates canonical GCC dumps for Psynergy inspection. `assets --network` draws map networks and assembles worlds ([ASSETS](ASSETS.md)). |
| `alchemy verify` | Run the staged repository's verification contract. |
| `alchemy coverage` | Rebuild and publish project coverage. `audit --target TARGET` inventories every ROM resource-directory pointer, physical spans only for byte-reproduced compressed streams, candidate executable overlay spans from canonical streams and assembler source-line evidence, and the bounded main image as the exact complement of ROM-verified asset regions. Raw pointers are hierarchical and never treated as file extents. `--calibrate` must reproduce the completed TBS audit before any executable method can become authoritative. The audit writes a candidate under `out/`; it never edits the committed scoring manifest. |
| `alchemy check` | `publication`, `commit-progress`, `source-tracking`, `owners`, `tla-owners`, `retained`, `coverage`, `integrate`, `no-asm`, `plan-tails`, `overlay-data`, `progress`, `routes`, `showcase` and `siblings`: repository contracts, not portable file operations. `progress` combines the canonical executable inventory with the current verified build receipt ([COMPLETION](COMPLETION.md)); `--json` reports DONE and exact C separately, and `--write-report` writes that same result under `out/`. |
| `alchemy cross-edition` | Compare reviewed owner correspondence across Golden Sun editions. |
| `alchemy overlay` | `adopt`, `park`, `audit` and `export`: Golden Sun loader, resource integration and byte-identical retained-source export. |
| `alchemy dashboard` | Serve project coverage. |
| `alchemy format` | Format native JSON; `--check` gates formatting and uppercase names. |

Retired entry points are rejected, not forwarded: `alchemy decompile`,
`alchemy convert` and `alchemy disassemble` belong to Psynergy; owner-annotated
disassembly is `alchemy inspect OWNER --asm`; the old `alchemy diff` is
`alchemy score`; the old `alchemy inspect allocator` is split into dump
generation and reading as above. Historical dossiers keep their original
command transcripts; they are not instructions to resurrect aliases. No retired
TypeScript entry points or invented family or wave commands.

Overlay scoring selects the game explicitly: `alchemy score RESOURCE:ADDRESS
--target tla` reads TLA's register, reviewed boundaries and canonical English
ROM; omit `--target` for TBS. Arbitrary `--rom` overrides cannot substitute a
different reference. Selecting a member of an exact overlay unit still verifies
the complete unit. Default score work directories are under ignored `out/score/`.

For another GBA game, carry portable decoding, comparison, bounded recovery and
compiler invocation plus its minimal build integration, not Golden Sun
ownership, asset offsets or agent machinery.

Portable tooling is capped at **100,000 Rust, TypeScript, JavaScript and CSS
lines** by `make tooling-size`. Pascal owns that ceiling and scope; do not raise
it or hide code outside it. New tooling must resolve a demonstrated recurring
blocker, reuse or replace existing machinery, and prove a conversion with
regression coverage.

## Formats

`alchemy format` writes native JSON with two-space indentation, a 120-column
layout, compact records and packed short data tuples. Long individual strings
stay intact; field order, values and recorded binary boundaries are preserved.

`psynergy convert FORMAT INPUT OUTPUT [options]` supports `decode-lz`, `words2bin`,
`pairs2bin`, `tilemap2bin`, `png2bpp4`, `bpp42png`, `png2bpp8`, `bpp82png`,
`png2bgr555`, `wav2pcm8` and `pcm82wav`. Bpp formats are GBA tile-major pixels,
BGR555 is little-endian palette data, and PCM8 is signed sample data. Reverse
tile conversion requires `--palette FILE --tiles-wide N`; PCM-to-WAV requires
`--rate HZ`. WAV input is canonical mono 8-bit PCM. Invalid palette,
transparency or index data and overwriting an existing output are refused, not
silently discarded. Engine headers and loop metadata stay in asset manifests.
Thumb is code, not a music format. Keep format names portable and directional.

## Dashboard and progress figure

`make coverage` and `make coverage-check` read existing verified build receipts;
they do not run a build or an executable audit. If source verification is stale,
run the relevant build or owner check first. `make verify` still requires the
full production build. Executable audits are explicit operations, not a side
effect of rendering progress. Historical refs require their own checked-out,
verified inputs; a current receipt cannot score a different revision.

The dashboard at `http://127.0.0.1:4650/` shows one full-window ROM tree of
files and folders, the same hierarchy as the README's progress figure; it has
no music player. Click a folder to open it and use Back to return. Addresses
belong in hover details, not tile captions. Labels in both use one 13px system
sans-serif font that does not scale with the layout. No game font is served or
embedded. The Lost Age executable audit separates code still represented as
assembly from cartridge data; exact C takes precedence where independently
proved. This display classification never grants DONE credit by itself.

`make dashboard-service-install` installs its macOS login service;
`make dashboard-restart` restarts it. Coverage inputs are watched, but the
service keeps the binary it started with, so restart it after any tooling merge
or it computes DONE under the old rules. Rust renders the dashboard and handles
folder navigation, file details and shared-resource links through HTML
requests. It serves no JavaScript, and its content security policy forbids
scripts. HTML forms can reveal a source in Finder through same-origin POST.

The figure keeps a 9:16 frame and the dashboard fills its window; both count
represented ROM bytes once while keeping the executable-only DONE denominator,
and show both games under `games/` by their real folder names. An area's shared
files link map resources to their existing source folders without adding
tiles or bytes; these links derive from the scene selector, map-load table,
resource directory and Atlas destinations and do not inventory every non-map
asset an area uses. Data-file details report the target's last asset-build
comparison separately from its content type; a ROM-byte match does not verify
picture layout or colors, and missing comparison evidence never shows as a
match. The viewer follows leaf files across package boundaries using the
build's recorded physical extents; compression counts at stored size, and
missing or inconsistent extents keep a package unsplit rather than guessing.
