# Alchemy

Golden Sun: **The Broken Seal (TBS)** ☀️ and **The Lost Age (TLA)** ⚓️,
rebuilt byte for byte from readable C, assembly and editable assets, in all
twelve editions: each game in Japanese, English, German, Spanish, French and
Italian. Japanese releases are the source editions; localizations are measured
differences. Build IDs are `tbs` and `tla`, then the language: `tbs-ja`,
`tla-en`.

This file is the only place Alchemy's rules live. `README.md` is for fans.
Every rule has an ID, and a rule a tool can enforce names its check. Pascal
decides rule wording, compilers, what counts as DONE, and deleting anything
that cannot be restored; git history records when. Everything else goes ahead
without asking him, including fixing a check that wrongly blocks honest work,
as long as it still refuses everything it refused before.

## Goal

**DONE is a pair, code and data** (Pascal, 2026-10-08). **DONE code =
matching C + proven library, handwritten or veneer assembly**, over each
game's executable bytes; **DONE data = the non-code bytes each image places
from source**, over all its other placed bytes (C2); each is counted in all
six of a game's editions together. 100% means both: every language of a game
builds all of its code and all of its data from source. `make progress`
prints both figures on every line, one line per edition, shows code in its
parts (C, assembly and 8-byte stubs) and shows the bytes of FAKEMATCH-steered
code as their own number. ☀️ has the priority; the target is ☀️ 100% and ⚓️ 100%,
sharing as much code between the games as possible.

Matching is not the finish. The project is finished when the source also
forms a readable, coherent reconstruction suited to the games' hardware and
approved toolchains: The Broken Seal in 2001 and The Lost Age in 2002 (S3).
Names, comments and formatting are Alchemy's reconstruction choices. Work
need not reach that form at once, but every change moves toward it (W8), and
the last pass before both games are declared finished, alongside removing
FAKEMATCH (S2), camelotizes what remains.

A game is finished when all six hold (Pascal, 2026-10-08):

1. 100% DONE code and 100% DONE data in all six editions.
2. Every asset ported: nothing reaches the ROM as copied bytes; all data is
   editable source our tools build (A1).
3. `recon/<game>` removed: no scaffold, draft or edition scaffolding left.
4. FAKEMATCH at 0%: no tagged device left (S2).
5. A limited set of stock compilers builds all of it (K1–K3).
6. The codebase camelotized (S3), which includes its structure:
   - Files are Camelot's own translation units, recovered from the bytes as
     pret recovers Game Freak's (object alignment, each object's trailing
     data and strings, link order), so the file count follows the original,
     not one file per adopted function.
   - Every name rests on evidence: in-game text, the table or id that
     reaches it, or the code that loads it. No codename folders, guessed
     character names or invented asset names; an unknown keeps a neutral
     placeholder.
   - The global layers Camelot's code shows are source: one character
     module that relates each character's portrait, field sprite, battle
     sprite, name and dialogue through its id; likewise scenes, areas and
     other units the code treats as one, with reusable modules, inline
     helpers and macros where the code repeats.
   - Assets are named the same way, never with invented names: each
     character's field sprite, battle sprite and portrait files and their
     resource directory rows take the character's name through that
     module (for example ROBIN, ROBIN_KO), other sprites and pictures take
     names from in-game text or the code that loads them, and anything
     unproven keeps its number.

   Order (Pascal, 2026-10-08): first 100% by pret's standard (W3), then this
   structure as the first order of camelotizing.

   Camelotizing runs in stages, each byte-identical in all twelve editions
   (`alchemy keep`) (Pascal, 2026-10-08):
   1. Mechanical: formatting, dead files and code (`alchemy dead`),
      includes and prototypes, the modern build's type warnings, stale
      FAKEMATCH devices (`alchemy sweep`) and undefined behaviour behind a
      `BUGFIX` switch for the modern build.
   2. Structure: Camelot's translation units, repeated code folded into
      `games/common` helpers, macros and inline layers, and the global
      layers above.
   3. Meaning: names and comments from evidence, assets named through
      their modules; linters flag what remains, and an unknown keeps its
      placeholder.

## Where things are

- `games/<game>/src`, `include`, `sound`, `text` and `games/common` hold the
  source; `<game>` is `tbs` or `tla`. `games/<game>/main.ld` is the Japanese
  object order.
