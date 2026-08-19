# Contributing to Alchemy

Alchemy is an unofficial clean-room reconstruction of the English release of
_Golden Sun_ for the Game Boy Advance. The goal is a repository of C and
independently described assets that rebuilds the released ROM byte for byte.

This is the only guide. `AGENTS.md` and `CLAUDE.md` are symlinks to it, and
`README.md` is the one other page, kept for end users. Please do not add new
`.md` or `.txt` files; a gate enforces it.

A change counts when the rebuilt bytes equal the released ROM. Everything else
-- readable C, better names, faster tools -- is worth doing, and is not the
same thing. A candidate the same _length_ as the reference, or whose opening
matches, or whose score once looked close, is not a change until the contents
match. This document is organised around how owners actually get closed:
derive facts from the reference, write the C those facts imply, and let
measurement arbitrate every disagreement.

## Contents

- [Provenance and copyright](#provenance-and-copyright)
- [Setting up](#setting-up)
- [The loop](#the-loop)
- [Measure honestly](#measure-honestly)
- [Reconstructing a large owner](#reconstructing-a-large-owner)
- [Reading a residual](#reading-a-residual)
- [How gcc 2.96 places things](#how-gcc-296-places-things)
- [Source style](#source-style)
- [Shapes that decide bytes](#shapes-that-decide-bytes)
- [Where the project is](#where-the-project-is)
- [Settled questions](#settled-questions)
- [Tools](#tools)
- [Build stages](#build-stages)
- [The compiler standard](#the-compiler-standard)
- [Before you commit](#before-you-commit)
- [Targets](#targets)

---

## Provenance and copyright

_Golden Sun_ and its original material are copyright Nintendo and Camelot
Software Planning. Alchemy is not affiliated with or endorsed by either
company. It is a decompilation and preservation effort -- not a remake, a ROM
hack, an emulator, or a game distribution. No ROM is distributed here.

### The evidence boundary

Game-specific knowledge may come only from the approved local ROM set and from
your own reconstruction work against it:

```
gs1-{en,ja,de,es,fr,it}.gba
gs2-{en,ja,de,es,fr,it}.gba
```

`gs1-en.gba` is the build target. The other editions may be compared locally
to tell shared engine code and data apart from edition-specific content. A
cross-edition match is evidence about layout or behaviour -- never about
authorship or an original name.

Do not consult any other Golden Sun checkout, repository history, source,
symbol list, pseudocode, script, generated output, or notes. Public
decompilation projects may inform generic repository, build, testing and
publication conventions; their game code, labels, assets and game knowledge
may not be used. Generic tools and public architecture or compiler
documentation are fine -- including gcc's own source, which this project reads
as a matter of method. Era-appropriate C _style_ -- C89, short names, macros
for repeated tails, `switch`/`break` -- may be read from public pret trees
such as pokeemerald. Their functions, types, and labels may not.

The boundary also constrains naming. A name may claim only what this
repository's own evidence demonstrates. Widely known series lore that has not
been derived here is outside knowledge, and importing it as an identifier is a
boundary violation even when it is probably right.

### The publication boundary

Publishable: reconstructed C and assembly, tools, semantic metadata,
deterministic encoder descriptions, and the source assets the build consumes.

Never commit or transmit: an approved ROM, a built ROM, raw ROM excerpts,
copied ROM gaps, binary patches, cross-ROM binary diffs, private analysis
output, compiler or disassembler output, toolchains, object files, ELFs, build
products, or credentials. Never send private ROM content or generated
artifacts to a network tool.

`check_publication` enforces the file-shape half of this on every staged
commit and on everything you push, including content added and later deleted
within the outgoing range. It cannot tell where knowledge came from -- that
part is yours to honour.

### Hard rules

- No `asm(...)`, `__asm(...)`, fixed-register bindings, empty assembly
  barriers, copied ROM bytes, or any equivalent escape hatch in C or headers.
  `no_asm_c` rejects them. Byte equality never overrides this. A function that
  is not yet C belongs in `asm/<address>.s`, linked at its address, which is
  the same standard pret holds and costs nothing: an undecompiled function is
  undecompiled wherever it is filed.
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

### Permanent assembly

A region is permanently assembly when its instructions could not have come
from this compiler. `asm/classification.json` records the claim as a kind, a
confidence and a list of evidence, and `core_retained_audit` reads that
evidence back off the file for every tag it can decide mechanically:
`arm_instruction_set` must show `.arm`, `fixed_ldr_r4_bx_r4_literal` the
veneer pair, `manual_return_address_preserved_in_ip` both `mov ip, lr` and
`bx ip`.

Claims that rest on judgement rather than a visible property cannot be checked
that way. They are not weaker for it, but they are the ones to read
sceptically before adding to them.

This is what permanent looks like, and no C expresses it:

```
Func_080f9b4c:
	mov	ip, lr          @ the return address is kept in ip
	bl	Func_080f9ab4   @ ...because this bl clobbers lr
	...
	bx	ip              @ and there is no stack save anywhere
```

Claim permanence only from what the instructions show. An owner is not
permanent because it is hard. `main:0x080bbb0c` is 6,332 bytes with no
permanence marker -- no Thumb multiple-transfer, no SWI, no veneer idiom, no
entry in `asm/classification.json`. Every byte of it is reachable as C. Hard
is not permanent.

### Tools cited by sources but no longer present

A reconstructed source cites the tool that derived a fact. Some of those tools
have since been replaced, and the citation is still the truthful record of how
the fact was obtained, so it stays. This table says where to find the tool.

| Cited tool                        | Last commit containing it | Recover with                                         |
| --------------------------------- | ------------------------- | ---------------------------------------------------- |
| `tools/m2c_guard.ts`              | `f185d7346`               | `git show f185d7346:tools/m2c_guard.ts`              |
| `tools/main_xref.ts`              | `f185d7346`               | `git show f185d7346:tools/main_xref.ts`              |
| `tools/overlay_dispatch_sites.ts` | `f185d7346`               | `git show f185d7346:tools/overlay_dispatch_sites.ts` |
| `tools/veneer_resolve.ts`         | `f185d7346`               | `git show f185d7346:tools/veneer_resolve.ts`         |
| `tools/overlay_driver.ts`         | `87d03abf0`               | `git show 87d03abf0:tools/overlay_driver.ts`         |
| `tools/overlay_unindexed.ts`      | `87d03abf0`               | `git show 87d03abf0:tools/overlay_unindexed.ts`      |

---

## Setting up

You need a Rust toolchain, `arm-none-eabi-binutils`, and the approved ROM set
in `roms/` (gitignored -- put it there yourself).

The tooling is Rust. Not Python, not shell, not a scripting language that
happens to be installed: a tool that is not Rust is not in this repository,
and `architecture` will not catalogue it. Throwaway analysis in the scratchpad
-- a `perl` one-liner over a tool's output, a python loop over a JSON file --
is reading, not tooling, and lives outside the tree.

The compiler lives in the `alchemy-gcc` submodule and its built binaries are
expected at `alchemy-gcc/dist/`. In a **git worktree** the submodule directory
is empty; link the staged compiler in rather than making `alchemy-gcc` itself
a symlink, which git refuses:

```bash
ln -s /path/to/main/checkout/roms roms
mkdir -p alchemy-gcc && ln -s /path/to/main/checkout/alchemy-gcc/dist alchemy-gcc/dist
```

Do not run `git submodule` commands inside a worktree -- they can rewrite the
shared configuration and break the main checkout.

Activate the hooks and the generated-file merge driver once per clone:

```bash
git config core.hooksPath .hooks
git config merge.generated.driver true
```

`pre-commit` runs the publication gate over the staged change, `commit-msg`
validates the progress prefix in your subject, and `pre-push` re-runs the
publication gate over every outgoing commit.

The second line resolves `.gitattributes`'s generated artifacts by keeping
your side of a merge, because neither side is correct once both branches have
adopted owners. Regenerate afterwards; `make verify` fails on a stale
artifact, so a forgotten regeneration cannot reach a commit. Without the line
git falls back to an ordinary conflict, which is safe and merely tedious.

Then confirm the tree is healthy and build the compiler driver once per
checkout (or after a tools change):

```bash
make build-claimed   # links every byte-exact owner
make verify          # the authoritative gate
cargo build --release --manifest-path tools/compiler/Cargo.toml
```

The inner loop invokes `tools/compiler/target/release/compiler` directly.
`cargo run` on every shot spends the whole budget on startup: measured on the
6 KB owner, about 510 ms against about 50 ms warm.

---

## The loop

Read the target assembly, work out what C the original author wrote, write
that C, compile it, compare the **instructions**, fix the difference. Repeat
until identical.

What this repository adds is **feedback**. You are not diffing by eye. The
tools compile your candidate and tell you whether the edit moved the assembly
at all, then, when you ask, whether the linked bytes match the ROM. A gate
refuses to adopt anything that does not reproduce.

There are two tiers and no third place that counts. A byte is exact C that
rebuilds identically, or it is assembly. In between, an owner you are working
on lives in exactly ONE file: `draft/<owner>.c`, committed, with its
measurement record `draft/<owner>.json` beside it. `scratch/` is gitignored
and holds everything disposable -- spelling sweeps, variant directories, tool
output. Nothing in either place counts toward coverage; the gates guarantee
that, not the gitignore.

### 1. Pick an owner

An _owner_ is one function-sized region with a fixed address.

- Main image: assembly in `asm/<address>.s`, C in `exact/<address>.c` once it
  matches.
- Code overlays: assembly in `assets/code/resource_<id>_overlay.s`, C in
  `exact/resource_<id>_c_<address>.c` once it matches.

**[Targets](#targets) ranks byte runs, not functions.** Its rows are
contiguous unresolved spans, so the address at the top of a row is where the
run starts, which is frequently the middle of a literal pool rather than an
entry point. Use the table to see the shape of what is left, not to pick.

Pick from `semantic/regions.json`, which holds audited owner boundaries -- an
overlay, an entry address and a span. Those addresses are entry points.

**Check the owner is adoptable before you write any C.** The adoption gate
requires the whole span to sit inside ONE audited executable interval from
`metrics/gs1-en-executable.json`. An owner that falls in an unaudited gap
between two intervals is refused however exact your C is: `resource_38a:04c4`
is byte-exact and unadoptable, because the pool before it ends at `0x4c4` and
the next thumb interval starts at `0x4d4`. A span that carries the owner's OWN
literal pool is fine -- `resource_3b9:1c6c` adopted at `--span 104`, function
plus pool word, where the bare 100 was not registered. Filtering the open
owners by the interval rule costs one pass over the JSON and saves writing C
you cannot land.

Two cheap checks before committing to one. The instruction mix decides how
hard it is: a scripted sequence of `movs` and `bl` with six compares in the
whole function is far easier than a third of its size in dense arithmetic. And
count the finished owners in the same overlay -- where a hundred are already
done, the structs, callees and idioms are established.

The owner's one file is `draft/080bbb0c.c`, or
`draft/resource_3b2_c_02000da4.c`. The rankers find drafts by that name, in
`draft/` and `scratch/` both; a stem present in both ranks once, from
`draft/`.

For a call-dense owner, `overlay reconstruct resource_3bd:13f8` writes a first
draft into `scratch/` from the owner's own disassembly; a main-image owner is the
same lift from its assembly, `compiler reconstruct asm/080bbb0c.s`. It models
what the bytes plainly show and is silent about everything else -- it writes
no `if`, so the control flow is yours to read and add. Its output goes to
`scratch/` and nowhere else; the tool refuses any other path -- promote it to
`draft/` when you have read it and started real work, because a generator
aimed at every owner is exactly the machine that produced 862,856 bytes of C
nobody could use. Score it before believing any of it.

### Work a cohort, not an owner

Pick a COHORT and read its members side by side. A cohort is a set of owners
that share an assembly SHAPE. It is not a subsystem, not an overlay, and not a
set of files that look related by name -- those groupings say nothing about
what the compiler did.

Four owners make the point. `resource_3b2:0da4`, `resource_3c4:1068`,
`resource_3c5:0cf0` and `resource_3c8:096c` sit in four different overlays and
have nothing to do with each other by feature. Disassembled, they are the same
90 bytes instruction for instruction, differing only in the address their one
`bl` encodes. One source shape closes all four.

Cohorts are why the levers in [Shapes that decide
bytes](#shapes-that-decide-bytes) exist at all. The bitfield rule was not
found by staring at one owner; it was found because four owners in different
overlays all carried `movs r3,#13 / negs r3,r3` in their span, and the thing
they had in common was the only thing that could explain it. A single owner
cannot tell you which of its features is the one that matters. Two owners that
differ in exactly one way can.

It is also the guard against looping. With one owner in front of you the only
move is to keep respelling it, and the residual either falls or you grind.
With a cohort you have a second reading of the same problem, and the
difference between two members is itself evidence. If a shape resists across
the whole cohort, that is a real finding worth recording; if it resists on one
member and not the others, the member is what is different, not the compiler.

A large main-image owner is the exception that still wants a cohort _of
shapes_, not of files: grep the open owners for the idiom in front of you and
read a finished sibling that already emits it. Inside one large function the
same idea applies to its own near-identical case bodies -- seventeen of
`080bbb0c`'s effect handlers are one family, and reading them as a family is
what exposed that their id bindings were scrambled.

Ways to find one:

- `overlay twins` groups owners by identical shape and names a solved member
  where one exists. Those groups are cohorts by construction.
- Disassemble the open owners in bulk and grep the output for an idiom -- the
  `negs` mask pair, a six-argument call spilling two stack slots, a
  `movs`/`lsls` constant pair next to a `movs r0`. Each grep is a cohort.
- Sort by instruction mix rather than by size. Owners with the same ratio of
  calls to memory operations to branches tend to have come from the same kind
  of source.

If you hand work to a subagent, hand it a cohort for the same reasons. An
agent given one owner has the same failure mode you do and no second reading
to escape it with.

### 2. Read the assembly before writing any C

C written before you understand the target is guesswork with extra steps, and
guesswork can score well: `080bbb0c` sat for a session at 2,744 differing
halfwords with case bodies that were the WRONG ALGORITHMS, because wrong
bodies still align against other bodies' instructions. A decompiler is a
reading aid; its output is a scaffold to check against the ROM, never a source
draft. Get these facts first:

- **Boundaries and entry.** Prologue shape, frame size, which registers are
  saved, stack slots and arguments, and whether a shared tail is reached from
  elsewhere. Declaration order of the locals is the stack-slot order: on
  `080bbb0c`, `offset` first and `action` second is `fp` and `[sp,#76]`.
- **Types and signedness**, read off the load widths. `ldrb`/`ldrsb`/`ldrh`/
  `ldrsh` are declarations, not hints. A `lsls`/`asrs` pair is a sign
  extension the source asked for. `cmp #7` / `bhi` is an unsigned compare;
  `ble` is signed. They are not interchangeable spellings of the same test.
- **Control flow.** Loop heads and bottoms, switch and jump-table layout,
  branch cascades, fall-through seams. A jump-table slot that points at the
  instruction _after_ the switch is an empty `case` with `break`, not a body
  that happens to live there.
- **Jump tables are a case-order oracle.** Case bodies are emitted in SOURCE
  ORDER -- verified in the lab with a descending-value switch -- so sorting a
  jump table's targets by address recovers the order the original author wrote
  the cases in, and the table itself binds every case value to its body.
  `080bbb0c`'s effect switch reads `64, 4, 61/62, 63, 59, 58, 9, 8, 7, 6, ...`
  -- descending runs no one would guess.
- **Text ids are readable.** Any `0x8xx` pushed to the event queue looks up
  in `assets/text/message_archive.json`; the string usually settles what the
  surrounding code is for. See [Source style](#source-style).
- **Pool words are ground truth.** Every `ldr rX, [pc, #N]` resolves to a
  word holding a text id, a struct offset, a table address or a constant, and
  those values arbitrate bindings that behaviour alone cannot. Resolving them
  is mechanical: `arm-none-eabi-as` the `.s`, `objdump -d`, and each pool load
  carries its target in a comment. Hand-stepping addresses over a thousand
  lines drifts at every `.align`; the assembler does not.
- **Aggregate layout.** Which offsets cluster around which base registers,
  and which cells are read through one pointer and written through another.
- **Constants.** Which live in the literal pool and which are built inline. A
  pooled word and an inline `movs` are different source spellings, and the
  boundary between them is decidable -- see [Settled
  questions](#settled-questions). Thumb `ldrsh` has no immediate offset: a
  load from `base + 100` is `movs r2, #100` / `ldrsh r3, [r1, r2]`, and
  `slots[50]` folds to `adds r1, #102` / `ldrsh [r1, r5]`. Different source
  shapes.
- **Calls and side effects**, and every value's lifetime across them.

`overlay disasm`, `overlay show` and `overlay entry` decode overlay bytes.
Main-image assembly is the `.s` already in `asm/`.

Before you start, check whether the owner is C at all. Grepping the span for
`stmia`, `ldmia`, `swi` and the `mov ip, lr` / `bx ip` pair takes a second. A
SINGLE-register `ldmia r1!, {r3}` is ordinary C: Thumb has no
post-incrementing `ldr`, so that is what `*p++` compiles to. The
multi-register form is the tell. A `bl` to a LOCAL label is not a call either:
it is the assembler's long branch where `b.n` cannot reach, which on a large
function usually marks a shared tail.

### 3. Write the compiler's input, not its output

The ROM is optimised compiler output. The C you want is what went _in_: plain
structs, arrays indexed by loop variables, ordinary expressions, natural
statement order. The optimiser then reproduces the ROM's shape by itself,
because that is exactly what it did the first time.

The common mistake is transcribing the optimiser's work back into the source
-- hand-strength-reduced pointer walks instead of `arr[i]`, hand-shared
temporaries instead of repeating an expression, hoisted invariants. That does
not lock in the ROM's shape; it changes what the earlier passes see, and the
result diverges somewhere else. The same applies to shared code the optimiser
made: two identical tails merged at one label may be CROSSJUMPING's work on
two plain arms, not a `goto` in the source. Write the plain arms first and
reach for `goto` only when the reference's label is entered from somewhere
crossjumping cannot reach.

A loop whose exit test is at the top reads as a `goto` in the disassembly.
Write the loop. The bound runs both ways: write machine-producing structure,
not modern style. Prefer the shape a 2001 author would have written. C89.
Macros for a tail that repeats across cases, not `static inline` -- at `-O2` a
static helper that is not inlined becomes a second symbol and `candidate-show`
refuses the object. `switch` / `break`. No `for (s32 i = 0; ...)`. Short
names. Faithful bugs: an original that reads an uninitialised local on six
switch arms is reproduced by a C89 switch with no `default`, not repaired.

### 4. Confirm the edit, then score the owner

Two tools, two questions. Do not use one for the other.

**Did this edit move the assembly?** gcc `-S` and `git diff` of the insn
lines. No ROM, no link, no objdump of the image. Invoke the binary, not
`cargo run`:

```bash
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --asm
```

Warm, on the 6 KB owner, that is about 60 ms. It prints insn counts, a
`git diff --stat` against `asm/<addr>.s`, and a second stat against the
previous candidate. `vs previous candidate: identical` means the C change was
a no-op in RTL -- stop theorising and write a different shape. A real edit
shows up as insertions and deletions.

**Do the linked bytes match?** That is the score, and it is the only number
that can go in a `draft/` record or an adoption:

```bash
tools/compiler/target/release/compiler candidate-show draft/080bbb0c.c --align
```

`--first` is `--align` cropped to the first residual window. `--patch FILE`
applies a unified diff to a copy and scores that, without touching your
source; single shots only -- a four-way `--patch` fan-out once took fourteen
minutes. Parallel theories go in separate `--work` directories instead.

For an overlay row the linked-byte score is the same idea, same `--align`, no
span argument:

```bash
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  score draft/resource_3b2_c_02000da4.c --align
```

Then make the smallest source change that explains what you saw, confirm it
moved the asm, and score again. [Measure honestly](#measure-honestly) says
which numbers to steer by; it is short and it is the part most sessions get
wrong first.

### 5. Adopt

Adoption is gated. It rebuilds and compares before it accepts anything, and it
refuses everything else -- a candidate that is 32 bytes against a 6,332-byte
reference is rejected at `+0x0` with both sizes named. `differing_halfwords`
must be 0. A matching file size is not that.

```bash
# main image: proves a src_<address>.c draft, installs to exact/
make dispatch-decomp ARGS='integrate_matches draft/<address>.c --apply'

# overlay: rehearses the whole overlay, then splices the row in
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  adopt resource_373:034c --source draft/resource_373_c_0200034c.c --apply
```

An owner discovery has not indexed needs its size given explicitly, as
`--span 64`; the tool says so rather than guessing.

An overlay row is spliced into a fixed-size hole, so the gate checks the
region boundary, that no label outside the region is destroyed, and that the
whole overlay still assembles to the same bytes. Copying a file into `exact/`
is not adoption and will not survive the build.

**Run one adoption at a time.** It reaches that verdict by splicing the row
into `assets/code/<overlay>.s` and restoring the file afterwards, so it writes
to the tree even when it rejects and even without `--apply`. `OverlayLock`
serialises it per overlay; score in parallel as much as you like, and keep
adoption serial.

A zero score is not an owner. Of 1,276 candidates once scored in bulk, eight
reproduced exactly and none were adoptable: six were 8-byte veneer stubs, and
the two real ones were measured across spans straddling several audited
intervals, so the gate refused them as not single owners. Put it through the
gate rather than believing the number.

The inverse exists, and you should use it rather than leaving a broken row in
place:

```bash
cargo run --release --manifest-path tools/overlay/Cargo.toml -- park resource_373:034c --apply
cargo run --release --manifest-path tools/overlay/Cargo.toml -- audit --all
```

`park` restores a row's assembly and returns its C to `draft/`. `audit`
compares every adopted row against the bytes it replaced and names any that no
longer reproduce. Run it after any merge that brought in adoptions from both
sides.

Delete the `draft/` pair -- the `.c` and its record -- when the owner is
adopted; `integrate_matches` does it for you on the main image. The gate will remind you about the record.

### 6. When an owner will not converge

Take another owner. A main-image owner needs nothing done to it -- the build
falls back to `asm/<address>.s` whenever `exact/<address>.c` is absent. An
overlay row must be parked with the command above, because its assembly has to
come back into the overlay.

Understanding accumulates: every owner you finish becomes a worked example of
what correct source looks like for this compiler, and the next one is easier
because of it. `exact_reading_list` and `overlay twins` are how you find the
relevant examples.

Record what you tried. `draft/<owner>.json` carries the score the tools
reported, `unexpressed` in instruction counts, and `rejected` with the number
that killed each dead end. The draft itself is already committed beside it.

### Merging another branch, or main back into yours

Every branch that adopts an owner rewrites the same generated artifacts, so a
merge between two working branches conflicts on all of them and neither side
is right afterwards. Do not resolve them by choosing:

```bash
git merge main -m '☀️ N% – Merge main: <what came in>'
make coverage      # regenerate the Targets section, figures and metrics
make verify        # refuses a stale artifact, so this is the proof
overlay audit --all
```

Pass `-m` yourself: `commit-msg` requires the progress prefix and git's
default `Merge branch 'main'` does not have one, so a merge with no conflicts
at all still stops at the hook with the result already staged.

`CONTRIBUTING.md` and `README.md` still conflict, in the Targets counts and
the figure hashes. Take either side there and let `make coverage` correct it;
the prose around them is hand-written and merges normally, which is exactly
why those two are not auto-resolved.

---

## Measure honestly

Every number below is a view of the same comparison, and each answers exactly
one question. Sessions go wrong by steering on the wrong one.

**`differing_halfwords` is the gate, not the compass.** It is zero when the
owner reproduces, and that is the only number adoption accepts. But it counts
differences at matching file offsets, so on any size mismatch it is mostly
phase: one missing instruction shifts everything after it. On a 6 KB owner a
draft can IMPROVE from 2,744 to a semantically better state and watch
halfwords rise to 2,957, purely because the sizes diverged. Quote it, chase it
when sizes agree, and do not let it veto a structural correction.

**The insn diff is the compass.** `--asm` reduces both sides to one canonical
instruction per line -- register-copy spellings, branch width suffixes, two-
and three-operand ALU forms, bare and explicit zero offsets, pool and label
references all folded -- so its `git diff` counts real differences. On
`080bbb0c` that canonicalisation removed 564 phantom rows in one session;
before it, half the reported drift was dialect.

**Normalise away what the allocator owns to see what you own.** Registers and
spill slots are the compiler's choice, downstream of your allocno set. Two
`perl` passes over the `.insns` files in the work directory produce the
structure-only view:

```bash
cd scratch/candidate-show/080bbb0c
perl -pe 's/\br(1[0-2]|[0-9])\b/rr/g; s/\b(sl|fp|ip)\b/rr/g; s/\[sp, #\d+\]/[sp,N]/g' \
  reference.insns > /tmp/ref.n
perl -pe 's/\br(1[0-2]|[0-9])\b/rr/g; s/\b(sl|fp|ip)\b/rr/g; s/\[sp, #\d+\]/[sp,N]/g' \
  candidate.insns > /tmp/cand.n
git diff --no-index --stat /tmp/ref.n /tmp/cand.n
```

The split matters: `080bbb0c` at one point showed 791 differing insn rows, of
which only 444 survived normalisation -- the other 347 were coloring, which
follows the body and is not chased row by row. The normalised count is the
honest measure of remaining structural work, and `git diff -U0` over the same
files ranks the hunks so the biggest misread is always the next target.

**A good-looking score can be wrong bodies aligned by luck.** The measured
case: a draft whose case bodies were misbound -- wrong algorithms attached to
wrong ids -- scored BETTER on halfwords and prefix than its corrected
successor, because plausible instructions align against other plausible
instructions. Structural verification against the reference beats every
aggregate: when a fix the bytes demanded makes the numbers worse, the numbers
were lying before, not after.

**The matching prefix is real and also a trap.** An opening that keeps enough
values live across calls reproduces the whole register-save sequence; each
correct block extends it. But extra or hotter allocnos recolour the WHOLE
function, prefix included, because gcc 2.96 does not split live ranges.
Protecting `matched_prefix=81` while refusing any edit that recolours it is
how a session spends its budget on the first residual. Drop the body's
wrongness first. The opening comes back when the allocno set does.

**The frame is a count of live slots, not a constant to match.**
`sub sp, #100` is as big as the widest spill anywhere in the body; a new live
value grows it to 104, dropping one shrinks it to 96. Read a frame difference
as "one extra spilled value somewhere", not as a number to pad.

**Never quote size as completeness.** Of 348 drafts at 85% of reference size
or better, the median matched ten percent of their instructions. A size-exact
6,332-byte draft differed in 2,929 halfwords. The 52 bytes a better draft was
short were missing insns, not a frame to pad.

---

## Reconstructing a large owner

No owner over 2 KB has been made byte-exact yet, and owners of 512 B and over
hold two thirds of the remaining audited overlay bytes. The method below is
how `080bbb0c` -- the largest target, 6,332 bytes -- went from guessed bodies
to measured convergence, and every step of it generalises. It is the ordinary
loop with the reading turned up.

**Slice the function by its own tables.** A jump table binds every case value
to a body address; body addresses sorted give the source order; consecutive
addresses delimit each body's span. That turns one 3,000-instruction problem
into forty bodies of 15-90 instructions, each independently checkable, each a
member of a family of near-twins.

**Resolve the pools before trusting any binding.** Assemble the reference
`.s` and objdump it; every `ldr [pc, #N]` then names its word. On `080bbb0c`
the resolved pools showed that SEVENTEEN effect ids were bound to the wrong
bodies -- the revive family on the wrong ids, attack and defense scrambled,
and what the C called agility split into two different modifier families that
the struct had never distinguished. Behaviour-level reading had produced a
plausible, internally consistent, wrong assignment; the pool words settled it
in one pass. Text constants pair by DIRECTION (one text for both strengths of
"attack fell"), field offsets identify the struct member, table addresses
identify the data.

**Verify per body, not per function.** The global diff is dominated by
whichever bodies are wrong; fix the biggest hunk, remeasure, repeat. Each
body's checklist is short: the reads (width and order), the algorithm shape
(see [Shapes that decide bytes](#shapes-that-decide-bytes)), the call
sequence, the texts, the terminator -- `break`, a `goto` into a shared tail,
or a fall-through the original really had.

**Shared tails are part of the structure.** Three cases jumping into one
store-and-update block, a text-push tail entered by `bl`-as-long-branch --
these are visible in the reference as labels entered from multiple bodies.
Some are source `goto`s, some are crossjumping. The tell: crossjumping only
merges IDENTICAL tail sequences; a tail entered with different values in the
same role, or entered from a body whose own copy would have differed, was a
`goto` in the source.

**Name as you verify, so mistakes become visible.** Every callee gets a
cautious alias at its declaration, every magic case value an enum name derived
from what its body demonstrably does. This is byte-neutral -- verify with
`--asm`, which must say `identical` -- and it is not cosmetic: the seventeen
misbound effect ids were repaired by editing enum VALUES in one screen,
because the names were attached to bodies and only the numbers were wrong.
Unnamed, the same fix would have been error-prone surgery across forty case
labels.

**Reproduce what a call-sequence draft cannot model.** The drafter emits
calls and constants; loads and stores through held pointers, loop counters and
their tests, and control flow are yours. On `resource_3bd:13f8`, teaching the
draft pointer-valued loads and the read-modify-write shape took 674 wrong
instructions to 601 -- the residual tracks how much of the source is
expressed, not a fixed limit. Check for backward branches before concluding
anything: a linear reconstruction can be structurally complete only for a
function that has none, which is why `resource_3bf:3054` -- 636 calls, no loop
anywhere -- got closest without closing.

**Keep the record current.** The `draft/` record for a large owner is the
session hand-off: score, per-body findings under `expressed`, the honest
remainder under `unexpressed`, and every measured dead end under `rejected`
with the number that killed it. `080bbb0c`'s record is the worked example.

---

## Reading a residual

A differing-halfword count says how much differs _at matching file offsets_.
It does not say what kind of problem you have. `overlay score` and the rankers
classify every residual, and the class is the first thing to read:

| class           | what it means                                                           | worth reading? |
| --------------- | ----------------------------------------------------------------------- | -------------- |
| `exact`         | the bytes match                                                         | adopt it       |
| `wrong`         | some instruction is genuinely different                                 | **yes**        |
| `size mismatch` | a statement is missing or extra                                         | **yes**        |
| `ordering`      | same instructions, different order                                      | sometimes      |
| `allocation`    | same instructions and operands, different registers                     | sometimes      |
| `unemittable`   | the reference _appears_ to use an instruction this compiler cannot emit | confirm first  |

Rank `wrong` and `size mismatch` first. A small halfword count on a blocked
row is not a near miss.

**Do not read `ordering` and `allocation` as compiler verdicts.** Both mean
the same thing far more often than they mean a wall: the source does not yet
express what the reference expresses. Twelve owners closed in one session, and
several arrived carrying an `allocation` or `ordering` label that dissolved
the moment the right source shape was written. An incomplete reconstruction
reads as an allocation difference -- closing the expression gap closes both.

`unemittable` is advisory and wrong about one time in five. It reads a
disassembled stream, and a literal pool word disassembles as whatever its
bytes encode: on ARM7TDMI that includes `stmia`, `ldmia`, and mnemonics the
part cannot execute at all. Measured against the assembler's listing, 18 of 89
rows carrying the verdict had no multiple-transfer anywhere in their span. It
also fires on any large size mismatch, where it means nothing. Where it is
true, it is decisive: `arm.md` gates both store-multiple peepholes on
`TARGET_ARM`, so a reference-side Thumb multi-register `stmia` or `ldmia`
cannot have come from any C source.

### When ordering really is the scheduler

Some ordering residuals are genuinely out of reach, and the answer is
mechanical rather than a matter of taste.

The pass that decides is the POST-RELOAD SCHEDULER, not reload. Compile one
function twice, identical except that the second has a `switch` with a
`default: return`, and both reach reload with the SAME insn chain:

| pass                          | chain around the pair                                                |
| ----------------------------- | -------------------------------------------------------------------- |
| `18.greg`, after reload       | `mov r2,#8` / `neg r2,r2` / `mov r0,#0` / `mov r1,#3` -- both builds |
| `23.sched2`, after scheduling | adjacent build: `mov r2,#8` / `neg` / `mov r1` / `mov r0`            |
|                               | interleaved build: `mov r2,#8` / `mov r1,#3` / `neg` / `mov r0`      |

Reload places them identically; `sched2` reorders them differently. What
varies is the function's basic-block structure, and the dependence graph the
scheduler builds from it.

`rank_for_schedule` consults, in order: priority; then (before reload only)
register weight; then the interblock comparisons; then the insn's dependence
class relative to the last scheduled insn; then **the number of later insns
that depend on it**; then `INSN_LUID`. Read them off the `-fsched-verbose=5`
dependence table, which prints every one:

```
;;      insn  code    bb   dep  prio  cost   blockage units
;;        29   173     0     0     3     1    1 - 32   core  : 42 36
;;        31   173     0     0     3     1    1 - 32   core  : 42 41 36
```

Here `29` is `mov r0,#31` and `31` is `mov r1,#0`. Same priority, but `31` has
THREE dependents against `29`'s two, so it wins and is emitted first. The
extra dependent is the second call's own `r1 = 0`: an output-dependence,
because nothing between them writes r1. r0's equivalent chain is CUT by the
intervening call, which returns its value in r0.

That is a lever reachable from the source. Declaring the intervening callee
`void` removes the `(set (reg r0) (call ...))` from its RTL, un-cuts r0's
chain, and flips the pair -- measured, on `resource_398:04b4`, from two
differing halfwords to one.

What is NOT a lever, measured rather than assumed: the spelling of the call or
its arguments. Twenty-five spellings of one three-argument call -- every
literal form, both return types, narrow and wide parameter types, the constant
in a local, in a `const`, in an `enum`, in a `volatile`, the pool address as
an integer, a cast, an `extern` array, an address-of -- all give the identical
residual. When the keys tie and only `INSN_LUID` separates the pair, LUID
follows the pre-reload argument-emission order, and no respelling of the
arguments changed that order.

So an `ordering` residual is reachable when you can change a key -- block
structure, or a dependence chain -- and unreachable when every key ties. Read
the dependence table before deciding which one you have, and record the answer
either way.

One shape in this family is currently unbeaten and worth naming so nobody pays
for it twice: a `movs r0, #N` that the reference places BETWEEN a
`movs`/`lsls` constant pair, where reload emits the pair first and LUID puts
our `movs r0` after it. Six owners sit at two or three differing halfwords
with zero wrong instructions on exactly this: `resource_3c7:0030`,
`resource_3bf:0c78`, `resource_389:09dc`, `resource_38d:1958`,
`resource_3b5:0260` and `resource_3a2:0870`.

Two shapes inside `wrong` are worth naming because they look like source
problems and are not. A candidate that holds a constant in a callee-saved
register and copies it into the argument register at each call, where the
reference rebuilds it, is cse unifying two identical `(const_int N)` trees
into one pseudo that reload then gives a register. And a pair of comparisons
that differ as `cmp #9`/`blt` against `cmp #8`/`ble` is `fold-const.c`
rewriting `<` against a positive constant. Neither is reachable by respelling
the source.

---

## How gcc 2.96 places things

This is the compiler the owners were built with, and its source is in
`alchemy-gcc/gs1cc/gcc/` -- READ IT when a residual resists. Every hard
question this project has settled about codegen was settled faster by the
pass's own code than by sweeping spellings: the scheduler's comparator
(`haifa-sched.c`), the spill-slot rule (`reload1.c`), the constant-building
boundary (`config/arm/arm.c` and `arm.md`). Reading the residual as "the
allocator picked wrong" is how a session respells the same function.

The passes, in order, are the ones that matter: local-alloc, then
global-alloc, then reload, then the post-reload scheduler. There is no
live-range splitting. An allocno is one pseudo for its whole life. Its
priority is roughly
`floor_log2(n_refs) * n_refs / live_length * 10000 * size`. Heating a local --
more refs, a longer life -- can steal a LO register from the opening of a 6 KB
function even when the local is block-scoped to a later `if`.

Thumb wants LO_REGS first. `-fcall-used-r4` means a value that lives across a
call and wants a LO register gets r5, r6 or r7. HI order is r8, sl, r9, fp.
Stack slots among spills follow declaration order, with one refinement from
`reload1.c`'s `alter_reg`: pseudos spilled FROM THE SAME hard register share
one slot, so two disjoint-lifetime locals that both coloured to r6 and both
spilled land at the same `[sp, #N]`. Merging two such locals into one variable
does not reproduce that -- measured on `080bbb0c`, one merged `pp`/`cur`
raised the insn count by 14. The reference gets one register for both roles
through two pseudos with disjoint lifetimes, not one hot one.

CSE hashes equivalent trees. Two reads of `tbl[1]` become one `ldrh` plus
`lsl`/`asr`; `p = (s16 *)((u8 *)target + 36)` after `tbl` already holds that
address becomes `mov r2, r4`. Integer punning, `volatile` on the wrong object,
unions, and `p += 18` all CSE back to the same `mov`.

`fold-const` rewrites `(x + 30) * 0x28f` into `x * 0x28f + 30 * 0x28f`.
`chance += 30; chance *= 0x28f` is the source that leaves `adds #30` / `muls`
in the RTL. It also rewrites `<` against a positive constant, which is the
`cmp #9`/`blt` versus `cmp #8`/`ble` pair above.

A ternary of two constants is if-converted: load the true-arm, maybe overwrite
with the false-arm. `(u32)target_id <= 7 ? player : enemy` therefore emits
`bls` where the reference's if/else through a temporary emits `bhi` with two
separate loads. The unsigned compare is necessary and not sufficient; the
_statement shape_ has to make both arms stores.

Constant building on Thumb is decidable from `thumb_shiftable_const`: a value
expressible as an 8-bit immediate shifted left builds as `movs`/`lsls`, and
`*thumb_movsi_insn` SPLITS every such constant -- a bare shiftable constant
NEVER pools, in any spelling (six tried). So a pooled word that passes the
shiftable test was not a bare constant in the source: the expression differed,
or the value is not what you think it is. Conversely `+= -2048` lets the
compiler narrow to `0xf800` and build it where the reference pools
`0xfffff800`; `-= 2048` keeps it wide.

Crossjumping at `-O2` merges IDENTICAL tail sequences between branches -- it
is why three plain `if` arms that each end with the same two-instruction call
setup emit one shared block, and why writing duplicate tails in plain C is
usually right even when the reference shows a label.

RTL dumps (`-da`, `rtl-insn`, `rtl-schedule`) answer which pass produced a
shape. `18.greg` is after reload; `23.sched2` is after the post-reload
scheduler. If the two listings already differ at greg, it is not the
scheduler.

`--asm` reporting `identical` against the previous candidate is CSE or
if-conversion eating the spelling you thought you had written. Write a
different shape. Do not add an allocno to force a register: on a large owner
that is how the opening dies.

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

**Bodies read in prose, boundaries stay verifiable.** No bare `Func_` in a
function body: keep the relocation name at the ABI boundary and alias it where
it is declared:

```c
u32 Func_08004938(s32 size);
#define Sys_Alloc Func_08004938
```

The same discipline applies to magic numbers a switch dispatches on. A case
value whose body's behaviour is demonstrated gets an enum name
(`EFX_HEAL_60`, `EFX_DRAIN_PP`); one whose meaning is only positional gets an
offset-derived name (`EFX_AIL138`). The enum states in a comment that these
are reconstruction aids, not claims about original identifiers. This is
byte-neutral -- run `--asm` and require `identical` -- and it is load-bearing:
when seventeen of `080bbb0c`'s effect bindings turned out to be scrambled, the
fix was an enum-value edit, because the names were attached to verified bodies
and only the numbers were wrong.

**Text ids resolve to the game's own script, and the script names things.**
`assets/text/message_archive.json` holds the decoded English text: id
`0x820` is bank `0x820 >> 8`, entry `0x820 & 255` -- "X's HP is fully
restored!". That is in-repo evidence, so a battle owner's magic text numbers
become `MSG_` constants named from their strings, the status byte a text
announces becomes `poison` or `stun` rather than `status_131`, and an effect
that pushes "X is confused!" is `EFX_CONFUSE`. Where one string names a game
term outright ("blocked by Refrain!"), the term is usable: the script is the
ROM speaking for itself. This is how `080bbb0c`'s seventeen scrambled effect
bindings were caught -- names bound to verified strings made the wrong numbers
visible -- and it is the difference between a file a human can read and a
wall of hex.

In an overlay, name a call by the **veneer entry that site reaches**, not by
the import behind it, and record the import in a comment:

```c
Record_02000e20 *Func_02003f32();  /* Func_0808a080 */
```

Every site gets its own entry in the overlay's import table, so the import's
name cannot say which one, and naming it directly leaves the `bl` encoding
wrong. `overlay show <overlay> <start> <end>` prints the address the
reference's `bl` encodes, and the same range with `--annotate` names the
import it reaches. The encoded address is the one to use; a finished sibling
in the same overlay settles it in seconds if you are unsure.

Fix the naming when you open an owner to work it, where the residual can be
read afterwards. A batch rename that moves scores and closes nothing is not
progress.

### Comments

Brief, factual, and about things a future contributor needs: relationships
between values, invariants, hardware constraints. Canonical comments are short
Japanese UTF-8 lines matching the naming convention; English is fine where it
makes a technical constraint clearer.

```c
/* マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。 */
```

Record what the bytes prove, including when it made the score worse: a source
that is right and scores badly is more useful than one that is wrong and
scores well, and the next reader needs to know which they have. Do not use
comments as a speculation diary, and do not claim to have recovered an
original identifier the evidence does not support.

---

## Shapes that decide bytes

Prefer the simplest shape the evidence supports. Keep uncertain fields, casts,
aliases, signedness and control flow explicit until they are proved. Risk
lives in types, scoping and control flow, not in spelling: a small change to a
type or an alias can move register allocation and break an exact match.

The rule that closed the most owners: **write the statement order the
reference's REGISTER LIFETIMES imply, then let the optimiser place the
instructions.** Everything below is a special case of it.

### Bitfields

`(x & ~M) | V` on a sub-byte field is a BITFIELD ASSIGNMENT, and writing it as
mask arithmetic does not reproduce. The tell is the width of the mask. A byte
field masked with `~12` in ordinary C narrows to QImode and the compiler emits
`movs r3, #243`; the bitfield path stays in SImode and builds -13 as

```
	movs	r3, #13
	negs	r3, r3
```

which is two instructions where the arithmetic spelling has one. So

```c
struct Rec { u8 pad00[9]; u8 lo : 2; u8 mode : 2; u8 hi : 4; };   /* +9, bits 2..3 */
rec->mode = 1;                    /* not  rec->f9 = (rec->f9 & ~12) | 4;  */
```

The assignment does its own masking and shifting, so `rec->mode = value` also
covers the `(value & 3) << 2` form. This one shape closed four owners --
`resource_399:1704`, `resource_39c:0030`, `resource_39a:0ed8` and
`resource_3b1:02f4` -- and each had been sitting at a residual that read like
an allocation problem. Not every mask is a bitfield: a `& ~1` that stays
narrow (`movs r3, #254`) really is byte arithmetic, and the mask width tells
you which you are looking at before you write anything.

### Types and widths

Narrow types are usually the wrong reading. The store width comes from the
pointer cast or the struct member, not from the variable, so a `u8` local that
the reference never truncates costs an extension the ROM does not have. Widen
by default and narrow where the bytes show a truncation. A narrow _parameter_
is worse than a narrow local: it silently truncates the argument at every call
site, so `f(0x301)` through a `u8` parameter compiles to `movs r0, #1`.

Mixed-width reads of ONE field are two source mentions with different types.
`080bbb0c`'s percentage heal reads hp with `ldrh` for the delta base and
`ldrsh` for the accumulator, and max twice the same way: a `u16` local and the
s16 field expression, each used where its extension pattern appears. The
`lsls #16`/`asrs #16` pairs in the reference mark exactly where the u16 value
is re-signed.

A callee's declared return type is part of the interface and is visible in
the bytes: a non-void return keeps a value live across the caller's argument
setup. If your arguments are set up in the wrong order, check the prototype
before you suspect the allocator.

Unsigned compares are declarations too. `cmp rX, #7` / `bhi` is
`(u32)id <= 7` as the `if` condition. The same test written signed is `ble`.
Twelve sites on `080bbb0c` are this one fact.

### Loads, stores and re-reads

Fixed addresses touched more than once should be declared objects, not
`(void *)` literals, so the base stays in a register:

```c
extern s16 Data_02000240[];
```

A global the reference re-reads on a path where the value provably cannot have
changed is a `volatile` object, and nothing else in ordinary C produces that
reload. This applies to a STRUCT FIELD as much as to a global, and the
qualifier goes on the pointer, not the pointee. `resource_3b5:06e8` stores
through `work->f80` twice and the reference loads `[r5, #80]` again for the
second store; `struct Rec *volatile f80;` reproduces it,
`volatile struct Rec *f80;` does not -- the first says the pointer may change,
which is what forces the reload.

A field the reference loads ONCE and uses twice is a local in the source. Read
it into the local before its first use rather than writing the field
expression twice; the compiler will common them either way, but the load lands
where the first mention is. A field the reference RE-READS after an update it
just computed is the opposite tell: the source mentions the field again rather
than reusing the local -- the heal shape below depends on it.

### Constants and locals

A value that appears both as an immediate and out of a register is a variable,
not a literal. When one call takes 8 as `movs r3, #8` for one argument and as
`mov r3, r8` for another, a literal cannot produce both: the second came from
a local assigned before the loop and held across it.

A constant the reference DERIVES from another is a division in the source, and
not a shift. `resource_3bc:3b40` writes 0x20000 and then 0x10000, and the
reference builds the second with `asrs r3, r3, #1` off the register that still
holds the first. Both `0x10000` and `scale >> 1` rebuild it with `movs / lsls`
instead; only `scale / 2` reproduces, because the signed division is what
leaves the shift in the RTL for the register value rather than folding a new
constant.

WHERE a local is assigned matters as much as that it exists. Two stack
arguments initialised at their declaration are materialised and stored one at
a time; the same two assigned after an intervening statement stay live
together and give the reference's `movs r3,#8 / movs r2,#32 / str / str`. On
`resource_3ad:0094` that distinction alone is the whole residual:

```c
    s32 a, b;
    if (rec != 0) { ... }
    a = 8;                      /* not  s32 a = 8, b = 32;  above the if */
    b = 32;
    Func_02001bb0(7, 32, 1, 1, a, b);
```

Reach for locals whenever the reference holds a value in a register across a
call or stores two of them together, and reach for literals when it rebuilds
the value each time -- including when it rebuilds 100 twice in the same block.

A candidate exactly four bytes short of its reference is usually missing one
pool word. Either a constant is spelled as a literal where the reference links
a symbol, or the value is dead in your source and the compiler dropped it --
check for a variable that is assigned in one branch and read in another before
reaching for `Value_<addr>`.

### Statement and control-flow shape

A chained assignment stores RIGHT TO LEFT. `work->f24 = work->f28 = f(...)`
writes +28 before +24; where the reference writes +24 first, the source is two
statements through a temporary. That was the entire residual on
`resource_39c:51b0`.

Apply-then-clamp and clamp-then-apply are different algorithms with different
bytes, and the reference tells you which it is by WHEN it re-reads. The
subtractive form that reproduces `080bbb0c`'s damage tails:

```c
    pp -= dmg;
    if (pp <= 0)
        pp = 0;                 /* not  if (pp <= dmg) { dealt = pp; pp = 0; }  */
```

and the additive form for its heals -- add, cap, then RE-DERIVE the display
delta from the field:

```c
    cur += dmg;
    if (cur > target->max_hp) {
        cur = target->max_hp;
        dmg = cur - target->hp; /* re-reads the field, not the saved copy */
    }
```

Where two guards return the same thing, the reference usually reaches one
shared block placed after the body. Writing `return 0` early puts an inline
copy near the top and burns a register the reference still has free. The shape
that matches is the body inside the `if` with the other value returned after
it -- and the same placement rule covers a guard ARM: `080bbb0c`'s case 3
emits its `hit == 0` arm AFTER the main body, which is
`if (hit != 0) { body } ...arm...`, not an early `if (hit == 0)` at the top.

Shared tails entered from several bodies are `goto` in the source, not
duplicated statements -- `080bbb0c` stores hp through one label from three
cases and pp through another from two. But only when crossjumping could not
have made them: identical short tails in plain arms merge on their own, and
writing the `goto` yourself where crossjumping would have done it changes
nothing or hurts.

An increment emitted after the last call of a loop body lives in the loop
TEST. `pass++;` as a statement emits `adds r5, #1` before whatever follows it;
the reference emitting it afterwards is
`while ((unsigned int)++pass <= 3)`.

An overlay function pointer is the LINKED address, not the overlay-relative
one. The pool word for `Func_02001d78` in an overlay linked at 0x02008000 is
0x02009d79: the routine, plus the Thumb bit, at its linked address. Read the
base off a finished sibling.

---

## Where the project is

Exact C stands at 275,672 bytes, 20.5% of the executable image. Permanent
assembly -- linker veneers, alignment, and routines whose instructions no C
can produce -- adds the rest of DONE, which is 37% of the ROM's executable
bytes and is what the sun in every commit subject reports.

Publish both figures or neither. The share of the ROM is the honest headline;
the share of the bytes that _can_ be C says how much of the job is left. That
second denominator is 1,130,844, and exact C is 24.4% of it.

### 100% is reachable, and the compiler is the minority obstacle

This is a measurement, not an opinion. All 1,276 overlay candidates from a
since-deleted tier were scored against the ROM. Counting only real owners --
32 bytes or more, so the 8-byte veneer stubs that score exact trivially are
excluded -- 1,242 of them classify as:

| share | class         | what it means                                    |
| ----: | ------------- | ------------------------------------------------ |
| 58.5% | size mismatch | a statement is missing or extra                  |
| 26.0% | wrong         | operands differ: a type, a prototype, a constant |
| 12.9% | ordering      | a scheduler tie                                  |
|  1.9% | allocation    | reload picked different registers                |
|  0.6% | unemittable   | a shape no stock gcc 2.96 emits                  |

**84.5% of what is left is ordinary reconstruction.** That understates it,
since the owners closed most recently included several labelled `ordering` or
`allocation` that were source problems after all. The sample is drawn from
sources someone already wrote and failed to close, so it is biased toward hard
cases -- which makes the finding stronger, not weaker.

### Large owners hold the bytes, and the method now exists

Broken out by size, the record is stark:

| owner size    |         byte-exact | share of open bytes |
| ------------- | -----------------: | ------------------: |
| under 128 B   | 819 of 1,211 (68%) |                5.0% |
| 128 B - 512 B |   169 of 834 (20%) |               29.4% |
| 512 B - 2 KB  |      4 of 246 (2%) |               36.6% |
| 2 KB and over |       0 of 49 (0%) |               29.1% |

No owner over 2 KB has been made byte-exact; the largest that has is 1,828
bytes. Owners of 512 B and over hold **65.6% of the remaining audited overlay
bytes**. Nothing about gcc 2.96 forbids matching a large function -- the
constraint is that every new local is a global colouring decision, and every
misread body poisons the aggregate scores. [Reconstructing a large
owner](#reconstructing-a-large-owner) is the method; `080bbb0c` is its working
example and its `recon/` record is the live state. Plan around the honest
numbers: a completion estimate extrapolated from the small owners is not a
forecast.

### Finish overlays, do not spread

Work one overlay to completion, smallest remaining first, rather than picking
owners by score across the whole ROM. Within one overlay the callees, types,
structures and idioms are shared, so every owner you finish makes the next one
cheaper, and `overlay twins` and `exact_reading_list` can actually find you a
worked example. Across the ROM that compounding is lost. It also changes what
a day's work produces: "this overlay is finished" is an artifact, "the number
went up 0.04%" is not.

We have finished most of the overlay owners by count and about a fifth of the
overlay bytes, because we did the small ones: the median adopted owner is 46
bytes and the median owner still parked is 204. What remains is filtered twice
over, once for size and once for having already failed. The way out is
reconstructing large functions by reading them.

### Veneers stay in the denominator

48,760 bytes of linker veneers and 3,020 of alignment are executable bytes in
the ROM, so they belong in the denominator. They can never be C, so a share
measured against that denominator can never reach 100. `resource_3cc` is
finished and reads 64%.

### Assets: named is the bar

An asset is finished when it is a standalone file whose name says what it is
-- `vale_night.png`, `rock_front.png`, `isaac_running_south_west.png`,
`djinn_venus.gif`, `growl.wav`, `good_morning.mid`, `alchemy.sf2`. A stem
carrying the ROM's own numbering is not that, whether the number is decimal
(`sfx_272`), a `resource_<hex>` index, or a short hex token standing in for
one.

Everything in a standard format but still ID-named is **Extracted**, which is
a real step and not the last one. Images are 1.8% Named and music 0.0%. Those
numbers were 32.6% and 99.6% until August 2026, when the top tier was being
awarded for the _category name_ -- anything called audio counted as finished
-- rather than for a file existing. Tiers are counted per file, so one
well-named member cannot promote its package.

### Where unfinished work lives

One owner, one file: `draft/<owner>.c`, committed, with `draft/<owner>.json`
beside it holding the measurements. `scratch/` is gitignored and holds
everything disposable. There is no third location and no copy step.

A byte is exact C or it is assembly. `draft/` does not weaken that:
`two-tier-check` still fails if unproven C appears where a coverage figure
could add it up, and no published number counts a draft. What `draft/` fixes
is survival and truth -- the file a session worked on is the file the next
session opens, on any machine, and there is never a question of which copy is
current.

**`draft/<owner>.c` -- the file.** Every scoring tool takes its path.
Adoption moves it to `exact/` when it matches and removes the record.
Unproven C in `draft/` is a workbench item, not an asset, and is never quoted
as coverage.

**`draft/<owner>.json` -- the record.** Measurements and a recipe, never ROM
bytes. The `score` object is what the tools reported.

```json
{
  "owner": "resource_3bd:13f8",
  "span_bytes": 6220,
  "score": {
    "candidate_bytes": 5908,
    "wrong_instructions": 629,
    "differing_halfwords": 2744,
    "class": "wrong"
  },
  "shape": { "calls": 703, "loops": 3, "memory_ops": 47 },
  "expressed": ["call sequence", "memory through a held pointer"],
  "unexpressed": [{ "op": "ldrsh", "count": 8, "where": "loop bodies" }],
  "rejected": [{ "shape": "one base local reassigned", "wrong": 671 }]
}
```

This is not the semantic tier returning. That tier stored unproven C and then
COUNTED it, which is how 862,856 bytes read as 74% coverage against a 20%
match rate. A record claims no bytes at all -- it carries the score the tools
reported, and that score says the owner does not reproduce. There is nothing
in it a coverage number could add up, and `check_unmatchable` fails a record
whose owner has since gone exact, whose `wrong_instructions` is 0, or which
carries no score at all.

**`scratch/` -- everything disposable.** Spelling sweeps, variant
directories, the drafter's raw output, per-owner tool work directories
(`scratch/candidate-show/<stem>`). Nothing here is committed, nothing here
survives a clone, and the publication gate refuses the directory outright if
it is ever staged.

**A draft is not progress, and its size is not its completeness.**
`overlay reconstruct` will happily fill `scratch/` with a thousand drafts in
eighty seconds. The dangerous part is not the files, it is the reporting: a
draft at 92% of the reference's SIZE sounds nearly finished, and of 348 drafts
at 85% of reference size or better, six matched 90% of their instructions and
the median matched ten percent. The figures that mean something are
`differing_halfwords`, `wrong_instructions`, and the normalised insn diff.
Quote those.

`semantic/regions.json` and `semantic/main-regions.json` survive as the
audited owner boundaries -- evidence about where an owner starts and ends,
which is independent of anyone's C.

---

## Settled questions

These were open, cost real effort, and are now closed by measurement. They are
recorded so nobody reopens them on a hunch, and so the shape of the evidence
is available if a residual ever looks like one of them again.

### The host does not reach the bytes

gcc 2.96 was never an FSF release: it is the upstream CVS tree between 2.95
and 3.0, which Red Hat shipped in Red Hat Linux 7.0 and 7.1 and upstream
disowned. The raw snapshot's own `version.c` reads
`2.96 20000731 (experimental)`, so the version string says nothing about which
distribution anyone was running. `PROVENANCE.txt` records that our base --
gcc-mirror `04179d4a511b` -- is the one Red Hat built gcc-2.96-54 from.

That raised a real worry, because `cse.c` hashes a `SYMBOL_REF` by the HOST
ADDRESS of its name string, and 2000-era Linux had no ASLR. It does not
matter: compiling the 577-symbol owner `resource_3bf:3054` with the
environment padded by 0, 8,000 and 60,000 bytes gives one identical hash, as
do five repeated runs under macOS ASLR. CSE decides equivalence by structural
comparison; the hash only orders buckets. Nobody needs a period-correct
machine image.

### Red Hat's patches do not reach these owners

Red Hat's shipped 2.96 is our base plus a patch series, and we build the bare
base. Development on the first game brackets the toolchain between Red Hat 7.0
(September 2000, `gcc-2.96-54`) and 7.1 (April 2001). `gcc-2.96-54.src.rpm` is
the era-correct one: same base tarball, 70 patches, 68 applied, changing 54
lines of `reload1.c`, 58 of `cse.c`, 38 of `local-alloc.c` and 24 of `calls.c`
against our tree.

Built in a 32-bit i386 container -- where the tree needs no host patches at
all -- and pointed at the owners whose residual is pure ordering:

| owner               | our compiler | Red Hat 7.0 | differing |
| ------------------- | -----------: | ----------: | --------: |
| `resource_3b8:3df8` |     11 insns |          11 |     **0** |
| `resource_37a:1380` |    128 insns |         128 |     **0** |
| `resource_3ce:029c` |    588 insns |         588 |     **0** |

Byte-identical assembly, including the 1,574-byte owner whose entire residual
is one swapped pair repeated at 196 call sites. Our unpatched tree is the
right compiler. Three owners is not the corpus, so this is strong evidence
rather than proof.

### The snapshot date has a five-day window

`PROVENANCE.txt` confirms the 3.0.0 tree and agbcc "by measurement" and names
what each beat. For gs1cc it says only "the 2000-07-31 development snapshot",
inherited because Red Hat used that base; nobody swept neighbouring commits.
Walking gcc's own history says how much room that leaves. For each pass, the
last change on or before 2000-07-31 and the next one after:

| pass                  | last change | next change       |
| --------------------- | ----------- | ----------------- |
| `reload1.c`           | 2000-07-28  | 2000-08-04        |
| `reload.c`            | 2000-07-28  | 2000-08-04        |
| `cse.c`               | 2000-07-28  | 2000-08-04        |
| `combine.c`           | 2000-07-30  | 2000-08-04        |
| `calls.c`             | 2000-07-18  | 2000-08-15        |
| `expr.c`              | 2000-07-12  | 2000-08-06        |
| `local-alloc.c`       | 2000-06-13  | 2000-08-04        |
| `global.c`            | 2000-06-13  | 2000-08-04        |
| `config/arm/thumb.c`  | 2000-04-08  | not again in 2000 |
| `config/arm/thumb.md` | 2000-04-08  | not again in 2000 |

Any CVS pull between 2000-07-30 and 2000-08-03 gives the same compiler for our
purposes, and ours sits in the middle of that window. The Thumb back end is
unchanged from April 2000 through the end of the year. A fine-grained bisect
is therefore pointless -- the granularity that could move bytes is WEEKS. A
sweep at that spacing is still worth doing, because a June tree, or a
September one once the 08-04 reload and cse changes land, IS a different
compiler.

### The scheduler tie-break is not version-dependent

`rank_for_schedule` was compared at gcc-mirror as it stood a year before our
snapshot, at the snapshot, and a year after. The final tie-break is the same
line in all three:

```c
/* If insns are equally good, sort by INSN_LUID (original insn order),
   so that we make the sort stable. ... */
return INSN_LUID (tmp) - INSN_LUID (tmp2);
```

2001 moved the interblock comparisons behind a callback and left the keys and
the tie-break untouched; gcc at HEAD, twenty-six years on, still ends the
function with that line. Our fork's `haifa-sched.c` is byte-identical to
upstream at the snapshot, so there is no local divergence to correct.

### What our fork still changes, and why

There are three kinds and only one is a gap.

**Host portability**, the bulk of it. `reload1.c`'s fourteen lines are
entirely `GEN_FCN (icode) (...)` becoming
`((insn_gen_fn3) GEN_FCN (icode)) (...)` -- a function-pointer cast so the
call has a correct prototype on a 64-bit host. These change how gcc is
COMPILED, not what it emits, and the Red Hat comparison above is the evidence:
two differently-patched compilers, one built on arm64 and one on i386,
agreeing instruction for instruction across three owners.

**The SYMBOL_REF hash** in `simplify-rtx.c`, which its own comment calls a
deviation rather than a fix. Measured inert: see the padding test above.

**`.align N, 0`** in `config/arm/elf.h`, one line, and this is the real gap.
It is not compensating for the compiler at all -- it is compensating for the
ASSEMBLER. Red Hat 7.0 shipped binutils 2.10.0.18; we assemble with 2.47,
whose default alignment fill is not zero. `binutils-2.10.0.18-1.src.rpm` sits
in the same Red Hat 7.0 archive as the compiler, and the container recipe that
built one will build the other. Doing that and dropping the `elf.h` line would
remove the last change we carry that moves bytes.

### A bare shiftable constant never pools

`thumb_shiftable_const` accepts any value expressible as an 8-bit immediate
shifted left, and `*thumb_movsi_insn` splits every such constant into
`movs`/`lsls`; the pool alternative exists only for constants that fail the
test. Six source spellings of a shiftable constant -- literal, if/else through
a variable, ternary, u16 and s16 locals, a folded sum -- all build inline,
none pool. So when the reference POOLS a value that passes the test (0x820 at
two of `080bbb0c`'s heal-text sites), the source expression there was not a
bare constant, or the value is not what the reading assumed. The question is
open per site; the compiler-side answer is closed.

### The permuter cannot reach an ordering residual

`alchemy_permuter` is a real port of pret's decomp-permuter, 29 randomisation
passes. On this corpus it closed nothing: 97 rows at 1,500 to 2,500 candidates
each improved 18 and matched 0. Pointed at the best targets the project has --
owners two halfwords from reproducing -- it did not improve them once, across
200,000 candidates and ten owners, with 89% of mutations failing to compile at
all.

That is not bad luck, it is the wrong instrument. The permuter searches SOURCE
SHAPE. An ordering residual is the post-reload scheduler choosing between two
independent instructions after the source has had its say. Aim it at rows the
score calls `wrong`, and read the residual first so you know which you have.
Treat a match it finds as something to explain before adopting.

### Smaller ones, so nobody pays twice

- **A matching file size is not a matching owner.** A 6,332-byte draft
  against a 6,332-byte reference still differed in 2,929 halfwords. Do not
  spend a session making the lengths agree.
- **Reusing a local still recolours if you extend its life.** The difference
  is the live range, not the name. Short-lived nested `off = 2` for one
  compare is safe; threading a loop through a dead variable is not.
- **Jump-table slots that share the after-switch address are empty cases.**
  Filling them with bodies makes the function longer and more wrong.
- **Bundling many `.c` files would not explain this output.** The owner is
  one translation unit at `-O2`. A static helper that is not inlined becomes a
  second symbol the scorer rejects.
- **`slots[50]` is not `base + 100`.** The register-offset form comes from a
  value the compiler cannot fold into the pointer; forcing it with a new
  allocno on a large owner is how the opening dies.
- **The tooling is frozen, except a flag that makes the loop faster.** The
  loop needs about eight commands and they all exist. A flag on an existing
  command that cuts confirmation from 510 ms to 50 ms is the loop, not a new
  tool; `--asm` was that flag, and fixing its canonicaliser was maintenance,
  not addition. Do not add a second way to git-diff two listings.

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

### The ones you will actually use

There are 95 public commands and the loop needs about eight of them. The full
catalog below exists so the registry has somewhere to be checked against; it
is reference, not a reading list.

|                                        |                                                           |
| -------------------------------------- | --------------------------------------------------------- |
| `candidate-show --asm`                 | gcc `-S` + git diff of insns; did this edit move anything |
| `candidate-show --align`               | linked-byte score for a main-image owner                  |
| `overlay score --align`                | the same for an overlay row                               |
| `overlay adopt` / `park`               | install a proven row, or take one back out                |
| `overlay audit --all`                  | re-prove every adopted row still reproduces               |
| `overlay candidate-rank` / `main-rank` | pick the next owner by what is wrong with it              |
| `overlay twins`                        | find a finished owner with the same shape                 |
| `overlay reconstruct`                  | draft C for a call-dense owner from its own disassembly   |
| `make verify`                          | the only result that proves anything                      |

Most of the rest are asset builders that run as part of `make build-assets`,
and gates that run as part of `make verify`. You should not need to invoke
them by hand, and if you find yourself doing so, that is worth a note in the
commit.

### decomp -- find owners and prove drafts

The main-image half of the loop: locate function-sized regions in the
executable image, survey what is still unresolved, and prove a candidate
against the bytes before it is allowed into `exact/`.

| Command             | What it tells you                                                                                                                    |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `discover`          | Produces the function, instruction and call discovery report from a local ROM.                                                       |
| `remaining_survey`  | Surveys the executable regions still unresolved.                                                                                     |
| `decomp_diagnose`   | Compiles a main-image candidate and classifies its residual; `--agent-brief` emits a bounded work contract with the canonical score. |
| `integrate_matches` | The main-image adoption gate: proves a `src_<address>.c` draft and installs it only with `--apply`.                                  |

### overlay -- the 96 loaded code modules

The overlay half, which is where most remaining work is. These decode overlay
bytes, resolve per-site veneers, rank candidates by residual class, find
repeated shapes, and gate adoption by rehearsing the whole overlay before
splicing a row.

| Command                    | What it tells you                                                               |
| -------------------------- | ------------------------------------------------------------------------------- |
| `overlay_disasm`           | Decodes overlay bytes to assembly.                                              |
| `overlay_show`             | Shows a byte range of one overlay, optionally annotated.                        |
| `overlay_entry`            | Reports overlay entry points.                                                   |
| `overlay_inventory`        | Rebuilds the diagnostic inventory the overlay tools read.                       |
| `overlay_gaps`             | Names unclaimed executable ranges.                                              |
| `overlay_unindexed`        | Names executable spans discovery has not indexed.                               |
| `overlay_twins`            | Finds owners that mirror one another, so a solved shape can be reused.          |
| `exact_reading_list`       | Lists finished owners worth reading as worked examples.                         |
| `overlay_candidate_rank`   | Ranks candidate residuals.                                                      |
| `overlay_call_order_check` | Compares resolved call order, including aliases and relocation spellings.       |
| `overlay_certify`          | Reports owner-certification findings; `--check` turns them into a failing gate. |
| `overlay_adopt`            | The overlay adoption gate: rehearses the whole overlay and refuses a mismatch.  |
| `overlay_showcase`         | Renders a representative finished overlay for regression and demonstration.     |
| `overlay_driver`           | The low-level overlay reconstruction driver.                                    |
| `overlay_mode_cohort`      | Compares one compiler hypothesis across a set of overlay owners.                |

The `overlay` binary also carries `score`, `park`, `audit` and `reconstruct`,
described in [the loop](#the-loop). `score` is the overlay counterpart of
`candidate-show`: same output, same `--align`, and it derives the row's span
rather than asking you for one.

### search -- bounded source and compiler exploration

Each of these holds one axis fixed and reports what changed. They are a last
resort for a residual you have already localised and understood, not a way to
find one. A result still has to be read as source and adopted through the
owner's gate.

| Command                 | What it tells you                                                                                             |
| ----------------------- | ------------------------------------------------------------------------------------------------------------- |
| `decomp_constraints`    | Derives bounded structural constraints for a candidate.                                                       |
| `shape_sweep`           | Tries bounded, behaviour-preserving source shapes with the compiler fixed; `--descend` drives it iteratively. |
| `search_compiler_modes` | Searches approved compiler modes with the source fixed.                                                       |
| `alchemy_permuter`      | A bounded source-permutation search with linked-byte scoring.                                                 |

### compiler -- routed builds and comparisons

Routed builds of the exact corpus and the flag matrix, plus the main-image
counterpart of the overlay ranker. Use these to prove a compiler hypothesis
across a cohort rather than on the single owner that suggested it.

| Command                      | What it tells you                                                                             |
| ---------------------------- | --------------------------------------------------------------------------------------------- |
| `compiler_corpus_regression` | Recompiles the exact corpus and reports any byte regression.                                  |
| `mode_sweep`                 | Searches the approved flag matrix for one fixed candidate.                                    |
| `mode_cohort`                | Tests one compiler hypothesis across a bounded set of owners.                                 |
| `main_candidate_rank`        | Ranks main-image candidates by residual class, as `overlay candidate-rank` does for overlays. |

The `compiler` binary additionally hosts `candidate-show` (`--asm` for gcc
`-S` plus git diff of canonicalised insns; `--align` / `--first` / `--patch`
for the linked-byte view), `reconstruct` (draft C from a main-image `.s` file
into `scratch/`), `thumb-disasm`, and the RTL readers `rtl-insn`, `rtl-sexpr`,
`rtl-schedule` and `rtl-align`, which read the compiler's own dumps when you
need to know which pass produced a shape.

### check -- the gates

The policy layer. These enforce the evidence and publication boundaries, the
two-tier rule, the architecture catalogue and the commit contract. A check
that scans nothing fails; a zero exit means the corpus was inspected.

| Command                 | What it tells you                                                                                        |
| ----------------------- | -------------------------------------------------------------------------------------------------------- |
| `architecture`          | Requires every crate, binary, dispatch target and path to be valid, reachable and catalogued here.       |
| `documented`            | Keeps the command groups and this catalog in step with the registry.                                     |
| `check_publication`     | Rejects ROMs, build products, opaque dumps, credentials and disallowed files.                            |
| `no_asm_c`              | Rejects inline assembly, register pins and assembly barriers in C and headers.                           |
| `source_citations`      | Checks that cited tools name paths that exist.                                                           |
| `check_unmatchable`     | Validates the two owner registers -- unmatchable and provisional -- and prints the queue with `--queue`. |
| `core_retained_audit`   | Requires retained main-image assembly to carry explicit justification.                                   |
| `cache_key_lint`        | Requires cached results to include every input that can change their meaning.                            |
| `check_commit_progress` | Compares your commit subject against the staged progress report.                                         |

### metrics -- measurement and charts

Everything that produces a published number or figure. `full_c_progress
--check` refuses to certify a share unless the last full build reproduced the
ROM byte-identically with no fallback bytes: ownership is a claim, and a
percentage over a failing build is not evidence.

| Command            | What it tells you                                                                         |
| ------------------ | ----------------------------------------------------------------------------------------- |
| `full_c_progress`  | Calculates, checks or writes the audited byte-exact report.                               |
| `full_c_history`   | Writes the first-parent progress ledger.                                                  |
| `coverage_map`     | Checks or regenerates the dashboard, the target index, the charts and the README figures. |
| `audit_residuals`  | Accounts for every remaining executable byte and its ownership class.                     |
| `compare_roms`     | Compares approved local editions without publishing byte diffs.                           |
| `dashboard_server` | Serves a live worktree dashboard on localhost.                                            |

One caution. `full_c_progress --write-inventory` re-derives
`metrics/gs1-en-executable.json`, and that file is the AUDIT rather than a
generated artifact. Regenerating it shrinks the permanent-assembly side and
drops the published DONE figure by a point or two with no change to the tree.
After adopting owners, write the report and regenerate the coverage map; leave
the inventory alone unless you are deliberately revising the audit, in which
case compare it entry by entry before committing.

### assets -- extraction and round-tripping

Decoding a resource from a local ROM, rendering it into a source asset, and
converting an edited source asset back into a build input. Each of these must
round-trip: an asset that does not re-encode to the same bytes is not
finished.

| Command            | What it tells you                                                             |
| ------------------ | ----------------------------------------------------------------------------- |
| `extract_resource` | Decodes one resource by id or address and can verify the round trip.          |
| `export_asset`     | Renders binary graphics, palette, byte and RGBA forms into source assets.     |
| `import_asset`     | Converts PNG, indexed-image and MIDI sources into build inputs.               |
| `tilemap`          | Round-trips the textual tilemap format.                                       |
| `bl_site_symbols`  | Recovers and audits branch-and-link symbol evidence, reporting every failure. |

### make -- build stages and asset builders

The build stages plus one builder per asset package. Nearly all of these run
as part of `make build-assets` and are listed here because the architecture
gate requires every dispatch target to be catalogued, not because you should
call them by hand.

| Command                    | What it tells you                                                             |
| -------------------------- | ----------------------------------------------------------------------------- |
| `build_claimed`            | Links the byte-exact owners.                                                  |
| `build_asm`                | Rebuilds the assembled stage.                                                 |
| `build_assets`             | Rebuilds the asset tree.                                                      |
| `build_full`               | Composes exact C, retained assembly and assets, and compares against the ROM. |
| `build_rom`                | Rebuilds the ROM.                                                             |
| `archive_asset`            | Rebuilds generic archive containers.                                          |
| `audio_engine_data`        | Rebuilds the audio engine data package.                                       |
| `audio_wave`               | Rebuilds audio samples.                                                       |
| `music`                    | Rebuilds sequences.                                                           |
| `music_residuals`          | Rebuilds residual audio ownership.                                            |
| `battle_effect_data`       | Rebuilds the battle-effect data package.                                      |
| `sentou_gamen_data`        | Rebuilds battle screen data.                                                  |
| `sentou_hyouji`            | Rebuilds battle display data.                                                 |
| `sentou_kouka_runtime`     | Rebuilds and verifies the battle-effect runtime.                              |
| `sentou_menu_data`         | Rebuilds battle menu data.                                                    |
| `sentou_resources`         | Rebuilds battle resources.                                                    |
| `encounter_data`           | Rebuilds encounter data.                                                      |
| `character_catalog`        | Rebuilds the character catalogue.                                             |
| `localization_font`        | Rebuilds the localisation font.                                               |
| `localization_tables`      | Rebuilds the localisation tables.                                             |
| `namae_nyuuryoku`          | Rebuilds the name-entry data.                                                 |
| `staff_roll`               | Rebuilds the credits data.                                                    |
| `title_resources`          | Rebuilds title-screen resources.                                              |
| `chiiki_map_resources`     | Rebuilds regional map resources.                                              |
| `tokushu_map_resources`    | Rebuilds special map resources.                                               |
| `kind1_map_grid`           | Rebuilds map grids.                                                           |
| `map_container_components` | Rebuilds map container components.                                            |
| `early_runtime_data`       | Rebuilds early runtime-support data.                                          |
| `runtime_support_data`     | Rebuilds runtime-support data.                                                |
| `late_runtime_residual`    | Rebuilds late runtime residuals.                                              |
| `executable_gap_sources`   | Rebuilds or classifies executable-gap inputs.                                 |
| `gba_header`               | Rebuilds the cartridge header.                                                |
| `f0_archive`               | Rebuilds the F0 archive format.                                               |
| `message_archive`          | Rebuilds message archives.                                                    |
| `wordstream`               | Rebuilds word streams.                                                        |
| `pairtable`                | Rebuilds pair tables.                                                         |
| `byte_henkan`              | Rebuilds byte-conversion tables.                                              |
| `byte_value_regions`       | Rebuilds byte-value regions.                                                  |
| `indexed_still`            | Rebuilds indexed still images.                                                |
| `static_sprite_series`     | Rebuilds static sprite families.                                              |
| `skip_sprite_archive`      | Rebuilds skipped sprite archives.                                             |
| `simple_resources`         | Rebuilds resources with a simple plan.                                        |
| `resource_directory`       | Rebuilds the resource directory.                                              |
| `resource_01c`             | Rebuilds resource 0x01c.                                                      |
| `resource_5`               | Rebuilds resource 5.                                                          |
| `resource_3ce`             | Rebuilds resource 0x3ce.                                                      |
| `resource_d1_d3`           | Rebuilds resources 0xd1–0xd3.                                                 |
| `resource_byte_canvases`   | Rebuilds byte-canvas resources.                                               |

---

## Build stages

Use the smallest stage that answers your question.

| Target               | What it does                                                                     |
| -------------------- | -------------------------------------------------------------------------------- |
| `make build-claimed` | Links the byte-exact owners. Fast; answers "does my owner still link and match?" |
| `make build-asm`     | Rebuilds the assembled stage.                                                    |
| `make inventory`     | Produces the overlay inventory the overlay tools read.                           |
| `make build-assets`  | Rebuilds the asset tree.                                                         |
| `make build-full`    | Composes everything and compares against the ROM.                                |
| `make build-rom`     | Rebuilds the ROM image.                                                          |
| `make progress`      | Prints the byte-exact share.                                                     |
| `make coverage`      | Refreshes the dashboard, the target index and the charts.                        |
| `make test`          | Lint plus every native self-test.                                                |
| `make lint`          | Architecture and policy gates.                                                   |
| `make verify`        | The authoritative gate.                                                          |

`make verify` is green only when the ROM rebuilds byte-identically with no
fallback bytes, every gate passes, and the tracked metrics match the tree.
That is the only result that proves anything.

---

## The compiler standard

Camelot shipped a makefile, not a per-file flag database. The build targets
the same shape: one flag set, plus a small number of sanctioned deviations.

```
-O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi
-fno-builtin -nostdinc -ffreestanding -fcall-used-r4 -Iinclude
```

The deviations are: one overlay stem that never interworks and so needs no
veneers; a small number of translation units built at `-O1`; and the stock m4a
audio engine, which is a different compiler rather than different flags.

Anything a source needs beyond this is **debt**, not configuration. It records
that the reconstruction is wrong and a flag is standing in for the fix.
`make standard-check` asserts that the documented flag set is the one the
build uses, and `make routing-debt` reports how many sources still deviate.

Do not add a compiler option that stock gcc 2.96 does not have. A byte match
reached by inventing an option is not a reconstruction; it moves the
difference out of the source, where it can be found, and into the compiler,
where it cannot. Adding a _stock_ option is allowed and is recorded as debt.

Price a stock option over whole translation units before routing it, not over
the owners it helps. An overlay whose own owners disagree about a flag was not
compiled with it, and a route that closes six rows by regressing seven is a
loss recorded as a gain.

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

`N` is the nearest whole `DONE` percentage -- exact C plus permanent assembly
-- from the staged progress report and the staged coverage map, so it matches
the figure on the README. `make progress-subject` prints it. The `commit-msg`
hook checks it, and both read the index rather than the working tree so an
unstaged metric cannot set the percentage of a commit that does not contain
it.

Agent-authored commits use the agent's own identity
(`Claude <noreply@anthropic.com>`, `Codex <noreply@openai.com>`); never a
human identity as the default author for a generated commit.

A deliberate regression or a denominator correction is fine, and must be
described plainly.

---

## Targets

This section is generated. It is the primary contributor target list:
non-overlapping audited source-owner scopes (or contiguous unresolved
executable runs), sorted largest to smallest. Broader multi-owner campaign cuts
belong in [Status](#status); they may overlap and therefore are not used for
byte accounting. Regenerate with `make coverage` -- do not edit by hand.

- **Unfinished scopes:** 2,232
- **Address spaces scanned:** 97 (87 still contain targets)
- **Target bytes:** 855,068 semantic-C or unresolved-assembly bytes
- **Resolved-only bytes:** 490,298 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,347,122

### Main target list

This table contains every scope of at least 1,000 bytes (228 rows). The complete
2,232-row index, including the smallest audited owners, is
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json).

| Rank | Scope | Target | Namespace / owner |
|---:|---:|---:|---|
| 1 | 6,332 | 6,332 | `main:0x080bbb0c` |
| 2 | 4,888 | 4,888 | `main:0x080ab5e4` |
| 3 | 4,224 | 4,224 | `main:0x08027114` |
| 4 | 4,138 | 4,138 | `resource_380:0x02002c10` |
| 5 | 4,122 | 4,122 | `resource_373:0x02002284` |
| 6 | 3,804 | 3,804 | `main:0x080f6440` |
| 7 | 3,702 | 3,702 | `resource_3c6:0x02000218` |
| 8 | 3,656 | 3,656 | `main:0x080dea70` |
| 9 | 3,442 | 3,442 | `resource_378:0x0200088c` |
| 10 | 3,408 | 3,408 | `resource_378:0x02001874` |
| 11 | 3,320 | 3,320 | `main:0x08023178` |
| 12 | 3,128 | 3,128 | `main:0x080a2680` |
| 13 | 3,104 | 2,810 | `main:0x080ad6d4` |
| 14 | 3,078 | 3,078 | `resource_3c9:0x0200124c` |
| 15 | 3,074 | 3,074 | `resource_383:0x02000b48` |
| 16 | 3,058 | 3,058 | `resource_3c5:0x0200186c` |
| 17 | 3,046 | 3,046 | `resource_395:0x02000470` |
| 18 | 3,046 | 3,046 | `resource_39d:0x02001adc` |
| 19 | 2,908 | 2,908 | `resource_381:0x020003cc` |
| 20 | 2,854 | 2,854 | `resource_374:0x0200155c` |
| 21 | 2,756 | 2,756 | `main:0x08023e70` |
| 22 | 2,734 | 2,734 | `resource_3a8:0x020026a8` |
| 23 | 2,672 | 2,672 | `resource_3a8:0x02000aac` |
| 24 | 2,574 | 2,574 | `resource_39e:0x02002ec8` |
| 25 | 2,534 | 2,534 | `resource_3bf:0x0200298c` |
| 26 | 2,508 | 2,508 | `main:0x080cbc0c` |
| 27 | 2,502 | 2,502 | `resource_3bf:0x02003c84` |
| 28 | 2,476 | 2,476 | `resource_3af:0x0200252c` |
| 29 | 2,444 | 2,444 | `main:0x080eb754` |
| 30 | 2,426 | 2,426 | `resource_3b8:0x0200338c` |
| 31 | 2,418 | 2,418 | `resource_38d:0x02000894` |
| 32 | 2,388 | 2,388 | `main:0x080f7460` |
| 33 | 2,386 | 2,386 | `resource_3b8:0x02001d7c` |
| 34 | 2,382 | 2,382 | `resource_383:0x02003c6c` |
| 35 | 2,356 | 2,356 | `main:0x080d2464` |
| 36 | 2,342 | 2,342 | `resource_3aa:0x020010f0` |
| 37 | 2,316 | 2,316 | `main:0x0808f52c` |
| 38 | 2,310 | 2,310 | `resource_3b8:0x02002a84` |
| 39 | 2,300 | 2,300 | `main:0x08021e6c` |
| 40 | 2,298 | 2,298 | `resource_380:0x02000a98` |
| 41 | 2,294 | 2,294 | `resource_3c9:0x02002360` |
| 42 | 2,268 | 2,268 | `main:0x080d91dc` |
| 43 | 2,236 | 2,236 | `resource_39e:0x02001494` |
| 44 | 2,230 | 2,230 | `resource_3b8:0x02000b40` |
| 45 | 2,230 | 2,230 | `resource_3bd:0x02001d4c` |
| 46 | 2,222 | 2,222 | `resource_3bc:0x02001a08` |
| 47 | 2,218 | 2,218 | `resource_378:0x020027d4` |
| 48 | 2,218 | 2,218 | `resource_3c9:0x02003e8c` |
| 49 | 2,206 | 2,206 | `resource_377:0x0200063c` |
| 50 | 2,194 | 2,194 | `resource_376:0x02000640` |
| 51 | 2,190 | 2,190 | `resource_396:0x0200069c` |
| 52 | 2,180 | 2,180 | `resource_3bd:0x02002c38` |
| 53 | 2,170 | 2,170 | `resource_3ca:0x02000430` |
| 54 | 2,166 | 2,166 | `resource_38f:0x020011ec` |
| 55 | 2,166 | 2,166 | `resource_3c9:0x02002c58` |
| 56 | 2,158 | 2,158 | `resource_373:0x02001a14` |
| 57 | 2,138 | 2,138 | `main:0x08026080` |
| 58 | 2,124 | 2,124 | `main:0x08024934` |
| 59 | 2,118 | 2,118 | `resource_371:0x020028e8` |
| 60 | 2,106 | 2,106 | `resource_3bd:0x020013d4` |
| 61 | 2,070 | 2,070 | `resource_3b1:0x02001b34` |
| 62 | 2,068 | 2,068 | `resource_3c2:0x02000240` |
| 63 | 2,052 | 2,052 | `resource_39e:0x020038d8` |
| 64 | 2,024 | 2,024 | `main:0x08077428` |
| 65 | 2,018 | 2,018 | `resource_3a5:0x020004e4` |
| 66 | 2,014 | 2,014 | `resource_3c9:0x02000a6c` |
| 67 | 2,006 | 2,006 | `resource_3a4:0x02001830` |
| 68 | 2,002 | 2,002 | `resource_38d:0x020019b0` |
| 69 | 2,002 | 2,002 | `resource_3af:0x02003a0c` |
| 70 | 1,980 | 1,980 | `main:0x080acab8` |
| 71 | 1,978 | 1,978 | `resource_3ad:0x02000808` |
| 72 | 1,968 | 1,968 | `main:0x080e823c` |
| 73 | 1,920 | 1,920 | `main:0x080f7f78` |
| 74 | 1,876 | 1,876 | `main:0x0802592c` |
| 75 | 1,854 | 1,854 | `resource_3c8:0x020026f8` |
| 76 | 1,822 | 1,822 | `resource_38f:0x02001e6c` |
| 77 | 1,816 | 1,816 | `main:0x080e99c0` |
| 78 | 1,816 | 1,712 | `main:0x08090a5c` |
| 79 | 1,810 | 1,810 | `resource_383:0x02002fd4` |
| 80 | 1,808 | 1,808 | `resource_3b1:0x02005c9c` |
| 81 | 1,796 | 1,664 | `main:0x080f3078` |
| 82 | 1,778 | 1,778 | `resource_3b9:0x02001cd4` |
| 83 | 1,770 | 1,770 | `resource_39e:0x020027dc` |
| 84 | 1,770 | 1,770 | `resource_3b1:0x020028c8` |
| 85 | 1,768 | 1,768 | `main:0x080d52c8` |
| 86 | 1,734 | 1,734 | `resource_383:0x02001e80` |
| 87 | 1,724 | 1,724 | `main:0x080e89ec` |
| 88 | 1,720 | 1,720 | `main:0x080e2974` |
| 89 | 1,704 | 1,704 | `resource_3a2:0x02000924` |
| 90 | 1,702 | 1,702 | `resource_381:0x02002150` |
| 91 | 1,702 | 1,702 | `resource_3b7:0x020001d8` |
| 92 | 1,698 | 1,698 | `resource_3ae:0x02000ba0` |
| 93 | 1,688 | 1,688 | `main:0x080b63c8` |
| 94 | 1,682 | 1,682 | `resource_39c:0x02004888` |
| 95 | 1,680 | 1,680 | `main:0x080a6ccc` |
| 96 | 1,660 | 1,660 | `main:0x080de2f8` |
| 97 | 1,648 | 1,648 | `main:0x080ed408` |
| 98 | 1,648 | 1,648 | `resource_3a2:0x020001dc` |
| 99 | 1,640 | 1,640 | `main:0x0800aa0c` |
| 100 | 1,638 | 1,638 | `resource_373:0x02004084` |
| 101 | 1,636 | 1,636 | `main:0x0800cacc` |
| 102 | 1,632 | 1,632 | `resource_39c:0x02002f58` |
| 103 | 1,626 | 1,626 | `resource_375:0x020000dc` |
| 104 | 1,614 | 1,614 | `resource_3bf:0x02003374` |
| 105 | 1,592 | 1,592 | `main:0x080168f4` |
| 106 | 1,588 | 1,588 | `main:0x08022b44` |
| 107 | 1,586 | 1,586 | `resource_3c5:0x02001238` |
| 108 | 1,576 | 1,576 | `main:0x080d2d98` |
| 109 | 1,556 | 1,556 | `main:0x080cf8e0` |
| 110 | 1,550 | 1,550 | `resource_379:0x02000484` |
| 111 | 1,540 | 1,540 | `main:0x080e3aa0` |
| 112 | 1,514 | 1,514 | `resource_3a4:0x02000d24` |
| 113 | 1,512 | 1,512 | `main:0x080e4e0c` |
| 114 | 1,504 | 1,504 | `main:0x08012518` |
| 115 | 1,480 | 1,480 | `resource_399:0x02000f84` |
| 116 | 1,468 | 1,468 | `main:0x080dbc30` |
| 117 | 1,462 | 1,462 | `resource_373:0x020039e8` |
| 118 | 1,458 | 1,458 | `resource_3c8:0x020047ac` |
| 119 | 1,448 | 1,448 | `resource_3b0:0x02000af8` |
| 120 | 1,448 | 1,448 | `resource_3b1:0x020050dc` |
| 121 | 1,410 | 1,410 | `resource_383:0x020036e8` |
| 122 | 1,408 | 1,408 | `main:0x08020244` |
| 123 | 1,402 | 1,402 | `resource_3b1:0x0200234c` |
| 124 | 1,398 | 1,398 | `resource_372:0x02001b18` |
| 125 | 1,382 | 1,382 | `resource_3c9:0x02003924` |
| 126 | 1,378 | 1,378 | `resource_380:0x02001780` |
| 127 | 1,376 | 1,376 | `main:0x0801a98c` |
| 128 | 1,366 | 1,366 | `resource_37a:0x02000488` |
| 129 | 1,366 | 1,366 | `resource_3aa:0x0200077c` |
| 130 | 1,360 | 1,360 | `main:0x080c91dc` |
| 131 | 1,336 | 1,336 | `main:0x080a9f10` |
| 132 | 1,326 | 1,326 | `resource_391:0x02002004` |
| 133 | 1,324 | 1,324 | `main:0x080bf678` |
| 134 | 1,310 | 1,310 | `resource_373:0x020034c8` |
| 135 | 1,308 | 1,308 | `main:0x080aa768` |
| 136 | 1,308 | 1,308 | `resource_37a:0x02002094` |
| 137 | 1,306 | 1,306 | `resource_3a8:0x02000590` |
| 138 | 1,290 | 1,290 | `resource_3af:0x02000c94` |
| 139 | 1,274 | 1,274 | `resource_387:0x0200066c` |
| 140 | 1,272 | 1,272 | `main:0x080b0aac` |
| 141 | 1,270 | 1,270 | `resource_371:0x02001064` |
| 142 | 1,270 | 1,270 | `resource_3b1:0x020037d8` |
| 143 | 1,264 | 1,264 | `main:0x08099da4` |
| 144 | 1,262 | 1,262 | `resource_38d:0x020012a0` |
| 145 | 1,252 | 1,178 | `main:0x0800f2f8` |
| 146 | 1,248 | 1,248 | `main:0x080d9ae8` |
| 147 | 1,238 | 1,238 | `resource_373:0x02004b24` |
| 148 | 1,234 | 1,234 | `resource_39d:0x02001608` |
| 149 | 1,234 | 1,234 | `resource_3af:0x02001db0` |
| 150 | 1,230 | 1,230 | `resource_371:0x02001f6c` |
| 151 | 1,230 | 1,230 | `resource_374:0x02000b8c` |
| 152 | 1,228 | 1,228 | `main:0x080b6f44` |
| 153 | 1,226 | 1,226 | `main:0x080f4f04` |
| 154 | 1,222 | 1,222 | `resource_37a:0x0200155c` |
| 155 | 1,218 | 1,218 | `resource_37b:0x02000554` |
| 156 | 1,210 | 1,210 | `resource_391:0x02001740` |
| 157 | 1,204 | 1,204 | `main:0x080ce034` |
| 158 | 1,198 | 1,198 | `resource_3b1:0x020057ec` |
| 159 | 1,192 | 1,192 | `main:0x080da6cc` |
| 160 | 1,190 | 1,190 | `resource_3bd:0x02002604` |
| 161 | 1,182 | 1,182 | `resource_3c9:0x02004738` |
| 162 | 1,180 | 1,180 | `resource_37b:0x0200101c` |
| 163 | 1,176 | 1,176 | `resource_37b:0x02000a18` |
| 164 | 1,176 | 1,176 | `resource_380:0x020043bc` |
| 165 | 1,174 | 1,174 | `resource_3bc:0x020040bc` |
| 166 | 1,170 | 1,170 | `resource_370:0x020003cc` |
| 167 | 1,170 | 1,170 | `resource_3b3:0x0200174c` |
| 168 | 1,168 | 1,168 | `main:0x080a4924` |
| 169 | 1,154 | 1,154 | `resource_37f:0x02001604` |
| 170 | 1,152 | 1,152 | `main:0x080191cc` |
| 171 | 1,152 | 1,152 | `main:0x0801d4cc` |
| 172 | 1,152 | 1,152 | `main:0x080f26ec` |
| 173 | 1,150 | 1,150 | `resource_375:0x020014f4` |
| 174 | 1,144 | 1,144 | `main:0x080f2028` |
| 175 | 1,142 | 1,142 | `resource_373:0x02004ffc` |
| 176 | 1,128 | 1,128 | `main:0x080d0000` |
| 177 | 1,126 | 1,126 | `resource_37f:0x02000f6c` |
| 178 | 1,116 | 1,116 | `main:0x080c02a4` |
| 179 | 1,114 | 1,114 | `resource_3c7:0x02000880` |
| 180 | 1,110 | 1,110 | `resource_372:0x02003394` |
| 181 | 1,110 | 1,110 | `resource_3ce:0x02000244` |
| 182 | 1,106 | 1,106 | `resource_39e:0x02001dbc` |
| 183 | 1,104 | 1,104 | `main:0x080beb08` |
| 184 | 1,102 | 1,102 | `resource_3bb:0x020010dc` |
| 185 | 1,102 | 1,102 | `resource_3c9:0x02004fec` |
| 186 | 1,098 | 1,098 | `main:0x080fae58` |
| 187 | 1,096 | 1,096 | `main:0x080d41a4` |
| 188 | 1,096 | 1,096 | `resource_3b9:0x020011c4` |
| 189 | 1,094 | 1,094 | `resource_371:0x02000c1c` |
| 190 | 1,094 | 1,094 | `resource_3bc:0x02001474` |
| 191 | 1,090 | 1,090 | `resource_372:0x020037ec` |
| 192 | 1,090 | 1,090 | `resource_39e:0x0200064c` |
| 193 | 1,078 | 1,078 | `resource_373:0x020015dc` |
| 194 | 1,078 | 1,078 | `resource_373:0x020046ec` |
| 195 | 1,078 | 1,078 | `resource_399:0x020019bc` |
| 196 | 1,076 | 1,076 | `main:0x080030f8` |
| 197 | 1,074 | 1,074 | `main:0x080f4318` |
| 198 | 1,074 | 1,074 | `resource_3ba:0x02000db8` |
| 199 | 1,066 | 1,066 | `main:0x080e5e28` |
| 200 | 1,056 | 1,056 | `main:0x080ae2f4` |
| 201 | 1,054 | 1,054 | `resource_39d:0x020011e8` |
| 202 | 1,054 | 1,028 | `main:0x080d77b4` |
| 203 | 1,052 | 1,052 | `resource_3b1:0x02004254` |
| 204 | 1,052 | 978 | `main:0x0808bec0` |
| 205 | 1,050 | 1,050 | `resource_372:0x020028a4` |
| 206 | 1,050 | 1,050 | `resource_3aa:0x02000360` |
| 207 | 1,050 | 1,050 | `resource_3aa:0x02000cd4` |
| 208 | 1,046 | 1,046 | `resource_380:0x020027f8` |
| 209 | 1,046 | 1,046 | `resource_381:0x02001b34` |
| 210 | 1,044 | 1,044 | `main:0x0808d9a4` |
| 211 | 1,044 | 1,044 | `main:0x080bfba4` |
| 212 | 1,044 | 1,044 | `main:0x080cb7f8` |
| 213 | 1,042 | 1,042 | `resource_377:0x02000f90` |
| 214 | 1,042 | 1,042 | `resource_3a5:0x02001490` |
| 215 | 1,042 | 1,042 | `resource_3c9:0x02004bd8` |
| 216 | 1,040 | 1,040 | `main:0x080ceb54` |
| 217 | 1,040 | 1,040 | `main:0x080e90a8` |
| 218 | 1,038 | 1,038 | `resource_379:0x02000074` |
| 219 | 1,034 | 1,034 | `resource_39d:0x02000ddc` |
| 220 | 1,030 | 1,030 | `resource_37b:0x020015d4` |
| 221 | 1,030 | 1,030 | `resource_38f:0x02001a64` |
| 222 | 1,030 | 1,030 | `resource_391:0x02001bfc` |
| 223 | 1,028 | 1,028 | `main:0x0801de5c` |
| 224 | 1,026 | 1,026 | `resource_37b:0x02000150` |
| 225 | 1,022 | 1,022 | `resource_3b8:0x020006dc` |
| 226 | 1,020 | 1,020 | `resource_3b9:0x02000db0` |
| 227 | 1,010 | 1,010 | `resource_37a:0x02001ca0` |
| 228 | 1,006 | 1,006 | `resource_394:0x020003f0` |
