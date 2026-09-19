# To do

Open work, most important first. [AGENTS.md](AGENTS.md) and its topic files
own how work is done; this file owns what is left. Remove an item in the commit
that finishes it.

## Now

### Twelve-edition build readiness

- Repair TLA compile-only module declarations: `tla-script-operands` lists 38
  exact owners while `OPERANDS.C` also defines three unresolved named helpers.
  The claimed builder now reads the selected game manifest; `make targets`
  passes all six TBS editions and stops on this incomplete TLA module contract.
  Preserve existing exact-owner verification and declare complete module
  membership without crediting the unresolved helpers.

- ROM identification, executable DONE and whole-ROM reconstruction are separate
  measurements. All twelve have physical indexes; only TBS EN currently has a
  supported full-ROM build. The other eleven remain compile-only, explicitly
  rejected by `alchemy build full`. Do not remove that guard before exact builds.
- Text source is `TEXT/{JA,EN,DE,ES,FR,IT}.PO` for each game. Rebuild all twelve
  message archives with `alchemy build assets --verify-text`; rebuild the indexes
  with `alchemy coverage audit --all --data`. Their layout/checksum authority is
  `recon/text.json`, not the coverage report. No copied compressed bytes are used.
- Finish edition-local identification of the remaining regional data. Exact
  byte correspondence and decoding are evidence; generic compression recognition
  is not a payload type. Do not rerun a broad AI scan as a substitute for readers.
- Complete the eleven edition link layouts, code/assembly bindings and regional
  asset manifests. Preserve shared source; isolate actual measured differences.
  A complete ROM may legitimately still contain reconstructed assembly.
- Add each full build to the existing target gate only after comparison of the
  complete image succeeds. The final milestone is twelve independently exact
  ROMs, not twelve successful compilation commands or 100% identified bytes.

### ☀️ 75%

- Maximize newly verified DONE bytes per hour across the whole TBS executable,
  including integration and verification time. Rank coherent main and overlay
  families together; prefer fixes that can propagate to unresolved siblings.
  Better draft similarity alone contributes zero. Close exhausted local axes
  and select the next family from current evidence.
- Recover the C bodies around the reviewed DMA primitive in `INCLUDE/DMA.H`.
  Verified callers now include graphics copies, Kolima palette transfers and
  stack-resident ARM-routine wrappers. The remaining multi-transfer family
  must match owner by owner. Compare coherent
  DMA-copy, stack-zero initialization and checksum work against other families. Keep
  the single shared primitive unchanged; fill macros remain unadmitted.
  The six-owner save unit now uses that primitive and ordinary completion
  polling; all remain drafts. The checksum reader has one differing halfword
  (address copy versus reload); park that local mismatch. Reconstruct fill
  buffer initialization and the summary loop before more spelling searches.
  The VRAM cache loader now has matching control flow and four entry-setup
  differences; park that local allocation search. The rendering generator
  uses the shared primitive but still needs its common size-budget/emission
  structure recovered; DMA alone does not explain that owner.
  The heap-resident decompressor caller now has a complete DMA-based draft,
  with six scheduling differences; park that residual. Stack-based wrappers
  match through ordinary variable-length arrays and typed indirect calls.
- Do not repeat exact-twin sweeps without changed inputs: the September 19
  census found no uncredited exact twins among 4,235 scanned owners. Main
  program recovery is necessary; completing the remaining overlays alone
  cannot reach 75%.

### Draft TBS broadly, then tighten

- Recover complete, readable TBS C across coherent modules before pursuing local
  register or scheduling residuals. Repair missing behavior, calls, types and
  owner boundaries first; keep scored drafts and unresolved questions in owner
  dossiers. Revisit related drafts together as shared interfaces improve.
- Track draft coverage and structural/byte agreement separately from verified
  DONE. An approximate 80% matching target is a diagnostic milestone, never
  permission to omit behavior or claim partial owners as complete.
