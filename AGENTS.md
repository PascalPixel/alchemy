This file is not for humans.

# Alchemy

It instructs the agents that work on Alchemy; people read `README.md`, `INSTALL.md` and the source. The agents keep this file current, its wording included (Pascal, 2026-10-18). Essentials come first; the rules in full, by ID, follow under Reference.

## What this is

Alchemy rebuilds Golden Sun: **The Broken Seal** (TBS, ☀️) and **The Lost Age** (TLA, ⚓️) byte for byte from readable C, assembly and editable assets, in twelve editions: each game in Japanese, English, German, Spanish, French and Italian. Build IDs are the game, then the language: `tbs-ja`, `tla-en`. Japanese releases are the source editions; localizations are measured differences.

- ☀️ is 100% matched (2026-10-14). The work there now is humanizing: making it read as modern C, as pret's games do (S3). `games/tbs/src/effect` is the model folder; the rest of the project is done the way it is.
- ⚓️ is still being matched; most of its code is listings and drafts under `recon/tla`.
- Every change keeps all twelve editions byte-identical. A readable form that changes a byte is wrong, not a trade-off (W8).

## Commands

Setup needs macOS or Linux with a C compiler, make, git, curl, patch, xz, the ARM binutils, pkg-config, libpng and utf8proc (`INSTALL.md`). ROMs go under `roms/` as `tla-ja.gba` and so on, checked against `rom.sha1`, never committed (L1).

```sh
git submodule update --init --recursive agscc
make bootstrap         # the tools, the pinned compilers, GAS 2.10 and libgcc
make -j8 compare-tbs   # the six ☀️ editions, which read no ROM
make compare-tla-en    # one edition: tbs or tla, then ja, en, de, es, fr or it
make compare-editions  # all twelve editions: required before every commit (W5)
make tbs-en            # build one edition without comparing
make modern            # the six ☀️ editions' C with a current GCC: no errors allowed
make test              # the C build tools' tests
```

Matching and progress use the optional Psynergy submodule (Rust, Cargo):

```sh
git submodule update --init psynergy
make psynergy          # optional Rust tools; CI uses Rust 1.98.1
make test-psynergy     # reconstruction tests against this checkout
git config core.hooksPath .hooks  # send verified measurements when pushing main
make progress          # DONE code and data, per game and edition (C2)
make land              # on main, before committing a landing (W2, C3)
```

The binary is `psynergy/target/release/psynergy`; it acts on the checkout it runs in (`ALCHEMY_ROOT` points a standalone checkout at this one).

```sh
psynergy diff DRAFT.c [ADDR] [--show]   # a draft against its listing; differ 0 matches
psynergy diff DRAFT.c --heat            # differing instructions per line, first divergence marked
psynergy diff DRAFT.c --rtl             # the compiler's per-pass dumps for the draft
psynergy permute DRAFT.c [ADDR] [--minutes N]   # rewrites toward differ 0, in out/permute
psynergy progress                       # what make progress prints, and what make land sends
```

## Boundaries

Always:
- See `make compare-editions` print OK for all twelve editions before every commit (W5). Send its output to a log file; it is long.
- Tag every compiler-steering device `/* FAKEMATCH: reason */` inside its function, with the measured difference without it (S2).
- Name things only from evidence; an unknown keeps a neutral placeholder (`sub_08012345`, `unk_14`) (S3).
- Commit each verified step on its own, attempts and near misses included (S4); a worker commits only verified work (W7).
- Look at how pret does something before inventing, and never be stricter than pret (W3).

Ask Pascal first:
- Any compiler, option, routing or agscc change. The set is frozen (K1 to K3).
- Anything that changes what counts as DONE (C2, W6).
- Deleting anything that cannot be restored.
- Pushing, pulling or fetching, and anything published outside this machine.

Never:
- Commit, upload or publish a ROM or any copy of the original game files (L1).
- Use or open another project's Golden Sun work (L2).
- Open, read, use or mention leaked Nintendo or Camelot code or SDKs, or anything derived from them, however it is presented (L3).
- Let ROM bytes or the expected output steer the build or the count, beyond pret's `.incbin` scaffold under `recon/` (O1).
- Write a function as inline assembly, patch compiler output, or give a function its own compiler option (S2, K1).
- Treat old git history, deleted files or backups as evidence (O5).
- Add JSON (A5), or Python, shell or TypeScript programs (W1).

Everything else goes ahead without asking, including fixing a check that wrongly blocks honest work, as long as it still refuses everything it refused before.

## Workflow

1. Work in the main checkout on a branch; `main` is the only long-lived branch (W2). Pascal prefers no worktrees.
2. Choose what to match by W4: first what other unfinished code calls, then the largest with nothing left to name.
3. Draft in `recon/<game>/en/main/<address>.c` and check with `psynergy diff`. A near miss stays a draft, its remaining difference in its header (S4).
4. Adopt a match into the module that holds its linker neighbours, in every edition whose bytes agree (C1, S4), and commit it.
5. Humanize in passes (Goal): mechanical, then structure, then meaning, each byte-identical.
6. Land on `main` with `make land` (W2, C3). Push only when Pascal asks.

## Pitfalls

