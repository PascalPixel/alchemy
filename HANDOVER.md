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

## The two lighthouses

The project runs as two parallel efforts, named after the Golden Sun lighthouses.
Know which one you are before you change anything.

| | **Mercury Lighthouse** | **Venus Lighthouse** |
| --- | --- | --- |
| branch | `mercury` | `venus` |
| goal | **exact C** — fully linked machine-code byte equality | **semantic C** — readable, correct, not byte-bound |
| this file | authoritative | background; the levers do not bind you |
| direction of flow | exports byte-exact sources to Venus | pulls Mercury's exact C in to override its own semantic C where one exists |

Mercury is the slower, stricter run: a function is done only when the linked
bytes match. Venus is the faster, wider run: it covers ground semantically and
adopts Mercury's exact sources whenever Mercury produces one for a region Venus
has already covered. **Exact always wins over semantic** — that is the whole
reason the flow is one-directional.

Practical consequence for Mercury: when Venus reports one of its regions is close
to exact, that region is worth re-probing here, because an exact result would
replace Venus's semantic version outright. Two such candidates were noted and are
still open (§8).

Alongside the exact lane, reviewed semantic C currently accounts for **369,358
executable bytes across 631 compiling sources**: 356,566 main-image bytes and
12,792 overlay bytes. Combined with exact C, **563,748 / 1,339,558 executable
bytes** are expressed as C. Build that lane
with `bun run build:semantic`; its sources live under `semantic/` and do not
claim byte equality. Use `semantic/ordinary-blockers.json` to keep proven ABI
and multi-region traps out of the ordinary review queue.

---

## 0. Venus Lighthouse speed policy

The 2026-07-30 speed run established a repeatable method, not a one-family
outlier. Consecutive three-owner cohorts converted tens of thousands of reviewed
bytes while preserving the exact lane and full verification.

1. **Use fresh agents for whole-owner rewrites.** Give each agent one complete
   owner, its measured byte span, call count, warned registers/thunks, and the
   requirement to account for every assembly call. Treat m2c as a hint only.
   Fresh agents completed large 57–74-call owners where long-lived agents began
   returning analysis without implementation. This runtime has three subagent
   slots, so a six-agent experiment runs as two immediately consecutive waves
   of three.
2. **Batch by established ABI or construct family first.** Owners that visibly
   publish callbacks through `Func_080cef64` or `Func_080ed408` share the proven
   six-argument renderer ABI. Fixed transfer, fill, scale, square-root, and
   owner-initializer callbacks are likewise reusable evidence. The semantic
   queue discounts only visibly established renderer families; unknown thunks
   retain the full penalty.
3. **Rewrite from assembly when m2c loses dataflow.** Missing stack-carried
   renderer dimensions, fake thunk arguments, unset call-clobbered registers,
   and conflated high-register lifetimes repeatedly proved to be decompiler
   defects rather than target blockers. Recover them from predecessor blocks,
   literal maps, and callback targets. Do not hand-clean a lossy draft for a
   whole session.
4. **A manifest row is not always a function.** Follow live stack and
   high-register state through direct continuation branches. Register the
   complete executable owner in `semantic/main-regions.json`, excluding literal
   pools and data gaps. `08026080` is the current witness: its advertised
   2,138-byte head is one 3,442-byte owner across three executable ranges.
   Pre-size `split_first` and `merge_with_continuations` candidates transitively
   before assigning them: `080e47b8` was ranked as a 768-byte dispatcher, but
   its live 184-byte frame crosses 16 manifest rows to the sole epilogue at
   `080e660a`. Its full span is 7,762 bytes including embedded pools/alignment,
   with 231 static calls—over 10 times the advertised size. Queue-row bytes
   therefore cannot be used to budget or compare these owners. Pool-map the
   mixed `split_first` rows before admitting their executable subranges.
5. **Verify and bank coherent cohorts.** Run `bun run build:semantic` while
   agents work, then one full `bun run verify` for the settled cohort. Update the
   authoritative metrics above, commit by semantic byte gain, and push before
   starting the next wave.

Parking rule: park only a specific, evidenced ABI or structural blocker. “m2c
is ugly,” “the owner is large,” and “the first agent ran out of implementation
time” are reassignment signals, not blockers.

**Measured six-agent trial.** Two consecutive three-agent waves admitted five
complete owners for **6,432 executable bytes**. Wave 1 admitted 3/3 owners and
4,408 bytes. Wave 2 admitted `080a112c` (964 bytes) and the complete split owner
`080d0ee0` (1,060 bytes); its third assignment became the `080e47b8` scope audit
above rather than a dishonest head-only conversion. Thus the method delivered
5/6 admissions while the sixth task found and explained a queue-wide sizing
defect. Fix transitive sizing/pool mapping before assigning another continuation
owner; ordinary single-row owners can continue immediately.

The next bounded-owner cohort then admitted **3/3 owners and 9,044 bytes**:
`08023178` (3,320), `08023e70` (2,756), and renderer-family `080ca60c`
(2,968). Their audits accounted for 270 assembly `BL` sites, including four
internal control edges represented as C flow rather than fake callees. This
confirms that large call counts are not themselves a reason to park a bounded
owner. `tools/semantic_queue.ts` now keeps transitive-unsized rows visible but
adds a scope-audit penalty so they cannot masquerade as the cheapest work.

A second large bounded cohort admitted **3/3 renderer-family owners and 8,960
bytes**: `080d1714` (3,384), `080d6970` (3,308), and `080d91dc` (2,268).
Together they account for 272 assembly `BL` sites. Exact dataflow review
resolved their apparent unset inputs, stack-carried publisher dimensions,
runtime handles, and internal frame-loop edges. The renderer family therefore
remains a proven high-yield lane; drain bounded members before returning to
unknown-thunk candidates.

The following unrelated-system cohort admitted **3/3 bounded owners and 6,008
bytes**: `0800aa0c` (1,640), `080acab8` (1,980), and `080f7460` (2,388).
Their 168 assembly `BL` sites include runtime allocator, transfer, renderer, and
uploader callbacks recovered from live register and workspace dataflow. This
confirms that the whole-owner method remains productive after the established
renderer family is exhausted.