- Aggregate fresh owner binary similarities over the potential C inventory,
  weighted by audited reference bytes. Exact C contributes one, missing drafts
  zero; distinguish unscored or stale drafts from absent drafts. Keep this
  separate from DONE. The owner scorer now supplies halfword edit distance and
  its denominator; no corpus-wide partial percentage is measured yet.
- Audit the no-registered-draft inventory against surviving and retired source
  before treating it as never drafted. Salvage only complete, behaviorally
  credible C; regenerate misleading drafts from the reference. Existing draft
  coverage is a source census, not a semantic quality certificate.
- World-map display sequence `resource_371:020039fc` now has a complete
  1,300-byte owner and readable scored C. Recover the common transfer-queue
  interface before tightening its allocation and pool differences.

- World-map trigger group `resource_371:020024a8`, `02002768`, `020027dc`
  has 884 bytes of complete scored drafts in `world_map_actor_events.c`.
  Preserve the corrected 64-call sequence; tighten table-loop representation
  and call signatures with related modules, without per-owner last-mile sweeps.

- World-map actor effects `resource_371:0200384c` and `02004058` have
  complete typed drafts with matching topology and call order. Leave their
  constant/allocation residuals for a family pass. The orbit callback
  `resource_371:020001c4` (140 bytes) now has an arithmetic C draft: the
  existing IWRAM source proves its Q16 multiply, but the ip-return call
  remains nonexact and uncovered by the topology analyzer.

- Korima Bridge `resource_391:02002974` now has a complete 356-byte typed
  falling-effect draft with equal topology and 93.82% binary similarity. Its
  early zero lifetime remains nonexact; the bounded control-flow tests are in
  the dossier. Related `resource_38f:020027ac` has distinct bindings.

### ☀️ 60%

- Prefer never-drafted unknown; when that pool is too small for 60%, first-try
  non-Stubborn draft_c / overlay virgin. Skip Far veneers and asm-manifest
  owners (C there does not raise DONE). Bounce at 10 minutes.

Reach 60% of the fixed executable inventory. Read the current verified count
with `make progress`; do not maintain a second score in this task list.

- On `cursor/tbs-to-60`, `Effect_Move` is adopted. Agents: if an owner is not
  exact inside a 10-minute window, park it under Stubborn and switch.
### Stubborn

- `main:080f9f6c` (512 B) — surveyed-unmeasured never-drafted `compiler_output`; first-try psynergy draft fails to compile (`while (? > ?)` / shadowing); bounce.
- `main:0801c9c8` (78 B) — surveyed-unmeasured gap-continuation; first-try draft compiles but 40 differing halfwords (`copy_versus_rematerialise` / wrong `+10` field offsets); bounce.
- `main:080e53f4` (136 B) — surveyed-unmeasured gap-continuation; reference disassembles as data (`lsrs #32` / `strb` island), first-try structural-topology; bounce.
- `main:0808d9a4` (1044 B) — surveyed-unmeasured never-drafted `compiler_output`; first-try 436 differing halfwords / structural-topology; bounce.
- `main:0800fd5c` (320 B) — surveyed-unmeasured gap-continuation (real Thumb); first-try 156 differing halfwords (`copy_versus_rematerialise` wall); bounce.
- `resource_39b:02002030` FieldScene_RunScene39bSequenceA (832 B) — draft-boundary-review / never-had-C; first-try psynergy draft fails to compile (subscripted non-pointer); bounce.
- `main:08016f38` FlashTimerIntr (36 B) / `main:08016f5c` SetFlashTimerIntr (56 B) — TLA game gcc 2.96 emits push{lr}/pop{pc} for branched volatile body; ROM is leaf bx lr. TBS twin is on AGBCC_SOURCES (main:080069a4); TLA addresses not yet in TLA_AGBCC_SOURCES. Needs user-authorized agbcc family membership; bounce.
 (hard-pass later)
