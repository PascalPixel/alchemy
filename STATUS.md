# Current status

Snapshot: 2026-08-14. This is the live status surface. Dated investigations and
their original measurements are archived under [`docs/history/`](docs/history/).

## Progress

- **Exact C:** 377,422 / 1,347,336 executable bytes (**28.01%**).
- **Public DONE:** **33%**, combining Exact C with audited permanent assembly.
- The generated coverage maps and dashboard are the live per-overlay source of
  truth; this page records the current contributor frontier rather than a
  second hand-maintained per-overlay ledger.
- Exact C is the contributor progress counter. DONE answers the public
  completion question by also including code deliberately retained as permanent
  assembly. Semantic C, function counts, and queue sizes are diagnostics.

## Working loop

The per-session loop is in [`AGENTS.md`](AGENTS.md) and the full working
method, including where each kind of search must stop, is in
[`CONTRIBUTING.md`](CONTRIBUTING.md). This page stays the measured frontier:
what is exact, what is parked, and what the next decisions are.

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

The compiler-routing debt is measured rather than anecdotal. Across 5,867 live
Exact-C and semantic-C sources, 698 currently differ from the baseline route;
those resolve to 173 distinct command lines, including 116 singleton cohorts,
from 162 evidence tables and 138 observed non-baseline flags. This is too broad
to treat as a reusable source-search matrix. It is not safe to delete entries
blindly: table order is byte-significant and several apparent duplicates are
mirrored historical evidence. Reduce this surface only when a source rewrite
or a single compiler-behavior consolidation proves the affected routed exact
corpus byte-identical, then remove or merge each obsolete route explicitly.

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

## Nearest overlay closure frontier

Overlay work is ordered by remaining executable owner bytes, not by overlay
number or by the size of one attractive function. Discovery proof remains
separate from adoption: the rows below do not enter Exact C until
`overlay_adopt` succeeds under an approved compiler bundle.

| Order | Overlay | Indexed unfinished bytes | Discovery-proved this round | Discovery remainder | Next fact |
|---:|---|---:|---:|---:|---|
| 1 | `resource_3c2` | 538 | 538 | 0 | Whole overlay awaits exact-path rehearsal. |
| 2 | `resource_3ce` | 700 | 700 | 0 | Whole overlay awaits exact-path rehearsal. |
| 3 | `resource_392` | 788 | 208 | 580 | The two remaining 296/284-byte owners are the shared `391`/`392`/`393` probe/redraw family. |
| 4 | `resource_393` | 760 | 180 | 580 | Same shared probe/redraw family; the 180-byte placement wrapper is exact. |
| 5 | `resource_36f` | 888 | 296 | 592 | Two exact small owners leave the 364-byte probe and 228-byte redraw. |
| 6 | `resource_391` | 972 | 212 | 760 | The 212-byte scene-call owner is exact; three owners remain. |
| 7 | `resource_38a` | 1,188 | 0 | 1,188 | Next untouched whole-overlay frontier. |
| 8 | `resource_3c3` | 1,374 | 0 | 1,374 | Its 142-byte scene bracket has correct size and calls but still needs evidenced instruction ordering. |

For the shared 284-byte redraw owner, target and candidate have the same size
and control flow. RTL identifies the dominant residual at global allocation:
the candidate assigns the stack work-record address and normalized width to
`r6`/`r7`, while the ROM assigns the same natural lifetimes to `r7`/`r6`.
Naming the loop sentinel as a separate scalar grows the function and is a
measured negative. Explicit x-before-z normalization and separate world-origin
scalars retain the ROM-witnessed source order; do not repeat the completed
24-order scheduler/declaration sweep without a new allocator or source-lifetime
fact.

For `resource_36f:0454`, the ROM and `resource_370:0054` targets prove the same
228-byte redraw body through the final workspace store; the latter's semantic C
is not an exact-source witness. The current candidate reaches the reference size
with the evidenced grouped-DMA lowering, but its tile loop still keeps one extra
callee-saved value. Natural `screen[row * 32 + column]` indexing is a measured
negative: it produces a 236-byte loop using `lr`, `ip`, and `r7`, rather than the
ROM's low-register walking recurrence. Do not repeat the saved register-order,
mode, DMA-form, rematerialization, or natural-index experiments without a new
RTL lifetime fact.

