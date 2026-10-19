# Debug rooms

This folder holds three debug rooms the retail game keeps: scene overlays
built like every other scene, each an entry table, the room's code and its
import table.

## Files

| Overlay | Files | What the room holds |
| --- | --- | --- |
| 0x3cc | `rooms.c` | four Djinn, one of each element, to receive and test |
| 0x3cd | `level.c`, [`level.h`](level.h) | level changes for the party, an item, icon and glyph browser, emote and motion tests |
| 0x3ce | `test.c`, `menu_test.h` | message browsers for every shop, shop and inventory tests, test stock and the name editor |
| | `entry/<room>.s` | the functions the scene loader calls (the room's entrances, exits, placements and events) |
| | `import/<room>.s` | the main-image functions the room calls |

Link order is in `games/tbs/objects.tsv`, from `debug/entry/rooms`. The unit
headers are in `games/tbs/include/unit/debug/`; [`level.h`](level.h) and
`menu_test.h` here are each room's private declarations.

## Words

- **owner**: a party member's number, as in `games/tbs/src/game`.
- **entrance**, **exit**, **placement** and **event** tables: what every
  scene gives the loader, here through the `*_Get*` functions.
- The rooms' messages are linker symbols; their text is in the PO catalogs in
  `games/tbs/text`.

## Prefixes

| Prefix | Room |
| --- | --- |
| `TestRooms_` | the Djinn room (`rooms.c`) |
| `ItemLevel_` | the level and browser room (`level.c`) |
| `MenuTest_`, `DebugMenu_` | the message and shop test room (`test.c`) |
| `SceneData_`, `Scene_`, `FieldScene_`, `Engine_` | scene entry points and engine calls every scene shares |

## Limits

- Each room is an overlay, loaded into the same 32 KB at 0x2008000 as every
  scene (`../system/README.md`).
