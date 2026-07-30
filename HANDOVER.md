# Alchemy handover

Updated: 2026-07-30

The single authoritative handover. Do not add dated handoff files; update this
one in place. Per-function detail lives in `work/claude/notes/`; per-commit
history lives in git. This file holds only what is still *actionable*.

**Read it top to bottom before starting.** Several sections correct advice given
earlier in the project, and the corrections are the parts that save time. Two
"blockers" that stopped work for whole sessions turned out to be stale notes
rather than real limits.

Exact means fully linked machine-code byte equality — not semantic similarity,
not equal object size.

---

## 1. Where to work

In descending order of measured value.

**Overlay strict queues.** Two discovery fixes took this queue from 20 rows /
6,110 bytes to **1,334 rows / 311,324 bytes**, and rediscovery of known-exact
functions from 14% to 67%. Recent lanes converted 2,056, 1,628, 880, 860 and 644
bytes out of it. Rank overlays by *strict* bytes.

Measured ranking after the second fix: resource_373 17,140 (21 rows),
resource_3b8 15,684 (15), resource_3bf 13,880 (46), resource_372 10,442 (37),
resource_38f 9,674 (22), resource_3b2 9,622 (74), resource_3c5 8,928 (32),
resource_374 8,730 (47), resource_3c8 8,696 (32), resource_3a8 8,670 (31),
resource_39f 8,306 (39), resource_383 7,864 (18). Of these, **resource_3b8,
372, 38f, 3b2, 374, 3c8 and 3a8 are effectively untouched territory.**

The two fixes were:
1. *Pool skipping* — the scan stopped after a return at the first halfword that
   was not `0x0000`/`0x46c0`, i.e. at every real literal pool.
2. *Entry-shape recognition* — a leaf opening with `bx lr` (`0x4770`),
   `movs rN,#imm` (`0x20xx-0x27xx`) or `ldr rN,[pc,#k]` (`0x48xx-0x4fxx`) was
   refused as an entry, so each such leaf broke the chain and darkened everything
   behind it. Individually worth +154, +37 and +21 rediscovered functions;
   `push {regs}` (`0xb4xx`) contributed 0 and was deliberately left out. These
   shapes are only ever a **relay** — the queue filter still demands
   `starts_with_prologue`, so a widened shape can never itself become a row.

**Raw row counts are now far noisier**: `unconverted_discoveries` grew 3,835 →
12,945 and `data_walk_discoveries` 2,768 → 10,031, because relay seeds landing on
data produce short junk walks. They are all labelled and excluded by the strict
filter, but any consumer summing raw rows will over-report badly. Filtering to an
empty `contained_by` matters more than it used to.

**Re-probing old park notes.** The return-type lever (§4) arrived late, so any
note whose residual is a two-halfword argument-setter swap is stale evidence
rather than a blocker. One sweep closed 8 functions / 1,628 bytes from 41
candidates. See §5 for how to sweep it mechanically.

**The main image** — `docs/DISCOVERY-QUEUE.md`, 727 regions, ~395 KB. Boundaries
are ROM-proven, but that was never the constraint: compiler fidelity is, exactly
as on the overlay side. Seven of ten regions a lane drafted reached a stable floor
within 4-8 probes and then moved for nothing. **Work it by leverage, never by
size** — identical sibling pairs (one draft, two regions), construct families
(one insight amortised over several), and the ten debt regions that already carry
non-baseline routing so the flag is not their unknown. The 2 KB-plus tier parks at
a higher rate than the 40-120 byte rows and costs an order of magnitude more time.

Do not read the Full-C denominator as the target: it deliberately includes linker
veneers, structural assembly and executable alignment that will never be C. The
tracked measure of real remaining work is `asm_c_debt_bytes`, printed by every
full build.

**The inventory over-reports.** Rows nest, so summing a row set re-counts the same
region many times: resource_381 inflates 17.6x, resource_379 9.1x, resource_37a
5.8x. Always filter to rows with an empty `contained_by`. A lane once burned a
whole session on resource_379 expecting ~22,000 bytes and found 2,628 — 65 of its
66 rows were nested walks inside the veneer bank. `span_bytes` is otherwise
reliable (exact on every row checked across four overlays); it over-reports only
where a walk crossed a jump table, so treat it as an upper bound and derive the
span from the pool rule.

---

## 2. Workflow

**Span rule.** Function start through its own literal pool. The pool follows the
final return after an optional 2-byte zero alignment word — include both. With no
pool, exclude a trailing `.2byte 0`.

