# The Lost Age: field

The Lost Age's Field code: the second bank's files, its
[scene](../../../tbs/glossary.md#scene) table (`scenes.s`), and the families
the field's code is grouped in: [actor scripts](../../../tbs/glossary.md#actor-script)
(`script/`), the [map](../../../tbs/glossary.md#map)'s cells and animation
(`map/`), [actors](../../../tbs/glossary.md#actor) (`object/`),
[events](../../../tbs/glossary.md#event) (`event/`),
[field effects](../../../tbs/glossary.md#field-effect) (`field_effect/`) and
the ground heights (`map/`, their samples in
`terrain/`). The bank that runs a scene is `field_sequence/`, the bank of
event commands and field Psynergy is `event_commands/`, Lucky Dice and Lucky
Wheels are `lucky_dice.c` and
`lucky_wheels.c`, and the casino's dice game, in a bank of its own, is
`dice_game.c`. The Broken Seal's [Field code](../../../tbs/src/field/README.md)
explains how this code works.

## What differs from The Broken Seal

- `field_sequence.c` holds the start of the field-sequence bank as parts
  under `field_sequence/`, cut where The Broken Seal's are: encounters,
  actors, walking, the run, the palette editor, triggers, targets, items,
  Psynergy and the treasure spots. The rest of the bank follows it as
  objects of their own in ROM order, in `field_sequence/` and the family
  folders.
  The main routine, which The Broken Seal also keeps in its System folder, is
  in [The Lost Age: system](../system/README.md).
- `event_commands.c` holds the start of the event-command bank as parts under
  `event_commands/`: `event.c`, `requests.c` and `actor_commands.c`, as The
  Broken Seal's object begins. `object/step_angle.c` follows it as an object of
  its own, since an overlay links it too; the rest of the bank follows as
  objects of their own in ROM order.
- `field.c` holds the start of the field bank's C as parts under `field/`,
  cut where The Broken Seal's field.c parts are: `decompress.c` and
  `sprite_draw.c` so far. The rest of the bank's C follows it as 23 objects
  in ROM order, in `object/`, `script/`, `map/` and this folder.
- The rest of the folder is grouped by family of the field's code rather than by The
  Broken Seal's objects. A family
  folder holds its files whatever their bank (`object/` holds files of three
  banks). `work9c/` and `worka0/` are named for the
  [heap slot](../../../tbs/glossary.md#heap-slot) their code works in, by its
  offset in the slot table (`0x9c` and `0xa0`, slots 39 and 40), and their
  files and functions by address (`Func_080d8d40`) until their meaning is
  read.
- The ground height functions in `map/f02c58c.c` are The Broken Seal's,
  described under
  [How high a step can go](../../../tbs/src/field/README.md#how-high-a-step-can-go),
  but this game drops the two bends along an axis: the sample tables come
  twice, the first pair with x and y swapped, and the last two kinds are new
  steps split three ways.
- Each row of the scene table is 8 bytes in both games, but here its last
  halfword is a sixth column, `flag`, which The Broken Seal's rows leave
  zero. This game's scene [overlays](../../../tbs/glossary.md#overlay) are numbered from `0x649`
  (`scene Title, 0x649, ...`).
- The actor script commands keep `Script_` where The Broken Seal says
  `ActorScript_`.