The largest remaining bounded-owner cohort admitted **3/3 owners and 7,244
bytes**: `0808c4f8` (2,428), `080a2680` (3,128), and `080b63c8` (1,688).
Their assembly contains 433 `BL` sites. The audits resolved false unset values,
high-register lifetimes, stack-carried outputs, zero-fill transfers, internal
dispatcher edges, and shared mutually-exclusive tails. High call count is now
measured as review cost, not an admission blocker; the three-agent whole-owner
method remains effective on the largest bounded remainder.

The final pre-Venus cohort admitted **3/3 bounded owners and 3,672 bytes**:
`080030f8` (1,076), `080b0aac` (1,272), and `080bf678` (1,324). Its audits
resolved five apparent unset values, reset/SVC thunk semantics, nine
high-register lifetimes, and 193 assembly `BL` sites. One downstream caveat
remains independently blocked: `080c1798` intentionally observes incoming
`r2` residue and is not made ordinary by the `080030f8` conversion.

**Remote-work intake.** Periodically fetch
`origin/mercury` after banking a clean semantic
cohort. Review its delta against the current `venus` ancestry, integrate only
verified nonduplicate work, run the same full verification, then update this
handover. Never merge or pull that branch into a dirty cohort, and never let a
remote metric snapshot overwrite newer authoritative counts.

Latest intake: remote tip `7f7b99b3` was merged after the 6,008-byte semantic
cohort. It added **17,816 exact-C bytes**, taking exact Full-C Byte Share to
**194,390 / 1,339,558 (14.51%)**. The combined tree passed `bun run verify`.

---

## 1. Where to work

In descending order of measured value.

**Overlay strict queues.** Two discovery fixes originally took this queue from
20 rows / 6,110 bytes to 1,334 rows / 311,324 bytes, and rediscovery of
known-exact functions from 14% to 67%. Subsequent exact-C waves have materially
drained that snapshot; **do not reuse its old "untouched" labels.**

Freshest inventory (taken on the Venus side after an exact-C intake): **1,081
strict rows / 205,918 bytes**. Leaders by non-contained strict span were
resource_3c8 8,878 (31 rows), resource_383 8,052 (16), resource_39f 7,638 (38),
resource_3c5 7,374 (16), resource_39a 7,096 (64), resource_3c4 7,024 (58),
resource_3b4 6,242 (65), resource_3b2 5,902 (23), resource_3b8 5,468 (5),
resource_3c6 5,250 (12), resource_3ae 5,212 (21), resource_3bf 5,132 (34).
Mercury has since worked 3c8, 39f, 38f, 372 and 3c5, so regenerate with
`bun tools/overlay_inventory.ts` rather than trusting any ranking in this file —
and never carry a ranking forward across a branch intake.

**Rank by small-row count, not by total strict bytes.** `resource_3b8` is the
witness: 5,468 bytes in only **5 rows**, the smallest 348 bytes and the largest
7,468. A lane spent a full session there and adopted nothing. Overlays with many
small rows convert; overlays with a few huge rows park. Count rows under ~400
bytes and rank on that.

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

**The main-image frontier is drafting, and here is the number (2026-07-30).**
616 `c_candidate` regions remain. Every one of them already has *some* draft, but
only **303 have a draft that both compiles and passes the `PROVENANCE.md`
screen**. The other **344 regions — 289,574 bytes — need C written**, because
their only drafts are raw m2c output that fails to compile on m2c's own artifacts
(`subroutine_arg0` undeclared and similar). That is why the exhaustive flag
matrices came back at 1-in-964: there is very little left to sweep, and a lot left
to draft. Budget accordingly.

**"Undrafted" is not "unconvertible", and neither is it a target list.** 104
regions have no draft anywhere, 40,924 bytes, and **not one is a `c_candidate`** —
they are all retained structural classes. `080a8904` is the trap: 14 bytes that
read as a trivially draftable countdown loop, classified `deliberate_busy_wait`.
Always filter by `retention == "c_candidate"` out of the asm manifest; ranking by
size alone will hand you retained regions.

Seed every fresh draft with `bun tools/reference_shape.ts --stem <stem>`, which
reads the return type off the region's own epilogue (§4's rule) instead of leaving
the drafter to discover it through a byte diff.

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

**The alias rule extends to arity zero, and that case is invisible to the return-
type sweep.** A repeated `bl` to one address where the second site sets *no*
arguments is a void-no-arg alias, not a re-passed constant — the reference is
reusing a live `r0` left by the preceding compare. On `resource_39f:0cd0`,
spelling it `Func_02003a98(0)` cost 2 bytes and 62 halfwords, and neither naming
the result nor passing it back recovered anything; `extern void
Func_02003a98_b(void);` took the function from 156/154 to exactly 0. Tell: the
draft is 2 bytes long and one call site has an argument the reference never sets.

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