**Boundary oracle (free).** A dry-run adopt with a two-line stub returns
`adopt=rejected ... size=N/N` when both ends land on instruction boundaries with
no straddling label — independent of your C. It also settles a call's argument
count: on one function a 4-argument spelling measured 17/60 against a 1-argument
spelling's 54/60.

**Overlays**

```sh
bun tools/overlay_show.ts <overlay> <offsetHex> -n <bytes>      # disassemble
bun work/claude/overlay_verify.ts <overlay>:<hex4> <draft> <span> [flags]
bun tools/overlay_adopt.ts <overlay>:<hex4> --span N --source <draft> --apply
```

`overlay_verify` prints `size=A/B differing_halfwords=N`; byte-exact is equal
sizes and N=0. ~0.12s per probe, so iterate freely.

**Main image**

```sh
bun tools/candidate_show.ts <path>/<8-hex-stem>.c    # candidate=N reference=M differing_halfwords=K
```

Reference is `asm/<stem>.s`, output is `src/<stem>.c` defining `Func_<stem>`.
`build_claimed` picks up `src/*.c` automatically, and **adoption deletes the
assembly region**: `cp work/claude/main/<stem>.c src/<stem>.c && rm asm/<stem>.s`,
then `bun run build:claimed`. Routing keys on the **stem only**, so a draft
anywhere gets the flags `src/` will get.

**Measuring big functions.** Over ~500 bytes, halfword counts stop meaning
anything: every `bl` displacement is target-absolute, so one positional drift
makes all later `bl` halfwords differ (an 8-of-288-groups draft reported 1,235
differing halfwords). Measure instruction-group equality, splitting at each `bl`.

**Banking.** `tools/bank_cycle.sh "<subject body>"` runs every required check,
picks the `metrics: correct executable denominator` prefix when the denominator
moves, commits and pushes. It runs the three builds concurrently and moves the
staleness check next to the commit, so concurrent walkers do not invalidate a
green run: ~15s warm versus ~190s for a naive sequential `bun run verify`.

---

## 3. Build caching

Content caches under `out/cache` (delete to force a cold rebuild). Measured on a
4-core host:

| stage | cold | warm |
| --- | --- | --- |
| build_claimed (1,376 units) | 15 s | 0 s |
| build_asm (1,807 regions) | 29 s | 0 s |
| build_assets (2,431 entries) | 37 s | 0 s |
| build_full (whole 8 MiB image) | 84 s | 1 s |
| one `overlay_verify` probe | 1.8 s | 0.12 s |

- `build_claimed` keys each object on sha256 of the source bytes plus a stamp of
  the command plan and the compiler binaries it names. Sound because no generated
  source carries a `#include`, so a unit's input closure is one file.
- `build_asm` keys each region on the assembly bytes plus the link address — same
  reasoning, no `.include`/`.incbin` anywhere.
- `build_assets` skips the whole stage against a stamp over every asset source,
  every encoder in `tools/`, the manifest and the build mode. Deliberately
  conservative: any change rebuilds everything.
- `overlay_disasm` caches already-adopted overlay C spans. Without it,
  `assembleOverlay` recompiles every prior adoption in the overlay on *every*
  verification, so the tool got slower as the project progressed.

The byte guarantee does not rest on any of this: `build_full` still re-reads every
region, compares it to the reference ROM, and compares the composed image. A stale
entry cannot produce a wrong image.

**Cache-key trap.** The command plan embeds a fresh mktemp directory, so a naive
key changes every call and never hits. Exclude anything under the work directory
from the stamp.

---

## 4. The lever playbook

Ordered roughly by how often they decide a function.

**Argument-setter order is set by the callee's declared return type.** `s32`
emits `movs r1` before `movs r0`; `void` emits r0 first. `(void)Func(...)` does
**not** work — the `CALL_EXPR`'s own type is unchanged, which is why years of
probes missed this. Where one symbol needs both orders in one function, cast at
the site: `((void (*)())Func_02001234)(0, 50)` — still a direct `bl`, no pool
word, no extra instruction. Mechanism: setters leave RTL expansion in ascending
register order and `sched2` permutes them; a value-returning call frees
`reg_last_sets[r0]`, so the pre-call `movs r0` loses its dependents and `movs r1`
wins, while a void call leaves it collecting dependents, the ranks tie, and the
tie-break falls to original order. **Not universal**: where both spellings give
identical output the call has no live r0 dependency — look elsewhere rather than
grinding. A prototype-less shared declaration blocks it entirely.

**Multi-arity callees use the `_b`/`_c` alias suffix**, not K&R declarations —
and this changes argument-setup order, not just hygiene. A shared prototype-less
declaration suppresses arg0-first ordering at *every* site that uses it.

