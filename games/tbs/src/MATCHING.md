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
numbers ignored, or use the optional `alchemy-devtools diff` on a function.

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
    place of the plain call or operator; a call through a cast to another
    function type is one wherever the callee has a declared type, with the
    declared call behind `UBFIX`, unlike a call to code with no function type
    (see Runtime);
  - `volatile`: a qualifier that forces a load or store to stay where it is
    written; one that is a macro's own form, such as the fill word of
    `Dma_Fill32` (pret's `DmaFill32`), is no device;
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
  as a test that is always true or an initializer each pass repeats
  (`Scheduler_SortTasks`), where the construct's own compare, branch, load or
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
- Palette RAM and VRAM through a volatile pointer or a plain one. Palette RAM
  as `BG_PLTT_COLOR`, which is volatile, or through a plain pointer to it
  such as `(u16 *)&BG_PLTT_COLOR(14, 0)`; `BattleFx_RunJudgment`'s
  brightening ramp keeps a `volatile u16 *` from `&BG_PLTT_COLOR(0, 1)`,
  where a plain pointer hoists red's mask out of the loop (24 of 2455
  instructions differ in tbs-en). `Menu_RunCommandRow` writes the
  command-icon tiles through `volatile u16 *vram`, where a plain pointer
  moves the address adds (18 of 1037 instructions differ in mode 1, 19 in
  mode 0).
- A work slot as `WORK_SLOT(n)`, as `gWorkSlot[n]`, or through a named
  pointer indexed past it (`(void **)&gCameraWork`), which is undefined
  behaviour with its defined form behind `UBFIX`.
- A member through a pointer to its own type, or to its own size with the
  other signedness (`*(T *)&p->member`, or `*&p->member`), for `p->member`.
  These are valid aliases; each site keeps an access where the member form
  moves it or merges it with a neighbour's:
  - `*&gProjection.centerX` ([`field/vertices.c`](field/vertices.c));
  - `*(s8 *)&output->index`, which keeps the index store ahead of the
    attribute word (`DjinnMenu_DrawStatArrow`);
  - `*(u16 *)&output->x`, which loads the -8 before x
    (`RenderOutput_UpdateScaleAnimation`);
  - `*(s32 *)&actor->y` for the union's `actor->y.fixed`, where the member
    form schedules the step to the next log after the store
    (`KolimaForestBend_CreateLogs`, 1 of 82 instructions in tbs-en);
  - `*(u8 *)&actor->priorityFlags`, which the compiler treats as no struct
    reference; as member stores, the loop is reallocated (Crossbone Isle's
    `Event_MovePillar`, 100 of 265 instructions in tbs-en);
  - `*(u8 *)&unit->field` in `BattleFx_RollSuccess`
    ([`game/game/battle_calc.c`](game/game/battle_calc.c)), where each zero
    test as `unit->field` merges with a neighbour's into one halfword or word
    load (sleep and psySeal; delusion, confusion and charm; haunt and
    grimReaper), and 30 of 204 instructions differ in tbs-en;
  - Altin's hop countdowns, which read and decrement `FieldActor.work` as a
    signed halfword and reset it through the declared unsigned member;
  - the Goma stump turns, which read `AnimationObject.rotation` as a signed
    halfword, keeping the negative literal.
- A value through a view of another type, which is undefined behaviour
  marked `// UB:`, with the declared access behind `UBFIX`:
  - a pointer read or stored as an integer: `*(s32 *)&metadata->frames`,
    which keeps the zero load ahead of it (`AnimationObject_Allocate`); the
    `u32` views of `gMapWork[0]` (`SceneActor_MoveAndRedraw`) and of
    `gEventWork` (`TolbiShipDeck_SightLand`, where the word may alias the
    halfword facing stores before it and so stays after them); the update
    hooks `AltmillerCave_Initialize` stores as `s32` (as `->update`, one
    store moves: 1 of 403 instructions in tbs-en), and those the Goma stump
    turns clear as integers (the pointer store swaps with the sprite-pointer
    load);
  - a member stored as a pointer of another type:
    `*(struct UiWindow **)&entry->attributes` and `*(void **)&entry->tile`
    for the unions' `.word` and `.value`, which keep the stores around them
    in order (`DjinnMenu_DrawStatArrow`), and
    `*(struct RenderOutput **)&list->tailLink = node` for
    `list->tailLink = &node->next` (`RenderOutput_AppendToList`);
  - a member through its neighbour's pointer, `((s16 *)nativeAttributes)[7]`
    for `entry->y` (`AffineEffect_UpdateFrame`).
