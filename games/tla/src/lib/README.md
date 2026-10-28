# The Lost Age: lib

The Lost Age's maths: the sine and cosine (`trig.s`, `sine_table.s`), the
tangent table the arctangent searches (`mathtan.s`), fixed-point multiplies
and the reciprocal, the 16.16 dot product, and the ground height functions
that work out the height at any pixel of a map [cell](../../../tbs/glossary.md#cell) (`curve.c`,
`step.c` and the files beside them). The Broken Seal has no folder of this
name.

## What differs from The Broken Seal

- These routines are kept together by subject, though the ROM places them
  apart: the trigonometry, `math.c`, the long multiplies and the dot product
  in the first [bank](../../../tbs/glossary.md#bank) with the runtime code (`rom_main_00`), the
  ground heights in the second with the field (`rom_main_01`), and the small
  multiplies and the reciprocal in `rom_main_12` (`games/tla/objects.tsv`).
  The Broken Seal's twins sit in the folders of their banks: `trig.s`,
  `q16_dot_product.s` and `runtime/math.c` under
  [Runtime](../../../tbs/src/runtime/README.md), and the ground heights in
  [terrain.c](../../../tbs/src/field/field/terrain.c), described under
  [How high a step can go](../../../tbs/src/field/README.md#how-high-a-step-can-go).
- The ground height functions keep their older names here: `Curve_` where The
  Broken Seal says `Terrain_` (`Curve_LerpTwoSamples` is
  `Terrain_LerpTwoSamples`).