**Constant spelling.** Two inputs decide it; using one alone will mislead you.
What the reference does is the target; the factorisation tells you what gcc does
by default and therefore whether you must intervene. A constant that is `k << n`
with `k <= 255` is buildable in two instructions, so a plain literal yields
`movs #k / lsls #n`; one with no such factorisation pools automatically.

| reference does | factorises `k<<n` | spelling |
| --- | --- | --- |
| pool-loads it | yes | `(s32)&Value_0000XXXX` — force the pool word |
| pool-loads it | no | plain literal — it pools by itself |
| builds `movs`/`lsls` | yes | plain literal — you already get it |

`&Value_` is right in exactly one cell. Applying it because a constant merely
factorises inverts the reference on the third row — a mistake made three times.
When a constant repeats across sites, plain literals also beat `&Value_`: the
symbol form is one rtx that CSE always merges.

**A masked read-modify-write must start its chain with the mask.**
`s32 v = ~12; v &= p->f09; v |= 4; p->f09 = v;` is exact, while the more natural
`s32 v = p->f09 & ~12;` gives the identical instruction stream with two registers
swapped. Splitting the RMW into two statements is necessary but not sufficient —
*which operand opens the chain* decides the register identities.

**Pad structs explicitly so fields land at their real offsets.** A naive
`u8 pad[0x48]; s32 f48; void *f50;` puts the pointer at 0x4c and silently shifts
every subsequent offset; the resulting diff looks like a scheduling problem.

**`&Value_` also applies to loop invariants, not only call arguments.** A pooled
constant used inside a loop is rematerialised per iteration from a plain literal;
declaring `extern u8 Value_fffff800; s32 d = (s32)&Value_fffff800;` in the block
*enclosing the loop* hoists it into a callee-saved register as the reference does.
Function-top placement instead costs 4 bytes.

**Size short by 2-4 bytes, and the reference re-reads one address, means
`volatile`.** The mechanical tell is the *short size*, not the shape of the diff:
we fold repeated loads the reference keeps. This closed two functions outright
from 18 and 25 halfwords. Two placement rules follow from it: a block-scoped
`volatile` pointer local declared **before** the sibling arithmetic forces two
pool-address pseudos to overlap and so take distinct hard registers (declaring it
after loses the effect, and this fixed a register-identity floor that survived all
seven modes); and a second pointer the reference keeps in a different register
needs its own named local.

**Re-check every `&Value_` in an old park note against the factorisation table
above.** Four functions in one overlay were parked at floor 2 as "no flag reaches
it" when the actual blocker was a wrong `&Value_` spelling — the scheduler was
never involved.

**Statement order matters.** Moving an independent assignment above a call has
produced exact matches where nothing else did — a permuter's single win in 65,543
candidates was one statement swap. Always try both orders of two independent
statements. Related: assign a pool-address local immediately before first use,
not at function top (33 halfwords on one function).

**Stack arguments want block-scoped locals opened at the call site**:
`{ s32 k5 = 4, k6 = 3; Func(0, 59, 15, 38, k5, k6); }` gives the reference's
`movs / movs / str / str`. Function-top locals do *not* work — the scope must open
at the call. This disproved a "compiler blocker" that had parked five large sheets.

**Big word-store init sheets are the cheapest bytes in the project.** Four
functions of 232-256 bytes each (49 word stores) came out exact on the first try
from flat `s32 *p = Data_...; p[i] = literal;`. Heavy `r8`-`fp` traffic and
`adds r3,#4` chains do not mean difficulty. Look for this shape first.

**For a constant-argument call sheet, declare one function-top `s32` local per
non-trivial constant argument** (exemplar `assets/code/resource_372_c_02002180.c`,
~100 such locals). Plain literals do not reproduce the `movs/movs/lsls` interleave.

**Read the shape off the reference:**
- The epilogue states the return type: `pop {r5} / pop {r0} / bx r0` means `void`
  (r0 dead); `pop {r1} / bx r1` means r0 is live.
- `negs rN,#K` is the mask `~(K-1)`, not `~K`.
- A `movs` chain through one scratch register is a pre-scheduling order
  fingerprint: anti-dependences serialise those materialisations, so their listing
  order *is* their ordinal order and cannot be rescheduled.
- Some `bl` targets resolve inside literal pools — those are out-of-overlay
  callees with link-patched displacements. Name them `Func_020xxxxx`; not data.

**Types and structures:**
- Give an object a `struct` with named fields rather than `u8 *` plus casts when
  a halfword store must reorder past a word load.
- A narrow bitfield (`u16 f06:9;`) beats a hand-written mask/or read-modify-write;
  int-typed mask locals give the identical instruction stream but wrong register
  identities.
