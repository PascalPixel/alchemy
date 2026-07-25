# Decompilation playbook

This is the operating guide for converting Alchemy's remaining reconstructed
assembly into byte-exact C. It optimizes verified conversions per wall-clock
hour. The evidence, clean-room, build, and publication rules in
[AGENTS.md](AGENTS.md), [STATUS.md](STATUS.md), and
[AGENTS.md](AGENTS.md#publication-boundary) take precedence.

## Evidence boundary

All Golden Sun-specific facts must be derived independently from the approved
ROM set and this repository's clean-room outputs. Never inspect or accept code,
symbols, labels, pseudocode, assembly patterns, scripts, outputs, or notes from
another Golden Sun checkout.

External advice may contribute game-agnostic process ideas only. Treat every
suggested compiler behavior, source shape, function boundary, name, type, or
game-specific claim as an untrusted hypothesis until it is independently
supported by approved evidence. Do not put an unverified hypothesis into source
or documentation.

Generic public tools and public architecture or compiler documentation remain
allowed, exactly as AGENTS.md rule 2 states. The boundary excludes game
knowledge from other Golden Sun work, not generic methodology: clean-room
caution is not a reason to delete legitimate generic tooling, and tooling is
not a cover for importing game facts.

Never send ROM bytes, ROM-derived assembly, private analysis, toolchains, or
build products to a network service. All matching work stays local.

## Definition of done

An ordinary assembly region is decompiled only when asm-free C emits the exact
linked bytes and relocations at its fixed address. Inline assembly, register
pinning, and compiler barriers do not count as C conversion.

The immediate English byte-closure milestone requires a source-only,
byte-identical `gs1-en.gba` build with no copied-ROM fallback. Full
decompilation additionally requires that no ordinary `asm/` C-debt remains;
only positively justified compiler-unproducible stubs may remain there. Project
completion is the larger twelve-ROM and repository-quality milestone defined in
`STATUS.md`.

## Operating rules

1. **Work the queue, not the address order.** Rank by exact reuse, structural
   family leverage, candidate closeness, type confidence, complexity, and
   expected conversions per unit time.
2. **Run batch-first.** Batch means whole-queue waves and cohort levers, not a
   fixed lane preference. Clear `reuse` and `family` members whenever they
   exist, but when the regenerated lane totals show the queue dominated by
   `research` singletons, the batch lever is context, draft normalization, and
   diagnosis-cohort work that lifts many singletons at once. A hard singleton
   must not block bulk delivery.
3. **Timebox hand work by kind.** Shape tuning against an existing draft stops
   after **10 minutes or three meaningful source variants**, whichever comes
   first. Semantics-first drafting, for functions the draft generator rejects
   (`M2C_ERROR`, `M2C_BITFIELD`, `M2C_MEMSET`, `M2C_MEMCPY`) or structurally
   mangles, is a separate activity with its own budget: up to **45 minutes**
   to produce a correct natural draft, which then enters the automated lanes
   instead of being tuned toward byte identity by hand. Either way, record the
   best result, diagnosis, and next hypothesis, then rotate.
4. **Prefer deterministic searches.** Search declarations, signedness,
   parameter types, expression order, temporary lifetime, common-subexpression
   boundaries, control-flow shape, and supported compiler modes before using an
   expensive permutation search.
5. **Solve families through representatives.** Normalize remaining assembly,
   group related regions, solve one representative, then independently compile
   and verify the source shape against every member. When structural families
   are exhausted, group by diagnosis signature instead (register-rotation,
   epilogue-shape, scheduler-hoist, entry-register classes) and design one
   lever per cohort; a cohort sharing one gate falls like a family.
6. **Turn discoveries into leverage.** A hand-won match should leave a
   generalized operator, template, type constraint, boundary rule, or concise
   independently evidenced note when the lesson is reusable.
7. **Measure results, not activity.** Optimize exact C regions and executable
   bytes per wall-clock hour, not commits or compiler invocations.
8. **Commit verified batches about every 10 minutes when matches exist.** Do
   not manufacture a commit when there is no verified conversion.

## Authoritative measurements

Regenerate reports rather than copying a stale snapshot into this document.

```sh
bun tools/build_full.ts --source-only
bun tools/decomp_queue.ts --refresh --jobs 16 --limit 400 \
  --targets-out out/decomp/targets.txt
bun tools/decomp_families.ts --top 10
bun tools/overlay_inventory.ts --top 10
```

`out/decomp/candidates/<stem>.c` is a **derived** file, not a store. Every
`decomp_queue.ts` run rewrites it from `out/permute1/state/<stem>.json`'s
`best.body`, so a candidate improved by hand and merely copied into that
directory is silently reverted by the next refresh — with no error, and the
queue then reports the *old* byte count as if the work had never happened. Two
improved near-misses were lost this way before the mechanism was noticed. To
keep a hand-written body, promote it into the search state:

```sh
bun tools/promote_candidate.ts /path/to/<stem>.c        # or --stem=08xxxxxx
```

It scores the candidate with the same weighted halfword metric `permute_v1.ts`
uses, refuses a regression unless given `--force`, and writes both the state and
the derived candidate. Use `--force` only for a body you have proven correct
whose score is worse for a known structural reason — a missing compiler-emitted
frame inflates the edit distance while the C is right (see the `080bd850` law).

Report at least:

- converted C region count and exact-C executable bytes;
- remaining ordinary assembly regions and bytes;
- retained structural assembly regions and bytes;
- candidates attempted and exact matches by lane;
- dominant-mismatch-class mix of the diagnosed failures (context improvements
  must shrink the semantic share; normalization must shrink the register-only
  share);
- compiler invocations and cache-hit rate when available;
- median wall-clock time per successful conversion;
- full-build identity and fallback-byte count.

If a lane produces no match within its declared budget, stop it and record the
failure mode. Do not silently continue a low-yield loop.

Plan against the C-debt regions and bytes in the families report, never
against the byte-reconstruction percentage: that percentage includes assets
and retained structural assembly and does not forecast decompilation time.

## Work lanes

### Reuse

Use when an existing candidate or verified C template is already close. Try
type and declaration constraints first, followed by a small deterministic
variant set. Verify the whole region, not only its instruction mnemonic shape.

### Family

Use when multiple regions share an exact or normalized structural fingerprint.
Choose the simplest representative. Keep family parameters explicit: constants,
callees, data symbols, offsets, widths, and signedness. A family is complete
only after each member independently passes exact compilation and linking.

### Module and boundary

Use when a mismatch suggests shared literal pools, alignment ownership,
neighboring functions compiled as one translation unit, embedded data, or a
multi-region function. Establish region ownership before source-shape search.
Use `tools/decomp_module.ts` for justified adjacent-region experiments. Never
decompile through an unresolved data island.

### Research

Use for large, unique, or unresolved-control-flow functions after higher-yield
lanes have work in flight. Recover semantics and types first. Keep experiments
bounded by the same 10-minute/three-variant rule unless the work is explicitly
a reusable tool or cohort investigation with a measurable acceptance test.

## Standard ordinary-region wave

Run this sequence after a material improvement to types, context, source
normalization, variant operators, or compiler-mode evidence.

```sh
# 1. Verify ownership and regenerate the ranked debt reports.
bun tools/build_full.ts --source-only
bun tools/decomp_families.ts --top 10

# 2. Refresh independently derived type and context evidence.
bun tools/track_bases.ts
bun tools/mine_structs.ts
bun tools/m2c_context.ts

# 3. Draft the whole queue. Drafting is cheap; bound the search stages,
#    not the drafting, or the large regions that hold most of the remaining
#    bytes never receive a draft at all.
bun tools/decompile_batch.ts --context work/m2c_context.c \
  --max-bytes 8192 --jobs 16 -o work/m2c-ctx roms/gs1-en.gba

# 4. Perform the cheap exact sweep.
bun tools/match_m2c.ts --jobs 16 roms/gs1-en.gba work/m2c-ctx

# 5. Diagnose and rerank failures.
bun tools/decomp_queue.ts --refresh --jobs 16 --limit 400 \
  --targets-out out/decomp/targets.txt

# 6. Search bounded source variants and evidenced compiler modes.
bun tools/search_queue_variants.ts --limit 100 --variants 128 --jobs 16
bun tools/search_compiler_modes.ts --limit 100 --jobs 16

# 7. Use permutation only on a bounded, diagnosed target set.
bun tools/permute_v1.ts --targets out/decomp/targets.txt \
  --steps 5000 --restarts 8 --jobs 16
```

The numeric limits are starting budgets, not promises. Reduce them when a lane
is unproductive; increase them only when recent results justify the cost.

`search_queue_variants.ts` also reads the optional ignored
`work/decomp-seeds.json` manifest. Its `seeds` object maps an eight-digit entry
stem to one path or an array of paths containing semantics-reviewed hand
candidates. The queue draft remains a seed too. This is the feedback bridge
from hand diagnosis into deterministic search; do not list a candidate merely
because it has a low byte-mismatch score. Use `--no-seeds` for a queue-only
control run or `--seeds FILE` for another manifest.

Prefer permutation targets that are the smallest independently verifiable
whole function or block preserving the mismatch. Small inputs improve compile
throughput and shrink the mutation space, but do not repeatedly permute a
register-only or scheduler-only plateau: reducing such a function can remove
the live ranges that produce the reference allocation. After a bounded run,
route those plateaus to compiler trace diagnosis.

**Overnight annealing is a recorded failure mode, not a default.** Repeated
unattended `decomp_overnight.ts` campaigns have spent 8–9 hours and produced
zero exact matches (Pascal, 2026-07-23): the remaining queue is dominated by
scheduler/allocation plateaus that statement-level mutation cannot reach at
any step count — a 40,000-step × 8-restart run floors exactly where a 3,000-step
run does. Short, bounded permuter attempts (minutes) followed by diagnosis of
the floor — mismatch class, pass-dump evidence, one recorded hypothesis — beat
unattended hours every time so far. Do not start an unattended overnight
campaign unless a specific recent result (new context, new operator, new
compiler mode) justifies re-sweeping the queue, and say which result that is.

For genuinely unattended hours, `bun tools/decomp_overnight.ts` remains
available (resumable, checkpointed waves). It references optional stages that
do not exist yet (`cfg_extract.ts`, `synthesize_expr.ts`, `synthesize_block.ts`,
`mine_blocks.ts`): build them or delete the references. A campaign script must
not imply capacity that is not there.

## Overlay wave

Overlay code has independent load addresses and boundary concerns. Do not feed
it blindly through the ordinary fixed-image pipeline.

Overlay conversions also sit outside the `[N of M]` commit counter, which
counts main-ROM C-target regions (`tools/check_commit_progress.ts`). The
settled choice is to track overlay progress **separately** rather than widen
the denominator: `M` is derived from the main-ROM region table, whereas the
overlay denominator would have to come from control-flow discovery, which the
paragraph below explains is not a trustworthy denominator. Folding an unstable
count into `M` would corrupt the one metric that is currently exact.

The practical consequence: an overlay-only commit carries the `[N of M]`
suffix **unchanged**, because the checker requires the suffix on every commit
and only forbids `N` from regressing. A flat counter on such a commit means
"no main-ROM region closed", not "no progress". State the overlay numbers in
the commit body instead, quoting `tools/overlay_inventory.ts`'s
`converted_functions` and `converted_instruction_bytes`, so the overlay lane
has its own visible, monotonic record.

### Shape cohorts first, singletons last

Measured over the 2026-07-25 overlay session, which recovered 154 functions:

| Method | Functions | Notes |
|---|---:|---|
| Shape cohort: one template, expanded | 131 | ~9 templates derived by hand |
| Hand-drafted singletons | 19 | one analysis each |
| Agent drafting lane | 4 | 12 targets, ~2M tokens |
| Permuter and variant sweeps | 0 | thousands of candidates, no exact overlay match |

Derive one template and expand it. A cohort member costs a compile; a singleton
costs an analysis. Run the sweeps only to *rank* near-misses, never expecting a
match from them.

```sh
# 1. Find the cohorts. Groups unclaimed, uninventoried functions by masked
#    opcode shape -- these are the ones discovery never seeded, where the
#    remaining cohorts live.
bun tools/overlay_shapes.ts --top 10

# 2. Fix boundaries first. Discovery seeds partway into functions it reaches
#    only through an interior pointer, and a fragment can never match a
#    template, so this precedes both drafting and template matching.
bun tools/overlay_entry.ts --all

# 3. Read one member: disassembly plus the pool words its pc-relative loads
#    resolve to, which is what a semantics-first draft needs.
bun tools/overlay_show.ts resource_39f 0ee0

# 4. Hand-derive its C using the overlay laws in LAWS.md (pool words are
#    symbols; initialisers follow source order; a byte threshold is tested on
#    the shifted value; loop counters are unsigned). Verify it alone first.

# 5. Expand it. Each member is renamed to its own entry symbol, byte-verified,
#    and adopted only on an exact match; constants that differ simply miss.
bun tools/overlay_cohort.ts --cohort 0 --template work/<name>.c
bun tools/overlay_cohort.ts --cohort 0 --template work/<name>.c --apply
```

When a cohort member misses by a handful of bytes, read its diff before
touching the template: the per-instance constants usually differ, and the fix
belongs in a per-member generator rather than in the shared shape.

```sh
# Inventory executable entries, boundaries, and structural families.
bun tools/overlay_inventory.ts --top 10

# Generate and verify bounded direct candidates.
bun tools/overlay_match.ts --limit 100 --max-bytes 512 \
  --jobs 16 --variants 16

# Compare overlay instruction shapes with the verified exact-C corpus.
bun tools/compiler_template_index.ts --jobs 16 --top 20
bun tools/overlay_template_match.ts

# Adopt one exact overlay reconstruction (dry run, then install).
bun tools/overlay_adopt.ts resource_3c7:0030 --source out/decomp/.../02000030.c
bun tools/overlay_adopt.ts resource_3c7:0030 --source ... --apply
```

`overlay_adopt.ts` is the only supported way to install an overlay conversion.
It locates the region with the assembler's own listing, swaps the instructions
for the `AlchemyC_<address>: / .space` placeholder the asset builder reads,
installs the C as the overlay's `_c_<address>.c` sibling, and refuses to write
anything unless the rebuilt overlay is byte-identical to the overlay as it
stands. Editing the placeholder by hand invites a silent width error, because
Thumb encodings are 2 or 4 bytes and the `.space` size must be exact.

The inventory's `unconverted_discoveries` count is deliberately not a
function-debt denominator: conservative control-flow discovery retains
veneers, shared helper-table slots, overlapping literal entries, and other
structural leads. `ordinary_unconverted_functions` is the high-confidence
bounded search queue, but it can still contain shared-tail or internal-label
entries and is not exhaustive of functions with unresolved control flow or
jump tables. Report converted functions separately until every remaining
discovery has an explicit C/structural/data classification; never add either
automatic queue count to the converted count and call the result function
coverage.

An instruction-shape template hit is a lead, not a conversion. Relocated bytes,
literal ownership, call targets, data symbols, and the entire claimed extent
must still match exactly.

## Choosing the next batch

Size order is a poor queue. `tools/rank_siblings.ts` orders the remaining
`compiler_output` debt by how close its opcode skeleton is to an already
byte-verified `src/*.c`, which is a much better predictor of a cheap match:

```sh
bun tools/rank_siblings.ts roms/gs1-en.gba --limit 40
```

Each region and each installed source is disassembled and normalized to opcode
tokens with registers collapsed to `R` and immediates to `K`, then matched by
Levenshtein distance. The report names, per candidate, its nearest installed
sibling and the token edit distance. Hand a matching agent that sibling
explicitly: its declaration order, temp count, loop form and struct-versus-cast
style usually transfer with only offsets, widths, callee names and constants
changing.

Two cautions. A low distance means the *opcode* skeleton matches; the
semantics frequently belong to an unrelated subsystem, which is expected and
harmless. And a sibling being byte-exact for its own region does not make its
shape canonical — see the induction-variable law in `LAWS.md`, where copying an
installed sibling's hand-written countdown loop stalled at 10 byte mismatches
that no declaration permutation and no compiler mode could close.

## Diagnosis and routing

Use `tools/decomp_queue.ts` and the compiler diff to choose the next action.

Read the diff before editing a draft. `tools/promote_candidate.ts` reports one
score, which tells you a change was worse but never which statement caused it;
drafting against that alone oscillates. `tools/candidate_show.ts <candidate.c>`
compiles the candidate and prints it beside the reference, aligned by offset
with every differing halfword marked, which is what turns a score into an edit:

```sh
bun tools/candidate_show.ts out/decomp/candidates/08004760.c
```

It reads pool words as resolved targets, so it also shows when the reference
loads one pool word twice rather than keeping a value live -- the tell that two
uses are separate constants in the source rather than one shared variable.

| Symptom | First response |
|---|---|
| Semantic or type mismatch | Recover parameter, return, pointee, width, and signedness constraints; regenerate the draft. |
| Correct operations, wrong registers | Minimize live temporaries; search declaration and expression-lifetime variants. |
| Instruction reordering | Search statement order and supported compiler modes with module-level evidence. |
| Branch or loop mismatch | Search a bounded library of equivalent loop, early-return, condition-polarity, and short-circuit shapes. |
| Literal or address mismatch | Audit boundary and pool ownership; route to the module lane. |
| Missing or extra instruction | Check expression decomposition, CSE boundaries, promotions, dead stores, and function extent. |
| Matching prefix with size mismatch | Recheck boundary, alignment tail, literals, and embedded data before tuning C. |

Compiler flags describe an original translation unit; they are not arbitrary
per-function matching knobs. Accept a flag only with independently reproduced
module-level or ABI evidence, and document that evidence beside its build-rule
implementation.

### Routing tells

Some gaps are not source-shape gaps at all: the region's translation unit was
built in a mode the region's stem is not yet routed to. These read directly off
the reference disassembly, and each is ABI- or module-level evidence in its own
right, so check them before spending variant budget.

| Reference fingerprint | Likely routing |
|---|---|
| Pushes a callee-save low register in a function with **no calls at all** | The unit was not built with `-fcall-used-r4`: `DEFAULT_ABI_SOURCES`, or `AGBCC_SOURCES` if the address also falls in an agbcc range. |
| `stmia rN!, {…}` writing consecutive words, usually followed by `subs rN, #12` | `GROUPED_DMA_STORE_SOURCES`. Applies to plain struct-block initialization, not only writes to `0x040000d4`. |
| Redundant register copies plus strict source-statement order, with no scheduling | `OPTIMIZE_O1_SOURCES` or `UNSCHEDULED_SOURCES`. |
| A constant materialized fresh where the compiler instead reuses a live register holding a wider value with the same low bytes | `NO_GCSE_SOURCES`. |
| Address inside an already-allowlisted neighbour's range | Same unit as the neighbour; inherit its mode. Adjacency is the cheapest evidence available — check it first. |

An `AGBCC_SOURCES` candidate must be written **without any preprocessor
directive**. That path runs `old_agbcc` — a bare cc1, not a driver — directly on
the `.c` file, so `#define`, `#include` and the usual `M2C_FIELD`/`NULL`
preamble are never expanded. The failure is loud but misleading: the compile
dies with `syntax error before 'u32'` on the first line that uses a macro, which
reads like a typing problem in the candidate. Every installed agbcc source
contains zero `#` lines; write field accesses out as casts, and keep the
`typedef`s, which are ordinary C and do work.

Two more triage tells are not about compiler mode but are just as cheap, and
both cost a whole batch agent's budget when missed:

- `instruction_mismatches` 0 with `actual_size` two bytes under
  `expected_size` is a region boundary, not a source shape — see the
  two-byte-shortfall law in [LAWS.md](LAWS.md). Split the padding out rather
  than spending variants.
- `tools/integrate_matches.ts` rejecting a candidate for "carries an m2c
  helper" is a text heuristic, not a verdict on the match. Try the named-local
  expansion; if it does not hold, install by hand.

A third tell says **stop**, not "try a different mode". Routing a stem is free;
widening a compiler transform is not, and no source shape substitutes for it:

- Twelve consecutive word stores lowered as three `stmia rN!, {r1,r2,r3,r4}`,
  where the block is **on the stack**, or the base **stays live** past the last
  store, or the function **consumes a call's return value**. The
  `thumb_group_four_word_records` transform cannot fire in any of those cases —
  see the over-fitted-transform law in [LAWS.md](LAWS.md). Report it as blocked
  on the toolchain and stop; four batch-8 agents each spent a full budget
  proving this independently.

A batch that ends with unexplained near-misses is worth re-triaging against
this whole section before the next batch is planned: of batch 7's near-misses,
every one turned out to be a routing or boundary tell rather than a
source-shape gap, and of batch 8's, five of seven were the grouped-DMA family
in one form or another.

Verify a routing claim before acting on it — an agent reporting "matches with
`-mgrouped-dma-store`" is reporting a measurement it could not install, and the
measurements are not always right. Of batch 8's two such claims, `080284dc`
held exactly (0 mismatches at 64 bytes, installed) and `080037d4` inverted
under test (26 → 32 with the flag). One `verifyCandidate` call with
`extraCompilerFlags` settles it without touching the registry.

### Sweep every mode before parking a near-miss

`verifyCandidate(source, rom, outputDirectory, extraCompilerFlags)` in
[tools/match_m2c.ts](tools/match_m2c.ts) takes flags directly, so the whole
mode registry can be swept for one candidate **without editing
`tools/alchemy_gcc.ts`** — which also makes it safe to run while a batch
workflow is compiling. Every entry in the registry is reachable as a flag list;
build the table from `cflagsForSource` and try all of them.

This is cheap and it is not hypothetical. Sweeping the four candidates parked
out of batch 7 as unexplained near-misses turned two of them into exact
installs on the spot:

| Stem | Parked at | Swept to | Mode it actually needed |
|---|---:|---:|---|
| `0800bc48` | 22 | **0** (40B) | `GROUPED_DMA_STORE_SOURCES` |
| `08002f10` | 17 | **0** (44B) | grouped-DMA **plus** `UNSCHEDULED_SOURCES` |
| `080c0eb8` | 35 | 35 | none — genuinely source-shape |
| `080fb2a4` | 30 | 30 | none — genuinely source-shape |

Two lessons. A near-miss can need **two** modes at once, so stopping at the
first flag that helps understates the routing. And a sweep that moves nothing
is itself a result: it converts "unexplained near-miss" into "confirmed
source-shape gap", which is what the next agent needs to know.

One trap makes a sweep lie in the other direction. `verifyCandidate` compares
against the **linked symbol** size, while `tools/decomp_diagnose.ts` compares
against the **region** size in `out/full/asm/manifest.json`. Where a region
carries more than its function — a trailing `bx lr` veneer, a shared tail, a
second entry point — a sweep reports a confident `EXACT` for something that
would still fail integration. Sweeping batch 7 and 8 produced four `EXACT`
hits and two were this: `08093054` (region 102, symbol 100, the residue being
a real `0x4770` veneer rather than padding) and `080c0228` (region 124, symbol
112). Both are region-boundary work, not decompilation. A sweep is a shortlist,
never a verdict — run `tools/decomp_diagnose.ts` on every hit before it goes
anywhere near `integrate_matches.ts`.

A sweep that lands on four mismatches is usually one line of source away, not
one flag away. Four mismatches in this Thumb encoding is two 16-bit
instructions swapped, and the swap almost always mirrors the order of two
adjacent assignments in the C. `080b2720` sat at 4 under `-fno-gcse` through
seventeen modes, four unused switches, and eight flag combinations; the
reference initialised the loop's destination pointer before its source pointer
and the candidate did the reverse. Exchanging those two statements — nothing
else — made it exact at 68 bytes. Read the transposed pair, find the statements
that produced them, and try that order before spending a sweep.

When the residual is that the reference recomputes a constant *inside* a loop
while the candidate hoists it above the loop, the lever is the loop keyword,
not a flag. `for (;;)`/`while (1)` give the RTL loop optimizer a preheader
block and it sinks loop-invariant constants into it; a backward `goto` to a
label does not produce one, so the constant stays in the body. `08029504`
compares three call results against `-1`, which Thumb cannot encode as an
immediate and so must live in a register. Written as `for (;;)` with `continue`
the two-instruction `movs`/`negs` pair for `-1` sat in a preheader ahead of the
first call; written as `goto retry` it sits after the call where the reference
has it, and the region went from 12 mismatches to exact at 80 bytes. Explicit
self-tail-recursion (`return Func_08029504();`) is byte-identical to the `goto`
form here, because GCC 2.96 eliminates it into the same jump before the loop
pass runs — prefer the `goto`, which does not misstate the return value.

A constant that appears as a bare literal in a narrow lvalue can cost a literal
pool word. This Thumb backend has no HImode immediate-move alternative, so
`*(u16 *)p = 136;` lowers to a pool load rather than `movs r3, #136`. Assigning
the constant to an `s32` local first and storing that local keeps the
materialisation in SImode, where the immediate alternative exists. On
`080287a8` this one change removed the pool word and, with it, all ten
mismatching bytes — two for the move itself, two for the shifted pc-relative
offset of an earlier pool load, and six from the pool contents sliding by four.
Any near-miss whose diff shows an `ldr rN, [pc, ...]` opposite a `movs` is this
tell, and the byte count it produces is much larger than the defect.

The same SImode-versus-narrow-mode split explains the opposite tell, a `subs`
where the candidate emits a `movs`. `08096c80` reaches the mask `0xF3` as
`subs r3, #17` from the `4` already live in `r3`. Written as any literal —
`0xF3`, `-0xD`, `~0xC` — combine narrows the AND to QImode and materialises the
mask directly. Written as `v - 17` against a named `s32 v = 4`, CSE keeps the
subtraction in SImode and emits the reference's instruction; the low byte is
the same either way, so the two forms are interchangeable semantically and not
at all in bytes. When the reference derives a constant from another live one,
name the earlier constant and write the arithmetic — do not fold it yourself.

A narrow load feeding a test has three spellings and they are not
interchangeable either. `*(u16 *)p & mask` expands as `zero_extend:SI (mem:HI)`,
which CSE will equate with any other `zero_extend` of the same address — on
`08079cbc` that merged the condition's `ldrh` with the call argument's and shrank
the region by two bytes. A `(u16)` cast keeps both loads but leaves the AND as
`(and:SI (subreg:SI (reg:HI ...)) ...)`, and the SImode-destination against
HImode-source mismatch stops reload tying the destination to the load, so it ties
to the other operand instead and the two registers come out exchanged. A named
`u16 v = *(u16 *)p;` is the form that does both: the load stays a plain HImode
set that CSE will not equate with a `zero_extend`, so both `ldrh`s survive, while
the integer promotion in `v & mask` emits its own `zero_extend` and leaves the
AND between two plain SImode pseudos, which reload ties to the load. Combine
folds the extension back into the `ldrh`, so the temporary costs nothing. When a
size change and a register swap look like alternatives you must choose between,
this temporary is usually the third option that avoids both.

When two quantities come out exchanged, stop guessing at source shapes and read
the allocator's own numbers. Compiling the candidate with `-dl` writes a `.lreg`
dump whose header lists `Register N used R times across L insns` for every
pseudo, and `-dg` writes a `.greg` dump whose `;; N regs to allocate:` line is
those pseudos in the order the allocator will serve them. `R` is the `n_refs`
that goes into `floor_log2(R) * R * size / D`, with loop-depth weighting already
applied. `size` is in words (`PSEUDO_REGNO_SIZE`), which on this target is 1 for
every scalar mode, so a byte quantity and a pointer quantity compete on equal
terms and the factor can be dropped; reading it as bytes is what turned
`08006dec` into a false park. `D` depends on which pass decides the quantity, and the `.greg` dump
tells you which: `;; N regs to allocate:` lists exactly the pseudos local-alloc
did *not* assign, already in the global pass's priority order. A pseudo on that
line is the global pass's (`allocno_compare`), and `D` is the header's `L`,
`reg_live_length` — the printed order is the answer, no arithmetic needed. A
pseudo absent from that line was taken by the local pass (`QTY_CMP_PRI`), and `D`
is `death - birth` in local-alloc's doubled per-block index space, i.e. twice the
insn-index span *inside the one basic block*. Read that span off the `.lreg` RTL;
substituting `L` there predicts the outcome backwards. The
allocator hands `r3` out first under `REG_ALLOC_ORDER`, so for two pseudos that
die into the same two-operand insn, the one born *last* wins `r3` and the result
shares the loser's register. Then ask which of the four inputs is not pinned. Reference counts almost always are — they are the emitted
instructions. Births and deaths often are not: on `08077348` the competing ratios
were `0.583` for a strength-reduced pointer and `0.556` for the loop bound, and
moving a single `total = 0;` statement above the call that defines the bound
shortened its live range from 18 insns to 17, took it to `0.588`, and closed the
region. The emitted code was identical either way because GCC sinks the zero-init
back below the call, so the edit is free. Any statement the compiler will hoist
or sink back into place is a free knob on a live length; look for one before
parking an exchange as compiler-internal.

Batch agents are forbidden from editing `tools/alchemy_gcc.ts`, so a region
needing one of these modes can only ever come back from a batch as an
unexplained near-miss. Triage every batch near-miss for these tells before
concluding the gap is source-shape, and re-probe the allowlists from the
integrating session rather than from the batch.

## Law ledger

Confirmed compiler behavior is a compounding asset; sessions must not relearn
it. Maintain a tracked `LAWS.md` with one entry per law: the assembly
fingerprint, the producing C idiom, one in-repo example (address and source
file), the evidence source, and the confirmation date. A law enters the ledger
only with in-repo provenance (an installed match, a tracked document, or an
evidenced build rule). Externally suggested compiler lore goes in a separate
hypothesis section until reproduced locally. Operating rule 6 has no force
without this durable home, and every hand-won match must end with a ledger
append or an operator/tool improvement.

Seed entries with in-repo provenance today:

- **Minimal live-variable form** reproduces the original allocation; each
  extra named temporary rotates the allocator. Confirmed by installed
  counter-family matches (`src/08079338.c`, `src/08079358.c`). Because the
  draft generator names every intermediate, temporary elimination to fixpoint
  is a normalization pass, not a search dimension.
- **Dual-use pre-read**: a fresh-destination shift pair appears when the
  shifted value has a second read before the shift; single-use forms fold in
  place. Confirmed during the same counter-family investigation.
- **Cast-literal table access** (`((s32 *)ADDR)[i]`) reproduces add-then-load
  addressing that array declarations do not. Structure confirmed at the still
  unconverted `080fb670`; treat the register floor there as a separate open
  gate.
- **Prologue `lr` rule**: the approved compiler saves `lr` in any Thumb
  function containing a branch, even a leaf ([CONVENTIONS.md](CONVENTIONS.md)). An
  internal branch without an `lr` save is reclassification evidence, not
  C debt.
- **Register-reservation flags are module declarations**, accepted only with
  bundle-level evidence and a comment, as implemented for the fixed-register
  handler bundle in `tools/alchemy_gcc.ts`.

## Hand-session protocol

Before starting, write down the candidate, diagnosis, expected experiment, and
stop condition. Prefer a natural, semantics-first draft with few live locals to
incrementally contorting generated C.

During the session:

1. Verify the function extent and instruction mode.
2. Recover observable behavior, calls, memory widths, signedness, and likely
   parameter/return constraints.
3. Compile a baseline and preserve its diff score.
4. Try at most three source changes that test distinct hypotheses.
5. Stop immediately on an exact match, or park at the time/variant limit.

A meaningful variant tests a different causal hypothesis. Cosmetic rewrites or
repeated random mutations count against the budget when they consume attention.

The parking record belongs under an ignored analysis directory and should
contain no raw ROM excerpt. Record:

- target and claimed extent;
- best candidate path and score;
- dominant mismatch class;
- variants tried and what each disproved;
- suspected lane or family;
- cheapest next experiment.

## Integration and commit gate

Integrate only exact matches. `tools/integrate_matches.ts` performs guarded
replacement from a match directory; review its proposed changes before using
`--apply`.

```sh
bun tools/integrate_matches.ts <matches-dir>
bun tools/integrate_matches.ts --apply <matches-dir>
bun tools/build_full.ts --source-only
bun tools/build_full.ts
git diff --check
git add <intended-source-and-manifest-files>
bun tools/check_publication.ts --staged
git diff --cached --name-status
```

Before committing, confirm:

- exact bytes and relocations pass at the linked address;
- no inline assembly, register pinning, or barrier was introduced;
- only canonical publishable inputs are staged;
- manifests and classifications agree with source ownership;
- the full source-only build is byte-identical with zero fallback bytes;
- the commit subject ends with the current `[N of M]` counter.

Commit a coherent verified batch as soon as the approximately ten-minute commit
interval arrives. If a long search produces no matches, report the attempt and
rotate; do not weaken the verification gate to preserve cadence.

## Tool-investment priorities

Prefer improvements that reduce work across many candidates:

1. authoritative debt and family classification;
2. type/signature constraint propagation;
3. generated-draft normalization and temporary-lifetime reduction;
4. permuter operator coverage: audit `permute_v1.ts` against the generic
   operator families public permuters document (temporary introduction and
   elimination, statement reordering, type flips, control-shape rewrites) and
   port what is missing, benchmarked on a fixed target set by matches per
   CPU-hour;
5. nearest matched-pair retrieval: given a target's normalized fingerprint,
   surface the closest installed matches and their C beside every dossier and
   hand session;
6. exact-C corpus templates with safe symbol parameterization;
7. literal-pool, alignment, and function-boundary ownership;
8. mismatch-block-focused deterministic mutations;
9. compiler-result caching and batch verification (a persistent compile server
   only if profiling shows process spawn dominating a search loop);
10. automatic, atomic integration and generated progress reporting.

Every tool spike needs a measurable acceptance test, such as more exact matches,
lower median mismatch, fewer compiler calls per success, or a resolved boundary
cohort. Park a tool idea that does not move its metric within the declared
budget.

## Endgame ordering

When the queue thins, sequence the last mile deliberately: split the mixed
code/data regions first (boundary debt precedes matching); merge proven
multi-region functions before attempting their C; clear module windows around
shared-pool walls; document every accepted flag set with its evidence; confirm
every surviving `asm/` file carries a positive structural justification in
`asm/classification.json`, where absence of a C match is never a
justification; then regenerate STATUS.md from the tools and re-verify the
full build.

## End-of-cycle checklist

- Regenerate the queue and family reports.
- Run cheap exact sweeps before expensive searches.
- Verify every candidate independently.
- Integrate and fully build each successful batch.
- Inspect and publication-check the staged paths.
- Commit verified progress at the requested cadence.
- Record throughput and parked failure modes.
- Start the next cycle in the highest-yield non-blocked lane.
