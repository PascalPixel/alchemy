# Alchemy

Alchemy is a decompilation of Golden Sun: **The Broken Seal (TBS)** ☀️ and
**The Lost Age (TLA)** ⚓️. Both games rebuild byte for byte from readable C,
assembly and editable assets. Japanese releases are the source editions;
localizations are measured differences. Build IDs are `tbs` and `tla`.

This is the only working guide; `README.md` is for fans. It is short on
purpose. Every stretch when this file grew into a long list of prohibitions
was a stretch when progress stalled, and every stretch when it stayed short
and pointed at methods was a stretch when progress moved. Add a line here only
when it will make the next agent faster.

## The goal and the one number

**DONE = matching C + proven library, hand-written or veneer assembly**, over
each game's executable bytes. `make progress` prints it in bytes;
`make progress-subject` gives the commit prefix. Optimise one thing:
**new ☀️ bytes landed on main per hour.** Report bytes, never rounded
percentages. Work that cannot move that number (compressor tails, packing,
provenance archaeology, tooling nobody asked for) is timeboxed and never
blocks a landing.

## Our model: pret's pokeemerald

Work the way pokeemerald did (a checkout lives at `~/Developer/pret/pokeemerald`;
method only, never code or symbols). When in doubt, do what pret did. The one
deliberate difference is presentation: our tree looks like the project Camelot
most plausibly had on disk in 2001.

- `games/<GAME>/SRC`, `INCLUDE`, `SOUND`, `TEXT`; uppercase 8.3-style names
  (`BATTLE/EFFECT/PARTICLE.C`, `FIELD_EVENT.H`); `Subsystem_VerbObject`
  functions; short `pos`, `cnt`, `tbl`, `buf`, `work` locals; C89.
- Places use romaji from the Japanese ROM plus one area word (`RUNPA_DOU`,
  `HAIDIA_MURA`); characters use their Japanese names (Gerald, Jasmine,
  Sukureta).
- Scaffolding lives apart in lowercase `recon/tbs` and `recon/tla` and shrinks
  to nothing at 100%.

## Rules that never bend

1. **Match first.** A function counts when its complete extent, literal pool
   included, compiles to the exact bytes and `make compare` stays identical.
2. **Fake matches count, tagged.** C that matches through an odd construct
   carries `/* FAKEMATCH: <what is odd> */` and is cleaned up later. Never:
   patched compiler output, ROM bytes copied into source, `asm` in game C
   (the reviewed `Dma_Set` and `Iwram_*` header macros are the only inline
   assembly), or changing a test or the measurement to pass.
3. **Never throw work away.** A near miss is committed as a draft with its
   remaining difference in its header. Every attempt ends in a commit.
4. **What may be committed:** code, tooling, our documentation and the
   editable inputs the build consumes, tracked the way pret tracks them. Never
   ROMs, the cartridge logo, raw dumps, another project's Golden Sun work, or
   SDK or leaked code. Evidence comes from our own ROMs, this repository and
   public documentation.
5. **Everything uncredited is C not yet written.** Camelot wrote the game in
   C; only SDK library code, genuinely hand-written routines and the veneer
   stubs are assembly, and only a credited `.S` module may say so. An owner
   still built from its disassembly is `not-yet-c` (`not_yet_c` in the
   overlay registry), never "assembly". No registry, gate, draft header or
   report may classify C as assembly because it resisted: not a hard
   instruction (`stmia` comes from `Dma_Set`), not a register wall, not a
   size. Label it by what remains to be done.
6. **Only Pascal** changes credit standards, approves compiler source,
   binaries and digests, and authorises pushes. Record each decision here with
   its date.

## Where the bytes come from

Measured 2026-09-24 and 25: about 200 KB landed before 21:00 on the 24th, then
the rate fell tenfold once overlay adoption stalled and lanes were paused.
Choose the method by its yield, not by the size of the target alone.

| Method | Yield | Use it on |
| --- | --- | --- |
| Overlay recipe | 6–9 KB/h per agent | unadopted overlay owners |
| Hand-writing from disassembly | about 8 min per matched function when the owner is whole | large registered main-image owners |
| Library identification | 13 routines at once | Nintendo SDK code (sound driver, flash) |
| Credit decisions from Pascal | 5.6 KB at once | whole classes of proven assembly |
| Batch lifting in the main image | about 30% similarity, almost no matches | nothing; don't |
| Re-sweeping the same 1–3 halfword residual with the same spellings | zero | only with a new structural idea; the owner stays a normal target |
| Writing an owner fresh from its disassembly | 0.3–2 KB per lane pass | owners with no draft; beats nudging an old draft that is far off |
| Splitting a listing that bundles several functions | the pieces often match first time | any listing with an inner prologue, or reached only by `bl` from its parent |
| Permuting a near-miss draft | about 1.4 KB per pass, then falling | drafts within ~10 halfwords; read every candidate for identical behaviour |

