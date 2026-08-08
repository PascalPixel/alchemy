> **C/H hard blocker:** Never use `asm(...)`, `__asm(...)`, `__asm_(...)`, `__asm__(...)`, fixed-register bindings, or empty assembly barriers. Byte equality never overrides this rule.

# Path to completion

Current verified exact **Full-C Byte Share: 265,924 / 1,343,410 executable
bytes (19.79%)**. Exact main-image ownership is 105,052 bytes and exact code-overlay
ownership is 160,872 bytes. After exact-C supersession and executable-extent
accounting, the reviewed semantic tier contributes another **804,102 bytes**;
together they cover **1,070,026 / 1,343,410 bytes (79.65%)**. This combined
figure is reviewed semantic coverage, not the Full-C headline or a completion
forecast; [`../HANDOVER.md`](../HANDOVER.md) is the authoritative current audit.

The primary integration point is `main`. The main-executable reviewed semantic-C
census is closed and enforced by `make semantic-check`; exact-C replacement
remains the principal completion task.

The latest exact promotion is main-image owner `0807a664`: a clean 316-byte C
span compiled byte-for-byte with a source-gated GCC 2.96 post-reload route.
The preceding promotion was `08011590`, a clean 180-byte C span compiled
byte-for-byte with the existing grouped-DMA and scheduler route.
The preceding exact promotions were `080907b0` (116 bytes),
`08098b10` (248 bytes) and `08077f70` (284 bytes); each compiled byte-for-byte
with a source-scoped
`-fno-sched-alias -fsched-store-first -fthumb-call-arg0-before-store` route.
The route is default-off, compiler-regression tested, and contains no assembly
escape hatch.

Everything below this paragraph is a historical planning study from 2026-07-28.
It is retained for the measured compiler and queue evidence, not as a live
counter or scheduling order. Use [`../HANDOVER.md`](../HANDOVER.md) for current
work and `make progress` plus `make coverage-check` for current numbers.

The main-image diagnostic ledger contains 1,369 exact-C regions and 631
`c_candidate` regions. These region counts are useful scheduling diagnostics,
not project progress. **Its denominator dropped from 2,058 to
1,999 on 2026-07-26 through classification cleanup: 43 `mov ip, pc` regions
moved into the existing `nonstandard_thumb_call_module` class, 14 regions that
read a callee-saved register they never write moved into
`hidden_register_context_module`, and two false ordinary-C candidates moved
into existing structural classes. Y then rose to 2,000 on 2026-07-27 when
`080944ec` was corrected in the opposite direction: its predecessor returns
before its own alignment and pool, while two odd-address callback references
prove `080944ec` is an independent ordinary function. The count of *converted*
regions did not move in any of those reclassifications. Y then returned to
1,999 when `080c0be4` was found to consume an incoming `r4` value that ordinary
C cannot name; its standard-ABI sibling consumes the corresponding fourth
argument from `r3`. Y rose to 2,000 again on 2026-07-28 when `080fada0`
compiled exactly with public `old_agbcc`, disproving its earlier
`nonstandard_thumb_branch_module` classification.** This file exists because a
remaining-region headline is a count, not a plan, and because two family sizes published
earlier today were both wrong from lazy fingerprints. Everything below is
measured by `tools/remaining-survey/target/release/remaining-survey`, which decodes each region and resolves
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
| 2026-07-26 | 1,292 | +50 |
| 2026-07-27 | 1,345 | +53 (partial day) |
| 2026-07-28 | 1,369 | +24 (decompilation resumed after humanization) |

**The recent three-day average is still roughly a factor of four below the
2026-07-23 peak.** That is not a slowdown in effort: the broad easy tier is
running out, and compiler lineage now matters as much as drafting. At the last
two completed days' rate, 632 regions is roughly 23 working days; at the
three-day average, about 18. The estimates move materially with one cohort and
neither is a schedule.

## What is actually left

| count | class | what it needs |
| --- | --- | --- |
| 459 | **plain** — no identified construct blocker | drafting time, and the usual allocation residuals |
| 130 | DMA descriptor, no poll | the grouped-store laws already in `LAWS.md` |
| 36 | `0xffff` used as an AND mask | `u32` locals; 8 of them also need a combine we perform |
| 7 | twelve-store record group | two compiler blockers, one of them unsafe to fix by inspection |

Removed from the table on 2026-07-26, into classes that already described them:
43 `mov ip, pc` call regions and 14 regions taking a hidden register input
(`r9` as a static base in most, `r8`/`sl` in the rest). Screen for both before
drafting — `080e73a0` was picked as a clean 49-instruction target and turned out
to read its base pointer out of `r9`, which no policy-valid C can express.

At the `[1,292 of 1,999]` provenance-audit snapshot, 214 of 529 plain regions
had prior target-specific hand, automated, manual,
or substantive permuter work. The other 315 are genuinely untouched. This
split was reconstructed against `fa930c71`: bulk m2c output, match rescoring,
and initialized-but-empty permuter states do not count as attempts; target C,
notes, or a nonempty search state created before that commit do.

| instructions | total | attempted | untouched | untouched instructions |
| --- | ---: | ---: | ---: | ---: |
| ≤ 40 | 21 | 21 | 0 | 0 |
| 41–80 | 98 | 98 | 0 | 0 |
| 81–160 | 213 | 84 | 129 | 14,551 |
| 161–320 | 109 | 11 | 98 | 22,009 |
| 321+ | 88 | 0 | 88 | 56,039 |
| **total** | **529** | **214** | **315** | **92,599** |

The previous 101/442 split, 51 untouched regions in the 41–80 band, and
105,312 untouched-instruction total came from checking an incomplete set of
artifact paths. They were not provenance-authoritative. The corrected table
also changes the immediate strategy: the remaining 41–80 tier is now entirely
rescue work, while almost all untouched volume starts above 80 instructions.
The two nominal untouched entries below 41 instructions were not ordinary
drafts. `0800070c` ends in a custom-context tail transfer to odd-tagged IWRAM
and is now in `custom_iwram_frame_entry_module`. `080b5138` is a relocated ARM
module, not two Thumb instructions; its 560-byte span is now split into 488
bytes of ARM code and 72 bytes of typed literals, workspace, and jump-table
data under `relocated_arm_runtime_module`. Those two corrections explain the
final denominator change from 2,001 to 1,999.

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
finished rarely. `08006088` was later rescued with measured compiler pass
controls; the table records the state of this first pass, not the live queue.

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

## What the rolling twelve-target cohort changed

