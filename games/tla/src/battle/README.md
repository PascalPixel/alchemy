# The Lost Age: battle

The Lost Age's battle bank: how a battle runs and an ability resolves.
[Battles](../../../tbs/src/battle/README.md) explains it for The Broken Seal;
the [battle effects](../../../tbs/glossary.md#battle-effect) are in
[The Lost Age: battle effects](../battle_effect/README.md).

## What differs from The Broken Seal

- The bank's C is one object, `battle.c`, as in The Broken Seal: eleven
  parts under `battle/`, cut where The Broken Seal's are, from the debug
  routines to the start of an action's staging, then the rest of the bank in
  ROM order, its parts under `presentation/`. One of those calls
  `BattleObject_Reset` with one argument where the object's definition has two,
  through a view (`battle.c` says more).
- The tables (the action flags and statuses, the
  [formations](../../../tbs/glossary.md#formation) and the debug parameter
  sets) are rows of assembly in `battle_tables.s`; The Broken Seal writes its tables as C
  arrays (`gFormations` in [formations.c](../../../tbs/src/battle/formations.c)).
- This game's Thumb files are compiled with options of their own
  ([The Lost Age: system](../system/README.md)), so the same C can compile to
  different instructions here.
