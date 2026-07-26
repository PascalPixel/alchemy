# Path to completion (measured 2026-07-26)

`[1,273 of 2,001]`. 728 `c_candidate` regions remain. **Y dropped from 2,058 to 2,001 on
2026-07-26 in two steps: 43 `mov ip, pc` regions into the existing
`nonstandard_thumb_call_module` class, and 14 regions that read a callee-saved
register they never write into `hidden_register_context_module`. Both classes
predate the change and both list this exact construct in their evidence. The
count of *converted* regions did not move.** This file exists because a
remaining-region headline is a count, not a plan, and because two family sizes published
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
| 2026-07-26 | 1,273 | +31 |

**The rate is still roughly a factor of five below the 2026-07-23 peak.** That is
not a slowdown in effort: the broad easy tier is running out, and compiler
lineage now matters as much as drafting. At the last two days' rate 728 regions
is roughly 39 working days; at the three-day average, about 20. The estimates
move materially with one cohort and neither is a session.

## What is actually left

| count | class | what it needs |
| --- | --- | --- |
| 549 | **plain** — no identified construct blocker | drafting time, and the usual allocation residuals |
| 136 | DMA descriptor, no poll | the grouped-store laws already in `LAWS.md` |
| 36 | `0xffff` used as an AND mask | `u32` locals; 8 of them also need a combine we perform |
| 7 | twelve-store record group | two compiler blockers, one of them unsafe to fix by inspection |

Removed from the table on 2026-07-26, into classes that already described them:
43 `mov ip, pc` call regions and 14 regions taking a hidden register input
(`r9` as a static base in most, `r8`/`sl` in the rest). Screen for both before
drafting — `080e73a0` was picked as a clean 49-instruction target and turned out
to read its base pointer out of `r9`, which no policy-valid C can express.

Of the 549 plain regions, 95 have been attempted and parked with written
root causes. The 454 never touched break down by size:

| instructions | regions |
| --- | --- |
| ≤ 40 | 5 |
| 41–80 | 63 |
| 81–160 | 193 |
| 161–320 | 105 |
| 321+ | 88 |

**106,225 instructions of fresh plain code.** That is the real remaining
workload, and it is drafting, not compiler archaeology.

## What five drafts on 2026-07-26 actually showed

Five regions were drafted after the easy tier ran out. **None converted**, and
every one landed the same way: **size exact, semantics right, parked on a
handful of instructions of register allocation or expression lowering.**

| region | insns | residual | what blocks it |
| --- | --- | --- | --- |
| `08006088` | 42 | 11 hw of 96 | a register copy and a bit-extract lowering that cancel each other |
| `080ae9f0` | 41 | 13 hw of 92 | which parameter the allocator leaves in place |
| `080936a0` | 43 | 30 hw of 112 | a rematerialised zero, one transposition |
| `080a1f74` | 40 | 26 hw of 96 | how `(s8)x == -1` is lowered |
| `0800651c` | 20 | 33 hw of 64 | a critical-section store our compiler will not emit |

In each case the C was written and understood in minutes. The semantics were
never the hard part. **The 41–80 band was called "the last tier where a single
sitting plausibly produces a conversion" earlier in this document, and today's
evidence says even that was optimistic** — the tier is entered easily and
finished rarely.

What did convert in that pass (`08005a78`) needed two new compiler options, and was only
attempted because its residual could be hand-verified by reordering the
generated assembly and relinking.

So that pass's cost model was not "drafting alone". It was "drafting is cheap,
and the last three to fifteen instructions of each region are a compiler
investigation". Any estimate that prices the remainder at drafting speed is
wrong.

## What the next cohort changed

The next bounded cohort produced **four exact conversions without adding a new
compiler transform**:

| region | prior state | exact route |
| --- | --- | --- |
| `08006c68` | fresh, 47 insns | adjacent flash-family `old_agbcc -O1` fingerprint |
| `0800711c` | fresh, 56 insns | the same flash-family fingerprint |
| `0801ed40` | fresh, 59 insns | natural table-selection C with the existing `-fno-gcse` route |
| `08021d88` | parked at four halfwords | source ordering plus existing `-fno-sched-depend-count` |

The fourth fresh draft, `080788c4` at 60 instructions, is semantically complete
and size-exact but remains parked at 23 halfwords: one signed-versus-unsigned
halfword load shape and one zero-literal allocation. Thus the fresh yield in
this cohort was three of four, not zero of five.