The next twelve targets were all genuinely fresh members of the
41–80-instruction band. Six converted exactly:

| region | insns | exact route |
| --- | ---: | --- |
| `08007220` | 76 | the established flash-family `old_agbcc -O1` fingerprint |
| `080174f8` | 75 | ordinary C; shared work-pointer control flow |
| `0801cf48` | 76 | ordinary C; split index and adjustment live ranges |
| `0808d8f0` | 80 | ordinary C; typed event dispatch |
| `080b2f4c` | 78 | ordinary C; typed three-state effect update |
| `080b874c` | 76 | ordinary C; switch/default tail reconstruction |

Those six add 1,112 exact-C bytes. The other six are semantics-complete and
remain assembly, with their best candidates and blockers recorded under
`work/hand/<region>/NOTES.md`:

| region | best | residual |
| --- | --- | --- |
| `0800eaf8` | 160/168 bytes, 70 halfwords | global-base materialization and coupled allocation |
| `08078e28` | 164/164 bytes, 35 halfwords | CSE shortens the masked-value lifetime |
| `080a5614` | 180/180 bytes, 2 halfwords | one adjacent initialization transposition |
| `080a602c` | 176/168 bytes, 67 halfwords | allocation rotation plus a mid-function pool |
| `080a7380` | 192/192 bytes, 25 halfwords | low-register coloring and second scheduling |
| `080a77a4` | 168/170 bytes, 41 halfwords | required mid-function pool; the early-pool mode ICEs |

The accounting is therefore six exact conversions and six newly attempted
parks, not twelve conversions. Together they remove 913 instructions from the
cohort's then-current worklist: 461 by conversion and 452 by bounded diagnosis,
while exact claims rise from 1,273 to 1,279. The later repository-wide
provenance audit showed that subtracting those twelve from a 63-region
"fresh queue" did not describe the whole band; the corrected live inventory is
in the table above.

## What the provenance-audit cohort changed

The audit found only four genuinely untouched plain regions in the
41–80-instruction band at `fa930c71`. All four now convert exactly:

| region | insns | bytes | exact route |
| --- | ---: | ---: | --- |
| `080b3398` | 71 | 172 | ordinary C; typed object restore and runtime state |
| `080b2e30` | 73 | 168 | ordinary C; separated item, halfword, and loop cursors |
| `08077c10` | 75 | 168 | ordinary C; advance the record before testing its saved kind |
| `08098c08` | 79 | 196 | source-scoped existing `-fno-gcse` fingerprint |

That adds 704 exact-C bytes and raises the claimed build to
`[1,283 of 2,001]`, with 79,284 claimed bytes and zero claimed-link failures.
It also exhausts genuinely untouched plain work in the band and established
the ranked rescue queue for the following cohort.

## What the rescue cohort changed

The first six ranked medium-small rescues all now convert exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `080a5614` | 180 | strict low-constant-before-high-move compiler fingerprint |
| `08006088` | 96 | existing CSE-rerun and regmove pass controls |
| `080ba918` | 94 | defined terminating-NULL return, the shared low-constant fingerprint, and pass controls |
| `08003adc` | 146 | strict dead-OR-input reuse compiler fingerprint |
| `0801c34c` | 156 | strict entry frame/global/table scheduling fingerprint |
| `0800307c` | 124 | disabled second scheduling plus strict literal-before-index-shift fingerprint |

That adds 796 exact-C bytes, raises the claimed build from 1,283 to 1,289, and
brings exact-C ownership to 80,080 bytes. Independently, the two structural
reclassifications above reduce the denominator from 2,001 to 1,999. That cohort
therefore closed at `[1,289 of 1,999]`, with 710 candidates left.

All four new backend fingerprints are default-off, source-routed, guarded by
exact hard-register and dependency conditions, and covered by opt-in, opt-out,
and unrelated-source fixtures. One low-constant mode closes two regions; the
other three each close one. The rescue band is no longer a pile of unexplained
near-matches, but it is also not automatic: each rule followed a hand-reordered,
relinked proof that the proposed instruction order was exact.

## What the follow-on rescues changed

Three more ranked regions now convert exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `0801e7c0` | 150 | default compiler route; typed text-position and counter source |
| `0808c30c` | 152 | existing `-fno-rerun-cse-after-loop` pass control |
| `080053e8` | 84 | four existing DMA scheduling/grouping modes, in their uniquely minimal combination |

That adds 386 exact-C bytes, raises the claimed build to `[1,292 of 1,999]`,
and brings exact-C ownership to 80,466 bytes. The measured remainder is 707
regions: 529 plain, 135 DMA, 36 mask, and 7 twelve-store regions. Every
nondefault mode is source-routed for GS1 only and self-tested against its
neighboring stem and the GS2 route.

## What the next ranked cohort ruled out

Five other clean drafts were preserved at reproducible floors rather than
counted as conversions:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `080044d0` | 212/212 bytes, 1 differing halfword | allocation chooses `r0` instead of `r1` for one comparison; register pinning or an empty compiler barrier is exact, but neither is accepted as maintainable C |
| `080cdb24` | 154/154 bytes, 2 differing halfwords | `mov lr,r3` and `movs r6,#0` are transposed; the existing high-register mode deliberately covers only `r8`–`r11` |
| `080a2144` | 108/108 bytes, 17 differing halfwords | CSE keeps one DMA source literal live in `r4`; splitting it creates a second pool entry |
| `080f60a0` | 164/168 bytes | reload deletes a repeated `r0 = r4` call-argument move after the grouped DMA store |
| `08099838` | 236/232 bytes, 62 differing halfwords | loop allocation/scheduling and one rematerialized callback constant |

These are bounded results for the current clean sources and supported compiler
modes, not claims that the functions are impossible. The diagnostic exact forms
for `080044d0` are specifically excluded from the numerator: byte identity alone
does not justify hard-register constraints or empty inline assembly when the
project is using `pokeemerald`-style maintainable C as its reconstruction ideal.

## What the 2026-07-27 ranked cohort changed

Three more regions now compile exactly with the ordinary routed GS1 flags:

| region | bytes | exact route |
| --- | ---: | --- |
| `080a76d0` | 212 | typed four-step menu state machine and defined unsigned cancellation test |
| `080984c0` | 232 | typed context/state/scene fields and source-ordered global derivation |
| `0801c7fc` | 164 | typed object-resource scan with a bounded 32-entry inner loop |

