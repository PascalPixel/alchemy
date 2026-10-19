# The Lost Age: battle

This folder holds The Lost Age's battle code and, in `effect/`, its battle
effects: ☀️'s [Battles](../../../tbs/src/battle/README.md) and
[Battle effects](../../../tbs/src/effect/README.md) in one folder. Those
guides explain how a battle runs, how an ability resolves and how an effect
draws; this page says only what differs here.

## What differs from ☀️

- **Compiler.** Every Thumb file is compiled with K2's options on top of
  ☀️'s: `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork` (the `Makefile`'s
  `CFLAGS_tla`). They were measured across ⚓️'s whole code, so the same C can
  compile to different instructions than its ☀️ twin.
- **Layout.** The code is not yet grouped into objects with parts. Most files
  hold one function or one small family, named for it
  (`battle_event.c`, `actor_reset_runtime_fields.c`); `effect/` alone has
  294 files, and `presentation/`, `motion/`, `party/` and `summon/` group the
  rest. Some data files are still named by their address (`d01297c4.s`).
- **Tables.** `actflags.tsv`, `actstat.tsv` and `formatn.tsv` are ⚓️'s own,
  with its own abilities and enemy groups; their rows are not ☀️'s.
- **Names.** A function that does what its ☀️ twin does takes the twin's
  name (`BattleEvPush`, `BattleTargetExec`), so a search finds both.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, one file per
address, each with its remaining difference in its header. An edition whose
bytes differ from English stays on its scaffold under `recon/tla/<lang>`.
`make progress` gives the counts.

## Evidence and open questions

- XXX: which of these files belong to one original object, and so where the
  parts of a future `battle.c` and `effect.c` begin and end.