- `main:08017500` RunFlashEraseVerifier (24 B) — TLA route emits inline `mov lr,r1`+0xF800 / `pop {pc}` vs TBS-inherited `bl _call_via_r1` / `pop {r1}; bx r1` (instruction-selection / interwork route wall); bounce.
- `main:081c0f70` Audio_SetWorkPairB (20 B) — allocation-uncovered (ldr/str via r2/r0 vs r3); bounce. Twin `main:081c0fac` same class.
- `main:081c332c` Runtime_CallWorkEntryWithArgs (20 B) — TLA emits pop {pc} vs ref pop {r0}; bx r0; bounce.
- `main:081c0fd0` AudioCommand_GetStateByte (12 B) — allocation-uncovered (ldrb via r0 vs r3); bounce.
- `main:0802d400` Curve_LookupSampleByTableReversed (20 B) — scheduling-floor (ldr/lsls order); bounce.
- `main:080ebe94` EffectSlot_SetPosition (18 B) — 1 halfword off; bounce.
- `main:08013300` Resource_GetTableEntry (12 B) — instruction-selection wall; bounce.
- Prefer never-drafted TLA regions over near-miss halfword floors.
Parked for a stronger model. Agents must not burn a 10-minute window here.
- `main:081c342c` Audio_EmptyCallback (2 B) — scores exact with `--size 2`, but lies outside audited main executable ranges in metrics/executable.json; bounce until ranges include it.
- `main:0802d2b0` Curve_StepAtDifferencePosition (48 B) — scheduling-floor on TLA; bounce. (LerpTwoSamples / LerpTwoSamplesB / StepAtSummedPosition adopted via TLA `pop {pc}` spelling.)
- `main:080fa514` (72 B), `main:08092878` (172 B) — still non-exact.
- `resource_3bd:020013f8` (6220 B) — scheduling-floor, 2 halfwords.
- Overlay `0809a65c` `do{}while(0)` copies; `SpawnConfiguredEffect` compiler gap.
- `main:080f9f6c` (512 B, TBS) — surveyed-unmeasured never-drafted; first-try draft does not compile; bounce.
- `main:0808f498` (148 B) — surveyed-unmeasured gap-continuation; first-try draft compiles but many differing halfwords (not exact); bounce.
- `main:08097c3c` (836 B) — surveyed-unmeasured never-drafted `compiler_output`; first-try psynergy draft fails to compile (subscripted non-pointer); bounce.
- Recorded `copy_versus_rematerialise` / `instruction-selection` walls — stop.
- TLA Venus `resource_64d:02000510` — lifter repair required (see below).
- TLA field-operand floors `08025bb4` / `08025c5c` / `08025f9c` — already parked.
- TLA `script-interpreter-control` beyond `main:08024c50` — layout/bindings
  differ from TBS; do not thrash WaitForEvent at wrong addresses.
- `Effect_Move` shared unit is adopted on `cursor/tbs-to-60` (24 overlay
  instances). Keep taking fresh twins / named-no-source owners that score
  exact quickly; if not exact inside a 10-minute window, park under Stubborn
  and switch owners.
- Rebuild coverage with `make coverage`, then rank unresolved owners from it,
  twins of exact owners first.
- `main:080f9a30` SoundNote_Detach (30 B) — retired-from-C allocation wall (note stays in r0; next reg reused for zero); raw already documents no ordinary C spelling.
- `main:08021e28` (32 B) — scheduling-floor, 2 halfwords; bounce.
- `main:08029274` (80 B) — scheduling-floor, 2 halfwords; bounce.
- `main:080a9d3c` (72 B) — instruction-selection wall; bounce.
- `main:080fa514` (72 B) — still many differing halfwords (not exact).
- `main:08092878` (172 B) — still many differing halfwords (not exact).
- `resource_3bd:020013f8` FieldScene_RunExtendedActorPresentation (6220 B) —
  scheduling-floor, 2 halfwords (arg load order for `Func_02003d20(0,1)`);
  no established repair.
