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
thing can lead it to different choices. The original was compiled from one
particular way of writing each line, so where the plain spelling gives
different instructions, the matching C keeps the one that gives the
original's.

Each idea has one preferred spelling. Where a site matches only with a plain-C
equivalent (an index or a pointer form, a macro or its expansion, a cast), that
site keeps the equivalent, untagged, with a short `//` note beside it saying
what the plain spelling would change. The patterns below are the ones that
recur, each with one example; the sites themselves carry the rest. They use the
engine's words: a [heap slot](../glossary.md#heap-slot) is one of the numbered
words of `gHeapSlots` that hold a system's working block, and an `ObjectEvent`
is the struct an [object event](../glossary.md#object-event) is read through.

### Indexes, pointers and casts

- `p[n]` and `*(p + n)` can compile differently, and so can `&p[n]` and
  `&*(p + n)`. `TilesetAnim_Update` writes `(gridState->animations +
  channelIndex)->`, where the index form changes 78 of 119 instructions in
  tbs-en.
- A member can be reached through a pointer to its own type, or to its own
  size with the other signedness (`*(T *)&p->member`, or `*&p->member`),
  instead of as `p->member`. These are valid aliases, kept where the member
  form would move the access or merge it with a neighbour's: in
  `Battle_RollEffectSuccess` each status test written as `unit->field` merges
  with its neighbour's into one halfword or word load.
- A field can be read or written by its byte offset,
  `*(T *)((u8 *)p + offsetof(struct ObjectEvent, field))`, or a run of fields
  through a byte cursor indexed with `sizeof` and `offsetof`. These views are
  valid and keep the original's address bases: `Colosso_RestoreWarrior`
  stores its two bytes that way, because a struct store at a varying address
  may pass a load the ROM keeps after it. Through `ObjectEvent`'s
  single/group union, the plain member form usually moves a load. A
  pointer can likewise be stepped as an integer: a window's tilemap cell is
  `work->tilemap + y * 32 + x` or a byte offset added to `(u32)work->tilemap`.
- A function that walks three coordinates in a row takes a byte address and
  steps across them, so it works on any struct whose x, y and z sit together
  without treating them as an array (`Vector_AddPolarOffset`).
- A value can be reached through a view of another type. That is undefined
  behaviour, marked `// UB:`, with the declared access behind `UBFIX`:
  `WindowSprite_AppendToList` stores the new tail through a
  `struct WindowSprite **` view of the list's tail link, which keeps both
  stores in order.
- A table read by columns is a `[][N]` array, as alchemy-gcc addresses a
  struct's later fields from the table's base: an element pair table is
  `s16 [4][2]` where the matching code indexes it, with
  `struct BattleElementStats` behind `UBFIX`.
- Palette RAM and VRAM are written through either a volatile pointer or a
  plain one. Palette RAM is either `BG_PLTT_COLOR`, which is volatile, or a
  plain pointer to it such as `(u16 *)&BG_PLTT_COLOR(14, 0)`.
  `BattleAnim_RunJudgment`'s brightening ramp keeps a `volatile u16 *`,
  because with a plain pointer the mask that picks out red is hoisted out of
  the loop.
- A cast to a narrower type can be part of the spelling: `(u16)~(...)` makes
  the halfword mask the ROM holds (`MapGrid_SetShownLayers`).

### Views of one struct

Some memory is read through a struct of its own, a field-for-field copy of
the layout that differs only where its reader does, because that reader's
code matches only through it. [An object](../glossary.md#object)'s 112 bytes
are `struct Object` almost everywhere, but `BattleObject_SpawnObjectsForList`
reads them as `BattleSpriteObject`, with the sprite word untyped; `update.c`,
ARM code built with GCC 2.95.2, which cannot read `Object`'s anonymous
unions, keeps `ScriptMotionObject`; and the operand handlers of `SC_SET`,
`SC_ADD` and `SC_CMP` keep `ScriptOperands`. The window sprites
(`struct WindowSpriteCellView`), the selection records (`struct GlyphCursor`), the
menu cursors (`union MenuCursorRef`) and each menu screen's work
(`ItemMenuWork`, `SummaryScreenWork` and the others) follow the same rule. A
field's own type is likewise the one every site matches with: `Object.sprite`
is a plain `struct Sprite *`, because as a union or a `void *` a store or a
load moves.

### Macros and their expansions

- A macro and its expansion can compile differently. `SerialRuntime_Initialize`
  writes `IRQ_LOCK` out as its steps, with the IME pointer set inside the
  save; as `IRQ_LOCK`, 1 of 123 instructions differs. Other sites keep
  arithmetic on `BLEND_ALPHA` rather than its `|` form, its operands written
  out, or a signed halving as its three steps.
- A statement macro takes the shape all its uses match with. In
  `BattleAnim_BuildBlitter`'s second walk, `PUT`, `COPY` and `PATCH` are
  `do { } while (0)` blocks and the others plain statements or a block.
- A register write is either `Io_Write16(value, &REG_X)`, which builds the
  value in a word first and keeps the store where it's written, or plain
  `REG_X = value` (`WorldMap_InitializePerspective` uses both).
- A DMA control word is written with its flags above its count:
  `(DMA_ENABLE | DMA_32BIT) << 16 | words`, or `DMA_ENABLE << 16 | halfwords`.
- A message that follows a named one is written as `MsgName + n`, such as
  `MsgSaveDataCorrupted + 2` for `MsgSaveRecoveryFailed`, where the ROM adds to
  the register already holding the first.

### Statements and variables

- A random value is masked in place, with the mask assigned first
  (`speed = 0x3ff; speed &= Random16();`). Written as `Random16() & 0x3ff`,
  the mask goes into a scratch register and the result is copied before the
  `and`, so 3 instructions differ at each site (`LuckyWheels_Run` and several
  battle animations).
- Where a statement sits and which variable holds a value decide the
  schedule and the registers: `Glyph_DrawArm` writes `nextRow = row + 1` in
  both arms of a test rather than once ahead of it, and `Message_Decode`
  counts letters in its `s32` value rather than in `i` or `j`.
- A global's member is reached through a local pointer to the global or
  through the global's own name, each where it matches: Crossbone Isle's
  `EventScript_MovePillar` takes `&state->mapId` with `state = &gSaveBlock`,
  because `&gSaveBlock.mapId` folds into one literal.
- A loop can be written with `goto`. A C loop's notes let GCC's loop pass
  (`loop.c` in its source) move constants and addresses out of the loop,
  strength-reduce its counters and delete it when it is empty, and they let
  its jump pass (`jump.c`) copy its exit test to its top; register allocation
  also counts the references inside a C loop as more frequent. A loop built
  from labels and `goto` gets none of this, so where the ROM shows none of it
  the loop stays a `goto`, and the comment beside it gives the measured cost
  of its loop form. The `goto`s without such a comment (the wander retries,
  the step searches, the map effects' [script](../glossary.md#script) loops and
  the debug loops) stay until someone finds their loops.
- A function whose result nothing reads keeps its `s32` return where `void`
  would change its epilogue, the instructions that end it; `UBFIX` gives the
  `void` form. An update hook that the bytes show returning a value does the
  same, with `// UB:` above it.

### Calls

- One routine can be reached by several routes, each kept where it matches: a
  game flag by `GameFlag_Get`, `GameFlag_Set` and `GameFlag_Clear` or by the
  script layer's inlines `Script_GetFlag`, `Script_SetFlag` and
  `Script_ClearFlag`; a `script.h` command or the service it names; a unit by
  `Unit_Get` or, from other [banks](../glossary.md#bank), the far call
  `Unit_GetFar`; a unit copy by `Iwram_CopyWords`, through `_call_via_r3`, or
  through `Item_CopyWords` (tagged). An inline call's argument is a pseudo of
  its own, so a flag number used twice is loaded twice; called directly, the
  number is kept in a callee-saved register (Bilibin's chef,
  `EventScript_TalkToBusyChef`).
- Some calls compile only against an old-style declaration or before any
  declaration: with the exact prototype in scope the arguments are set up in
  another order. Helpers in Colosso and in `MapId_KolimaForestFloodgate` are
  called ahead of their definitions, with the correct declarations in their
  unit headers under `UBFIX`, and `script_layer.h` gathers the engine routines
  the maps call that way (`Engine_GameFlagSet`).
- Code with no function type is called through a cast function pointer. Code
  copied to RAM is called through one too, or through `_call_via_r3`, the
  compiler library's call through r3, with the code's address as its fourth
  argument. `System_SoftReset` calls the untyped fixed address `Cart_Restart`
  through a local `s32 (*reset)(void)`. With no declared type to differ from,
  these casts are no `wrapper`.
- The sound driver, compiled with pret's `old_agbcc`, calls its command table
  through a pointer read from the entry, `*(&gMPlayJumpTable[n])`; indexing
  it directly adds an instruction. `engine.c` keeps the labelled jumps its
  matching forms need.

### Heap slots

The engine keeps each system's working state in a
[heap](../glossary.md#heap-and-pools) block whose pointer sits in a numbered
slot of `gHeapSlots`, and many of those slots also have a name of their own
(`gFieldWork`, `gPaletteFadeWork` and so on), so the same block can be reached
several ways: `HEAP_SLOT_BLOCK(n)`, `gHeapSlots[n]`, `*(gHeapSlots + n)`, the
slot's own name (`gWindowWork[0]`, `WINDOW_WORK`), from a neighbouring slot
(`gLightningWork[HEAP_SLOT_PALETTE_FADE - HEAP_SLOT_LIGHTNING]` in
`Weather_UpdateLightning`), through a base pointer (`slots[n - base]`,
`ANIM_SLOT`, `ANIM_BLITTERS[n]`), or with `AllocHeapSlotEwram`, which returns
the existing block. Indexing past a named pointer (`(void **)&gCameraWork`) is
undefined behaviour, with its defined form behind `UBFIX`. Not every site has
been measured against the other spellings; each keeps its spelling until it
is, and its note says so.

### Where data is placed

- A table whose shared extern lacks `const` is placed in read-only data with
  `__attribute__((section(".rodata")))` instead (Lunpa's
  `gPushedObject_DirectionSteps`, `gPushedObject_FootprintKinds` and
  `gPushedObject_FootprintBounds`); `gFont_MarkGlyphCodes` is placed the same
  way because a `const` extern changes the schedule of its reader.
- One table can go by several extern names, given as `__asm__` labels, where
  the ROM keeps a literal for each use (`RenderResource_LoadTableEntry`).
- A local array of constants compiles to an anonymous read-only constant
  copied into the frame, and a struct copy of a named table to the same code;
  which one a site keeps decides where the data lands
  (`ImilFuchinFalls_SpawnDust`).
