# The Lost Age: scene

The Lost Age's [scenes](../../../tbs/glossary.md#scene), one folder per scene
[overlay](../../../tbs/glossary.md#overlay), 106 of them, each loaded at the same address as The
Broken Seal's. [Scenes](../../../tbs/src/scene/README.md) explains how a scene
runs, its tables and its [events](../../../tbs/glossary.md#event).

## What differs from The Broken Seal

- A scene's overlay is a folder, not a file. The Broken Seal writes each as
  one `<scene>.c` with its [entry and import tables](../../../tbs/glossary.md#entry-table-and-import-table)
  beside it. Here the folder holds `entry.s`, `import.s`, `scene_tables.c`,
  `data.c`, and its code, one function or family to a file, named for what it
  does or, until that is known, for its offset in the overlay
  (`airs_rock_inside/s0128.c`), with a header of its own names where it has
  any (`airs_rock_inside.h`).
- Each overlay is linked by its own script,
  `recon/tla/raw/overlays/resource_<id>.ld`, which lists the folder's objects
  in ROM order and, between them, the sections of `resource_<id>_overlay.s`
  that are not yet C. The Broken Seal links all its overlays through one
  `overlay.ld` and its object list.
- The scene table is in the field folder, with what differs in it
  ([The Lost Age: field](../field/README.md)).
- The main-image code a scene imports is in `games/tla/src/field/shared/` and
  the other folders of this game.
