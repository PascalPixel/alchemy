# Owners that need compiler behaviour we do not have

100 main-image owners live in `semantic/` rather than `exact/`. They stopped
matching when the compiler axe deleted 120 invented options, and 12 have since
been reconstructed in C. They are not parked or quarantined -- they are
ordinary not-yet-matching C, in the folder all not-yet-matching C lives in,
with their assembly in `asm/` building the ROM meanwhile.

This file records what blocks them, because three separate bulk attempts have
now failed and the reasons are specific rather than "needs more effort".

## The split

| blocker | owners | bytes | reachable in C? |
|---|---|---|---|
| Thumb store-multiple | 49 | 4,600 | **no** |
| register allocation | 29 | — | no, on current evidence |
| literal pool placement | 13 | — | no, on current evidence |
| instruction scheduling | 8 | — | **no** |
| other | 1 | — | unknown |

## Thumb store-multiple -- proven unreachable

The reference emits a bare `stmia rB!, {r0, r1, r2}` + `subs rB, #12`. Stock
gcc 2.96 has exactly one Thumb `stmia` emitter, `thumb_output_move_mem_multiple`,
and it *always* pairs the store with an `ldmia` -- it is the block-move path.
The store-combining peepholes that produce a bare STM exist in the same file,
gated `TARGET_ARM`.

Checked across the whole upstream window 1999-2002: **no snapshot gates
store-multiple for Thumb**. No C shape reaches this instruction. See
[THUMB-STORE-MULTIPLE.md](THUMB-STORE-MULTIPLE.md) for the implementation that
does enable it, why it is not admitted, and the two conditions for revisiting.

## Register allocation -- ruled out at the macro level

`arm.h` sets `REG_ALLOC_ORDER` to `3, 2, 1, 0` (descending) while the reference
frequently wants ascending registers, which makes flipping it the obvious fix.
It was measured over the whole main image:

    stock   3,2,1,0    1107 exact
    flipped 0,1,2,3     258 exact     gained 0, lost 849

The descending order is load-bearing for four fifths of everything that
matches. The residue is not explained by allocation *order*.

## Instruction scheduling -- worked example