- Overlay copies of `0809a65c` with `do { } while (0)` barriers; leave parked.
- `SpawnConfiguredEffect` (23 copies) — recorded compiler gap.
- `main:0800383c`–`main:08003a3c` family (~129 B each) — scheduling-floor, 2 halfwords each; bounce.
- Allocation / instruction-selection walls already labeled
  `copy_versus_rematerialise` or `instruction-selection` (e.g.
  `main:080b6d30`, `main:080b6e7c`, `resource_380:02004260`,
  `resource_3af:02001db0`) — do not respell.

### Reconstruct The Lost Age

- Twenty-eight further overlay exports reproduce their complete images and compressed streams, but add 27,578 raw integer values. Keep them private until their data and C are reconstructed; the existing raw-data ceiling must not grow. Results and owners are in TLA `recon/en/dossiers.json`.
- Correct the existing `DAIRA` source prefix to `DERI` in one verified path migration: Japanese message `0xe62` says デリィ. Japanese location rules use message base `0xe5a`, two higher than English `0xe58`; do not reuse English message IDs for names.

The executable inventory covers 2,578,466 bytes: 1,535,116 in the main image
and 1,043,350 across all 114 overlays. Grow verified source coverage against
that fixed inventory; its denominator is not a claim of a complete TLA build.
Credit only ranges meeting [COMPLETION](.agents/COMPLETION.md).

- Continue the TBS transfer toward 25% with complete modules, ranked by new
  verified bytes per hour. Object dispatch, by-ID motion, child state and
  motion events now contribute 27 additional exact owners (1,068 bytes).
  Their complete extents and parked neighbors are in TLA's EN dossiers.
- Inventory and Djinn management add another 13 exact owners (886 bytes).
  Six Djinn neighbors remain uncredited because of scheduling differences;
  four inventory neighbors materialize `0x1ff` or `0x7ff` from literal pools
  where the reference synthesizes the constants. Existing `-mgs2` handles
  synthesis only during reload, as one indivisible output instruction, while
  `080b0ab8` interleaves another instruction inside its three-instruction
  synthesis. Pascal approved a bounded shared-lowering experiment on September
  19. Post-reload splitting matches that complete 192-byte owner and preserves
  all 140 exact TLA game owners; broader early synthesis matches Inventory_Add
  but regresses two exact message functions. Neither experimental compiler is
  installed. Results and the isolated licensed checkout are recorded in the
  `main:080b0ab8` dossier. Keep one game-wide option set.
- Make TLA `08120454` (8,300 bytes) byte-exact using TBS `080bbb0c`
  (6,332 exact bytes) as its control. Direct command access, shared raw action
  power, separate loop-counter lifetimes, saved-state field access, guard
  order and healing reads now explain more of the reference. The approved
  compiler emits 8,300 bytes / 3,618 differing halfwords; the isolated general
  constant experiment emits 8,288 / 3,369. Neither is exact. Continue from its
  EN dossier; compiler alternatives use one game-wide configuration, and
  existing small matches are evidence rather than a veto. No new credit.
- TLA staged actors at `resource_64e:02000314–0200056c`: the four-owner
  candidate reuses TBS's staged-actor module with reviewed TLA layout and
  binding changes. None is exact: shift scheduling, loop-invariant hoisting,
  constant sharing and map-pointer allocation remain. Two hypotheses yielded
  no adoption; do not repeat them. The 18-overlay anchor survey supplies
  leads only, not proven siblings. See `tla-staged-actor` and its EN dossiers.

- Field-script operands at `08025b58–08026320`: 38 exact owners (1,820 bytes)
  adopted as `tla-script-operands` from `FIELD/COMMON/SCRIPT/OPERANDS.C`.
  Three 2-halfword scheduling floors remain uncredited at `08025bb4` (46),
  `08025c5c` (46), and `08025f9c` (52); do not repeat the five parked source
  hypotheses or change compiler flags. They stay in the C for layout only.