This does not establish exponential acceleration. It does show that the first
five-draft sample was too pessimistic: selecting a coherent translation-unit
family and testing already-evidenced compiler routes can remove the final
residual in batches. That is now the preferred fast path before proposing
another backend option.

## What the following cohort changed

The next six selected regions in the 41–80-instruction band all converted
exactly: five fresh drafts and one previously parked region.

| region | insns | prior state | exact route |
| --- | ---: | --- | --- |
| `08006f84` | 61 | fresh | the established flash-family `old_agbcc -O1` fingerprint |
| `08016868` | 65 | parked at residual 91 | duplicated case-tail source shape |
| `0801fc84` | 65 | fresh | ordinary C; early-result control flow |
| `0808e5d8` | 65 | fresh | ordinary C; typed call sequence |
| `080a847c` | 65 | fresh | materialized final argument and object load |
| `080b595c` | 63 | fresh | ordinary C; distinct existing literal-pool symbol |

That is five exact conversions from 319 fresh instructions plus one parked
rescue, with no new compiler transform. Five used the default compiler; the
flash sibling extended an already-proven compiler fingerprint by one focused
route. The `080a847c` residual is particularly useful: a semantically identical
140-byte draft was 14 halfwords away until two call operands were materialized
in source order, which changed GCC's live ranges and reproduced the original
high-register allocation without a register pin or backend change.

Two good cohorts do not imply exponential growth: each conversion still
removes only one region. They do, however, overturn the zero-of-five sample as
a delivery forecast. Family selection, existing compiler fingerprints, and
small source-level live-range changes are producing a temporarily high hit
rate in the remaining medium-small band.

## What changes the rate

1. **The bulk is volume, not blockers.** 549 of 728 have nothing exotic in
   them. They are not converting because each one is a hand-written function
   that has to match byte-for-byte, and the median is now 81–160 instructions
   rather than the 20–40 that carried the early rate.

2. **New compiler transforms have a poor and falling return; existing compiler
   fingerprints do not.** The first pass landed two options for one conversion.
   The next pass changed no compiler code and converted four regions by routing
   them through three already-evidenced modes. Four other regions remain traced
   to single-instruction compiler disagreements — a comparison lowering, a
   critical-section store, a mask fold, and a transform-ordering conflict —
   each of which would still be its own investigation for one to eight regions.
   The fork's history records seven modes proposed by inspection that made
   regions worse. The discipline is: test family/compiler fingerprints first;
   for a new transform, reorder the generated assembly by hand, relink, and
   confirm exactness *before* writing anything.

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

   - ~~**Whether byte-exactness remains the bar.**~~ **Settled 2026-07-26, and
     the earlier entry here was wrong on its facts.** It claimed pokeemerald
     "shipped with `NONMATCHING` fallbacks". Checked against the repo: **zero**
     `NONMATCHING` in `src/` or `include/`, **zero** `.s` files under `asm/`,
     310 C sources and 239 headers. The only two `NAKED` functions left are
     `MultiBootWaitCycles`, a cycle-counted delay that reads its own `pc` to
     tell ROM from EWRAM, and a librfu dummy callback — code that is inherently
     assembly, not code that resisted matching.

     `NONMATCHING` existed as scaffolding *during* pret's effort and was treated
     as debt and eliminated. Since the stated goal is pokeemerald as the desired
     *outcome*, and that outcome is 100% matching C with no assembly fallbacks,
     introducing a non-matching tier would move away from the goal, not toward
     it. **Byte-exactness stays.** Roughly 39 working days at the current
     two-day rate is the honest cost.

## Recommended order

Screen every target first: `grep 'mov\s*ip, pc'`, and check for a callee-saved
register read but never written. Both classes are now reclassified out of Y, but
the screen still costs less than a wasted draft.

1. Work the 63 plain regions in the 41–80 band. They are the last tier where a
   single sitting plausibly produces a conversion.
2. Apply the `u32`-locals law to the 25 single-mask regions. Eleven more use the
   mask two to four times and need the corresponding combine; the two small
   enough to have been drafted are both in that repeated-mask group, so the
   reachable set starts at 121 instructions.
3. Only then the compiler families. The transform-ordering conflict behind the
   twelve-store group is the best-specified of them and is written up in
   `LAWS.md`.
