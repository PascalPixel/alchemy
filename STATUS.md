# Current status

Snapshot: 2026-08-13. This is the live status surface. Dated investigations and
their original measurements are archived under [`docs/history/`](docs/history/).

## Progress

- **Exact C:** 376,432 / 1,347,264 executable bytes (**27.94%**).
- **Public DONE:** **33%**, combining Exact C with audited permanent assembly.
- The generated coverage maps and dashboard are the live per-overlay source of
  truth; this page records the current contributor frontier rather than a
  second hand-maintained per-overlay ledger.
- Exact C is the contributor progress counter. DONE answers the public
  completion question by also including code deliberately retained as permanent
  assembly. Semantic C, function counts, and queue sizes are diagnostics.

## Working loop

1. Pick an owner with the dispatcher commands `semantic_queue` or
   `overlay_twins --leads`.
2. Reconstruct and review the owner in `semantic/` until its boundaries,
   control flow, data flow, calls, and side effects are credible.
3. Use witnessed sibling shapes and bounded deterministic source searches for a
   small residual. Search compiler modes only when the residual supports it.
4. Use `tools/alchemy-permuter` only as a bounded rescue search. Candidates must
   preserve C dependencies and compile to the owner; a near-match is not an
   adoption.
5. Adopt an exact result through the owner-specific gate: `integrate_matches`
   for main-image drafts or `overlay_adopt` for code overlays. Ensure the
   diagnostic inventory exists with `make inventory` before overlay adoption,
   then regenerate metrics, run verification, and commit with the required
   progress-bearing subject.

The authoritative gate is `make verify`. During iteration, use `make test`,
`make lint`, `make progress`, and the focused build stages documented in
[`AGENTS.md`](AGENTS.md).

## Tooling state

The root Makefile is the workflow facade: its `dispatch-*` targets invoke the
dispatcher registry, while its hyphenated build and check targets invoke the
native implementations directly. The completed audit leaves 128 Rust crates
and 121 binary targets. Nine
dispatcher groups expose 99 public commands; all 22 remaining binaries are
explicitly classified as internal diagnostics, support/self-test hosts, or
benches. The native runner executes 112 runtime self-tests, including every
public command. Architecture rejects an unclassified or stale binary and the
documentation gate checks every public command against
[`docs/TOOLS.md`](docs/TOOLS.md).

One genuine orphan, `text-bg`, was removed after its behavior was proved to be
owned by active asset crates. The other library and internal crates have real
consumers or demonstrated diagnostic roles. The full trial and rationale are
recorded in
[`docs/history/2026-08-09-native-tool-audit.md`](docs/history/2026-08-09-native-tool-audit.md).

The pinned compiler fork now has admitted linux-x64 runtime digests for the
gs1 `cc1`, the gs2 bundle, and `old_agbcc`, each proven by the zero-regression
routed corpus and a byte-identical `build-full` before admission; the full
byte-exact pipeline is exercised end to end on linux hosts. Fresh-environment
provisioning notes: the mandatory bundle surface still includes the
comparison probes `dist/pret-early-thumb/cc1` and `dist/gcc2951/cc1`, whose
source trees were pruned from the `alchemy-gcc` head in `382014a` — build
them from the pre-prune commit (`382014a^`, `./build.sh pretearlythumb`
and `./build.sh gcc2951`) and stage the two `cc1` binaries by hand. The
`source_citations` gate also needs full git history, so unshallow a
depth-limited clone before running `make lint`.

Recent operational findings are reflected in the native tools: the permuter is
dependency-aware, bounded, and restricted to safe output cleanup; compiler
corpus selection uses `exact/*.c`; semantic and remaining-owner tools work from
the repository root; overlay scans reject invalid input instead of hiding scan
errors; and the dashboard supports both localhost and the configured LAN host
on port 4649.