**The mask-first RMW rule covers OR too — and does *not* generalise past stores.**
`{ s32 w = 1; w |= *p; *p = w; }` gives the reference's `ldrb r2 / movs r3,#1 /
orrs`, while `*p |= 1;` swaps the two registers; that was the last 2 halfwords on
`resource_3c8:1150`, previously filed as "no source lever found". The limit: where
the masked value feeds an *add* rather than being stored back, inline is correct
and splitting is worse (`resource_39f:02a8`, 12 → 19). The lever is about the
store, not the mask.

**Arm order decides branch sense, and it is worth ~15 halfwords.**
`if (x <= K) {A} else {B}` emits `cmp / bgt .else / A`, laying A out first; the
natural `if (x > K) {B} else {A}` lays B first and inverts the condition. Match
the reference's *layout* order, not the reading order you would choose.

**Pad structs explicitly so fields land at their real offsets.** A naive
`u8 pad[0x48]; s32 f48; void *f50;` puts the pointer at 0x4c and silently shifts
every subsequent offset; the resulting diff looks like a scheduling problem.

**Function-top locals for word constants can subsume routed flags — try them
before routing a flag.** The shifted-constant rule above is the narrow case; the
general lever is to hoist *every* non-trivial word call argument into its own
function-top `s32` local, including constants that pool rather than factorise.
Measured on `resource_38f:2608`: 44 halfwords → 15 under
`-fno-cse-two-insn-immediate` → 9 adding `-fsched-low-dest-first` → 2 after a
struct retype → **0 at baseline with all flags removed**, once `0x620000`,
`0x690000` and the pool-only `0x010d0000` each became a function-top local. Two
rules attach: block-scoping the same constants at the call site stayed at 2 (the
function-top/block-scope split is real and load-bearing), and a constant used at
several sites wants **one local per site** — a single shared local re-creates the
`push {r7}` hoist. This is the cheapest thing to try on a function you are about
to route, and it leaves the tree flag-free.

  *But it inverts on call sheets with repeated shifted constants.* A second lane
  measured the opposite sign on `resource_39f:1818` (132 halfwords → 104 worse)
  and `:2004` (148 → 116 worse): there, plain literals composed with
  `-fno-cse-two-insn-immediate` is right and hoisting is actively harmful. The
  two results agree on the mechanism — hoisting wins when each site gets its own
  local, and loses when one constant is shared across many call sites, which is
  precisely the call-sheet shape. **Probe both directions; do not assume the
  sign.** It costs two probes at 0.12 s each.

**Give a sub-object reached through a pointer field its own struct type.** With
`u8 *q = p->f50`, gcc cannot prove `q[0x26]` independent of `p->f23` and keeps
program order, where the reference hoists the `ldrb` above the `strb`. Retyping
`q` as `struct Sub *` closed 7 of 9 halfwords in a single edit. Same family as the
`union Slot` alias lever, but the fix *adds* type distinction rather than blocking
a hoist. Tell: a store/load pair in program order that the reference has swapped,
with both objects reached through a `u8 *` field.

**`&Value_` also applies to loop invariants, not only call arguments.** A pooled
constant used inside a loop is rematerialised per iteration from a plain literal;
declaring `extern u8 Value_fffff800; s32 d = (s32)&Value_fffff800;` in the block
*enclosing the loop* hoists it into a callee-saved register as the reference does.
Function-top placement instead costs 4 bytes.

**Narrowing decides literal-pool *placement*, not just load width.** A pool-split
residual is a source-shape problem, not a CSE problem — neither CSE mode moves a
pool. On `resource_3b8:0108`, `s16 t = (u16field + K) & ~0x3fff;` emits the
reference's `ldr` **and** its mid-function pool with a `b` jumping over it, while
hoisting the same arithmetic through an `s32` local emits an identical `ldr` but
floats the whole pool to the function end, losing 8 bytes and the duplicate pool
word. Attack pool placement by re-narrowing the expression.

**`(s16)x == K` is spelled `((s32)x << 16) == (K << 16)`.** A direct comparison
on an `s16` local compiles to a plain `cmp` and comes out 4 bytes short of the
reference's `lsls #16 / movs #0x80 / lsls #23 / cmp`.

**To pin a constant into a callee-saved register, reuse a live local rather than
declaring a fresh one.** Assigning the constant to the local whose value the
preceding compare just consumed went 117 → 32 halfwords; an equivalent fresh
`s32 m = 0x4000;` is rematerialised at the call site. This is the mirror image of
the "one local holding two call results misallocates" rule — for *call results*
sharing a local hurts, for *constants* sharing is what matches.

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

