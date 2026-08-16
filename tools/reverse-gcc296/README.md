# reverse-gcc296

Systematic source search against the byte oracle.

This is not a decompiler. Decompilation is hard because there is no oracle: you
guess at intent and nothing tells you whether you were right. Here the compiler
is a known, deterministic function from C to bytes, and we can compile and
compare. So the problem is not "recover the source", it is "find any preimage".

That reframing matters because gcc 2.96 is provably not injective on this
codebase. Identifier renames, callee aliasing through `#define`, and duplicating
a shared tail all produce byte-identical output. We do not need what Camelot
typed. We need any member of a large equivalence class.

## How it differs from `alchemy-permuter`

The permuter has the machinery: routed compilation, real linking, byte scoring,
deduplication, bounded parallelism. What it does is sample random mutations by
weight.

`reverse-gcc296` evaluates the **complete single-edit neighbourhood** of a small,
**measured** rewrite set and descends. The distinction is not academic. On
`080bbb0c`, 4,634 random permuter candidates found nothing, while a systematic
sweep of one transformation found a large improvement. Structure in the search
beat volume.

Use the permuter for broad randomized exploration. Use this when you want a
specific axis swept to exhaustion.

## The objective is size-independent

The score is mismatching instructions between the two disassemblies after
canonicalising addresses, **registers**, and **pc-relative offsets**. Lower is
better. The emitted byte size is reported and never constrained.

Three things are deliberate:

Registers must be blinded, or a run differing only in register allocation has no
matching text and surfaces as a large unmatched block, which reads as evidence of
block reordering and is not. Blinding dropped one owner from 974 mismatching
instructions to 489.

pc-relative offsets must be blinded, or every size delta is charged as structural
difference, which quietly reintroduces a bias toward the reference size.

`differing_halfwords` is **not** the objective and must not be used as one across
sizes. It is positional, so a size delta reshuffles which halfwords align and can
improve by luck. Measured: one candidate scored 1801 against another's 1656 while
being 17 instructions *closer*. In this tool's own validation run, six accepted
edits moved structural distance 519 to 385 while halfwords went 1801 to 1902.
Ranking by halfwords would have rejected all six.

## Rewrites

The set is small on purpose. Transformations measured to lose on real owners are
absent, because spending compiles on a known negative is waste.

- **Arm order.** `if (C) { A } else { B }` becomes `if (!(C)) { B } else { A }`.
  Behaviour-preserving; the condition is still evaluated once, in place. This is
  the axis that pays. Condition polarity alone is free; which arm is emitted
  first is not.
- **Statement order.** Exchange adjacent simple statements. Conservative: any
  call, any compound lvalue (`p->f`, `*p`, `a[i]`), any shared identifier, or any
  control-flow keyword blocks the swap.

Known losers, not implemented: natural indexed `for` loops for pointer-walking
scans (+263 to +309), clean loop rotation for the turn-order insertion (+340 to
+736), wholesale struct typing (+27 over pointer retyping alone).

## Usage

```
reverse-gcc296 semantic/080bbb0c.c              # descend, report, write nothing
reverse-gcc296 semantic/080bbb0c.c --apply      # write the improvement back
reverse-gcc296 semantic/080bbb0c.c --rounds 20 --jobs 12
reverse-gcc296 --self-test                      # no compiler needed
```

Requires `tools/candidate-show` built in release, and `arm-none-eabi-objdump`.
Parallelism defaults to 80 percent of cores.

Scratch candidates are written under the system temporary directory, named
`<stem>.c`, because routing resolves the reference by file stem.

## Limits

Greedy, not beam: it takes the best strict improvement each round and never
backtracks. Neighbourhoods are regenerated every round rather than batching
independent wins, because individually-improving edits are known not to compose
(four field renames each improved alone and were worse together than the best
one).

It does not touch register allocation, which is global and is where roughly half
of `080bbb0c`'s residual lives. No source-level rewrite reaches it.

The distance here is a longest-common-subsequence count and is not on the same
numeric scale as the hunk-based figure printed by the scratch `score.sh`. Compare
runs of the same tool, not numbers across tools.
