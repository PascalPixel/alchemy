# Contributing

Issues and pull requests are welcome. If something is wrong, badly named or hard to find, [open an issue](https://github.com/PascalPixel/alchemy/issues); if you know the fix, send it.

Things you can help with:

- [Solve a fake match](#solving-a-fake-match)
- [Give something a better name](#names)
- [Make code easier to read](#code)
- [Write or fix a comment](#comments), or answer one of the open questions
- [Improve the guide](#the-guide)
- [Fix a picture, a table or the build](#pictures-sound-text-and-tables)

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

Assembly lives in one header a game, `include/system/asm.h`, a macro to each instruction sequence; no other file has an `asm` with anything in it, and a second name for a symbol is a label where the symbol is defined.

Don’t replace a fake match with a worse kind. A local the ROM keeps in one named register is declared with `ASM_REGISTER`, and only where the routine matches with nothing else; a bare `register int x asm("r4")` is not accepted.

## Where the System comes from

Nobody outside Camelot has seen their source. This is what the ROMs show when they are read beside [SGL’s manual](https://www.infochunk.com/saturn/segahtml/sgl/sglr/) and [GBATEK](https://problemkaputt.de/gbatek.htm), and it is why the assembly is kept the way it is. Each line is something you can check in the tree.

The System is one library, carried from the Saturn to the Game Boy Advance by someone who wrote assembly as readily as C. It has three layers:

1. **Routines written in assembly**, resident in IWRAM: the multiply, the divide, the square root, the matrix product, the point transform, the fills and the decompressor. They are `.s` files and stay so.
2. **A handful of macros** for the operations too small to be worth a call. These are the `ASM_` macros of `include/system/asm.h`.
3. **C**, which is everything else.

**Its shape is SGL’s.**

- Numbers are 16.16 `FIXED`, angles are 65,536 to the turn, and a matrix is twelve words: a 3×3 rotation, then the translation.
- `Matrix_RotateX`, `Matrix_RotateY` and `Matrix_RotateZ` write the same four cells, with the same signs, as the matrices SGL prints for `slRotX`, `slRotY` and `slRotZ`.
- The fixed-point divide takes its divisor first and returns b / a, which is how SGL documents `slDivFX`.
- One caller of the arctangent was compiled against a prototype that returns a halfword. `slAtan` returns an `ANGLE`, 16 bits.
- The unit matrix is stored two ways, at a pointer and at the current matrix. `slUnitMatrix` takes a pointer, or `CURRENT`.
- `Dma_Set` takes source, destination, count and a fourth word, as `slDMAXCopy` does, and the loops that wait on a channel’s busy bit are `slDMAWait`.
- The debug text prints strings and hex numbers at a place on the screen, as SGL’s `slPrint` and `slPrintHex` do.

**Its hardware is the Game Boy Advance’s, taken raw.**

- Camelot’s own code makes four BIOS calls: Halt, Stop, CpuSet and SoundBias (GBATEK’s SWI 02h, 03h, 0Bh and 19h). It never calls the BIOS to divide, take a square root or an arctangent, set an affine matrix, decompress, or wait for VBlank: the library has a routine for each.
- The compiler’s own division, `__divsi3`, `__udivsi3` and `__modsi3`, is the library’s routines in IWRAM under those names. libgcc’s are not linked.
- A DMA channel’s three registers are written as three words with one instruction.

**It has its own calling rule.** Every routine written in assembly saves r5 and up and uses r0 to r4 freely: of the 138 register saves in that assembly, outside the sound driver, none keeps r4 for a caller. The interrupt handler, which must keep whatever a routine may destroy, saves r0 to r4. The Thumb C is compiled with `-fcall-used-r4`, which tells the compiler the same thing, and the unit matrix macro destroys exactly r0 to r4. So a macro behaves as a call to the library would: arguments in the registers a call passes them in, and nothing destroyed that a call would keep. The one exception is the four-word copy, which needs a fifth register and lists r5.

**Three compilers built it.** The Thumb C is GCC’s snapshot of July 2000, the one Red Hat shipped as 2.96. The C compiled as ARM is GCC 2.95.2. Only the sound driver and the save code, which are Nintendo’s libraries, were built by Nintendo’s compiler.

**It was still being worked on between the games.** ☀️ reaches the multiply through a macro that returns through `ip`, so that the caller keeps `lr`. ⚓️ has neither the macro nor that entry point: its compiler calls any routine through a register with `mov lr, rN` and the second half of a `bl`, four bytes and no helper, which no released GCC does. The trick moved from a header into the compiler.

What this predicts, and where it stands:

| It predicts | What we find |
| --- | --- |
| A macro is text, so two versions can live in one game | ⚓️’s battle code stores the unit matrix in ☀️’s instruction order while its System uses a newer order |
| The form the library uses on its own matrix may differ from the one it gives callers | The current-matrix store is the same text with r3 left off its list, and needs no other device |
| Lines compiled out leave statements that emit nothing | `ASM_NOTHING` after a menu sprite’s two stores; the cartridge also keeps debug rooms, the debug text and a CPU load display |
| An empty asm left in C stands for something the library’s author wrote plainly | `Matrix_BuildLookAt`, SGL’s `slLookAt`, had three; they were the addresses of the multiply, the divide and the dot product, each kept in a named register. Six others are open |

When you meet assembly that fits none of this, say so in the pull request: it is either a fourth layer or a hole in the theory.

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

Camelot made Shining Force III for the Saturn before these games, and a lot of how this code does its maths seems to come from [SGL](https://www.infochunk.com/saturn/segahtml/sgl/sglr/), Sega’s library for that console: 16.16 fixed-point numbers, angles of 65,536 to the turn, a current matrix you push and pop. So where the design matches, the names follow SGL’s reference, as `toFIXED`, `DEGtoANG`, `Matrix_RotateX` and the `FX` suffix do.

Music takes the title of its track on the official soundtrack where we could match the two (`MUS_TO_THE_OPEN_SEA`), and otherwise the place or moment it plays for (`MUS_PROX`). A sound effect is named for what it sounds like and a picture for what it shows, with a number at the end only for a real series, like `digit_0` to `digit_9`.

## Code

The source is C89 with the old compilers’ GNU extensions, written the way [pret](https://github.com/pret) writes its games.

- Read data through structs and enums, wherever the build still matches, and keep offsets and casts out.
- A number gets a name once you know what it means. After that, neither code nor comments use the bare number.
- Numbers are decimal. Hex stays where a programmer of the time wrote hex: flag numbers, hardware addresses, register bits and masks.
- Positions and speeds are plain pixels: `Actor_SetPosition(ACTOR_MIA, 880, 502)`. Fixed-point values are written `toFIXED(0.3)` and angles `DEGtoANG(45)`, after Sega’s SGL.
- What differs by language is chosen with `LANGUAGE`: text, fonts and layout that fits the text. Every other difference between editions is a patch level, `TBS_PATCH_2` or `TLA_PATCH_1`.
- A macro that makes more than one call is a comma expression. Wrapped in `do { } while (0)` it compiled differently in every edition.
- Formatting is clang-format 20 with the repository’s `.clang-format`. `make test` checks it with `clang-format-20` (earlier versions format some casts differently).

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

## What to work from

Your own ROMs, this repository and public documentation. No other project’s Golden Sun work and no leaked Nintendo or Camelot code. Don’t commit, attach or share a ROM, original or modified.
