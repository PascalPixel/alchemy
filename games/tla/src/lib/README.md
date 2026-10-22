# The Lost Age: lib

This folder holds The Lost Age's maths: the sine and cosine (`trig.s`,
`sine_table.s`), the arctangent (`mathtan.s`), fixed-point multiplies and
reciprocals, the 16.16 dot product, and the curves and steps that ease a
value towards a target. ☀️ has no folder of this name: its twin of this code
is part of [Runtime](../../../tbs/src/runtime/README.md) (`trig.s`,
`q16_dot_product.s`, the math part) and of the field's curves
([Field](../../../tbs/src/field/README.md), `Curve_`). Those guides explain
the units; this page says only what differs here.

## What differs from ☀️

- **Grouping.** ⚓️ keeps these routines together in one folder by subject,
  though the ROM places them apart: the trigonometry and the dot product in
  the first bank with the runtime (`rom_main_00`), the curves and steps in
  the second with the field (`rom_main_01`), and the small multiplies and the
  reciprocal in `rom_main_12` (`games/tla/objects.tsv`). ☀️'s twins sit in
  the folders of their banks.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`. The `.s` files are
  handwritten, each proven in its header.
- **Names.** Routines that do what their ☀️ twins do take their names
  (`Trig_Sin`, `Trig_Cos`).

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address.
`make progress` gives the counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`FixedPoint_`, `Math_`, `Palette_` and `Vector_`.