The other three assigned regions were stopped at reproducible compiler floors
and are not counted as conversions:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `08092f84` | 188/188 bytes, 7 differing halfwords | one global `r6`/`r7` allocation swap; GCC gives the equally referenced saved key priority over the slightly longer-lived masked index |
| `080b0840` | 84/84 bytes, 15 differing halfwords | reload frees `r0` with the entry parameter save, then uses it for folded offsets; the ROM uses `r2` and delays the save |
| `080949a8` | 288/288 bytes, 25 differing halfwords | keeping the DMA base live across a call preserves the prefix but forces it into `r5`; creating it later gets `r3` and rotates the rest of the allocation |

Each floor survived the relevant full mode/pair sweep and a bounded
source-order or declaration-shape search. None uses a hard-register local or
inline assembly.

That adds 608 exact-C bytes, raises the claimed build to `[1,295 of 1,999]`,
and brings exact-C ownership to 81,074 bytes. The source-only full build owns
all 8,388,608 bytes with zero fallback or unowned bytes. The measured remainder
is 704 regions: 526 plain, 135 DMA, 36 mask, and 7 twelve-store regions.

## What the next nine-region cohort changed

Nine more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `08005394` | 84 | the four existing copied-decompressor scheduling and grouped-DMA modes |
| `08099738` | 216 | default compiler; typed controller, state, object, and global fields |
| `08028f98` | 252 | default compiler; typed menu defaults and explicit result exits |
| `080981b0` | 228 | existing `-fno-gcse` route; preserved random-result lifetimes |
| `08026e80` | 296 | default compiler; typed countdown and link-signature state |
| `080b3284` | 276 | existing `-fno-gcse` route; shared message-base lifetime |
| `0801fba8` | 220 | default compiler; shared lower and upper storage-bank pointers |
| `080b7994` | 280 | default compiler; typed effect-cycle state |
| `08099160` | 400 | default compiler; typed 16-particle orbit and finish sequence |

That adds 2,252 exact-C bytes, raises the claimed build to
`[1,304 of 1,999]`, and brings exact-C ownership to 83,326 bytes. The measured
remainder is 695 regions: 518 plain, 134 DMA, 36 mask, and 7 twelve-store
regions. Eight of the nine use either the default compiler or one existing
pass-control route. `08005394` extends the exact `080053e8` copied-decompressor
family to one more source; all 16 subsets of its four already-supported modes
were measured, and only the complete combination matches. Its route has
GS1-target, neighboring-stem, direct-command, and GS2 opt-out tests.

Three further typed drafts were retained as measured floors rather than counted:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `08095680` | 248/248 bytes, 70 differing halfwords | the reference keeps the global `0x236` offset live and subtracts 66 to reach the selected-object slot; GCC rematerializes `0x1f4`, adding an instruction and shifting the remaining allocation |
| `08016f2c` | 208/216 bytes, 60 differing halfwords | GCC merges two reference dirty-flag address/store paths into a shared tail; that merge is the complete eight-byte size delta, with the remaining reported differences dominated by shifted branches and literals |
| `08096810` | 336/336 bytes, 9 differing halfwords | the complete residual is one global `r6`/`r7` allocation swap between the mode and selected-object state; control flow, calls, stores, pool, and size all agree |

All three floors use natural typed C and survived the relevant routed mode sweeps;
none uses hard-register locals, volatile matching devices, compiler
barriers, or inline assembly.

## What the five-region exact batch changed

Five more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `08005340` | 84 | the four existing copied-decompressor scheduling and grouped-DMA modes |
| `080a3e88` | 102 | default compiler; typed runtime, entry array, and count fields |
| `080ad508` | 172 | default compiler; typed four-object teardown and reconstruction |
| `080b8fd4` | 216 | default compiler; typed state, secondary state, transfer block, and IWRAM calls |
| `080a9aec` | 168 | strict GS1-only three-word Thumb minipool ordering fingerprint |

That adds 742 exact-C bytes, raises the claimed build to
`[1,309 of 1,999]`, and brings exact-C ownership to 84,068 bytes. The measured
remainder is 690 regions: 514 plain, 133 DMA, 36 mask, and 7 twelve-store
regions. `080a3d6c`, which `080a3e88` calls, also now has its recovered argument
and return-pointer ABI instead of an unprototyped m2c declaration; its existing
48-byte claim remains exact.

The `080a9aec` source already reproduced every instruction, register, branch,
and pool position with the stock compiler. Its entire six-halfword residual
was the rotation of three literal-pool words. The new compiler option is
default-off, requires exactly three live four-byte Thumb pool nodes, checks
their recorded reach constraints, and is routed only to GS1 `080a9aec`.
Compiler tests cover default behavior, explicit opt-out, the exact opt-in
order, and an unchanged two-word control pool.

Seven more clean reconstructions were stopped at reproducible floors rather
than counted:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `0800c004` | 194/194 bytes, 10 differing halfwords | the grouped DMA setup still schedules its independent descriptor inputs in a different order |
| `080c0eb8` | 36/36 bytes, 18 differing halfwords | DSE removes the transient `+4` zero store before the twelve-store recognizer can form the reference group |
| `080b7548` | 148/148 bytes, 19 differing halfwords | induction-variable allocation plus an unexplained return scratch value |
| `080f4028` | 140/140 bytes, 14 differing halfwords | first local CSE merges two independently constructed `0x00fa0000` values across indirect calls |
| `0801fd34` | 80/80 bytes, 4 differing halfwords | two independent instruction transpositions remain after disabling second scheduling |
| `0808d394` | 148/148 bytes, 47 differing halfwords | register allocation and scheduling after the four sentinel-list semantics were recovered |
| `0801c188` | 148/148 bytes, 13 differing halfwords | one independent transfer-base/index/table scheduling window; a closer volatile form was rejected as a codegen device |

Each floor uses maintainable C and excludes hard-register locals, inline
assembly, barriers, and volatile matching tricks. The searches included the
relevant complete existing mode/pair sweeps; no non-exact compiler route was
added.

## What the six-region family cohort changed

Six more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `080aac84` | 140 | default compiler; exact palette-transform sibling exposed the original loop and live-range shape |
| `080ad274` | 164 | default compiler; typed four-object teardown/rebuild sibling |
| `080ad5b4` | 64 | default compiler; typed object-position update with defined scalar exit values |
| `080babdc` | 144 | existing source-scoped `-mthumb-immediate-latency` scheduling fingerprint |
| `080b90ac` | 76 | default compiler; natural counted actor-list loop shared with an exact sibling |
| `080a47b4` | 76 | default compiler; typed state/table access with a source-visible repeated value use |