Measured, and easy to repeat:
- `do { ... } while (0)` is not neutral in agscc: it compiles as a one-pass block and can move code. A statement macro takes the shape its sites match with (S2).
- agscc pads every struct to 4 bytes. A table read by columns is a two-dimensional array with an `enum` naming its columns (Notes).
- Locals declared in an inner block change register allocation; a macro cannot take its caller's locals as its own (Notes).
- `p[n]` and `*(p + n)` are not interchangeable in agscc: turning the effect parts' `gWorkSlot[n]` into `WORK_SLOT(n)`, which is `*(gWorkSlot + (n))`, changed bytes in 23 of 32 files, and `&p[n]` against `&*(p + n)` in another. Each site keeps the spelling it matches with (2026-10-18).
- Declare an object only where its struct type is complete. Declared earlier, through an incomplete type, it keeps a smaller alignment, and code that reads it compiles differently: moving one `struct SpriteScale` declaration ahead of its struct changed two German effects (2026-10-18).
- Builds from another branch leave `out/*/deps.mk` and `out/*/objects.mk` that break this one: delete them. `tools/out` is shared across checkouts: after switching to a branch whose tools differ, run `rm -rf tools/out/obj tools/out/ags-* && make tools`.
- A comment says what the code cannot: what the player sees, units, why a forced form is forced, the evidence for a name. It does not restate the code.

---

# Reference

## Goal

**DONE is a pair, code and data** (Pascal, 2026-10-08). **DONE code = matching C + proven library, handwritten or veneer assembly**, over each game's executable bytes; **DONE data = the non-code bytes each image places from source**, over all its other placed bytes (C2); each is counted in all six of a game's editions together. 100% means both: every language of a game builds all of its code and all of its data from source. `make progress` prints both figures for each game and each edition, with code in its parts (C, assembly and 8-byte stubs). ☀️ has the priority; the target is ☀️ 100% and ⚓️ 100%, sharing as much code between the games as possible.

**100% is pret's standard** (Pascal, 2026-10-11), and nothing stricter (W3). A game is 100% when all of these hold:

1. All six editions build byte-identical from source, and anyone with their own cartridge dumps can verify it from the published hashes.
2. DONE code and DONE data are both 100% in all six editions (C2), the way decomp.dev counts them.
3. No function is matched by an assembly body or by instruction-emitting inline asm other than the proven `CAMELOT_ASM` helpers (S2); the raw listings under `recon/<game>` are reference only, none linked.
4. Nothing in the build reads a ROM (O1), confirmed by the independent audit O4 requires.
5. The source compiles as C with a current GCC (`make modern`): no match expected, no errors.
6. Every compiler-steering device is tagged (S2) and counted (a count of each kind), and the counts are recorded with the game's 100%, as ☀️'s are below. pokeemerald reached 100% with its fakematches and removed them over the years after; pokeruby is 100% with empty asm barriers in six files today. A register pin or an empty asm barrier is a tagged device like a volatile or a one-pass block, not a bar to 100%.

☀️ reached 100% by this standard on 2026-10-14. Its counts that day, measured in a clean clone with no ROM by the tools of that day (C2 then credited assembly by proof; `make progress` now counts by folder and finds ☀️ at 8,247,796 code and 45,187,016 data bytes, 100% of each):

- DONE code 8,236,546 of 8,236,546 bytes and DONE data 45,190,274 of 45,190,274 bytes over the six editions; code is 94.79% C, 1.56% proven assembly (32,064 bytes of it the compiler library) and 3.64% far-call stubs.
- FAKEMATCH: 401 tags, steering 1,578,532 bytes of C, 19.16 points of the code figure. Devices: 11 register pins, 19 empty asm barriers, 0 inline asm outside the proven `CAMELOT_ASM` helpers, 163 volatile (hardware volatiles included until the counter tells them apart), 274 goto, 50 one-pass blocks, 262 tagged-macro uses, 3 once-only loops.
- The linker scripts pin 15 far-call banks at fixed addresses per edition (W3); 10 bytes in 7 assets are written back after packing (W3); some data, such as the sprite animation tables, is still rows of hex in TSV.
- `make modern`: 0 errors, 3,679 distinct warnings with arm-none-eabi-gcc 16.2.0.
- The six editions' SHA-1 are in `rom.sha1` and README.

Matching is not the finish. The project is finished when the source also reads as modern C a person can follow, as pret's games do (S3). Work need not reach that form at once, but every change moves toward it (W8). Removing the tagged devices (S2) is humanizing work that continues after 100%.

A game is finished when all six hold (Pascal, 2026-10-08; humanizing, 2026-10-14):

1. 100% DONE code and 100% DONE data in all six editions.
2. Every asset ported: nothing reaches the ROM as copied bytes; all data is editable source our tools build (A1).
3. `recon/<game>` removed: no scaffold, draft or edition scaffolding left.
4. FAKEMATCH at 0%: no tagged device left (S2).
5. A limited set of stock compilers builds all of it (K1–K3).
6. The codebase humanized (S3): English names and comments, pret's naming and formatting, files grouped as the original's objects were (object alignment, trailing data and strings, link order) rather than one file per function, repeated code folded into `games/common` helpers and macros, and every name resting on evidence (in-game text, the table or id that reaches it, or the code that uses it), an unknown keeping a neutral placeholder.

   Humanizing runs in passes, each byte-identical in all twelve editions (`make compare-editions`):
   1. Mechanical: formatting, dead files and code, includes and prototypes, the modern build's type warnings, stale FAKEMATCH devices and undefined behaviour, which keeps the original's form and gets its defined form behind pret's `UBFIX` switch, on in the modern build. Each gameplay bug in the original keeps its code and takes pret's `// BUG:` comment, with its fix behind the opt-in `BUGFIX` switch, and each unused original function is marked `UNUSED`, as pret marks them (Pascal, 2026-10-14). Code behind either switch is not dead code.
   2. Structure: files grouped as the original's objects, repeated code folded into `games/common` helpers and macros.
   3. Meaning: English names and comments from evidence; an unknown keeps its placeholder.

## Where things are