- Descriptor locals must be structs, not `s32 buf[N]`, when any field is a
  halfword — otherwise gcc materialises a second frame base.
- A `union Slot { s32 w; u16 h[2]; void *p; }` blocks an alias-based load hoist
  that a same-type cast does not. Tell: `-fno-strict-aliasing` fixes it.
- `switch` on an unsigned global dispatches with `bhi`, not `bgt`.
- Unsigned induction variables wherever a back-edge is `bls`/`bhi`.
- A `u8 f:2` bitfield struct for small masked field stores.

**Arithmetic and control flow:**
- `x / 0x100000` and signed `/ 65536` rather than hand-expanded negative-bias
  shifts; in-place s16 clamp `t <<= 16; t >>= 16;`; `t = (s8)t` for sign
  extension; `(x << 2) + (x << 1)` for `x * 6`.
- `value >= 0xa001 && value <= 0xdfff` reproduces the
  `adds r5,#-0xa001 / cmp #0x3ffe / bhi` range-check idiom.
- Duplicate a call in both arms of an if/else rather than using a ternary:
  cross-jumping then emits the reference's `cmp/beq/movs/b/movs/bl` shape.
- `if (x == 0) { …; return 1; } return 0;` branches to the shared tail where
  `if (x != 0) return 0;` inlines the constant.
- Split a masked byte read-modify-write into two statements to keep the mask
  32-bit; give a destructive shift its own compound-assignment statement.
- Split a shared addend out of sibling scaled expressions: `Func(a, x*16+8,
  y*16+8)` emits `movs r3,#8` twice, while hoisting the multiplies shares one.
- A `pop {r1}/bx` tail is `return Func_xxx(...);` with an `s32` return type.
- `p[108 + i]` (offset inside the index) reproduces a `biv init = base+K` pairing
  that `p += 108` before the loop does not.
- `extern u8 Base[]` plus a function-top `s32 off = 500;` where gcc would
  otherwise fold `Base + 500` into the pool.

**Allocation:**
- **Store order moves register allocation.** Allocno priority is
  `2*refs/live_length`, and a parameter's store position sets its live length.
  Permuting independent store groups is the general fix when two equal-priority
  parameter pseudos land in the wrong registers — 24 orderings found two that
  give zero where the obvious one gave 15.
- A narrow-mask local (`s32 mask = -0xD;`) must sit *inside* the loop,
  immediately before use, or it becomes a loop-invariant allocno and buys an extra
  callee-save.
- `fp` is not usable by Thumb's hi-register `add`, so a constant allocated there
  gets rematerialised; declaring it inside the loop body makes it a loop hoist.
- Named `u32` locals for long-lived masks push them into `r8/lr/ip` (allocated
  last), leaving low registers for products.
- A seemingly dead `= 0` initialiser on a pointer that later receives a call
  result can be load-bearing (84 → 49 halfwords on one function).
- Entry-hoisted int-typed local (`g = 0x986;` before the first branch) makes gcse
  rematerialise a pool constant per block — fails if a use sits in the entry block.
- Two-address `muls` avoids a copy only as `x = a & mask; x *= scale;`.
- gcc 2.96 does not share stack slots between disjoint scopes.
- `volatile` reproduces a repeated non-CSEd load of one address — **only** where
  the reference genuinely repeats the load. Single-read siblings stay plain
  `extern`. Never a general matching device.

---

## 5. Sweep levers mechanically

`tools/return_type_sweep.sh <draft.c> <overlay:offset> <span> [flags...]` greedily
flips callee return types **in both directions** — `void`→`s32` and `s32`→`void` —
keeps improvements, and repeats to a fixpoint. Both directions matter: the
`s32`→`void` half closed two functions the other half could not touch, and for a
while only one direction was tooled, so half the search space was invisible.
At ~0.12s per probe a 60-callee function converges in seconds. It found a 404-byte
function needing a specific 5-of-10 partition of identical-looking call sites that
no hand sweep would have located. One lane closed 8 functions / 1,628 bytes from
41 candidates this way.

**The levers compose, and the sweep is worth re-running after each flag.** The
working order is `-fno-cse-two-insn-immediate` → sweep → `-fsched-low-dest-first`
→ **sweep again**: the second sweep closed the last 3 halfwords on one function,
because adding a scheduler flag changes *which* return types help. On two other
functions the return type fixed the `movs`/`movs` swaps and
`-mthumb-immediate-latency` then fixed the `movs`/`lsls` ones, neither reaching
zero alone.

