# To do

Open work, most important first. [AGENTS.md](AGENTS.md) and its topic files
own how work is done; this file owns what is left. Remove an item in the commit
that finishes it.

## Now

### Twelve-edition build readiness

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

### ☀️ 60%

- Prefer never-drafted unknown; when that pool is too small for 60%, first-try
  non-Stubborn draft_c / overlay virgin. Skip Far veneers and asm-manifest
  owners (C there does not raise DONE). Bounce at 10 minutes.

Reach 60% of the fixed executable inventory. Read the current verified count
with `make progress`; do not maintain a second score in this task list.

- On `cursor/tbs-to-60`, `Effect_Move` is adopted. Agents: if an owner is not
  exact inside a 10-minute window, park it under Stubborn and switch.
### Stubborn
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
- `resource_382:02001090` ActorDraw_SetupActorSceneCells twin (172 B) — equivalent to `resource_385:02000c1c`, but instance must place every `actor-motion-event-scene` member; bounce until full resource_382 map exists.
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

The executable inventory covers 2,578,466 bytes: 1,535,116 in the main image
and 1,043,350 across all 114 overlays. Grow verified source coverage against
that fixed inventory; its denominator is not a claim of a complete TLA build.
Credit only ranges meeting [COMPLETION](.agents/COMPLETION.md).

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

- Close the village's recorded 472-byte `Effect_Spawn` compiler gap using
  ordinary C, without volatile fields or scheduler helpers. The complete
  four-halfword draft is kept in
  `games/THE BROKEN SEAL/recon/en/overlays/resource_3ab_c_0200013c.c`;
  removing the accumulator local, splitting both loads into temporaries and
  viewing both records through fixed-point unions, using C89 `register`
  declarations and fully prototyping the rate helpers leave the same two
  `sched2` load-pair reversals. Hoisting target X, inline delta helpers and
  algebraic subtraction forms regress. The three rate calls are distinct
  overlay imports, so replacing them with `/` is not the original source. A
  minimal scale branch emits the reference target-before-current order; adding
  only the later script block reverses it, locating the break at the options
  pointer's cross-call lifetime rather than the subtraction. Block-scoped
  locals and a whole-scale inline helper keep the four-halfword floor. A local
  target/current aggregate preserves the required order but changes three
  register instructions. Jev 1.13 abstained from choosing a source model and
  ranked scoped lifetimes as the least-uncertain bounded probe; that probe was
  neutral.

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
- Close the unknown executable gaps that block whole-overlay completion.
- Admit compiler bundle builds on hosts other than Apple Silicon macOS.

## At 100%

Open Alchemy to outside contributions.
