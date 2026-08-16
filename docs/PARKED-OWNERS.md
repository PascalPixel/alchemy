# The parked owners: what is actually blocking them

100 main-image owners sit in `non_matching/`. They were parked when the compiler
axe deleted 120 invented options, and 12 have since been reconstructed in C.
This file records what blocks the rest, because three separate bulk attempts
have now failed and the reasons are specific rather than "needs more effort".

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