That adds 664 exact-C bytes, raises the claimed build to
`[1,315 of 1,999]`, and brings exact-C ownership to 84,732 bytes. The measured
remainder is 684 regions: 508 plain, 133 DMA, 36 mask, and 7 twelve-store
regions. No denominator class moved.

`080aac84` and `080ad274` are direct structural siblings of already exact
sources, but their behavior was independently recovered from their own ROM
regions. `080babdc` is a new main-ROM witness for an existing default-off
compiler mode: without it, the natural 144-byte source differs only by one
independent two-instruction ordering pair; with it, all 144 bytes match. Its
GS1 source route is covered through the ordinary flags and direct compiler
paths, with neighboring-stem and GS2 opt-out tests.

Eight further investigations produced cleaner measured floors rather than
claims:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `08090824` | 104/104 bytes, exact C | the new narrowly fingerprinted post-reload grouped-DMA rule restores the stack-zero-before-base order; it is routed with `-mgrouped-dma-store -fthumb-group-control-last` |
| `080108c4` | 32/32 bytes, exact C | executable-gap continuation promoted with the existing source-scoped `-fno-cse-two-insn-immediate` and `-fno-sched-depend-count` routes |
| `0800430c` | 76/76 bytes, 3 differing instruction slots | one loop block wants copy/AND before a constant, while enabling second scheduling damages the otherwise exact entry, exit, and pool |
| `080bd7a4` | 52/56 bytes, 28 differing halfwords | volatile grouping preserves all three repeated DMA descriptors, but CSE hoists the shared zero/control constants instead of rematerializing each group |
| `080b1dec` | 140/148 bytes, 51 differing halfwords | the loop offset loses the allocator contest and forces an eight-byte frame; narrowing it restores the reference register family but adds mandatory truncation |
| `080c1ebc` | 148/148 bytes, 30 differing halfwords | an exact fallthrough form relied on undefined non-void return behavior and was rejected; fully defined returns change allocation |
| `08004c1c` | 84/80 bytes, 31 differing halfwords | early DSE removes four overwritten identity lanes before the post-RA twelve-store recognizer can see the reference group |
| `0800fec8` | 140/140 bytes, 2 differing halfwords (2 bytes) | reload uses a temporary for one shift pair instead of coalescing it in place; every other instruction and pool byte agrees |
| `080200cc` | 132/132 bytes, 19 differing halfwords | two existing entry-order modes recover the target lifetimes, but sixteen entry scheduling slots and one r9 copy register still differ |

The `080bd7a4` compiler experiment was isolated from the live compiler fork.
Its repeated, mixed, and gapped volatile-store tests pass, but no compiler
change is admitted here because the experiment did not produce an exact
region.

## What the nine-region 172–184-byte cohort changed

Nine more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `08016670` | 172 | default compiler; typed slot selection and state initialization |
| `08017dd4` | 180 | default compiler; signed decimal formatting and width trimming |
| `0807a498` | 182 | default compiler; typed ownership transfer and bitset updates |
| `08094730` | 180 | existing grouped-DMA and no-depend-count fingerprints |
| `08095160` | 180 | existing grouped-DMA fingerprint |
| `08095290` | 184 | grouped DMA plus the new strict high-move-before-stack-store fingerprint |
| `080a21b0` | 184 | default compiler; page-count and label rendering |
| `080bf5a8` | 178 | default compiler; timed placement cleanup |
| `080d40ec` | 184 | existing `-fno-gcse`; typed relocated word-copy call |

That adds 1,624 exact-C bytes, raises the claimed build to
`[1,324 of 2,000]`, and brings exact-C ownership to 86,356 bytes. The measured
remainder is 676 regions: 502 plain, 131 DMA, 36 mask, and 7 twelve-store
regions. The denominator rose by one because callback references and a clean
predecessor return/pool boundary prove that `080944ec` is an independent
88-byte function, not an executable-gap continuation.

Six of the nine use either the default compiler or one ordinary pass control.
`08094730` and `08095160` transfer already-proven grouped-DMA behavior.
`08095290` supplied one new narrow compiler witness: sched2 had swapped a
stack-zero store with the saved-high-register copy of the same zero. The new
default-off mode recognizes the complete four-instruction, hard-register,
stack-address, independence, and death-note fingerprint; it is routed only to
that GS1 source and has neighboring-source, direct-compiler, and GS2 exclusion
tests. The compiler fork's fixture also proves stock behavior and a non-stack
control are unchanged.

Five further reconstructions reached measured floors rather than claims:

| region | best measured shape | remaining blocker |
| --- | --- | --- |
| `080ad35c` | 176/176 bytes, 25 differing halfwords | allocation and scheduling after the typed reconstruction |
| `08097f80` | 172/172 bytes, 2 differing halfwords | one independent instruction reorder |
| `08020198` | 168/172 bytes, 22 differing halfwords | allocation and lowering leave a four-byte size deficit |
| `0801a4fc` | 166/166 bytes, 2 differing halfwords (4 bytes) | adjacent DMA-source move and control-load order after grouped DMA plus value2-in-place |
| `0809ba90` | 164/164 bytes, 11 differing halfwords | two allocation/order windows after grouped DMA plus control-last; semantics, control flow, and literals agree |
| `080944ec` | 88/88 bytes, 18 differing halfwords | the recovered callback is exact-size under grouped DMA plus `-fno-schedule-insns2`; DMA-base allocation rotates r3/r4 and one independent entry pair remains transposed |

These floors reinforce the current queue change: fresh medium regions are still
producing exact default-compiler wins, while the short queue consists almost
entirely of known compiler-residual rescues.

## What the four-region typed batch changed

Four more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `0800b7c0` | 168 | default compiler; typed four-object metadata refresh |
| `08017004` | 192 | default compiler; fixed-point window interpolation through the RAM scale callback |
| `0809b11c` | 236 | default compiler; typed three-state effect update |
| `080fa6a0` | 248 | independently proven stock `old_agbcc -O2` audio-initializer fingerprint |

That adds 844 exact-C bytes, raises the claimed build to
`[1,328 of 1,999]`, and brings exact-C ownership to 87,200 bytes. The measured
remainder is 671 regions: 497 plain, 131 DMA, 36 mask, and 7 twelve-store
regions. Ordinary assembly debt is 404,300 bytes.