- `games/<game>/src`, `include`, `sound`, `text` and `games/common` hold the source; `<game>` is `tbs` or `tla`.
- ☀️'s code sits in one folder for each bank of the main image, named for the bank (`runtime`, `field`, `window`, `game`, `menu`, `shop`, `battle`, `effect`, `object`), beside `graphics`, `map`, `sound`, `system` and `debug`, and in `scene`: each scene overlay is `scene/<place>.c` with its entry table `scene/entry/<place>.s` and its import table `scene/import/<place>.s`, and text several scenes include is under `scene/common`. The six overlays that are not scenes sit the same way in `menu` and `debug`. Unit headers follow the same paths under `include/unit`. `games/common/src` has the same bank folders: a shared file sits under the bank that links it.
- `games/tbs/src/effect/effect.c` is the battle effect object, written as parts: it holds the guide, includes and shared macros, and includes the 66 files under `effect/parts/` in ROM order (S3).
- `games/tbs/objects.tsv` is ☀️'s object list: every object of the main image and of each overlay in link order, one row each, with the sections it places and the editions that link it. `games/tbs/main.ld` is the one main script of all six editions and names no object: it has the memory, the output sections, an `INCLUDE` for each place of the list and the far-call banks' fixed addresses for each edition, chosen by `--defsym TBS_EDITION_<LANG>=1`. The Makefile's `ags-objects` writes each place's objects for the edition it links into `out/<edition>/ld`.
- ⚓️ is laid out the same way: `games/tla/objects.tsv` lists its main image for all six editions and `games/tla/main.ld` is its one main script, chosen by `--defsym TLA_EDITION_<LANG>=1`. Its overlays are still one script each under `recon/tla/raw/overlays`.
- `recon/tla/raw` holds ⚓️'s English disassembly that is not C yet, one listing per function or run. ☀️ is 100% code and data and has no `recon/` folder: nothing in its build or tools reads one.
- ☀️'s overlays link by the one `games/tbs/src/system/overlay/overlay.ld` and their rows of the object list; `games/tbs/overlays.tsv` gives each overlay's `.bss` section its header. The overlay's own data and bss not yet in its modules are in `ovl_<id>.s` beside the script. Each overlay's import table sets `import.NAME` for each stub it imports, by edition where they differ, and includes `games/tbs/src/system/overlay.inc`, whose rows give the stubs their order. ⚓️'s overlay listings are `recon/tla/raw/overlays`.
- `games/<game>/src/system/rom_head.s` is the cartridge header, linked first in every edition, as pret's `rom_header.s`: the logo is `.space 156` and the complement 0. Right after objcopy the Makefile runs the header fix-up (`ags-header`, following pret's gbafix) on every ROM it links, which writes the logo and the complement (L1).
- `games/tbs/include/event.h` is the scene command vocabulary (S3); `field_event.h` and `field_service.h` hold the script layer the commands expand to.
- `recon/tla/<lang>` holds that ⚓️ edition's scaffold (`main/`, `overlays/`, `units/`) beside `recon/tla/unidentified.s` (data not yet identified, placed from the ROM) and the symbol files.
- The `Makefile` builds everything, every rule readable there. `tools/` holds the C programs it runs where make cannot do the job: `ags-graphics`, `ags-audio`, `ags-music`, `ags-text`, `ags-objects`, `ags-deps`, `ags-overlays` and `ags-header`. The optional `psynergy` submodule holds the Rust matching aids (`psynergy diff`, `psynergy permute`) and the progress count; nothing in it takes part in a build. Assets build beside their sources and are ignored; `out/` and `tools/out/` are build output and are ignored.
- `.github/workflows/progress.yml` publishes progress after each push to main: the `progress` branch holds `history.tsv` and README's two figures, written only by CI; the repository keeps no copy of either.

## Rules

### Legal

- **L1** Never commit, upload or publish a ROM, private inputs or any other copy of the original game files. The cartridge header's Nintendo logo lives only in the build's header fix-up tool, which writes it into each built ROM as pret's gbafix does (Pascal, 2026-10-08).
- **L2** Evidence comes only from our own ROMs, this repository and public documentation. Never use another project's Golden Sun work, and never open one to compare.
- **L3** Never use leaked Nintendo or Camelot code or SDKs, and never open, read or mention sources that might contain them.

### Counting

- **C1** A function counts only when its complete extent, literal pool included, compiles exactly and every edition stays byte-identical. A function counts in an edition only when that edition's build links its C. _Check: make compare-editions._
- **C2** DONE is pret's calcrom over the linker maps of byte-identical builds (`make progress`), by folder (Pascal, 2026-10-14): a byte the images place from `games/` is done, one from `recon/` is not yet. Each image counts from its first placed byte to its last: the ROM, and each overlay once, in its unpacked image as linked; an overlay's compressed stream in the ROM is outside the totals, as is cartridge padding after the last placed byte. DONE code is the bytes of `.text` sections: C where the object's source is C, assembly otherwise, with the far-call banks' and overlay tables' 8-byte stubs and the compiler library (done) reported apart. DONE data is every other placed byte: done from `games/`, the compiler library or the linker's fill, not yet from `recon/` (incbin scaffolds, not-yet-C listings, drafts). Each edition counts its own maps and a game's figure sums its six; an unfinished edition's code total is at least the English build's, whose listings hold the code the other editions still place as scaffold bytes. A game is 100% only when DONE code and DONE data are both 100% in all six editions (Pascal, 2026-10-08).
- **C3** Every push to main carries its measurement: `make land` compares all twelve editions, runs the tests and measures both games, and the pre-push hook uploads that count by commit. CI records it on the `progress` branch, draws README's two figures there and sends decomp.dev its report.

### The oracle stays out of the build

