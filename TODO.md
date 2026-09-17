# To do

Open work, most important first. [AGENTS.md](AGENTS.md) and its topic files
own how work is done; this file owns what is left. Remove an item in the commit
that finishes it.

## Now

### ☀️ 60%

☀️ is 56.19%: 761,772 of 1,355,730 executable bytes. 60% is 813,438 bytes,
51,666 more.

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

### Classify The Lost Age

Most of _The Lost Age_'s ROM is still unclassified: it has 824 bytes of matched
main-image C, eight overlay listings for its opening scenes and its extracted
assets, but no main-image disassembly and no executable audit. Audit its
executable map, produce assembly listings where the build needs them, and
credit only ranges that meet the retained-assembly evidence standard in
[COMPLETION](.agents/COMPLETION.md); everything else stays unresolved. The
audit gives ⚓️ its denominator, so it stops being pending.

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
