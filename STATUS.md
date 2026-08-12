# Current status

Snapshot: 2026-08-12. This is the live status surface. Dated investigations and
their original measurements are archived under [`docs/history/`](docs/history/).

## Progress

- **Exact C:** 352,670 / 1,347,256 executable bytes (**26.18%**).
- **Public DONE:** **30%**, combining Exact C with audited permanent assembly.
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

## Core targets

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

| Rank | State | Core target | Measured scope | Current ownership | Cut and evidence |
|---:|---|---|---:|---:|---|
| 1 | selected | Configurable spawn/copy family | 5,192 executable bytes, 22 owners | at least 2,360 exact; 2,832 immediate semantic targets | Eleven overlays share a 472-byte spawner and 56-byte integrator. Five exact spawners witness six semantic copies in `resource_3c9`, `39c`, `39d`, `39e`, `380`, and `3a5`. Freeze per-overlay tables, callbacks, and veneer identities as shims. |
| 2 | selected | `resource_3ba` / `3bb` paired render tasks | 2,888 bytes, two mirrored 1,444-byte slices | 0 exact / 2,888 semantic | `3ba:31c0-3764` and `3bb:3458-39fc` cross-check one another and each has only eight distinct imported callees. Exact adjacent installers at `3ba:3764` and `3bb:39fc` are the cut seam. |
| 3 | selected | `resource_39f` slot procession | 2,896 bytes, 11 adjacent owners | 2,388 exact / 508 semantic | Nine exact owners constrain `39f:15d0` and `39f:1d04`. Keep slot records, party-heading reads, and the `Data_02000240 + 0x22b` completion byte at the perimeter. `1d04` is already localized to two swapped halfwords. |
| 4 | surveyed | Main menu presentation/OAM | `0x080f6000-0x080f86f8`, 9,976 bytes | 604 exact / 9,364 semantic / 8 retained | A repeated update/render pipeline around `080f6440`, `080f7460`, and `080f7f78`. DMA, renderer slots 46/47, and allocation are shims; per-frame state and OAM construction are the core. |
| 5 | surveyed | Main scene/effect C island | `0x080da2ac-0x080dd2ac`, 12,288 bytes | 226 exact / 12,062 semantic | A contiguous zero-assembly island with repeated renderer and scene plumbing. Cut at renderer-table dispatch, resource services, and IWRAM helpers. This is the cleanest single main-image subsystem boundary. |
| 6 | surveyed | Staged actor movement/redraw | 4,890 executable bytes, 10 owners in five overlays | mixed; `373:0608` is a 696-byte exact witness | `373`, `389`, `391`, `392`, and `393` share the movement/effect owner and kind-selected redraw. Displacement tables, orientation tables, and callbacks are shims. |
| 7 | surveyed | Command/interpolation renderer | 3,792 bytes, three owners | 0 exact / 3,792 semantic | `3ba:21b8`, `3bb:2450`, and `3bc:2ee8` share the queue decoder, three interpolation passes, mode tail, and ten-call sequence. Private state cells, callback, palette, and cache addresses form the perimeter. |
| 8 | surveyed | Main interactive comparison UI | `0x080ab314-0x080ae714`, 13,312 bytes | 1,048 exact / 12,264 semantic | `080ab5e4` and `080ad6d4` form a screen-local state-machine family. Generic `080150xx` window/OAM and `080770xx` runtime services stay outside the cut. |
| 9 | surveyed | Main battle action/script core | `0x080ba27c-0x080bdfec`, 15,728 bytes | 1,338 exact / 14,378 semantic / 12 retained | `080bd898` drives the script state and invokes resolver `080bbb0c`. The centre is action decode, target resolution, message, and cleanup; renderer, audio, and resource services are shims. High call density makes this valuable but risky. |
| 10 | surveyed | `resource_3c5` paired cutscene drivers | 4,660 bytes | 0 exact / 4,660 semantic | Two bounded scripts share a source model and one local-helper seam, but expose 29-30 imports and more than 500 call sites. Recover one type/vocabulary contract without introducing a shared C helper. |