The bounded-round protocol is currently enforced by coordinator ownership plus
the existing compiler, architecture, adoption, and verification gates. Native
round manifests, shared-output leases, and transactional adoption are not yet
implemented. Until they are, agents must treat generated outputs and adoption
as serialized shared state exactly as required by
[`CONTRIBUTING.md`](CONTRIBUTING.md).

## Primary target index

[`TARGETS.md`](TARGETS.md) is the main contributor queue. It is generated from
the same exhaustive owner partition as the dashboard, sorted largest to
smallest, and contains every scope of at least 1,000 bytes. The complete
machine-readable index in
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json) retains
all 1,727 unfinished scopes down to the smallest owner.

The index accounts for all 1,347,264 executable bytes: 891,846 target bytes are
reviewed semantic C, and 451,890 bytes occur only in Exact C or audited
permanent assembly scopes. Its rows never overlap. There are currently seven
independent unfinished owner scopes of at least 5 kB;
the first ten rows are:

| Rank | Namespace / owner | Scope |
|---:|---|---:|
| 1 | `main:0x080bbb0c` | 6,332 bytes |
| 2 | `resource_3bd:0x020013f8` | 6,220 bytes |
| 3 | `resource_380:0x020027f8` | 5,932 bytes |
| 4 | `main:0x080ea0d8` | 5,756 bytes |
| 5 | `resource_3bf:0x02003054` | 5,604 bytes |
| 6 | `resource_373:0x020015dc` | 5,240 bytes |
| 7 | `resource_381:0x02001410` | 5,136 bytes |
| 8 | `main:0x080ab5e4` | 4,888 bytes |
| 9 | `resource_39d:0x02001af0` | 4,840 bytes |
| 10 | `resource_3c9:0x02002360` | 4,708 bytes |

`make coverage` regenerates both forms and `make coverage-check` rejects a
stale ranking. This owner-level list is the authoritative byte-accounted
priority surface. The broader cuts below are secondary coordination views:
they deliberately include exact witnesses or combine related owners, and may
therefore overlap one another.

## Campaign cuts (secondary; may overlap)

The default unit of progress is now a bounded logic core rather than an
isolated owner. A core is a measured owner family or contiguous C island with a
coherent internal model and an explicit perimeter. Calls, tables, callbacks,
hardware services, and uncertain dispatch arms at that perimeter may remain
semantic interface shims while the centre is reconstructed. They may not be
guessed into Exact C: every owner still needs production-path byte proof before
adoption.

States are `surveyed`, `selected`, `active`, `complete`, and `rejected`.
Measured scope includes each owner's literal-pool bytes. Boundary counts are
estimates until a core brief records a frozen call census.

