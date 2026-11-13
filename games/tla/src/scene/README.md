# The Lost Age: scene

The Lost Age's [scenes](../../../tbs/glossary.md#scene), one folder per scene
[overlay](../../../tbs/glossary.md#overlay), 109 of them, each loaded at the same address as The
Broken Seal's. [Scenes](../../../tbs/src/scene/README.md) explains how a scene
runs, its tables and its [events](../../../tbs/glossary.md#event).

## What differs from The Broken Seal

- A scene is one `<scene>.c`, as The Broken Seal's are, with its [entry and import tables](../../../tbs/glossary.md#entry-table-and-import-table)
  in `entry/` and `import/` and a header of its own names beside it where it
  has any (`airs_rock_inside.h`). Where a shared object, or files that
  disagree about a type, split the overlay's code, each other stretch is one
  file in a folder named for the scene, itself named for its first file
  (`mt_aleph_summit/s0478.c`, `airs_rock_inside/s0038.c`); a file named for
  its offset in the overlay waits for what it does to be known. The
  overlay's tables follow its last code in the same file, or are
  `data/<scene>.c` where the code still reads them through a view of its own.
- Both games link every overlay through one `overlay.ld`, one block per
  overlay: an overlay's block names its folder's objects in ROM order, and
  where its globals are still written in assembly they are
  `bss/<scene>.s`, as in The Broken Seal.
- The scene table is in the field folder, with what differs in it
  ([The Lost Age: field](../field/README.md)).
- The main-image code a scene imports is in the bank folders of this game,
  most of it under `games/tla/src/field/`. Code that only overlays use sits
  there too: compiled inside each scene that uses it, which includes it, or its
  own object where an overlay still links it so. `scene/shared/` holds such
  pieces: `drift_spawn.c`, compiled inside Idejima and its own object in one
  other overlay, `scene_set_arrival_flags.h`, compiled inside each scene that
  links it, and the routines several scenes copy, each written once as a macro
  a scene expands where its copy sits, in headers by kind: `teleporter.h`,
  `whirlwind.h`, `push.h`, `map_cells.h` and the rest.
- The Title (`title/`), the scene in the row after it, `SceneId_Ov64a`
  (`clear/`, `0x64a`), with its password grid and button codes, and the lobby
  where a link battle is arranged (`lobby/`, `0x6b5`) are overlays here, as
  The Broken Seal's Title, File Screen and Battle Arena are.
