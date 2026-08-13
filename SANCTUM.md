# Exhausted-search ledger (Sanctum)

This file prevents contributors from repeatedly attacking an owner after both
bounded search axes have completed with nonzero residuals. “Sanctum” is the
*Golden Sun* shorthand: like Sol Sanctum, an owner is sealed until new evidence
provides a key. The useful mechanism is the ledger and queue, not the name.

This is not permanent assembly. Permanent assembly records code deliberately
kept outside C. A sealed owner may still have an exact C form; it is simply
withdrawn from routine search until its source model, compiler evidence, or
tooling changes.

## Admission rule

An owner may be sealed only when all of these are true:

1. its C has passed the semantic-readiness review in `CONTRIBUTING.md`;
2. the bounded compiler-mode search has completed without an exact result;
3. the bounded deterministic source-shape search has completed without an
   exact result; and
4. the smallest witnessed residual is recorded and understood well enough to
   distinguish it from a bad owner boundary or guessed source model.

The two deterministic axes are:

| Axis | Command | Holds fixed |
|---|---|---|
| compiler | `compiler mode_sweep` | source |
| source shape | `search shape_sweep` | compiler route |

Run these through `tools/dispatch`. A stochastic permuter run that finds
nothing is cost evidence, not proof of exhaustion, and cannot seal an owner.

## Entry format

`check check_sanctum` enforces one list item per owner under `## Sealed`:

```text
- `<owner>` floor=<N>hw axes=compiler,shape — <the localized residual>
```

`<owner>` is the filename stem under `semantic/`. `floor` is the smallest
differing-halfword count reached by either completed axis; the two axes need
not end at the same floor. Remove an entry as soon as that owner becomes exact;
the checker treats a solved sealed owner as stale evidence.

## Sealed

- `resource_39f_c_02001d04` floor=2hw axes=compiler,shape — the reference
  keeps the split constant `movs r3, #192 / lsls r3, #11` adjacent before the
  four-argument presentation call and copies `r1` from `r5` after it; the fork
  schedules the copy between the halves. The exact slot-11 sibling
  `resource_39f_c_02001244` compiles the identical sheet correctly on the same
  default route. 155 single modes, 400 deterministic pairs, and the complete
  shape-transform set all leave the two swapped halfwords at 0x02001d9a and
  0x02001d9c; `-fno-sched-depend-count` fixes the site but reorders 27 bytes
  of matching code elsewhere in the owner.
- `resource_3b7_c_02000880` floor=2hw axes=compiler,shape — in the
  eight-frame BLDALPHA loop the reference orders `adds r5, #2` before the
  volatile `strh` and `movs r0, #8` after it; the routed
  `-fno-sched-depend-count` path emits the same four instructions with the
  independent pair transposed (0x020008c4/0x020008c8). 555 mode
  configurations including pairs, the shape-transform set, split-increment
  spellings, and a bounded permuter run all hold the floor at two halfwords.

## Immediate source-shape queue

There are currently 32 owners with a completed compiler axis and no recorded
source-shape pass. This is a live queue, not 32 sealed owners. Regenerate it:

```sh
cargo run --release --manifest-path tools/dispatch/Cargo.toml -- \
  check check_sanctum --queue
```

Start with the lowest localized floors. If a source is wrong-sized or its
residual is diffuse, return it to reconstruction instead of treating a failed
shape sweep as exhaustion.
