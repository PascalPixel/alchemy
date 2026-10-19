# Matching

This page owns what the folder guides share about matching: how the build is
judged, what the markers in the source mean, and the plain-C spellings that
some sites keep because they are what compiles to the original bytes. Each
guide points here instead of repeating it, and keeps its own pages for how
its code works.

## How a build is judged

`make compare-editions` builds all twelve editions and checks each ROM's
SHA-1 against its line of `rom.sha1` (the `compare-%` rule of the
`Makefile`); each edition that matches prints a line ending `: OK`. Its
output is long, so send it to a log and count those lines: twelve is a pass.
`make compare-tbs` checks the six ☀️ editions, `make compare-tbs-en` one.

The judgement is a hash of the whole image, because the hash is the only
thing from the original cartridge the build is allowed to see: the reference
ROM judges the build and never feeds it. That makes the check all or
nothing. One wrong byte anywhere fails its edition, and the hash does not say
where. To find the byte, compile the one object you changed to assembly with
the `Makefile`'s command and diff it against a baseline with `.L` label
numbers ignored, or use the optional `psynergy diff` on a function.

The ☀️ editions need no ROM. The ⚓️ editions need your own ROMs under
`roms/`, because what is not yet C comes from `.incbin "baserom.gba"`
scaffolds under `recon/tla`, which never count.

A change made on purpose, such as a mod or a fixed bug, changes the bytes and
so fails the comparison. That is expected: keep such work apart from the
matching tree, or behind `BUGFIX`.

## Markers in the source

- `/* FAKEMATCH(kind): the form kept; as <plain form>, N of M instructions
  differ in tbs-en. */` marks a device that steers the compiler towards the
  original instructions, inside the function it steers. N is measured over
  the whole function against the edition named. The kinds:
  - `wrapper`: a call made through a helper, a cast or a library routine in
    place of the plain call or operator;
  - `volatile`: a qualifier that forces a load or store to stay where it is
    written;
  - `temporary`: a variable that holds a value the plain form would not name;
  - `dead`: code that emits nothing and only changes allocation or order;
  - `one-pass`: a block or loop that runs once (`do { } while (0)`), whose
    loop notes end a CSE block and act as a scheduling barrier;
  - `barrier`: an empty `asm`, which emits nothing but counts four bytes when
    literal pools are placed;
  - `pin`: a variable fixed to a register.

  A device comes only after reshaping the code has failed, and removing one
  is humanizing. Some older tags have no kind yet (`/* FAKEMATCH: ... */`);
  they say the same thing.
- `// DEAD: why` marks code the ROM keeps that cannot change the result, such
  as a test that is always true, where the construct's own compare, branch or
  loop is in the ROM. It is then the source's form, not a device.
- `// BUG: what goes wrong` marks a bug of the original game, kept as it is;
  the fix, where there is one, is behind `BUGFIX` (`include/config.h`), which
  is off.
- `// UB:` marks undefined behaviour kept in the form that matches; the
  defined form is behind `UBFIX`, which `make modern` and `BUGFIX` turn on.
- `CAMELOT_ASM("proof")` marks the few proven handwritten helpers inside C
  (`Dma_Set`, `Transform_SetIdentity`, `Iwram_Call2`, `WaitFrames`' stack
  switch, `MulQ16`, and the `swi 2` and `swi 3` BIOS calls). It expands to
  nothing; it is there to be read.
- `UNUSED` marks a function nothing calls, kept because its bytes are in the
  ROM.
- Assembly files open with `@ C2:` lines that prove why no C compiler of the
  set would produce them (an instruction, frame or pattern agscc cannot
  emit).

## Spellings that match at different sites

Each idea has one preferred spelling. Where a site matches only with a
plain-C equivalent (an index or pointer form, a macro or its expansion, a
cast), that site keeps the equivalent, untagged, and it is listed here. These
are one idea each, and can compile differently.

### Everywhere

- `p[n]` and `*(p + n)`, and `&p[n]` and `&*(p + n)`.
- A function whose result nothing reads keeps its `s32` return where `void`
  changes its epilogue; `UBFIX` gives the `void` form. An update hook that
  the bytes show returning a value does the same, with `// UB:` above it.
- Palette RAM through `BG_PLTT_COLOR`, which is volatile, or through a plain
  pointer to it such as `(u16 *)&BG_PLTT_COLOR(14, 0)`.
- A work slot as `WORK_SLOT(n)`, as `gWorkSlot[n]`, or through a named
  pointer indexed past it (`(void **)&gCameraWork`), which is undefined
  behaviour with its defined form behind `UBFIX`.
- A message after a named one as `MsgName + n`, such as
  `MsgSaveDataCorrupted + 2` for `MsgSaveRecoveryFailed`, where the ROM adds
  to the register holding the first.

### Runtime

- A register write as `Io_Write16(value, &REG_X)`, which builds the value in a
  word first and keeps the store where it is written, or as `REG_X = value`.