| Priority | State | Core target | Measured scope | Current ownership | Cut and evidence |
|---:|---|---|---:|---:|---|
| 1 | complete | Configurable spawn/copy family | 5,192 executable bytes, 22 owners | family spawners exact | Eleven overlays share a 472-byte spawner and 56-byte integrator. The six witnessed semantic copies in `resource_3c9`, `39c`, `39d`, `39e`, `380`, and `3a5` were adopted 2026-08-12. |
| 2 | selected | `resource_3ba` / `3bb` paired render tasks | 2,888 bytes, two mirrored 1,444-byte slices | 0 exact / 2,888 semantic | `3ba:31c0-3764` and `3bb:3458-39fc` cross-check one another and each has only eight distinct imported callees. Exact adjacent installers at `3ba:3764` and `3bb:39fc` are the cut seam. |
| 3 | selected | `resource_39f` slot procession | 2,896 bytes, 11 adjacent owners | 2,608 exact / 288 semantic | Ten exact owners now constrain the remaining `39f:15d0` owner. Keep slot records, party-heading reads, and the `Data_02000240 + 0x22b` completion byte at the perimeter. `1d04` was adopted byte-exact on 2026-08-12. |
| 4 | surveyed | Main menu presentation/OAM | `0x080f6000-0x080f86f8`, 9,976 bytes | 604 exact / 9,364 semantic / 8 retained | A repeated update/render pipeline around `080f6440`, `080f7460`, and `080f7f78`. DMA, renderer slots 46/47, and allocation are shims; per-frame state and OAM construction are the core. |
| 5 | surveyed | Main scene/effect C island | `0x080da2ac-0x080dd2ac`, 12,288 bytes | 226 exact / 12,062 semantic | A contiguous zero-assembly island with repeated renderer and scene plumbing. Cut at renderer-table dispatch, resource services, and IWRAM helpers. This is the cleanest single main-image subsystem boundary. |
| 6 | active | Staged actor movement/redraw | 6,380 executable bytes in five overlays | 3,480 exact / 2,900 semantic | All five 696-byte movement owners are now exact. The remaining probe/redraw owners share the same displacement tables, orientation tables, and callback perimeter. |
| 7 | surveyed | Command/interpolation renderer | 3,792 bytes, three owners | 0 exact / 3,792 semantic | `3ba:21b8`, `3bb:2450`, and `3bc:2ee8` share the queue decoder, three interpolation passes, mode tail, and ten-call sequence. Private state cells, callback, palette, and cache addresses form the perimeter. |
| 8 | surveyed | Main interactive comparison UI | `0x080ab314-0x080ae714`, 13,312 bytes | 1,048 exact / 12,264 semantic | `080ab5e4` and `080ad6d4` form a screen-local state-machine family. Generic `080150xx` window/OAM and `080770xx` runtime services stay outside the cut. |
| 9 | surveyed | Main battle action/script core | `0x080ba27c-0x080bdfec`, 15,728 bytes | 1,338 exact / 14,378 semantic / 12 retained | `080bd898` drives the script state and invokes resolver `080bbb0c`. The centre is action decode, target resolution, message, and cleanup; renderer, audio, and resource services are shims. High call density makes this valuable but risky. |
| 10 | surveyed | `resource_3c5` paired cutscene drivers | 4,660 bytes | 0 exact / 4,660 semantic | Two bounded scripts share a source model and one local-helper seam, but expose 29-30 imports and more than 500 call sites. Recover one type/vocabulary contract without introducing a shared C helper. |

Reserve targets, in order, are the 3,132-byte entity/handle family, the
568-byte paired attachment constructor, the 6,220-byte `resource_3bd` scene
driver, the 8,956-byte `resource_3c9` choreography block, and the 18,600-byte
`resource_373` cinematic campaign (whose non-overlapping script island is
16,312 bytes). The last three are semantic-architecture
campaigns until their call perimeters are narrower; shared calls alone do not
prove a reusable source skeleton.

### Five-kilobyte assault map

The 2026-08-12 large-core survey replaced broad subsystem labels with audited
cuts. These are real contiguous logic cores or repeated families, not totals
manufactured from unrelated neighbours.