- `recon/<game>/raw` holds the English disassembly of code that is not C yet,
  one listing per function or run. ☀️'s `games/tbs/main_en.ld`
  is the English object order; German, Spanish, French and Italian follow it
  through their own `main_de.ld` ... beside it. ⚓️'s edition scripts are
  still `recon/tla/<lang>/main.ld`.
- ☀️'s overlays link by `games/tbs/src/system/overlay/ovl_<id>.ld`,
  with the overlay's own data and bss not yet in its modules in
  `ovl_<id>.s` beside it; `recon/tbs/raw/overlays` keeps the original
  listings as reference only. ⚓️'s are `recon/tla/raw/overlays`.
- `games/<game>/src/system/rom_head.s` is the cartridge header, linked
  first in every edition, as pret's `rom_header.s`: the logo is
  `.space 156` and the complement 0. Right after objcopy the build runs
  the header fix-up (`alchemy gbafix`, pret's gbafix) on every ROM it
  links, which writes the logo and the complement (L1).
- `games/tbs/include/event.h` is the scene command vocabulary
  (S3); `field_event.h` and `field_service.h` hold the script layer the
  commands expand to.
- `recon/<game>/<lang>` holds that edition's ROM scaffold (`rom.s`,
  `overlays.s`) and its drafts (`main/`, `overlays/`).
- `tools/` holds alchemy (one `alchemy` command for both games: build,
  checks, the count, matching, camelotizing and reports; see Commands) and
  ags (asset encoders and decoders). `out/` and `tools/out/` are build
  output and are ignored.
- `.github/workflows/progress.yml` publishes progress after each push to main:
  the `progress` branch holds `history.tsv` and README's two figures, written
  only by CI. `make figure` draws the same two figures locally from
  `history.tsv` beside them, a private copy of that history.

## Rules

### Legal

- **L1** Never commit, upload or publish a ROM, private inputs or any other
  copy of the original game files. The cartridge header's Nintendo logo lives
  only in the build's header fix-up tool, which writes it into each built ROM
  as pret's gbafix does (Pascal, 2026-10-08). _Check: publication._
- **L2** Evidence comes only from our own ROMs, this repository and public
  documentation. Never use another project's Golden Sun work, and never open
  one to compare.
- **L3** Never use leaked Nintendo or Camelot code or SDKs, and never open, read
  or mention sources that might contain them.

### Counting

- **C1** A function counts only when its complete extent, literal pool
  included, compiles exactly and every edition stays byte-identical. A
  function counts in an edition only when that edition's build links its C.
  _Check: compare, compare-tla, compare-other-editions._
