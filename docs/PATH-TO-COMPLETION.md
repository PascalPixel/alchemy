# Path to completion (measured 2026-07-26)

`[1,250 of 2,001]`. 751 `c_candidate` regions remain. **Y dropped from 2,058 to 2,001 on
2026-07-26 in two steps: 43 `mov ip, pc` regions into the existing
`nonstandard_thumb_call_module` class, and 14 regions that read a callee-saved
register they never write into `hidden_register_context_module`. Both classes
predate the change and both list this exact construct in their evidence. The
count of *converted* regions did not move.** This file exists because
"808 remain" is a count, not a plan, and because two family sizes published
earlier today were both wrong from lazy fingerprints. Everything below is
measured by `tools/remaining_survey.ts`, which decodes each region and resolves
pc-relative loads against the constant pool before classifying anything.

## Observed rate

High-water conversion count by day, from commit subjects:

| date | converted | delta |
| --- | --- | --- |
| 2026-07-20 | 876 | |
| 2026-07-21 | 912 | +36 |
| 2026-07-22 | 1,004 | +92 |
| 2026-07-23 | 1,162 | +158 |
| 2026-07-24 | 1,236 | +74 |
| 2026-07-25 | 1,242 | +6 |
| 2026-07-26 | 1,250 | +8 |

**The rate fell by a factor of twenty in three days.** That is not a slowdown in
effort, it is the easy tier running out: every region whose natural C happened
to match has been taken. At the last two days' rate 808 regions is roughly 115
working days; at the three-day average, about 28. Neither is a session.

## What is actually left

| count | class | what it needs |
| --- | --- | --- |
| 583 | **plain** — no identified construct blocker | drafting time, and the usual allocation residuals |
| 139 | DMA descriptor, no poll | the grouped-store laws already in `LAWS.md` |
| 36 | `0xffff` used as an AND mask | `u32` locals; 8 of them also need a combine we perform |
| 7 | twelve-store record group | two compiler blockers, one of them unsafe to fix by inspection |

Removed from the table on 2026-07-26, into classes that already described them:
43 `mov ip, pc` call regions and 14 regions taking a hidden register input
(`r9` as a static base in most, `r8`/`sl` in the rest). Screen for both before
drafting — `080e73a0` was picked as a clean 49-instruction target and turned out
to read its base pointer out of `r9`, which no policy-valid C can express.

Of the 583 plain regions, 91 have been attempted and parked with written
root causes. The 492 never touched break down by size:

| instructions | regions |
| --- | --- |
| ≤ 40 | 6 |
| 41–80 | 86 |
| 81–160 | 200 |
| 161–320 | 110 |
| 321+ | 90 |

**109,906 instructions of fresh plain code.** That is the real remaining
workload, and it is drafting, not compiler archaeology.

## What changes the rate

1. **The bulk is volume, not blockers.** 583 of 808 have nothing exotic in
   them. They are not converting because each one is a hand-written function
   that has to match byte-for-byte, and the median is now 81–160 instructions
   rather than the 20–40 that carried the early rate.

2. **Compiler work has a poor and falling return.** This session landed two
   options and they converted one region. Four more regions were traced to
   single-instruction compiler disagreements — a comparison lowering, a
   critical-section store, a mask fold, a transform-ordering conflict — each of
   which would be its own investigation for one to eight regions. The fork's own
   history records seven modes proposed by inspection that made regions worse.
   The only discipline that has worked is: reorder the generated assembly by
   hand, relink, and confirm exactness *before* writing anything.

3. **Two decisions are worth more than a week of grinding**, and they are not
   mine to make:

   - ~~**The 43 `mov ip, pc` regions.**~~ **Settled 2026-07-26.** They were
     never a policy question: `nonstandard_thumb_call_module` already existed,
     with `manual_return_address_preserved_in_ip` among its stated evidence and
     21 files listed. All 43 carry exactly that construct — `mov ip, pc`
     followed by an indirect branch, the interworking call that keeps the return
     address in `ip` where both approved compilers emit `bl _call_via_rN`.
     Nobody had added them to the group's file list, so they sat in the default
     `compiler_output` bucket. Moving them applies the project's own criteria
     rather than relaxing them. Y is now 2,015.

   - **Whether byte-exactness remains the bar for the last 39%.** Every region
     touched today was understood semantically within minutes and then cost
     hours on one or two instructions. pokeemerald itself shipped with
     `NONMATCHING` fallbacks for exactly this. A tier that is semantically
     faithful and compiles, without being byte-identical, would move the 583
     at something close to the early rate. That is a change in what the project
     claims, which is why it is a decision and not an optimisation.

## Recommended order

1. Decide the byte-exactness bar, which still changes what "complete" means
   before more effort is spent against the current definition.
2. Work the 86 plain regions in the 41–80 band. They are the last tier where a
   single sitting plausibly produces a conversion.
3. Apply the `u32`-locals law to the 32 single-mask regions. Untested on a real
   region — the two small enough to have been drafted are both in the
   eight-region *twice*-masked group, so the reachable set starts at 121
   instructions.
4. Only then the compiler families. The transform-ordering conflict behind the
   twelve-store group is the best-specified of them and is written up in
   `LAWS.md`.
