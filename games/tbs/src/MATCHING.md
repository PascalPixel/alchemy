# Matching

Alchemy's C has to compile, with an old [compiler](../glossary.md#the-compilers), into exactly the
instructions [Camelot](../glossary.md#camelot) shipped.

A [decompilation](../glossary.md#matching-decompilation) is a program written by hand, in readable C, that a compiler
turns back into the very bytes on the cartridge. Matching is the work of
getting there one function at a time: write the function the way a person
would, build the game, and compare. That takes some care. Two ways of writing
the same thing in C can compile to different instructions, and now and then a
function only matches in an odd shape until someone finds the plain one. When
you're reading the source and a line looks stranger than it needs to be, the
reason is usually written down here.

The build makes twelve [editions](../glossary.md#editions): two games, [The Broken Seal](../glossary.md#the-two-games) (`tbs`) and The
Lost Age (`tla`), each in Japanese, English, German, Spanish, French and
Italian (`tbs-ja` to `tbs-it`). The Broken Seal's six are rebuilt entirely
from source; The Lost Age is still being matched. Measurements on this page
are taken in `tbs-en`, the English edition, unless they say otherwise.

## How a build is judged

Each built edition is compared with the original cartridge by its
fingerprint, and twelve matching fingerprints are a pass.

`make compare-editions` builds all twelve editions and checks each ROM's
SHA-1 hash against its line in `rom.sha1` (the `compare-%` rule in the
`Makefile`). Each edition that matches prints a line ending in `: OK`, and
twelve of them is a pass. The output is long, so it's easiest to send it to a
log file and count those lines. The hash says only whether an edition is exact,
never where it differs: changing a single bit anywhere gives a completely
different one. `make compare-tbs` checks the six Broken Seal editions
on their own, and `make compare-tbs-en` just the one.

The judgement is a hash of the whole image because the hash is the only thing
from the original cartridge the build is allowed to see: the reference ROM
judges the build and never feeds it. That makes the check all or nothing. One
wrong byte anywhere fails its edition, and a hash can't say where that byte
is. To find it, compare the compiler's output before and after your change.
The build keeps each C file's assembly beside its object
(`out/tbs-en/obj/games/tbs/src/runtime/runtime.s` for `runtime.c`): copy it
from a build that compared OK, rebuild, and diff the two with the `.L` label
numbers ignored. The optional `alchemy-devtools diff` instead compares one
function with the original's instructions read from your own ROM; it is a
tool for people and never part of the build.

The Broken Seal's editions need no ROM at all. The Lost Age's need your own
ROMs under `roms/`, because the parts of The Lost Age that aren't C yet still
come from `.incbin "baserom.gba"` scaffolds under `recon/tla`: bytes copied
from the ROM, which don't count as decompiled.

A change made on purpose, such as a mod or a fix for one of [the original](../glossary.md#the-original)'s
bugs, changes the bytes, so it fails the comparison. That's expected: the
comparison only tells you whether a tree is still the original game. So that
kind of work lives apart from the matching tree, or behind [`BUGFIX`](../glossary.md#bug-bugfix-and-ubfix), which is
off.

## What the markers mean

### FAKEMATCH

Sometimes the natural way to write a piece of code compiles to slightly
different instructions from the original's, and no amount of reshaping finds
a plain form that matches. Then the code uses a device: something that changes
how the compiler lays the code out without changing what the code does. Every
device carries a tag inside the function it steers:

```c
/* FAKEMATCH(kind): the form kept; as <plain form>, N of M instructions
 * differ in tbs-en. */
```

The tag names the plain form and says how far it is from matching: N of the
function's M instructions differ, measured over the whole function against the
edition named. A real one, from `WaitFrames`:

```c
/* FAKEMATCH(volatile): the size reread through a volatile pointer, a
 * load apart from the test's; as *size, 2 of 403 instructions differ in
 * tbs-en. */
base -= *(volatile u32 *)size;
```

The kind says which device it is:

- `wrapper` is a call made through a helper, a cast or a library routine in
  place of the plain call or operator. Calling a declared function through a
  cast to another function type is a `wrapper`, with the declared call behind
  `UBFIX`. Calling code that has no declared function type at all, such as
  code copied into RAM, is not (see Runtime below).
- `volatile` is a qualifier that forces a load or store to stay where it's
  written. One that is part of a macro's own form, such as the fill word of
  `Dma_Fill32` ([pret](../glossary.md#pret)'s `DmaFill32`), is no device. Nor is one on memory an
  interrupt handler shares with the main loop, such as `gVBlankDone` and
  `gFrameCount` (`include/frame.h`): there the C needs it to be correct.
- `temporary` is a variable holding a value the plain form wouldn't name.
- `dead` is code that emits nothing and only changes which registers the
  compiler picks or the order of the instructions.
- `one-pass` is a block or loop that runs once (`do { } while (0)`). GCC
  marks every C loop with internal loop notes, and those notes end the
  stretch in which its common subexpression pass (CSE) reuses values it has
  already computed, and stop its scheduler moving instructions across them.
- `barrier` is an empty `asm`. It emits nothing, but counts as four bytes when
  the compiler places literal pools, the blocks of constants it puts among a
  function's code for the function to load.
- `pin` is a variable fixed to a register.
- `signature` is a function declared with a return or parameter type it
  doesn't have, such as a `void` function typed `int`.

A device is the last resort, used only after reshaping the code has failed.
Taking one out again is part of humanizing, the project's second stage after
matching, which renames, types and explains the matched C until it reads
like ordinary C. Some older tags in The Lost Age's code have no kind yet
(`/* FAKEMATCH: ... */`); they mean the same thing.

### The other markers

`// DEAD: why` marks code the ROM keeps that can't change the result, such as
a test that is always true or an initializer that every pass repeats
(`Task_Sort`). It's only used where the construct's own compare,
branch, load or loop is there in the ROM, which makes it the source's form
rather than a device.

`// BUG: what goes wrong` marks a bug of the original game, kept as it is.
Where there's a fix, it sits behind `BUGFIX` (`games/tbs/include/config.h`),
which is off. `// UB:` marks undefined behaviour kept in the form that
matches; the defined form is behind `UBFIX`, which `make modern` and `BUGFIX`
both turn on.

`CAMELOT_ASM("proof")` marks the few handwritten instructions inside C, each
an `asm` statement right after it whose instruction no C compiles to:
`Dma_Set`, `Transform_SetIdentity`, `Iwram_Call2`, `WaitFrames`' stack switch,
`MulQ16`, and the `swi 2` and `swi 3` BIOS calls. The macro expands to
nothing; it is there to be read, and its proof says why the `asm` is needed.

[`UNUSED`](../glossary.md#unused) marks a function nothing calls, kept because its bytes are in the
ROM. It's GCC's `unused` attribute, so a current GCC doesn't warn about it.

An assembly file in `games/` has to show why it is assembly at all: that no
C compiler of the project's set could produce it. Each one opens with that
proof, written as comment lines (`@` starts a comment in assembly) that begin
`@ C2:`. C2 is the project's counting rule, which measures how much of each
game is rebuilt from source; under it, assembly counts only with such a
proof. In a handwritten file the
proof names the instruction, frame or pattern alchemy-gcc can't emit. The
[far-call](../glossary.md#far-call) tables, such as the `entry/` and `import/` files of the debug rooms,
the menus and the [maps](../glossary.md#map), carry one too. Each of their entries is a [veneer](../glossary.md#veneer),
a tiny piece of code that only jumps on: 8 bytes holding an `ldr r4` (load
the target's address), a `bx r4` (jump to it) and that address, which the
`overlay_veneer` macro writes out whole.

## Spellings that match at different sites

A compiler does not translate C word for word. It looks at the whole
function and picks instructions, and two lines that mean exactly the same
thing can lead it to different choices, as the masked random value in the
list below shows. The
original was compiled from one particular way of writing each line, so where
the plain spelling gives different instructions, the matching C keeps the one
that gives the original's.

Each idea has one preferred spelling. Where a site matches only with a plain-C
equivalent (an index or a pointer form, a macro or its expansion, a cast), that
site keeps the equivalent, untagged, and it's listed here. Each pair means the
same thing in C, and each can still compile differently.

The lists below are reference, grouped by the folders of the source: search
them for a strange line you meet in the code rather than reading them
through. They use the engine's words: a [heap slot](../glossary.md#heap-slot) is one of
the numbered words of `gHeapSlots` that hold a system's working block, and an
`ObjectEvent` is the struct an [object event](../glossary.md#object-event) is read through.

### Everywhere

These turn up all over the game.

- `p[n]` and `*(p + n)` can compile differently, and so can `&p[n]` and
  `&*(p + n)`.
- A function whose result nothing reads keeps its `s32` return where `void`
  would change its epilogue, the instructions that end it; `UBFIX` gives the
  `void` form. An update hook that the bytes show returning a value does the
  same, with `// UB:` above it.
- Palette RAM and VRAM are written through either a volatile pointer or a
  plain one. Palette RAM is either `BG_PLTT_COLOR`, which is volatile, or a
  plain pointer to it such as `(u16 *)&BG_PLTT_COLOR(14, 0)`.
  `BattleAnim_RunJudgment`'s brightening ramp, which raises each colour's
  red, green and blue levels in turn, keeps a `volatile u16 *` from
  `&BG_PLTT_COLOR(0, 1)`, because with a plain pointer the mask that picks out
  red is hoisted out of the loop (24 of 2455 instructions differ in tbs-en).
  `BattleCommandRow_Run` writes the command icons' tiles through
  `volatile u16 *vram`, because a plain pointer moves the address adds (18 of
  1037 instructions differ in a member's row, `COMMAND_ROW_MEMBER`, and 19 in
  the party's, `COMMAND_ROW_PARTY`).
- A heap slot is written `HEAP_SLOT_BLOCK(n)`, `gHeapSlots[n]`, or through a named
  pointer indexed past it (`(void **)&gCameraWork`). The last is undefined
  behaviour, with its defined form behind `UBFIX`.
- A member can be reached through a pointer to its own type, or to its own
  size with the other signedness (`*(T *)&p->member`, or `*&p->member`),
  instead of as `p->member`. These are valid aliases. Each of these sites keeps
  one where the member form would move the access or merge it with a
  neighbour's:
  - `*&gProjection.centerX` ([`field/vertices.c`](field/vertices.c));
  - `*(s8 *)&sprite->index`, which keeps the index store ahead of the
    attribute word (`DjinnMenu_DrawStatArrow`);
  - `*(u16 *)&windowSprite->x`, which loads the -8 before x
    (`WindowSprite_UpdateScaleAnimation`);
  - `*(s32 *)&actor->y` for the union's `actor->y.fixed`, because the member
    form schedules the step to the next log after the store
    (`KolimaForestFloodgate_CreateLogs`, 1 of 82 instructions in tbs-en);
  - `*(u8 *)&actor->priorityFlags`, which the compiler doesn't treat as a
    struct reference; written as member stores, the loop's registers are
    allocated differently (Crossbone Isle's `EventScript_MovePillar`, 100 of 265
    instructions in tbs-en);
  - `*(u8 *)&unit->field` in `Battle_RollEffectSuccess`
    ([`game/game/battle_calc.c`](game/game/battle_calc.c)), where each zero
    test written as `unit->field` merges with a neighbour's into one halfword
    or word load (sleep and psySeal; delusion, confusion and charm; haunt and
    grimReaper), and 30 of 204 instructions differ in tbs-en;
  - the hop countdowns of Altin, a town (`map/altin.c`), which read and
    decrement `ObjectEvent.work` as a signed halfword and reset it through
    the declared unsigned member;
  - the turns of the stump that leaps out of the way at Goma Cave's entrance
    (`map/goma_cave_entrance.c`), which read `Sprite.rotation` as a
    signed halfword, keeping the negative literal.
- A value can be reached through a view of another type. That is undefined
  behaviour, marked `// UB:`, with the declared access behind `UBFIX`:
  - a pointer read or stored as an integer: `*(s32 *)&metadata->frames`,
    which keeps the zero load ahead of it (`SpriteAnim_Alloc`); the
    `u32` views of `gMapGridWork[0]` (`ObjectEvent_MoveAndRedraw`) and of
    `gFieldWork` (`TolbiShipDeck_SightLand`, where the word may alias the
    halfword facing stores before it and so stays after them); the update
    hooks `AltmillerCave_Initialize` stores as `s32` (as `->update`, one store
    moves: 1 of 403 instructions in tbs-en); and the hooks the Goma Cave stump's
    turns clear as integers (otherwise the pointer store swaps with the
    [sprite](../glossary.md#sprite-and-oam)-pointer load);
  - a member stored as a pointer of another type:
    `*(struct Window **)&entry->attributes` and `*(void **)&entry->tile`
    for the unions' `.word` and `.value`, which keep the stores around them in
    order (`DjinnMenu_DrawStatArrow`), and
    `*(struct WindowSprite **)&list->tailLink = node` for
    `list->tailLink = &node->next` (`WindowSprite_AppendToList`);
  - a member through its neighbour's pointer: `((s16 *)nativeAttributes)[7]`
    for `entry->y` (`IconMenu_DrawIcons`).
- A loop can be written with `goto`. A C loop's notes let GCC's loop pass
  (`loop.c` in its source) move
  constants and addresses out of the loop, strength-reduce its counters and
  delete it when it is empty, and they let its jump pass (`jump.c`) copy its exit test to its
  top; register allocation also counts the references inside a C loop as
  more frequent. A loop built from labels and `goto` gets none of this, so
  where the ROM shows none of it the loop stays a `goto`, and the comment
  beside it gives the measured cost of its loop form.
- A random value is masked in place, with the mask assigned first
  (`speed = 0x3ff; speed &= Random16();`). Written as `Random16() & 0x3ff`,
  the mask goes into a scratch register and the result is copied before the
  `and`, so 3 instructions differ at each site. `BattleAnim_RunNeptune`,
  `BattleAnim_RunPlanetDiver`, `BattleAnim_RunRamses`, `BattleAnim_RunMoteFountain`,
  `BattleAnim_RunTitanBlade` (`0x1FF`), `BattleAnim_RunThor` (`THOR_LAUNCH_SPARKS`
  too) and `LuckyWheels_Run` ([`overworld/lucky_wheels.c`](overworld/lucky_wheels.c)) keep it.
- A message that follows a named one is written as `MsgName + n`, such as
  `MsgSaveDataCorrupted + 2` for `MsgSaveRecoveryFailed`, where the ROM adds to
  the register already holding the first.

### Runtime

These belong to the engine's lowest layer: positions, hardware registers,
interrupts and calls into RAM.

- `Vector_AddPolarOffset` walks a position with a byte cursor, so it works on
  any three coordinates in a row, whether they're a struct's members or an
  array. Its two one-coordinate steps across y keep the original's writeback;
  combining them changes the register allocation and the stores.
- A register write is either `Io_Write16(value, &REG_X)`, which builds the
  value in a word first and keeps the store where it's written, or plain
  `REG_X = value`. `Map_RunEntrySetup`
  ([`map/venus_lighthouse.c`](map/venus_lighthouse.c)) clears
  `REG_BLDCNT` with `Io_Write16(zero, &REG_BLDCNT)`, where `REG_BLDCNT = 0`
  doesn't match. `WorldMap_InitializePerspective`
  ([`field/field/world_map.c`](field/field/world_map.c)) writes the affine
  identity with `Io_Write16` and clears `REG_BG3Y` with `Io_Write16(0,
  &REG_BG3Y)`, which keep the 0x100 for `REG_BG2PD` built just before its
  store; as plain stores, 3 of 331 instructions differ in tbs-en.
- `IRQ_LOCK` is written out as its steps in `SerialRuntime_Initialize`, with
  the IME pointer set inside the save:
  `IME_SAVE((ime = &REG_IME), interruptEnable); IME_DISABLE(ime);`. That reads
  IME before the stack frame is made; as `IRQ_LOCK`, 1 of 123 instructions
  differs.
- Code with no function type is called through a cast function pointer. Code
  copied to RAM is called through one too, or through `_call_via_r3`, the
  compiler library's call through r3, with the code's address as its fourth
  argument. `System_SoftReset` (inlined into `WaitFrames`) calls the untyped
  fixed address `Cart_Restart` through a local `s32 (*reset)(void)`, which
  builds the address in r0 at both resets. Through a `void (*)(void)` local,
  one reset moves to r1 (3 of 403 instructions differ), and cast and called
  with no local, 10 differ. With no declared type to differ from, these casts
  are no `wrapper`.
- A DMA control word is written with its flags above its count:
  `(DMA_ENABLE | DMA_32BIT) << 16 | words`, or `DMA_ENABLE << 16 | halfwords`.

### Field

The field engine's spellings are about its map state and a few of its loops.

- `(gridState->anim + channelIndex)->` stands for `gridState->anim[channelIndex].`
  (`TilesetAnim_Update`).
- `(u16)~(...)` makes a halfword mask (`MapGrid_SetShownLayers`).
- Three loops are written with `goto` (`Sprite_ComposeFrame`,
  `Sprite_StartAnim`), and the comments beside them give the
  measured cost of their loop forms. The other `goto`s (the wander retries,
  the step searches, the map effects' [script](../glossary.md#script) loops and the debug loops) stay
  until someone finds their loops.
- An [object](../glossary.md#object)'s 112 bytes are read as `struct Object`
  almost everywhere, and the engine's other views of it have folded into it,
  as Field's [one object, a few views](field/README.md#one-object-a-few-views)
  lists. What the merge measured:
  - `Object.sprite` is a plain `struct Sprite *`. As an anonymous union of a
    sprite and a group list, its stores move after the next store
    (`Object_CreateField`, `ObjectEvent_Restore`); as `void *`, the leader's sprite priority load in
    `FieldEffect_UpdatePhasedRadialParticle` moves. A group's list is read
    through a `(void **)` or `(struct Sprite **)` cast.
  - `BattleObject_SpawnObjectsForList` matches only with the sprite word
    untyped, and only through a field-for-field copy of the layout
    (`BattleSpriteObject`): a view that pads the fields it does not read with
    byte arrays moves the same store.
  - `Object.wait` is signed, as the motion loop (`Object_UpdateAllThumb`)
    reads it; every other site matches with either signedness.
  - `Object_UpdateAllThumb` indexes the script as words through
    `(const ScriptMotionWords *)object->script`, which compiles as the old
    word-array member did.
  - `update.c` keeps `ScriptMotionObject`: it is ARM code built with GCC
    2.95.2, which cannot read `Object`'s anonymous unions.
  - The operand handlers of `SC_SET`, `SC_ADD` and `SC_CMP` keep
    `ScriptOperands`, whose fields are the unsigned widths they read.

### Running a map

The engine keeps each system's working state in a [heap](../glossary.md#heap-and-pools) block whose pointer
sits in a numbered slot of `gHeapSlots`, and many of those slots also have a
name of their own (`gFieldWork`, `gFieldEffectWork` and so on), so the same block
can be reached two or three ways. Not all of these sites have been measured;
each keeps its spelling until it is.

- The field work is `gFieldWork`,
  `HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD)`, or `AllocHeapSlotEwram(HEAP_SLOT_FIELD,
  sizeof(struct FieldWork))`, which returns the existing block.
- The effect scene is `gFieldEffectWork` or `*(gHeapSlots + HEAP_SLOT_FIELD_EFFECT)`.
- The map grid's view is `gMapGridWork[0]` or `HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID)`.
- The field blend work is `gLightningWork[0]`,
  `HEAP_SLOT_BLOCK(HEAP_SLOT_LIGHTNING)` (`ValeInteriorsInn_Initialize`), or
  `AllocHeapSlotEwram(HEAP_SLOT_LIGHTNING, sizeof(struct
  LightningWork))`, which returns the existing block.
- The palette fade is `gPaletteFadeWork`,
  `HEAP_SLOT_BLOCK(HEAP_SLOT_PALETTE_FADE)` (`TretPalette_UpdateMapTint`,
  `SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue`), or
  `*(gHeapSlots + HEAP_SLOT_PALETTE_FADE)`
  (`FieldEffect_FadeToEffectColor`). It's also reached from a
  neighbouring slot, as in Menus below: as
  `gLightningWork[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_LIGHTNING]`
  (`Weather_UpdateLightning`), or as
  `workCells[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_FIELD]` through
  `(u32 *)&gFieldWork` (`Shop_SaveAndTintFieldPalette`), which is undefined
  behaviour with `gPaletteFadeWork` behind `UBFIX`.

One more spelling in the objects has been measured:
`((16 - blend) << 8) | blend` in
[`overworld/field_services/world_map.c`](overworld/field_services/world_map.c)
is `BLEND_ALPHA(blend, 16 - blend)` with its operands swapped, and the macro
changes the registers.

### Maps

Map code reaches straight into object events and their tables, so many of its sites
come down to how a struct's member is addressed.

- `FixedPoint_Distance` takes the two positions as byte addresses, which its
  callers take from whole object events. Its byte cursor steps across x, y
  and z, so it reads the object events' own signed coordinate members without treating them as
  an array or copying them into one.
- Some helpers in Colosso and in `MapId_KolimaForestFloodgate`, where logs
  roll, are called before they're declared. With their exact
  `void` prototype in scope, the arguments are set up in another order, so the
  matching build keeps the call ahead of the declaration, and their unit
  headers give the correct declarations under `UBFIX`.
- A field is read or written through its offset,
  `*(T *)((u8 *)p + offsetof(struct ObjectEvent, field))`, where `p->field`
  goes through ObjectEvent's single/group union and moves a load
  ([`map/common/effect_spawn.h`](map/common/effect_spawn.h),
  [`map/common/mercury_lighthouse_pillar.h`](map/common/mercury_lighthouse_pillar.h),
  [`map/crossbone_isle_stepping_stones.c`](map/crossbone_isle_stepping_stones.c),
  and `Imil_MiaSaysFarewellToApprentices` in [`map/imil.c`](map/imil.c)).
- An [object event](../glossary.md#object-event) is read as `struct FieldEffect`, union FieldObject's effect view of
  the same object, whose sprite pointer is a plain member. Through
  ObjectEvent's sprite union, the flags address moves after the next store
  (`SolSanctum_StartFloatingLights` in
  [`map/common/sol_sanctum_elemental_star_ring_setup.h`](map/common/sol_sanctum_elemental_star_ring_setup.h)).
- Field stores are byte-addressed with `sizeof` and `offsetof` rather than
  made through the struct members in `Colosso_RestoreWarrior`,
  `TolbiShipInteriors_SelectObjectEventTemplates`, and for row 10's talk facing in
  `Imil_GetObjectEventTemplates`, where `script[10].talkFacing` adds the row's 240 first
  and stores at 22 from another base. These byte views are valid, and they
  keep the original's address bases and registers.
- The same byte view in `Xian_SpawnTrainingParticles` keeps the object event's Z load
  ahead of the options-script store. Through the members, the load moves after
  the store and the constant load that follows it in the English build.
- A global's member is reached through a local pointer to the global or
  through the global's own name, each where it matches. `EventScript_MovePillar`
  ([`map/crossbone_isle.c`](map/crossbone_isle.c)) takes `&state->mapId`
  with `state = &gSaveBlock`, because `&gSaveBlock.mapId` folds into one
  literal (1 of 78 instructions differ in tbs-en, 1 of 65 in tbs-ja). McCoy
  Palace's `Map_GetObjectEventTemplates` writes the reward rows as
  `gMcCoyPalaceObjectEventTemplates[6].talkFacing` and on, because the local
  `templates[6].talkFacing` splits the address into base+144 and member+22
  (9 of 38 in tbs-en).
- A table of `s16` pairs is read a word per index through a struct with an
  array member, `((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading]`,
  because an `s32` pointer to the table loses the original's loads
  (`VaultCave_PushPillarAhead`); `UBFIX` builds the word from its halfwords.
- `gPushedObject_DirectionSteps`, `gPushedObject_FootprintKinds` and
  `gPushedObject_FootprintBounds`
  ([`map/lunpa_fortress.c`](map/lunpa_fortress.c)) are put in read-only
  data with `__attribute__((section(".rodata")))` instead of `const`. Their
  shared externs in `field.h` and `common/object_staged_object_event.h` lack `const`,
  so `const` would conflict, and without the attribute they'd go to `.data`.
- The Colosso race work (`HEAP_SLOT_COLOSSO_RACE`) is `gColossoWork`
  (`Colosso_DrawGauge`, `Colosso_UpdatePathOpponent`, `Colosso_StartOpponent`,
  `ColosseumNavampaMatch_CollectFinishEquipment`, `ColosseumAzartMatch_CollectFinishItem`) or
  `HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE)`, whose literal pool entry is
  `gHeapSlots+236` (`Colosso_SetGaugeHold`,
  `ColosseumSatrageMatch_ApproachFinishAndTakeItem`, `ColosseumSatrageMatch_StartOpponentPath`). It's
  the same address through another relocation, so no instruction or byte
  differs; it is listed so a reader knows the two are one block.
  Where only the gauge is read, a `struct ColossoGaugeWork *` (the race work's
  first member) compiles the same as the race work's `->gauge`.
- A table of three scripts is either a local `const s32 *scripts[3] = {A, B,
  C};`, which compiles to an anonymous read-only constant copied into a
  12-byte frame, or a struct copy of a named table, which compiles to the same
  code. `ImilFuchinFalls_SpawnDust` keeps the local array: its old table was
  the first datum of the object's `.data` and the object has no other
  `.rodata`, so the constant lands where the table was.
  `GomaCavePillar_SpawnFragment` copies its named `struct EffectScripts`
  table, which sits in the middle of `.data`.
- A command of `script.h` and the service it names are interchangeable; each
  site keeps the one it matches with.
- A game flag is tested, set or cleared by a direct call
  (`GameFlag_Get`, `GameFlag_Set`, `GameFlag_Clear`) or through the script
  layer's inline of the same verb (`Script_GetFlag`, `Script_SetFlag`,
  `Script_ClearFlag`), each where it matches. An inline call's argument is a
  pseudo of its own, so a flag number used twice is loaded twice; called
  directly, the number is kept in a callee-saved register (Bilibin's chef,
  `EventScript_TalkToBusyChef`). Imil's and Sol Sanctum's Psynergy Stone
  room's calls of `GameFlag_Set` compile only against a `void` declaration and
  call it as `Engine_GameFlagSet` (`script_layer.h`).

### Windows

The window code keeps its state in a heap block of its own and writes
straight into tilemaps, so most of these are about reaching that block and
its cells.

- The work block is `WINDOW_WORK` or `gWindowWork[0]`, and a neighbouring heap
  block is `gWindowWork[n]`.
- A tilemap cell is `work->tilemap + y * 32 + x`, or a byte offset added to
  `(u32)work->tilemap`.
- The save summaries are read through a signed byte cursor from
  `summary[0].party[4]`, indexed by `i * sizeof(struct SaveFileSummary) +
  SUMMARY_DISTANCE(party[4], field)`, for `summary[i].sendFlag` and
  `.flagCount` (`Save_ScanRecordFlags` in
  [`window/screens/save.c`](window/screens/save.c)), which keeps
  the literal `0x1070` and the offsets 1 and 2.
- A selection entry's kind is cleared through a view from its base field,
  `((struct GlyphCursor *)&work->records[n].base)->cursor = 0`, for
  `work->records[n].kind = 0` (`SelectionScreen_Create`, entries 7 and 8).
- A window sprite is read as `struct SpriteOutput`, whose sprite cell is the
  member `attr`, rather than as `struct WindowSprite` with a cast of `unk10`
  (`Window_PutGlyph`, `WindowSprite_UpdateAll`).
- A menu cursor whose screens write only the sprite pointer is a
  `union MenuCursorRef` rather than a struct of the same size, which keeps the
  store order (`OptionMenu_Open`).
- `gHeadingPictures` goes by three more extern names, given as
  `__asm__` labels, one per switch case, because the ROM keeps a literal for
  each case (`RenderResource_LoadTableEntry`).
- The Spanish and Italian possessive copy counts its letters in the
  function's `s32` value, compared as `(u32)value < 16`, rather than in `i` or
  `j` (`Message_Decode`).
- In the Japanese small font, `nextRow = row + 1` is in both arms of
  `if (bits == 0)` rather than once ahead of the test (`Glyph_DrawArm`
  in [`window/glyph_arm.c`](window/glyph_arm.c)).
- `gFont_MarkGlyphCodes`, the Japanese kana marks, is declared without
  `const` and put in read-only data with `__attribute__((section(".rodata")))`
  ([`window/articles.c`](window/articles.c)), as Lunpa's pushed-object tables
  are. Through a `const` extern, its load in `Text_DrawGlyphPair` is
  scheduled ahead of the spill of `out` (2 of 185 instructions in tbs-ja).

### Menus

Two spellings recur in the menus, one for copying a party member's unit and
one for reaching a heap block.

- A unit is copied with the resident IWRAM routine as `Iwram_CopyWords`
  (`Unit_Copy` in `menu/djinn.c`), through
  `_call_via_r3(..., IwramCopyWords)` (`StatusPanel_CopyUnit`), or
  through `Item_CopyWords` (tagged).
- A neighbouring heap block is reached from the menu control slot as
  `cache[HEAP_SLOT_FIELD - HEAP_SLOT_OBJECT_SYSTEM]`.
- The menu work is read through one struct per screen
  (`ItemMenuWork`, `PsynergyMenuWork`, `SummaryScreenWork`,
  `DjinnMenuWork`, `DjinnFiguresWork`), as the object's views
  are, because each is what its own screen's code matches with.

### Battles

The battle engine's few spellings are a store, its background's blend
arithmetic and its heap slots.

- An action slot's first halfword is cleared through a cast,
  `*(u16 *)&member->psynergy[i] = 0`, because the field store loads its
  zero differently ([`battle/battle/debug.c`](battle/battle/debug.c)).
- The background's blend steps are `BLEND_ALPHA(16, 16) - alphaStep` and
  `step + BLEND_ALPHA(0, 16)`, where the macro's `|` form changes the code
  ([`battle/battle/background.c`](battle/battle/background.c)).
- Heap slots are reached through `(void **)&gCameraWork` and
  `(void **)&gBattleWork` indexed past them (see Everywhere).

### Battle animations

The animations lean hard on their heap slots, and some of them on arithmetic and
macros whose exact shape decides the code.

- A heap slot is read as `HEAP_SLOT_BLOCK(n)` or `gHeapSlots[n]`, through a base
  pointer as `slots[n - base]`, `ANIM_SLOT` or `CANVAS_SLOT`, and when it
  holds a blitter (a drawing routine `BattleAnim_BuildBlitter` writes into a heap
  slot) as `ANIM_BLITTERS[n]` or `(DrawRectangle)gHeapSlots[n]`.
  `BattleAnim_RunParticleStreams` reads the background and blitter slots through
  one base pointer, where two absolute reads make the function 16 bytes longer
  in tbs-en.
  `BattleAnim_RunIce` reads its two blitters one each way (both cast, 2 of 586
  instructions differ; both through `ANIM_BLITTERS`, 3).
- A signed halving is written out as its expansion, `x = p->x; x += (u32)x >>
  31; x >>= 1;`, which lets the final shift follow later arithmetic (the
  centre of the Forcible Arm and Truncheon Fist flame in
  `BattleAnim_RunCastingImpact`). As `p->x / 2` in each arm, 57 of 3070
  instructions differ; as `x /= 2`, 6.
- A statement macro takes the shape that all its uses match with. In
  `BattleAnim_BuildBlitter`'s second walk
  ([`battle_anim/parts/blitter.c`](battle_anim/parts/blitter.c)), `PUT`, `COPY` and
  `PATCH` are `do { } while (0)` blocks, `COPY_FROM` and `PUT_BRANCH` plain
  statements and `COPY_TWICE` a plain `{ }` block. As plain statements, `PUT`,
  `COPY` and `PATCH` change 18, 6 and 1 of 737 instructions in tbs-en.

### Game rules

The rules code reads [party](../glossary.md#the-party) members and [element](../glossary.md#the-four-elements) tables in two ways each.

- A unit is `Unit_Get(owner)`, or from other [banks](../glossary.md#bank) `Unit_GetFar(owner)`, the
  far call that reaches it.
- An element pair array is `s16 [4][2]` where the matching code indexes it,
  or goes through `struct BattleElementStats` with `UBFIX`.

### Sound

The sound driver is compiled with pret's `old_agbcc` rather than alchemy-gcc.

- The driver's command table is called through a pointer read from the
  table's entry (`*(&gMPlayJumpTable[n])`); indexing it directly adds an
  instruction with `old_agbcc`.
- `engine.c` keeps the labelled jumps its matching forms need.