**Run `tools/finish_draft.sh <draft.c>` before reasoning about any residual.** It
composes the entire enumerable search in one command and takes a few seconds at
16-way concurrency: ~50 single flag settings (both CSE modes alone and paired per
this section's protocol, the scheduler modes, the fork modes from `FORK_MODES`),
then **137 pairs and descriptor-family triples over a curated interacting-mode
pool**, then **the other approved compiler families** (`old-agbcc`, `gcc2951`,
`pret-early-thumb`, each with and without `-O1`), then the return-type sweep at
baseline and at the winning flag set, then the statement-order sweep. It prints
the best result, the exact options to reproduce it, and the surviving residual.

Two of its stages exist because single-flag probing provably misses things.
Pairing is required because the winning combination is often built from modes that
are individually neutral or worse (`08091174` reaches 3 on
`-mgrouped-dma-store,-fno-cse-pool-immediate` with neither in its top six alone).
Family probing is required because 2 of the 20 conversions banked on 2026-07-30
needed `--family old-agbcc`, which no `--flags` value can express: on `08006878`
the whole gcc 2.96 lane floors at 72 while `old-agbcc -O1` is **0**. A family win
is reported as `BEST IS A FAMILY`, and adopting it means routing the stem through
`AGBCC_SOURCES` rather than a flag set. It writes only to its `--out`
directory and never builds, commits or touches `src/`.

Validated on known cases: it recovers `08090824`'s `-mgrouped-dma-store` win
(9 → 6) and reproduces `080c1fa8`'s floor of 2 with the correct residual. It also
propagates the statement-order probe-count warning, so a null that never exercised
the lever says so instead of reading as a park. The point is division of labour: by
the time you look at a function by hand, everything enumerable is already done, so
the only question left is which §4/§5 lever the residual implies.

**The statement-order lever is tooled too, and its probe count is part of its
result.** `tools/statement_order_sweep_main.ts <draft.c> [--flags …]` permutes
maximal runs of *independent* top-level statements to a fixpoint — the §4 rule
that a parameter's store position sets its live length and therefore its allocno
priority. It was swept over 53 size-exact main-image targets for 10,982 probes:
no zeros, five drafts improved (`080c1fa8` to 5 halfwords, `080a6a98` to 11).
The binding limit is the independence test, not the search: two statements that
both touch memory or call anything are held in order, because source alone cannot
prove they do not alias, and on pointer-heavy drafts that leaves nothing to
permute (`08077394` produced 1 probe, `080ae99c` 3). Where a run *is* independent
the sweep is exhaustive — `08011fd8` took all 721 orderings and proved a real
floor. **So read the probe count before recording a park:** 1-3 probes means the
lever was never actually exercised, not that it failed.

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

**When two live names hold one call result, a copy *round trip* decides which one
a compare reads — and it needs `-fno-rerun-cse-after-loop` to survive.** A
quotient copied `ratio = value;` leaves both names tracing to the call's return
register, so the following compare reads r0 where the reference reads the copy in
r1. Writing the round trip `ratio = value; value = ratio;` makes `value` the copy
instead and the compare takes `ratio`. The second assignment is dead, and it is
load-bearing — the same family as §4's "seemingly dead `= 0` initialiser". Both
halves are required: the round trip alone leaves the halfword, because the rerun
of CSE after loop optimisation collapses the pair and restores the return
register, and the flag alone has nothing to preserve. This closed `080044d0`
(212 bytes) from a floor of 1 that eleven other source spellings and the whole
§7 mode matrix could not move. **Tell:** a single differing halfword on a
size-exact draft where our operand register is the callee's return register and
the reference's is a copy that both sides already emit. Do not read that as §6's
"register-identity-only swap" park class without trying this first.

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
- ~~The same **two-instruction immediate** built at two or more call sites.~~
  **This class is now disproved — see below.** It is listed here only because
  hundreds of old notes still cite it.
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

**Residual fingerprints that are *not* what they look like:**
- **Pool halfwords transposed within each word** (`ours=0000 ref=034b` alternating
  with `ours=034b ref=0000`) is a **mis-read constant**, not a span or emission
  problem: the draft had `0x34b0000` where the pool word is `0x0000034b`. Read
  constants off `overlay_show`'s pool listing, never off the `lsls` context.
- **A diff inside the literal pool** can be the *sign* of a range-check operand:
  `(u16)(h - 0x3001)` pools `0xffffcfff`, `(u16)(h + 0xcfff)` pools `0x0000cfff`.
  Easily misread as a span error.

**Disproved — do not trust these in old notes:**
- **"Repeated same-bb two-instruction immediate; cse1/cse2 merge it; needs an
  unrouted const-remat mode."** The paired CSE modes *are* that mode, and the
  notes predate them. The tell is that the pair only works together — either
  alone is worse than baseline. Measured: `resource_372:0f38` 84 halfwords
  baseline → 172 with `-fno-cse-two-insn-immediate` alone (and 4 bytes long) →
  152 with `-fno-cse-pool-immediate` alone (4 bytes short) → **54 paired**, and
  the function then closed. `:1154` 140 → 199 / 170 → **36 paired**. Every note
  reading "PARKED ON SIGHT (same-bb repeated 2-insn const)" is stale evidence and
  should be re-probed with both modes on. This is the single largest known pool
  of recoverable parks — treat it the way the return-type lever's backlog was
  treated in §5.
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
| `-fthumb-group-value2-in-place` | the copy forced by `thumb_store_multiple3`'s hard-coded `(reg:SI 2)` when a grouped descriptor's third word is a constant | 1 routed |

**A finished fork mode can sit unrouted and therefore unsweepable — check the
binary, not just this table.** `-fthumb-group-value2-in-place` was implemented in
`alchemy-gcc` during the grouped-descriptor work, is documented in the fork's own
`flags.h`, and exists in the **pinned** `cc1` — but it was in no routing set here
*and* absent from `FORK_MODES` in `tools/mode_sweep.ts`, so neither the router nor
the explorer could ever name it. It was found by extracting option strings out of
the `cc1` binary and diffing them against `tools/`. Sweeping it over 300 clean
drafts moved exactly one function, and moved it a long way: `080b5ad4` went from
28 halfwords at the wrong size to **0 at 64 bytes**, once its tail was also
spelled as a returned call. No re-pin was needed because the binary already
carried it; admission was the routed set plus the `FORK_MODES` entry.

Worth knowing why it was invisible: the routing comment above
`GROUPED_DMA_STORE_SOURCES` had described `080b5ad4`'s blocker precisely — "value0
has a special case in that pass and value2 has none" — and the fork had since
written that special case. The note was correct when made and stale when read,
which is §6's pattern applied to the compiler lane rather than to a park note.
**When a routing comment names a missing compiler capability, check whether the
fork has since grown it.** Extract the option strings from `cc1` and diff:

```sh
strings toolchain/alchemy-gcc/cc1 | grep -oE '^(thumb-[a-z0-9-]+|grouped-dma-store)$' | sort -u
```

Everything else that scan turned up is already either routed or in `FORK_MODES`;
`-fthumb-group-value2-in-place` was the only dark one, so this seam is now closed,
but re-run the diff after any fork change.

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
- **`0801a4fc`** (main image, floor 2, 166 bytes): **value0's copy versus value2's
  retargeted definition** in the grouped-descriptor pass. With
  `-mgrouped-dma-store,-fthumb-group-value2-in-place` everything matches except
  the order of two insns before the `stmia`: we emit `ldr r2,[pc]` then
  `adds r0,r5,#0`, the reference emits the copy first. In `arm.c` the pass emits
  `SET r0, value0` *before* `store2` while `-fthumb-group-value2-in-place`
  retargets value2's constant definition **in place**, so their relative order is
  decided by where the constant was defined in the RTL stream, which no source
  spelling moved (four placements of the control word tried, plus the full flag
  matrix with pairing). This is the natural next fork change: emit the value0 copy
  ahead of the retargeted value2 definition. It needs the §7 admission ritual
  (rebuild, re-pin, prove the source-only build still reproduces the SHA-1), so it
  is a deliberate compiler-lane task, not a probe. Draft is at floor 2 and ready.
- **`080c1fa8`** (main image, floor 2): a **`mov rN,sp` versus argument-setter**
  ordinal tie-break — the reference materialises the stack base one slot earlier
  (`mov r6,sp` before `adds r0,r3,#0`; we emit the reverse). Distinct from the
  r0-r3 case `-fsched-low-dest-first` covers, because the earlier insn writes a
  high register from `sp`. Hand levers took this function 5 → 2 (see
  `docs/REPROBE-2026-07-30.md`); the residual survived all 40 flag settings,
  every pairing with `-fno-sched-depend-count`, four statement placements, a
  declaration-initialiser spelling and a return-type sweep. Draft ready at
  `work/reprobe-2026-07-30/reordered/080c1fa8.c`, 84 bytes, baseline flags.
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

**Venus Lighthouse candidates worth re-probing here.** Venus covers these
semantically today; an exact result from Mercury would override its version, so
they carry more value than their byte counts suggest. Both were identified but
never re-probed: `resource_3c8:1d48` (floor 3 halfwords) and `resource_379:0074`
(8 of 288 instruction groups differing — measure this one by group equality, not
halfwords, per §2).

**The 60-byte squared-distance family — floor 20/60 bytes, two named residuals,
780 bytes behind it.** One fingerprint (`1fwqz6zhzfrzo`), 13 strict unconverted
members, every one 60 bytes: `resource_373:0030`, `389:0030`, `391:0030`,
`392:0030`, `393:0030`, `39f:0030`, `3b2:0030`, `3b4:0030`, `3b5:0040`,
`3bf:0030`, `3c4:0030`, `3c5:0030`, `3c8:02f0`. It is the cheapest large payoff
in the overlay queue — one correct draft closes all 13.

The shape is a 3D squared distance on 16.16 fixed point, and **the call takes four
arguments, not one**. That was the whole discovery: the reference's `ldr r3,[pc]`
of `0x030001d8` immediately before the `bl` is the *fourth* argument, and the
`dz*dz`/`dy*dy` left in r1/r2 are arguments 2 and 3, not dead intermediates.
Spelling it one-argument floors at 46 differing bytes; two-argument at 23;
four-argument at **20**. The constant is a plain literal, not `&Value_` — it has
no `k<<n` factorisation, so it pools by itself (§4's third row).

Best draft (`differing_bytes=20`, size exact; `e.c`/`j.c`/`l.c` all reach it):

```c
typedef signed int s32;
extern s32 Func_020061c0(s32 sum, s32 c, s32 b, s32 context);
s32 Func_02000030(s32 *a, s32 *b) {
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    return Func_020061c0(dx * dx + dy * dy + dz * dz, dz * dz, dy * dy, 0x030001d8);
}
```

Instructions 1-9 and 13-20 match the reference exactly. Exactly two residuals
remain, and they are worth attacking as one:

1. The reference emits `subs r3,r3,r2` (the `dz` delta) **before** all three
   `asrs`; we emit it between the second and third. Writing the three deltas as
   plain statements and shifting them afterwards *does* produce that order, but
   scrambles register allocation from instruction 4 onward (24 bytes, worse).
2. The reference spends an extra `mov r3,r1` scratch copy before the final
   `adds r0,r0,r3`, then immediately reloads r3 with the pooled constant. We fold
   it to `adds r0,r0,r1`. This is the one instruction of the size difference
   (26 emitted against 27).

**All nine routed modes were swept against the four-argument draft and none beats
baseline** — `-fsched-low-dest-first` is actively worse (15 mismatched
instructions against 10), `-fno-sched-depend-count` worse (12), the rest are
neutral. So this is a source-shape problem, not a routing problem; do not spend
another sweep on it. The untried levers are §4's return-type lever on
`Func_020061c0` (it is currently spelled `s32`-returning) and a struct/array
spelling of the coordinate triple.

Note for whoever picks this up: `work/claude/overlay_verify.ts` still does not
exist on `mercury`, and `mode_sweep.ts` is main-image only — it resolves a stem to
a ROM address, so it cannot take an overlay target. The working overlay loop is a
**dry-run `overlay_adopt.ts`** (no `--apply`), which prints
`differing_bytes=N size=A/B` in about a second:

```sh
bun tools/overlay_adopt.ts resource_373:0030 --source draft.c --span 60
```

Equal sizes confirm the span; drive `differing_bytes` to 0. Because that path
takes flags from routing rather than argv, sweep modes by compiling with `xgcc`
directly and diffing the assembly against `overlay_show.ts` output.

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

**How to decide the `volatile` question mechanically, instead of arguing intent.**
Three drafting lanes hit it on 2026-07-30 and it resolved three different ways, so
the rule is worth stating: **delete the `volatile`, re-probe, then classify.**

| after deleting it | what it means | verdict |
| --- | --- | --- |
| count unchanged | it was never load-bearing | drop it, adopt (`080060e8`) |
| count worsens, address is memory-mapped or externally mutated, and the reference genuinely repeats the access | semantics the target requires | keep it, adopt (`08006cdc`, `0800eaf8`) |
| count worsens, object is an ordinary local | a matching device | **reject the draft** (`080b386c`) |

The worked cases: `080060e8` marked a timer register `volatile` and reached 0
without it, so it was noise. `08006cdc` writes a Flash command sequence to
`0x0E005555`/`0x0E002AAA`, where repeated stores to one address *are* the
protocol, and plain C would let the compiler fold them away and break the
hardware. `0800eaf8` re-reads one IWRAM global across a chain of `else if` tests
and comes out 8 bytes short without `volatile`, which is exactly §4's documented
tell. `080b386c` declared `volatile u16 slots[15]` — an ordinary stack array with
nothing external touching it, worth 49 halfwords purely by suppressing store
elimination. That last one is the prohibited case and its draft was rejected
despite measuring 0.

**A lane's `confirmed` is not sufficient to adopt.** One verify lane passed
`080060e8` on the reasoning that its `volatile` was legitimate MMIO; the
coordinator's independent screen caught it and the delete-and-re-probe test showed
it was not even needed. Re-screen and re-measure everything at the coordinator
before installing it. Two policy escapes were caught this way in one session, the
other being the register-pinned `080044d0` drafts.

**Concurrency — do not inherit the old two-lane rule.** That limit was measured
when a verification probe cost 1.8 s and a bank cycle 190 s, so compute really was
the binding constraint. The content caches (§3) cut those to 0.12 s and 15 s, and
the limit was never re-derived. Measured afterwards on the same 4-core host with
two lanes running: **load average 1.18, and a probe costs 137 ms under load versus
120 ms idle** — roughly 70% of the machine idle. Lanes are reasoning-bound, not
compute-bound, so run **five or six** and re-measure `/proc/loadavg` before
assuming a ceiling. If probe latency starts climbing well above ~150 ms, that is
the real signal to stop adding lanes.

**Runtime subagent slots are a separate ceiling from cores.** Some runtimes cap
concurrent subagents independently of the hardware — the Codex runtime exposes
three, so a six-agent experiment there runs as two immediately consecutive waves
of three, and Venus Lighthouse cohorts are sized in threes for that reason. On a
runtime exposing five or six slots the measurements above support testing the
larger walker count rather than inheriting the historical two-lane ceiling.
Whichever binds first — slots or cores — is the real limit; check both.

**Where five lanes actually saturates a 4-core host.** Measured with five lanes
running: load average **3.60**, and a warm `build_claimed` that costs ~0 s idle
took **17.6 s** — longer than the entire warm bank cycle on a quiet machine. That
is the ceiling arriving, not a fluke, and a sixth lane there buys nothing but
contention. Memory is never the constraint (943 MB of 16 GB at five lanes). So
the rule is not "five lanes"; it is **roughly one lane per core, verified against
loadavg and probe latency**. On a bigger host, scale up — see §10.

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

**Do not spend an agent on enumerable search — that is the expensive mistake, and
it is independent of model tier.** Measured 2026-07-30 on an 18-core host: `xargs`
over `candidate_show` sustains **114 ms per probe at 18-way concurrency**, so a
flag matrix of 38,480 probes finishes in about four minutes and produced 2 of that
day's 8 conversions on its own. An agent exploring 20 source spellings takes about
six minutes. That is roughly four orders of magnitude less search per minute.
Split the work by whether it can be enumerated:

- **Enumerable** — flag sets and their pairings, callee return types, independent
  statement orderings, constant spellings: script it. `tools/mode_sweep.ts`,
  `tools/return_type_sweep_main.sh`, `tools/statement_order_sweep_main.ts`.
- **Not enumerable** — deciding which lever a residual implies, decoding assembly
  into semantics, noticing that an epilogue contradicts a draft's signature: that
  is what an agent is for, and it wants a strong model.

**Cheap-model breadth was measured and did not substitute for lever selection.**
Four haiku lanes were each given a size-exact near-miss with its residual quoted,
the lever sections named, and the sweeps pointed out. Result: **0 of 4 improved**
across 66 spellings, ~6-8 minutes and ~90k tokens per lane, with honest reports
(one correctly spotted that the statement-order sweep had only produced 3 probes
on its target, so that lever had never been exercised there). Caveat, stated
plainly: those four were the residual-hard remainder — two had already resisted a
full-effort attempt — so this is not a controlled comparison and should not be
read as a model benchmark. What it does show is that quoting the fingerprint and
listing the levers is not enough; choosing the lever is the costly step.

**Workflow orchestration is not the overhead people assume.** Verify stages fired
the moment each solve returned and nothing queued at 12 lanes. But note the cap:
concurrent agents per workflow are `min(16, cores - 2)`, so on an 18-core host a
32-agent workflow runs 16 and queues the rest. For very wide cheap breadth,
separate `Agent` calls avoid that ceiling; for pipelined solve-then-verify work
the workflow is the right shape.

---

## 10. Mercury Lighthouse — running the exact-C effort on a high-compute host

This section is written for an agent taking `mercury` over with substantially
more compute than the 4-core cloud container the measurements below come from.
Those sessions were **reasoning-bound per lane but core-bound in aggregate**; more
cores changes only the second term, so read this as "how to spend cores", not
"how to work". Everything in §1-§9 still applies unchanged.

**Bringing up a bare cloud container (2026-07-30, measured end to end: ~35 min).**
A fresh clone cannot run anything in the exact lane: `bun run verify`,
`candidate_show.ts` and `overlay_verify` all read `roms/gs1-en.gba`, and every
compile routes through `../alchemy-gcc/dist`, which is *built*, not committed.
Four separate things have to be right, and three of them fail in ways that look
like a repository bug rather than a missing prerequisite.

1. **The ROM is yours to supply.** `roms/` and `*.gba` are gitignored and must
   stay that way. Drop the approved image at `roms/gs1-en.gba` and check it
   against tracked `rom.sha1` (`5c46952054…`) before trusting a single measurement.
   Then `git config core.hooksPath .hooks` exactly as `PROVENANCE.md` says.
2. **`arm-none-eabi-as`/`objcopy` are host binutils, not part of the bundle.**
   `build_asm.ts` shells out to them by bare name. `apt-get install -y
   --no-install-recommends binutils-arm-none-eabi` — a few seconds.
3. **Build the compilers from the sibling repo.** Clone `PascalPixel/alchemy-gcc`
   next to this checkout so `../alchemy-gcc/dist` resolves, then
   `./build.sh <target> && ./stage.sh <target>`. gcc296 takes ~3 min on 4 cores;
   agbcc, gcc3 and gs2 a few more each. **`stage.sh` has no token for
   `pretearlythumb` or `gcc2951`** even though `build.sh` does — copy those two
   `cc1` binaries to `dist/pret-early-thumb/cc1` and `dist/gcc2951/cc1` by hand.
   Both are needed: `alchemy_gcc.ts --self-test` builds a plan for each, so
   `bun run verify` fails without them.
4. **Re-stamp the vendored generated files after cloning.** `alchemy-gcc` ships
   pre-generated `c-parse.c`/`c-gperf.h`/`configure` "timestamp-pinned newer than
   their inputs" — but **git does not preserve mtimes**, so a fresh clone lands
   them all in the same checkout second and make regenerates them. gcc-2.95.1
   then dies in modern bison on a 1999 grammar (`$$ for the midrule at $4 of
   'structsp' has no declared type`). `touch` the generated files in each
   vendored tree before building. Symptom to recognise: any build failure that
   names bison, gperf or autoconf is this, not a broken tree.

**Match the pinned Bun.** `package.json` pins `bun@1.3.14`; the container shipped
1.3.11 and `gba_header.ts --self-test` failed with "GBA logo source must be the
canonical 104x16 monochrome PNG". That check re-encodes the tracked PNG and
demands byte equality, and its IDAT comes from `deflateSync(rows, {level: 9})` —
i.e. it is pinned to Bun's bundled zlib. It is a Bun-version tell, not a corrupt
asset; `git status` on the PNG is clean throughout. Install the pinned version
before concluding anything about the assets.

**A locally built bundle will not match the pinned digests, and that is expected.**
gcc embeds prefix paths, so a from-source build at a different path differs
byte-for-byte while its *codegen* is identical — `build_claimed` came back
`linked=1406 failures=0` on the first try. The one target that reproduced the
pinned digest exactly was `pret-early-thumb`. So do not read a digest mismatch as
a broken build, and do not read a matching digest as the only admissible outcome.
`EXPECTED` now holds a **list** of approved digests per file rather than one, so a
second host can be admitted without evicting the first. The bar for adding to
that list is unchanged and is the whole point: run `bun run verify` and get
`byte_identical=yes rom_fallback_bytes=0` with that bundle first. This container's
bundle was admitted that way on 2026-07-30.

**What the ceiling actually is.** Measured on the 4-core cloud host: one lane
idles at ~120 ms per probe; two lanes, load 1.18, 137 ms; five lanes, load 3.60,
and a warm `build_claimed` that costs ~0 s idle stretched to 17.6 s. Memory never
bound (943 MB of 16 GB at five lanes). So the working rule is **about one lane per
core**, and the falsifiable stopping signal is **probe latency climbing past
~150 ms** — measure it, do not guess it. On a 16-core machine expect 16-20 lanes
to be sane; the scaling is close to linear because lanes share nothing but the
repo.

**Give every drafting lane its own output directory.** Lanes told to write
`work/<shared>/<stem>.c` destroyed each other's files between tranches on
2026-07-30: five confirmed byte-exact drafts vanished, four were recovered only
because the *verify* lanes had copied them into their own scratch, and two regions
that had been solved had to be re-drafted from scratch. Use
`work/<run>/lanes/<stem>/<stem>.c`, and adopt confirmed results promptly instead
of batching them across tranches. Bytes measured exact and then lost are the most
expensive outcome available.

**What must stay serial, at any core count.** These are correctness constraints,
not performance ones:
- Two lanes must **never share an overlay**. The overlay `.s` file is the only
  mutable artifact, and adoption rewrites it. Assign whole overlays — the
  "whole owner, not manifest row" rule.
- Only the coordinator runs git, the build scripts, `full_c_progress.ts` or
  `overlay_inventory.ts`. `tools/bank_cycle.sh` already tolerates lanes adopting
  mid-build (it retries), so lanes never need pausing to bank.

**Assign overlays by small-row count, not by total strict bytes.** This was
measured the hard way: `resource_3b8` ranks second by strict bytes (15,684) but
its queue is 7 rows whose smallest is 348 bytes and whose largest is 7,468 — a
lane spent a full session there and adopted nothing. Overlays with many small
rows convert; overlays with a few huge rows park. Rank candidates by rows under
~400 bytes.

**The largest recoverable backlog is stale park notes, not new territory.** Two
independent classes were disproved this session (§6): the "repeated same-bb
two-instruction immediate" class, which the paired CSE modes close, and several
notes predating `-fsched-low-dest-first` and `-fno-sched-depend-count`. One lane
re-probed six old notes in ~4 minutes and closed three. With many cores, a
dedicated **re-probe sweep across every existing note in `work/claude/notes/`** is
almost certainly higher yield per core-hour than walking new overlays, and it is
purely mechanical. Do that first.

  *Measured on the main image, 2026-07-30, and it does **not** hold there.* An
  18-core run took that advice to exhaustion on `mercury`: 1,035 drafted-but-
  unadopted stems, deduplicated to 3,188 drafts, of which 964 both compile and
  pass the policy screen. Sweeping all 964 against 20 flag settings — each routed
  mode alone, both CSE modes paired, and the §5 compositions — is **19,280 probes
  in 122 s** and yielded **exactly one exact match** (`08078144`, 228 bytes,
  adopted). A 1-in-964 yield is not a backlog. The overlay-side seam this
  paragraph describes was real; **the main-image drafted population has no
  equivalent, so do not budget a session for it.** Full measurement in
  `docs/REPROBE-2026-07-30.md`.

  The tempting inference was that because *no* note under `work/` mentions
  `-fsched-low-dest-first` or either CSE mode — the corpus wholly predates them —
  those modes must be holding back a batch of main-image functions. That is the
  §6 stale-evidence shape, it was the right thing to test, and it is **false**:
  across 964 drafts they produced no additional exact match. They still move
  individual halfword counts, so keep probing them per function; do not expect a
  cohort.

**Screen drafts against `PROVENANCE.md` before believing a zero.** The same run
found two drafts scoring 0 halfwords at 212/212 in `work/hand/080044d0/exact*/`
that are **inadmissible**: they get there with `register s32 ratio asm("r1")`.
Ranking a draft population by score alone puts register-pinned and barrier drafts
at the top of the list, where they read as free adoptions. Grep for `asm(`,
`register … asm`, `volatile` and `__attribute` first. `080044d0`'s best clean
variant is 1 halfword at exact size, which makes it the best ratio in the project
(212 bytes behind one register identity) — and the pinned drafts conveniently
prove which assignment to aim a legitimate allocation lever at.

**The return-type lever had never been run on the main image at all.**
`tools/return_type_sweep.sh` scores through `work/claude/overlay_verify.ts`,
which does not exist on `mercury` and only accepts `<overlay:offset>` targets, so
every main-image park predates the lever entirely — §5's "treat a sweep null
before 2026-07-30 as unmeasured" understates it here, because there was never a
null to record. `tools/return_type_sweep_main.sh` ports it to
`candidate_show.ts`, never overwrites the input draft, and prefers a size-exact
result at equal halfword count. 494 sweeps in 29 s improved 67 drafts (`080c0be4`
105 → 84, `080aad10` 112 → 95) and closed none; improved drafts are saved in
`work/reprobe-2026-07-30/drafts/`. Run it on any newly drafted main-image
function before parking it.

**Host concurrency: trust probe latency, not load average.** On an 18-core M5 Max
with 18 concurrent probes, load average reached **25.6** while per-probe latency
stayed at **~114 ms against a 116 ms idle baseline**. §10's stopping signal
(latency past ~150 ms) never triggered. The 4-core "roughly one lane per core"
rule understates a large machine; load average above core count is not by itself
the ceiling.

**Where to resume walking**, with the next offset each lane had already decoded:
`resource_3c8:07d8` (200), `resource_39f:1520` (176), `resource_38f:0304` (196),
`resource_372:1348` (336 — a direct structural sibling of the adopted `0f38` and
the drafted `1154`; copying `work/claude/resource_372-1154.c` and substituting
constants should land it quickly). `resource_3b2` (74 rows) and `resource_374`
(47 rows) are the best unassigned overlays by the small-row criterion.

**Measured 2026-07-30: an 18-core host did not beat the 4-core one on bytes.**
Daily exact-byte gains on the 4-core cloud host, from `docs/full-c-history.csv`:
1,426 / 12,720 / 5,148 / 7,400 / 2,484 / 6,328 / 21,792 / 7,174 / 14,634 / 6,468 /
10,282 / 5,134. Median about 6,800. A full session on the 18-core machine
produced **2,554 bytes**, below their typical day. Confounders, stated so the
comparison is not oversold: those days include overlay conversions, which come in
larger and easier rows, while that session was main-image only; a large share of
it went into tooling and measurement rather than conversion; and it was hours, not
a day.

The conclusion is not "the big machine is bad", it is that §9 was right and the
implication is sharper than it looks: **compute makes enumerable search free, and
enumerable search is nearly exhausted.** In that session 38,480 flag probes ran in
about four minutes and yielded 2 conversions, while agent drafting lanes yielded
7. Cores now buy you `tools/finish_draft.sh` finishing in ~2 s instead of an hour
of hand-probing — real, permanent, and *not* where the remaining bytes are.

So the thing to scale is **concurrent drafting lanes on unconverted regions**, not
probes and not lanes grinding near-miss residuals. Measured hit rates on the same
day: fresh drafting converted **7 of ~18 lanes**; re-probing existing drafts
converted **2 of 1,259 draft-probes**; four cheap-model lanes on residual-hard
near-misses converted **0 of 4**. The binding limits are agent concurrency and
model quality on assembly-to-semantics, so pick the host for how many strong
drafting lanes it can run, not for its core count.

**When extra compute stops paying — hand the branch back.** More cores buy
throughput on *parallelisable* work, and this project has two kinds. Fan-out work
scales: walking fresh overlays, the stale-note re-probe sweep, mode sweeps,
`return_type_sweep.sh`, `permute_overlay.ts` runs. Serial work does not: the
compiler problems in §8, a park that needs a new lever rather than another probe,
and any question about whether a mode is admissible at all. Those are one-agent,
one-thread problems where twenty lanes produce twenty copies of the same floor.

The signal to hand back is therefore **not** a byte count — it is when the
*remaining* work is mostly of the second kind. Concretely, hand `mercury` back
when any of these hold:

- The re-probe sweep over `work/claude/notes/` is exhausted and the newly closed
  functions have dried up.
- Lanes are returning parks rather than adoptions — say, under a third of started
  functions closing — because that means the frontier has moved from "apply a
  known lever" to "find a new one".
- The queue that remains is dominated by large rows (over ~1 KB) and the §8
  compiler problems, which are transcription and analysis work, not throughput.
- Probe latency stays flat as lanes are added, but conversion rate does not rise —
  the machine has headroom and the *method* is the constraint.

**How to hand back.** Push `mercury`; every cycle is banked and pushed, so it is
always resumable from origin, and there is no session state outside the repo.
Before handing back, make sure this file reflects what the run learned: §1's
overlay ranking, §4 any new levers, §6 any park class proved or disproved, §7 any
routed mode, §8 the open compiler problems. Levers and disproved park classes are
the most valuable thing a high-compute run produces — a closed function is worth
its bytes, but a lever is worth every function of its shape. Record them even when
the run that found them had cores to spare.

Do not leave findings only in `work/claude/notes/`; the notes decay into stale
evidence exactly the way §6 describes, and this session recovered five separate
categories of it. If a note's blocker was later closed by a mode or a lever, edit
the note rather than leaving the contradiction for whoever reads it next.

**`work/` is in `.gitignore` — findings left there do not survive the session.**
This is stronger than the paragraph above and was learned the hard way on
2026-07-30: a full session of notes and 238 improved drafts sat under
`work/reprobe-2026-07-30/` and were invisible to `git`, so a bank cycle reported
"nothing staged; tree matches HEAD" while the tree looked full of new work. Every
per-function note this project has ever written is in the same position, which is
the mechanical reason the notes corpus decays.

So: **anything you want the next session to have goes in `HANDOVER.md` or
`docs/`.** That session's record is `docs/REPROBE-2026-07-30.md`, with per-stem
floors and their winning flags in `docs/reprobe-2026-07-30-floors.tsv` and
`docs/reprobe-2026-07-30-mode-matrix.tsv`. Drafts stay untracked by convention;
regenerate a floor with `tools/finish_draft.sh` rather than trusting a number
copied from a note.
