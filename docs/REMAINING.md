# What is actually left, measured (2026-07-26)

813 `c_candidate` regions remain at `[1,245 of 2,058]`. Breaking that down
changes what the next move should be.

| | count |
| --- | --- |
| contains `mov ip, pc`, not emittable by either approved compiler | 43 |
| already attempted (mostly the documented plateaus) | 267 |
| never attempted and reachable | 503 |

And the 503 by size:

| instructions | regions |
| --- | --- |
| <= 40 | **14** |
| 41-80 | 72 |
| 81-160 | 156 |
| 161-320 | 126 |
| > 320 | 135 |

## The easy tier is exhausted

Every conversion landed today came from a small region whose natural C happened
to match. **There are 14 small regions left**, and the ones at the head of that
list are already spoken for:

- `0800d304`, `080c08a8`, `080170c4` are the pinned grouped-DMA near-misses,
  scheduler-internal residuals, documented in `alchemy_gcc.ts`;
- `08004c6c`, `08004c1c` are the twelve-store matrix family, blocked on the
  recognizer breaking at `index != 12` before its guards
  (`work/hand/08004bd4/NOTES.md`);
- `08005394` is the alloca-DMA family, blocked on allocation around the alloca
  (`work/hand/080054e4/NOTES.md`);
- `0800070c` tail-jumps to RAM via `ldr r3, [pc]` / `bx r3` and calls an
  odd-tagged address: another nonstandard-call construct, not ordinary compiler
  output, and probably belongs with the 43.

So the next region converted by hand will be a **41-80 instruction** one at
best, and 417 of the 503 are 81 instructions or more. Bigger functions carry
more live values, which is precisely what produces the allocation and scheduling
plateaus that ~20 documented near-misses already sit on.

## What this implies

Hand conversion got 6 regions today and the tier that made that possible is now
empty. The remaining 503 are not more of the same work, they are harder work.
Three things are worth more than continuing:

1. **The `rank_for_schedule` tie-break.** ~15 documented near-misses are within
   10 bytes on ready-list ordering alone, and three of them
   (08003e10 / 08019bac / 08002fb0) share one identical residual. That is one
   compiler behaviour standing between a batch of regions and conversion. The
   forensics in `work/hand/080a1090/NOTES.md` narrowed it to a single clause;
   the next step is instrumenting that clause, not guessing another mode.
2. **Classify the 43.** They are counted in Y, they cannot be emitted, and two
   of them sit near the top of any size-ordered target list. Every session that
   picks one up loses hours. `0800070c` is a candidate 44th.
3. **The compiler-blocked families.** The alloca-DMA and twelve-store matrix
   groups are ~11 regions between them with diagnosed, specific blockers.

Only after those does grinding 81-160 instruction regions by hand make sense.
