# The Lost Age: scene

The Lost Age's [scenes](../../../tbs/glossary.md#scene), one folder per scene
[overlay](../../../tbs/glossary.md#overlay), 109 of them, each loaded at the same address as The
Broken Seal's. [Scenes](../../../tbs/src/scene/README.md) explains how a scene
runs, its tables and its [events](../../../tbs/glossary.md#event).

## What differs from The Broken Seal

- Most scenes' overlays are a folder, not a file. The Broken Seal writes each as
  one `<scene>.c` with its [entry and import tables](../../../tbs/glossary.md#entry-table-and-import-table)
  beside it. Here the folder holds `entry.s`, `import.s`, `scene_tables.c`
  and its code, with a header of its own names where it has any
  (`airs_rock_inside.h`). The overlay's tables follow its last code in the same
  file, or are `data.c` where the code still reads them through a view of its
  own. Where a scene's code has been gathered, it is one
  `<scene>.c` beside the folder, as The Broken Seal's are
  (`mercury_lighthouse.c`). Where a shared object, or files that disagree
  about a type, split the overlay's code, each other stretch is one file in
  the folder, named for its first file (`mt_aleph_summit/s0478.c`,
  `airs_rock_inside/s0038.c`); a file named for its offset in the overlay
  waits for what it does to be known.
- Both games link every overlay through one `overlay.ld`, one block per
  overlay: an overlay's block names its folder's objects in ROM order, and
  where its globals are still written in assembly they are the folder's
  `bss.s`.
- The scene table is in the field folder, with what differs in it
  ([The Lost Age: field](../field/README.md)).
- The main-image code a scene imports is in the bank folders of this game,
  most of it under `games/tla/src/field/`. Code that only overlays use sits
  there too: compiled inside each scene that uses it, which includes it, or its
  own object where an overlay still links it so. `scene/shared/` holds three
  such pieces: `drift_spawn.c`, compiled inside Idejima and its own object in
  one other overlay, `scene_set_arrival_flags.h`, compiled inside each
  scene that links it, and `cell_with_bit.h`, the lighthouse rooms' cell
  targets' one computation.
- The Title (`title/`), the scene in the row after it, `SceneId_Ov64a`
  (`clear/`, `0x64a`), with its password grid and button codes, and the lobby
  where a link battle is arranged (`lobby/`, `0x6b5`) are overlays here, as
  The Broken Seal's Title, File Screen and Battle Arena are.
