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

The source is C89 with the old compilers’ GNU extensions, written the way [pret](https://github.com/pret) writes its games.

- Read data through structs and enums, wherever the build still matches, and keep offsets and casts out.
- A number gets a name once you know what it means. After that, neither code nor comments use the bare number.
- Numbers are decimal. Hex stays where a programmer of the time wrote hex: flag numbers, hardware addresses, register bits and masks.
- Positions and speeds are plain pixels: `Actor_SetPosition(ACTOR_MIA, 880, 502)`. Fixed-point values are written `toFIXED(0.3)` and angles `DEGtoANG(45)`, after Sega’s SGL.
- What differs by language is chosen with `LANGUAGE`: text, fonts and layout that fits the text. Every other difference between editions is a patch level, `TBS_PATCH_2` or `TLA_PATCH_1`.
- A macro that makes more than one call is a comma expression. Wrapped in `do { } while (0)` it compiled differently in every edition.
- Formatting is clang-format 20 with the repository’s `.clang-format`. `make test` checks it with `clang-format-20` (earlier versions format some casts differently).

## Names

| What                     | How it’s written                         |
| ------------------------ | ---------------------------------------- |
| Functions                | `Module_PascalCase`                      |
| Globals and statics      | `gName`, `sName`                         |
| Constants and macros     | `UPPER_SNAKE_CASE`                       |
| Locals and struct fields | `camelCase`                              |
| Files and folders        | lowercase, short, the same in both games |

Spell words out, apart from abbreviations a programmer reads without stopping.

Pick a word in this order: the game’s own English text, for anything the player sees; then the term a Game Boy Advance programmer already uses (for hardware, [GBATEK](https://problemkaputt.de/gbatek.htm)’s name); then plain English. Use one word for one thing everywhere. The glossaries in [`docs/tbs/glossary.md`](docs/tbs/glossary.md) and [`docs/tla/glossary.md`](docs/tla/glossary.md) hold the words already settled, each with its sources.

Camelot made Shining Force III for the Saturn before these games, and the System does its maths the way [SGL](https://www.infochunk.com/saturn/segahtml/sgl/sglr/), Sega’s library for that console, does. Where the design matches, the names follow SGL’s reference, as `toFIXED`, `DEGtoANG`, `Matrix_RotateX` and the `FX` suffix do.

Music takes the title of its track on the official soundtrack where we could match the two (`MUS_TO_THE_OPEN_SEA`), and otherwise the place or moment it plays for (`MUS_PROX`). A sound effect is named for what it sounds like and a picture for what it shows, with a number at the end only for a real series, like `digit_0` to `digit_9`.

## Comments

A comment says what the code does for the game. How the compiler came to emit it, registers and instruction counts included, stays out.

These marks are used in the source. Keep them true when you change the code around them.

| Mark                                    | Meaning                                                                         |
| --------------------------------------- | ------------------------------------------------------------------------------- |
| `// BUG:` and `BUGFIX`                  | The original’s bug, kept, with its fix behind the `BUGFIX` option               |
| `// UB:` and `UBFIX`                    | The same for undefined behaviour                                                |
| `// DEAD:`                              | Code the ROM keeps that can’t change the result, with what the ROM keeps        |
| `Unused:` and `UNUSED`                  | Something nothing reaches, and a function nothing refers to                     |
| `LEFTOVER(frame)`, `LEFTOVER(template)` | Camelot’s own code that does nothing in this edition but still shapes its bytes |
| `VIEW(part)`                            | One part of a file sees a name differently from the rest; unfinished            |
| `XXX:`                                  | An open question, with what would settle it                                     |

Answering an `XXX:` is a good first contribution: `grep -rn "XXX:" games`.

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

- A routine written in assembly is a `.s` file.
- A sequence of instructions that C can’t say is one `ASM_` macro in the game’s `include/system/asm.h`, used by name. No other file has an `asm` with anything in it. Each macro is still marked as a fake match.
- A second name for a symbol is a label where the symbol is defined, or a line of the link script.

A local the ROM keeps in one named register is declared with `ASM_REGISTER`, and only where the routine then matches with nothing else. A bare `register int x asm("r4")` is not accepted.

The reason for all this is on [the System’s page](games/tbs/src/system/README.md#where-this-library-comes-from): the System is one library with its own rule for sharing registers, and the macros are its smallest pieces. If you meet assembly that fits none of it, say so in the pull request.

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
