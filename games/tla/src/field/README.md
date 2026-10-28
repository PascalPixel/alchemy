# The Lost Age: field

The Lost Age keeps its [scene](../../../tbs/glossary.md#scene) table (`scenes.s`) here and, under
`shared/`, the field code its scenes use: [actor scripts](../../../tbs/glossary.md#actor-script)
(`shared/script/`), the [map](../../../tbs/glossary.md#map)'s cells and animation (`shared/map/`),
[actors](../../../tbs/glossary.md#actor) (`shared/object/`), [events](../../../tbs/glossary.md#event),
[field effects](../../../tbs/glossary.md#field-effect) and the camera. The Broken Seal spreads the
same code over [The field bank](../../../tbs/src/field/README.md) and
[Running a scene](../../../tbs/src/overworld/README.md), which explain how it
works.

## What differs from The Broken Seal

- The main routine, `MainRoutine_Run`, is here in `shared/main_routine.c`,
  where The Broken Seal keeps it with Running a scene; it adds four debug
  starts and keeps background palettes 14 and 15 across three of its
  sequences.
- The folder is grouped by family of the field's code rather than by The
  Broken Seal's objects, one function or small family to a file.
  `shared/work9c/` and `shared/worka0/` are named for the
  [heap slot](../../../tbs/glossary.md#heap-slot) their code works in, by its offset in the slot
  table (`0x9c` and `0xa0`, slots 39 and 40), and their files and functions
  by address (`Func_080d8d40`) until their meaning is read.
- Each row of the scene table is 8 bytes in both games, but here its last
  halfword is a sixth column, `flag`, which The Broken Seal's rows leave
  zero. This game's scene [overlays](../../../tbs/glossary.md#overlay) are numbered from `0x649`
  (`scene Title, 0x649, ...`).
- The actor script commands keep `Script_` where The Broken Seal says
  `ActorScript_`.