**Probe both CSE modes together AND each alone — the full protocol, never a
shortcut.** Measured over 20 re-probed parks, roughly three quarters were
mis-measured. Four were *both-modes-only* wins where each mode alone is neutral or
actively worse: one went 400 → 419 and 411 with each single mode → **18** paired.
A single-mode probe there would have been honestly recorded as a regression and
wrongly inherited as a negative. But the reverse also occurs — on one function the
pool mode alone is harmful while the two-insn mode helps — so the pair is not a
substitute for testing each. A note recording only one CSE mode is unmeasured.

**Size-exactness, not the halfword count, is the signal that the CSE seam is
cleared.** On five of these functions the paired modes brought the emitted size to
exactly the reference span while the differing-halfword count was still in the
tens or hundreds. A size-exact residual is a draft or allocation problem that the
non-flag levers finish; it is not a compiler problem. Judge progress by size
first.

**Sign the range-check operand the way the reference's pool word reads.**
`(u16)(h - 0x3001)` emits the `0xffffcfff` pool word while `(u16)(h + 0xcfff)`
emits `0x0000cfff`. The difference appears *inside the literal pool*, which is
easy to misread as a span error.

**Cross-jump parameterisation.** Where the reference merges two identical blocks
that differ only in one constant — pre-loading `movs r1,#K` before branching into
a shared tail — gcc will not find that merge from two spelled-out blocks. Write it
explicitly: `lim = 194; goto common;` / `lim = 241; goto common;` then
`common: if (y > (lim << 16))`. That took one function from 236 bytes to exactly
224, and it is the size-fixing lever on that shape.

**A `u16` call result needs a `u16` local, not a `(u16)` cast of an `s32` local.**
The cast form folds the `lsls #16 / lsrs #16` zero-extend away and comes out
4 bytes short.

**Two masked byte read-modify-writes need two separate mask locals.** Reusing one
cost a function its register identities *and* sank the second `strb` past four
stores — the same family as one local holding two call results, extended to masks.

**Flat `y > A && y <= B` on two constants** is converted by gcc into the unsigned
`adds`/`cmp`/`bhi` range-check idiom. Nesting the ifs is required to keep two
separate signed compares.

**Decrementing a halfword in place pools `0xffff`.** `*(u16 *)h = *(u16 *)h - 1;`
emits a pooled `0xffff` and an `adds`, where
`{ s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }` gives the reference's
`ldrh / subs #1 / strh`. Worth 6 bytes and the whole tail alignment on one
function.

**A struct cursor is actively harmful where an index is right.** Spelling
`struct Ent *e = &Table[k];` and then `e->field` took one function from exact size
and 2 halfwords to 156/160 and 56. Index every field as `Table[k].field` unless
the reference clearly holds a cursor.

**One local holding two independent call results misallocates.** A function sat
at 21 halfwords — a pure r5/r6/r7 identity swap — solely because one `s32` held
the results of two unrelated calls. Giving the second call its own named local
took it to 0; declaration-order permutations did nothing. Same family: **do not
decrement in place when the pre-decrement value is needed again later.**
`x -= 1; f(x); g(x)` sat at 11 where `s32 m = x - 1; f(m); g(x - 1)` reached 0.
In-place mutation lengthens the allocno's live range and flips the priority tie.

**For shifted constant arguments, function-top locals beat the tie-break flag —
and block-scoped locals fail.** Hoisting two `0x…00000` arguments into
function-top `s32` locals reached 0 at baseline where `-fsched-low-dest-first`
was needed otherwise, and closed four functions without routing. Note this is the
**opposite** of the stack-argument rule, where the scope must open at the call
site: register arguments want function-top, stack arguments want block-scoped.

**A `ldr K1 / ldr K2 / subs` pair is a runtime difference of two `&Value_`
symbols.** Plain literals constant-fold, so the reference's two pool words are
only reproducible as `(s32)&Value_000008c8 - (s32)&Value_0000007e`.

**`ldrsh` off a stack descriptor needs a named `s16 *` alias local**:
`((s16 *)&v)[1]` materialises a second frame base, while
`s16 *h = (s16 *)&v; h[1]` is exact. Thumb `LDRSH` has only the register-offset
form, so `movs r3,#10 / ldrsh r2,[r6,r3]` is **not** a `volatile` tell.

**The allocno-priority lever applies to constant locals, not just parameters.**
Swapping *declaration* order is neutral; declaring `s32 o;` uninitialised and
assigning `o = 1;` **between its first two uses** changes the live length and
flips the register allocation. That closed a 10-halfword r5/r6 identity swap.

