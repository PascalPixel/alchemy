# To do

Open work, most important first. [AGENTS.md](AGENTS.md) and its topic files
own how work is done; this file owns what is left. Remove an item in the commit
that finishes it.

## Now

### ☀️ 60%

Reach 60% of the fixed executable inventory. Read the current verified count
with `make progress`; do not maintain a second score in this task list.

- Recheck and adopt what the stopped recovery run found. Exact:
  `main:080fa514` (72 bytes) and `main:08092878` (172 bytes). Exact at 54 bytes
  each: `resource_39e:02000104` and `resource_3c9:02000104`, the `Effect_Move`
  family; eleven more copies differ by one halfword, so the family wants one
  shared source with instances.
- Rebuild coverage with `make coverage`, then rank unresolved owners from it,
  twins of exact owners first.
- Recount the owners still parked because their only match needed a
  scheduling trick. The five overlay copies of `0809a65c` still hold a
  `do { } while (0)` barrier. `SpawnConfiguredEffect` (23 copies) stays a
  recorded compiler gap unless new evidence reopens it.

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