Where the rest is (2026-09-25): about 25 overlay drafts far from exact and a
handful of overlay owners with no draft; about 190 main drafts that differ in
structure or miss code; about 55 drafts within a few halfwords that differ
only in register choice or instruction order. An investigation built nine
patched compilers and found agscc's allocation, reload and scheduling
tie-breaks confirmed by the exact owners (each flip broke 68–419 of them), so
those residuals are the C's shape, not the compiler.

**The overlay recipe.** `alchemy overlay trial OWNER` opens the owner: it
registers a source path and a single-overlay translation unit, puts the
scoring placeholder in, and writes a first draft from `psynergy decompile`
with every call renamed to its `Engine_*` service in `FIELD_EVENT.H`, a
registered name or the overlay's own function, and the literal pool listed
from the ROM; the unit binds each name at its runtime address (an import
veneer's listing offset plus 0x8000). Score and sweep the near misses with
`alchemy overlay try OWNER [FILE...]`, which rebinds the unit for every name
the files use and ranks them (`--install` keeps the best); adopt with
`alchemy overlay adopt OWNER --apply`, which takes the trial in place and
drops the owner's not_yet_c rows and recon draft, or park a near miss with
`alchemy overlay draft OWNER --note TEXT`, which commits the recon draft
with its residual in the header and undoes the trial; `--undo` drops a
trial. Area and script lookup functions (a switch on game state
+0x1c0/+0x1c2) take about five minutes each by hand.

**Reuse first.** `alchemy check siblings --report out/siblings.json` also
finds near matches to existing exact C. Adapt the proven source after checking
its changed constants and callbacks: a 308-byte Vinasu owner closed first try
this way despite its old draft differing in 113 halfwords.

**Hand-writing.** Read `alchemy inspect <owner> --asm`, its callers, callees
and exact neighbours, and write the C a Camelot programmer would have written,
reusing existing headers, structs and registered names. Fix one hypothesis at
a time. After about 30 minutes, or three attempts without a new idea, commit
the draft and take the next owner. Its header records the remaining
difference (`N differing halfwords`) and, when the lane stopped on a
structural obstacle, one `WALL: <reason>` line; `alchemy targets` lists every
not-yet-C owner by size with both. An owner registered as pieces
(`Region_`, `Fragment_`, `Continuation_`) or bundled with a neighbour must be
made whole first: one complete function, one listing, one owner. An overlay
owner's extent lives in `recon/<game>/semantic/regions.json` and a
not-yet-C owner's retention row in `semantic/overlay-assembly.json`; overlay
lane scripts read both; an adopted owner's retention row is removed with its adoption.

**Library code.** SDK objects keep their own compiler family and flags,
recorded with the reason in `tools/alchemy/src/compiler/routing_data.rs`, as
pret builds its library and flash files: the flash library is agbcc `-O`, the
MusicPlayer2000 C is agbcc `-O2`. A whole file shares one family; never route
a single function. Hand-written SDK assembly is credited as `library`.

## What makes code match

Each of these closed real owners. Try them before inventing anything new.

- **Prototypes decide argument order.** A value-returning callee gets r0 set
  last; declare unused-result callees `void` and match registered return
  types.
- **Constants.** Pool-loaded constants are often `Value_XXXXXXXX` link
  symbols. A halfword I/O store of a constant forces an early literal pool;
  a zero held in a one-halfword struct (`struct Half { u16 v; }`) gives the
  64-byte pool reach of a halfword move instead of 60. A constant stored
  through a cast pointer goes to the pool; the same store through a struct
  member becomes `movs`. Write `& -0x1000`, not `& 0xfffff000`.
- **Memory.** Use typed structs and indexed arrays, not byte-pointer offsets;
  put fields in one struct when the reference keeps a store order that
  implies aliasing; declare neighbouring globals as separate externs; write
  IWRAM tables as extern arrays; use bitfields for flag masks; a union gives
  byte and halfword views of the same game-state rows.
- **Locals and control flow.** Reuse one local across branches and give each
  loop its own counter; declare locals in stack-slot order; one shared mask
  local when the reference keeps 255 in a register; an explicit `case 0:`
  gives the reference's switch table; `||` of two `!= 0` tests merges into one
  load where `a | b` does not; a goto loop keeps constants from being hoisted;
  separate constants the decompiler merged across branch tails.
- **Machine interfaces.** `Iwram_Call2(left, right, routine)` passes the
  routine last so its address loads first; alternate two locals across chained
  `Iwram_MulQ16` calls. `Dma_Set` in `INCLUDE/DMA.H` produces the
  `stmia r3!; subs r3, #12` idiom; call it from C. Write `_call_via` calls as
  ordinary calls through a function pointer; derive its prototype from the
  callee, not leftover values in argument registers.
- **Zeros and pools.** A plain `0` held in a `u8` or `u16` local is loaded as a
  halfword pool constant, whose short reach puts the literal pool mid-function
  where the ROM has it; a `(u16)(s32)&Value_XXXX` constant does the same for a
  mask. Clamp helpers and IWRAM copy/fill calls as `static __inline__`
  wrappers make constants reload per call instead of living in saved
  registers. An inline call wrapper can also rematerialize a stack address
  instead of copying a saved pointer (both Makyuri actor-move routines).
  Read `-fsched-verbose=5` (scheduling ties) and `-dL` (strength
  reduction) dumps before sweeping spellings.
- **Last resort, tagged:** a `do { } while (0);` around one or two statements,
  a statement swap, or a temporary that fixes one evaluation order.
- **Scoped temporaries.** A local declared in a block after parameter setup
  can change spill allocation where declaration order alone cannot. This
  closed Vinasu's 628-byte dust routine: form the parameter address before
  declaring its temporary velocity. Tag a scope kept only for matching.

## Making the C read like Camelot's

Matching comes first, but names are evidence too. Name things from the game
itself: messages from `TEXT/EN.PO` (msgid is the zero-padded decimal), items
from message 0x182 plus the item id, places from the table
`alchemy build assets --locations tbs-en` writes to `out/tbs-en/locations.tsv`.
Before naming a value, check which call consumes it: a value passed to
`GameFlag_*` is a flag, not a message (Lunpa Fortress was once misnamed after
Vale dialogue this way). A function's name lives once, in
`recon/<game>/source-paths.json`; the build injects the link names, so never
keep in-file `#define Name Func_addr` aliases. Shared declarations have one
header. Code built into several overlays is one instanced source that spells
no address names; bind named symbols per instance. `SRC/FIELD/COMMON/KUUPUAPPU_RUNPA/JAIL.C`
and `SRC/FIELD/SORU_SEKIZO/SETUP_STAGED_ACTORS.C` are the finished examples.

## Running agents

The lead gets more bytes by keeping three agents productive and landing their
work than by working alone.

- **Never `cd` into a lane worktree.** Worktrees live outside the session's
  directory, and `cd … && …` asks Pascal for permission every time. Use
  `git -C`, `make -C`, absolute paths and the built binaries by path.
- **Keep lanes running.** Four or five lanes on the best-yielding slices;
  don't pause them for tooling or cleanup work, and change a registry format
  only when every writer lanes use accepts the new one in the same commit.
- **Brief with specifics.** Give each agent a slice (an address range or
  overlay range no other agent touches), its concrete targets with sizes and
  known residuals, the methods and cookbook above, and the scripts earlier
  agents left. Slices by address never collide.
- **Start from origin.** Create each worktree **outside the checkout** with
  `git worktree add ../alchemy-worktrees/<name> -b wf/<name> origin/main`
  after `git fetch`, then `make worktree-setup`. A stale local `main` silently
  costs an agent everything landed since.
- **Commit every adoption immediately** and every draft before moving on. An
  agent with no commit in 30 minutes is checked; one with none in an hour is
  stopped and its slice rebriefed.
- **Spend tokens on bytes.** A residual that is only register choice gets one
  attempt guided by the allocator dump, then its draft header records it and
  the lane moves on. Send `make verify` and hook output to a log and read the
  errors. Brief from one ranked target list (`alchemy targets`) rebuilt at each landing, and keep
  one shared set of lane scripts rather than a copy per lane.
- **Land every 30 minutes** from one landing worktree with `alchemy land
  BRANCH... --message TEXT`: it merges each finished branch (registries by a
  structural three-way JSON merge that keeps both sides' additions), runs
  `make test`, `make compare-all`, `make coverage` and `make verify`, and
  squash-commits with the progress prefix; `--push` pushes `main`. Then
  remove the landed worktree and branch. `main` is the only long-lived branch.
- **Workflows** (many agents at once) need Pascal's approval.
- Scripts are TypeScript on Bun or Rust, never Python or shell. The only prose
  files are `AGENTS.md` and `README.md`; no notes, plans or reports anywhere.

## Compiler, build and assets

Game code uses the approved agscc bundle (GCC 2.96) through
`tools/alchemy/src/compiler/routing.rs`; TLA enables its reconstructed `-mgs2`
lowering. A whole source file may use other flags when the original evidently
differed; record the reason beside the route. Code the ROM copies into RAM and
runs in ARM state uses `-marm -mno-apcs-frame`.

```sh
git submodule update --init && git config core.hooksPath .hooks
make bootstrap       # pinned agscc/agbcc and binutils under tools/out
make compare         # TBS: rebuild what changed, check rom.sha1 (compare-tla, compare-all)
make test            # tooling unit tests (test-integration reads local ROMs)
make coverage        # progress figure and the README progress line
make verify          # the landing gate: both ROMs, owners, publication, documents
```

Everything caches by content, so one edited file rebuilds alone in seconds.
Score with `tools/out/cargo-target/release/alchemy`, which skips the cargo
rebuild that `./alchemy` does. A byte-identical build writes the receipt DONE
reads; until the current tree has one, DONE is `?`.

Assets are editable files the build converts, tracked in pret's shape: one
indexed PNG per asset with its real palette, one identified BIN per tilemap or
table, and WAV, MIDI, JSON and PO. The grey sheets (`CHAR_COMMON.PNG`,
`TILE_BANK.PNG`), whole-area BIN bundles and `DATA.BIN` are dumps, restored
from your own ROM through `recon/<game>/private-inputs.json` until each is
split. Compressed streams the encoder does not yet reproduce keep a per-file
option recorded beside the file, or remain stored until it does.

## Pascal's decisions

- 2026-09-22: frozen compression answers at commit `d08ee3a2` may stay until
  the encoder replaces them; build-machine facts may be recorded as observed
  host facts in `recon/<game>/machine.json`.
- 2026-09-23: fake matches count, tagged. The six stored TLA streams stay. ARM
  code runs through the `-marm -mno-apcs-frame` route. Private inputs appear
  only in the local dashboard.
- 2026-09-24: the main-image far-call stub tables, built from the overlay
  veneer macro with whole aligned 8-byte entries, count as reconstructed
  veneers, as the overlay entry veneers already do.

## Tooling index

Prefer existing commands and read `--help` before scripting around one.
**Alchemy builds, Psynergy reads:** Alchemy owns game policy, compilation,
encoding, linking and verification; Psynergy owns portable reading, decoding,
analysis and comparison over explicit input, with no Golden Sun defaults. New
tooling must fix a demonstrated recurring blocker and carry a test. The
dashboard and both README figures share one design, Weyard UI, whose palette,
corner step and bevel opacity live in `tools/alchemy/src/coverage/palette.rs`.

| Tool | Responsibility |
| --- | --- |
| [alchemy](tools/alchemy/) | Golden Sun commands: `inspect` (owners, `--asm`, `--siblings`), `extract`, `score` (owner or `--unit`, `--all-instances`; `--variants DIR`, `--diff`, `--dump FLAGS`), `targets` (every not-yet-C owner by size), `overlay trial` (open an overlay owner: unit, placeholder, first draft; `--undo`), `overlay try` (score and rank candidates for a trial; `--install`), `overlay draft` (commit a trial as a recon draft with its residual), `adopt` and `overlay adopt` (adopts an open trial in place and retires its evidence rows and draft), `check integrate` (main-image adoption), `land` (merge, prove and squash-commit lane branches), `unit`, `raw rebuild`, `build` (`full`, `assets`, `allocator`), `coverage`, `check` (publication, owners, siblings, progress), `verify` (the landing gate, one line per gate), `cross-edition`, `dashboard`, `format`, `bootstrap`. |
| [psynergy](tools/psynergy/) | Portable commands over explicit files: `decompile`, `disassemble`, `decode-lz`, plus the Thumb decoder, C recovery, comparison, twin search and the image, sound, text and LZ codecs. |

## Open work

- ☀️ to 75%: about 163,000 bytes. The largest remaining pools are main-image
  owners registered as fragments (make them whole, then hand-write), the
  remaining unadopted overlay owners, and large registered main owners.
- ⚓️: most TLA functions resemble their TBS counterparts but only about 12 KB
  compiles unchanged under the TLA route; prove twins module by module
  (`alchemy check siblings --common`) and move shared code to `games/COMMON`.
- Split the asset dumps into pret-shaped assets; replace stored compression
  streams with per-file encoder options.
- Map viewer: group places by world position; show story variants one at a
  time ("Vale: Stormy"). Music: restore the synth voices of the player retired
  on September 10.
- Later, as pokeemerald did: a `MODERN` build with a current compiler and
  `BUGFIX` switches, for the recompilation.