**Guard-clause shape decides where early-exit blocks land.** Flat
`if (bad) return;` guards put those blocks inline; the reference wants them at the
tail. Rewriting as *nested* ifs with the main body innermost was worth 8 bytes on
one function and 143 halfwords on another (it stopped a `movs r0,#0` being
hoisted). Related: `Base + K` folds into the pool, so a function-top base local is
what restores the reference's `movs`/`lsls`/`adds` offset rematerialisation.

**"Not drafted (scan rule)" is the largest remaining seam of stale parks.** Ten
rows in one overlay were triaged on sight as duplicated two-instruction immediates
or duplicated pool loads and never drafted — but that triage predates the two
routed modes that suppress exactly those. Six of the seven then drafted went to 0.
Any note whose reason is a park *class* rather than a measured floor should be
re-read the same way: the class may now be routable.

**A SYMBOL_REF spelling pins *where* a pool load is emitted, not only whether one
is.** §4's factorisation table correctly puts a non-factorising constant on the
"plain literal, it pools by itself" row — but gcc then hoists that `ldr` above
intervening calls. Block-scoping the local does *not* fix it; the
`extern u8 Value_0000240d; s32 t = (s32)&Value_0000240d;` spelling does. That was
the last 11 halfwords on one function.

Minor but mechanical: storing a small constant through an `s16 *` pools it as a
halfword literal (`ldrh r3,.L7`), so the `s16 *q; s32 v; v = 94; *q = v;` idiom is
required rather than stylistic.

**A sweep null was untrustworthy until 2026-07-30.** The script originally matched
only the literal spellings `void` and `s32`, so a callee declared `u32`, `u16`,
`s16` or pointer-returning was invisible to it and the sweep returned a *false
null* on exactly the function the lever would have closed. One function sat parked
at floor 2 for that reason; changing a `u32` return to `void` reaches 0 at
baseline flags. The script now matches any declared return type — but treat any
sweep null recorded in a note **before that date** as unmeasured.

**"movs/lsls interleave" is not a real park class.** A function parked at floor 2
after trying `-mthumb-immediate-latency`, `-fno-sched-depend-count`,
`-mgrouped-dma-store` and the CSE modes went straight to 0 under
`-fsched-low-dest-first`, which had simply never been tried — it post-dates those
notes. Re-probe every note whose residual is a `movs`/`lsls` interleave. This is
the same stale-evidence pattern as the return-type lever, one flag later.

**Size smaller than the reference by 4-8 bytes, with a `push {r6,r7}`/`mov r8`
prologue you do not want, means `-fno-cse-two-insn-immediate` first.** Shifted
constants such as `0x5000`/`0x3000` reused across call sites get hoisted into
callee-saved registers; on one function that flag was the gate that let the other
two levers land.

---

## 6. Park classes

**Real — recognise and skip in seconds:**
- The same **two-instruction immediate** built at two or more call sites (unless
  routed, §7).
- **DMA descriptor `stmia` groups** from struct members. `-mgrouped-dma-store`
  does *not* group struct-member stores. Correcting `LAWS.md`: with a `volatile`
  descriptor plus the flag, every group *but the last* forms — the peephole does
  fire on volatile MEMs, and what defeats the final group is that its three values
  never land in three consecutive ascending registers.
- **`pop {pc}` epilogues** — that region was built without `-mthumb-interwork`.
- In-line **`0x0200_8xxx` jump tables** (per-overlay runtime base differs from our
  link base), **`mov ip,pc / bx rN` IWRAM calls**, **hand-assembly blobs** (a `bl`
  landing mid-instruction), **shared epilogue fragments**, **veneer thunks**, and
  **mid-function literal-pool dumps** at barriers we cannot reproduce.
- **Register-identity-only swaps**, often where our allocation is strictly better
  or one instruction shorter.
- **Per-site contradictory argument orders** where a prototype-less shared
  declaration is required — the per-site cast is the only escape.

**Disproved — do not trust these in old notes:**
- "Stack-argument allocation blocker" — a source shape (§4), and it had parked
  five large sheets.
- "Not producible from C / needs linker work" on resource_379 — refuted with the
  boundary oracle; the region is adoptable today.
- Two-halfword argument-setter swaps described as "no flag reaches it" — the
  return-type lever post-dates them (§4, §5).
- `-mgrouped-dma-store` by analogy: two regions resembling a routed twelve-store
  block were made *worse* by it (21→24, 25→26). Measure before routing.

---

## 7. Compiler modes

Seven admitted modes, all default-off and routed per source in
`tools/alchemy_gcc.ts`. Mechanism, prototype diffs and collateral lists are in
`docs/compiler-evidence/`.