For `resource_36f:02e8`, the 360-byte candidate already has the ROM's eight
calls and five queue blocks; the apparent broad call residual is displacement
fallout from the ROM's first interior literal pool. The target keeps the IME
pointer, queue base, and fade counter in `r5`, `r7`, and `r6`, respectively.
Changing the saved IME scalar from `u32` to `u16` is a measured negative: gcc
adds sign-extension pairs to every queue block and grows the owner to 372
bytes. The remaining first-order issue is the two-pool layout, followed by
callee-register lifetime reuse; do not rewrite the proved queue CFG.

For `resource_391:0a68`, the ROM's `ldmia`/`stmia` copy into the two outgoing
stack arguments proves that the six-word placement result is passed to the
redraw helper as an aggregate by value, not as six independently spelled
scalars. That source correction plus a direct `Func_0808a080(10)[35] = 2`
store reduces the candidate to 178 bytes against the 180-byte owner and makes
the query, gate, scene calls, and actor update structurally agree. The remaining
floor is confined to the two six-argument call sheets: even the exact
`resource_391:0c68` source shape and its `stack-args-before-stores` route leave
the first pair's constant order reversed and allocate the second pair's zero
to `r2` instead of the ROM's reused `r5`. Narrowing the zero scalar's scope is
codegen-neutral. Return only with a new allocator-lifetime fact; do not resume
scalar-call or declaration-order search.

## Primary target index

[`TARGETS.md`](TARGETS.md) is the main contributor queue. It is generated from
the same exhaustive owner partition as the dashboard, sorted largest to
smallest, and contains every scope of at least 1,000 bytes. The complete
machine-readable index in
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json) retains
all 1,727 unfinished scopes down to the smallest owner.

The index accounts for all 1,347,336 executable bytes: 890,990 target bytes are
reviewed semantic C, and 452,818 bytes occur only in Exact C or audited
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

The exact 6,700-byte `resource_391:0d3c` scene driver is a useful semantic,
control-flow, and naming witness for these large sheets, but it is not a
compiler-route template: its current exact build uses nine narrow routed
behaviors. Transfer a proved source shape from it when the target agrees; do
not copy that route stack into another owner. Large diffuse residuals stay on
the source-reconstruction path, and any compiler experiment must hold the
source fixed and test one evidenced behavior at a time.

The 2026-08-14 large-owner checkpoint leaves the following measured source
baselines. Main-image diagnostics report differing bytes; overlay diagnostics
report differing halfwords, so the residual column states its unit explicitly.

| Owner | Candidate / target | Current residual | Retained source evidence |
|---|---:|---:|---|
| `080bbb0c` | 6,332 / 6,332 | 3,698 bytes | `battle_types.h`, `battle_event.h`, `battle_runtime.h`, and `battle_command.h` now separate proved layouts, the shared event queue, runtime helpers, and command records. `item.h` owns the one canonical 44-byte item definition. All register-derived locals in this resolver now have behavior-based or offset-neutral names, while its load-bearing unsigned byte views remain intact; the candidate and object digests did not move. In the second dispatch, cases `0x3d`, `0x01`, and all but one scratch-register choice in `0x3c` remain normalized-exact; continue the healing load order without perturbing them. |
| `resource_3bd:13f8` | 6,144 / 6,220 | 2,947 halfwords | `RunActorFormationScene` names the bounded choreography. Typed actor positions, sprite pointer, visibility flag, actor-8 subrecord lifetime, and the scoped formation-script pointer are ROM-witnessed; continue from those phase boundaries. |
| `resource_380:27f8` | 5,932 / 5,932 | 2,616 halfwords | `RunResource380GrandFinale` now reaches target size. Reference initialization order, separate pointer/scalar lifetimes, and scoped closing-text fields remove 183 differing halfwords while keeping both record lookups before dereference. Reconstruct actor 9's eight-byte workspace next. |
| `080ea0d8` | 5,732 / 5,756 | 5,353 bytes | `RunCinematicProjectionEffect` names the owner and its retained context. Five independent geometry scalars replace a misleading aggregate without changing the byte residual and remove 29 instruction mismatches; preserve full-width RNG values and explicit low-16-bit extraction. |
| `resource_3bf:3054` | 5,656 / 5,604 | 2,509 halfwords | Preserve the line-cursor aggregate. |
| `resource_373:15dc` | 5,216 / 5,240 | 2,227 halfwords | `RunValeRoofRepairScene` now records Isaac, Dora, Jenna, Garet, and the four witnessed dialogue beats. Exact-sibling declaration scopes regress here, so continue from this neutral vocabulary rather than copying its route shape. |
| `resource_381:1410` | 5,088 / 5,136 | 2,478 halfwords | `RunSolSanctumCollapse` now names Isaac, Garet, the Wise One, camera state, and Elemental Star fields from exact-sibling evidence without changing codegen; continue phase-by-phase from that vocabulary. |

