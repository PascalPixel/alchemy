This file is not for humans.

# Alchemy

It instructs the agents that work on Alchemy; people read `README.md` and `INSTALL.md`. The agents keep it current, wording included. Pascal decides compilers, what counts as DONE, and deleting anything that cannot be restored; everything else goes ahead without asking, including fixing a check that wrongly blocks honest work as long as it still refuses everything it refused before.

Alchemy rebuilds Golden Sun: **The Broken Seal** (☀️, build `tbs`) and **The Lost Age** (⚓️, `tla`) byte for byte from readable C, assembly and editable assets, in Japanese, English, German, Spanish, French and Italian: twelve editions, `tbs-ja` to `tla-it`. Japanese releases are the source editions; localizations are measured differences.

## Goal

- **DONE** is pret's 100%, never stricter: all six editions of a game build byte-identical from source, verifiable from `rom.sha1`; DONE code and data are both 100% (C2); no function is matched by an assembly body or inline asm beyond the proven `CAMELOT_ASM` helpers; nothing in the build reads a ROM, confirmed by an independent audit (O4); the C compiles with a current GCC (`make modern`); every steering device is tagged and counted. ☀️ has reached it; ⚓️ is still being matched.
- **Finished** means more: every asset ported (A1), `recon/<game>` gone, no FAKEMATCH device left, a limited set of stock compilers (K1 to K3), and every mechanism and scene humanized (H4).
- **Humanized** means a reader learns how the game works from its source: each mechanism, scene and event says what it is and why, under the name the game gives it, with its data read through types, and the docs are written from that source (H1).

## Where things are

- `games/<game>/src`, `include`, `sound`, `text`: each game owns its copy of what it uses. ☀️'s code sits in one folder per bank, grouped by the credits' divisions (System: `system`, `gamepak`; Field: `field`; Battle: `battle`, `battle_effect`; Interface: `window`, `menu`, `shop`), with `game`, `sound`, `graphics`, `map`, `scene` (one overlay per file) and `debug`. `games/tbs/src/README.md` maps them; module headers follow the same paths under `include/module`.
- `games/<game>/objects.tsv` lists every object in link order; `games/<game>/main.ld` is the one main script, chosen by `--defsym <GAME>_EDITION_<LANG>=1`.
- `recon/tla` holds ⚓️'s not-yet-C work: `raw/` listings, `<lang>/` scaffolds, `en/main/<address>.c` drafts. ☀️ has no `recon/`.
- The `Makefile` builds everything; `tools/` holds its small C programs (`ags-*`); the optional `alchemy-devtools` submodule holds the Rust matching aids, the renderer and the progress count. `out/` and `tools/out/` are build output.

## Commands

Setup is in `INSTALL.md`; ROMs go under `roms/` as `tla-ja.gba` and so on, checked against `rom.sha1`, never committed.

```sh
git submodule update --init --recursive alchemy-gcc
make bootstrap         # the tools, the pinned compilers, GAS 2.10 and libgcc
make compare-editions  # all twelve editions: required before every commit (W5)
make -j8 compare-tbs   # the six ☀️ editions; compare-tla-en for one edition
make modern            # ☀️'s C with a current GCC: no errors
make test              # the build tools' tests
git submodule update --init alchemy-devtools && make devtools  # optional, Rust
git config core.hooksPath .hooks   # send verified measurements when pushing main
make progress          # DONE code and data (C2)
make land              # on main, before a landing (W2, C3)
alchemy-devtools/target/release/alchemy-devtools diff DRAFT.c [ADDR] [--show|--heat|--rtl]
alchemy-devtools/target/release/alchemy-devtools permute DRAFT.c [ADDR] [--minutes N]
```

`make compare-editions` prints a lot: send it to a log file and count the `: OK` lines.

## Boundaries

Always: keep all twelve editions byte-identical (W5); tag every compiler-steering device (S2); name only from evidence (H3); commit each verified step, attempts included (S4); look at how pret does it first (W3).

