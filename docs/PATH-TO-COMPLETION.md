# Path to completion (measured 2026-07-26)

`[1,263 of 2,001]`. 738 `c_candidate` regions remain. **Y dropped from 2,058 to 2,001 on
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
| 2026-07-26 | 1,263 | +21 |

**The rate fell by roughly a factor of ten in three days.** That is not a slowdown in
effort, it is the easy tier running out: every region whose natural C happened
to match has been taken. At the last two days' rate 738 regions is roughly 55
working days; at the three-day average, about 22. Neither is a session.

## What is actually left

| count | class | what it needs |
| --- | --- | --- |
| 559 | **plain** — no identified construct blocker | drafting time, and the usual allocation residuals |
| 136 | DMA descriptor, no poll | the grouped-store laws already in `LAWS.md` |
| 36 | `0xffff` used as an AND mask | `u32` locals; 8 of them also need a combine we perform |
| 7 | twelve-store record group | two compiler blockers, one of them unsafe to fix by inspection |

Removed from the table on 2026-07-26, into classes that already described them:
43 `mov ip, pc` call regions and 14 regions taking a hidden register input
(`r9` as a static base in most, `r8`/`sl` in the rest). Screen for both before
drafting — `080e73a0` was picked as a clean 49-instruction target and turned out
to read its base pointer out of `r9`, which no policy-valid C can express.

Of the 559 plain regions, 96 have been attempted and parked with written
root causes. The 463 never touched break down by size:

| instructions | regions |
| --- | --- |
| ≤ 40 | 5 |
| 41–80 | 72 |
| 81–160 | 193 |
| 161–320 | 105 |
| 321+ | 88 |

**106,766 instructions of fresh plain code.** That is the real remaining
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

What did convert (`08005a78`) needed two new compiler options, and was only
attempted because its residual could be hand-verified by reordering the
generated assembly and relinking.

So the honest cost model is not "106,766 instructions of drafting". It is
"drafting is cheap, and the last three to fifteen instructions of each region
are a compiler investigation". Any estimate that prices the remainder at
drafting speed is wrong.

## What changes the rate

1. **The bulk is volume, not blockers.** 559 of 738 have nothing exotic in
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
     it. **Byte-exactness stays.** Roughly 55 working days at the current
     two-day rate is the honest cost.

## Recommended order

Screen every target first: `grep 'mov\s*ip, pc'`, and check for a callee-saved
register read but never written. Both classes are now reclassified out of Y, but
the screen still costs less than a wasted draft.

1. Work the 72 plain regions in the 41–80 band. They are the last tier where a
   single sitting plausibly produces a conversion.
2. Apply the `u32`-locals law to the 25 single-mask regions. Eleven more use the
   mask two to four times and need the corresponding combine; the two small
   enough to have been drafted are both in that repeated-mask group, so the
   reachable set starts at 121 instructions.
3. Only then the compiler families. The transform-ordering conflict behind the
   twelve-store group is the best-specified of them and is written up in
   `LAWS.md`.
