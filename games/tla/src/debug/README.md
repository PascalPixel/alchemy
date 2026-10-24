# The Lost Age: debug

This folder holds The Lost Age's debug rooms, scene overlays built like every
other scene: ☀️'s [Debug rooms](../../../tbs/src/debug/README.md) has the
rest. This page says only what differs here.

## What differs from ☀️

- **Five rooms, not three**: `rooms/`, `level/`, `psynergy_test/`,
  `psynergy_test_2/` and `shop_test/`, each a folder with its entry table
  (`entry.s`), its import table (`import.s`), its scene tables and its code.
  `presets.tsv` and `stream.c` sit beside them.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Layout.** A room's code is split into files by function, several still
  named by their offset in the overlay (`level/s004c.c`). Each overlay is
  linked by its own script under `recon/tla/raw/overlays`, not by a shared
  `overlay.ld`.

## Status

Functions not yet matched are drafts under `recon/tla/en/overlays`, by
overlay and address. `make progress` gives the counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`GameFlag_`, `Input_`, `Map_`, `Object_`, `ObjectMotion_`, `Scene_` and
`Unit_`. Those only The Lost Age's code has so far:

| Prefix | What it acts on |
| --- | --- |
| `Stream_` | reading halfwords from a byte stream |
| `BattleMap_` | the view a scene entrance sets for the battle map |