| Priority | Core | Audited scope | Fresh semantic opportunity | Exact leverage |
|---:|---|---:|---:|---|
| 1 | Staged actor probe/move/redraw family (`373`, `389`, `391`, `392`, `393`) | 6,380 bytes | 2,900 bytes | All five 696-byte movement owners are exact; their proved ABI and local-call maps now constrain the remaining probe/redraw owners at the clean `034c`, `08c0`, and `09dc` seams. |
| 2 | Battle action preparation / target selection | `080ba27c-080bbb0c`, 6,288 bytes | 5,146 bytes | 1,134 exact bytes constrain three action builders and the 1,864-byte target selector. |
| 3 | Comparison-render subsystem | `080ac8fc-080ae714`, 7,704 bytes | 6,656 bytes | 1,048 exact bytes fix the renderer ABI, portrait/motion infrastructure, and both outer seams. |
| 4 | Main scene launch suite | `080da2ac-080db6e0`, 5,172 bytes | 5,148 bytes | Two exact tail wrappers witness the following renderer modes; four scene owners share one runtime/render vocabulary. |
| 5 | Main effect/render suite | `080db6e0-080dd2ac`, 7,116 bytes | 6,914 bytes | Seven exact mode wrappers constrain `080dbc30`; exact allocator and dispatch owners bound the suite. |
| 6 | Menu/OAM controller | `080f60a0-080f7db4`, 7,444 bytes | 7,444 bytes | Exact entry/palette helpers bound the front; a 452-byte exact barrier separates the independent compressor. |
| 7 | `resource_3c9` choreography | `12c8-35c4`, 8,956 bytes | 8,620 bytes | Three semantic scripts are interleaved with 336 exact helper/callback bytes and exact outer seams. |
| 8 | `resource_373` cinematic script island | `15dc-5594`, 16,312 bytes | 8,032 bytes | The now-exact 5,604-byte `3fb0` scene driver joins five exact setup/dispatcher owners, leaving three bounded semantic scripts with 8,280 exact witness bytes around them. |
| 9 | Queued battle-action resolver | `080bbb0c-080bd3c8`, 6,332 bytes | 6,332 bytes | One coherent owner; exact queue emitter is called 154 times, but no duplicate source witness exists. |
| 10 | `resource_391` cinematic scene driver | `0d3c-2768`, 6,700 bytes | complete | Adopted byte-exact on 2026-08-12. Exact local helpers, installed callbacks, and the sibling scene-sheet witness closed its perimeter. |

The staged-actor round corrected the raw `0xb084` prologue classification,
transferred the exact by-value movement ABI to `389`, `391`, `392`, and `393`,
and independently resolved each overlay's local calls, tables, and callback
veneer. All five movement owners now account for 3,480 exact bytes. Redraw
residuals previously reached 74 bytes in `373` and 72 bytes in `392`; those
remaining owners are reconstruction targets, not permutation-ready residuals.

For each selected core, create one coordinator brief that freezes its owner
set, byte accounting, exact witnesses, import/export seam, compiler routes, and
adoption order. Do not expand the core mid-round. A core is complete only when
every centre owner is exact or explicitly moved outside the centre as a
reviewed semantic edge shim, and the resulting build passes `make verify`.

### `resource_39e` scene transfer

The Vale roof-repair source model has now transferred to five more owners:
the 5,000-byte scripted roof scene at `2ad0`, the 224-byte script beat at
`0db4`, the 336-byte scene-effect routine at `41ec`, the 214-byte named scene
exit at `0afc`, and the 132-byte named facing selector at `1334`. Together they
add 5,906 Exact-C bytes. The strict scripted-scene set is now 10 / 17 owners
exact, with seven scenes remaining. The broader 16-owner transfer queue, which
also includes scene drivers, effects, selectors, and helpers, is 5 / 16
complete this round, leaving 7,042 of its original 12,948 bytes unresolved.

The nearest remaining witnessed residuals are `3e58` (644 bytes, seven
differing halfwords after all 39 calls were resolved and its two compiler
routes were proved), `0518` (320 bytes, 93 differing
bytes), and `102c` (282-byte owner, 130 differing bytes with a boundary/source
shape mismatch). Larger scene drivers at `27ec`, `1160`, `1494`, and `1dbc`
have complete call maps but still need structural reconstruction; they are not
permutation-ready.

This round also corrected 36 raw `push {...,lr}` prologue halfwords that sat
immediately before already-audited Thumb bodies. That adds 72 executable bytes
to the denominator without claiming them as C and restores the true lower
bound of the `0cd4`, `0f80`, and `102c` owners. Scene source now uses
evidence-backed behavioral aliases in executable statements while retaining
the synthetic `Func_...` spellings only at the overlay ABI boundary.

The authoritative measurements are
[`metrics/gs1-en-executable.json`](metrics/gs1-en-executable.json),
[`semantic/regions.json`](semantic/regions.json), and the generated
`out/decomp/overlays.json`. Preserve clean-room rules from
[`PROVENANCE.md`](PROVENANCE.md), compiler evidence in [`LAWS.md`](LAWS.md),
and exhausted axes in [`SANCTUM.md`](SANCTUM.md).