The reference ROM judges the build and never feeds it. Any path by which the expected answer steers the build or the count is cheating, whatever its form. Reconstructing Camelot's compiler from habits that run through a whole game is allowed (K3); fitting a compiler, an option or a routing to particular files or functions is exactly this cheating.

- **O1** ROM bytes reach the build only through pret's `.incbin "baserom.gba", OFFSET, SIZE` scaffold in `recon/<game>`, which never counts, and the cartridge logo the header fix-up writes (L1). Nothing else derived from the ROM or the expected output (bytes, addresses, sizes, tokens, tables, hashes) may steer the build or the count, `out/` included; reports under `out/` are for people only.
- **O2** Every name is defined where its bytes are, as a C definition or a label, and layout is the build's object list order (`games/<game>/objects.tsv`) where a game has one, and the linker scripts' object order otherwise. No equate, alias, `#define` or table gives a name an address, except Camelot's own fixed addresses: resident IWRAM routines called through fixed entries, RAM buffers its code addressed as constants, and the hardware cartridge restart entry, each listed once in a game header and checked against where the linker placed it. One name per place; never a name that spells its own value.
- **O3** Scene and resource ids come only from their tables and are used whole. Overlay code never branches straight into the main image.
- **O4** A mismatch is fixed in source, or the code stays disassembly or a draft. Before a milestone is recorded, run an independent audit for leaks.
- **O5** Old git history, deleted files, backups and unreachable git objects are never evidence. Re-derive an answer from our ROMs and the current tree, or ask Pascal.

### Source