`080043e0` is the closest parked owner, `differing_halfwords=2` out of 64
bytes. The entire difference is placement:

    candidate            reference
    movs r0, #1          movs r0, #1
    negs r0, r0          ldr  r4, [pc, #48]
    ldr  r4, [pc, #48]   negs r0, r0

The reference's scheduler slots an independent pool load between the two halves
of the `-1` materialisation; ours does not. Seven C variants were tried --
declaration reordering (four ways), hoisting the loop index, and three spellings
of `-1` (`-1`, `~0`, `0xFFFFFFFF`). **Every one produced exactly
`differing_halfwords=2`.** The residual is invariant to C shape, which is what
identifies it as a scheduler decision rather than a source problem.

## All three workable owners tested by hand, same answer

The first workflow left eleven owners reverted. One (`0800be70`) was later
recovered; seven are store-multiple, above. The remaining **three were each
worked by hand**, and every one is invariant to C shape:

| owner | residual | variants tried | best result |
|---|---|---|---|
| `080043e0` | 2 of 64 bytes | 7 -- four declaration orderings, index hoisting, three spellings of `-1` | **always exactly 2** |
| `080b6a60` | 5 of 128 bytes | 2 -- decrement into the `while` condition, decrement moved earlier | 5, or 14 (worse) |
| `080038bc` | 11 of 64 bytes | 2 -- constant referenced in the other order, inner scope removed | **always exactly 11** |

In every case the candidate and reference contain the *same instructions* in a
different order, and `080038bc` additionally swaps two literal-pool words.
Nothing expressible in C moved any of them. Three owners, eleven variants, zero
change in residual: that is what identifies this class as scheduling rather than
source, and it is why a fourth reconstruction pass is not the answer.

## What would actually move these

Not another reconstruction pass -- two have run, recovering 4 owners against 42
failures citing register allocation or pool placement. The open leads are
compiler-side and each needs its own measured experiment:

1. `config/arm/elf.h` emits `.align N, 0` instead of `.align N`. Inherited from
   the original import, changes emitted padding bytes, and **never measured**.
   See `upstream-deviations.txt`.
2. The `08005258` counterexample in THUMB-STORE-MULTIPLE.md, explained rather
   than accommodated.
3. Whether Camelot's scheduler differed -- the `080043e0` case above is a clean,
   2-byte, fully-characterised witness to test any such hypothesis against.

## 080bbb0c, measured 2026-08-16

The largest single owner, 6,332 bytes, and the clearest picture of what the
remaining distance actually is.

    candidate=6324  reference=6332  differing_halfwords=2286   (73% of rows)

**The C is substantially right.** Instruction multiset overlap is **94%** --
the same instructions, in different places. The first 258 bytes are
byte-identical, then it diverges and does not resynchronise.

**The divergence is adjacent-instruction reordering**, not wrong code. It
re-syncs after every swap:

    ! 0202  mov  r0, r8          adds r1, r5, #0
    ! 0204  adds r1, r5, #0      ands r2, r6
    ! 0206  ands r2, r6          mov  r0, r8
    ! 0218  movs r4, #0          mov  ip, r1
    ! 021c  mov  ip, r1          movs r4, #0

Identical instructions, permuted, roughly 1,140 times over. That is why 73%
looks catastrophic while the real disagreement is much smaller.

**No stock flag helps.** All 18 stock options and all 153 pairs were tried:
171 trials, and not one improves on the no-flag baseline of 2286. The
scheduler flags (`-fno-schedule-insns`, `-fno-schedule-insns2`) change the
emitted code substantially -- thousands of lines -- without getting closer, so
the ordering is coming out of RTL generation rather than the scheduler. That
makes it a C-shape question in principle, and one with ~1,140 sites.

**The 8-byte size gap is distributed, not at the tail.** The trailing literal
pool matches almost exactly (one word differs), so the missing 8 bytes are
spread through the function's interleaved pools. Reference emits 70 more `ldr`
against our extra `movs`/`lsls`: it reloads where we keep values in registers,
which is a register-pressure difference.

**The permuter is not yet allowed at this owner.** CONTRIBUTING's gate requires
reference size reached and the residual localized into explained clusters.
Neither holds: the size is 8 bytes short and the residual is diffuse across the
whole function.

**Current state: `1961 / 6332`, size EXACT.** Down from 2286 / 6324.

**The working axis: un-cache field reads.** The `+70 ldr` signal was real. Our C
caches field reads in locals where the reference RE-READS them. Inlining the
right ones moves the score, and inlining the right ones *while holding size at
6332* moves both at once.

The reconciliation matters more than the score. Optimising differing-halfwords
alone drifts the size (best was 1992 at 6336, four bytes over). Constraining the
search to subsets that land on 6332 exactly and minimising within that produced
`[target_pp, target_max_hp_signed, actor_max_pp_for_drain]` -- **1961 differing
at exactly reference size**, better on both axes than the size-blind optimum.
The size constraint has to lead; the ordering score follows.

Reaching reference size also opens the permuter, which CONTRIBUTING gates behind
exactly that.

**THE STRUCTURAL FACT (extracted after the searches stalled).** With size exact,
the diff finally aligns and the residual has shape: **184 differing runs**, of
which 111 are single isolated instructions -- but eight long runs hold nearly
half the total. The largest, `0x12c6-0x1480`, is 199 instructions.

Reading that region gives the disease, and it is not ordering noise:

    ! 12c0  movs r0, #4      | movs r0, #1
      12c2  bl   0xffffffb0  | bl   0xffffffb0
    ! 12c6  movs r0, #155    | ldr  r1, [pc, #752]
    ! 12c8  lsls r0, r0, #1  | movs r0, #4
    ! 12ca  adds r2, r7, r0  | bl   0xffffffb0
    ! 12cc  b.n  0x14fc      |

The reference calls that function **twice**, with 1 and then 4. We call it once,
and branch to a shared tail at `0x14fc` where the reference has the code written
out inline. **Our C factors a tail the original duplicated.** That is a source
structure difference, and it explains why every ordering-level search plateaued:
they were permuting around a block that should not be shared at all.

Re-running all 171 flag trials against this improved baseline confirms no stock
option changes it, so the fix belongs in the C: un-share that tail, and check the
other seven long runs for the same disease before assuming they are noise.

**Four axes are now exhausted at this owner**, all measured:

| axis | trials | result |
|---|---|---|
| stock flags and pairs | 171 | nothing beats baseline |
| random declaration orders | 24 | all worse |
| pairwise declaration swaps | 12,090 | 0 better |
| inline cached field reads | 31 + 387 subsets | **2286 -> 1961**, converged |
| alchemy-permuter, 7 seeds | 4,634 candidates | 0 better; 4,480 tie at 1961 |

The permuter stalling for seven rounds is the documented tripwire: two stalled
rounds with no structural insight end the axis. Further search here is looping.
What is needed next is a new structural fact out of the assembly or the RTL
dumps, not another round.

**Searched, not guessed.** Everything below was measured on this owner with a
harness whose unshuffled control reads exactly `2286 / 6324`.

| axis | trials | best result |
|---|---|---|
| every stock flag and pair | 171 | none beats baseline 2286 |
| random declaration orders | 24 | all worse (2345-2357) |
| every pairwise declaration swap | **12,090** | **0 better**, 9,453 neutral, 2,637 worse |
| C variants at the first divergence | 6 | 3 were no-ops, 3 real ones all worse |

Declaration order is therefore at a strict local optimum: no single swap of the
156 locals improves it, and the aggregate shuffles show the current order is
already far better than chance. **Size never moved from 6324 in any of the
12,090 swaps** -- declaration order drives the ordering residual and cannot
touch the 8-byte gap. They are two independent problems.

A CORRECTION worth keeping, because it nearly became a false conclusion in this
file: an earlier round of three C variants each scored exactly 2286, and that
was reported as evidence the residual is invariant to source changes. It was
not. Those three produced **byte-identical assembly** -- gcc folded every temp
away and the only diff was internal label renumbering. Variants gcc cannot fold
move the score immediately (2294, 2583), and one of them reached size 6328,
four bytes closer. The residual IS C-responsive. Any claim that it is not
should be checked against whether the variant changed the emitted code at all.

So the order of work is fixed: close the 8 bytes first -- it is structural, and
the +70 `ldr` register-pressure signal is the lead -- then localize the residual
into clusters, and only then permute. Attempting the permuter now would be
searching noise with a structural error still in the source.