| flag | what it suppresses | collateral |
| --- | --- | --- |
| `-fno-cse-two-insn-immediate` | sharing a repeated two-instruction immediate | 109/1,335 |
| `-fno-cse-pool-immediate` | sharing a repeated literal-pool constant | 110/2,202 |
| `-fsched-low-dest-first` | ordinal tie-break on r0-r3 argument setters, **and a pool-load hoist** | 139 |
| `-fsched-high-dest-first` | the same on r4-r12, ties with no call in them | 125 |
| `-fno-sched-alias` | a store/load pair proved independent and reordered | 82 |
| `-fsched-store-first` | a store sinking behind arithmetic | 308 |
| `-fno-sched-depend-count` | a store/load swap `-fsched-store-first` does not reach | — |
| `-fno-gcse-insert-load` | a PRE-inserted load the reference lacks | 9 |

**`-fsched-low-dest-first` reaches three residuals, not just the one in the table**:
the r0-r3 `movs` ordinal tie-break; a **pool-load hoist**, an `ldr r2` scheduled
ahead of the `movs r0`/`movs r1` of an argument group; and a
**`movs rN,#K / negs rN,rN` versus `movs r1,#imm` order swap** that appeared
identically on two functions where three source spellings each reached nothing. Neither the `&Value_` spelling nor function-top or
block-scoped locals touch that, so a six-function family would otherwise have been
triaged as a SYMBOL_REF-placement park. Try the flag before believing that park.

**Cautions.** Never combine `-fsched-low-dest-first` with
`-mthumb-immediate-latency` — the latter subsumes and then breaks the same
targets. Route per *function*, never per overlay: in resource_37b
`-fno-cse-pool-immediate` is free on 21 functions while `-fsched-low-dest-first`
regresses three large sheets.

**Adding a mode.** Commit the fork change to `alchemy-gcc`, rebuild
(`./build.sh gcc296 && ./stage.sh gcc296`), re-pin the `cc1` digest, and prove the
source-only build reproduces gs1-en.gba at SHA-1
`5c4695205413df7db52b9a184815a07783999971` *before* the re-pin is admissible
(`PROVENANCE.md`). Then add the routed set **and** a matching explorer mode in
`tools/mode_sweep.ts` — a coverage contract fails the test chain otherwise. Prefer
extending an existing predicate with a disjoint participation set over adding a
competing rule; both scheduler modes and both CSE modes are built that way.

**Routing sets are easy to edit into the wrong one.** Two functions that verified
exact were rejected by the adopter because a scripted edit put their paths in the
neighbouring set — the surrounding lines matched in both. The symptom mimics a
tooling bug: `overlay_verify` takes flags from the command line and says 0, while
`overlay_adopt` takes them from routing and disagrees. When those two disagree,
`grep -n "<stem>" tools/alchemy_gcc.ts` against the set boundaries first.

---

## 8. Open problems

**Compiler lane, well specified:**
- **Immediate-build transposition** — but try the composition first. The
  reference schedules an independent `movs r0,#K` *between* a two-instruction
  immediate's `movs` and its `lsls`. **`-fsched-low-dest-first` applied after
  `-fno-cse-two-insn-immediate` closes many of these**: three functions sitting at
  8-17 halfwords with exactly this residual went to 0. Only what survives that
  pairing is a genuine compiler problem, and it is an `insn_cost`/`tmp_class`
  question at `haifa-sched.c:4068-4090`, not a dependent-count one.
- **`0808fecc`** (main image, floor 2): the last-scheduled-insn class rule fires
  before the ordinal tie-break, so a separate mode is needed.
- **resource_391:02a8** (floor 7/164): the reference reuses the register holding
  an `ldrsh` offset for a table cursor; not a sched1 decision.
- **Pool-word emission**: `resource_3af:0bb8` has one surplus pool word (60 vs 56
  bytes) — a dedup/emission issue, not CSE.

**Known-unreachable, do not re-attack:**
- `resource_373:2cb0` — its reference both reloads pool words *and* keeps one in a
  register to derive a related value by an add. Related-value reuse needs exactly
  the recording `-fno-cse-pool-immediate` suppresses, so no whole-function gate can
  match it.
- The 608-byte main-image sibling pair `08098954`/`0809a294` is **solved** by
  `-fsched-high-dest-first` and adopted; the note describing it as unreachable is
  obsolete.

**Discovery: 308 of 934 known-exact functions are still not rediscovered.** The
predecessor census that found the entry-shape gate is the way to find the next
one: of the previously-missed set, 374 had no decoded instruction within 512
bytes — whole dark runs — while the rest were chain breaks. Re-run that census
against the current 308 before inventing a heuristic.

