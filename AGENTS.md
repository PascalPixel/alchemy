This file is not for humans.

# Alchemy

For decompilation work (matching, measuring, humanizing the source, writing its docs), use Alchemy Devtools: install it by cloning https://github.com/PascalPixel/alchemy-devtools into this checkout's `alchemy-devtools/` folder, which git ignores, and running `cargo build --release` there. Its `AGENTS.md` says how to proceed and holds the rules for that work.

This file instructs the agents that help people build with Alchemy: change the game, add to it, read it. People read `README.md`. The agents keep this file current, wording included. Rule letters are kept, so old citations resolve; the rules not here are in the devtools' `AGENTS.md`. Alchemy rebuilds Golden Sun: **The Broken Seal** (☀️, build `tbs`) and **The Lost Age** (⚓️, `tla`) byte for byte from readable C, assembly and editable assets, in Japanese, English, German, Spanish, French and Italian: twelve editions, `tbs-ja` to `tla-it`. Japanese releases are the source editions; localizations are measured differences. Both games build byte for byte from this repository.

## Where things are

- `games/<game>/src`, `include`, `sound`, `text`: each game owns its copy of what it uses. ☀️'s code sits in one folder per bank, grouped by the credits' divisions, with a scene's overlay one file; `games/tbs/src/README.md` maps the folders, and module headers follow the same paths under `include/module`.
- Each game links from one linker script, `games/<game>/ld_script.ld`, run through the C preprocessor with the edition defined as its C is: it lists every object of the main image in link order, and includes `games/<game>/src/system/overlay/overlay.ld`, one block per overlay, when the build defines `OVERLAY`. An overlay's globals still written in assembly are `scene/bss/<name>.s` in both games. Neither game has a `recon/`.
- An overlay reaches a main-image routine `X` through the far-call export `XFar` (`src/gamepak/far_call/*.S`) and its own veneer to it, named `X` (⚓️'s `src/scene/import/<name>.s`, ☀️'s `imports.inc`); a placeholder veneer is renamed once its target is named.
- The docs start at `games/README.md` and `games/tbs/README.md`; a folder's README is its page, and `games/tbs/glossary.md` defines the words. The site, <https://alchemy.pascalpixel.com/>, is built from them in `alchemy-docs`.
- The `Makefile` builds everything; `tools/` holds its small C programs (`ags-*`). `out/` and `tools/out/` are build output.

## Commands

```sh
make                   # tbs-en, the compilers and tools first, once (make bootstrap: those alone)
make compare-tla-de    # one edition (tbs or tla, then ja en de es fr it) against rom.sha1; make tla-de builds it
make -j8 compare-tbs   # the six ☀️ editions; compare-tla for ⚓️'s
make compare-editions  # all twelve
make modern            # both games' C with a current GCC (MODERN_CC): no errors
make test              # the build tools' tests, the format and device checks
make SHIFT=4 tbs-en    # the shift test: everything after the header moves 4 bytes, into out/shift-4/
```

Setup is in `README.md`, and `make help` lists the rest. `make compare-editions` prints a lot: send it to a log file and count the `: OK` lines. After switching branches, delete `out/*/deps.mk` and `out/*/objects.mk`; after renaming sources, delete the editions' `out/` folders; after switching to a branch whose tools differ, `rm -rf tools/out/obj tools/out/ags-* && make tools`.

## Your game or this repository

A change in your own game may move bytes freely: it no longer matches `rom.sha1`, and `compare` says so, as it should. Please don't share built ROMs, original or modified (L1). A change sent back to this repository keeps every edition byte-identical (W5): `make compare-editions` prints OK for all twelve, `make modern` and `make test` pass. Where such a change moves bytes, it is reshaped in source until it does not, measured with the devtools' `diff` as their `AGENTS.md` describes. Ask Pascal first for any compiler, option or routing change, or for deleting what cannot be restored.

## Marks in the source

