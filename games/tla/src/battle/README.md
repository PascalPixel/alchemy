# The Lost Age: battle

The Lost Age keeps its battle code and, under `effect/`, its
[battle effects](../../../tbs/glossary.md#battle-effect) in one folder, where The Broken Seal has
two: [Battles](../../../tbs/src/battle/README.md) and
[Battle effects](../../../tbs/src/battle_effect/README.md). Those guides
explain how a battle runs, how an ability resolves and how an effect is drawn.

## What differs from The Broken Seal

- The code is not yet gathered into objects with parts. Most files hold one
  function or a small family and are named for it (`battle_event.c`,
  `actor_reset_runtime_fields.c`); `effect/`, `presentation/`, `motion/`,
  `party/` and `summon/` group the rest, and some files are still named by
  their address (`f11b2c4.c`, `d01297c4.s`).
- The tables (`actflags.tsv`, `actstat.tsv`, and the
  [formations](../../../tbs/glossary.md#formation) in `formatn.tsv`) are still TSV, packed by the
  build into the `.table` files that `battle_action.s` and `battle_vars.s`
  include; The Broken Seal writes its tables as C arrays (`gFormations` in
  [formations.c](../../../tbs/src/battle/formations.c)).
- This game's Thumb files are compiled with options of their own
  ([The Lost Age: system](../system/README.md)), so the same C can compile to
  different instructions here.
