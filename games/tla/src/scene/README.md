# The Lost Age: scene

This folder holds The Lost Age's scenes: one folder per place, 106 of them,
each an overlay loaded at 0x2008000 like ☀️'s. ☀️'s
[Scenes](../../../tbs/src/scene/README.md) explains how a scene runs, its
tables and its events; this page says only what differs here.

## What differs from ☀️

- **A scene is a folder, not a file.** ☀️ writes each scene as one
  `<place>.c` with its entry and import tables beside it. ⚓️'s scene is a
  folder: `entry.s`, `import.s`, `scene_tables.c`, `data.c`, and its code in
  files named for the offset in the overlay where each function begins
  (`airs_rock_inside/s0128.c`), with a header of its own names where it has
  any (`airs_rock_inside.h`).
- **Linking.** Each overlay is linked by its own script,
  `recon/tla/raw/overlays/resource_<id>.ld`, which lists the folder's objects
  in ROM order and, between them, the sections of
  `resource_<id>_overlay.s` that are not yet C. ☀️ links all its overlays
  through one `overlay.ld` and its object list.
- **Numbers.** ⚓️'s overlays are numbered from 0x649
  (`games/tla/src/field/scenes.s`), and its scene table has a sixth column, a
  flag. A place both games have is a separate ⚓️ scene with its own number
  and map.
- **Engine calls.** The main-image code a ⚓️ scene imports is in
  `games/tla/src/field/common/` and the other ⚓️ folders, under the names of
  its ☀️ twins where it does the same.
- **Compiler.** K2's options on top of ☀️'s (`CFLAGS_tla` in the
  `Makefile`): `-mthumb-split-constants`, `-mthumb-call-via-lr` and
  `-mtune=arm9tdmi`, without `-mthumb-interwork`.

## Status

Functions not yet matched are drafts under `recon/tla/en/overlays`, by
overlay and address (`resource_64b_c_02002594.c`). `make progress` gives the
counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`Actor_`, `Animation_`, `Camera_`, `Engine_`, `Event_`, `FieldEvent_`,
`GameFlag_`, `Map_`, `Motion_`, `Object_`, `ObjectDispatch_`, `ObjectMotion_`,
`OverlayObject_`, `Scene_`, `SceneActor_`, `SceneState_`, `Scheduler_`,
`Vector_` and `Vram_`, with a place's name for its own events. Those only The
Lost Age's code has so far:

| Prefix | What it acts on |
| --- | --- |
| `FelixWake_` | the steps of the waking event in `idejima/wake.c` |
| `StoryActor_` | the actors the world map's story scenes place |
| `Exit_` | the exit the Gabomba Statue scene sets |
| `Blend_`, `Frame_`, `Speed_` | writing the blend alpha, the frame count modulo 6, and setting an actor's speed |
| `Mem_`, `Pool_`, `Words_` | allocating from a scene's memory pool, and copying words |

## Evidence and open questions

- XXX: which functions several ⚓️ overlays carry as copies, as ☀️'s do in
  its `common/` headers; ⚓️ has no such headers yet.
- XXX: what the scene table's sixth column means.
