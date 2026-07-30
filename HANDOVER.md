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
| `-fsched-low-dest-first` | ordinal tie-break on r0-r3 argument setters | 139 |
| `-fsched-high-dest-first` | the same on r4-r12, ties with no call in them | 125 |
| `-fno-sched-alias` | a store/load pair proved independent and reordered | 82 |
| `-fsched-store-first` | a store sinking behind arithmetic | 308 |
| `-fno-gcse-insert-load` | a PRE-inserted load the reference lacks | 9 |

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
- **Immediate-build transposition.** The reference schedules an independent
  `movs r0,#K` *between* a two-instruction immediate's `movs` and its `lsls`. This
  is the residual class behind most remaining 2-5 halfword parks, and it is an
  `insn_cost`/`tmp_class` question at `haifa-sched.c:4068-4090`, not a
  dependent-count one. Attack it as a Thumb latency-model defect.
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

**Tooling.** `resource_39c:10c0` verifies at 0 halfwords but `overlay_adopt`
rejects it with 55 differing bytes. Its literal pool sits *inside* its span rather
than after the body, so the splice may displace a neighbour's pool words. Check
the routing-set trap in §7 first — that produced an identical symptom.

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

**Concurrency.** This host has 4 cores. Two walker lanes plus the main agent is
the useful maximum; more thrash the compile step. Walkers must never share an
overlay — that is the only mutable artifact — and should be told not to run git,
the build scripts, `full_c_progress.ts` or `overlay_inventory.ts`.

**Agent economics.** Permuting is an audit pass, not an engine: one exact hit in
65,543 candidates, though it cost ten minutes and cracked a function a careful
lane had declared unreachable. Run `tools/permute_overlay.ts` over a newly parked
near-miss before believing the park; do not expect it to close a backlog. Cheap
models are not useful for drafting here — the ≤32-byte "easy" population is 90%
veneer thunks, word-table interiors and mid-function fragments, and the genuine
small leaves are already taken as each walk passes them.