- **S1** `games/` holds only real source: C from an approved compiler, or proven assembly written as instructions, never copied bytes. Drafts and scaffolding live under `recon/` and shrink toward zero. Assembly is proven as the compiler library, as whole aligned 8-byte far-call stubs, or as handwritten, which it is only with an instruction, frame or pattern Camelot's compilers cannot emit (rrx; a branch, adc or sbc on a shift's carry; r11 as an ordinary register; a callee-saved register save without lr; CPSR/SPSR; swi; computed pc into code; stores into code; unpaired multiply halves; a pc-relative block load of data; a mode switch inside a function; movs with no shift, where their copy-and-test is subs #0; ldm or stm writing back off sp, other than a block copy's list from r0 or a doubleword pair; a conditional branch on a data-processing instruction's flags with no compare, where agscc's branch patterns always emit cmp or cmn; one literal pool shared by two functions, where GCC flushes its pool at each function's end) or a linkage no procedure-call standard produces; anything else stays a not-yet-C listing in `recon/` until it matches as C.
- **S2** Tagged `/* FAKEMATCH: reason */` code counts, at 100% as it did in pret's games (Goal); removing it is humanizing (S3, W8), and a finished game has none. Compiler-steering C (wrappers, volatile, forced temporaries, dead code, one-pass blocks), empty asm barriers and fixed-register variables are allowed when each carries a `FAKEMATCH` tag with its measured reason inside the function. All of them are devices of one standing: the counts of pins, barriers, inline asm and volatiles may only fall; trading one device for another is not progress unless the new form is the source's, and a device that stays records in its tag the plain forms measured against it (S4).
  - Goto: the count of `goto` statements may only fall too. A `goto` is ordinary C and no FAKEMATCH device: it carries no tag and is outside the FAKEMATCH figures. But one that only forces a layout is not how the function was written, and it goes when its loop or branch is found (Pascal, 2026-10-12).
  - What a device may do: a device is for a difference of instruction order or register choice, an unused local of a real type whose size exactly explains the native stack frame, or a form the bytes prove the source had (a CSE block that ended where no code is left, a block loop.c kept inside its loop, an add kept apart from the store it addresses): any tagged device may change which instructions are chosen when it restores such a form, and its tag states the form and the measured difference (Pascal, 2026-10-07). A device comes only after reshaping the source has failed, and a function is never written as inline assembly. A fake match stays in the source, tagged; it is never replaced by a compiler option or routing that fits only those functions (K1, K3).
  - Proven asm helpers: `CAMELOT_ASM("proof")` marks a small handwritten asm helper inside C, proven handwritten assembly under C2; its few instructions stay inside the C functions that use it, which stay C. Four are proven (Pascal, 2026-10-08): `Dma_Set` and `Transform_SetIdentity` (multi-register `stmia` with no paired `ldmia`, which no pattern in agscc's machine description emits), `Iwram_Call2` (a return through ip, which no procedure-call standard produces) and `WaitFrames`' stack switch (a write to sp). A fifth is proven (2026-10-11, decided under Pascal's delegation): `MulQ16`, the 16.16 multiply (`smull` with RdLo and RdHi in any two registers, even over its inputs, where mulsidi3 pairs them and smulsi3_highpart discards RdLo; its joining `orr` takes a fresh output reload register beside two dying inputs and nothing is scheduled across it, as only asm statements do). It corrects the earlier handwritten classification of `Object_UpdateAll` and `Transform_UpdateVertices`, which are compiled C around it. It also marks the BIOS calls `swi 2` (Halt, as in `VBlankIntrWait`) and `swi 3` (Stop) in `runtime.c`, which C2 already counts as handwritten.
  - Layers: a layer that explains a habit of the whole game is source, not a device: the ☀️ scene script layer (inline helpers forwarding to the engine services in `field_event.h`, `field_service.h` and `scene/lunpa_fortress.h`) matches every scene where direct calls break 642 functions (Pascal, 2026-10-05). So is the ☀️ IWRAM call layer (one inline per resident routine in `iwram_call.h`, calling its checked fixed entry): 360 call sites match through it, and five sites in two functions keep a tagged direct call (2026-10-11, decided under Pascal's delegation). So is the ☀️ engine call layer (Pascal, 2026-10-13): `engine_call.h`, which `field_service.h` includes, has one typed inline for each of the thirteen main-image routines it names that the scene, menu and debug overlays import, the scheduler's `Scheduler_AddOrUpdateCallback` among them. The thirteenth, which joined last, is the type-01 decoder `Resource_DecodeType01`, which the Colosso, Ramakan and world map scenes and the title and clear menus import: called directly, `Title_LoadBackground` and `Clear_LoadBackground` each take two more instructions. Every overlay call of those routines goes through it and none stays direct: 281 call sites, and 47 more through the scene layer's `Task_AddCallback` and `FieldScheduler_AddOrUpdateCallback`, which forward through it. As direct calls, 22 functions of the English build differ, 19 distinct (shared included code compiles two of them into several scenes). `scene/common/scroll_initialize_hblank_dma.h`, which `lemurian_ship.c` and the main image's `object/psynergy_effects.c` both include, reaches it from both images; other main-image code calls these routines as ordinary functions, outside the layer, and `field/camera.c`'s identical decode keeps `call.h`'s tagged `Value2`.
  - Statement macros: a statement macro written `do { ... } while (0)`, C's idiom for a macro used as one statement, is source too where every use matches through that shape: `Io_Write16` (106 sites), `Battle_SetStart` (34), `UiRender_MarkRow` (10) and `MenuCell_GetCursor` (4). Their comments say why they have that shape, and the wrapper is no device. A device inside a macro body (an inner one-pass block, an empty asm, a register pin) still counts at each use, tagged or not, and a macro whose tag records that its stores match plainly elsewhere stays a device (Pascal, 2026-10-13). The wrapper is not neutral in agscc: `do { ... } while (0)` compiles as a one-pass block and can move code, so a statement macro takes the shape its sites match with, and no shape is assumed (2026-10-18). Measured on the TBS effect macros: written as `do { ... } while (0)`, `EFFECT_PRESENT_FRAME`, `EFFECT_RELEASE_BLITTERS` and `EFFECT_START_TRANSFER` broke TBS effect.c, and `EFFECT_CLAMP` broke some of its sites; as comma expressions the first three match at every site, and `CLAMP` matches as a plain `{ }` block (which cannot stand before an `else`). `macros.h`'s `SET_VECTOR`, `SET_POSITION` and `SET_VELOCITY` (comma expressions) and `CLAMP` (plain block) match at 154 TBS and 67 TLA sites outside effect.c. The other way round, `BattleFx_BuildBlitter`'s `PUT`, `COPY` and `PATCH` match only as one-pass blocks and stay tagged devices.
  - Output: never patch compiler output.
- **S3** Humanize: make the source readable modern C, as pret writes its games, in English, which is less confusing (Pascal, 2026-10-14). The bytes stay identical; nothing has to look like the original's own source.

  ```c
  #include "event.h"

  #define MONK    8

  // The monk walks to the well.
  static void Event_WalkToWell(void) {
    ...
    SetObjectPosition(MONK, 192, 264);
    ...
  }
  ```

  - Layout: `games/<game>/src`, `include`, `sound`, `text`, the same folders in both games (`games/tbs`, `games/tla`) and `games/common`; lowercase file and folder names, `.c` and `.h`, short and descriptive. An assembly file the C preprocessor reads is `NAME.S`; plain assembly is `name.s`. Rename a file and its include and linker references (its rows of the object list, where its game has one) together. Shared code and declarations live once in `games/common`; a function goes into the module that holds its linker neighbours, not into a new file of its own. An object the ROM shows as one may be written in parts: one `.c` file that includes them at its end in ROM order, as `effect/effect.c` does. Each part is one family or run of the object's code, named for what it holds, and opens by saying it is compiled only through that file; the include order is the code's order, so no list orders them. The parts are ours, not Camelot's files, which the ROM does not keep (2026-10-18).
  - Language: C89 with the GNU extensions the compilers accept, `//` comments included. Use prototypes, `typedef`, `enum`, structs and ordinary C idioms. Choose types, signedness, declarations and scopes from the program and the matching evidence.
  - Naming, as pret's: functions in PascalCase (`StartBattle`), globals `gName`, file statics `sName`, constants and macros in UPPER_SNAKE_CASE, locals and struct fields in camelCase. Characters, places, items and other game terms take their English names from the English release (`ISAAC`, `SATUROS`, `VAULT`, `scene/lunpa_fortress.c`). A name says what the thing does; unknown meaning stays a neutral placeholder (`sub_08012345`, `unk_14`), never a guess.
  - Comments: English, short, saying what the code does and why where it is not obvious. FAKEMATCH tags (S2) say what they steer.
  - Formatting: clang-format's default (LLVM) style (Pascal, 2026-10-15): two spaces, no tabs, braces on the same line, 80 columns. `.clang-format` records it; it leaves include order alone, since that decides which declarations are in scope. Preserve directive order, macro semantics and deliberate data-table layout.
  - Numbers stay numbers until evidence establishes their meaning, then take a name; do not invent meanings to remove literals. Decimal suits quantities, hexadecimal masks, packed fields and hardware values. A named constant never invents an address or table ID (O2/O3). Message names are linker symbols (W8): a message is written by its bare name, with no cast (`ShowMessage(MsgName)`), and `MSGNO(name)` is its number as a constant (Pascal, 2026-10-14). Flag and song ids take their names from `flags.h` and `songs.h`, plain `#define` lists like pret's: `FLAG_` for flags, `MUS_` for music and `SE_` for sound effects; only proven names go in, and an unproven sound takes only its kind (`SE_138`, `MUS_027`). A temporary flag is `FLAG_SCENE_TEMP_xx`, with a trailing comment saying what each scene uses it for (Pascal, 2026-10-14).
  - Values are written in the author's units, whatever the bytes store: scene coordinates are pixels, and the command shifts them into fixed point.
  - Scenes are written in `event.h` commands, macros that expand to exactly the call the bytes need: the scene script layer's inline helper (S2), or the engine service itself where the native calls it directly.
  - The ROM keeps no names (only credits text and ⚓️'s build stamp `020610172024`). Names and comments are ours: never present them as recovered.
  - Keep necessary casts, hardware `volatile`, `goto`, work arrays and S2 devices with their reasons.

- **S4** Never throw work away: commit each adoption and attempt, and keep a near miss as a draft with the remaining difference in its header. One draft per listing, overwritten as it improves: a trial is a commit or a line in that header, never another file or folder. Workers may edit and delete `recon/` listing and linker lines when adopting a function. An edition whose bytes differ stays on its scaffold until its branch is written; it never blocks the adoption in the others.

### Assets

- **A1** Assets are editable files built by our tools in every build, as pret's are: indexed PNGs with the game's real palettes, TSV tables and tilemaps, WAV, MIDI, PO and text definitions. Never raw dumps, grey sheets without a built palette, whole-area blobs or stored compression tokens. Sources may include the files the build makes from these inputs. PO catalogs hold editable messages and control commands; the build encodes them into each game's native text format. The Makefile builds each asset beside its source, ignored by git, as pret's do.
- **A2** A picture is drawn the way the game shows it: tile sheets at their real width or sprite shape, pictures with a tilemap as the map shows them, animations one frame wide.
- **A3** A PNG may carry an all-grey palette only when the build writes that palette into the ROM from it or from the first part of its part list.
- **A4** Encoder options live beside their assets; recipes may carry a sprite shape (one of the 12 hardware sizes) in the file name.
- **A5** No JSON anywhere: tables are TSV.
- **A6** Each edition combines its message catalog, character mapping, font images, glyph metrics and layout with its measured code differences. Common code and interfaces stay shared. Select differing resources in their owning module or build rule and keep code branches beside the behavior they change. Group editions where the measured behavior agrees. Text layout uses the actual font's pixel widths and control commands. _Check: make compare-editions._

### Compilers

Alchemy uses a limited set of pinned compiler families and fixed profiles: the July 2000 GCC Thumb family for game code, early GCC 2.95 for ARM C, and pret's agbcc lineage for the separately compiled sound and flash libraries. This is a reconstruction choice measured against the twelve editions, not a claim to have recovered Camelot's exact compiler executable or original Makefile. An instruction set or a proven library class selects a route; a particular function or mismatch never selects a compiler, option or patch. K3 governs changes to these choices.

Pascal approved this bounded set on 2026-10-10: three C generators and six profiles (the two game Thumb profiles, shared libgcc C, game ARM C, sound and flash). The `agscc` GPL submodule owns all three, with pret/agbcc as an unmodified nested submodule and GCC 2.95.2's archive pinned under `arm/`. Alchemy pins one toolchain revision and builds it through `make -C agscc`; compiler source and host patches stay outside this repository. No fourth generator is built or installed. agscc builds from that source on macOS and Linux, 32- and 64-bit, ARM and Intel, under one host configuration and one host model, with no Rosetta; its README lists the hosts tested and how.

The set is frozen (Pascal, 2026-10-11: "This will be agscc with no changes for years"). No compiler research is started, and no compiler, option, route or agscc commit is added. The one exception is a host fix: a supported host on which the pinned source fails to build or gives different output, fixed in agscc and proven to leave every compile of the twelve editions byte-identical. A mismatch in game code is never a reason (K2, K3).

- **K1** Game Thumb code compiles with agscc, based on GCC upstream revision `04179d4a511b13cef92eacdb10a51bcd124fea7a` (2000-07-31), and one option set per game: `-O2 -mthumb -mcpu=arm7tdmi -nostdinc -fcall-used-r4`, plus `-mthumb-interwork` for TBS and K2's options for TLA. Agscc is a documented host port, not an untouched GNU release. Its source and host changes remain in the GPL submodule. Shared libgcc C uses the same generator with its runtime recipe: `-O2 -mthumb -mcpu=arm7tdmi -fno-inline`, ordinary ABI and no interworking.

  Both games' ARM C uses GCC 2.95.2 with the stock optimizer and ARM backend: `-O2 -mthumb-interwork -fomit-frame-pointer -fcall-used-r4`. Its GNU archive is pinned by SHA-256 in agscc/arm. It is built as a native program on every host, with five documented host fixes in agscc/arm/host.patch (a cast-as-lvalue, Darwin's `sys_nerr` declaration, the unprototyped generator table, compile-time checks of the host model, and real constants copied one operand slot at a time); none changes instruction selection, allocation or scheduling. The same route serves the Makefile, `psynergy diff --arm` and the ARM permuter. It replaces agbcc_arm; there is one ARM C generator and no alternate ARM library route.

  Nintendo's sound and flash library source uses pret's `old_agbcc` at its pinned source revision, with `-O2 -mthumb-interwork` for MusicPlayer2000 and `-O -mthumb-interwork` for flash. This is the `2.9-arm-000512` lineage, not a compiler renamed as stock GCC 2.95.2. Includes and edition definitions serve preprocessing; they do not select code-generation variants. As in pret, that pin is the matching build's only. The modern build (`make modern`) is no part of this set and adds no route to it: its current GCC compiles these libraries as it compiles Camelot's C, and what that compiler breaks gets its defined form behind `UBFIX`, as in pret's `m4a.c` (Pascal, 2026-10-14).

  Matching establishes practical compatibility, not exact historical identity. No other routing or function-specific compiler flag is allowed: the Makefile's three file lists (SOUND_C, FLASH_C, ARM_C) are the only routes.

- **K2** TLA uses the same agscc Thumb executable with two existing reconstruction options and one stock scheduling option enabled for every game Thumb file:
  - `-mthumb-split-constants` builds suitable constants with a shifted byte and an add instead of a literal-pool load.
  - `-mthumb-call-via-lr` calls a Thumb target through lr with a lone BL suffix instead of a call-via-register stub.
  - `-mtune=arm9tdmi` uses GCC's stock ARM9TDMI scheduling model; the target CPU and permitted instructions remain ARM7TDMI.

  These reconstruct habits measured across TLA's code. The 2026-10-10 bounded English-image scan observed 8,064 adjacent mov/shift/add constant sequences and 2,495 lr/suffix calls in game-labelled code; TBS had none of either in the same scan. TLA also has explicit ARM-kernel loaders and separate library stub calls, so the game is not described as wholly free of ARM interworking. The scanner excludes decoded literal pools and jump tables; these counts are bounded observations, not every executable byte in the game.

  No tested stock compiler supplies both general forms. Disabling the two reconstruction options reduces matching existing TLA Thumb objects from 1,255/1,255 to 875/1,255 in the study's expanded inventory. These fractions describe objects, not whole-game completion or independent functions. The data does not establish who implemented the original behavior or prove that its original implementation was a particular GCC patch. The options remain explicit reconstruction features with their evidence in agscc's README.

  This decision admits no additional scheduler or allocator patch. Further changes require K3's whole-game evidence and Pascal's approval; a remaining instruction-order mismatch alone is not grounds to change compilers.

- **K3** A compiler, option or change is admitted only when one option set explains the great majority of a game's C and assembly, counted across the whole game rather than listed by function; no public compiler of the time already explains it; and it is written as a plausible change by a Camelot engineer, in GCC's own style, with its evidence and reason recorded here and in agscc's README. Pascal approves each. Every compiler is built from pinned source, on any system, and the twelve editions judge what it produces.

### Work

- **W1** The build is a plain Makefile where every rule can be read, as pret's (Pascal, 2026-10-14). A build tool is a small single-purpose C program for a step make cannot do; Rust reconstruction tools live in the optional Psynergy repository, and building or modding Alchemy requires no Rust (Pascal, 2026-10-16). No TypeScript, JavaScript, Python or shell beyond a command line. Each tool solves a demonstrated recurring blocker and carries a test, and tooling nothing needs is removed. The goal is decompilation: pret builds its games with about 15,000 lines of tools.
- **W2** Work on your own branch; `main` is the only long-lived branch, and a worktree is removed when its branch lands. Land on main with `make land`, which builds and compares all twelve editions, runs the tests and counts progress.
- **W3** The Makefile owns compiling and linking; `tools/` owns the C asset encoders and the build's other small tools; the optional `psynergy` submodule owns the Rust matching aids and the count. C tools use the `ags-` prefix, the repository's clang-format style, plain names, and comments explaining formats and constraints (Pascal, 2026-10-16). Look at how pret does something before inventing. Alchemy is never stricter than pret: where pret's practice settles a question, follow it (Pascal, 2026-10-08). Settled this way (2026-10-08): Camelot's far-call banks start at fixed addresses per edition, written in the linker scripts as pokeemerald pins its graphics blobs (`__gfx_load_address`), with the gap as linker fill; leftovers of Camelot's packer buffer after compressed streams are kept as tagged literal bytes beside their asset, as pret keeps garbage data (an S1 exception for proven packer garbage only); and each encoder takes one rule for all its streams, verified on every stream of its kind in both games. Resource_DirectoryTable keeps its per-edition fixed bank pins: three independent audits found no source extent for the zeros before it (Pascal, 2026-10-09). A standard compressor may take its standard flags per file, as pokecrystal's `gfx/lz.mk` does; a reconstructed packer takes no invented per-file flag without clear evidence Camelot ran its tool that way (Pascal, 2026-10-08). A stream Camelot edited after packing keeps its encoder's one rule and takes a patch file beside its asset instead: the build packs the asset with a neutral stand-in at each listed position, then writes the shipped byte back into that literal. Each entry is tagged with its measured proof: the stream matches only with that one literal changed, and nearly any stand-in value works. Patch bytes count as data and are reported apart, like FAKEMATCH (Pascal, 2026-10-08). What 100% means is settled the same way, in Goal (Pascal, 2026-10-11): the earlier wording here, which refused 100% while any register pin or empty asm barrier remained, was stricter than pret.
- **W4** Choose what to match: first the functions other unfinished functions call, with the structures they share, then the ones with nothing left to name, largest first.
- **W5** The whole ROM judges. Before every commit, `make compare-editions` must print OK for all twelve editions; a per-object or per-function diff misses a scaffold section placed two bytes early or a lost alignment pad.
- **W6** A change to what counts, a rule, a compiler option or routing lands as its own commit, with DONE before and after in its message, never mixed with new C. Most of DONE's large jumps and drops have been changes of this kind, not matching.
- **W7** Parallel work divides by owner: each worker takes whole modules or overlays, and each humanizing worker one area. Before naming a field or function, check no other branch named it (`git log --all -S`). When branches merge, an adoption wins over edits to its draft, and code that used an old name takes the new one. A worker commits only verified work; anything unverified at a stop is discarded.
- **W8** Humanizing moves code toward S3's target and never away from it. Every rewrite, mechanical or by hand, leaves all twelve editions byte-identical (`make compare-editions`). Message names stay linker symbols: as an enum, small ids become immediate moves instead of literal-pool loads and the bytes change.

### Notes from matching

Not rules; what earlier work measured.

- Gaps between locals and spill slots come from register allocation, not from locals; an unused local exactly the size of a native frame gap is the only frame device S2 allows.
- An empty asm barrier between two tests blocks jump threading; `const` declarations change scheduling; a store through a struct field and one through a cast pointer build a halfword constant differently.
- CSE can reuse a register that already holds 0 or 1, so a native store of a register is often a constant in the source.
- A two-byte padding piece next to a function becomes the compiler's alignment once the function is C; the listing or scaffold section that follows then starts with `.balign 4`.
- Editions differ: Japanese is often an older layout of the same function and gets its own `#if` branch, or stays on its scaffold (S4). The German, Spanish, French and Italian far-call tables sometimes name a stub differently.
- Nested functions adopt with their parent.
- Scenes reach the engine through one inline layer: as direct calls the services change 642 of 5,621 ☀️ English functions, through the layer all match. Some scenes call a service directly, and those calls stay direct.
- All 1,798 coordinate constants in ☀️ scene calls are whole pixels, and 769 of them fit no half-cell grid: the authors placed actors in pixels.
- C89 and GCC 2.96 take only ASCII identifiers; Shift-JIS in comments and strings was normal, minus the ダメ文字 (表, ソ, 能 ...) whose second byte is `0x5C`, the backslash that escapes a quote or continues a line.
- Measured in the pin and barrier pass (2026-10-11), read from agscc's own source and its RTL dumps (`psynergy diff --rtl`):
  - Register choice is arithmetic. `REG_ALLOC_ORDER` is r3, r2, r1, r0, ip, lr, r4 to r7, r8, r10, r9, r11. local-alloc gives a block-local pseudo the first register in that order free over its lifetime; global.c ranks pseudos by floor_log2(refs) * refs / live_length, refs weighted by natural loop depth, and hands out registers in that order. A pseudo whose first set carries a constant `REG_EQUAL` note has its live length doubled (update_equiv_regs). A fresh `movs r0, #0` where r3 was free means the zero's pseudo conflicted with r1 to r3 for longer than its statement shows.
  - Thumb `movhi` lists its memory-or-immediate alternative before its immediate one, so a halfword constant that survives to a register loads from the literal pool (`ldr r0, =0x0`). HImode registers come only from halfword stores of a constant, volatile halfword loads and subreg copies for halfword stores; a `u16` local is promoted to SImode.
  - CSE follows a conditional branch into its target only when the label has one use and is preceded, past notes and unused labels, by a barrier; a loop-end note before the label stops it. On the path it records the branch condition, and a later constant equal to a known register becomes that register, narrower modes included. A one-pass block's loop-end note also ends CSE's block and is a scheduling barrier for the first insn after it; sched2 breaks ties by RTL order.
  - regmove's backward pass copies operand 1 of a two-address and/or/add into the result unless that operand is a hard register or not a REG: a `u16` field read directly is a subreg operand and is skipped, so the mask is copied instead.
  - expand_call precomputes an argument that itself contains a call before the later arguments, shortening the live range of values used only earlier. A void call through a function pointer loads it into r3; a value-returning call loads it into r0.
  - loop.c moves an invariant when threshold * savings * life reaches the loop's insn count, threshold = 1 + the non-fixed registers (15 with a call) less 3 per earlier move; -O2 runs it twice. reload_cse_move2add turns a constant load into `add rN, delta` when rN still holds a nearby constant; reload_cse deletes a store cselib proves redundant.
  - arm_reorg places literal pools before shorten_branches, from default lengths: an empty asm counts 4 estimated bytes and emits none, a conditional branch 8, a halfword pool load 4. No C statement and no single pattern length supplies that 4; the five pool barriers all follow a sprite attribute pair, and one "pair, then empty asm" idiom is byte-identical at twelve of thirteen such pairs across editions (a layer candidate for Pascal under S2).
  - A function that saves r9 to its first frame slot and never uses it is a nested function storing its static chain; it goes when its parent is adopted with it.
  - `psynergy permute` cannot parse a file with nested function definitions.
- Measured on ☀️'s effect tables and macros (2026-10-18):
  - agscc pads every struct to a multiple of 4 bytes, so a row of two `u8`s is 4 bytes. With `packed`, any field after the first still compiles differently: it adds the field's offset to the table's address, where the native adds it to the index. A table read by columns is written as a two-dimensional array with an `enum` naming the columns (`gIce_Counts[v][ICE_FRAMES]`), which compiles as the flat index did.
  - Locals declared inside a block take their pseudos when the block is expanded, after the function's own: moving a function's loop variables into a block of their own changes register allocation, so a macro cannot take its caller's locals as its own.
  - Within one object, code and initialized data follow definition order after preprocessing, so an object's parts are ordered by their includes. Across objects only the object list orders them.
  - Every Thumb function starts on a 4-byte boundary, so the ROM cannot show where one object's code ends and the next begins. Data can: an object's data starts on a 4-byte boundary, while `u8` tables pack at any offset, so a gap no type alignment explains marks a boundary. All 21 gaps in ☀️'s effect tables are explained by the type that follows.
- ⚓️ near misses (2026-10-18, 796 drafts under `recon/tla/en/main`): 22 matched in English (19 adopted, in every edition whose bytes agree, two already source, one waiting on a map id's name); 400 differ only in instruction order (306 by ten rows or fewer), 48 only in register choice, and 325 otherwise. In the order-only drafts the native most often issues a literal-pool load earlier than agscc (242 times in 132 drafts) and a constant, shift or ALU instruction later. This corpus is the evidence a K3 question about ⚓️'s scheduling would start from.