The denominator fell by one independently of those conversions. `080c0be4`
reads incoming `r4` before defining it, while the same-sized sibling
`080c0cec` receives its fourth argument conventionally in `r3`. The
`-fcall-used-r4` project ABI permits clobbering `r4`; it does not turn `r4`
into a fifth argument register. `080c0be4` therefore moved to the existing
`hidden_register_context_module` structural class rather than being credited
as C.

`080fa6a0` is the first delivered initializer in the already-proven stock
audio-library compiler cohort. Its `0xfb0`-byte state layout is independently
accounted for as a `0x50`-byte header, twelve `0x40`-byte channel records, and
two `0x630`-byte PCM buffers. The source uses generic, locally evidenced
callback slots; no names or types were imported from another game project.
Because the repository invokes `old_agbcc` directly, this source is
preprocessor-free and carries its own fixed-width typedefs.

One further 228-byte reconstruction, `08002e00`, is semantics-complete and
exact-size but remains assembly. Its best bounded result is 17 differing
halfwords with grouped DMA, early frame allocation, and one-cycle Thumb load
latency; four coupled post-reload scheduling windows remain after 696 targeted
existing-mode/source-shape probes.

`080c0cec` is likewise semantics-complete and exact-size at 264 bytes, but its
best default-route form remains 40 differing halfwords. The residual is one
connected entry allocation cascade: preserving the reference's in-place fourth
argument shift prevents the recovered quotient and callback from taking their
reference saved registers. Its tail and literal pool are already exact, and a
74-mode sweep found no supported route that improves the floor.

## What the seventeen-region exact batch changed

Seventeen more regions now compile byte-for-byte exactly:

| region | bytes | exact route |
| --- | ---: | --- |
| `08003d28` | 196 | default compiler; typed affine-matrix allocation and fixed-address signed division |
| `08012204` | 168 | default compiler; typed two-layer collision-map sampling |
| `0801b4ec` | 212 | default compiler; typed menu-selection and scroll-state update |
| `0801d014` | 220 | existing grouped-DMA fingerprint; typed settings and five-value initialization |
| `080209d0` | 144 | default compiler; typed window tile copy into VRAM and its mirror |
| `0808bde0` | 224 | default compiler; typed bounded-region selection and fixed-point tests |
| `08096048` | 248 | default compiler; typed three-state effect initialization and update |
| `08097f80` | 172 | default compiler; source-ordered four-state effect update |
| `08099678` | 192 | default compiler; typed object, map-layer, and collision-tile lookup |
| `080a56c8` | 188 | default compiler; typed paged menu renderer plus its four-byte helper |
| `080a9e48` | 200 | default compiler; typed object-effect application plus its four-byte helper |
| `080ad35c` | 176 | default compiler; typed four-object fixed-point motion setup |
| `080b82c4` | 208 | default compiler; typed two-object interpolation and motion setup |
| `080b8b48` | 212 | default compiler; typed caller state and object-pair orchestration |
| `080d6888` | 216 | default compiler; typed group/object update traversal |
| `080f7e60` | 208 | default compiler; typed circular-slot input and cleanup loops |
| `080fa55c` | 280 | independently proven stock `old_agbcc -O2` CGB-audio initializer fingerprint |

That adds 3,464 exact-C bytes, raises the claimed build to
`[1,345 of 1,999]`, and brings exact-C ownership to 90,664 bytes. The measured
remainder is 654 regions: 481 plain, 130 DMA, 36 mask, and 7 twelve-store
regions. The source-only full build still owns all 8,388,608 bytes with zero
fallback or unowned bytes; ordinary assembly debt is now 400,836 bytes.

Two earlier compiler floors, `08097f80` and `080ad35c`, closed through better
maintainable source shapes rather than register pinning, fake volatility, or
inline assembly. `08003d28` was independently rebuilt through the approved
compiler, assembler, linker, and objcopy and matched all 196 bytes; its audit
also checks the complete region boundary, twelve callers plus dispatch, the
eight-byte request/matrix layouts, and defined shift arithmetic.

The batch also tightened interfaces that cross region boundaries:
`080122ac` now passes the recovered position pointer to `08012204`;
`080a32b8` uses the actual three-word ABI of `080a9e48`; `080b8b48` and
`080b82c4` agree on their four scalar arguments; and `080fa2a0` passes the
typed CGB-channel pointer to `080fa55c`. The claimed-build symbol gate caught
and rejected an initially misnamed trailing helper before integration; its
verified address is `080a5780`.

## The first post-humanization family

The first fresh 81–160-instruction pass converted the two-member font-transfer
family:

| region | instructions | bytes | exact route |
| --- | ---: | ---: | --- |
| `08019fcc` | 81 | 188 | default compiler; unsigned glyph bound and slot initialized before allocation |
| `0801a404` | 81 | 188 | default compiler; sibling table and bound function, slot initialized after allocation |

Both use the same recovered 0x608-byte transfer layout and the same allocation,
optional base-glyph upload, indexed glyph upload, source allocation/reuse, and
final transfer sequence. The placement of the `slot = 0` assignment is the
real compiler-shape distinction: moving it across `Func_080048b0` reproduces
the two original saved-register allocations without a compiler flag.

This raises the claimed build to `[1,347 of 1,999]`, exact-C ownership to
91,040 bytes, and lowers ordinary assembly debt to 652 regions / 400,460 bytes.
The full ROM remains byte-identical with zero fallback or unowned bytes.

## The second fresh 81–160 pass

Two more plain regions converted with the default compiler:

| region | instructions | bytes | exact route |
| --- | ---: | ---: | --- |
| `0801fe2c` | 108 | 232 | typed descriptor-driven object placement and callback scheduling |
| `080b1e80` | 86 | 204 | typed battle runtime, effect flags, and selection orchestration |

`0801fe2c` recovered the four-slot object, coordinate, and fixed-point scale
layout. `080b1e80` recovered the runtime object pointer, saved coordinates,
mode byte, and effect flag contract. Neither needs a compiler mode, volatile
fiction, register pin, or inline assembly.

Three other screened regions were not promoted: `080286a0` remains an
exact-size three-halfword scheduling wall; `080b1f4c` is semantics-complete but
GCC collapses its five message-selection branches; and `0808b090` did not
preserve the reference's long-lived register and table-walk shape. Their drafts
were kept out of `src`.

The current claimed build is `[1,349 of 1,999]` and 91,476 exact-C bytes.
Ordinary assembly debt is 650 regions / 400,024 bytes.

## The third fresh 81–160 pass

`08097b70` converted as 204 bytes of default-compiler C. Its recovered source
tracks a target with a clamped angular turn, randomizes a spawn position, then
creates and configures an effect object. The exact result uses typed source,
position, and object layouts without compiler flags, register pins, or inline
assembly.