- A random value masked in place, the mask assigned first
  (`speed = 0x3ff; speed &= Random16();`): as `Random16() & 0x3ff` the mask
  goes into a scratch register and the result is copied before the `and`, so
  3 instructions differ at each site. `BattleFx_RunNeptune`,
  `BattleFx_RunPlanetDiver`, `BattleFx_RunRamses`,
  `BattleFx_RunMoteFountain`, `BattleFx_RunTitanBlade` (`0x1FF`),
  `BattleFx_RunThor` (`THOR_LAUNCH_SPARKS` too) and `ReelGame_Run`
  ([`object/colorfx.c`](object/colorfx.c)) keep it.
- A message after a named one as `MsgName + n`, such as
  `MsgSaveDataCorrupted + 2` for `MsgSaveRecoveryFailed`, where the ROM adds
  to the register holding the first.

### Runtime

- `Vector_AddPolarOffset` walks positions with a byte cursor so both struct
  and array positions are valid. Its two one-coordinate steps retain the
  native writeback; combining them changes register allocation and stores.
- A register write as `Io_Write16(value, &REG_X)`, which builds the value in a
  word first and keeps the store where it is written, or as `REG_X = value`.
  `Scene_RunEntrySetup`
  ([`scene/venus_lighthouse.c`](scene/venus_lighthouse.c)) clears
  `REG_BLDCNT` with `Io_Write16(zero, &REG_BLDCNT)`, where `REG_BLDCNT = 0`
  does not match.
- `IRQ_LOCK` written as its steps, with the IME pointer set inside the save,
  `IME_SAVE((ime = &REG_IME), saved); IME_DISABLE(ime);`
  (`SerialRuntime_Initialize`), which reads IME before the frame is made; as
  `IRQ_LOCK`, 1 of 123 instructions differs.
- Code with no function type is called through a cast function pointer. Code
  copied to RAM is called through one, or through `_call_via_r3`, the
  compiler library's call through r3, with the code's address as its fourth
  argument. `System_SoftReset` (inlined into `WaitFrames`) calls the untyped
  fixed address `Cart_Restart` through a local `s32 (*reset)(void)`, which
  builds the address in r0 at both resets: through a `void (*)(void)` local,
  one reset moves to r1 (3 of 403 instructions differ), and cast and called
  with no local, 10 differ. With no declared type to differ from, these
  casts are no `wrapper`.
- A DMA control word is written with its flags above its count:
  `(DMA_ENABLE | DMA_32BIT) << 16 | words`, or `DMA_ENABLE << 16 | halfwords`.

### Field

- `(mapState->anim + channelIndex)->` for `mapState->anim[channelIndex].`
  (`MapAnimation_Update`).
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
- the map's view: `gMapWork[0]` or `WORK_SLOT(8)`;
- the field blend work: `gFieldBlendWork[0]`,
  `WORK_SLOT(HEAP_SLOT_FIELD_BLEND)` (`ValeBabi_Initialize`), or
  `Runtime_AllocateBlock(HEAP_SLOT_FIELD_BLEND, sizeof(struct
  FieldBlendWork))`, which returns the existing block;
- the colour buffer: `gPaletteBufferWork`,
  `WORK_SLOT(HEAP_SLOT_PALETTE_BUFFER)` (`TretPalette_UpdateSceneTint`,
  `SolSanctumLunaSolRooms_ShowFloatingEye`), or
  `*(gWorkSlot + HEAP_SLOT_PALETTE_BUFFER)`
  (`BattleEffect_InitializeSharedScene`); or from a neighbouring slot, as in
  Menus, as `gFieldBlendWork[HEAP_SLOT_PALETTE_BUFFER -
  HEAP_SLOT_FIELD_BLEND]` (`FieldFx_UpdateLightning`), or as
  `workCells[HEAP_SLOT_PALETTE_BUFFER - HEAP_SLOT_EVENT]` through
  `(u32 *)&gEventWork` (`Shop_SaveAndTintFieldPalette`), which is undefined
  behaviour with `gPaletteBufferWork` behind `UBFIX`.