Reserve targets, in order, are the 3,132-byte entity/handle family, the
568-byte paired attachment constructor, the 6,220-byte `resource_3bd` scene
driver, the 8,956-byte `resource_3c9` choreography block, and the 18,600-byte
`resource_373` cinematic core. The last three are semantic-architecture
campaigns until their call perimeters are narrower; shared calls alone do not
prove a reusable source skeleton.

### Five-kilobyte assault map

The 2026-08-12 large-core survey replaced broad subsystem labels with audited
cuts. These are real contiguous logic cores or repeated families, not totals
manufactured from unrelated neighbours.

| Priority | Core | Audited scope | Fresh semantic opportunity | Exact leverage |
|---:|---|---:|---:|---|
| 1 | Staged actor probe/move/redraw family (`373`, `389`, `391`, `392`, `393`) | 6,380 bytes | 5,684 bytes | Exact `373:0608` witnesses the 696-byte movement shape; all five triplets share clean `034c`, `0608`, `08c0`, and `09dc` seams. |
| 2 | Battle action preparation / target selection | `080ba27c-080bbb0c`, 6,288 bytes | 5,146 bytes | 1,134 exact bytes constrain three action builders and the 1,864-byte target selector. |
| 3 | Comparison-render subsystem | `080ac8fc-080ae714`, 7,704 bytes | 6,656 bytes | 1,048 exact bytes fix the renderer ABI, portrait/motion infrastructure, and both outer seams. |
| 4 | Main scene launch suite | `080da2ac-080db6e0`, 5,172 bytes | 5,148 bytes | Two exact tail wrappers witness the following renderer modes; four scene owners share one runtime/render vocabulary. |
| 5 | Main effect/render suite | `080db6e0-080dd2ac`, 7,116 bytes | 6,914 bytes | Seven exact mode wrappers constrain `080dbc30`; exact allocator and dispatch owners bound the suite. |
| 6 | Menu/OAM controller | `080f60a0-080f7db4`, 7,444 bytes | 7,444 bytes | Exact entry/palette helpers bound the front; a 452-byte exact barrier separates the independent compressor. |
| 7 | `resource_3c9` choreography | `12c8-35c4`, 8,956 bytes | 8,620 bytes | Three semantic scripts are interleaved with 336 exact helper/callback bytes and exact outer seams. |
| 8 | `resource_373` cinematic script island | `15dc-5594`, 16,312 bytes | 13,636 bytes | Five exact setup/dispatcher owners divide three large scripts at proved boundaries. |
| 9 | Queued battle-action resolver | `080bbb0c-080bd3c8`, 6,332 bytes | 6,332 bytes | One coherent owner; exact queue emitter is called 154 times, but no duplicate source witness exists. |
| 10 | `resource_391` cinematic scene driver | `0d3c-2768`, 6,700 bytes | 6,700 bytes | Exact local helpers and installed callbacks close its perimeter; source residual remains diffuse. |

The first staged-actor round corrected the raw `0xb084` prologue
classification in four overlays, transferred the exact by-value movement ABI
to `389`, `391`, and `392`, and recovered the shared redraw record/call model.
Routed movement residuals are currently 115, 116, and 90 bytes respectively;
redraw residuals reached 74 bytes in `373` and 72 bytes in `392`. These are
reconstruction baselines, not Exact C, and must not be permuted while diffuse.

For each selected core, create one coordinator brief that freezes its owner
set, byte accounting, exact witnesses, import/export seam, compiler routes, and
adoption order. Do not expand the core mid-round. A core is complete only when
every centre owner is exact or explicitly moved outside the centre as a
reviewed semantic edge shim, and the resulting build passes `make verify`.

The authoritative measurements are
[`metrics/gs1-en-executable.json`](metrics/gs1-en-executable.json),
[`semantic/regions.json`](semantic/regions.json), and the generated
`out/decomp/overlays.json`. Preserve clean-room rules from
[`PROVENANCE.md`](PROVENANCE.md), compiler evidence in [`LAWS.md`](LAWS.md),
and exhausted axes in [`SANCTUM.md`](SANCTUM.md).