`080a6a98` was screened but not promoted. Its semantic reconstruction reached
the exact 204-byte extent with 12 differing halfwords, confined to register
allocation around a halfword-table lookup and loop-constant initialization.
The draft remains outside `src` as a bounded compiler-shape lead.

The current claimed build is `[1,350 of 1,999]` and 91,680 exact-C bytes.
Ordinary assembly debt is 649 regions / 399,820 bytes.

## The fourth fresh 81–160 pass

`080903bc` converted as 204 bytes of exact C. It advances a signed interpolation
step, updates the current display value through the relocated signed-division
helper, and writes the two display bounds. The recovered lower bound is
`159 - value`; using 160 was a one-halfword semantic error, not a scheduling
artifact. A full-width signed delta preserves the reference's signed loads and
multiplication allocation.

The existing source-scoped `-fno-sched-depend-count` mode closes the remaining
independent instruction reorder. No new compiler transform was added.
`08097540` was also reconstructed to an exact 200-byte extent and parked at a
two-halfword entry-scheduler floor; its desired ordering is unique among the
screened siblings, so broadening a compiler rule would be harmful.

The current claimed build is `[1,351 of 1,999]` and 91,884 exact-C bytes.
Ordinary assembly debt is 648 regions / 399,616 bytes.

## The fifth fresh 81–160 pass

Two default-compiler regions converted:

| region | bytes | recovered responsibility |
| --- | ---: | --- |
| `0801ec6c` | 212 | choose a side and object ID, construct an object, then record its ID and generated value |
| `080955b0` | 208 | resolve a menu selection, dispatch its page handler, clear the slot, and finalize the UI |

The last four halfwords in `0801ec6c` exposed real reconstruction errors rather
than compiler noise: the fifth and sixth arguments to `Func_0801a4fc` were
reversed, and the state arrays at `0x12ec` and `0x12f0` had opposite roles.
Correcting both produced exact code and a more accurate ABI.

`0808f1c0` was parked at an exact-size two-halfword scheduling floor after its
packed child bitfields were recovered. Its remaining difference is only the
order of an independent byte load and immediate argument setup; existing modes
and maintainable source reorderings do not close it.

The current claimed build is `[1,353 of 1,999]` and 92,304 exact-C bytes.
Ordinary assembly debt is 646 regions / 399,196 bytes.

## The sixth fresh 81–160 pass

`080bb588` converted as 212 bytes of default-compiler C. It clears four actor
state bytes and an adjacent 24-byte range, refreshes the actor state, resolves
the related value, and tail-returns the final state update. The explicit byte
assignments reproduce the original unrolled output without compiler flags or
assembly.

Five other medium regions were screened without promotion. `08078708` reached
an exact 212-byte extent with nine register-only halfword differences, and
`080b386c` reached 23 differences confined to one three-register permutation.
`08021cb8`, `080f61e8`, and `080b5d3c` remained broader allocation or loop
lowering mismatches. Their assembly ownership remains unchanged.

The current claimed build is `[1,354 of 1,999]` and 92,516 exact-C bytes.
Ordinary assembly debt is 645 regions / 398,984 bytes.

## The seventh fresh 81–160 pass

`08028aa8` converted as 216 bytes of default-compiler C. It detects a menu-mode
change, clears the affected window region, and draws two, three, or five
consecutive text entries. Recovering the original signed decision tree and
keeping each branch's text cursor local eliminated repeated literal loads and
matched the ROM without a compiler mode.

`08016f2c` and `080be0b4` were reconstructed but remained broad compiler-shape
mismatches, so neither was promoted.

The current claimed build is `[1,355 of 1,999]` and 92,732 exact-C bytes.
Ordinary assembly debt is 644 regions / 398,768 bytes.

## The eighth fresh 81–160 pass

`080b153c` converted as 216 bytes of default-compiler C. It resolves an actor's
held-item quantity, derives and caps an effect chance, selects battle action 12,
positions the UI, and dispatches the resulting item action. Typed actor-slot,
effect, battle, and global-state layouts reproduce the ROM without compiler
flags or assembly.

`08094da0` and `080f0538` were semantically reconstructed but remained broad
allocation, DMA, and scheduling mismatches, so their assembly remains.

The current claimed build is `[1,356 of 1,999]` and 92,948 exact-C bytes.
Ordinary assembly debt is 643 regions / 398,552 bytes.

## The ninth fresh 81–160 pass

`08092624` and `080a6794` converted as 226 and 224 bytes of
default-compiler C. The former creates and initializes a randomized battle
effect object; explicit bitfields recovered its packed flag transfer. The
latter builds a two-row menu object array; reusing the original live ranges
recovered the high-register allocation. Neither needs compiler flags or
assembly.

`0800b6b8` was semantically reconstructed but remained a broad allocation and
search-loop mismatch, so its assembly remains.

The current claimed build is `[1,358 of 1,999]` and 93,398 exact-C bytes.
Ordinary assembly debt is 641 regions / 398,102 bytes.

## The tenth fresh 81–160 pass

`080119cc` converted as 184 bytes of default-compiler C. It interprets the
blend script at runtime state offset `0xd8`, including reset and indexed-jump
commands, blend-register writes, and duration countdown. Its typed state
reconstruction matches the ROM without compiler flags or assembly.

`080120dc` was semantically reconstructed but remained a coherent
register-allocation and evaluation-order mismatch, so its assembly remains.
The adjacent `080200cc` family rescue is exact-sized and parked at 21 differing
halfwords rather than being promoted approximately.

The current claimed build is `[1,359 of 1,999]` and 93,582 exact-C bytes.
Ordinary assembly debt is 640 regions / 397,918 bytes.

## The eleventh fresh 81–160 pass

`08091c7c` converted as 264 bytes of default-compiler C. It waits for the
runtime transition gates, derives the longer of two timing spans, starts the
outcome transition, and dispatches the corresponding success or failure object
state. Typed runtime and timing-entry layouts reproduce the ROM without
compiler flags or assembly.

`080a68ec` was semantically reconstructed but remained a broad register
allocation mismatch, so its assembly remains. A preliminary sibling-ranked
small-target pass was also rejected: it disproportionately selected known DMA,
matrix-store, hidden-ABI, and scheduler-only families.

The current claimed build is `[1,360 of 1,999]` and 93,846 exact-C bytes.
Ordinary assembly debt is 639 regions / 397,654 bytes.

