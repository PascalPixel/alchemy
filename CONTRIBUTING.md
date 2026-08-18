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
- [The loop](#the-loop)
- [Reading a residual](#reading-a-residual)
- [Source style](#source-style)
- [Where the project is](#where-the-project-is)
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

A region is permanently assembly when its instructions could not have come from
this compiler. `asm/classification.json` records the claim as a kind, a
confidence and a list of evidence, and `core_retained_audit` reads that evidence
back off the file for every tag it can decide mechanically: `arm_instruction_set`
must show `.arm`, `fixed_ldr_r4_bx_r4_literal` the veneer pair,
`manual_return_address_preserved_in_ip` both `mov ip, lr` and `bx ip`.

Claims that rest on judgement rather than a visible property cannot be checked
that way. They are not weaker for it, but they are the ones to read sceptically
before adding to them.

This is what permanent looks like, and no C expresses it:

```
Func_080f9b4c:
	mov	ip, lr          @ the return address is kept in ip
	bl	Func_080f9ab4   @ ...because this bl clobbers lr
	...
	bx	ip              @ and there is no stack save anywhere
```

Claim permanence only from what the instructions show. An owner is not
permanent because it is hard.

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

## Setting up

You need a Rust toolchain, `arm-none-eabi-binutils`, and the approved ROM set in
`roms/` (gitignored — put it there yourself).

The tooling is Rust. Not Python, not shell, not a scripting language that
happens to be installed: a tool that is not Rust is not in this repository, and
`architecture` will not catalogue it. Write the throwaway in whatever you like,
outside the tree.

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

Then confirm the tree is healthy:

```bash
make build-claimed   # links every byte-exact owner
make verify          # the authoritative gate
```

---

## The loop

Read the target assembly, work out what C the original author wrote, write that
C, compile it, compare the bytes, fix the difference. Repeat until identical.

What this repository adds is **feedback**. You are not diffing by eye. The tools
compile your candidate, link it at its real address, compare it against the
reference bytes, and say how far off you are and in what way. A gate then
refuses to adopt anything that does not reproduce.

There are two tiers and no third place for work to sit. A byte is exact C that
rebuilds identically, or it is assembly. Your unfinished candidate lives in
`work/`, which is gitignored — not in the tree, and not in a commit.

### 1. Pick an owner

An *owner* is one function-sized region with a fixed address.

- Main image: assembly in `asm/<address>.s`, C in `exact/<address>.c` once it
  matches.
- Code overlays: assembly in `assets/code/resource_<id>_overlay.s`, C in
  `exact/resource_<id>_c_<address>.c` once it matches.

**[Targets](#targets) ranks byte runs, not functions.** Its rows are contiguous
unresolved spans, so the address at the top of a row is where the run starts,
which is frequently the middle of a literal pool rather than an entry point.
`resource_3bf:0x0200298c` and `resource_373:0x02002284` both sit in the first
ten rows and both disassemble as data. Use the table to see the shape of what
is left, not to pick.

Pick from `semantic/regions.json`, which holds 2,340 audited owner boundaries —
an overlay, an entry address and a span — of which 1,348 are unfinished. Those
addresses are entry points, and the biggest of them are ordinary functions:

```bash
python3 - <<'EOF'
import json, os
d = json.load(open('semantic/regions.json'))
exact = set(os.listdir('exact'))
rows = [(r['span_bytes'], r['overlay'], int(r['entry'], 16))
        for r in d['manual_regions']
        if f"{r['overlay']}_c_{int(r['entry'], 16):08x}.c" not in exact]
for n, ov, a in sorted(rows, reverse=True)[:10]:
    print(f'{n:>6}  {ov}:{a & 0xffff:04x}')
EOF
```

Two cheap checks before committing to one. The instruction mix decides how hard
it is: `resource_3bf:3054` is 5,604 bytes but 1,168 of its 2,187 instructions
are `movs` and 636 are `bl`, with six compares in the whole function — a
scripted sequence, far easier than a third of its size in dense arithmetic.
And count the finished owners in the same overlay: `resource_3bf` has 101, so
the structs, callees and idioms are already established.

`overlay twins` finds owners that mirror one you have already finished, which is
usually the cheapest next thing to do — a 90-byte shape that appears four times
pays for itself three more times. `discover` and `remaining_survey` map what is
left.

Name your draft after the owner: `work/080bbb0c.c`, or
`work/resource_3b2_c_02000da4.c`. The rankers find drafts by that name.

For a call-dense owner -- a scene driver, hundreds of `bl` with literal
arguments -- `overlay reconstruct resource_3bd:13f8` writes a first draft into
`work/` from the owner's own disassembly: the calls and their arguments, the
literal pools skipped rather than decoded, a value held in a callee-saved
register recognised as a local, loads and stores through a held pointer, and
counted loops with their real bounds.

It is a starting point and not a reconstruction. It models what the bytes
plainly show and is silent about everything else -- it writes no `if`, so the
control flow is yours to read and add. Its output goes to `work/` and nowhere
else; the tool refuses any other path, because a generator aimed at every owner
is exactly the machine that produced 862,856 bytes of C nobody could use. Score
it before believing any of it.

### 2. Read the assembly before writing any C

C written before you understand the target is guesswork with extra steps. A
decompiler is a reading aid; its output is a scaffold to check against the ROM,
never a source draft. Get these facts first:

- **Boundaries and entry.** Prologue shape, frame size, which registers are
  saved, stack slots and arguments, and whether a shared tail is reached from
  elsewhere.
- **Types and signedness**, read off the load widths. `ldrb`/`ldrsb`/`ldrh`/
  `ldrsh` are declarations, not hints. A `lsls`/`asrs` pair is a sign extension
  the source asked for.
- **Control flow.** Loop heads and bottoms, switch and jump-table layout,
  branch cascades, fall-through seams.
- **Aggregate layout.** Which offsets cluster around which base registers, and
  which cells are read through one pointer and written through another.
- **Constants.** Which live in the literal pool and which are built inline. A
  pooled word and an inline `movs` are different source spellings.
- **Calls and side effects**, and every value's lifetime across them.

`overlay disasm`, `overlay show` and `overlay entry` decode overlay bytes.

Before you start, check whether the owner is C at all. A region is permanently
assembly only when its instructions could not have come from this compiler —
see [Permanent assembly](#permanent-assembly). Grepping the span for `stmia`,
`ldmia`, `swi` and the `mov ip, lr` / `bx ip` pair takes a second and tells you
whether you are looking at a reconstruction job or a registered exception.

### 3. Write the compiler's input, not its output

The ROM is optimised compiler output. The C you want is what went *in*: plain
structs, arrays indexed by loop variables, ordinary expressions, natural
statement order. The optimiser then reproduces the ROM's shape by itself,
because that is exactly what it did the first time.

The common mistake is transcribing the optimiser's work back into the source —
hand-strength-reduced pointer walks instead of `arr[i]`, hand-shared temporaries
instead of repeating an expression, hoisted invariants. That does not lock in
the ROM's shape; it changes what the earlier passes see, and the result diverges
somewhere else.

A loop whose exit test is at the top reads as a `goto` in the disassembly. Write
the loop. Scaffolding it as `x = g; goto test; body: …; test: if (x) goto body;`
leaves the first read of `g` used only inside the test block, so it sinks there,
and the reference loads it before the loop.

The bound runs both ways: write machine-producing structure, not modern style.
Prefer the shape a 2001 author would have written.

### 4. Score it and read the residual

```bash
# a main-image owner
cargo run --release --manifest-path tools/compiler/Cargo.toml -- \
  candidate-show work/080bbb0c.c --align

# an overlay row -- same output, same flag, no span argument
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  score work/resource_3b2_c_02000da4.c --align
```

**Read the diff, not the number.** `--align` pairs the two instruction streams
as sequences, so an extra or missing instruction shows up as itself:

```
  + push  {lr}                          <- we emit this, the reference does not
  ! ldr   r3, [pc, #20]    ldr r0, ...  <- both have it, operands differ
    ldr   r3, [r0, #0]     ldr r3, ...  <- identical
```

Without `--align` the two sides are matched by offset, which only works while
they are the same length: one extra instruction shifts everything after it and
every later row reads as a difference. The tool warns you when that applies.

Then make the smallest source change that explains what you saw, and score
again.

**A large owner still gives partial signal.** The matching prefix at the top of
`--align` is real: on `080bbb0c`, 6,332 bytes in a single function, an opening
that merely keeps enough values live across calls reproduces the whole
seven-instruction register-save sequence exactly. Each block you write correctly
extends that prefix, and the first `+`/`-` row is the next thing to write. Only
the frame size — `sub sp, #100` — waits for the whole body, because the frame is
as big as the widest spill anywhere in it.

`overlay candidate-rank` and `compiler main-candidate-rank` rank the drafts in
`work/` by what is wrong with them rather than by how much differs. Read
[Reading a residual](#reading-a-residual) before picking from either.

### 5. Adopt

Adoption is gated. It rebuilds and compares before it accepts anything, and it
refuses everything else — a candidate that is 32 bytes against a 6,332-byte
reference is rejected at `+0x0` with both sizes named.

```bash
# main image: proves a src_<address>.c draft, installs to exact/
make dispatch-decomp ARGS='integrate_matches /path/to/draft --apply'

# overlay: rehearses the whole overlay, then splices the row in
cargo run --release --manifest-path tools/overlay/Cargo.toml -- \
  adopt resource_373:034c --source work/resource_373_c_0200034c.c --apply
```

An overlay row is spliced into a fixed-size hole, so the gate checks the region
boundary, that no label outside the region is destroyed, and that the whole
overlay still assembles to the same bytes. Copying a file into `exact/` is not
adoption and will not survive the build.

**Run one adoption at a time.** It reaches that verdict by splicing the row into
`assets/code/<overlay>.s` and restoring the file afterwards, so it writes to the
tree even when it rejects and even without `--apply`. `OverlayLock` serialises
it per overlay; score in parallel as much as you like, and keep adoption serial.

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

`park` restores a row's assembly and returns its C to your worktree. `audit`
compares every adopted row against the bytes it replaced and names any that no
longer reproduce. Run it after any merge that brought in adoptions from both
sides.

### 6. When an owner will not converge

Take another owner. A main-image owner needs nothing done to it — the build
falls back to `asm/<address>.s` whenever `exact/<address>.c` is absent. An
overlay row must be parked with the command above, because its assembly has to
come back into the overlay.

Understanding accumulates: every owner you finish becomes a worked example of
what correct source looks like for this compiler, and the next one is easier
because of it. `exact_reading_list` and `overlay twins` are how you find the
relevant examples.

### Merging another branch, or main back into yours

Every branch that adopts an owner rewrites the same generated artifacts, so a
merge between two working branches conflicts on all of them and neither side is
right afterwards. Do not resolve them by choosing:

```bash
git merge main -m '☀️ N% – Merge main: <what came in>'
make coverage      # regenerate the Targets section, figures and metrics
make verify        # refuses a stale artifact, so this is the proof
overlay audit --all
```

Pass `-m` yourself: `commit-msg` requires the progress prefix and git's default
`Merge branch 'main'` does not have one, so a merge with no conflicts at all
still stops at the hook with the result already staged.

`CONTRIBUTING.md` and `README.md` still conflict, in the Targets counts and the
figure hashes. Take either side there and let `make coverage` correct it; the
prose around them is hand-written and merges normally, which is exactly why
those two are not auto-resolved.

`metrics/gs1-en-executable.json` is the audit, not a generated file. When both
sides have touched it, compare the two entry by entry before choosing — twice
now the difference has been interval coalescing with identical byte totals,
which is safe to drop, and a real change would not be.

---

## Reading a residual

A differing-halfword count says how much differs. It does not say what kind of
problem you have, and the two are not related. `overlay score` and the rankers
classify every residual, and the class is the first thing to read:

| class | what it means | worth reading? |
|---|---|---|
| `exact` | the bytes match | adopt it |
| `wrong` | some instruction is genuinely different | **yes** |
| `size mismatch` | a statement is missing or extra | **yes** |
| `ordering` | same instructions, different order | rarely |
| `allocation` | same instructions and operands, different registers | rarely |
| `unemittable` | the reference *appears* to use an instruction this compiler cannot emit | confirm first |

Rank `wrong` and `size mismatch` first. A small halfword count on a blocked row
is not a near miss.

`ordering` is settled after the source has had its say -- but by RELOAD, not by
the scheduler, and the difference matters because it changes what the source can
reach. Dumping every pass with `-da` on `resource_3b8:3df8` shows where the
order is actually set:

| pass | insn chain around the pair |
|---|---|
| `17.lreg`, before reload | `33 37 39 41 43 ...` -- the `-16` is still a pseudo |
| `18.greg`, after reload | `33 66 67 39 41 ...` -- reload MADE 66 and 67 and put them here |
| `23.sched2`, after scheduling | `33 66 67 41 39 ...` -- only 39 and 41 swapped |

Reload materialises the constant into two insns and places them before the
other argument's load. The scheduler then leaves that trio exactly as reload
left it. The reference wants `66, 41, 67`, which the scheduler was never in a
position to produce.

So reading only the sched2 dump gives the wrong answer. The tie there is real
and total, and every key is equal -- but the tie-break never got to decide these
three, because reload had already fixed their order. Reload's placement responds
to live ranges and register pressure, which the source does influence. That is a
different and more workable problem than a scheduler coin-flip.

The scheduler tie is still worth understanding, so here it is. On `resource_3b8:3df8` the whole residual is one pair, and the sched2 dump
shows why it cannot be reached:

| uid | insn | priority | dep count | cost | dependents |
|---|---|---:|---:|---:|---:|
| 67 | `r2 = -r2` | 67 | 2 | 1 | 5 |
| 41 | `r1 = 2` | 67 | 2 | 1 | 5 |

Every key `rank_for_schedule` consults is equal, including the dependent sets,
which are the same five insns. gcc then takes them in ready-list array order,
which follows RTL order, and the reference's RTL has `r1 = 2` between
`r2 = 16` and its `neg` where ours has the pair adjacent. Four spellings were
measured -- the bare literal, a prototype on the callee, the constant in a
local, and `0 - 16` -- and all four give the same two differing halfwords.

The snapshot is not the lever either, and that is now checked against
upstream rather than assumed. `rank_for_schedule` was compared at
gcc-mirror/gcc as it stood a year before our 2000-07-31 snapshot, at the
snapshot, and a year after. The final tie-break is the same line in all three:

```c
/* If insns are equally good, sort by INSN_LUID (original insn order),
   so that we make the sort stable. ... */
return INSN_LUID (tmp) - INSN_LUID (tmp2);
```

1999 and 2000 differ only in comment capitalisation and a `GENERIC_PTR` to
`PTR` typedef rename. 2001 moved the interblock comparisons behind a
`current_sched_info->rank` callback and left the keys and the tie-break
untouched. 2002's comparator is identical to 2001's. And gcc at HEAD, twenty-six
years on, still ends `rank_for_schedule` with that same line.

Our fork's `haifa-sched.c` is byte-identical to upstream at the snapshot, so
there is no local divergence to correct, and no version of gcc ever written
decides this tie differently. The compiler-snapshot axis is closed.

So an `ordering` residual is not a source problem in the sense that reading
harder fixes it. Read it once to confirm it is only order, record it, and move
on. `allocation` is the same story one pass later, in reload. They are not always immovable — a standalone
`x = p->field;` statement gives the scheduler a free-floating load to hoist,
where folding the read into the expression that uses it can anchor it — but
`shape_sweep` exhausts the bounded transforms quickly, and when it reports the
axis exhausted, believe it.

`unemittable` is advisory and wrong about one time in five. It reads a
disassembled stream, and a literal pool word disassembles as whatever its bytes
encode: on ARM7TDMI that includes `stmia`, `ldmia`, and mnemonics the part
cannot execute at all. Measured against the assembler's listing, 18 of 89 rows
carrying the verdict had no multiple-transfer anywhere in their span. It also
fires on any large size mismatch, where it means nothing. Before writing an
owner off, check that the instruction is inside its span in the assembly.

Where it is true, it is decisive: `arm.md` gates both store-multiple peepholes
on `TARGET_ARM`, so a reference-side Thumb `stmia` or `ldmia` cannot have come
from any C source, and hand-written assembly and library objects sit in every
ROM.

Two shapes inside `wrong` are worth naming because they look like source
problems and are not. A candidate that holds a constant in a callee-saved
register and copies it into the argument register at each call, where the
reference rebuilds it, is cse unifying two identical `(const_int N)` trees into
one pseudo that reload then gives a register. And a pair of comparisons that
differ as `cmp #9`/`blt` against `cmp #8`/`ble` is `fold-const.c` rewriting `<`
against a positive constant. Neither is reachable by respelling the source.

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

In an overlay, name a call by the **veneer entry that site reaches**, not by
the import behind it, and record the import in a comment:

```c
Record_02000e20 *Func_02003f32();  /* Func_0808a080 */
```

Every site gets its own entry in the overlay's import table, so the import's
name cannot say which one, and naming it directly leaves the `bl` encoding
wrong. `overlay show <overlay> <start> <end>` prints the address the reference's
`bl` encodes, and the same range with `--annotate` names the import it reaches.

Fix the naming when you open an owner to work it, where the residual can be read
afterwards. A batch rename that moves scores and closes nothing is not progress.

### Types and structure

Prefer the simplest shape the evidence supports. Keep uncertain fields, casts,
aliases, signedness and control flow explicit until they are proved. Risk lives
in types, scoping and control flow, not in spelling: a small change to a type
or an alias can move register allocation and break an exact match. Do not
retype or re-scope a source to make it look modern.

Narrow types are usually the wrong reading. The store width comes from the
pointer cast or the struct member, not from the variable, so a `u8` local that
the reference never truncates costs an extension the ROM does not have. Widen
by default and narrow where the bytes show a truncation. A narrow *parameter* is
worse than a narrow local: it silently truncates the argument at every call
site, so `f(0x301)` through a `u8` parameter compiles to `movs r0, #1`.

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
a symbol, or the value is dead in your source and the compiler dropped it —
check for a variable that is assigned in one branch and read in another before
reaching for `Value_<addr>`.

A value that appears both as an immediate and out of a register is a variable,
not a literal. When one call takes 8 as `movs r3, #8` for one argument and as
`mov r3, r8` for another, a literal cannot produce both: the second came from a
local assigned before the loop and held across it. This is the cheapest way to
tell a hoisted invariant from a constant, and it is usually worth more than it
looks -- on resource_39a:1e08 it closed 12 bytes of size difference in one edit.

An overlay function pointer is the LINKED address, not the overlay-relative one.
The pool word for `Func_02001d78` in an overlay linked at 0x02008000 is
0x02009d79: the routine, plus the Thumb bit, at its linked address. Read the base
off a finished sibling; `exact/resource_39a_c_02002094.c` writes
`(void *)Func_0200a014` for an owner at offset 0x2014.

An increment emitted after the last call of a loop body lives in the loop TEST.
`pass++;` as a statement emits `adds r5, #1` before whatever follows it; the
reference emitting it afterwards is `while ((unsigned int)++pass <= 3)`.

Where two guards return the same thing, the reference usually reaches one shared
block placed after the body. Writing `return 1` twice puts an inline copy near
the top instead; a `goto` to one label at the end is the shape that matches.

### Comments

Brief, factual, and about things a future contributor needs: relationships
between values, invariants, hardware constraints. Canonical comments are short
Japanese UTF-8 lines matching the naming convention; English is fine where it
makes a technical constraint clearer.

```c
/* マップチップ切替。ヘッダ値に従い表示窓へ文字ブロックを割り当てる。 */
```

Record what the bytes prove, including when it made the score worse: a source
that is right and scores badly is more useful than one that is wrong and scores
well, and the next reader needs to know which they have. Do not use comments as
a speculation diary, and do not claim to have recovered an original identifier
the evidence does not support.

---
## Where the project is

Exact C stands at 274,776 bytes, 20.4% of the executable image. Permanent
assembly — linker veneers, alignment, and routines whose instructions no C can
produce — adds 216,278. Together those are DONE, 36% of the ROM's executable
bytes, and DONE is what the sun in every commit subject reports.

Publish both figures or neither. The share of the ROM is the honest headline;
the share of the bytes that *can* be C says how much of the job is left. That
second denominator is 1,130,844, and exact C is 24.3% of it.

### 100% is reachable, and the compiler is the minority obstacle

This is a measurement, not an opinion. All 1,276 overlay candidates from a
since-deleted tier were scored against the ROM. Counting only real owners — 32
bytes or more, so the 8-byte veneer stubs that score exact trivially are
excluded — 1,242 of them classify as:

| share | class | what it means |
|---:|---|---|
| 58.5% | size mismatch | a statement is missing or extra |
| 26.0% | wrong | operands differ: a type, a prototype, a constant |
| 12.9% | ordering | a scheduler tie |
| 1.9% | allocation | reload picked different registers |
| 0.6% | unemittable | a shape no stock gcc 2.96 emits |

**84.5% of what is left is ordinary reconstruction** — read the assembly again,
find the statement you missed, fix the type. About one owner in six sits in the
families the compiler decides.

The sample is drawn from sources someone already wrote and failed to close, so
it is biased toward hard cases. That makes the finding stronger, not weaker:
even among owners known to have resisted once, five in six failed for a reason
the contributor controls.

The largest single target, `main:0x080bbb0c` at 6,332 bytes, carries no
permanence marker at all — no Thumb multiple-transfer, no SWI, no veneer idiom,
and no entry in `asm/classification.json`. Every byte of it is reachable as C.

### Large owners are unproven, and that is where the bytes are

The 84.5% figure above is measured across all owner sizes and is dominated by
small ones. Broken out by size, the record is much starker:

| owner size | byte-exact | share of open bytes |
|---|---:|---:|
| under 128 B | 819 of 1,211 (68%) | 5.0% |
| 128 B - 512 B | 169 of 834 (20%) | 29.4% |
| 512 B - 2 KB | 4 of 246 (2%) | 36.6% |
| 2 KB and over | 0 of 49 (0%) | 29.1% |

No owner over 2 KB has ever been made byte-exact; the largest that has is 1,828
bytes. Owners of 512 B and over hold **65.6% of the remaining audited overlay
bytes** and account for 4 of the 992 closed. Plan around that: the method is
demonstrated at small scale and unproven at the scale where most of the work is.

That is not evidence it cannot be done -- four owners above 512 B did close, and
nothing about gcc 2.96 forbids matching a large function. It is evidence that
nobody has yet shown it here, and that a completion estimate extrapolated from
the small owners is not a forecast.

The one large owner taken to a complete reconstruction, `resource_3bf:3054`,
reached 636 of 636 calls with every argument value, branch and constant verified
against the reference, and still missed on register allocation alone. It is also
the most call-pure large owner measured -- 0.5% of its instructions touch memory,
against 1.9% for `resource_3bd:13f8` and 5.1% for `resource_38f:08ec` -- so it is
the case where a call-sequence reconstruction is most nearly the whole source,
and it is the case that got closest without closing.

Before drawing a conclusion from that, note what a call-sequence reconstruction
does NOT model: loads and stores through a held pointer. `resource_3bd:13f8`
keeps a pointer in r7 and reads one field repeatedly, and a reconstruction that
emits only calls and constants cannot express that.

Adding that modelling was tried and it moves the number the way completeness
should. Teaching the reconstruction pointer-valued loads, field reads and
writes, and the read-modify-write shape -- `*(u8 *)(p + 90) &= 0xfe`, which is a
call returning a pointer and one bit being cleared in a byte -- took
`resource_3bd:13f8` from 674 wrong instructions to 601, and its `ldrb` and `str`
counts are now exact against the reference at 10 and 2. So the residual tracks
how much of the source is expressed, not a fixed limit.

What remains after that is still two things, and they should not be conflated.
Roughly 18 memory operations are unmodelled -- 8 `ldrsh` and 10 `strh` -- which
is ordinary unfinished work. And the parking persists: 84 high-register moves
against the reference's 15, with 104 fewer `lsls` and 107 fewer `movs`, which is
the same shape as `resource_3bf:3054` at smaller magnitude.

The strongest result from that experiment is what happened to the parking as the
reconstruction got more complete. On `resource_3bd:13f8` the excess
high-register moves fell as each modelling gap closed:

| what the reconstruction expressed | excess `mov` | bytes of 6,220 |
|---|---:|---:|
| calls and constants only | +80 | 5,732 |
| plus memory through a held pointer | +69 | 5,848 |
| plus the three loops | **+48** | **5,908** |

`bl`, `ldrb` and `strb` are exact against the reference at 703, 10 and 11. So
what looks like the compiler parking constants is substantially a source that
does not yet say everything the original said -- an incomplete reconstruction
reads as an allocation difference, and closing the gap closes both.

Backward branches matter more than their count suggests. `resource_3bf:3054` has
NONE: it is 636 calls with no loop anywhere, which is why a linear
reconstruction could be structurally complete for it. `resource_3bd:13f8` has
three and `resource_38f:08ec` has two, and a linear walk emits each body once
without its counter arithmetic or its branch. Check for backward branches before
concluding anything about a large owner's residual.

The useful correction is that parking is NORMAL at this scale. The reference for
`resource_3bd:13f8` saves three high registers itself, and the candidate is one
register over it rather than four. `resource_3bf:3054`'s reference, which saves
only `{r5, lr}` across 636 calls, is the outlier -- and it is also the only large
owner with essentially no memory access at all, so it is the one where a
call-sequence reconstruction is closest to the whole source.

### Why the project stalled at 20% for two weeks

Not a compiler wall. A third tier held 862,856 bytes of C that did not
reproduce and reported it as 74% coverage against a 20% match rate, so the work
that would actually close owners never got picked up. The queue was full of
already-attempted owners and the thing missing from each was another read of
the assembly.

The tier is gone. Its sources are recoverable per file with
`git show 84ea2392a:semantic/<name>.c`, and their comments carry call
resolutions and field layouts worth reading before you reopen an owner. Their C
did not build the ROM and never would have. When an owner is the wrong size,
expect to rewrite it against the score rather than repair it.

### Finish overlays, do not spread

Work one overlay to completion, smallest remaining first, rather than picking
owners by score across the whole ROM. Within one overlay the callees, types,
structures and idioms are shared, so every owner you finish makes the next one
cheaper, and `overlay twins` and `exact_reading_list` can actually find you a
worked example. Across the ROM that compounding is lost. It also changes what a
day's work produces: "this overlay is finished" is an artifact, "the number
went up 0.04%" is not.

We have finished 65% of the overlay owners and 22% of the overlay bytes, because
we did the small ones: the median adopted owner is 46 bytes and the median owner
still parked is 204. What remains is filtered twice over, once for size and once
for having already failed. The way out is reconstructing large functions by
reading them.

### Veneers stay in the denominator

48,760 bytes of linker veneers and 3,020 of alignment are executable bytes in
the ROM, so they belong in the denominator. They can never be C, so a share
measured against that denominator can never reach 100. `resource_3cc` is
finished and reads 64%.

### The host does not reach the bytes, and that was measured

gcc 2.96 was never an FSF release: it is Red Hat's branding of a pre-3.0
snapshot, shipped in Red Hat Linux 7.0 and 7.1 and disowned upstream. Debian
stayed on 2.95 and deliberately avoided it, and the era's GBA SDK shipped
agbcc, which is 2.95. So whoever built this ROM almost certainly had Red Hat,
and PROVENANCE.txt records that our base -- gcc-mirror `04179d4a511b`,
`2.96 20000731 (experimental)` -- is the one Red Hat built gcc-2.96-54 from.

That raises a real worry, because `cse.c` hashes a `SYMBOL_REF` by the HOST
ADDRESS of its name string, and 2000-era Linux had no ASLR. If bucket placement
reached codegen, matching the bytes could require matching the machine, and
that would be a different project.

It does not. Compiling the 577-symbol owner `resource_3bf:3054` with the
environment padded by 0, 8,000 and 60,000 bytes gives one identical hash, as
does the same test on `resource_3b8:3df8`, as do five repeated runs under macOS
ASLR. CSE decides equivalence by structural comparison; the hash only orders
buckets, and a lookup that lands in a different bucket still compares the same
way. `simplify-rtx.c` was made content-hashed anyway and `cse.c` was left alone,
which turns out to be harmless rather than an oversight to chase.

Nobody needs a period-correct machine image. If a residual ever looks
host-shaped, re-run that padding test before believing it.

The COMPILER is a different question, and it is open. Red Hat's shipped 2.96 is
this base plus a patch series, and we build the bare base. `gcc-2.96-108.1.src.rpm`
from `archive.kernel.org/centos-vault/2.1/source/i386/SRPMS/` carries
`gcc-2.96-20000731.tar.bz2` -- byte-identical to our tree for `reload1.c`,
`cse.c`, `haifa-sched.c` and `config/arm/arm.c` -- and 384 patches, 379 of them
applied by the spec.

Applying that series to the base and diffing against what we build:

| file | lines changed by Red Hat |
|---|---:|
| `gcc/reload1.c` | **285** |
| `gcc/calls.c` | 64 |
| `gcc/local-alloc.c` | 61 |
| `gcc/global.c` | 30 |
| `gcc/config/arm/arm.c` | 12 |
| `gcc/haifa-sched.c` | 2, and both inside `print_value`, a dump routine |

Read that table against the pass-order finding above. The scheduler is
untouched, and reload -- which the `-da` dumps show is what actually places
these insns -- is the most heavily patched pass in the series. Nine applied
patches touch it, and `gcc-reload-hardreg-free.patch`, `gcc-cselib-mode.patch`
and `gcc-pure-reload.patch` are generic rather than target-specific.

That was the state of the question until the compiler was actually built, and
building it answered it. Development on the first game ran about a year to an
August 2001 release, which brackets the toolchain between Red Hat 7.0
(September 2000, `gcc-2.96-54`) and 7.1 (April 2001); 7.2 and RHEL 2.1 both
post-date the ROM. `gcc-2.96-54.src.rpm` is the era-correct one: same base
tarball, 70 patches, 68 applied, and against our tree it changes 54 lines of
`reload1.c`, 58 of `cse.c`, 38 of `local-alloc.c` and 24 of `calls.c`.

Built in a 32-bit i386 container -- where the tree needs NO host patches at all,
which is the whole reason to build it there -- and pointed at the owners whose
residual is pure ordering:

| owner | our compiler | Red Hat 7.0 | differing |
|---|---:|---:|---:|
| `resource_3b8:3df8` | 11 insns | 11 | **0** |
| `resource_37a:1380` | 128 insns | 128 | **0** |
| `resource_3ce:029c` | 588 insns | 588 | **0** |

Byte-identical assembly, including the 1,574-byte owner whose entire residual is
one swapped pair repeated at 196 call sites. Red Hat's reload changes do not
reach these owners.

So the compiler axis is closed the same way the host axis was: by measurement,
not by reading. Our unpatched 2000-07-31 tree is the right compiler, and if
Camelot used Red Hat's build instead it makes no difference to these bytes.

That leaves the fair question of why `alchemy-gcc` carries changes matching
neither upstream nor Red Hat. There are three kinds and only one is a gap.

**Host portability**, the bulk of it. `reload1.c`'s fourteen lines are entirely
`GEN_FCN (icode) (...)` becoming `((insn_gen_fn3) GEN_FCN (icode)) (...)` -- a
function-pointer cast so the call has a correct prototype on a 64-bit host,
where an unprototyped call through a pointer worked by accident at 32 bits.
`expr.c`, `emit-rtl.c`, `optabs.c` and the rest are the same shape. These change
how gcc is COMPILED, not what it emits, and the Red Hat comparison above is the
evidence: two differently-patched compilers, one built on arm64 and one on i386,
agreeing instruction for instruction across three owners.

**The SYMBOL_REF hash** in `simplify-rtx.c`, which its own comment calls a
deviation rather than a fix. Measured inert: see the padding test above.

**`.align N, 0`** in `config/arm/elf.h`, one line, and this is the real gap. It
is not compensating for the compiler at all -- it is compensating for the
ASSEMBLER. Red Hat 7.0 shipped binutils 2.10.0.18; we assemble with 2.47, whose
default alignment fill is not zero. Neither upstream nor Red Hat needs this line
because neither was using a 2026 assembler.

One thing the provenance does NOT claim, and it matters: the gs1cc snapshot DATE
was never measured. The 3.0.0 and agbcc trees both say "confirmed by
measurement" and name what they beat; gs1cc says only "the 2000-07-31
development snapshot", inherited because Red Hat used that base. Nobody swept
neighbouring commits.

Walking gcc's own history says how much room that leaves. For each pass, the
last change on or before 2000-07-31 and the next one after:

| pass | last change | next change |
|---|---|---|
| `reload1.c` | 2000-07-28 | 2000-08-04 |
| `reload.c` | 2000-07-28 | 2000-08-04 |
| `cse.c` | 2000-07-28 | 2000-08-04 |
| `combine.c` | 2000-07-30 | 2000-08-04 |
| `calls.c` | 2000-07-18 | 2000-08-15 |
| `expr.c` | 2000-07-12 | 2000-08-06 |
| `local-alloc.c` | 2000-06-13 | 2000-08-04 |
| `global.c` | 2000-06-13 | 2000-08-04 |
| `config/arm/thumb.c` | 2000-04-08 | not again in 2000 |
| `config/arm/thumb.md` | 2000-04-08 | not again in 2000 |

So any CVS pull between 2000-07-30 and 2000-08-03 gives the same compiler for
our purposes, and ours sits in the middle of that window. The Thumb back end is
stable for far longer -- unchanged from April 2000 through the end of the year.

That is why the date was worth checking and why a fine-grained bisect is not:
the granularity that could move bytes is WEEKS, not days. A snapshot from June,
or from September once the 08-04 reload and cse changes land, is a different
compiler; one from 2000-08-01 is not. If the snapshot is ever swept, sweep it at
that spacing.

So the toolchain is reproduced faithfully in its compiler and papered over in
its assembler. `binutils-2.10.0.18-1.src.rpm` sits in the same Red Hat 7.0
archive as the compiler, and the container recipe that built one will build the
other. Doing that and dropping the `elf.h` line would remove the last change we
carry that moves bytes.

### The tooling is frozen

The loop needs about eight commands and they all exist. Adding a tool now
requires deleting one, and measurement that explains why we are stuck is no
longer a deliverable.

### The permuter cannot reach an ordering residual, and now we know why

`alchemy_permuter` is a real port of pret's decomp-permuter, 29 randomisation
passes. On this corpus it closed nothing: 97 rows at 1,500 to 2,500 candidates
each improved 18 and matched 0.

Pointed at the best targets the project has -- owners that are byte-count exact
with zero wrong instructions, two halfwords from reproducing -- it did not
improve them once. 200,000 candidates across ten owners, eight seeds apiece on
the closest two, 89% of the mutations failing to compile at all, and the best
score never moved off the baseline.

That is not bad luck, it is the wrong instrument. The permuter searches SOURCE
SHAPE. An `ordering` residual is the post-reload scheduler choosing between two
independent instructions after the source has had its say, and when the source
is already right -- right size, right instructions, right operands -- there is
no shape left to mutate that is not worse. Every candidate it generates is
either a compile error or a step backwards.

Aim it at rows the score calls `wrong`, where a statement really is missing or a
type really is off, and read the residual first so you know which you have.
Treat a match it finds as something to explain before adopting.

### Assets: named is the bar

An asset is finished when it is a standalone file whose name says what it is —
`vale_night.png`, `rock_front.png`, `isaac_running_south_west.png`,
`djinn_venus.gif`, `growl.wav`, `good_morning.mid`, `alchemy.sf2`. A stem
carrying the ROM's own numbering is not that, whether the number is decimal
(`sfx_272`), a `resource_<hex>` index, or a short hex token standing in for one.

Everything in a standard format but still ID-named is **Extracted**, which is a
real step and not the last one. Images are 1.8% Named and music 0.0%. Those
numbers were 32.6% and 99.6% until August 2026, when the top tier was being
awarded for the *category name* — anything called audio counted as finished —
rather than for a file existing. Tiers are counted per file, so one well-named
member cannot promote its package.

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

There are 99 public commands and the loop needs about eight of them. The full
catalog below exists so the registry has somewhere to be checked against; it is
reference, not a reading list.

| | |
|---|---|
| `overlay score` | compile a candidate, diff it, classify the residual |
| `candidate-show` | the same for a main-image owner |
| `overlay adopt` / `park` | install a proven row, or take one back out |
| `overlay audit --all` | re-prove every adopted row still reproduces |
| `overlay candidate-rank` / `main-rank` | pick the next owner by what is wrong with it |
| `overlay twins` | find a finished owner with the same shape |
| `overlay reconstruct` | draft C for a call-dense owner from its own disassembly |
| `make verify` | the only result that proves anything |

Most of the rest are asset builders that run as part of `make build-assets`,
and gates that run as part of `make verify`. You should not need to invoke them
by hand, and if you find yourself doing so, that is worth a note in the commit.

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
| `overlay_inventory` | Rebuilds the diagnostic inventory the overlay tools read. |
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

The `overlay` binary also carries `score`, `park`, `audit` and `reconstruct`,
described in [the loop](#the-loop). `score` is the overlay counterpart
of `candidate-show`: same output, same `--align`, and it derives the row's span
rather than asking you for one.

### Where unfinished work lives

Your C lives in `work/`, which is gitignored. What you have LEARNED lives in
`recon/`, which is not. Those are different things and the split is the point.

A byte is exact C or it is assembly. There is still no third directory for C
that does not reproduce, and `two-tier-check` still fails if one appears.

**`work/` — the scratchpad.** Every tool that scores a source takes a path, so
`candidate-show work/080a1234.c --align` and
`score work/resource_373_c_0200034c.c --align` work exactly as they would from
anywhere else. Adoption moves the file to `exact/` when it matches. Nothing here
is committed and nothing here survives a fresh clone, which is correct: an
unproven source is not an asset.

**`recon/<owner>.json` — the record.** One per owner under active
reconstruction, holding measurements and a recipe. Never C, never ROM bytes.

```json
{ "owner": "resource_3bd:13f8", "span_bytes": 6220,
  "score":  { "candidate_bytes": 5908, "wrong_instructions": 629, "class": "wrong" },
  "shape":  { "calls": 703, "loops": 3, "memory_ops": 47 },
  "expressed":   ["call sequence", "memory through a held pointer", "three counted loops"],
  "unexpressed": [{ "op": "ldrsh", "count": 8, "where": "loop bodies" }],
  "verified_against_reference": ["bl count 703", "ldrb count 10"],
  "rejected": [{ "shape": "one base local reassigned", "wrong": 671 }] }
```

This is not the semantic tier returning. That tier stored unproven C and then
COUNTED it, which is how 862,856 bytes read as 74% coverage against a 20% match
rate. A record claims no bytes at all -- it carries the score the tools reported,
and that score says the owner does not reproduce. There is nothing in it a
coverage number could add up, and `check-unmatchable` fails a record whose owner
has since gone exact, whose `wrong_instructions` is 0, or which carries no score
at all.

What it buys is the thing `work/` cannot: a large owner taken most of the way
used to leave nothing behind but prose in a commit message, so a clone or a
fresh worktree started again from zero. `unexpressed` is the honest remaining
work in instruction counts, and `rejected` carries the number that killed each
dead end so nobody pays for it twice.

Delete the record when the owner is adopted. The gate will remind you.

### A draft is not progress, and its size is not its completeness

`overlay reconstruct` will happily fill `work/` with a thousand drafts in eighty
seconds. That is a thousand pieces of C that do not reproduce, which is what the
semantic tier was, and the only things keeping it from becoming that again are
mechanical: `work/` is gitignored, no drafter output may be written anywhere
else, and no published figure counts it. `exact` counts adopted owners and
nothing else.

The dangerous part is not the files, it is the reporting. A draft that is 92% of
the reference's SIZE sounds nearly finished. Measured against the reference
instead of guessed at:

| draft size vs reference | owners | instructions actually matching |
|---|---:|---:|
| 95-105% | 147 | **13%** |
| 85-95% | 198 | 9% |
| 60-85% | 240 | 4% |
| under 60% | 497 | 0% |

Of 348 drafts at 85% of reference size or better, six match 90% of their
instructions. The median matches ten percent. Size ratio is not completeness and
must never be quoted as it -- that is the same move that made 862,856 bytes of
semantic C read as 74% coverage, and it was made again in this repository in
August 2026 before anyone checked it.

The figures that mean something are `differing_halfwords`, which is zero only
when the owner reproduces, and `wrong_instructions`, which is zero when the
source is structurally right and only the schedule differs. Those are what a
`recon/` record stores. Quote those.

`semantic/regions.json` and `semantic/main-regions.json` survive as the audited
owner boundaries for regions discovery did not index -- evidence about where an
owner starts and ends, which is independent of anyone's C.

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
| `main_candidate_rank` | Ranks main-image candidates by residual class, as `overlay candidate-rank` does for overlays. |

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
| `make inventory` | Produces the overlay inventory that overlay tools. |
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

Price a stock option over whole translation units before routing it, not over
the owners it helps. An overlay whose own owners disagree about a flag was not
compiled with it, and a route that closes six rows by regressing seven is a loss
recorded as a gain.

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

`N` is the nearest whole `DONE` percentage — exact C plus permanent assembly —
from the staged progress report and the staged coverage map, so it matches the
figure on the README. It was the exact-C share alone until August 2026, which
meant the sun said 20% beside a README that said 36%. `make progress-subject`
prints it. The `commit-msg` hook checks it, and both read the index rather than
the working tree so an unstaged metric cannot set the percentage of a commit
that does not contain it.

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

- **Unfinished scopes:** 2,236
- **Address spaces scanned:** 97 (87 still contain targets)
- **Target bytes:** 855,986 semantic-C or unresolved-assembly bytes
- **Resolved-only bytes:** 489,380 Exact C or audited permanent assembly bytes
- **Executable bytes accounted for:** 1,347,122

### Main target list

This table contains every scope of at least 1,000 bytes (228 rows). The complete
2,236-row index, including the smallest audited owners, is
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
| 74 | 1,882 | 1,882 | `resource_3b9:0x02001c6c` |
| 75 | 1,876 | 1,876 | `main:0x0802592c` |
| 76 | 1,854 | 1,854 | `resource_3c8:0x020026f8` |
| 77 | 1,822 | 1,822 | `resource_38f:0x02001e6c` |
| 78 | 1,816 | 1,816 | `main:0x080e99c0` |
| 79 | 1,816 | 1,712 | `main:0x08090a5c` |
| 80 | 1,810 | 1,810 | `resource_383:0x02002fd4` |
| 81 | 1,808 | 1,808 | `resource_3b1:0x02005c9c` |
| 82 | 1,796 | 1,664 | `main:0x080f3078` |
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
