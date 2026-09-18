# To do

Open work, most important first. [AGENTS.md](AGENTS.md) and its topic files
own how work is done; this file owns what is left. Remove an item in the commit
that finishes it.

## Now

### ☀️ 60%

- Prefer never-drafted unknown; when that pool is too small for 60%, first-try
  non-Stubborn draft_c / overlay virgin. Skip Far veneers and asm-manifest
  owners (C there does not raise DONE). Bounce at 10 minutes.

Reach 60% of the fixed executable inventory. Read the current verified count
with `make progress`; do not maintain a second score in this task list.

- `Effect_Move` shared unit is adopted on `cursor/tbs-to-60` (24 overlay
  instances). Keep taking fresh twins / named-no-source owners that score
  exact quickly; if not exact inside a 10-minute window, park under Stubborn
  and switch owners.
- Rebuild coverage with `make coverage`, then rank unresolved owners from it,
  twins of exact owners first.

### Stubborn (hard-pass later)

Parked for a stronger model. Agents must not burn a 10-minute window here.

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
