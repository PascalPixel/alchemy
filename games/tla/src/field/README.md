# The Lost Age: field

The Lost Age's Field code: the second bank's files, its
[scene](../../../tbs/glossary.md#scene) table (`scenes.s`), and the families
the field's code is grouped in: [actor scripts](../../../tbs/glossary.md#actor-script)
(`script/`), the [map](../../../tbs/glossary.md#map)'s cells and animation
(`map/`), [actors](../../../tbs/glossary.md#actor) (`object/`),
[events](../../../tbs/glossary.md#event) (`event/`),
[field effects](../../../tbs/glossary.md#field-effect) (`field_effect/`), the
camera (`camera/`) and the ground heights (`terrain/`). The bank that runs a
scene is `field_sequence/`, the bank of event commands and field Psynergy is
`event_commands/`, and Lucky Dice and Lucky Wheels are `lucky_dice/` and
`lucky_wheels/`. The Broken Seal's [The field bank](../../../tbs/src/field/README.md)
and [Running a scene](../../../tbs/src/overworld/README.md) explain how this
code works.

## What differs from The Broken Seal

- `field_sequence/` holds, as separate objects, what The Broken Seal writes as
  the parts of one object, `field_sequence.c`. The main routine, which The
  Broken Seal also keeps in its System folder, is in
  [The Lost Age: system](../system/README.md).
- The folder is grouped by family of the field's code rather than by The
  Broken Seal's objects, one function or small family to a file. A family
  folder holds its files whatever their bank (`object/` holds files of three
  banks). `work9c/` and `worka0/` are named for the
  [heap slot](../../../tbs/glossary.md#heap-slot) their code works in, by its
  offset in the slot table (`0x9c` and `0xa0`, slots 39 and 40), and their
  files and functions by address (`Func_080d8d40`) until their meaning is
  read.
- The ground height functions in `terrain/` keep their older names: `Curve_`
  where The Broken Seal says `Terrain_` (`Curve_LerpTwoSamples` is
  `Terrain_LerpTwoSamples`), described under
  [How high a step can go](../../../tbs/src/field/README.md#how-high-a-step-can-go).
- Each row of the scene table is 8 bytes in both games, but here its last
  halfword is a sixth column, `flag`, which The Broken Seal's rows leave
  zero. This game's scene [overlays](../../../tbs/glossary.md#overlay) are numbered from `0x649`
  (`scene Title, 0x649, ...`).
- The actor script commands keep `Script_` where The Broken Seal says
  `ActorScript_`.
