# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the English release of
*Golden Sun* for the Game Boy Advance. The goal is a repository of C and
independently described assets that rebuilds the released ROM byte for byte.

This is the only guide. `AGENTS.md` and `CLAUDE.md` are symlinks to it, and
`README.md` is the one other page, kept for end users. Please do not add new
`.md` or `.txt` files; a gate enforces it.

A change counts when the rebuilt bytes equal the released ROM. Everything else
— readable C, better names, faster tools — is worth doing, and is not the same
thing.

## Contents

- [Provenance and copyright](#provenance-and-copyright)
- [Setting up](#setting-up)
- [The working method](#the-working-method)
- [Source style](#source-style)
- [Tools](#tools)
- [Build stages](#build-stages)
- [The compiler standard](#the-compiler-standard)
- [Before you commit](#before-you-commit)
- [Targets](#targets)

---

## Provenance and copyright

*Golden Sun* and its original material are copyright Nintendo and Camelot
Software Planning. Alchemy is not affiliated with or endorsed by either
company. It is a decompilation and preservation effort — not a remake, a ROM
hack, an emulator, or a game distribution. No ROM is distributed here.

### The evidence boundary

Game-specific knowledge may come only from the approved local ROM set and from
your own reconstruction work against it:

```
gs1-{en,ja,de,es,fr,it}.gba
gs2-{en,ja,de,es,fr,it}.gba
```

`gs1-en.gba` is the build target. The other editions may be compared locally to
tell shared engine code and data apart from edition-specific content. A
cross-edition match is evidence about layout or behaviour — never about
authorship or an original name.

Do not consult any other Golden Sun checkout, repository history, source,
symbol list, pseudocode, script, generated output, or notes. Public
decompilation projects may inform generic repository, build, testing and
publication conventions; their game code, labels, assets and game knowledge may
not be used. Generic tools and public architecture or compiler documentation
are fine.

### The publication boundary

Publishable: reconstructed C and assembly, tools, semantic metadata,
deterministic encoder descriptions, and the source assets the build consumes.

Never commit or transmit: an approved ROM, a built ROM, raw ROM excerpts,
copied ROM gaps, binary patches, cross-ROM binary diffs, private analysis
output, compiler or disassembler output, toolchains, object files, ELFs, build
products, or credentials. Never send private ROM content or generated
artifacts to a network tool.

`check_publication` enforces the file-shape half of this on every staged commit
and on everything you push, including content added and later deleted within
the outgoing range. It cannot tell where knowledge came from — that part is
yours to honour.

### Hard rules

- No `asm(...)`, `__asm(...)`, fixed-register bindings, empty assembly
  barriers, copied ROM bytes, or any equivalent escape hatch in C or headers.
  `no_asm_c` rejects them. Byte equality never overrides this.
- Reconstruction assembly is publishable source, but it is not a
  decompilation. A region counts as decompiled only when its C compiles to
  identical bytes.
- Cite the tool that derived a fact when it matters; `source_citations` checks
  that live citations name a path that exists.
- A citation must name the tool's current path. The table below is the only
  exception; do not repoint a citation at a different tool that happens to
  survive, because that would claim a derivation which never happened.
- Preserve unrelated work in a dirty checkout. Inspect the working tree before
  editing and keep unrelated changes out of your commit.

### Tools cited by sources but no longer present

A reconstructed source cites the tool that derived a fact. Some of those tools
have since been replaced, and the citation is still the truthful record of how
the fact was obtained, so it stays. This table says where to find the tool.

| Cited tool | Last commit containing it | Recover with |
|---|---|---|
| `tools/m2c_guard.ts` | `f185d7346` | `git show f185d7346:tools/m2c_guard.ts` |
| `tools/main_xref.ts` | `f185d7346` | `git show f185d7346:tools/main_xref.ts` |
| `tools/overlay_dispatch_sites.ts` | `f185d7346` | `git show f185d7346:tools/overlay_dispatch_sites.ts` |
| `tools/veneer_resolve.ts` | `f185d7346` | `git show f185d7346:tools/veneer_resolve.ts` |
| `tools/overlay_driver.ts` | `87d03abf0` | `git show 87d03abf0:tools/overlay_driver.ts` |
| `tools/overlay_unindexed.ts` | `87d03abf0` | `git show 87d03abf0:tools/overlay_unindexed.ts` |

Activate the hooks and the generated-file merge driver once per clone:

```bash
git config core.hooksPath .hooks
git config merge.generated.driver true
```

`pre-commit` runs the publication gate over the staged change, `commit-msg`
validates the progress prefix in your subject, and `pre-push` re-runs the
publication gate over every outgoing commit.

The second line resolves `.gitattributes`'s generated artifacts by keeping your
side of a merge, because neither side is correct once both branches have adopted
owners. Regenerate afterwards; `make verify` fails on a stale artifact, so a
forgotten regeneration cannot reach a commit. Without the line git falls back to
an ordinary conflict, which is safe and merely tedious.

---

## Setting up

You need a Rust toolchain, `arm-none-eabi-binutils`, Python 3, and the
approved ROM set in `roms/` (gitignored — put it there yourself).

The compiler lives in the `alchemy-gcc` submodule and its built binaries are
expected at `alchemy-gcc/dist/`. In a **git worktree** the submodule directory
is empty; link the staged compiler in rather than making `alchemy-gcc` itself a
symlink, which git refuses:

```bash
ln -s /path/to/main/checkout/roms roms
mkdir -p alchemy-gcc && ln -s /path/to/main/checkout/alchemy-gcc/dist alchemy-gcc/dist
```

Do not run `git submodule` commands inside a worktree — they can rewrite the
shared configuration and break the main checkout.

Then confirm the tree is healthy:

```bash
make build-claimed   # links every byte-exact owner
make verify          # the authoritative gate
```

---

## The working method

The method is the ordinary decompilation loop: read the target assembly, work
out what C the original author wrote, write that C, compile it, compare the
bytes, and fix the difference. Repeat until identical.

What this repository adds is **feedback**. You are not diffing by eye. The
tools compile your candidate, link it at its real address, compare it against
the reference bytes, and tell you how far off you are and in what way. A gate
then refuses to adopt anything that does not reproduce. Use that loop tightly:
edit, score, read the residual, edit again.

### 1. Pick an owner

An *owner* is one function-sized region with a fixed address.

- Main image: assembly in `asm/<address>.s`, C in `exact/<address>.c` once it
  matches, `semantic/<address>.c` while it does not.
- Code overlays: assembly in `assets/code/resource_<id>_overlay.s`, C in
  `exact/resource_<id>_c_<address>.c` or `semantic/…` respectively.

`semantic_queue` ranks candidates that already have reviewed C. `overlay_twins`
finds owners that mirror one you have already finished, which is usually the
cheapest next thing to do. [Targets](#targets) lists every unfinished scope,
largest first.

### 2. Read the assembly before writing any C

C written before you understand the target is guesswork with extra steps. A
decompiler is a reading aid; its output is a scaffold to check against the ROM,
never a source draft. Get these facts first:

- **Boundaries and entry.** Prologue shape, frame size, which registers are
  saved, stack slots and arguments, and whether a shared tail is reached from
  elsewhere.
- **Types and signedness**, read off the load widths. `ldrb`/`ldrsb`/`ldrh`/
  `ldrsh` are declarations, not hints. A `lsls`/`asrs` pair is a sign
  extension the source asked for.
- **Control flow.** Loop heads and bottoms, switch and jump-table layout,
  branch cascades, fall-through seams.
- **Aggregate layout.** Which offsets cluster around which base registers, and
  which cells are read through one pointer and written through another.
- **Constants.** Which live in the literal pool and which are built inline. A
  pooled word and an inline `movs` are different source spellings.
- **Calls and side effects**, and every value's lifetime across them.

`overlay_disasm`, `overlay_show` and `overlay_entry` decode overlay bytes;
`discover` and `remaining_survey` map what is left.

### 3. Write the compiler's input, not its output

The ROM is optimised compiler output. The C you want is what went *in*: plain
structs, arrays indexed by loop variables, ordinary expressions, natural
statement order. The optimiser then reproduces the ROM's shape by itself,
because that is exactly what it did the first time.

The common mistake is transcribing the optimiser's work back into the source —
hand-strength-reduced pointer walks instead of `arr[i]`, hand-shared
temporaries instead of repeating an expression, hoisted invariants. That does
not lock in the ROM's shape; it changes what the earlier passes see, and the
result diverges somewhere else.

The bound runs both ways: write machine-producing structure, not modern style.
Prefer the shape a 2001 author would have written.

### 4. Score it and read the residual

```bash
# a main-image owner
cargo run --release --manifest-path tools/compiler/Cargo.toml -- \
  candidate-show semantic/080a1234.c --align

# an overlay row -- same output, same flag, no span argument
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  score semantic/resource_373_c_0200034c.c --align

# a decomposed view, with the residual classified
make dispatch-decomp ARGS='decomp_diagnose semantic/080a1234.c'
```

**Read the diff, not the number.** `candidate-show` prints the two instruction
streams side by side; the summary line is a progress indicator, not a
diagnosis. `--align` pairs the streams as sequences, so an extra or missing
instruction shows up as itself:

```
  + push  {lr}                          <- we emit this, the reference does not
  ! ldr   r3, [pc, #20]    ldr r0, ...  <- both have it, operands differ
    ldr   r3, [r0, #0]     ldr r3, ...  <- identical
```

Without `--align` the two sides are matched by offset, which only works while
they are the same length: one extra instruction shifts everything after it and
every later row reads as a difference. The tool warns you when that applies.

Read the result in this order:

- **Wrong size** — something is missing or extra. `+` and `-` rows name it
  exactly: a statement, a pool word, an unwanted sign extension, a prologue
  that saves a register the reference does not. Fix that before anything else.
- **Right size, differing operands** — the shape is right and the difference is
  local. `!` rows are usually a type, a callee prototype, or the order two
  values were computed in.

The summary count is only comparable between candidates of the same size. Once
sizes differ, the shift reshuffles which halfwords happen to line up and the
number stops meaning anything — which is why the diff is the instrument and the
count is not.

Then make the smallest source change that explains what you saw, and score
again.

### 5. Adopt

Adoption is gated. It rebuilds and compares before it accepts anything.

```bash
# main image: proves a src_<address>.c draft, installs to exact/
make dispatch-decomp ARGS='integrate_matches /path/to/draft --apply'

# overlay: rehearses the whole overlay, then splices the row in
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  adopt resource_373:034c --source semantic/resource_373_c_0200034c.c --span 320 --apply
```

An overlay row is spliced into a fixed-size hole, so the gate checks the region
boundary, that no label outside the region is destroyed, and that the whole
overlay still assembles to the same bytes. Copying a file into `exact/` is not
adoption and will not survive the build.

**Run one `adopt` at a time.** It reaches that verdict by splicing the row into
`assets/code/<overlay>.s` and restoring the file afterwards, so it writes to the
tree even when it rejects and even without `--apply` — `--where` is not a
read-only flag. Twelve rehearsals in parallel raced on those files and left
seven overlays with real assembly replaced by `.space` placeholders, 368 lines
gone, while every single rehearsal run beforehand had been clean. `git status`
after a batch is the cheap check; `git checkout -- assets/code/` is the repair.
Score in parallel as much as you like, and keep adoption serial.

The inverse exists, and you should use it rather than leaving a broken row in
place:

```bash
cargo run --release --manifest-path tools/overlay/Cargo.toml -- park resource_373:034c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- audit --all
```

`park` restores a row's assembly and moves its C to `semantic/`. `audit`
compares every adopted row against the bytes it replaced and names any that no
longer reproduce.

### Merging another branch, or main back into yours

Every branch that adopts an owner rewrites the same generated artifacts, so a
merge between two working branches conflicts on all of them and neither side is
right afterwards. Do not resolve them by choosing:

```bash
git merge main
# generated artifacts resolve themselves; CONTRIBUTING.md and README.md may not
make coverage      # regenerate the Targets section, figures and metrics
make verify        # refuses a stale artifact, so this is the proof
overlay audit --all
```

`CONTRIBUTING.md` and `README.md` still conflict, in the Targets counts and the
figure hashes. Take either side there and let `make coverage` correct it; the
prose around them is hand-written and merges normally, which is exactly why
those two are not auto-resolved.

Run `overlay audit --all` after a merge that brought in adoptions from both
sides. It compares every adopted row against the bytes it replaced, which is the
one check that a merged tree has not silently broken an owner neither branch
touched.

### 6. When an owner will not converge

Move it to `semantic/`, take another owner. A main-image owner moves by putting
its C at `semantic/<address>.c`; the build falls back to `asm/<address>.s`
automatically. An overlay row must be parked with the command above, because
its assembly has to come back into the overlay.

Understanding accumulates: every owner you finish becomes a worked example of
what correct source looks like for this compiler, and the next one is easier
because of it. `exact_reading_list` and `overlay_twins` are how you find the
relevant examples.

---

## Source style

### Naming

Short, period-appropriate names, in the style of Japanese console games of the
era: `Flg`, `Ev`, `Btl`, `Chr`, `Tbl`, `Work`, `Pos`, `No`, `Get`, `Set`, and
romanised asset names where the meaning is established.

Keep a name opaque while the meaning is unproven. `Func_`, `Data_`,
address-based names and `unknown_` are better than a confident guess. Three
levels of confidence:

- unknown behaviour → keep `Func_`, `Data_` and raw field offsets
- demonstrated behaviour → a cautious descriptive name
- established identity → a real name

Once local evidence establishes what a function does, keep its relocation name
at the ABI boundary and use a descriptive name in the body:

```c
extern u8 *Func_0200538a(s32 actor);
#define GetSceneActor Func_0200538a
```

These are reconstruction aids, not claims about the original identifiers.
Renaming locals and aliasing callees this way is byte-neutral, so do it freely
— but run the owner's comparison anyway.

### Types and structure

Prefer the simplest shape the evidence supports. Keep uncertain fields, casts,
aliases, signedness and control flow explicit until they are proved. Risk lives
in types, scoping and control flow, not in spelling: a small change to a type
or an alias can move register allocation and break an exact match. Do not
retype or re-scope a source to make it look modern.

Narrow types are usually the wrong reading. The store width comes from the
pointer cast or the struct member, not from the variable, so a `u8` local that
the reference never truncates costs an extension the ROM does not have. Widen
by default and narrow where the bytes show a truncation.

A callee's declared return type is part of the interface and is visible in the
bytes: a non-void return keeps a value live across the caller's argument setup.
If your arguments are set up in the wrong order, check the prototype before you
suspect the allocator.

Fixed addresses touched more than once should be declared objects, not
`(void *)` literals, so the base stays in a register:

```c
extern s16 Data_02000240[];
```

A global the reference re-reads on a path where the value provably cannot have
changed is a `volatile` object, and nothing else in ordinary C produces that
reload. The tell is small and easy to misread: the whole owner matches except
one conditional branch whose target is a single instruction earlier than yours,
because your branch was threaded past a load the compiler knew was redundant.
Qualify the declaration, the pointer and any alias to it together.

A candidate exactly four bytes short of its reference is usually missing one
pool word. Either a constant is spelled as a literal where the reference links
a symbol, or the value is dead in your source and the compiler dropped it --
check for a variable that is assigned in one branch and read in another before
reaching for `Value_<addr>`.

### Comments

Brief, factual, and about things a future contributor needs: relationships
between values, invariants, hardware constraints. Canonical comments are short
Japanese UTF-8 lines matching the naming convention; English is fine where it
makes a technical constraint clearer.

```c
/* マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。 */
```

Do not use comments as a speculation diary, and do not claim to have recovered
an original identifier the evidence does not support.

---

## Tools

Everything runs through nine dispatch groups:

```bash
make dispatch-<group> ARGS='<command> [args]'

# or directly
cargo run --release --manifest-path tools/dispatch/Cargo.toml -- <group> <command> [args]
```

`<group> --list` enumerates a group; `<group> <command> --help` gives the
contract. Commands that change the tree require an explicit `--apply` or
`--write`; everything else is read-only.

### decomp — find owners and prove drafts

| Command | What it tells you |
|---|---|
| `discover` | Produces the function, instruction and call discovery report from a local ROM. |
| `remaining_survey` | Surveys the executable regions still unresolved. |
| `decomp_diagnose` | Compiles a main-image candidate and classifies its residual; `--agent-brief` emits a bounded work contract with the canonical score. |
| `integrate_matches` | The main-image adoption gate: proves a `src_<address>.c` draft and installs it only with `--apply`. |

### overlay — the 96 loaded code modules

| Command | What it tells you |
|---|---|
| `overlay_disasm` | Decodes overlay bytes to assembly. |
| `overlay_show` | Shows a byte range of one overlay, optionally annotated. |
| `overlay_entry` | Reports overlay entry points. |
| `overlay_inventory` | Rebuilds the diagnostic inventory that overlay tools and semantic compilation read. |
| `overlay_gaps` | Names unclaimed executable ranges. |
| `overlay_unindexed` | Names executable spans discovery has not indexed. |
| `overlay_twins` | Finds owners that mirror one another, so a solved shape can be reused. |
| `exact_reading_list` | Lists finished owners worth reading as worked examples. |
| `overlay_candidate_rank` | Ranks candidate residuals. |
| `overlay_call_order_check` | Compares resolved call order, including aliases and relocation spellings. |
| `overlay_certify` | Reports owner-certification findings; `--check` turns them into a failing gate. |
| `overlay_adopt` | The overlay adoption gate: rehearses the whole overlay and refuses a mismatch. |
| `overlay_showcase` | Renders a representative finished overlay for regression and demonstration. |
| `overlay_driver` | The low-level overlay reconstruction driver. |
| `overlay_mode_cohort` | Compares one compiler hypothesis across a set of overlay owners. |

The `overlay` binary also carries `score`, `park` and `audit`, described in
[the working method](#the-working-method). `score` is the overlay counterpart
of `candidate-show`: same output, same `--align`, and it derives the row's span
rather than asking you for one.

### semantic — reviewed C that is not yet exact

| Command | What it tells you |
|---|---|
| `semantic_queue` | Ranks current candidates, ignoring resolved blockers. |
| `semantic_owner_scope` | Verifies a source covers its complete audited owner. |
| `semantic_superseded` | Finds sources made obsolete by an adoption. |

Passing these does not make a source an exact claim. `semantic/regions.json`
records the reviewed boundary for an owner discovery did not index;
`semantic/sealed.json` holds the small set withdrawn from routine work.

`exact/provisional.json` is the opposite register: owners whose bytes are final
but whose source is not. Add an entry when you got the bytes by writing toward
them rather than by reconstructing — a constant respelled as a symbol to force a
pool word, a duplicated local to defeat common subexpression elimination, a
prototype changed for its effect on register allocation rather than because the
interface says so.

The match still counts and the build is still correct; the bytes are right. What
the entry withdraws is the source's standing as *evidence*. `exact/` is the
corpus every later owner is compared against, so a source that was contrived to
match teaches a shape the compiler never asked for. Empty is the goal, and a
growing list means matches are being bought rather than earned.

### search — bounded source and compiler exploration

| Command | What it tells you |
|---|---|
| `decomp_constraints` | Derives bounded structural constraints for a candidate. |
| `shape_sweep` | Tries bounded, behaviour-preserving source shapes with the compiler fixed; `--descend` drives it iteratively. |
| `search_compiler_modes` | Searches approved compiler modes with the source fixed. |
| `alchemy_permuter` | A bounded source-permutation search with linked-byte scoring. |

These hold one axis fixed and report what changed. They are a last resort for a
residual you have already localised and understood, not a way to find one. A
result still has to be read as source and adopted through the owner's gate.

### compiler — routed builds and comparisons

| Command | What it tells you |
|---|---|
| `compiler_corpus_regression` | Recompiles the exact corpus and reports any byte regression. |
| `mode_sweep` | Searches the approved flag matrix for one fixed candidate. |
| `mode_cohort` | Tests one compiler hypothesis across a bounded set of owners. |

The `compiler` binary additionally hosts `candidate-show` (size, reference size
and differing halfwords for a candidate), `thumb-disasm`, and the RTL readers
`rtl-insn`, `rtl-sexpr`, `rtl-schedule` and `rtl-align`, which read the
compiler's own dumps when you need to know which pass produced a shape.

### check — the gates

| Command | What it tells you |
|---|---|
| `architecture` | Requires every crate, binary, dispatch target and path to be valid, reachable and catalogued here. |
| `documented` | Keeps the command groups and this catalog in step with the registry. |
| `check_publication` | Rejects ROMs, build products, opaque dumps, credentials and disallowed files. |
| `no_asm_c` | Rejects inline assembly, register pins and assembly barriers in C and headers. |
| `source_citations` | Checks that cited tools name paths that exist. |
| `check_unmatchable` | Validates the two owner registers -- unmatchable and provisional -- and prints the queue with `--queue`. |
| `core_retained_audit` | Requires retained main-image assembly to carry explicit justification. |
| `cache_key_lint` | Requires cached results to include every input that can change their meaning. |
| `check_commit_progress` | Compares your commit subject against the staged progress report. |

A check that scans nothing fails. A zero exit means the corpus was inspected.

### metrics — measurement and charts

| Command | What it tells you |
|---|---|
| `full_c_progress` | Calculates, checks or writes the audited byte-exact report. |
| `full_c_history` | Writes the first-parent progress ledger. |
| `coverage_map` | Checks or regenerates the dashboard, the target index, the charts and the README figures. |
| `audit_residuals` | Accounts for every remaining executable byte and its ownership class. |
| `compare_roms` | Compares approved local editions without publishing byte diffs. |
| `dashboard_server` | Serves a live worktree dashboard on localhost. |

`full_c_progress --check` refuses to certify a share unless the last full build
reproduced the ROM byte-identically with no fallback bytes. Ownership is a
claim; a percentage over a failing build is not evidence.

### assets — extraction and round-tripping

| Command | What it tells you |
|---|---|
| `extract_resource` | Decodes one resource by id or address and can verify the round trip. |
| `export_asset` | Renders binary graphics, palette, byte and RGBA forms into source assets. |
| `import_asset` | Converts PNG, indexed-image and MIDI sources into build inputs. |
| `tilemap` | Round-trips the textual tilemap format. |
| `bl_site_symbols` | Recovers and audits branch-and-link symbol evidence, reporting every failure. |

### make — build stages and asset builders

| Command | What it tells you |
|---|---|
| `build_claimed` | Links the byte-exact owners. |
| `build_asm` | Rebuilds the assembled stage. |
| `build_assets` | Rebuilds the asset tree. |
| `build_semantic` | Compiles the semantic sources. |
| `build_full` | Composes exact C, retained assembly and assets, and compares against the ROM. |
| `build_rom` | Rebuilds the ROM. |
| `archive_asset` | Rebuilds generic archive containers. |
| `audio_engine_data` | Rebuilds the audio engine data package. |
| `audio_wave` | Rebuilds audio samples. |
| `music` | Rebuilds sequences. |
| `music_residuals` | Rebuilds residual audio ownership. |
| `battle_effect_data` | Rebuilds the battle-effect data package. |
| `sentou_gamen_data` | Rebuilds battle screen data. |
| `sentou_hyouji` | Rebuilds battle display data. |
| `sentou_kouka_runtime` | Rebuilds and verifies the battle-effect runtime. |
| `sentou_menu_data` | Rebuilds battle menu data. |
| `sentou_resources` | Rebuilds battle resources. |
| `encounter_data` | Rebuilds encounter data. |
| `character_catalog` | Rebuilds the character catalogue. |
| `localization_font` | Rebuilds the localisation font. |
| `localization_tables` | Rebuilds the localisation tables. |
| `namae_nyuuryoku` | Rebuilds the name-entry data. |
| `staff_roll` | Rebuilds the credits data. |
| `title_resources` | Rebuilds title-screen resources. |
| `chiiki_map_resources` | Rebuilds regional map resources. |
| `tokushu_map_resources` | Rebuilds special map resources. |
| `kind1_map_grid` | Rebuilds map grids. |
| `map_container_components` | Rebuilds map container components. |
| `early_runtime_data` | Rebuilds early runtime-support data. |
| `runtime_support_data` | Rebuilds runtime-support data. |
| `late_runtime_residual` | Rebuilds late runtime residuals. |
| `executable_gap_sources` | Rebuilds or classifies executable-gap inputs. |
| `gba_header` | Rebuilds the cartridge header. |
| `f0_archive` | Rebuilds the F0 archive format. |
| `message_archive` | Rebuilds message archives. |
| `wordstream` | Rebuilds word streams. |
| `pairtable` | Rebuilds pair tables. |
| `byte_henkan` | Rebuilds byte-conversion tables. |
| `byte_value_regions` | Rebuilds byte-value regions. |
| `indexed_still` | Rebuilds indexed still images. |
| `static_sprite_series` | Rebuilds static sprite families. |
| `skip_sprite_archive` | Rebuilds skipped sprite archives. |
| `simple_resources` | Rebuilds resources with a simple plan. |
| `resource_directory` | Rebuilds the resource directory. |
| `resource_01c` | Rebuilds resource 0x01c. |
| `resource_5` | Rebuilds resource 5. |
| `resource_3ce` | Rebuilds resource 0x3ce. |
| `resource_d1_d3` | Rebuilds resources 0xd1–0xd3. |
| `resource_byte_canvases` | Rebuilds byte-canvas resources. |

---

## Build stages

Use the smallest stage that answers your question.

| Target | What it does |
|---|---|
| `make build-claimed` | Links the byte-exact owners. Fast; answers "does my owner still link and match?" |
| `make build-asm` | Rebuilds the assembled stage. |
| `make inventory` | Produces the overlay inventory that overlay tools and `make build-semantic` need. |
| `make build-semantic` | Compiles the semantic sources, checking each has an admitted owner. |
| `make build-assets` | Rebuilds the asset tree. |
| `make build-full` | Composes everything and compares against the ROM. |
| `make build-rom` | Rebuilds the ROM image. |
| `make progress` | Prints the byte-exact share. |
| `make coverage` | Refreshes the dashboard, the target index and the charts. |
| `make test` | Lint plus every native self-test. |
| `make lint` | Architecture and policy gates. |
| `make verify` | The authoritative gate. |

`make verify` is green only when the ROM rebuilds byte-identically with no
fallback bytes, every gate passes, and the tracked metrics match the tree. That
is the only result that proves anything.

---

## The compiler standard

Camelot shipped a makefile, not a per-file flag database. The build targets the
same shape: one flag set, plus a small number of sanctioned deviations.

```
-O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi
-fno-builtin -nostdinc -ffreestanding -fcall-used-r4 -Iinclude
```

The deviations are: one overlay stem that never interworks and so needs no
veneers; a small number of translation units built at `-O1`; and the stock m4a
audio engine, which is a different compiler rather than different flags.

Anything a source needs beyond this is **debt**, not configuration. It records
that the reconstruction is wrong and a flag is standing in for the fix.
`make standard-check` asserts that the documented flag set is the one the build
uses, and `make routing-debt` reports how many sources still deviate.

Do not add a compiler option that stock gcc 2.96 does not have. A byte match
reached by inventing an option is not a reconstruction; it moves the difference
out of the source, where it can be found, and into the compiler, where it
cannot. Adding a *stock* option is allowed and is recorded as debt.

Never resolve the routing tables or the fork's ARM backend by taking one side
of a merge. Three-way merge and review each entry.

---

## Before you commit

Regenerate metrics after any change to executable sources, then run the full
gate:

```bash
make verify
```

The commit subject starts with the generated progress prefix:

```
☀️ N% – description
```

`N` is the nearest whole byte-exact percentage from the staged report — not the
public `DONE` figure. `make progress-subject` prints it. The `commit-msg` hook
checks it.

Agent-authored commits use the agent's own identity
(`Claude <noreply@anthropic.com>`, `Codex <noreply@openai.com>`); never a human
identity as the default author for a generated commit.

A deliberate regression or a denominator correction is fine, and must be
described plainly.

---

## Targets

This section is generated. It is the primary contributor target list:
non-overlapping audited source-owner scopes (or contiguous unresolved
executable runs), sorted largest to smallest. Broader multi-owner campaign cuts
belong in [Status](#status); they may overlap and therefore are not used for
byte accounting. Regenerate with `make coverage` -- do not edit by hand.

- **Unfinished scopes:** 2,132
- **Address spaces scanned:** 97 (87 still contain targets)
- **Target bytes:** 996,094 semantic-C or unresolved-assembly bytes
- **Resolved-only bytes:** 347,500 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,347,122

### Main target list

This table contains every scope of at least 1,000 bytes (227 rows). The complete
2,132-row index, including the smallest audited owners, is
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json).

| Rank | Scope | Target | Namespace / owner |
|---:|---:|---:|---|
| 1 | 7,468 | 7,468 | `resource_3b8:0x02002014` |
| 2 | 7,356 | 7,356 | `resource_38f:0x020008ec` |
| 3 | 6,700 | 6,700 | `resource_391:0x02000d3c` |
| 4 | 6,332 | 6,332 | `main:0x080bbb0c` |
| 5 | 6,220 | 6,220 | `resource_3bd:0x020013f8` |
| 6 | 5,932 | 5,932 | `resource_380:0x020027f8` |
| 7 | 5,756 | 5,756 | `main:0x080ea0d8` |
| 8 | 5,604 | 5,604 | `resource_373:0x02003fb0` |
| 9 | 5,604 | 5,604 | `resource_3bf:0x02003054` |
| 10 | 5,240 | 5,240 | `resource_373:0x020015dc` |
| 11 | 5,136 | 5,136 | `resource_381:0x02001410` |
| 12 | 5,000 | 5,000 | `resource_39e:0x02002ad0` |
| 13 | 4,888 | 4,888 | `main:0x080ab5e4` |
| 14 | 4,840 | 4,840 | `resource_39d:0x02001af0` |
| 15 | 4,708 | 4,708 | `resource_3c9:0x02002360` |
| 16 | 4,268 | 4,268 | `resource_3aa:0x02000360` |
| 17 | 4,224 | 4,224 | `main:0x08027114` |
| 18 | 4,080 | 4,080 | `resource_378:0x0200088c` |
| 19 | 4,064 | 4,064 | `resource_3a8:0x02000590` |
| 20 | 4,044 | 4,044 | `resource_39d:0x02000b24` |
| 21 | 3,804 | 3,804 | `main:0x080f6440` |
| 22 | 3,796 | 3,796 | `resource_383:0x020036f8` |
| 23 | 3,790 | 3,790 | `resource_38d:0x020008c0` |
| 24 | 3,656 | 3,656 | `main:0x080dea70` |
| 25 | 3,640 | 3,640 | `main:0x080e7404` |
| 26 | 3,604 | 3,604 | `resource_3c9:0x020012c8` |
| 27 | 3,548 | 3,548 | `resource_381:0x02000054` |
| 28 | 3,400 | 3,400 | `resource_378:0x0200187c` |
| 29 | 3,384 | 3,384 | `main:0x080d1714` |
| 30 | 3,320 | 3,320 | `main:0x08023178` |
| 31 | 3,308 | 3,308 | `main:0x080d6970` |
| 32 | 3,128 | 3,128 | `main:0x080a2680` |
| 33 | 3,104 | 2,810 | `main:0x080ad6d4` |
| 34 | 3,048 | 3,048 | `resource_395:0x02000488` |
| 35 | 2,968 | 2,968 | `main:0x080ca60c` |
| 36 | 2,816 | 2,816 | `resource_374:0x020017c8` |
| 37 | 2,792 | 2,792 | `resource_373:0x020034c8` |
| 38 | 2,756 | 2,756 | `main:0x08023e70` |
| 39 | 2,756 | 2,756 | `resource_3a8:0x020026c0` |
| 40 | 2,716 | 2,716 | `resource_372:0x020031ac` |
| 41 | 2,710 | 2,710 | `resource_3c9:0x02004bec` |
| 42 | 2,664 | 2,664 | `resource_376:0x02000658` |
| 43 | 2,628 | 2,628 | `resource_379:0x02000074` |
| 44 | 2,532 | 2,532 | `resource_3c7:0x02000508` |
| 45 | 2,508 | 2,508 | `main:0x080cbc0c` |
| 46 | 2,508 | 2,508 | `resource_3c9:0x020008b4` |
| 47 | 2,448 | 2,448 | `resource_3ad:0x02000828` |
| 48 | 2,444 | 2,444 | `main:0x080eb754` |
| 49 | 2,428 | 2,428 | `main:0x0808c4f8` |
| 50 | 2,396 | 2,396 | `resource_3c5:0x02001b10` |
| 51 | 2,388 | 2,388 | `main:0x080f7460` |
| 52 | 2,372 | 2,372 | `main:0x080dc968` |
| 53 | 2,356 | 2,356 | `main:0x080d2464` |
| 54 | 2,354 | 2,354 | `resource_371:0x02000c28` |
| 55 | 2,352 | 2,352 | `resource_370:0x020003cc` |
| 56 | 2,316 | 2,316 | `main:0x0808f52c` |
| 57 | 2,300 | 2,300 | `main:0x08021e6c` |
| 58 | 2,268 | 2,268 | `main:0x080d91dc` |
| 59 | 2,264 | 2,264 | `resource_3c5:0x02001238` |
| 60 | 2,250 | 2,250 | `resource_3c6:0x020007c4` |
| 61 | 2,236 | 2,236 | `resource_39e:0x02001494` |
| 62 | 2,212 | 2,212 | `resource_3bf:0x020027b0` |
| 63 | 2,204 | 2,204 | `resource_3b1:0x02001f3c` |
| 64 | 2,188 | 2,188 | `main:0x080e302c` |
| 65 | 2,168 | 2,168 | `resource_3bd:0x02002c44` |
| 66 | 2,138 | 2,138 | `main:0x08026080` |
| 67 | 2,124 | 2,124 | `main:0x08024934` |
| 68 | 2,124 | 2,124 | `resource_3b7:0x02000e5c` |
| 69 | 2,092 | 2,092 | `resource_3b8:0x020017e8` |
| 70 | 2,072 | 2,072 | `resource_3b1:0x020027d8` |
| 71 | 2,068 | 2,068 | `resource_3c2:0x02000240` |
| 72 | 2,060 | 2,060 | `resource_38d:0x020019b0` |
| 73 | 2,036 | 2,036 | `resource_375:0x02000170` |
| 74 | 2,032 | 2,032 | `resource_3b8:0x02000ff8` |
| 75 | 2,024 | 2,024 | `main:0x08077428` |
| 76 | 2,024 | 2,024 | `main:0x080d89ac` |
| 77 | 1,980 | 1,980 | `main:0x080acab8` |
| 78 | 1,968 | 1,968 | `main:0x080e823c` |
| 79 | 1,932 | 1,932 | `resource_378:0x0200290c` |
| 80 | 1,920 | 1,920 | `main:0x080f7f78` |
| 81 | 1,876 | 1,876 | `main:0x0802592c` |
| 82 | 1,876 | 1,876 | `main:0x080bd898` |
| 83 | 1,864 | 1,864 | `main:0x080bae40` |
| 84 | 1,836 | 1,836 | `main:0x08025200` |
| 85 | 1,828 | 1,828 | `resource_383:0x02002fd4` |
| 86 | 1,816 | 1,816 | `main:0x080e99c0` |
| 87 | 1,816 | 1,712 | `main:0x08090a5c` |
| 88 | 1,804 | 1,804 | `resource_3b9:0x02001cd4` |
| 89 | 1,796 | 1,796 | `resource_3ca:0x02000430` |
| 90 | 1,796 | 1,664 | `main:0x080f3078` |
| 91 | 1,776 | 1,776 | `main:0x080dab74` |
| 92 | 1,768 | 1,768 | `main:0x080d52c8` |
| 93 | 1,764 | 1,764 | `main:0x080d4604` |
| 94 | 1,728 | 1,728 | `resource_375:0x02000be0` |
| 95 | 1,728 | 1,728 | `resource_3ae:0x02000e40` |
| 96 | 1,724 | 1,724 | `main:0x080e89ec` |
| 97 | 1,724 | 1,724 | `resource_3b9:0x02000710` |
| 98 | 1,720 | 1,720 | `main:0x080e2974` |
| 99 | 1,720 | 1,720 | `resource_3a8:0x02002008` |
| 100 | 1,712 | 1,712 | `main:0x080d5e54` |
| 101 | 1,704 | 1,704 | `resource_39e:0x02001dbc` |
| 102 | 1,692 | 1,692 | `resource_3c9:0x0200423c` |
| 103 | 1,688 | 1,688 | `main:0x080b63c8` |
| 104 | 1,680 | 1,680 | `main:0x080a6ccc` |
| 105 | 1,680 | 1,680 | `resource_37f:0x02000f8c` |
| 106 | 1,672 | 1,672 | `resource_387:0x0200067c` |
| 107 | 1,660 | 1,660 | `main:0x080de2f8` |
| 108 | 1,656 | 1,656 | `resource_37b:0x02000614` |
| 109 | 1,648 | 1,648 | `main:0x080ed408` |
| 110 | 1,640 | 1,640 | `main:0x0800aa0c` |
| 111 | 1,636 | 1,636 | `main:0x0800cacc` |
| 112 | 1,632 | 1,632 | `resource_3a2:0x020001ec` |
| 113 | 1,620 | 1,620 | `main:0x08018038` |
| 114 | 1,616 | 1,616 | `resource_39c:0x02002f68` |
| 115 | 1,612 | 1,612 | `main:0x080e40a4` |
| 116 | 1,608 | 1,608 | `resource_383:0x02001348` |
| 117 | 1,600 | 1,600 | `resource_396:0x02000918` |
| 118 | 1,592 | 1,592 | `main:0x080168f4` |
| 119 | 1,588 | 1,588 | `main:0x08022b44` |
| 120 | 1,576 | 1,576 | `main:0x080d2d98` |
| 121 | 1,574 | 1,574 | `resource_3ce:0x0200029c` |
| 122 | 1,556 | 1,556 | `main:0x080cf8e0` |
| 123 | 1,540 | 1,540 | `main:0x080e3aa0` |
| 124 | 1,512 | 1,512 | `main:0x080e4e0c` |
| 125 | 1,504 | 1,504 | `main:0x08012518` |
| 126 | 1,492 | 1,492 | `main:0x080cf2b8` |
| 127 | 1,488 | 1,488 | `resource_371:0x02001ca4` |
| 128 | 1,468 | 1,468 | `main:0x080dbc30` |
| 129 | 1,468 | 1,468 | `resource_399:0x02000f90` |
| 130 | 1,452 | 1,452 | `resource_3c6:0x02000218` |
| 131 | 1,440 | 1,440 | `resource_3aa:0x02001494` |
| 132 | 1,432 | 1,432 | `resource_3bc:0x02001474` |
| 133 | 1,412 | 1,412 | `resource_372:0x02001b18` |
| 134 | 1,408 | 1,408 | `main:0x08020244` |
| 135 | 1,400 | 1,400 | `resource_380:0x0200178c` |
| 136 | 1,400 | 1,400 | `resource_380:0x02001d04` |
| 137 | 1,388 | 1,388 | `resource_37a:0x02000488` |
| 138 | 1,380 | 1,380 | `resource_383:0x02000de4` |
| 139 | 1,376 | 1,376 | `main:0x0801a98c` |
| 140 | 1,360 | 1,360 | `main:0x080c91dc` |
| 141 | 1,340 | 1,340 | `main:0x080c9ca8` |
| 142 | 1,340 | 1,340 | `resource_399:0x020019e8` |
| 143 | 1,340 | 1,340 | `resource_3a8:0x02001930` |
| 144 | 1,336 | 1,336 | `main:0x080a9f10` |
| 145 | 1,332 | 1,332 | `main:0x080c972c` |
| 146 | 1,324 | 1,324 | `main:0x080bf678` |
| 147 | 1,308 | 1,308 | `main:0x080aa768` |
| 148 | 1,308 | 1,308 | `main:0x080e1040` |
| 149 | 1,308 | 1,308 | `resource_39c:0x020028b0` |
| 150 | 1,296 | 1,296 | `resource_371:0x020039fc` |
| 151 | 1,288 | 1,288 | `main:0x080e94b8` |
| 152 | 1,280 | 1,280 | `resource_3b8:0x02000af8` |
| 153 | 1,272 | 1,272 | `main:0x080b0aac` |
| 154 | 1,264 | 1,264 | `main:0x08099da4` |
| 155 | 1,264 | 1,264 | `resource_3ba:0x020021b8` |
| 156 | 1,264 | 1,264 | `resource_3bb:0x02002450` |
| 157 | 1,264 | 1,264 | `resource_3bc:0x02002ee8` |
| 158 | 1,252 | 1,178 | `main:0x0800f2f8` |
| 159 | 1,248 | 1,248 | `main:0x080d9ae8` |
| 160 | 1,240 | 1,240 | `main:0x080d05fc` |
| 161 | 1,240 | 1,240 | `resource_3a4:0x02000ec0` |
| 162 | 1,236 | 1,236 | `resource_3a4:0x02001838` |
| 163 | 1,228 | 1,228 | `main:0x080b6f44` |
| 164 | 1,226 | 1,226 | `main:0x080f4f04` |
| 165 | 1,224 | 1,224 | `resource_374:0x02000bbc` |
| 166 | 1,220 | 1,220 | `resource_37b:0x02000150` |
| 167 | 1,220 | 1,220 | `resource_380:0x02000f8c` |
| 168 | 1,210 | 1,210 | `resource_39c:0x02003d20` |
| 169 | 1,208 | 1,208 | `main:0x080dd2c4` |
| 170 | 1,204 | 1,204 | `main:0x080ce034` |
| 171 | 1,192 | 1,192 | `main:0x080da6cc` |
| 172 | 1,192 | 1,192 | `resource_37a:0x02002108` |
| 173 | 1,174 | 1,174 | `resource_3bc:0x02000da4` |
| 174 | 1,172 | 1,172 | `main:0x08020bd8` |
| 175 | 1,172 | 1,172 | `main:0x080d33c0` |
| 176 | 1,172 | 1,172 | `resource_377:0x02000578` |
| 177 | 1,168 | 1,168 | `main:0x080a4924` |
| 178 | 1,156 | 1,156 | `resource_372:0x020028a4` |
| 179 | 1,156 | 1,156 | `resource_3b8:0x02000674` |
| 180 | 1,152 | 1,152 | `main:0x080191cc` |
| 181 | 1,152 | 1,152 | `main:0x0801d4cc` |
| 182 | 1,152 | 1,152 | `main:0x080f26ec` |
| 183 | 1,148 | 1,148 | `resource_371:0x02002cb4` |
| 184 | 1,144 | 1,144 | `main:0x080f2028` |
| 185 | 1,132 | 1,132 | `main:0x080d3c80` |
| 186 | 1,128 | 1,128 | `main:0x080d0000` |
| 187 | 1,124 | 1,124 | `main:0x080db264` |
| 188 | 1,124 | 1,124 | `resource_3b1:0x020038ac` |
| 189 | 1,120 | 1,120 | `resource_37a:0x02001ca8` |
| 190 | 1,116 | 1,116 | `main:0x080c02a4` |
| 191 | 1,116 | 1,116 | `resource_37b:0x0200105c` |
| 192 | 1,116 | 1,116 | `resource_3b1:0x020057ec` |
| 193 | 1,116 | 1,116 | `resource_3ba:0x02000db8` |
| 194 | 1,108 | 1,108 | `resource_3a8:0x020032a4` |
| 195 | 1,104 | 1,104 | `main:0x080beb08` |
| 196 | 1,098 | 1,098 | `main:0x080fae58` |
| 197 | 1,096 | 1,096 | `main:0x080d41a4` |
| 198 | 1,092 | 1,092 | `main:0x080db6e0` |
| 199 | 1,076 | 1,076 | `main:0x080030f8` |
| 200 | 1,074 | 1,074 | `main:0x080f4318` |
| 201 | 1,068 | 1,068 | `main:0x080d3854` |
| 202 | 1,066 | 1,066 | `main:0x080e5e28` |
| 203 | 1,064 | 1,064 | `resource_377:0x02000a0c` |
| 204 | 1,060 | 1,060 | `resource_3cb:0x020012e0` |
| 205 | 1,056 | 1,056 | `main:0x080ae2f4` |
| 206 | 1,056 | 1,056 | `main:0x080da2ac` |
| 207 | 1,056 | 1,056 | `resource_399:0x02000b70` |
| 208 | 1,054 | 1,028 | `main:0x080d77b4` |
| 209 | 1,052 | 978 | `main:0x0808bec0` |
| 210 | 1,044 | 1,044 | `main:0x0808d9a4` |
| 211 | 1,044 | 1,044 | `main:0x080bfba4` |
| 212 | 1,044 | 1,044 | `main:0x080cb7f8` |
| 213 | 1,040 | 1,040 | `main:0x080ceb54` |
| 214 | 1,040 | 1,040 | `main:0x080e90a8` |
| 215 | 1,036 | 1,036 | `main:0x080d0ad4` |
| 216 | 1,036 | 1,036 | `resource_3c8:0x02003068` |
| 217 | 1,032 | 1,032 | `resource_3b1:0x02001b34` |
| 218 | 1,028 | 1,028 | `main:0x0801de5c` |
| 219 | 1,024 | 1,024 | `resource_370:0x02000de4` |
| 220 | 1,024 | 1,024 | `resource_3af:0x02000ca0` |
| 221 | 1,024 | 1,024 | `resource_3b1:0x02005d10` |
| 222 | 1,018 | 1,018 | `resource_3c5:0x020028a0` |
| 223 | 1,012 | 1,012 | `main:0x080be378` |
| 224 | 1,006 | 1,006 | `resource_394:0x020003f0` |
| 225 | 1,006 | 1,006 | `resource_3bb:0x02000bd4` |
| 226 | 1,000 | 1,000 | `resource_3a4:0x02003028` |
| 227 | 1,000 | 1,000 | `resource_3bd:0x02003644` |