Measured:

- `((16 - blend) << 8) | blend` in
  [`object/psynergy_effects/world_map.c`](object/psynergy_effects/world_map.c)
  is `BLEND_ALPHA(blend, 16 - blend)` with its operands swapped; the macro
  changes the registers.

### Scenes

- `FixedPoint_Distance` keeps its coordinate-address interface with byte
  views derived from whole actors. Its byte cursor reaches the actual
  signed coordinate members without crossing scalar bounds or copying
  them into temporary arrays.
- Calling a helper before its declaration, or with its exact `void`
  prototype in scope. In the affected Colosso and Kolima Forest bend helpers,
  adding the prototype changes the argument setup order. Their unit headers
  provide the correct declarations under `UBFIX`; the matching build keeps
  the call before the declaration.
- A field read or written through its offset, `*(T *)((u8 *)p +
  offsetof(struct FieldActor, field))`, where `p->field` goes through
  FieldActor's single/group union and moves a load
  ([`scene/common/effect_spawn.h`](scene/common/effect_spawn.h),
  [`scene/common/mercury_lighthouse_pillar.h`](scene/common/mercury_lighthouse_pillar.h),
  [`scene/crossbone_isle_pillar_rooms.c`](scene/crossbone_isle_pillar_rooms.c),
  and `Imil_MiaSaysFarewellToApprentices` in [`scene/imil.c`](scene/imil.c)).
- An actor read as `struct FieldEffect`, union FieldObject's effect view of
  the same object, whose sprite pointer is a plain member: through
  FieldActor's sprite union the flags address moves after the next store
  (`SolSanctum_StartFloatingLights` in
  [`scene/common/sol_sanctum_elemental_star_ring_setup.h`](scene/common/sol_sanctum_elemental_star_ring_setup.h)).
- Byte-addressed field stores with `sizeof` and `offsetof` rather than direct
  struct members (`Colosso_RestoreCompetitor`,
  `TolbiShipInteriors_SelectPlacements`, and row 10's talk facing in
  `Imil_GetPlacements`, where `script[10].talkFacing` adds the row's 240
  first and stores at 22 from another base). These valid byte views keep the
  native address bases and registers.
- The same byte view for `Xian_SpawnTrainingParticles` keeps the actor's Z
  load ahead of the options-script store. Direct members move it after the
  store and the following constant load in the English build.
- A global's member through a local pointer to the global, or through the
  global's own name, each where it matches. `Event_MoveBlock`
  ([`scene/crossbone_isle.c`](scene/crossbone_isle.c)) takes `&state->scene`
  with `state = &gGameState`, where `&gGameState.scene` folds into one
  literal (1 of 78 instructions differ in tbs-en, 1 of 65 in tbs-ja). McCoy
  Palace's `Scene_GetPlacements` writes the reward rows as
  `gMcCoyPalacePlacements[6].talkFacing` and on, where the local
  `placements[6].talkFacing` splits the address into base+144 and member+22
  (9 of 38 in tbs-en).
- An `s16` pair table read a word per index through a struct with an array
  member, `((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading]`,
  where an `s32` pointer to the table loses the native loads
  (`VaultCave_PushBlockAhead`); `UBFIX` builds the word from its halfwords.
- `__attribute__((section(".rodata")))` for `const` on
  `gStagedActor_DirectionSteps`, `gStagedActor_FootprintKinds` and
  `gStagedActor_FootprintBounds`
  ([`scene/lunpa_fortress.c`](scene/lunpa_fortress.c)): their shared externs
  in `field.h` and `common/object_staged_actor.h` lack `const`, so `const`
  conflicts, and without the attribute they go to `.data`.