Ask Pascal first: any compiler, option, routing or alchemy-gcc change (K1 to K3); anything that changes what counts (C2, W6); deleting what cannot be restored; pushing or publishing outside the landing Workflow 6 authorizes.

Never: commit or publish a ROM or game files (L1); use another project's Golden Sun work (L2), or leaked Nintendo or Camelot code or SDKs or anything derived from them (L3); let the ROM steer the build or count (O1); write a function as inline assembly or place data through `asm()` (S1); patch compiler output or give a function its own compiler option (S2, K1); treat old history or deleted files as evidence (O5); add JSON, Python, shell or TypeScript programs (A5, W1); commit a cache, a hand-kept registry, tool scaffolding or a background file (N1 to N6).

## Workflow

1. Work on a branch; `main` is the only long-lived branch. Work in the main checkout unless another session shares it, then in a worktree of your own (W2).
2. Match what other unfinished code calls first, then the largest with nothing left to name (W4).
3. Draft in `recon/<game>/en/main/<address>.c`; a near miss stays a draft with its remaining difference in its header (S4).
4. Adopt a match into the module of its linker neighbours, in every edition whose bytes agree, and commit (C1).
5. Humanize one mechanism or scene at a time, all the way through, each step byte-identical (H2).
6. Land with `make land` on the tree you push; Pascal authorizes syncing each verified batch to `origin/main` with its measurement (C3). Other pushes need his request.

## Rules

### Legal

- **L1** Never commit, upload or publish a ROM, private inputs or any copy of the game files. The cartridge logo lives only in `ags-header`, as in pret's gbafix.
- **L2** Evidence is our own ROMs, this repository and public documentation; never another project's Golden Sun work.
- **L3** Never use, open, read or mention leaked Nintendo or Camelot code or SDKs, or anything derived from them.

### Counting

- **C1** A function counts only when its whole extent, literal pool included, compiles exactly and every edition stays byte-identical; in an edition, only when that edition links its C.
- **C2** DONE is pret's calcrom over the linker maps, by folder: bytes placed from `games/` are done, from `recon/` not yet. Code is `.text`; data is everything else. A game's figure sums its six editions.
- **C3** A landing carries its measurement: `make land` compares, tests, checks formatting, builds `make modern` and measures the tree you push; the pre-push hook uploads the reports; CI rebuilds ☀️ from a clean clone without a ROM, checks a receipt against its own measurement, and starts the docs build. `.github/publication.md` describes publication and recovery.

### The oracle stays out of the build

The reference ROM judges the build and never feeds it. Reconstructing Camelot's compiler from habits across a whole game is allowed (K3); fitting a compiler, option or routing to particular functions is cheating.

- **O1** ROM bytes reach the build only through pret's `.incbin "baserom.gba"` scaffold in `recon/`, which never counts, and the logo `ags-header` writes. Nothing derived from the ROM steers the build or count.
- **O2** Every name is defined where its bytes are; layout is the object list's order. No equate, alias or table gives a name an address, except Camelot's own fixed addresses (`FIXED_ADDRESS`), each listed once. An assembler equate holds only what its table computes or a value its evidence names; a size set only so the bytes come out right is a copied byte (S1); `.set` only switches assembly in or out.
- **O3** Scene, map and resource ids come only from their tables, used whole; overlay code never branches straight into the main image.
- **O4** A mismatch is fixed in source, or the code stays a draft. Before a milestone, run an independent audit for leaks.
- **O5** Old git history, deleted files, backups and unreachable objects are never evidence.

### Humanizing