Marks left by the reconstruction, whose rules are in the devtools' `AGENTS.md`; a change keeps them true:
- `FAKEMATCH(<kind>)`: a form kept only so the old compiler emits the original's code, with what the plain form costs (S2).
- `LEFTOVER(<kind>)`: Camelot's own code that does nothing in this edition but still shapes its bytes (S2).
- `// DEAD:`: code whose instructions the ROM keeps and that cannot change the result (S2, S3).
- `// BUG:` and `BUGFIX`, `// UB:` and `UBFIX`: the original's bug or undefined behaviour, kept, with its fix behind the option (S3).
- `VIEW(<part>)`: a part of a joined object that sees a name otherwise than the rest; unfinished.
- `XXX:`: an open question at the code, with the evidence that would settle it (H4).
- `★ Named from ...`: the evidence a name was taken from (H3).

## Legal

- **L1** Never commit, upload or publish a ROM, private inputs or any copy of the game files. The cartridge logo lives only in `ags-header`, as in pret's gbafix.
- **L2** Evidence is our own ROMs, this repository and public documentation; never another project's Golden Sun work.
- **L3** Never use, open, read or mention leaked Nintendo or Camelot code or SDKs, or anything derived from them.

## Source

- **S3** Source is readable modern C in English, as pret writes its games:
  - Layout: lowercase short file names, the same folders in both games, code in its bank's folder, a function in the module of its linker neighbours. An object over about 3,000 lines holding several families may be written as parts in `<object>/` with a private `<object>_internal.h`, included in ROM order; a scene's overlay stays one file.
  - Language: C89 with the compilers' GNU extensions; types, signedness and scopes from the game's code; data read through structs and enums rather than offsets and casts wherever a match allows.
  - Naming: functions `Module_PascalCase`, globals `gName`, statics `sName`, constants and macros `UPPER_SNAKE_CASE` (a macro that stands for a call is named as a function, as the plain `Actor_SetPosition` over `Actor_SetPositionFixed` is), locals and fields `camelCase`; words spelled out except the abbreviations a programmer reads without stopping; an acronym is a word in mixed case and capitals in constants, as GBATEK writes them. `wait()` and `px()` are the lowercase primitives. The m4a driver keeps its own spellings. An unknown stays a placeholder (`sub_08012345`, `unk_14`).
  - Vocabulary: one name per concept, and each word in one sense, as `glossary.md` defines them. An event calls a command by its name in `event.h` (`Actor_FaceDirection`, `Event_ShowMessage`); the main-image routine's own name is written in a scene only where that site compiles differently through the inline. A word comes from, in order: the game's text (only for what the player sees), Camelot's own provenance (its labels, credits, staff interviews), the Sega lineage, the period tradition, plain English; never another decompilation's. The settled words are the glossary's; renaming one needs Pascal. Retired: tick, monitor, program (for a sequence), common, engine, library (for this code), job, main loop, kernel (for the main routine), task manager, metatile, text printer, scanline effect, sprite anim, round, battle animation, song (for every sound), record (in the save sense), interior (for a room), object (for an actor), and ⚓️'s place.
  - Numbers stay numbers until evidence gives them a meaning; then they have a name (an enum or macro in a shared header), and neither code nor comments use the number again. A number is decimal, or composed (`240 * 160`, `px(4) / 5` when exact, `TRIG_TURN / 8`, `DMA_COPY_WORDS(n)`, `RGB(r, g, b)`; a fraction of a pixel quantity, a position, speed, gravity or distance, is `px(n) / d`, and `Q16_ONE * n / d` is only for a unitless factor, a scale, decay, bounce or ratio); hex is kept only where the hex is the point: flag numbers (Camelot's debug flag editor shows them in hex), addresses, register values and bits, masks and packed fields, layout offsets, values from `0x80000000` up, all-ones markers and one-less bounds, raw stored bytes and the m4a driver's stock tables, and 16.16 values with no exact fraction (Pascal, 30 October 2026). A call or table row takes plain pixels and plain decimal speeds (`Actor_SetPosition(ACTOR_MIA, 880, 502)`, `Actor_SetSpeed(ACTOR_STATUE, 0.8)`), converted to 16.16 inside its definition by `Q16()`, which multiplies so that a negative is never shifted. A live 16.16 value goes through its `...Fixed` form (`Actor_SetPositionFixed`); a macro that reads each argument once may take live plain pixels (`Actor_SetPosition(actorId, x, z)`), and the macros that read one twice (`Actor_SetSpeed`, `FieldCamera_SetSpeed`, `FieldCamera_MoveTo`, `MapGrid_SetShake`, `ShipRooms_MoveCameraAndWait`) take only constants. `px()` turns whole pixels, a constant or a live value (`px(leader->x.part.pixel)`), into 16.16 only in arithmetic, comparisons and assignments on a stored 16.16 value (Pascal, 12 November 2026). Flags and sounds come from `flags.h` and `sounds.h`; a scene's temporary flag is named beside its includes.
  - Original behaviour: undefined behaviour keeps its form behind `UBFIX`; a bug keeps its code with `// BUG:` and a fix behind `BUGFIX`; `// DEAD:` marks code whose own instructions the ROM keeps and that cannot change the result (S2), and says what the ROM keeps: `// DEAD: <what>; the ROM keeps <its instructions>.` Unreachable things open their comment with `Unused:`; a function nothing refers to is `UNUSED`.
  - Editions: C and the linker scripts compare `edition.h` and `version.h` values; only plain assembly and `.S` files use edition names. `LANGUAGE` chooses only what language decides: text, message ids, fonts and glyph widths, and layout that fits the text. Every other difference between editions (a fix, a changed rule, value, check, timing or table) is a patch level from `version.h` (`TBS_PATCH_n`, `TLA_PATCH_n`), combined when no single level selects its editions (German alone is `PATCH_2 && !PATCH_3`) (Pascal, 7 November 2026). A value that differs by edition is one named macro chosen at the top of its file.
  - Macros: a macro that makes more than one call is a comma expression; wrapped in `do { } while (0)` it compiled differently in every ☀️ edition (10 November 2026).
  - Formatting: clang-format's LLVM style (`.clang-format`); include order is left alone; header guards are `ALCHEMY_<NAME>_H`.

## Assets

- **A1** The build is as plain as pret's: C, assembly, indexed PNGs with real palettes, WAV, MIDI, PO text, one `map.txt` per map, and TSV only for a table an encoder packs; never raw dumps, blobs or stored compression tokens (A7 excepted). Each asset builds beside its source, ignored by git.
- **A2** A picture is drawn the way the game shows it: sheets at their real width or sprite shape, tilemapped pictures as the map shows them, animations one frame wide.
- **A3** An all-grey palette only where the build writes that palette from it.
- **A4** Every file an encoder reads is a prerequisite: one Makefile rule per kind of asset, with the encoder listing what else it read in a `.d` file beside its output.
- **A5** No JSON, apart from `.recomp.json` and `.recomp-2.json` at the root, one per game, which recomp.fyi reads and nothing here does (Pascal, 5 November 2026). A table the code reads as it stands is a C array at its place in the ROM, read through its struct with designated initializers and a comment naming each entry from the game's text.
- **A6** Each edition combines its text, fonts and layout with its measured code differences; differing resources are chosen in their owning module or rule.

## Work

- **W1** A plain Makefile whose every rule can be read; a build tool is a small tested C program; Rust only in `alchemy-devtools`; no TypeScript, JavaScript, Python or shell beyond a command line; tooling nothing needs is removed.
- **W3** Follow pret where it settles a question, and never be stricter.
- **W5** Before every commit, `make compare-editions` prints OK for all twelve editions. Builds are incremental, as pret's are; CI builds every push from a clean clone.