- The Colosso race work (`HEAP_SLOT_COLOSSO_RACE`) as `gColossoWork`
  (`Colosso_DrawGauge`, `Colosso_UpdatePathRival`, `Colosso_StartRival`,
  `ColossoLog_CollectFinishEquipment`, `ColossoRiver_CollectFinishItem`) or
  as `WORK_SLOT(HEAP_SLOT_COLOSSO_RACE)`, whose pool entry is
  `gWorkSlot+236` (`Colosso_SetGaugeHold`,
  `ColossoWall_ApproachFinishAndTakeItem`, `ColossoWall_StartRivalPath`):
  the same address through another relocation, so no instruction differs.
  Where only the gauge is read, a `struct ColossoGaugeWork *` (the work's
  first member) compiles as the race work's `->gauge`.
- A three-script table as a local `const s32 *scripts[3] = {A, B, C};`,
  which compiles to an anonymous read-only constant copied into a 12-byte
  frame, or as a struct copy of a named table, which compiles to the same
  code. `ImilFuchinFalls_SpawnDust` keeps the local array: its old table was
  the first datum of the object's `.data` and the object has no other
  `.rodata`, so the constant lands where the table was.
  `GomaCavePillar_SpawnFragment` copies its named `struct EffectScripts`
  table, which sits mid-`.data`.
- A command of `event.h` and the service it names; each site keeps the one it
  matches with.

### Windows

- The work block as `UI_WORK` or as `gWindowWork[0]`, and a neighbouring heap
  block as `gWindowWork[n]`.
- A tilemap cell as `work->tilemap + y * 32 + x`, or as a byte offset added to
  `(u32)work->tilemap`.
- Save summaries through a signed byte cursor from `summary[0].party[4]`,
  indexed by `i * sizeof(struct SaveSummary) + SUMMARY_DISTANCE(party[4],
  field)`, for `summary[i].sendFlag` and `.flagCount`
  (`SaveState_ScanRecordFlags` in
  [`window/battle_command/save.c`](window/battle_command/save.c)), which
  keeps the literal 0x1070 and the offsets 1 and 2.
- A selection record's kind cleared through a view from its base field,
  `((struct GlyphCursor *)&work->records[n].base)->cursor = 0`, for
  `work->records[n].kind = 0` (`UiGlyph_ResetWorkState`, records 7 and 8).
- A render output read as `struct SpriteOutput`, whose sprite cell is the
  member `attr`, rather than as `struct RenderOutput` with a cast of `unk10`
  (`UiWindow_PutGlyph`, `UiWork_AnimateSpriteSlots`).
- A menu cursor whose screens write only the output pointer as
  `union MenuCursor` rather than an equal-size struct, which keeps the store
  order (`OptionMenu_Open`).
- `gRenderResource_PairSourceTable` under three more extern names, given as
  `__asm__` labels, one per switch case, where the ROM keeps a literal for
  each case (`RenderResource_LoadTableEntry`).
- The Spanish and Italian possessive copy counts letters in the function's
  `s32` value, compared as `(u32)value < 16`, rather than in `i` or `j`
  (`UiText_BuildRenderEntries`).
- In the Japanese small font, `nextRow = row + 1` in both arms of
  `if (bits == 0)` rather than once ahead of the test (`UiText_DrawGlyphArm`
  in [`window/glyph_arm.c`](window/glyph_arm.c)).

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
  `BattleFx_RunParticleStreams` reads the background and blitter slots
  through one base pointer, where two absolute reads grow it by 16 bytes in
  tbs-en; `BattleFx_RunIce` reads its two blitters one each way (both cast,
  2 of 586 instructions differ; both through `EFFECT_BLITTERS`, 3).
- A signed halving written as its expansion, `x = p->x; x += (u32)x >> 31;
  x >>= 1;`, which lets the final shift follow later arithmetic (the Forcible
  Arm and Truncheon Fist flame's centre in `BattleFx_RunCastingImpact`). As
  `p->x / 2` in each arm, 57 of 3070 instructions differ; as `x /= 2`, 6.
- A statement macro takes the shape all its uses match with. In
  `BattleFx_BuildBlitter`'s second walk
  ([`effect/parts/blitter.c`](effect/parts/blitter.c)), `PUT`, `COPY` and
  `PATCH` are `do { } while (0)` blocks, `COPY_FROM` and `PUT_BRANCH` plain
  statements and `COPY_TWICE` a plain `{ }` block; as plain statements,
  `PUT`, `COPY` and `PATCH` change 18, 6 and 1 of 737 instructions in tbs-en.

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