This checkpoint also retained thirty-two smaller but structurally useful advances:

Candidate/target spans below include owner-owned literal or alignment tails;
the generated core-target index counts executable bytes only. Thus
`resource_39d:1af0` is a 4,844-byte compilation span but a 4,840-byte target.

| Owner | Candidate / target | Current residual | Retained source evidence |
|---|---:|---:|---|
| `080ab5e4` | 4,776 / 4,888 | 4,585 bytes | `RunTwoPaneSelectionMenu` now models the ROM's contiguous 16-byte availability array and eight slot flags at the witnessed `+0x54` and `+0x64` frame offsets. This valid 108-byte-frame baseline replaces an overlapping scalar/array draft despite its lower superficial score. |
| `resource_39d:1af0` | 4,852 / 4,844 | 2,377 halfwords | `RunScene59ValeSequence` names the actor/workspace roles. The scene-system pointer cell and its adjacent workspace cell are now modeled as one scoped IWRAM aggregate, keeping every workspace-counter path defined before use. |
| `08027114` | 4,216 / 4,224 | 3,798 bytes | `RunSequenceRecordEditor` names the output-record, identifier-list, and evolving-result boundary without changing code generation. The candidate's 104-byte frame remains one spill larger than the ROM's 100-byte frame. |
| `resource_3aa:0360` | 4,260 / 4,268 | 1,834 halfwords | Holding the IWRAM workspace pointer cell matches the ROM lifetime and removes 90 differing halfwords. |
| `resource_39d:0b24` | 4,016 / 4,044 | 1,954 halfwords | `RunScene58Sequence` names the owner. Both branches now load the scene-workspace pointer before updating its `+0x1d8` counter; this removes undefined C even though the current compiler score is worse than the invalid draft. |
| `resource_3a8:0590` | 4,108 / 4,092 | 1,847 halfwords | The 4,092-byte stored owner contributes 4,064 executable bytes; cautious actor and phase vocabulary is recorded without changing code generation. |
| `resource_378:088c` | 3,984 / 4,080 | 1,879 halfwords | `RunEnsembleCutscene` names the owner. ROM-witnessed per-site veneer symbols remove 128 differing halfwords while preserving all 489 imported-call sites. The `BL` at `0x02000912` enters this owner's shared exit tail, so it remains source-level control flow rather than a fabricated callee. |
| `080f6440` | 3,792 / 3,804 | 3,609 bytes | `UpdateMenuPresentationAndOam` now names the subsystem boundary. A typed five-entry menu-record aggregate recovers enabled flags, signed spawn delays, positions, and choice bytes, removing 59 differing bytes. One-shot DMA scopes are codegen-neutral and an OAM aggregate regresses; recover the ROM's 40-byte frame through renderer-state lifetimes instead. |
| `resource_383:36f8` | 3,768 / 3,796 | 1,572 halfwords | `RunDialoguePromptScene` names the owner. All 387 imported calls retain their ROM-witnessed veneer identities, removing 168 differing halfwords while preserving the complete call order. |
| `080dea70` | 3,724 / 3,656 | 3,449 bytes | `RunSelectorDrivenBattleEffect` names its effect context and eleven witnessed selector modes without changing code generation. Continue from the already-audited renderer-46 cache and renderer-47 republish boundaries. |
| `080e7404` | 3,416 / 3,640 | 3,463 bytes | `RunTwoStageProjectionVisualEffect` names the context/mode boundary. ROM offsets establish the frame's `+7` vector as world position and `+10` vector as screen position; phase-localizing all six aliases together regresses the byte score, so reconstruct their individual aggregate boundaries instead. |
| `resource_3c9:12c8` | 3,616 / 3,604 | 1,711 halfwords | `RunScriptedActorChoreography` names the owner. ROM-witnessed veneer identities remove 51 differing halfwords while all 363 calls remain valid. Phase-local flags are neutral; reconstruct callee prototypes and rematerialize constants locally so the candidate stops preserving `fp/r9/sl/r8` across calls. |
| `resource_378:187c` | 3,378 / 3,400 | 1,531 halfwords | `RunSceneEightEnsembleSequence` names the witnessed scene phase. Per-site veneer identities remove 105 differing halfwords while all 400 calls remain valid. Phase-localizing its fixed-point delta is neutral; reproduce the ROM's ascending `0..31` loop rather than the optimized countdown next. |
| `080d1714` | 3,404 / 3,384 | 3,092 bytes | Five real eight-actor arrays and two three-word projection vectors replace scalar placeholders and undefined pointer arithmetic. A monolithic ROM-ordered scratch aggregate ICEs gcc296 and its alias-safe form regresses; test reversed independent declaration order while preserving the separate arrays. |
| `08023178` | 3,424 / 3,320 | 3,310 bytes | `ShowPartyMemberDetails` now names the modal. Four selection bytes, an 11-entry sprite array, and separate row-9/row-10 special sprites recover the witnessed stack model. Moving the row tile after the eleven-entry array regresses; reconcile the ROM's 352-byte frame and `sp+96` navigation state with the candidate's 380-byte frame and `sp+124` state. |
| `080d6970` | 3,308 / 3,308 | 3,106 bytes | Direct calls through the two renderer slots remove a spurious wrapper and reach target size. Counted `!=` shuffle loops regress; recover the ROM's 100-byte frame by keeping runtime in `r9`, the scene argument live, and only render context at `sp+64`, rather than the candidate's 140-byte all-alias frame. |
| `080a2680` | 3,100 / 3,128 | 2,799 bytes | `RunCharacterItemTransferMenu`; the ROM's `0xa8` animation/abort sentinel and `amount > 1` quantity condition replace two incorrect guesses. Local reordering is neutral and signed `amount` regresses; recover the state aggregate and branch-local temporaries before more frame work. |
| `080ca60c` | 2,952 / 2,968 | 2,840 bytes | `RunCombatVisualEffect` names the context/variant boundary. A 156-byte coordinate scratch aggregate with 12-byte entries restores the witnessed frame shape. A typed runtime aggregate regresses; reproduce the ROM's shared base load and `ldmia` prologue with a local base-pointer pattern. |
| `08023e70` | 2,728 / 2,756 | 2,319 bytes | ROM-shaped character masks and four-byte inventory records remove 46 differing bytes. Callers prove the first argument is category and the second unused; recover the ROM's 224-byte frame, especially description at `sp+84` and cursor/OAM at `sp+212`, before retaining those names. |
| `resource_3c7:0508` | 2,544 / 2,532 | 1,148 halfwords | Both branch arms now update the workspace skip counter directly, removing a non-ROM helper and 12 differing halfwords. |
| `080cbc0c` | 2,462 / 2,508 | 2,436 bytes | `RunCircularRevealEffect` now contains the ROM's direct two-pixel-thick symmetric-circle rasterizer rather than hiding roughly 700 owner bytes behind fabricated local helpers. |
| `0808c4f8` | 2,378 / 2,428 | 2,297 bytes | `RunMapFieldEventLoop`; direct `Func_0808c4c0` calls replace a fabricated private wrapper, restoring the ROM's call shape and removing 59 differing bytes. |
| `080f7460` | 2,260 / 2,388 | 2,265 bytes | `RunStatusMenuPresentation` shares the five-record position/enabled/spawn-delay vocabulary with `080f6440`; removing persistent hardware/map pointers restores the ROM's shorter lifetimes and removes 33 differing bytes. |
| `080d2464` | 2,288 / 2,356 | 2,260 bytes | `RunDirectionalParticleSceneEffect` contains both ROM-inline particle-spawn loops, reuses the saved scene slot, and recomputes particle-bank pointers at their witnessed phases. A persistent runtime-header alias regresses; recover the ROM's 72-byte frame and setup declaration order instead. |
| `08021e6c` | 2,212 / 2,300 | 2,110 bytes | Setup now installs callback `0x08021e15`; the sibling `0x08021dfd` callback is reserved for cleanup, matching the ROM pools and call sites. |
| `080d91dc` | 2,088 / 2,268 | 2,200 bytes | Separate persistent camera, object, projection, and origin vectors plus a function-scope renderer table restore ROM-witnessed lifetimes, removing nine differing bytes and 28 instruction mismatches. |
| `08024934` | 2,156 / 2,124 | 1,970 bytes | The ROM's inline division and 36-entry stack buffers remove 86 differing bytes; the four active-row flags retain the target's redraw-before-draw lifetime. |
| `08077428` | 2,000 / 2,024 | 1,130 bytes | Caching each signed item modifier before updating scratch state restores the ROM's evaluation order in both adjustment loops, removing four differing bytes and 37 instruction mismatches. |
| `080e823c` | 1,904 / 1,968 | 1,867 bytes | ROM arithmetic establishes `0x4000` as the particles' late vertical acceleration. The correction is byte-neutral but removes a semantic guess before structural matching continues. |
| `080bd898` | 1,868 / 1,876 | 1,765 bytes | The dispatcher and exact queue producer now share `BattleEventQueue` and named event opcodes. Its 64 opcodes, 64 operands, target cursor, and count are guarded at their ROM offsets; dispatcher names and both candidate digests remain unchanged. |
| `08025200` | 1,816 / 1,836 | 1,521 bytes | The ROM's 12-byte sprite/OAM record removes 203 differing bytes and now names attributes 0–3 directly. |
| `080d89ac` | 1,708 / 2,024 | 1,910 bytes | Removing a spurious scene field restores the ROM-witnessed layout at `+0x1c` and object-ID array at `+0x24`, removing one differing byte and one instruction mismatch. |

Negative source-shape evidence from this round is also load-bearing.
`resource_3c9:2360` remains 4,612 / 4,708 with 2,229 differing halfwords:
delaying its vector alias and reordering three independent buffers do not
help; the next test is one 136-byte `OrbitScratch` aggregate ordered as the
40-byte second descriptor, 12-byte vector, and 40-byte first descriptor at
`sp+44`, `sp+84`, and `sp+96`. `resource_381:0054` remains 3,512 / 3,548 with
1,404 differing halfwords after direct `scriptPhase` field access regressed;
its next axis is rematerializing the scene pointer at phase boundaries.
`resource_383:36f8` remains 3,768 / 3,796 with 1,572 differing halfwords after
splitting direct and cached workspace access regressed; preserve the cached
workspace pointer through prompt save/restore instead.

`08026080` remains an explicit rewrite lead rather than a permutation target:
it is 3,568 / 2,138 bytes with a 400-byte candidate frame versus the ROM's
324-byte frame. Shrinking the standalone frame array alone regresses both size
and residual; rebuild all of its simultaneously live m2c temporaries as one
explicit phase-overlapped 324-byte aggregate before trying another leaf-level
spelling.

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