- **H1** A reader learns how the game works from its source; the bytes stay identical, nothing has to look like the original's source, and our names are never presented as recovered.
- **H2** The unit is one mechanism or one scene, taken all the way through in order: find out what it is (callers, data, the game's text, renders), type it, name it from that evidence, shape it (the game's own helpers, original behaviour kept, stale devices cleared), then explain it in comments and its page. Research comes before writing: a name or comment written before the thing is understood is redone. Work that needs no understanding (formatting, moving files) may run across the tree; names and comments may not.
- **H3** Names come from evidence: a scene from its area name (`gArea_NameRules`), a character from the game's text, an unnamed room from what the player sees there, checked against its render. The code's own words are a guess, never a name. Game terms are the English release's. A game thing's `///` comment carries its evidence in one line: `★ Named from the game: "..." (MsgName)`, `★ Named from its area name: ...` or `★ Named from what the player sees: ... (its render)`.
- **H4** A unit is done when no placeholder its evidence could settle remains, its data is read through types, every meaningful value has a name, the unreachable says so, and its comments and page meet D1 to D9. An open question is a `//` comment at the code: `XXX: <question>; <the evidence that would settle it>.` or `XXX (no evidence): <question>; <why nothing in reach settles it>.`, never on a page.
- **H5** `alchemy-devtools progress --humanity` is a worklist, not the definition; where a count asks for what these rules forbid, fix the count. A wrong name is fixed where it is; it becomes a rule only when it shows a principle these rules miss.

### Source