Two successor rules were built, measured and **rejected**; do not rebuild them
(the reasoning is recorded above `nextEntryAfterReturn`):
- *Tail-call chaining* (`b` to a target outside the body) seeds **0** across 96
  overlays. `walk_thumb` follows unconditional branches, so a tail call's callee
  is already absorbed into the caller and the caller's last instruction is the
  callee's return — the existing return chain covers it.
- *Chaining after an unresolved indirect* (`bx rN`) seeds 3,574 with **zero**
  precision: none of them landed on a known entry. What follows an unresolved
  indirect is the dispatch data it reads.

`tools/remaining_survey.ts` filters `retention === "c_candidate"` and so is blind
to 96 debt rows (38,456 bytes); widening it to all five debt retentions is one
line.

**Tooling.** ~~`resource_39c:10c0` verifies at 0 halfwords but `overlay_adopt`
rejects it.~~ **RESOLVED 2026-07-30: it was the §7 routing-set trap**, exactly as
that section warned. `assets/code/resource_39c_c_020010c0.c` is in no routing set,
so the adopt rehearsal (which copies the draft to that path and routes flags by
path) compiled at baseline while `overlay_verify` took
`-fno-cse-two-insn-immediate` from argv. Not a splice bug; its pool is a normal
post-body pool. Adding the path to `NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES`
closes it (`notes/resource_39c-10c0.md`). **General rule: whenever verify and
adopt disagree, the difference is flags, and the first check is whether the
*installed* asset path is routed.**

**Span audit (2026-07-30): mis-spanning is NOT systemic.** The "an advertised row
may be a fragment of a larger owner" hypothesis was tested against six stubborn
parks (`39c:10c0`, `373:2a54`, `379:0074`, `3b2:0030`, `381:29a4`, `3af:3a0c`) —
**all six spans are correct**, and none of the parks is a span problem. Across all
1,337 strict rows, 1,334 end at exactly one return: the walker follows
unconditional branches, so a direct continuation is absorbed into the row and the
fragment failure mode is structurally rare here. **The bulk detector is
`returns == 0`** — the only three such rows are `resource_3bd:0024` (a veneer
misread), `resource_3ca:0f80` (body complete, trailing pool cut 12 bytes short;
true span 340 not 328) and `resource_399:15b4` (**a genuine fragment**: the
advertised end 0x1690 lands inside a mid-function pool that a `b` jumps over, and
the epilogue is at 0x16a4; true span **248**, not 220). A secondary check —
whether a row's end lands on a non-prologue row — flags 74 rows but is almost all
veneer banks, so it is not worth tooling. Adding a `returns == 0` warning to the
inventory writer would cover the real signal in one line.

---

## 9. Required checks

`tools/bank_cycle.sh` does all of this. Manually:

```sh
bun tools/full_c_progress.ts --write-inventory
bun tools/full_c_progress.ts --write-report
bun run verify
```

Commit subjects must end in the suffix from
`bun tools/full_c_progress.ts --subject`, and a subject that changes the
executable denominator must begin `metrics: correct executable denominator`.

`PROVENANCE.md` is authoritative on clean-room rules: semantics only from the
target's own disassembly and this repo. **No `asm()`, no inline assembly, no
register pinning, no barriers, and no `volatile` as a matching device.**

**Concurrency — do not inherit the old two-lane rule.** That limit was measured
when a verification probe cost 1.8 s and a bank cycle 190 s, so compute really was
the binding constraint. The content caches (§3) cut those to 0.12 s and 15 s, and
the limit was never re-derived. Measured afterwards on the same 4-core host with
two lanes running: **load average 1.18, and a probe costs 137 ms under load versus
120 ms idle** — roughly 70% of the machine idle. Lanes are reasoning-bound, not
compute-bound, so run **five or six** and re-measure `/proc/loadavg` before
assuming a ceiling. If probe latency starts climbing well above ~150 ms, that is
the real signal to stop adding lanes.

The binding constraints are instead: walkers must never share an overlay (the
overlay `.s` file is the only mutable artifact), and only the main agent may run
git, the build scripts, `full_c_progress.ts` or `overlay_inventory.ts`. The bank
cycle tolerates lanes adopting mid-build, so there is no need to pause them.

**Agent economics.** Permuting is an audit pass, not an engine: one exact hit in
65,543 candidates, though it cost ten minutes and cracked a function a careful
lane had declared unreachable. Run `tools/permute_overlay.ts` over a newly parked
near-miss before believing the park; do not expect it to close a backlog. Cheap
models are not useful for drafting here — the ≤32-byte "easy" population is 90%
veneer thunks, word-table interiors and mid-function fragments, and the genuine
small leaves are already taken as each walk passes them.
