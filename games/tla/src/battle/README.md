# The Lost Age: battle

The Lost Age's battle bank: how a battle runs and an ability resolves.
[Battles](../../../tbs/src/battle/README.md) explains it for The Broken Seal;
the [battle effects](../../../tbs/glossary.md#battle-effect) are in
[The Lost Age: battle effects](../battle_effect/README.md).

## What differs from The Broken Seal

- The code is only partly gathered into objects with parts. `battle.c` holds
  the bank's first eleven parts, cut where The Broken Seal's are, from the
  debug routines to the start of an action's staging; it ends before
  `presentation/f11e3ac.c`, which calls `BattleObject_Reset` with one argument
  where the object's prototype has two (`battle.c` says more). The rest is
  still one function or a small family to a file, named for it
  (`battle_event.c`, `actor_reset_fields.c`); `presentation/`, `motion/`,
  `party/` and `summon/` group some, and some files are still named by their
  address (`f120454.c`, `d01297c4.s`).
- The tables (`actflags.tsv`, `actstat.tsv`, the
  [formations](../../../tbs/glossary.md#formation) in `formatn.tsv` and the
  debug parameter sets in `debug_parameter_sets.tsv`) are still TSV, packed by
  the build into the `.table` files that `battle_action.s`, `battle_vars.s`
  and `debug_parameter_sets.s` include; The Broken Seal writes its tables as C
  arrays (`gFormations` in [formations.c](../../../tbs/src/battle/formations.c)).
- This game's Thumb files are compiled with options of their own
  ([The Lost Age: system](../system/README.md)), so the same C can compile to
  different instructions here.
