# Contributing

Issues and pull requests are welcome. If something is wrong, badly named or hard to find, [open an issue](https://github.com/PascalPixel/alchemy/issues); if you know the fix, send it.

Things you can help with:

- [Solve a fake match](#solving-a-fake-match)
- [Give something a better name](#names)
- [Make code easier to read](#code)
- [Write or fix a comment](#comments), or answer one of the open questions
- [Improve the guide](#the-guide)
- [Fix a picture, a table or the build](#pictures-sound-text-and-tables)

## What to work from

Your own ROMs, this repository and public documentation. No other project’s Golden Sun work and no leaked code or development kits. Don’t commit, attach or share a ROM, original or modified.

## Before you send a pull request

Set up the build as the [README](README.md#build) says. Then:

```bash
make             # all twelve editions, each checked against the original ROM
make test        # the tools' tests, the formatting and a check of every file
```

Alchemy is a matching decompilation, so `make` has to print OK twelve times. `make test` tells you which file is wrong and why. CI runs both on every push.

More commands, for when you need them:

```bash
make compare-tla-de   # one edition: tbs or tla, then ja en de es fr it
make modern           # the C through a current GCC (needs arm-none-eabi-gcc)
make test-linking     # adds a scene, a map and a resource, and checks that every name still links
make help             # the rest
```

If a build acts strangely after you switch branches or rename a file, delete `out/*/deps.mk` and `out/*/objects.mk` and build again.

## Code

The source is C89 with the old compilers’ GNU extensions. It is written in the manner of [SGL](https://www.infochunk.com/saturn/segahtml/sgl/sglr/), Sega’s library for the Saturn. Camelot made Shining Force III for the Saturn before these games, and the system library is modelled on SGL, so the whole source takes SGL’s names and layout.

- Read data through structs and enums, wherever the build still matches, and keep offsets and casts out.
- A number gets a name once you know what it means. After that, neither code nor comments use the bare number.
- Numbers are decimal. Hex stays where a programmer of the time wrote hex: flag numbers, hardware addresses, register bits and masks.
- The integer types are SGL’s: `Uint8`, `Sint8`, `Uint16`, `Sint16`, `Uint32` and `Sint32`. The 32-bit pair is `int`, as the original was compiled, where SGL’s is `long`. A truth value is a `Bool`, `TRUE` or `FALSE`; a switch is `ON` or `OFF`, and a result that only says whether something worked is `OK` or `NG`. The development kit’s code keeps its own `u8` to `s32`.
- Positions and speeds are plain pixels: `fdActorSetPos(ACTOR_MIA, 880, 502)`. Fixed-point values are written `toFIXED(0.3)` and angles `DEGtoANG(45)`, as in SGL. One is `toFIXED(1)`. `INTtoFIXED` is left only where `toFIXED` compiles to other instructions.
- Every difference between editions that is not the language itself is a feature with a name that says what it does, such as `LUCKY_DICE_HELP_AFTER_LOSS`. Each game’s `include/edition.h` turns on each edition’s features in one place and says what every feature does. The code asks for a feature by name: `#if FEATURE` for a block, `WHEN(FEATURE, on, off)` for a single value. The assembly reads the few features it needs from `include/edition.inc`, set the same way.
- What the language decides, text, fonts and the layout that fits the text, is picked with `JAPANESE_OR(ja, others)`, `GERMAN_OR(de, others)` and the other `<LANGUAGE>_OR` pickers, or `BY_LANGUAGE(ja, en, de, es, fr, it)`. Each expands to the edition’s value (`include/features.h`, the same in both games). A block that only one language has is tested with `LANGUAGE`, as in `#if LANGUAGE == LANGUAGE_GERMAN`.
- A macro that makes more than one call is a comma expression. Wrapped in `do { } while(0)` it compiled differently in every edition.

## Names

A function’s name is two small letters for the part that owns it, then its words run together with a capital for each: `fdActorGetById`, `btEffectRunCastingImpact`. The owner is the folder the function is defined in.

| Prefix | Owner                                          |
| ------ | ---------------------------------------------- |
| `sy`   | the system library: `system/`, `sound/`        |
| `bt`   | the battle engine: `battle/`, `battle_effect/` |
| `fd`   | the field engine: `field/`                     |
| `mn`   | the menu engine: `window/`, `menu/`, `shop/`   |
| `gm`   | the game logic: `game/`                        |
| `sc`   | a scene: `scene/`                              |
| `db`   | the debug scenes: `debug/`                     |

A function a scene’s shared files define for that scene is the scene’s, `sc`. A routine the system library keeps resident in IWRAM is `syIWRAM` and its words: `syIWRAMDivFX`.

The SDK’s functions, the BIOS calls and the ids the assembler builds keep their own names.

Where SGL has the same call, the function takes SGL’s name with `sy` for SGL’s `sl`: `sySetEvent`, `sySynch`, `syPushMatrix`, `syRotX`, `syMulFX`, `sySin`, `syDMAXCopy`, `syIntFunction`, `syPrint`. [The system library’s page](games/tbs/src/system/README.md#the-calls) lists each one beside SGL’s. A variant keeps SGL’s stem and adds a word, as SGL’s `slPutPolygonS` does: `sySquart` and `sySquartIWRAM`, `syUnitMatrix` and `syUnitMatrixCurrent`, `syLookAt` and `syLookAtLoad`. A comment that cites SGL writes SGL’s own name: `slDMAXCopy`.

An abbreviation of hardware or of a format stays in capitals inside a name, as SGL writes `slDMAWait` and `slBGMOn`: `syVRAMCacheLoad`, `syIRQLock`, `syOAMAddSprAtDepth`, `syIOWriteQueueStore16`, `syHeapAllocEWRAM`, `syDecompressByteLZ`. Game words such as `Hp` and `Pp` stay words. A stub table’s entry is its target’s name followed by `Stub`: `sySquartFXStub`.

Words are spelled out, apart from SGL’s short forms:

| Word                         | Function | Type   | Field                      |
| ---------------------------- | -------- | ------ | -------------------------- |
| initialize, initial          | `Init`   |        | `init`                     |
| display                      | `Disp`   |        |                            |
| scroll screen                | `Scr`    | `SCR`  | `scr`                      |
| color                        | `Col`    | `COL`  | `col`                      |
| sprite                       | `Spr`    | `SPR`  | `spr`                      |
| sound                        | `Snd`    | `SND`  | `snd`                      |
| position                     | `Pos`    | `POS`  | `pos`                      |
| number                       | `Num`    | `NUM`  | `num`                      |
| calculate                    | `Calc`   |        |                            |
| attribute                    |          | `ATTR` |                            |
| parameters                   |          | `PARM` | `prm`                      |
| status                       |          | `STAT` | `stat`                     |
| animation, index, count      |          |        | `anm`, `idx`, `cnt`        |
| flags, frame, timer, pointer |          |        | `flg`, `frm`, `tmr`, `ptr` |
| address, offset, buffer      |          |        | `adr`, `ofs`, `buf`        |
| table, message, window       |          |        | `tbl`, `msg`, `win`        |
| target, source, destination  |          |        | `tgt`, `src`, `dst`        |
| previous, current, velocity  |          |        | `prev`, `cur`, `vel`       |
| angle, priority, callback    |          |        | `ang`, `pri`, `func`       |

`Scr` is a background layer, as SGL’s scroll screen; the whole display is written out, `Screen`: `syScreenFadeToBlack`. A function that does nothing is a `NoOperation`, as SGL’s `slNoOperation`: `btEffectNoOperation`.

The rest of each name follows these rules:

| What                 | How it’s written                                                                                   |
| -------------------- | -------------------------------------------------------------------------------------------------- |
| Global variables     | the name alone, `HeapSlots`, `FrameCount`; `Work` added where the name is taken, `BattleStateWork` |
| Statics              | `sName`                                                                                            |
| Types                | capitals, `ACTOR`, `BATTLE_EFFECT_PARM`, written `typedef struct ACTOR { ... } ACTOR;`             |
| Struct fields        | `camelCase` with the short forms: `sprId`, `anmIdx`, `velX`                                        |
| Locals               | `camelCase`                                                                                        |
| Constants and macros | `UPPER_SNAKE_CASE`, a macro that is a statement too: `DMA_FILL16`                                  |
| Files and folders    | lowercase, short, the same in both games                                                           |

A type keeps its tag beside its capital name. A forward declaration is `struct ACTOR;`, and a use is written `struct ACTOR *` where the typedef is not in sight. The old compilers refuse a typedef repeated in two headers. Two globals keep an old `g`, `gWorldMapIodem` and `gWorldMapBlend`, because the linker orders them by a hash of their names.

Pick a word in this order: the game’s own English text, for anything the player sees; then the term a Game Boy Advance programmer already uses (for hardware, [GBATEK](https://problemkaputt.de/gbatek.htm)’s name); then plain English. Use one word for one thing everywhere. The glossaries in [`docs/tbs/glossary.md`](docs/tbs/glossary.md) and [`docs/tla/glossary.md`](docs/tla/glossary.md) hold the words already settled, each with its sources, and [say where the style comes from](docs/tbs/glossary.md#the-style-and-where-it-comes-from).

Music takes the title of its track on the official soundtrack where we could match the two (`MUS_TO_THE_OPEN_SEA`), and otherwise the place or moment it plays for (`MUS_PROX`). A sound effect is named for what it sounds like and a picture for what it shows, with a number at the end only for a real series, like `digit_0` to `digit_9`.

## Layout

The layout is that of SGL’s sample code:

- One tab for each level, four columns wide.
- A function’s opening brace on a line of its own, and `else` after the closing brace: `} else {`.
- A line continued from the one above is one tab further in.
- No space between `if`, `for`, `while` or `switch` and its parenthesis: `if(count > 0)`.
- Lines up to 80 columns.

Formatting is clang-format 20 with the repository’s [`.clang-format`](.clang-format), which says all of this. `make test` checks it with `clang-format-20` (earlier versions format some casts differently).

## Comments

A comment says what the code does for the game. How the compiler came to emit it, registers and instruction counts included, stays out.

Every comment is a block comment, `/* ... */`, never `//`. A comment of more than one line is one comment, not a stack of them: its later lines start three columns in, under its text, and it closes at the end of its last line. A blank line inside it starts a new paragraph. A file opens with one such comment that says what the file is. A comment after code on the same line is lined up with the ones around it; one too long to sit beside its code goes on its own lines above it.

These marks are used in the source. Keep them true when you change the code around them.

| Mark                                    | Meaning                                                                         |
| --------------------------------------- | ------------------------------------------------------------------------------- |
| `/* BUG: */` and `BUGFIX`               | The original’s bug, kept, with its fix behind a `FIX_` flag `BUGFIX` turns on   |
| `/* UB: */` and `UBFIX`                 | The same for undefined behaviour, with a `FIX_` flag `UBFIX` turns on           |
| `/* DEAD: */`                           | Code the ROM keeps that can’t change the result, with what the ROM keeps        |
| `Unused:` and `UNUSED`                  | Something nothing reaches, and a function nothing refers to                     |
| `LEFTOVER(frame)`, `LEFTOVER(template)` | Camelot’s own code that does nothing in this edition but still shapes its bytes |
| `VIEW(part)`                            | One part of a file sees a name differently from the rest; unfinished            |
| `XXX:`                                  | An open question, with what would settle it                                     |

Answering an `XXX:` is a good first contribution: `grep -rn "XXX:" games`.

## Headers

The system library has one header, `include/system.h`. A file that uses the system library includes it and nothing else of the system library’s, as a Saturn program includes SGL’s `sgl.h`. It takes its types and constants from `system/definitions.h` and its variables from `system/globals.h`.

Each module’s main header declares each function of the module’s stub table once, in the table’s order, with the definition’s signature. No other file declares them again. Three of The Lost Age’s modules keep that block in a header beside the main one: `field_functions.h`, `field_mode_functions.h` and `game_functions.h`. A C file includes only the headers its code needs.

## Solving a fake match

A fake match is code bent into an odd shape so the old compiler produces the original’s instructions. Every one is marked with a comment that says what it is and what the plain code would be:

```c
/* FAKEMATCH(temporary): the bits held in a local; as *flags |= bits. */
```

Find them with `grep -rn "FAKEMATCH(" games`. The kinds, worst first:

| Kind        | What it is                                              |
| ----------- | ------------------------------------------------------- |
| `signature` | a type that isn’t the real one                          |
| `volatile`  | `volatile` on something no hardware or interrupt shares |
| `asm`       | any other `asm`: instructions, a label, a register name |
| `barrier`   | an empty `asm`                                          |
| `wrapper`   | a call layer with no purpose of its own                 |
| `temporary` | a local that only stands for another value              |
| `dead`      | a statement that changes nothing                        |

To solve one, write the plain form and build that edition with `make compare-tla-en`. If it still prints OK, remove the mark and run `make` for all twelve. If it doesn’t, try other ways of writing the same thing: the order of declarations, the type of a local, an `if` turned around.

Don’t replace a fake match with a worse kind.

## Assembly

Camelot wrote a little of these games in assembly, and the source keeps it in three places and nowhere else.

- A function written in assembly is a `.s` file.
- A sequence of instructions that C can’t say is one `ASM_` macro in the game’s `include/system/asm.h`, used by name. No other file has an `asm` with anything in it. Each macro is still marked as a fake match.
- A second name for a symbol is a label where the symbol is defined, or a line of the link script.

A local the ROM keeps in one named register is declared with `ASM_REGISTER`, and only where the function then matches with nothing else. A bare `register int x asm("r4")` is not accepted.

The reason for all this is on [the system library’s page](games/tbs/src/system/README.md#where-the-system-library-comes-from): the system library has its own rule for sharing registers, and the macros are its smallest pieces. If you meet assembly that fits none of it, say so in the pull request.

## The guide

Each folder has a `README.md` that explains what’s in it, and GitHub shows it under the folder’s files. The longer pages are in [`docs/`](docs). [alchemy.pascalpixel.com](https://alchemy.pascalpixel.com) is built from both.

When you change how something works, change the page that describes it in the same pull request.

## Pictures, sound, text and tables

- A picture is an indexed PNG with its real palette, drawn the way the game shows it: sheets at their real width, animations one frame wide.
- Sound is WAV and MIDI. Text is in the `.po` files under `text/`.
- A table is a `.tsv`. Its first line names the columns, one row is one thing, and numbers are decimal.
- A table the game’s code reads is a C array where it can be, and rows under a small assembler macro where it holds a scene or a resource. Those rows name what they mean, and the linker fills in the number:

  ```
  encounter_rule SolSanctumCorridor, -1, FLAG_SOL_SANCTUM_ERUPTED, 1, 1
  ```

- No raw dumps and no blobs. Two things exist only so the original ROMs come out byte for byte, and they don’t grow: the `*_patch.tsv` files, and the `left` columns that hold bytes Camelot’s packer left behind.
- The Makefile stays plain enough to read. A build tool is a small C program in `tools/` with tests in `tools/tests`, and it knows a file format, never this game.