## The twelfth fresh 81–160 pass

`080931ec` converted as 280 bytes of default-compiler C. It creates two effect
handles, waits for the shared transition gates, releases both effect IDs, and
then waits for each handle to finish. Preserving the original nested argument
evaluation and treating the secondary setup calls as side effects recovered
the ROM without compiler flags or assembly.

`08092f84` reached exact size with a seven-halfword `r6`/`r7` allocation
rotation, and `08093874` remained a broad allocator mismatch. Their assembly
therefore remains authoritative.

The current claimed build is `[1,361 of 1,999]` and 94,126 exact-C bytes.
Ordinary assembly debt is 638 regions / 397,374 bytes.

## The thirteenth fresh 81–160 pass

`0801b424` converted as 200 bytes of default-compiler C. It waits for the menu
state to settle, dispatches left/right input, reports the selected result, and
loops until one of the accepted completion conditions occurs. Keeping the
memory-mapped input word volatile preserves the two source reads across the
ordered `if` / `else if` without a compiler-specific route.

`080286a0` was recovered through an exact instruction prefix ending at offset
`0x74`; its closest maintainable draft is 184/188 bytes. GCC coalesces the
absolute-distance temporary that the reference keeps in a distinct register.
The useful draft is retained under the ignored candidate workspace, while its
assembly remains authoritative.

The current claimed build is `[1,362 of 1,999]` and 94,326 exact-C bytes.
Ordinary assembly debt is 637 regions / 397,174 bytes.

## The fourteenth fresh 81–160 pass

`0801bcd4` converted as 196 bytes of default-compiler C. It recovers the
menu-mode dispatcher through the same nearby callback family while preserving
the reference's physical switch-arm order. It needs no compiler mode or
assembly.

Three other complete semantic reconstructions remain measured floors:
`08016178` is exact-sized and improves from 74 to 23 differing halfwords under
`-fno-gcse`; `080113e4` remains exact-sized at 63 halfwords; and `08022768`
remains exact-sized at 66 halfwords after its best bounded mode. Those are
broad allocation/control-flow residuals rather than evidence for triple-mode
search, so their assembly remains authoritative.

The current claimed build is `[1,363 of 1,999]` and 94,522 exact-C bytes.
Ordinary assembly debt is 636 regions / 396,978 bytes.

## The fifteenth fresh 81–160 pass

`080fac44` converted as 180 bytes of typed C through the already-approved
historical `old_agbcc` m4a compiler family. Its implementation was adapted from
the approved pret/pokeemerald `src/m4a.c:TrkVolPitSet` at commit
`83df84e40623b79281f2397faa611cbf044170bd`, then checked against this ROM
region and exact compiler output. It updates the track's derived left/right
volume and pitch fields from its modulation, pan, bend, tuning, and shift
state, then clears the handled dirty flags.

The compiler allowlist now has 39 independently exact witnesses. `080fac44`
needs no new backend mode or per-function flag: it extends the existing stock
m4a translation-unit family.

The current claimed build is `[1,364 of 1,999]` and 94,702 exact-C bytes.
Ordinary assembly debt is 635 regions / 396,798 bytes.

## The sixteenth fresh pass

Two more stock-m4a regions converted through the already-approved historical
`old_agbcc` translation-unit family. Their implementations were adapted from
the approved pret/pokeemerald `src/m4a.c` functions `FadeOutBody` and
`CgbOscOff`, respectively, at commit
`83df84e40623b79281f2397faa611cbf044170bd`, then checked against the GS1 ROM
regions and exact compiler output:

| region | bytes | recovered behavior |
| --- | ---: | --- |
| `080fab7c` | 200 | fade interval/countdown, track stop, completion status, and per-track volume scaling |
| `080fada0` | 80 | channel-select switch that disables the corresponding GBA sound control registers |

Both are maintainable typed C, compile without assembly or a new backend mode,
and independently match every byte. The old-agbcc allowlist now has 41 exact
witnesses; these two reduce one-off routing by extending an existing coherent
library family.

`080fada0` also corrects the denominator upward by one. It had been classified
as `nonstandard_thumb_branch_module` because the Camelot fork did not reproduce
its bare `bx lr` leaf. Its exact public `old_agbcc` reconstruction proves that
the prologue/return is ordinary compiler output, not structural assembly. The
conversion count rises by two while the ordinary denominator rises by one.

The current claimed build is `[1,366 of 2,000]` and 94,982 exact-C bytes.
Ordinary assembly debt is 634 regions / 396,598 bytes.

## The seventeenth fresh pass

`0801ffd8` converted as 176 bytes of default-compiler C. It is a sibling of the
existing `0801fe2c` placement routine: it creates four table-selected objects,
records them in the shared placement state, derives their X/Y coordinates from
the supplied origin, and schedules the follow-up callback.

The reconstruction was exact on its first complete source shape apart from one
real flag-mask error. Correcting `~0x0d` to the ROM's `~0x0c` produced
176/176 bytes and zero differing halfwords; the integration gate then accepted
the region without a compiler mode. A separate fresh `080bad7c` reconstruction
was semantics-complete and exact-sized but remained a broad register-allocation
floor, so it was not promoted.

The current claimed build is `[1,367 of 2,000]` and 95,158 exact-C bytes.
Ordinary assembly debt is 633 regions / 396,422 bytes.

## The eighteenth fresh pass

`080c1a34` converted as 200 bytes of default-compiler C. It selects a 16-byte
record, confirms that one of its five entries is present, resolves the enabled
member objects, rejects disallowed members, and returns the average of the
contributing values.

The exact source preserves two non-obvious shapes: the first presence scan has
an explicit post-increment boundary break followed by a separate `index == 5`
test, and the five-entry processing loop uses an unsigned bound. The reference
also reserves a 28-byte local workspace whose contents are unused; a volatile
byte array preserves that evidenced frame without inline assembly, attributes,
or register pinning. The result is 200/200 bytes, zero differing halfwords, and
an integration-gate acceptance with no compiler mode.

Five other fresh family-ranked regions were reconstructed during the same pass
and parked after bounded checks. `08003538` was the strongest at 280/280 bytes
and 17 differing halfwords; all 60 routed single configurations tied at that
floor. The others remained broader register-allocation or induction-variable
floors, so none was manually permuted or promoted approximately.

The current claimed build is `[1,368 of 2,000]` and 95,358 exact-C bytes.
Ordinary assembly debt is 632 regions / 396,222 bytes.

## The nineteenth fresh pass