- **C2** DONE code is pret's calcrom over the linker maps of byte-identical builds:
  the code the linker places from `games/`, counted once for each of the
  game's six editions whose build links it, out of six; sizes are the English
  build's, so code that exists only in another edition is outside the total,
  and code an edition's own source leaves out, with no symbol in any of its
  images, is outside that edition's total.
  Uncredited disassembly is `not-yet-c` in `recon/<game>/raw`; only proven
  library, handwritten and veneer assembly counts as assembly. Assembly
  counts as handwritten only with an instruction, frame or pattern Camelot's
  compilers cannot emit (rrx; a branch, adc or sbc on a shift's carry; r11 as
  an ordinary register; a callee-saved register save without lr; CPSR/SPSR;
  swi; computed pc into code; stores into code; unpaired multiply halves; a
  pc-relative block load of data; a mode switch inside a function; movs with
  no shift, where their copy-and-test is subs #0; ldm or stm writing back
  off sp, other than a block copy's list from r0 or a doubleword pair) or a
  linkage no procedure-call standard produces; anything else is not-yet-C
  until it matches as C. Whole aligned 8-byte far-call stubs count as veneers.
  Padding a source marks as carrying no credit does not count as code. It
  is outside the code total, as the linker's fill is, and is data; a span
  may hold only padding, data and nop.
  DONE data (Pascal, 2026-10-08) is every other byte each image places, the
  ROM and each overlay, from its first placed byte to its last: data, assets,
  headers and fill. Each overlay counts once, in its unpacked image as
  linked; its compressed stream in the ROM is outside the total. A byte is
  done when an object built from `games/` places it (data, assets, tagged
  packer bytes) or the build writes it as fill or alignment; a byte placed
  from `recon/` (incbin scaffolds, not-yet-C listings and their overlay
  counterparts, drafts) is not. Uncredited padding counts as data, compiler
  library and linker bytes count as done, and each edition's data total is
  its own images', not the English build's; a game's figure sums its six.
  Cartridge padding after the last placed byte is outside the total. A game is
  100% only when DONE code and DONE data are both 100% in all six editions
  (Pascal, 2026-10-08).
  _Check: coverage-report, publication._
- **C3** Every push to main carries its measurement: `make land` compares all
  twelve editions and measures both games, and the pre-push hook uploads that
  count by commit. CI records it on the `progress` branch, draws README's two
  figures there and sends decomp.dev its report. Model credit comes from
  main's commit trailers; days through 2026-10-04 keep their approved counts.
  _Check: land, pre-push._

### The oracle stays out of the build

The reference ROM judges the build and never feeds it. Any path by which the
expected answer steers the build or the count is cheating, whatever its form.
Reconstructing Camelot's compiler from habits that run through a whole game is
allowed (K3); fitting a compiler, an option or a routing to particular files
or functions is exactly this cheating.

- **O1** ROM bytes reach the build only through pret's
  `.incbin "baserom.gba", OFFSET, SIZE` scaffold in `recon/<game>`, which never
  counts, and the cartridge logo the header fix-up writes (L1). Nothing else
  derived from the ROM or the expected output (bytes, addresses, sizes,
  tokens, tables, hashes) may steer the build or the count, `out/` included;
  reports under `out/` are for people only.
  _Check: publication._
- **O2** Every name is defined where its bytes are, as a C definition or a
  label, and layout is the linker scripts' object order. No equate, alias,
  `#define` or table gives a name an address, except Camelot's own fixed
  addresses: resident IWRAM routines called through fixed entries, RAM
  buffers its code addressed as constants, and the hardware cartridge restart
  entry, each listed once in a game header and checked against where the
  linker placed it. One name per place; never a
  name that spells its own value. _Check: publication, fixed-address check, aliases._
- **O3** Scene and resource ids come only from their tables and are used whole.
  Overlay code never branches straight into the main image.
  _Check: the build's id gate._
- **O4** A mismatch is fixed in source, or the code stays disassembly or a
  draft. Before announcing a milestone, run an independent audit for leaks.
- **O5** Old git history, deleted files, backups and unreachable git objects
  are never evidence. Re-derive an answer from our ROMs and the current tree,
  or ask Pascal.

### Source

- **S1** `games/` holds only real source: C from an approved compiler, or
  proven assembly written as instructions, never copied bytes. Drafts and
  scaffolding live under `recon/` and shrink toward zero. _Check: publication._
- **S2** Tagged `/* FAKEMATCH: reason */` code counts until both games reach
  100%, and is then removed as the final step, as pret's was. Until then
  compiler-steering C (wrappers, volatile, forced temporaries, dead code),
  inline assembly and fixed-register variables are allowed when each carries a
  `FAKEMATCH` tag with its measured reason inside the function. A device is
  for a difference of instruction order or register choice, an unused
  local of a real type whose size exactly explains the native stack frame,
  or a form the bytes prove the source had (a CSE block that ended where
  no code is left, a block loop.c kept inside its loop, an add kept apart
  from the store it addresses): any tagged device may change which
  instructions are chosen when it restores such a form, and its tag states
  the form and the measured difference (Pascal, 2026-10-07). A device comes
  only after reshaping the source has failed, and a function is never
  written as inline assembly. A fake match stays in the source, tagged; it is never
  replaced by a compiler option or routing that fits only those functions
  (K1, K3). `CAMELOT_ASM("proof")` marks a small handwritten asm helper inside C,
  proven handwritten assembly under C2; its few instructions stay inside
  the C functions that use it, which stay C. Four are proven (Pascal, 2026-10-08): `Dma_Set` and
  `Transform_SetIdentity` (multi-register `stmia` with no paired `ldmia`,
  which no pattern in agscc's machine description emits), `Iwram_Call2` (a
  return through ip, which no procedure-call standard produces) and
  `WaitFrames`' stack switch (a write to sp). A layer that
  explains a habit of the whole game is source, not a device: the ☀️ scene
  script layer (inline helpers forwarding to the engine services in
  `field_event.h`, `field_service.h` and `runpa_jo/jo.h`) matches every scene
  where direct calls break 642 functions (Pascal, 2026-10-05). Never patch
  compiler output. _Check: no-asm (in lint-staged)._
- **S3** Camelotize: reconstruct readable source consistent with the games'
  hardware, approved compilers and demonstrated program structure. Separate
  what the bytes establish, what the compiler requires and Alchemy's house
  style. The following conventions guide the reconstruction
  (Pascal, 2026-10-07):

  ```c
  #include "event.h"

  #define BOUZU       8           // 坊主

  // 井戸の前へ移動
  static void EV03_IDO(void)
  {
      ...
      OBJ_POS_SET(BOUZU, 192, 264);
      ...
  }
  ```

  - Layout: `games/<game>/src`, `include`, `sound`, `text`, the same folders
    in both games (`games/tbs`, `games/tla`) and `games/common`; lowercase
    8.3 file and folder names, `.c` and `.h` (Pascal, 2026-10-08: no evidence
    for the uppercase style). The one exception follows the GNU convention:
    an assembly file the C preprocessor reads (`#include`, `#define`, `#if`)
    is all capitals, name and extension (`NAME.S`); plain assembly such as
    `rom_head.s` is lowercase. The build chooses preprocessing by the
    extension, as GCC's driver does (Pascal, 2026-10-08). Directives stay
    lowercase (`#include`): the preprocessor reads no other. Rename a file
    and its include and linker references together. _Check: hygiene,
    publication._
    Shared code and declarations live once in `games/common`; a function is
    adopted into the module that holds its linker neighbours, not into a new
    file of its own.
  - Language: C89 with only extensions supported by the approved compiler;
    GNU C `//` comments are allowed. Identifiers are ASCII. Use prototypes,
    `typedef`, `enum` and ordinary C idioms where appropriate. Choose types,
    signedness, declarations, linkage and scopes from the program and matching
    evidence.
  - Naming: ordinary English technical vocabulary and abbreviations may mix
    with romaji. Use the Japanese game names for characters and places
    (`ROBIN`, `GERALD`, `SUKURETA`, `RUNPA_JO`) and established game terminology
    where useful. Keep spelling consistent within a concept and names concise
    and meaningful. Unknown meaning stays unknown.
  - Comments: prefer concise Japanese explanations of verified game behavior,
    units, fields and constraints. English technical terms and useful English
    explanations are allowed. Use trailing comments for fields and cases, and
    short blocks or section labels when they help explain a larger operation.
    Keep comments useful and UTF-8. FAKEMATCH explanations (S2) stay English.
  - Formatting: the showcase uses tabs at four columns, opening braces on
    their own lines, a space before control parentheses, and blank lines
    between logical steps. Group related declarations and constants; align
    short trailing comments where it aids scanning. Use section separators
    for larger operations. Preserve directive order, macro semantics and
    deliberate data-table layout. The repository's `.clang-format` records
    these choices. Format only the source being reviewed, and keep approved
    library source out of style passes.
  - Numbers stay numbers until evidence establishes their meaning. Then use
    an appropriate technical or game-domain name; do not invent meanings to
    remove literals. Decimal suits quantities and hexadecimal suits masks,
    packed fields and hardware values. `#define` and `enum` are both allowed
    where their types and generated bytes fit. Message names remain linker
    symbols (W8); a named constant never invents an address or table ID (O2/O3).
  - Values are written in the author's units, whatever the bytes store:
    scene coordinates are pixels, and the command shifts them into fixed
    point.
  - Scenes are written in `event.h` commands (`OBJ_POS_SET`, `MES_PUT`,
    `WAIT`, `FLAG_CHK` ...), all-caps macros that expand to exactly the call
    the bytes need: the scene script layer's inline helper (S2), or the
    engine service itself where the native calls it directly.
  - An event function is named `EV<id>_<SUBJECT>`: its decimal id from the
    scene's event table, then one romaji noun for what it is about
    (`EV08_KAGI` 鍵, `EV52_IDO` 井戸, `EV99_ROUYA` 牢屋), or a clear English
    verb where the code does not show the subject (`EV12_SET`,
    `EV20_MAIN`, `EV94_MAIN`). It is `static` once the table that reaches it
    lives in the same object. These event names are our indexing convention.
    Engine functions use compact CamelCase (`BtlTgtSet`), with meaningful
    technical vocabulary and game terms; existing `Subsystem_VerbObject`
    names may remain until their module is reviewed. Locals and struct fields
    express their role rather than a register number or unexplained offset.
  - The bytes prove layers, units and calls; the ROM keeps no names (only
    credits text and ⚓️'s build stamp `020610172024`). Names, numbering and
    comments are reconstruction: never present them as recovered.
  - Mechanical cleanup scores prioritize review. Review meaning, naming,
    comments, organization and byte identity, preserving necessary casts,
    hardware `volatile`, `goto`, work arrays and S2 devices with their reasons.
  - The showcase is `BtlTgtExec` in
    `games/tbs/src/battle/tgtexec.c`.
- **S4** Never throw work away: commit each adoption and attempt, and keep a
  near miss as a draft with the remaining difference in its header. One draft
  per listing, overwritten as it improves: a trial is a commit or a line in
  that header, never another file or folder. Workers may edit and delete
  `recon/` listing and linker lines when adopting a function. An edition whose
  bytes differ stays on its scaffold until its branch is written; it never
  blocks the adoption in the others.

### Assets

- **A1** Assets are editable files built by our tools in every build, as pret's
  are: indexed PNGs with the game's real palettes, TSV tables and tilemaps,
  WAV, MIDI, PO and text definitions. Never raw dumps, grey sheets without a
  built palette, whole-area blobs or stored compression tokens. Sources may
  include the files the build makes from these inputs. PO catalogs hold
  editable messages and control commands; the build encodes them into each
  game's native text format. Generated resources live in ignored build output.
  _Check: publication._
- **A2** A picture is drawn the way the game shows it: tile sheets at their real
  width or sprite shape, pictures with a tilemap as the map shows them,
  animations one frame wide. _Check: layout._
- **A3** A PNG may carry an all-grey palette only when the build writes that
  palette into the ROM from it or from the first part of its part list.
  _Check: publication._
- **A4** Encoder options live beside their assets; recipes may carry a sprite
  shape (one of the 12 hardware sizes) in the file name.
- **A5** No JSON anywhere: tables are TSV. _Check: publication._
- **A6** Each edition combines its message catalog, character mapping, font
  images, glyph metrics and layout with its measured code differences. Common
  code and interfaces stay shared. Select differing resources in their owning
  module or build rule and keep code branches beside the behavior they change.
  Group editions where the measured behavior agrees. Text layout uses the
  actual font's pixel widths and control commands.
  _Check: compare-editions for byte identity._

### Compilers

Camelot built each game with one compiler and one set of options, as one
Makefile would. Alchemy rebuilds that compiler: GCC 2.96 as Camelot's
engineers used it and, where the whole game shows it, as they modified it.
The line between that and bending a compiler to fit is K3: a compiler, an
option or a change counts only when it explains a game's code as a whole,
never a file or a function.

- **K1** Game code compiles with agscc (GCC 2.96, the 2000-07-31 snapshot) and
  one option set per game, the same for every game C file:
  `-O2 -mthumb -mcpu=arm7tdmi -nostdinc -fcall-used-r4`, plus
  `-mthumb-interwork` for ☀️ and K2's options for ⚓️. `-fcall-used-r4` is a
  stock GCC option: 265 of 266 ☀️ and 1,648 of 2,302 ⚓️ functions that save lr
  start saving at r5. Nintendo's library code is not Camelot's and keeps
  the compiler and flags its own bytes show, one setting per whole library
  file, as pret names its library files: pret's agbcc for MusicPlayer2000
  (`-O2`) and the flash library (`-O`; its last two files moved from `-O2`
  once the address words their `-O` output emits were found, 2026-10-08), and agbcc_arm for RAM-executed ARM code
  (`-fomit-frame-pointer`). Game ARM C of either game, the files listed
  in `GAME_ARM_SOURCES`, compiles with that same stock agbcc_arm at that
  same library ARM setting (`alchemy diff --arm`): Camelot's few IWRAM ARM
  routines came from the SDK's 2.95-line ARM compiler, as Nintendo's do,
  while their Thumb code came from 2.96. One unmodified compiler and one
  option set for all ARM code; only the instruction set is chosen per file,
  as a Makefile does, and no compiler source is changed or carried in this
  repository. Measured over all seven compiler-shaped ARM functions (no APCS
  frame, saves from r5, r11 never allocated, interworking returns), differ
  per draft as IwramSqrt, ColorBuffer_DarkenKernel,
  ColorBuffer_DarkenPartialKernel, Transform_UpdateVertices,
  Object_UpdateAll, UiText_DrawGlyphArm, Tile_BuildMetatiles: stock
  agbcc_arm 38, 66, 90, 76, 107, 177, 6 (560); agscc `-marm` with the game
  options 28, 64, 100, 183, 468, 377, 19 (1,239), and its nearest stock
  setting, `-mno-apcs-frame -ffixed-fp`, 1,215: agscc builds an APCS frame
  by default and differs in loop and stack-slot habits throughout. Copying
  2.96's `arm_adjust_cost` into agbcc_arm closed Tile_BuildMetatiles' six
  scheduling instructions but was a compiler change fitted to one function
  (K3) and carried GCC code here; it is not a route. Measured again over
  all game ARM C on 2026-10-08 (2.96's anti/output-dependence rule alone and
  2.96's whole function): Tile_BuildMetatiles matched without its two order
  devices, but Transform_UpdateVertices matched under neither even with 8 of
  its 19 devices removed, and the hybrid is no stock compiler, so the
  question is closed (Pascal, 2026-10-08: a limited set of stock compilers).
  What scheduling
  remains is a difference of instruction order, which S2 handles in the
  source. The lead's decision under Pascal's delegation (Pascal, 2026-10-07:
  an ARM mode in agscc, or agbcc for its provenance, never anyone else's
  compiler). No other routing: never a per-file or per-function compiler,
  option or flag for game code.
  _Check: routing tests._
- **K2** ⚓️'s compiler is agscc with two changes a Camelot engineer made to
  GCC's Thumb backend between the games, rebuilt as options in GCC's own
  style, and one stock scheduling option, all turned on for all of ⚓️'s game
  code:
  - `-mthumb-split-constants` builds a constant that no single Thumb
    immediate holds inline, from a shifted byte and an add, instead of loading
    it from the literal pool. 13,907 of the 14,101 constant-building sequences
    in ⚓️'s code follow the rule exactly; ☀️ has 18.
  - `-mthumb-call-via-lr` calls through a register by moving the target into
    lr and branching with the second half of a BL, instead of calling a
    `_call_via_rX` stub. ⚓️ makes 2,549 such calls and 3 stub calls, all in the
    separately compiled sound library; ☀️ makes 596 stub calls.
  - `-mtune=arm9tdmi` (stock GCC; Pascal, 2026-10-05) schedules loads for the
    ARM9TDMI pipeline. About 30 ⚓️ near misses that differed only by load
    order matched under it; 14 objects that matched before changed, 12 were
    repaired and `Menu_CenterResourceEntries` and
    `Menu_LayoutResourceEntries` went back to listings with their C kept as
    drafts. It is not yet the whole answer: some native sequences fit a load
    latency of 3 cycles and others leave a load's delay slot unfilled, and
    many remaining ⚓️ drafts differ by one swapped load. A load model that
    explains both is an open K3 study for Pascal.

  Why a person at Camelot made the two changes: each habit runs through the whole game,
  so it belongs to the compiler, not to some functions; neither is in ☀️,
  built a year earlier with the same compiler family; no public GCC from
  2000-2002 has either (the study in agscc's README read all 417 ARM backend
  changes and 28 snapshots and releases); and each is a small, local change a
  toolchain engineer makes for speed: the first skips a slow cartridge-ROM
  load for the constant, the second skips a stub on every indirect call. The
  second only works because ⚓️'s game code no longer interworks with ARM code,
  one coherent change to one compiler. Without them, 37 credited ⚓️ functions
  no longer match. _Check: compiler-source-check, bundle validation._
- **K3** A compiler, option or change is admitted only when one option set
  explains the great majority of a game's C and assembly, counted across the
  whole game rather than listed by function; no public compiler of the time
  already explains it; and it is written as a plausible change by a Camelot
  engineer, in GCC's own style, with its evidence and reason recorded here and
  in agscc's README. Pascal approves each. Every compiler is built from pinned
  source, on any system, and the twelve editions judge what it produces.
  _Check: compiler-source-check, bundle validation, routing tests._

### Work

- **W1** Tooling is Rust: no TypeScript, JavaScript, Python or shell beyond
  a command line. New tooling solves a demonstrated recurring blocker and
  carries a test; tooling nothing needs is removed. Pret builds its games with
  about 15,000 lines of tools. _Check: language-check._
- **W2** Work on your own branch; `staging` is the only long-lived branch, and
  a worktree is removed when its branch lands. Land on staging with `make land`,
  which builds and compares all twelve editions and runs the tests.
  _Check: land._
- **W3** Alchemy owns compilation, linking, verification, reading and analysis,
  behind one `alchemy` command for both games; the ags crate owns asset
  encoding and decoding (agsgfx, mid2ags, wav2ags, po2ags). Look at how pret
  does something before inventing. Alchemy is never stricter than pret: where
  pret's practice settles a question, follow it (Pascal, 2026-10-08).
  Settled this way (2026-10-08): Camelot's far-call banks start at fixed
  addresses per edition, written in the linker scripts as pokeemerald pins
  its graphics blobs (`__gfx_load_address`), with the gap as linker fill;
  leftovers of Camelot's packer buffer after compressed streams are kept as
  tagged literal bytes beside their asset, as pret keeps garbage data (an
  S1 exception for proven packer garbage only); and each encoder takes one
  rule for all its streams, verified on every stream of its kind in both
  games. A standard compressor may take its standard flags per file, as
  pokecrystal's `gfx/lz.mk` does; a reconstructed packer takes no invented
  per-file flag without clear evidence Camelot ran its tool that way
  (Pascal, 2026-10-08). A stream Camelot edited after packing keeps its
  encoder's one rule and takes a patch file beside its asset instead: the
  build packs the asset with a neutral stand-in at each listed position,
  then writes the shipped byte back into that literal. Each entry is
  tagged with its measured proof: the stream matches only with that one
  literal changed, and nearly any stand-in value works. Patch bytes count
  as data and are reported apart, like FAKEMATCH (Pascal, 2026-10-08).
  "100%" is announced only as a pret reviewer would accept it: no game
  function matching through instruction-emitting inline asm other than
  the proven `CAMELOT_ASM` helpers (S2), no register pin or empty asm
  barrier left, nothing in the
  build read from the ROM, and `recon/<game>` gone except its raw listings
  kept for reference.
- **W4** Choose what to match from `make deps`: first the functions other
  unfinished functions call, with the structures they share, then the ones
  with nothing left to name, largest first.
- **W5** The whole ROM judges. Before every commit, `make compare-editions`
  must print OK for all twelve editions; a per-object or per-function diff
  misses a scaffold section placed two bytes early or a lost alignment pad.
  `alchemy objects` names both; `alchemy keep` checks all twelve.
- **W6** A change to what counts, a rule, a compiler option or routing lands
  as its own commit, with DONE before and after in its message, never mixed
  with new C. Most of DONE's large jumps and drops have been changes of this
  kind, not matching.
- **W7** Parallel work divides by owner: `alchemy queues` gives each worker
  whole modules or overlays, and each camelotize worker one area. Before
  naming a field or function, check no other branch named it
  (`git log --all -S`). When branches merge, an adoption wins over edits to
  its draft, and code that used an old name takes the new one. A worker
  commits only verified work; anything unverified at a stop is discarded.
- **W8** Camelotizing moves code toward S3's target and never away from
  it. A mechanical rewrite is `alchemy camelotize` followed by `alchemy
  keep`; a rewrite by hand meets the same test. Message names stay
  linker symbols: as an enum, small ids become immediate moves instead of
  literal-pool loads and the bytes change.

### Notes from matching

Not rules; what earlier work measured.

- Gaps between locals and spill slots come from register allocation, not
  from locals; an unused local exactly the size of a native frame gap is the
  only frame device S2 allows (`alchemy sizes` lists the candidates).
- An empty asm barrier between two tests blocks jump threading; `const`
  declarations change scheduling; a store through a struct field and one
  through a cast pointer build a halfword constant differently.
- CSE can reuse a register that already holds 0 or 1, so a native store of a
  register is often a constant in the source.
- A two-byte padding piece next to a function becomes the compiler's
  alignment once the function is C; the listing or scaffold section that
  follows then starts with `.balign 4`.
- Editions differ: Japanese is often an older layout of the same function
  and gets its own `#if` branch, or stays on its scaffold (S4). The
  German, Spanish, French and Italian far-call tables sometimes name a stub
  differently; `alchemy label` names a scaffold-only stub.
- Nested functions adopt with their parent.
- Scenes reach the engine through one inline layer: as direct calls the
  services change 642 of 5,621 ☀️ English functions, through the layer all
  match. Some scenes call a service directly, and those calls stay direct.
- All 1,798 coordinate constants in ☀️ scene calls are whole pixels, and 769
  of them fit no half-cell grid: the authors placed actors in pixels.
- C89 and GCC 2.96 take only ASCII identifiers; Shift-JIS in comments and
  strings was normal, minus the ダメ文字 (表, ソ, 能 ...) whose second byte is
  `0x5C`, the backslash that escapes a quote or continues a line.

## Commands

Setup needs Apple Silicon macOS with Xcode's command-line tools, and Rust
with Cargo. Put your own ROMs under `roms/` as `tbs-en.gba`,
`tla-ja.gba` and so on; each is checked before use and none is ever
committed (L1).

```sh
git submodule update --init && git config core.hooksPath .hooks
make bootstrap         # pinned compilers; BUNDLE=/path imports an approved bundle
make worktree          # in a new worktree, before its first build
make compare           # the Japanese base of ☀️; compare-tla for ⚓️
make compare-tbs-en    # one edition: tbs or tla, then ja, en, de, es, fr or it
make compare-editions  # all twelve editions (W5)
make build-rom TARGET=tbs-en  # build a changed game without comparing
make test
make verify
make land              # on staging, before committing a landing
```

The `alchemy` binary is `tools/out/cargo-target/release/alchemy`; it acts on
the checkout it runs in, and `alchemy --help` lists every command.

```sh
# Choose what to match
make deps                              # what unblocks the most (W4)
alchemy similar                        # nearest C twins in either game
alchemy similar --editions             # each ☀️ listing's copies in all twelve builds
# Match one function
alchemy diff DRAFT.c [ADDR] [--show]   # a draft against its listing; differ 0 matches
alchemy diff DRAFT.c --heat            # differing instructions per line, first divergence marked; permute --heat aims there
alchemy permute DRAFT.c [ADDR] [--minutes N]   # rewrites and PERM_* choices toward differ 0, in out/permute
alchemy sizes DRAFT.c [SIZE]           # struct sizes; the locals that fill a frame gap
alchemy show EDITION FUNCTION          # a built function beside the ROM
alchemy adopt ADDR LABEL NAME [--into MODULE.c]   # into its module, out of all scaffolding
alchemy label NAME ENGLISH_ADDR LEN    # name English-only scaffold data in other editions
alchemy adopt-data OBJECT PIECE...     # a data block's object into all six scripts, out of all scaffolds
# Check the whole build
alchemy objects EDITION...             # what differs from the ROM, object or placement
alchemy dead [--apply PATH]            # what no edition links, includes or selects
make progress                          # DONE code and data, one line per edition (alchemy progress)
alchemy check devices [--update]       # register pins, asm barriers, inline asm, volatile: may only fall
# Camelotize
make worst                             # maintained C ranked by how mechanical it reads
alchemy camelotize REWRITE FILE...     # actors, events, registers, declarations, barriers
alchemy camelotize numbers|casts|idioms FILE...  # decimal quantities, needless casts, C idioms
alchemy keep [PATH...]                 # keep only edits that leave all twelve identical
alchemy sweep FILE.c...                # drop FAKEMATCH pins, barriers and volatiles the bytes no longer need
# Work in parallel
alchemy queues --workers N --out-dir DIR   # work that cannot collide (W7)
alchemy integrate BRANCH...            # merge, reconcile linker scripts, build all twelve
# Report for people
make figure                            # PROGRESS.png and PROGRESS_CHART.png after a build
```