- **S1** `games/` holds only real source: C from an approved compiler, or proven assembly written as instructions, never copied bytes. Assembly is proven as the compiler library, as 8-byte far-call veneers, or as handwritten code with an instruction, frame or pattern alchemy-gcc cannot emit, its `@ Why` saying which: rrx; a branch, adc or sbc on a shift's carry; r11 as an ordinary register; a callee save without lr; CPSR/SPSR; swi; computed pc into code; stores into code; unpaired multiply halves; a pc-relative block load of data; a mode switch inside a function; movs with no shift; ldm or stm writing back off sp other than a block copy from r0 or a doubleword pair; a conditional branch on flags with no compare; one literal pool shared by two functions; or a linkage no procedure-call standard produces. Anything else stays in `recon/` until it matches as C. No C file places bytes through `asm()`.
- **S2** Compiler-steering devices count, as in pret, when each carries one tag inside its function: `/* FAKEMATCH(<kind>): the form kept; as <plain form>, N of M instructions differ in tbs-en. */`, N measured by a whole-function `alchemy-devtools diff`. The kinds, worst to mildest (Pascal, 29 October 2026), from what GCC does not support, through what is not C and C that misstates the program, to C its programmer might have written: `pin` (a fixed-register variable), `signature` (a wrong return or parameter type), `barrier` (an empty asm), `one-pass` (a once-only block or loop), `volatile`, `wrapper`, `temporary` (a forced temporary), `dead` (dead code); `games/tbs/src/MATCHING.md` defines each. In ☀️ a device is added only in place of a worse one, `goto` may only fall, and ☀️ takes no `pin` or `signature` (`make test` refuses either); ⚓️ may add tagged devices to reach a match. Replacing a device with milder ones is progress, however many it takes; sideways or up is not, unless the new form is the source's. A device comes only after reshaping has failed. `CAMELOT_ASM("proof")` marks the proven handwritten helpers inside C. A layer that explains a habit of the whole game (`event.h`, `iwram_call.h`, `system_call.h`) is source, not a device; a `do { } while (0)` macro is no device where every use matches through it. An unused local of a real type that emits nothing and only sizes the stack frame is no device, kept with a `// Unused: gives the stack frame its N bytes.` note; dead stores, a taken address, cancelling updates and anything that emits or reorders instructions stay `dead` (Pascal, 30 October 2026). A `volatile` global, or a type whose members are volatile, is the source's form where its rereads (a reload after its own store, a discarded read kept, a reread CSE would merge) or stores kept in order show in two or more unrelated functions, named in a `// volatile:` note at its declaration; rereads in one function stay a device (Pascal, 30 and 31 October 2026). Never patch compiler output.
- **S3** Source is readable modern C in English, as pret writes its games:
  - Layout: lowercase short file names, the same folders in both games, code in its bank's folder, a function in the module of its linker neighbours. An object over about 3,000 lines holding several families may be written as parts in `<object>/` with a private `<object>_internal.h`, included in ROM order; a scene's overlay stays one file.
  - Language: C89 with the compilers' GNU extensions; types, signedness and scopes from the game's code; data read through structs and enums rather than offsets and casts wherever a match allows.
  - Naming: functions `Module_PascalCase`, globals `gName`, statics `sName`, constants and macros `UPPER_SNAKE_CASE`, locals and fields `camelCase`; words spelled out except the abbreviations a programmer reads without stopping; an acronym is a word in mixed case and capitals in constants, as GBATEK writes them. `wait()` is the one lowercase primitive. The m4a driver keeps its own spellings. An unknown stays a placeholder (`sub_08012345`, `unk_14`).
  - Vocabulary: one name per concept, and each word in one sense, as `glossary.md` defines them. A word comes from, in order: the game's text (only for what the player sees), Camelot's own provenance (its labels, credits, staff interviews), the Sega lineage, the period tradition, plain English; never another decompilation's. The settled words are the glossary's; renaming one needs Pascal. Retired: tick, monitor, program (for a sequence), common, engine, library (for this code), job, main loop, kernel (for the main routine), task manager, metatile, text printer, scanline effect, sprite anim, round, battle animation, song (for every sound), record (in the save sense), interior (for a room), object (for an actor), and ⚓️'s place.
  - Numbers stay numbers until evidence gives them a meaning; then they have a name (an enum or macro in a shared header), and neither code nor comments use the number again. A number is decimal, or composed (`240 * 160`, `Q16_ONE * 4 / 5` when exact, `TRIG_TURN / 8`, `PIXELS(n)`, `DMA_COPY_WORDS(n)`, `RGB(r, g, b)`); hex is kept only where the hex is the point: flag numbers (Camelot's debug flag editor shows them in hex), addresses, register values and bits, masks and packed fields, layout offsets, values from `0x80000000` up, all-ones markers and one-less bounds, raw stored bytes and the m4a driver's stock tables, and 16.16 values with no exact fraction (Pascal, 30 October 2026). Flags and sounds come from `flags.h` and `sounds.h`; a scene's temporary flag is named beside its includes.
  - Original behaviour: undefined behaviour keeps its form behind `UBFIX`; a bug keeps its code with `// BUG:` and a fix behind `BUGFIX`; `// DEAD:` marks code the ROM keeps that cannot change the result. Unreachable things open their comment with `Unused:`; a function nothing refers to is `UNUSED`.
  - Editions: C compares `edition.h` and `version.h` values; only linker scripts, plain assembly and `.S` files use edition names. A value that differs by edition is one named macro chosen at the top of its file.
  - Spellings: where a site matches only with a plain-C equivalent spelling, it keeps it untagged with a short `//` note; `games/tbs/src/MATCHING.md` describes each recurring pattern once.
  - Formatting: clang-format's LLVM style (`.clang-format`); include order is left alone; header guards are `ALCHEMY_<NAME>_H`.
- **S4** Never throw work away: commit each adoption and attempt; a near miss stays a draft with its remaining difference in its header; an edition whose bytes differ stays on its scaffold and never blocks the others.

### Documentation

The docs are the folder READMEs, the four division pages, the front page, the glossary, MATCHING.md, the `///` comments and the site built from them (`alchemy-docs`). People read them; AGENTS.md is the only file written for agents. A unit's comments and page are written last, from what the earlier steps found.

- **D1** The docs explain how the game works and how its parts fit, a level above the code; not a modding manual, a wiki or a walkthrough. Everything on a page must help a reader understand some code.
- **D2** Every page is a chapter of a textbook, with the front page (`games/tbs/README.md`) as the model: it opens from what the player sees, names the few mechanisms it explains in order (a list, and a diagram where they nest or follow one another), gives each idea its own paragraph with its recording and, where it has a structure or an order, a small diagram, closes each mechanism with a table of its names and values in the player's units, and ends by following one moment of the game through its mechanisms with a table naming the mechanism behind each step. A folder guide then has `## Design`, `## How it fits with the rest of the game`, `## Limits`, `## Words used here` and, where it has any, `## Evidence`; a division page (`system.md`, `battle.md`, `field.md`, `interface.md`) is shorter, ends each section with a link to its folder guide, then `## Design` and `## Limits`. The glossary defines the words every page shares, each with one `Evidence:` line. A page is as long as its ideas need at that pace and no longer.
- **D3** Every place, person and event a page names is checked against the game's text and data and named as H3 names it; an internal number never stands in for a name.
- **D4** Pictures are the game itself: recordings and screenshots from `alchemy-devtools render` on a tbs-en build, published to the Blob store and referenced by their `https://alchemy.pascalpixel.com/` URL, with no text baked in. Every section about something the player sees opens with its recording; one about memory or timing has none. A recording that passes through a black screen shows at least ten seconds of the game before, between and after each black screen (`render check` refuses one that doesn't). Small Mermaid diagrams, in the front page's colours and with the concept emoji (🏡 the field, ⚔️ a battle, 🎲 Lucky Dice, 🎰 Lucky Wheels, 🚩 a flag, 🎵 music, 📣 a sound effect), show a structure or order no recording can. Never a hand-drawn picture or a data dump.
- **D5** Plain English a curious reader who has never seen the code can follow. Explain every idea before any sentence or diagram leans on it. Say what a thing is, the rule it keeps and why, never what it isn't. Function names are references, not the words a sentence is built from. No narration, mood, plot, fragments, process language or page describing itself; `you` is only the reader, the player is the player; headings name their subject or ask the reader's question.
- **D6** Say a thing once: what the project is, only in `games/README.md`; a sentence every site would repeat, once at the layer it describes. Before landing a docs batch, count the phrases it repeats across pages.
- **D7** A folder's README is its page; its `#` heading is unique. Link a glossary word at its first use, once. Where a word comes from is said only in the glossary's `Evidence:` lines and a folder's `## Evidence`, never in prose. Prose names the games, never ☀️ or ⚓️, and cites no rule of this file. A heading other pages link to is renamed only with those links. The site's settings live in `alchemy-docs`, never as markup in the source.
- **D8** A number is said by the name the code gives it and in the player's units; a number with no name gets one in the code first. Hex only as data a reader meets, in code formatting, as every symbol is. A cell behavior is said by what it does, never by its number.
- **D9** Comments say what the code cannot: what the player sees, units, why a forced form is forced, the evidence for a name. A comment the name or body already says is deleted. Declarations take `///` (a trailing `///<`), everything else `//`; a file opens with `/// @file`; no `@brief`; `@param` only where a parameter needs saying; the first sentence is the summary. FAKEMATCH tags, `// BUG:`, `// DEAD:` and comments inside functions stay `//`.

### Assets

- **A1** The build is as plain as pret's: C, assembly, indexed PNGs with real palettes, WAV, MIDI, PO text, one `map.txt` per map, and TSV only for a table an encoder packs; never raw dumps, blobs or stored compression tokens. Each asset builds beside its source, ignored by git.
- **A2** A picture is drawn the way the game shows it: sheets at their real width or sprite shape, tilemapped pictures as the map shows them, animations one frame wide.
- **A3** An all-grey palette only where the build writes that palette from it.
- **A4** Every file an encoder reads is a prerequisite: one Makefile rule per kind of asset, with the encoder listing what else it read in a `.d` file beside its output.
- **A5** No JSON. A table the code reads as it stands is a C array at its place in the ROM, read through its struct with designated initializers and a comment naming each entry from the game's text.
- **A6** Each edition combines its text, fonts and layout with its measured code differences; differing resources are chosen in their owning module or rule.

### Compilers

The set is frozen: three C generators built from the pinned `alchemy-gcc` submodule, no new compiler, option, route or alchemy-gcc commit, except a host fix that leaves every edition byte-identical. Matching shows practical compatibility, not Camelot's exact compiler.

- **K1** Game Thumb C: alchemy-gcc (GCC of 2000-07-31) with `-O2 -mthumb -nostdinc -fcall-used-r4`, plus `-mthumb-interwork` for ☀️ and K2's options for ⚓️; libgcc with `-O2 -mthumb -mcpu=arm7tdmi -fno-inline`. ARM C: GCC 2.95.2 with `-O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4`. Sound and flash: pret's `old_agbcc`. The Makefile's SOUND_C, FLASH_C and ARM_C are the only routes.
- **K2** ⚓️ adds `-mthumb-split-constants`, `-mthumb-call-via-lr`, `-mthumb-slow-loads` and `-mtune=arm9tdmi` for every Thumb file. Its base compiler is settled as GCC from before 6 September 2000, never 3.0.4 or 3.1; alchemy-gcc's README records the evidence. `-mthumb-slow-loads` is an approximation: a rule that replaces it must match more and break none of what matches. An instruction-order mismatch alone is no reason to change compilers.
- **K3** A compiler change needs one option set that explains most of a game's code, no public compiler of the time that already does, its evidence recorded here and in alchemy-gcc's README, and Pascal's approval.

### Work

- **W1** A plain Makefile whose every rule can be read; a build tool is a small tested C program; Rust only in `alchemy-devtools`; no TypeScript, JavaScript, Python or shell beyond a command line; tooling nothing needs is removed.
- **W2** Work on a branch; `main` is the only long-lived branch; land with `make land`.
- **W3** Follow pret where it settles a question, and never be stricter.
- **W4** Match first what other unfinished code calls, with the structures it shares.
- **W5** Before every commit, `make compare-editions` prints OK for all twelve editions; a tree that merges branches does so from a clean build.
- **W6** A change to what counts, a rule, an option or routing lands as its own commit with DONE before and after.
- **W7** Parallel work divides by owner; check a name is free (`git log --all -S`) before using it; commit only verified work.
- **W8** Humanizing follows H2 toward H4 and keeps every edition byte-identical.

### Nothing but the source

The repository is a few short C files, pictures, music and tiny tools, as pret's are; caches, registries and scaffolding that make it look bigger than it is drift from the code.

- **N1** A tracked file under `games/` is read by the build or is a guide's Markdown.
- **N2** No caches: what a script can produce from the source is produced by the build or the docs site, never committed.
- **N3** No registries kept by hand: a fact lives once, beside the thing it is about.
- **N4** No per-asset recipe files, stored offsets or sizes; one rule per kind of asset.
- **N5** No background files: notes and plans stay in the agent's scratch space.
- **N6** Volume is never progress: add no file, table, section or line to look thorough; every new file says what reads it.

## What matching has measured

Facts about alchemy-gcc, not rules.

- `p[n]` and `*(p + n)` can compile differently; each site keeps the spelling it matches with.
- A struct declared before its type is complete keeps a smaller alignment: a header includes its structs' headers first. alchemy-gcc pads every struct to 4 bytes, so a table read by columns is a `[][N]` array.
- Locals declared in an inner block take their pseudos later and change allocation. Allocation order is r3, r2, r1, r0, ip, lr, r4 to r7, r8, r10, r9, r11; global.c ranks pseudos by refs over live length; a constant `REG_EQUAL` note doubles a pseudo's live length.
- A loop built from labels and `goto` has no loop notes, so loop.c and jump.c leave it alone; an empty C loop is deleted.
- An empty asm counts 4 bytes when literal pools are placed and emits none; a one-pass block's loop-end note ends CSE's block and is a scheduling barrier; sched2 breaks ties by RTL order. CSE reuses a register holding 0 or 1.
- A file that tests `LANGUAGE` without including `edition.h` silently gets the Japanese branch.
- Within an object, code and data follow definition order; only the object list orders objects.
- Japanese is often an older layout of a function. A function that saves r9 and never uses it is a nested function storing its static chain.
- To measure a rewrite in seconds, compile its one object to assembly for one edition and diff it against the build's, `.L` numbers ignored; then `make compare-editions`.
- After switching branches, delete `out/*/deps.mk` and `out/*/objects.mk`; after renaming sources, delete the editions' `out/` folders; after switching to a branch whose tools differ, `rm -rf tools/out/obj tools/out/ags-* && make tools`.
