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

The source is C89 with the old compilers’ GNU extensions, written in the style of [SGL](https://www.infochunk.com/saturn/segahtml/sgl/sglr/), Sega’s Saturn library, which Camelot’s own system library is modelled on.

- Read data through structs and enums, and give a number a name once you know what it means.
- Numbers are decimal, except where a programmer of the time wrote hex: flags, addresses, register bits and masks.
- Types are SGL’s: `Uint8` to `Sint32`, `Bool` with `TRUE` and `FALSE`, `ON` and `OFF`, `OK` and `NG`. Fixed-point values are `toFIXED(0.3)` and angles `DEGtoANG(45)`. Positions are plain pixels.
- A difference between editions has a name that says what it does, such as `LUCKY_DICE_HELP_AFTER_LOSS`, set per edition in the game’s `include/edition.h`. Ask for it with `#if FEATURE` or `WHEN(FEATURE, on, off)`. Text and layout that depend on the language use `JAPANESE_OR(ja, others)` and the other `<LANGUAGE>_OR` pickers, or `BY_LANGUAGE(ja, en, de, es, fr, it)`.

## Names

A function’s name is two letters for the part that owns it, then its words: `fdActorGetById`, `btEffectRunCastingImpact`.

| Prefix | Owner                                          |
| ------ | ---------------------------------------------- |
| `sy`   | the system library: `system/`, `sound/`        |
| `bt`   | the battle engine: `battle/`, `battle_effect/` |
| `fd`   | the field engine: `field/`                     |
| `mn`   | the menu engine: `window/`, `menu/`, `shop/`   |
| `gm`   | the game logic: `game/`                        |
| `sc`   | a scene: `scene/`                              |
| `db`   | the debug scenes: `debug/`                     |

Where SGL has the same call, the function takes SGL’s name: `sySetEvent`, `sySynch`, `syRotX`. [The system library’s page](games/tbs/src/system/README.md#the-calls) lists them.

Globals and tables are the name alone (`TickCount`, `Formations`), types are capitals (`ACTOR`), fields and locals are `camelCase`, and constants are `UPPER_SNAKE_CASE`.

Words are written out. The exceptions are the short forms modern C code commonly uses: `ptr`, `buf`, `src`, `dest`, `len`, `str`, `msg`, `init`, `min`, `max`, `num`, `tmp`, `err`, `ctx`, `args`, `info`, `pos`, `prev`, `addr`, `param` and `config`, and `i`, `j`, `k` for loop counters.

A tick is one step of the game’s loop; a frame is one drawing of the screen.

Pick a word from the game’s own English text first, then from what Game Boy Advance programmers already say, then plain English, and use one word for one thing. The glossaries in [`docs/tbs/glossary.md`](docs/tbs/glossary.md) and [`docs/tla/glossary.md`](docs/tla/glossary.md) hold the settled words.

## Layout and comments

Formatting is clang-format 20 with the repository’s [`.clang-format`](.clang-format), and `make test` checks it.

A comment says what the code does for the game, and is a block comment, `/* ... */`. These marks are used in the source:

| Mark                     | Meaning                                                     |
| ------------------------ | ----------------------------------------------------------- |
| `/* BUG: */`, `BUGFIX`   | The original’s bug, kept, with a fix behind a `FIX_` flag   |
| `/* UB: */`, `UBFIX`     | The same for undefined behaviour                            |
| `/* DEAD: */`            | Code the ROM keeps that can’t change the result             |
| `Unused:`, `UNUSED`      | Something nothing reaches                                   |
| `XXX:`                   | An open question, with what would settle it                 |

Answering an `XXX:` is a good first contribution: `grep -rn "XXX:" games`.

## Solving a fake match

A fake match is code bent into an odd shape so the old compiler produces the original’s instructions. Each one is marked with what the plain code would be:

```c
/* FAKEMATCH(temporary): the bits held in a local; as *flags |= bits. */
```

Find them with `grep -rn "FAKEMATCH(" games`. To solve one, write the plain form and run `make compare-tla-en`. If it still prints OK, remove the mark and run `make` for all twelve. If not, try other ways of writing the same thing: the order of declarations, the type of a local, an `if` turned around. Don’t replace a fake match with a worse kind (`signature` and `volatile` are the worst, `dead` the mildest).

Assembly lives in `.s` files and in the `ASM_` macros of `include/system/asm.h`, and nowhere else.

## The guide

Each folder has a `README.md` that explains what’s in it, and the longer pages are in [`docs/`](docs). [alchemy.pascalpixel.com](https://alchemy.pascalpixel.com) is built from both. When you change how something works, change the page that describes it in the same pull request.

## Pictures, sound, text and tables

- A picture is an indexed PNG with its real palette, drawn the way the game shows it.
- Sound is WAV and MIDI. Text is in the `.po` files under `text/`.
- A table is a `.tsv`: a header line, one row per thing, decimal numbers.
- No raw dumps and no blobs. A build tool is a small C program in `tools/` with tests, and it knows a file format, never this game.
