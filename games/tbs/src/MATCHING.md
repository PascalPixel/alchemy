# Matching

![How a function is matched. Write it in plain C. Build all twelve editions and compare each one's fingerprint with the one recorded for the original cartridge. If all twelve match, the C is the original game, byte for byte, and it is kept. If one does not, find the instructions that differ, by comparing the compiler's output with the original, and reshape the C: another plain way of writing the same thing, or an equivalent spelling. Then build and compare again. Only when no plain form matches is a steering device kept, tagged FAKEMATCH with how far the plain form is from matching.](https://alchemy.pascalpixel.com/illustrations/matching/loop.svg)

Alchemy's C has to compile, with an old compiler, into exactly the
instructions Camelot shipped.

A decompilation is a program written by hand, in readable C, that a compiler
turns back into the very bytes on the cartridge. Matching is the work of
getting there one function at a time: write the function the way a person
would, build the game, and compare. That takes some care. Two ways of writing
the same thing in C can compile to different instructions, and now and then a
function only matches in an odd shape until someone finds the plain one. When
you're reading the source and a line looks stranger than it needs to be, the
reason is usually written down here.

## How a build is judged

Each built edition is compared with the original cartridge by its
fingerprint, and twelve matching fingerprints are a pass.

![The build never sees the original cartridge, only its fingerprint: a SHA-1 hash, 40 characters long, recorded in rom.sha1. Each built edition is hashed the same way. When the English build of The Broken Seal comes out byte for byte the same as the cartridge, its fingerprint is identical, starting 5c4695205413, and the edition prints OK. Changing a single bit anywhere in its 8 MB, as a test, gives a completely different fingerprint, starting c214a5f65e8b. The fingerprint says whether the build is exact, never where it differs.](https://alchemy.pascalpixel.com/illustrations/matching/fingerprint.svg)

`make compare-editions` builds all twelve editions and checks each ROM's
SHA-1 hash against its line in `rom.sha1` (the `compare-%` rule in the
`Makefile`). Each edition that matches prints a line ending in `: OK`, and
twelve of them is a pass. The output is long, so it's easiest to send it to a
log file and count those lines. `make compare-tbs` checks the six ☀️ editions
on their own, and `make compare-tbs-en` just the one.

The judgement is a hash of the whole image because the hash is the only thing
from the original cartridge the build is allowed to see: the reference ROM
judges the build and never feeds it. That makes the check all or nothing. One
wrong byte anywhere fails its edition, and a hash can't say where that byte
is. To find it, compile the one object you changed to assembly with the
`Makefile`'s own command and diff it against a baseline with the `.L` label
numbers ignored, or run the optional `alchemy-devtools diff` on the function.

The ☀️ editions need no ROM at all. The ⚓️ editions need your own ROMs under
`roms/`, because the parts of The Lost Age that aren't C yet still come from
`.incbin "baserom.gba"` scaffolds under `recon/tla`, which never count.

A change made on purpose, such as a mod or a fix for one of the original's
bugs, changes the bytes, so it fails the comparison. That's expected: the
comparison only tells you whether a tree is still the original game. So that
kind of work lives apart from the matching tree, or behind `BUGFIX`, which is
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
edition named. The kind says which device it is:

- `wrapper` is a call made through a helper, a cast or a library routine in
  place of the plain call or operator. A call through a cast to another
  function type is one wherever the callee has a declared type, and the
  declared call is behind `UBFIX`. A call to code that has no function type at
  all is not (see Runtime below).
- `volatile` is a qualifier that forces a load or store to stay where it's
  written. One that is part of a macro's own form, such as the fill word of
  `Dma_Fill32` (pret's `DmaFill32`), is no device. Nor is one on memory an
  interrupt handler shares with the main loop, such as `gVBlankDone` and
  `gFrameCount` (`include/frame.h`): there the C needs it to be correct.
- `temporary` is a variable holding a value the plain form wouldn't name.
- `dead` is code that emits nothing and only changes which registers the
  compiler picks or the order of the instructions.
- `one-pass` is a block or loop that runs once (`do { } while (0)`). Its loop
  notes end a CSE block, the stretch in which the compiler's common
  subexpression pass reuses values it has already computed, and they act as a
  scheduling barrier that no instruction is moved across.
- `barrier` is an empty `asm`. It emits nothing, but counts as four bytes when
  the compiler places literal pools, the blocks of constants it puts among a
  function's code for the function to load.
- `pin` is a variable fixed to a register.
- `signature` is a function declared with a return or parameter type it
  doesn't have, such as a `void` function typed `int`.

A device is the last resort, used only after reshaping the code has failed,
and taking one out again is part of humanizing, the passes that make matched C
read like ordinary C. Some older tags in The Lost Age's code have no kind yet
(`/* FAKEMATCH: ... */`); they mean the same thing.

### The other markers

`// DEAD: why` marks code the ROM keeps that can't change the result, such as
a test that is always true or an initializer that every pass repeats
(`Scheduler_SortTasks`). It's only used where the construct's own compare,
branch, load or loop is there in the ROM, which makes it the source's form
rather than a device.

`// BUG: what goes wrong` marks a bug of the original game, kept as it is.
Where there's a fix, it sits behind `BUGFIX` (`games/tbs/include/config.h`),
which is off. `// UB:` marks undefined behaviour kept in the form that
matches; the defined form is behind `UBFIX`, which `make modern` and `BUGFIX`
both turn on.

`CAMELOT_ASM("proof")` marks the few proven handwritten helpers inside C:
`Dma_Set`, `Transform_SetIdentity`, `Iwram_Call2`, `WaitFrames`' stack switch,
`MulQ16`, and the `swi 2` and `swi 3` BIOS calls. It expands to nothing. It's
there to be read, and its proof says what no C compiles to.

`UNUSED` marks a function nothing calls, kept because its bytes are in the
ROM. It's GCC's `unused` attribute, so a current GCC doesn't warn about it.

An assembly file in `games/` has to show why it is assembly at all: that no
C compiler of the project's set could produce it. Each one opens with that
proof, written as comment lines (`@` starts a comment in assembly) that begin
`@ C2:`, after the project's counting rule, under which assembly counts as
done only with such a proof. In a handwritten file the
proof names the instruction, frame or pattern alchemy-gcc can't emit. The
far-call tables, such as the `entry/` and `import/` files of the debug rooms,
the menus and the scenes, carry one too. Each of their entries is a veneer,
a tiny piece of code that only jumps on: 8 bytes holding an `ldr r4` (load
the target's address), a `bx r4` (jump to it) and that address, which the
`overlay_veneer` macro writes out whole.

## Spellings that match at different sites

![Two ways of writing the same thing in C, from a small test compiled with alchemy-gcc, the compiler the game is built with. Both keep the low ten bits of a random number. Written as Random16() and 0x3ff in one expression, the compiler copies the result aside, loads the mask from a constant and combines them: three instructions after the call. Written as a mask assigned first and then combined with Random16(), it shifts the number left and back right instead: two instructions. Both are correct C with the same result, but only the instructions the original cartridge has will match, so each place in the game keeps the spelling that gives them.](https://alchemy.pascalpixel.com/illustrations/matching/spellings.svg)

A compiler does not translate C word for word. It looks at the whole
function and picks instructions, and two lines that mean exactly the same
thing can lead it to different choices, as the small test above shows. The
original was compiled from one particular way of writing each line, so where
the plain spelling gives different instructions, the matching C keeps the one
that gives the original's.

Each idea has one preferred spelling. Where a site matches only with a plain-C
equivalent (an index or a pointer form, a macro or its expansion, a cast), that
site keeps the equivalent, untagged, and it's listed here. Each pair means the
same thing in C, and each can still compile differently.

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
  `BattleEffect_RunJudgment`'s brightening ramp keeps a `volatile u16 *` from
  `&BG_PLTT_COLOR(0, 1)`, because with a plain pointer red's mask is hoisted
  out of the loop (24 of 2455 instructions differ in tbs-en).
  `Menu_RunCommandRow` writes the command icons' tiles through
  `volatile u16 *vram`, because a plain pointer moves the address adds (18 of
  1037 instructions differ in mode 1, 19 in mode 0).
- A work slot is written `WORK_SLOT(n)`, `gWorkSlot[n]`, or through a named
  pointer indexed past it (`(void **)&gCameraWork`). The last is undefined
  behaviour, with its defined form behind `UBFIX`.
- A member can be reached through a pointer to its own type, or to its own
  size with the other signedness (`*(T *)&p->member`, or `*&p->member`),
  instead of as `p->member`. These are valid aliases. Each of these sites keeps
  one where the member form would move the access or merge it with a
  neighbour's:
  - `*&gProjection.centerX` ([`field/vertices.c`](field/vertices.c));
  - `*(s8 *)&output->index`, which keeps the index store ahead of the
    attribute word (`DjinnMenu_DrawStatArrow`);
  - `*(u16 *)&output->x`, which loads the -8 before x
    (`RenderOutput_UpdateScaleAnimation`);
  - `*(s32 *)&actor->y` for the union's `actor->y.fixed`, because the member
    form schedules the step to the next log after the store
    (`KolimaForestFloodgate_CreateLogs`, 1 of 82 instructions in tbs-en);
  - `*(u8 *)&actor->priorityFlags`, which the compiler doesn't treat as a
    struct reference; written as member stores, the loop's registers are
    allocated differently (Crossbone Isle's `Event_MovePillar`, 100 of 265
    instructions in tbs-en);
  - `*(u8 *)&unit->field` in `Battle_RollEffectSuccess`
    ([`game/game/battle_calc.c`](game/game/battle_calc.c)), where each zero
    test written as `unit->field` merges with a neighbour's into one halfword
    or word load (sleep and psySeal; delusion, confusion and charm; haunt and
    grimReaper), and 30 of 204 instructions differ in tbs-en;
  - Altin's hop countdowns, which read and decrement `FieldActor.work` as a
    signed halfword and reset it through the declared unsigned member;
  - the Goma stump turns, which read `AnimationObject.rotation` as a signed
    halfword, keeping the negative literal.
- A value can be reached through a view of another type. That is undefined
  behaviour, marked `// UB:`, with the declared access behind `UBFIX`:
  - a pointer read or stored as an integer: `*(s32 *)&metadata->frames`,
    which keeps the zero load ahead of it (`Animation_AllocateLayer`); the
    `u32` views of `gMapWork[0]` (`SceneActor_MoveAndRedraw`) and of
    `gEventWork` (`TolbiShipDeck_SightLand`, where the word may alias the
    halfword facing stores before it and so stays after them); the update
    hooks `AltmillerCave_Initialize` stores as `s32` (as `->update`, one store
    moves: 1 of 403 instructions in tbs-en); and the hooks the Goma stump
    turns clear as integers (otherwise the pointer store swaps with the
    sprite-pointer load);
  - a member stored as a pointer of another type:
    `*(struct UiWindow **)&entry->attributes` and `*(void **)&entry->tile`
    for the unions' `.word` and `.value`, which keep the stores around them in
    order (`DjinnMenu_DrawStatArrow`), and
    `*(struct RenderOutput **)&list->tailLink = node` for
    `list->tailLink = &node->next` (`RenderOutput_AppendToList`);
  - a member through its neighbour's pointer: `((s16 *)nativeAttributes)[7]`
    for `entry->y` (`AffineEffect_UpdateFrame`).
- A loop can be written with `goto`. A C loop's notes let `loop.c` move
  constants and addresses out of the loop, strength-reduce its counters and
  delete it when it is empty, and they let `jump.c` copy its exit test to its
  top; register allocation also counts the references inside a C loop as
  more frequent. A loop built from labels and `goto` gets none of this, so
  where the ROM shows none of it the loop stays a `goto`, and the comment
  beside it gives the measured cost of its loop form.
- A random value is masked in place, with the mask assigned first
  (`speed = 0x3ff; speed &= Random16();`). Written as `Random16() & 0x3ff`,
  the mask goes into a scratch register and the result is copied before the
  `and`, so 3 instructions differ at each site. `BattleEffect_RunNeptune`,
  `BattleEffect_RunPlanetDiver`, `BattleEffect_RunRamses`, `BattleEffect_RunMoteFountain`,
  `BattleEffect_RunTitanBlade` (`0x1FF`), `BattleEffect_RunThor` (`THOR_LAUNCH_SPARKS`
  too) and `ReelGame_Run` ([`object/colorfx.c`](object/colorfx.c)) keep it.
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
  `REG_X = value`. `Scene_RunEntrySetup`
  ([`map/venus_lighthouse.c`](map/venus_lighthouse.c)) clears
  `REG_BLDCNT` with `Io_Write16(zero, &REG_BLDCNT)`, where `REG_BLDCNT = 0`
  doesn't match. `Map_InitializePerspectiveScene`
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

- `(mapState->anim + channelIndex)->` stands for `mapState->anim[channelIndex].`
  (`MapAnimation_Update`).
- `(u16)~(...)` makes a halfword mask (`Map_SetShownLayers`).
- Three loops are written with `goto` (`Sprite_ComposeAnimationFrame`,
  `AnimationObject_SelectAnimation`), and the comments beside them give the
  measured cost of their loop forms. The other `goto`s (the wander retries,
  the step searches, the map effects' script loops and the debug loops) stay
  until someone finds their loops.
- A field object's 112 bytes are read through several structs, as Field's
  [one object, many views](field/README.md#one-object-many-views) lists. They
  stay apart because the compiler addresses a field differently by its
  declared type and by whether it is reached through a union (the
  `FieldActor` union moves a load), so one merged struct would change the
  bytes at sites each view now matches. XXX: likely one struct in Camelot's
  source, because every view describes the same 112 bytes; merging them
  waits until every field has one name and type that every site matches
  with.

### Running a place

The engine keeps each system's working state in a heap block whose pointer
sits in a numbered slot of `gWorkSlot`, and many of those slots also have a
name of their own (`gEventWork`, `gEffectWork` and so on), so the same block
can be reached two or three ways. Not all of these sites have been measured;
each keeps its spelling until it is.

- The field work is `(struct FieldWork *)gEventWork`,
  `WORK_SLOT(HEAP_SLOT_EVENT)`, or `Runtime_AllocateBlock(HEAP_SLOT_EVENT,
  sizeof(struct FieldWork))`, which returns the existing block.
- The effect scene is `gEffectWork` or `*(gWorkSlot + HEAP_SLOT_EFFECT)`.
- The map's view is `gMapWork[0]` or `WORK_SLOT(HEAP_SLOT_MAP)`.
- The field blend work is `gFieldBlendWork[0]`,
  `WORK_SLOT(HEAP_SLOT_FIELD_BLEND)` (`ValeInteriorsInn_Initialize`), or
  `Runtime_AllocateBlock(HEAP_SLOT_FIELD_BLEND, sizeof(struct
  FieldBlendWork))`, which returns the existing block.
- The colour buffer is `gPaletteBufferWork`,
  `WORK_SLOT(HEAP_SLOT_PALETTE_BUFFER)` (`TretPalette_UpdateSceneTint`,
  `SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue`), or
  `*(gWorkSlot + HEAP_SLOT_PALETTE_BUFFER)`
  (`FieldEffect_FadeToEffectColor`). It's also reached from a
  neighbouring slot, as in Menus below: as
  `gFieldBlendWork[HEAP_SLOT_PALETTE_BUFFER - HEAP_SLOT_FIELD_BLEND]`
  (`FieldEffect_UpdateLightning`), or as
  `workCells[HEAP_SLOT_PALETTE_BUFFER - HEAP_SLOT_EVENT]` through
  `(u32 *)&gEventWork` (`Shop_SaveAndTintFieldPalette`), which is undefined
  behaviour with `gPaletteBufferWork` behind `UBFIX`.

One more spelling in the field objects has been measured:
`((16 - blend) << 8) | blend` in
[`object/psynergy_effects/world_map.c`](object/psynergy_effects/world_map.c)
is `BLEND_ALPHA(blend, 16 - blend)` with its operands swapped, and the macro
changes the registers.

### Scenes

Scene code reaches straight into actors and their tables, so many of its sites
come down to how a struct's member is addressed.

- `FixedPoint_Distance` takes the two positions as byte addresses, which its
  callers take from whole actors. Its byte cursor steps across x, y and z, so
  it reads the actors' own signed coordinate members without treating them as
  an array or copying them into one.
- Some helpers in Colosso and in `SceneId_KolimaForestFloodgate`, where logs
  roll, are called before they're declared. With their exact
  `void` prototype in scope, the arguments are set up in another order, so the
  matching build keeps the call ahead of the declaration, and their unit
  headers give the correct declarations under `UBFIX`.
- A field is read or written through its offset,
  `*(T *)((u8 *)p + offsetof(struct FieldActor, field))`, where `p->field`
  goes through FieldActor's single/group union and moves a load
  ([`map/common/effect_spawn.h`](map/common/effect_spawn.h),
  [`map/common/mercury_lighthouse_pillar.h`](map/common/mercury_lighthouse_pillar.h),
  [`map/crossbone_isle_stepping_stones.c`](map/crossbone_isle_stepping_stones.c),
  and `Imil_MiaSaysFarewellToApprentices` in [`map/imil.c`](map/imil.c)).
- An actor is read as `struct FieldEffect`, union FieldObject's effect view of
  the same object, whose sprite pointer is a plain member. Through
  FieldActor's sprite union, the flags address moves after the next store
  (`SolSanctum_StartFloatingLights` in
  [`map/common/sol_sanctum_elemental_star_ring_setup.h`](map/common/sol_sanctum_elemental_star_ring_setup.h)).
- Field stores are byte-addressed with `sizeof` and `offsetof` rather than
  made through the struct members in `Colosso_RestoreWarrior`,
  `TolbiShipInteriors_SelectPlacements`, and for row 10's talk facing in
  `Imil_GetPlacements`, where `script[10].talkFacing` adds the row's 240 first
  and stores at 22 from another base. These byte views are valid, and they
  keep the original's address bases and registers.
- The same byte view in `Xian_SpawnTrainingParticles` keeps the actor's Z load
  ahead of the options-script store. Through the members, the load moves after
  the store and the constant load that follows it in the English build.
- A global's member is reached through a local pointer to the global or
  through the global's own name, each where it matches. `Event_MovePillar`
  ([`map/crossbone_isle.c`](map/crossbone_isle.c)) takes `&state->scene`
  with `state = &gGameState`, because `&gGameState.scene` folds into one
  literal (1 of 78 instructions differ in tbs-en, 1 of 65 in tbs-ja). McCoy
  Palace's `Scene_GetPlacements` writes the reward rows as
  `gMcCoyPalacePlacements[6].talkFacing` and on, because the local
  `placements[6].talkFacing` splits the address into base+144 and member+22
  (9 of 38 in tbs-en).
- A table of `s16` pairs is read a word per index through a struct with an
  array member, `((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading]`,
  because an `s32` pointer to the table loses the original's loads
  (`VaultCave_PushPillarAhead`); `UBFIX` builds the word from its halfwords.
- `gStagedActor_DirectionSteps`, `gStagedActor_FootprintKinds` and
  `gStagedActor_FootprintBounds`
  ([`map/lunpa_fortress.c`](map/lunpa_fortress.c)) are put in read-only
  data with `__attribute__((section(".rodata")))` instead of `const`. Their
  shared externs in `field.h` and `common/object_staged_actor.h` lack `const`,
  so `const` would conflict, and without the attribute they'd go to `.data`.
- The Colosso race work (`HEAP_SLOT_COLOSSO_RACE`) is `gColossoWork`
  (`Colosso_DrawGauge`, `Colosso_UpdatePathOpponent`, `Colosso_StartOpponent`,
  `ColosseumNavampaMatch_CollectFinishEquipment`, `ColosseumAzartMatch_CollectFinishItem`) or
  `WORK_SLOT(HEAP_SLOT_COLOSSO_RACE)`, whose literal pool entry is
  `gWorkSlot+236` (`Colosso_SetGaugeHold`,
  `ColosseumSatrageMatch_ApproachFinishAndTakeItem`, `ColosseumSatrageMatch_StartOpponentPath`). It's
  the same address through another relocation, so no instruction differs.
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
- A command of `event.h` and the service it names are interchangeable; each
  site keeps the one it matches with.

### Windows

The window code keeps its state in a heap block of its own and writes
straight into tilemaps, so most of these are about reaching that block and
its cells.

- The work block is `UI_WORK` or `gWindowWork[0]`, and a neighbouring heap
  block is `gWindowWork[n]`.
- A tilemap cell is `work->tilemap + y * 32 + x`, or a byte offset added to
  `(u32)work->tilemap`.
- The save summaries are read through a signed byte cursor from
  `summary[0].party[4]`, indexed by `i * sizeof(struct SaveSummary) +
  SUMMARY_DISTANCE(party[4], field)`, for `summary[i].sendFlag` and
  `.flagCount` (`SaveState_ScanRecordFlags` in
  [`window/battle_command/save.c`](window/battle_command/save.c)), which keeps
  the literal `0x1070` and the offsets 1 and 2.
- A selection record's kind is cleared through a view from its base field,
  `((struct GlyphCursor *)&work->records[n].base)->cursor = 0`, for
  `work->records[n].kind = 0` (`UiGlyph_ResetWorkState`, records 7 and 8).
- A render output is read as `struct SpriteOutput`, whose sprite cell is the
  member `attr`, rather than as `struct RenderOutput` with a cast of `unk10`
  (`UiWindow_PutGlyph`, `UiWork_AnimateSpriteSlots`).
- A menu cursor whose screens write only the output pointer is a
  `union MenuCursor` rather than a struct of the same size, which keeps the
  store order (`OptionMenu_Open`).
- `gRenderResource_PairSourceTable` goes by three more extern names, given as
  `__asm__` labels, one per switch case, because the ROM keeps a literal for
  each case (`RenderResource_LoadTableEntry`).
- The Spanish and Italian possessive copy counts its letters in the
  function's `s32` value, compared as `(u32)value < 16`, rather than in `i` or
  `j` (`UiText_BuildRenderEntries`).
- In the Japanese small font, `nextRow = row + 1` is in both arms of
  `if (bits == 0)` rather than once ahead of the test (`UiText_DrawGlyphArm`
  in [`window/glyph_arm.c`](window/glyph_arm.c)).
- `gUiText_MarkGlyphCodes`, the Japanese kana marks, is declared without
  `const` and put in read-only data with `__attribute__((section(".rodata")))`
  ([`window/articles.c`](window/articles.c)), as Lunpa's staged-actor tables
  are. Through a `const` extern, its load in `UiText_RenderGlyphPair` is
  scheduled ahead of the spill of `out` (2 of 185 instructions in tbs-ja).

### Menus

Two spellings recur in the menus, one for copying a party member's record and
one for reaching a heap block.

- A unit is copied with the resident IWRAM routine as `Iwram_CopyWords`,
  through `_call_via_r3(..., IwramCopyWords)` (`Owner_Copy`), or through
  `Item_CopyWords` (tagged).
- A neighbouring heap block is reached from the menu control slot as
  `cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL]`.
- The menu work is read through one struct per screen
  (`InventoryMenuState`, `PsynergyMenuState`, `CharacterMenuState`,
  `DjinnMenuWork`, `FourObjectMotionState`), as the field object's views
  are, because each is what its own screen's code matches with.

### Battles

The battle engine's few spellings are a store, its background's blend
arithmetic and its work slots.

- An action slot's first halfword is cleared through a cast,
  `*(u16 *)&member->actionSlots[i] = 0`, because the field store loads its
  zero differently ([`battle/battle/debug.c`](battle/battle/debug.c)).
- The background's blend steps are `BLEND_ALPHA(16, 16) - alphaStep` and
  `step + BLEND_ALPHA(0, 16)`, where the macro's `|` form changes the code
  ([`battle/battle/background.c`](battle/battle/background.c)).
- Work slots are reached through `(void **)&gCameraWork` and
  `(void **)&gBattleWork` indexed past them (see Everywhere).

### Battle effects

The effects lean hard on their heap slots, and some of them on arithmetic and
macros whose exact shape decides the code.

- A work slot is read as `WORK_SLOT(n)` or `gWorkSlot[n]`, through a base
  pointer as `slots[n - base]`, `EFFECT_SLOT` or `CANVAS_SLOT`, and when it
  holds a blitter (a drawing routine `BattleEffect_BuildBlitter` writes into a heap
  slot) as `EFFECT_BLITTERS[n]` or `(DrawRectangle)gWorkSlot[n]`.
  `BattleEffect_RunParticleStreams` reads the background and blitter slots through
  one base pointer, where two absolute reads make the function 16 bytes longer
  in tbs-en.
  `BattleEffect_RunIce` reads its two blitters one each way (both cast, 2 of 586
  instructions differ; both through `EFFECT_BLITTERS`, 3).
- A signed halving is written out as its expansion, `x = p->x; x += (u32)x >>
  31; x >>= 1;`, which lets the final shift follow later arithmetic (the
  centre of the Forcible Arm and Truncheon Fist flame in
  `BattleEffect_RunCastingImpact`). As `p->x / 2` in each arm, 57 of 3070
  instructions differ; as `x /= 2`, 6.
- A statement macro takes the shape that all its uses match with. In
  `BattleEffect_BuildBlitter`'s second walk
  ([`effect/parts/blitter.c`](effect/parts/blitter.c)), `PUT`, `COPY` and
  `PATCH` are `do { } while (0)` blocks, `COPY_FROM` and `PUT_BRANCH` plain
  statements and `COPY_TWICE` a plain `{ }` block. As plain statements, `PUT`,
  `COPY` and `PATCH` change 18, 6 and 1 of 737 instructions in tbs-en.

### Game rules

The rules code reads party members and element tables in two ways each.

- A unit is `Unit_Get(owner)`, or from other banks `Unit_GetFar(owner)`, the
  far call that reaches it.
- An element pair array is `s16 [4][2]` where the matching code indexes it,
  or goes through `struct BattleElementStats` with `UBFIX`.

### Sound

The sound driver is compiled with pret's `old_agbcc` rather than alchemy-gcc.

- The driver's command table is called through a pointer read from the
  table's entry (`*(&gSound_CommandTable[n])`); indexing it directly adds an
  instruction with `old_agbcc`.
- `engine.c` keeps the labelled jumps its matching forms need.