`080ba584` converted as 296 bytes of default-compiler C. It resolves a primary
battle object, derives and stores its facing angle, builds the shared 84-byte
battle work record, starts motion toward a secondary object, and selects either
the scripted presentation sequence or the ordinary battle update path.

The exact source uses the layouts already evidenced by the installed
`080b8b48`, `080b9d34`, and motion-object family. Three source details preserve
the original allocation without a compiler mode: the second coordinate is
materialized before the first, the raw/adjusted/final facing values remain
separate, and the low-ID classification is an explicit `if`/`else`. The result
is 296/296 bytes, zero differing halfwords, and an integration-gate acceptance.

The same untouched cohort reconstructed `080d0468` and `080dc1ec`
semantically, but both hit broad register-coloring cascades across nested
loops. They remain ignored scratch rather than approximate C claims.

The current main claimed build contains 95,654 compiler-linked bytes, of which
95,542 qualify as canonical Full-C after excluding two legacy inline-assembly
fakematches. The overlays contribute another 18,984 Full-C bytes. Ordinary
assembly debt is 631 regions / 395,926 bytes; both are diagnostics.

The canonical headline, including audited main and overlay executable spans,
is **114,526 / 1,338,242 executable bytes (8.56%)**.

## Bounded compiler-configuration explorer

`tools/mode-sweep` now treats compiler search as evidence collection rather
than source promotion. It runs the routed GCC 2.96/Thumb configuration and
historically meaningful single changes first, then compatible pairs on request.
Triples are considered only when a pair improves every single-mode result and
leaves an exact-sized two-to-five-halfword floor.

Successful and failed compilations are content-addressed under ignored
`out/modesweep/`, and each run writes a classified report plus a floor record.
The classifier separates exact identity, size, instruction-order, register,
literal-placement, and control-flow evidence. A truncated run records only its
best observed score; it cannot claim an irreducible floor. An exhausted
two-to-five-halfword search points to RTL/scheduler tracing and never promotes
the numerically closest result automatically.

The first full single/pair validation used the existing exact-sized
`0800430c` floor. All 251 routed, single, and compatible-pair configurations
compiled; none improved its three-halfword instruction-order residual. A
second identical run reused all 251 cached results, and the completed floor
correctly escalates to compiler RTL/scheduler tracing.

The exact-sized `08097540` floor was then traced through post-reload scheduling.
After loading `166`, its shift and the independent first-argument save have
equal priority. GCC's ready-list tie break selects the save first; the ROM
selects the shift first. Reversing that tie break globally would damage five
measured inverse-order functions, so this is recorded as a compiler floor
rather than routed through a one-function backend flag.

`tools/mode-cohort/target/release/mode-cohort` now applies the same cached phased explorer to a group of
semantically verified candidates and reports only configurations that make
more than one member exact. Its first bounded cohort contained six strong
near-matches; it found zero shared exact configurations, so compiler work
stopped immediately and effort returned to fresh decompilation.

## What changes the rate

1. **The bulk is volume, not blockers.** 465 of 638 have nothing exotic in
   them. They are not converting because each one is a hand-written function
   that has to match byte-for-byte, and the median is now 81–160 instructions
   rather than the 20–40 that carried the early rate.

2. **New compiler transforms are expensive, but a proven shared fingerprint can
   pay twice.** This rescue pass added four narrow post-reload modes and closed
   five regions with them; existing pass controls closed the sixth. The fork's
   history still records seven modes proposed by inspection that made regions
   worse. The discipline remains: test family/compiler fingerprints first; for
   a new transform, reorder the generated assembly by hand, relink, and confirm
   exactness *before* writing anything.

3. **Two classification and policy decisions were worth more than a week of
   grinding**, and both are now settled:

   - ~~**The 43 `mov ip, pc` regions.**~~ **Settled 2026-07-26.** They were
     never a policy question: `nonstandard_thumb_call_module` already existed,
     with `manual_return_address_preserved_in_ip` among its stated evidence and
     21 files listed. All 43 carry exactly that construct — `mov ip, pc`
     followed by an indirect branch, the interworking call that keeps the return
     address in `ip` where both approved compilers emit `bl _call_via_rN`.
     Nobody had added them to the group's file list, so they sat in the default
     `compiler_output` bucket. Moving them applies the project's own criteria
     rather than relaxing them. At that step, Y became 2,015.

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
     it. **Byte-exactness stays.** Roughly 27 working days at the current
     two-day rate is the honest cost.

## Recommended order

**2026-07-30 semantic-speed correction.** The recommendations below describe
byte-exact adoption. Measured consecutive cohorts also established a
higher-throughput semantic-C route:

1. Work on complete owners, with exact assembly as authority and m2c only as a
   hint.
2. Drain established callback and construct families before unknown thunks.
   Typed renderer, transfer, fill, scale, square-root, and initializer ABIs
   amortize across unrelated owners.
3. Then run assembly-led direct-call owners. A three-owner, no-thunk trial
   converted all 5,876 bytes, showing the method is broader than renderer
   families.
4. Treat split manifest regions as possible single functions. Follow live frame
   state through continuations and account complete executable ranges in
   `semantic/main-regions.json`; never count a head-only C file.
   Resolve and size the transitive continuation graph before assigning a
   `split_first` or `merge_with_continuations` target. `080e47b8` demonstrates
   why: its 768-byte queue row is only a dispatcher for a 16-row owner whose
   live 184-byte frame reaches the sole epilogue at `080e660a`. The complete
   span is 7,762 bytes including embedded data and contains 231 static calls,
   more than ten times the work implied by the queue.
5. Verify each coherent cohort with `make verify`. Semantic C does not weaken
   the exact standard: exact builds must remain byte-identical and source-only
   ownership must remain complete.

The exact-C order follows:

Screen every target first: `grep 'mov\s*ip, pc'`, and check for a callee-saved
register read but never written. Both classes are now reclassified out of Y, but
the screen still costs less than a wasted draft.

1. Continue the fresh 81–160 and 161–320 instruction queues, ranked by
   structural siblings and family fingerprints. All remaining plain regions
   below 81 instructions have prior work, so that tier is now rescue work
   rather than the primary conversion queue.
2. Apply the `u32`-locals law to the 25 single-mask regions. Eleven more use the
   mask two to four times and need the corresponding combine; the two small
   enough to have been drafted are both in that repeated-mask group, so the
   reachable set starts at 121 instructions.
3. Only then the compiler families. The transform-ordering conflict behind the
   twelve-store group is the best-specified of them and is written up in
   `LAWS.md`.
