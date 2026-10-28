# The Lost Age: battle effects

The Lost Age's battle effect bank: the
[battle effects](../../../tbs/glossary.md#battle-effect) that abilities and
summons play, and the blitter fragments they draw with
(`blitter_fragments.s`, `blitter_color_fragments.s`).
[Battle effects](../../../tbs/src/battle_effect/README.md) explains how an
effect is drawn in The Broken Seal.

## What differs from The Broken Seal

- The code is not gathered into one object with parts: each file holds one
  function or a small family, many still named by address (`f15e22c.c`).