- Venus approach event `resource_64d:02000510` has a reviewed 5,404-byte extent.
  Its loaded reference ends at `02009a2c`, including the last literal. The
  private candidate at `out/tla-en/overlay-64d-scene/SCENE.C` scores 5,360
  emitted bytes, 2,293 differing halfwords and 285 aligned wrong instructions;
  no bytes adopted. Calls were bound from reference sites, not candidate
  offsets. Repair the lifter before repeating this scene family: a fresh
  `psynergy decompile` duplicates actor lookups, appends two spurious calls
  after the event-end call, and introduces `volatile` on ordinary actor RAM
  (`tools/psynergy/src/lift.rs` byte-mask emission). The private candidate
  removes those errors. Its first remaining mismatch is constant 257 loaded
  from a pool versus `mov 2; add 255`; the existing compiler synthesizes such
  constants during reload, so equivalent constant spellings are not a new
  search axis. Compiler, flags and credit remain unchanged.

### Alchemy builds, Psynergy reads

The law in [TOOLING](.agents/TOOLING.md). Psynergy still holds operations that
produce game formats or source, such as the encoders behind `psynergy convert`
and `psynergy repair`: decide for each whether it builds, move what builds into
Alchemy, and update the tooling index in the same change.

### The first demo

A scene player that plays the end of _The Broken Seal_ cut back and forth with
the start of _The Lost Age_, built as a ROM from decompiled scenes of both
games. Every scene it needs must be byte-exact in both ROMs first. The staff
roll and title screens are out of scope.

### Lunpa as the finished standard

Grow the Lunpa showcase until it shows what the whole repository will look
like when it is done, then carry that standard to every module.

## Maps and the 3D world

- Raise rooms into 3D from their walking maps or boundaries. _The Lost Age_
  has a collision layer; find what plays that part in _The Broken Seal_.
- Give the viewer controls for draw order within a floor and for story states
  of one place.
- Twelve Anemos and Atteka inlet links still miss by 16 to 80 pixels: a door
  and its way back land a step or two apart. Decide whether arrivals should
  count one step inside.
- Scene 239's exits into Anemos are placed like doors. Decide how warps join.
- Assemble every world map family, not only Anemos and Contigo.

## Before 100%

- Rebuild all twelve target ROMs byte-identically from a clean checkout. Only
  the TBS English production gate exists today.
- Repair the existing TLA compile-target binding failure: `make targets` stops
  at TLA JA `GET_BYTE.C` with unsupported external `GameFlagBytes`. The six
  TBS compile targets and TLA English owner verification pass.
- Prove Japanese correspondence before calling English source shared, and
  measure _The Lost Age_'s own behavior rather than counting one recovery
  twelve times.
- Map _The Lost Age_'s source layout; it keeps its current layout until then.
- Remove the legacy `Func_`, `Data_` and `Value_` names from every `COMMON` and
  `LIB` source.
- Lower `OVERLAY_DATA_DIRECTIVES_MAX` as raw overlay data becomes typed tables
  or private inputs.
- Let the encoder derive the trailing `lookahead` bytes compression plans still
  record, and drop recipes the encoder reproduces.
- Identify music titles; never number them by guess.
- Move area-exclusive maps and graphics beside their code once their consumers
  establish ownership.
- Debug item menu `resource_3ce:02000cf4` now has a complete 532-byte typed draft in `recon/en/overlays/debug_item_menu.c`. Resolve the DMA register-transfer source form and loop block order before further matching; ordinary aggregate and direct-field models were measured, neither exact. The separate two-byte alignment at `02001186` remains unresolved.
- Raribero facing dialogue `resource_3c7:020000c8` is one 148-byte function, not two code islands. Its flag literal at `02000150..02000154` is missing from the executable inventory; reconcile that audit gap before claiming full-overlay completion. The adjacent 236-byte pose sequence is now exact.
- Close the unknown executable gaps that block whole-overlay completion.
- Admit compiler bundle builds on hosts other than Apple Silicon macOS.

## At 100%

Open Alchemy to outside contributions.

- World-map entry `resource_371:020006ec` (928 B): complete switch draft in `recon/en/overlays/world_map_entry.c` scores 920 B / 284 differing halfwords. Verify callee prototypes, state-field addressing, and the two small pool constants before continuing; no credit. The owner dossier records the complete residual.