- Code copied to RAM is called through a cast function pointer, or through
  `_call_via_r3`, the compiler library's call through r3, with the code's
  address as its fourth argument.
- A DMA control word is written with its flags above its count:
  `(DMA_ENABLE | DMA_32BIT) << 16 | words`, or `DMA_ENABLE << 16 | halfwords`.

### Field

- `*&gProjection.centerX` for `gProjection.centerX`
  ([`field/vertices.c`](field/vertices.c)), and `*(s32 *)&metadata->frames`
  for the frames pointer (`AnimationObject_Allocate`).
- `(mapState->anim + channelIndex)->` for `mapState->anim[channelIndex].`
  (`MapAnimation_Update`).
- `Scheduler_AddOrUpdateCallback` called through a cast to a `void` function
  (`Map_LoadLayeredScene`, with the declared call behind `UBFIX`).
- `(u16)~(...)` for a halfword mask (`Map_SetShownLayers`).
- Three loops written with `goto` (`Sprite_ComposeAnimationFrame`,
  `AnimationObjects_SelectAnimation`), with their loop forms measured in the
  comments beside them. The other `goto`s (the wander retries, the step
  searches, the map effects' script loops and the debug loops) stay until
  their loops are found.

### Field objects

Not all of these sites have been measured; each keeps its spelling until it
is:

- the field work: `(struct FieldWork *)gEventWork`,
  `WORK_SLOT(HEAP_SLOT_EVENT)`, or `Runtime_AllocateBlock(HEAP_SLOT_EVENT,
  sizeof(struct FieldWork))`, which returns the existing block;
- the effect scene: `gEffectWork` or `*(gWorkSlot + HEAP_SLOT_FX)`;
- the map's view: `gMapWork[0]` or `WORK_SLOT(8)`.

Measured:

- `((16 - blend) << 8) | blend` in
  [`object/psynergy_effects/world_map.c`](object/psynergy_effects/world_map.c)
  is `BLEND_ALPHA(blend, 16 - blend)` with its operands swapped; the macro
  changes the registers.

### Scenes

- A field read or written through its offset, `*(T *)((u8 *)p +
  offsetof(struct FieldActor, field))`, where `p->field` goes through
  FieldActor's single/group union and moves a load
  ([`scene/common/effect_spawn.h`](scene/common/effect_spawn.h),
  [`scene/common/mercury_lighthouse_pillar.h`](scene/common/mercury_lighthouse_pillar.h)).
- Another struct's view of the same object (`struct SolSanctumRingObject`
  for a FieldActor) in
  [`scene/common/sol_sanctum_elemental_star_ring_setup.h`](scene/common/sol_sanctum_elemental_star_ring_setup.h).
- A byte store through a `u8 *` and `offsetof` in place of a struct field
  (`Colosso_RestoreCompetitor`).
- `gMapWork[0]` read as a `u32` (`SceneActor_MoveAndRedraw`).
- A command of `event.h` and the service it names; each site keeps the one it
  matches with.

### Windows

- The work block as `UI_WORK` or as `gWindowWork[0]`, and a neighbouring heap
  block as `gWindowWork[n]`.
- A tilemap cell as `work->tilemap + y * 32 + x`, or as a byte offset added to
  `(u32)work->tilemap`.

### Menus

- A unit copied with the resident IWRAM routine as `Iwram_CopyWords`, through
  `_call_via_r3(..., IwramCopyWords)` (`Owner_Copy`), or through
  `Item_CopyWords` (tagged).
- A neighbouring heap block as `cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL]`
  from the menu control slot.

### Battles

- An action slot's first halfword is cleared through a cast,
  `*(u16 *)&member->actionSlots[i] = 0`, where the field store loads its
  zero differently ([`battle/battle/debug.c`](battle/battle/debug.c)).
- `BLEND_ALPHA(16, 16) - step` and `step + BLEND_ALPHA(0, 16)` where the
  macro's `|` form changes the code (`background.c`).
- Work slots through `(void **)&gCameraWork` and `(void **)&gBattleWork`
  indexed past them (see Everywhere).

### Battle effects

- A work slot is read as `WORK_SLOT(n)` or `gWorkSlot[n]`, through a base
  pointer as `slots[n - base]`, `EFFECT_SLOT` or `CANVAS_SLOT`, and as a
  blitter as `EFFECT_BLITTERS[n]` or `(DrawRectangle)gWorkSlot[n]`.

### Game rules

- A unit as `UnitGet(owner)`, or as the far-call `UnitGetFar(owner)` from
  other banks.
- An element pair array as `s16 [4][2]` where the matching code indexes it, or
  through `struct BattleElementStats` with `UBFIX`.

### Sound

- The driver's command table is called through a pointer read from the
  table's entry (`*(&gSound_CommandTable[n])`); indexing it directly adds an
  instruction with `old_agbcc`.
- `engine.c` keeps the labelled jumps its matching forms need.
