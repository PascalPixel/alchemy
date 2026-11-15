# How the two games relate

The Lost Age is The Broken Seal's program, grown bigger. Most of its code
works as the first game's does, and reading the two side by side shows
more than that: which Broken Seal the sequel grew from, what the first game
already holds for the second, and what both carry without ever using it.

Everything on this page is read from the code of the two games. It says
what each one holds, not when or why Camelot made it so.

## It grew from the Japanese Broken Seal

The clearest sign is a test room. **Increase item and level** is one of the
developers' [debug scenes](src/debug/README.md), and it is in both games.
The Broken Seal has it in two forms. In the Japanese edition there are four
testers, the level editor is drawn at 1.7 times its size, and casting Force
at the portrait browser runs a small motion test. The
[international editions](glossary.md#editions) add an ability browser and
two testers for emotes, draw the level editor at 1.6 times, and give Force
a different test. Every edition of The Lost Age has the Japanese room
([level.c](src/debug/level.c), and
[The Broken Seal's](../tbs/src/debug/level.c)).

The spring game says the same from the other side. The Japanese Lost Age
writes Lemuria's spring the way the Japanese Broken Seal writes Tolbi's:
one loop around the choice of coin or medal and the toss. The international
editions of both games write it another way
([lemuria_spring.c](src/scene/lemuria_spring.c) and
[tolbi_games.c](../tbs/src/scene/tolbi_games.c)). The Japanese Lost Age
also takes the coin or medal as soon as it is chosen, so backing out of the
toss loses it. No edition of The Broken Seal does that, and The Lost Age's
international editions take it only once the toss is confirmed.

In both places the Japanese Lost Age has the Japanese Broken Seal's
version. The test room never changed again. The spring was rewritten for
The Lost Age's international editions, much as it had been for The Broken
Seal's.

## What The Broken Seal already holds

The Broken Seal knows all eight party members. Its character ids run from
Isaac to Piers, and each of the eight has a portrait slot
([character_ids.h](../tbs/include/game/character_ids.h)). Felix, Sheba and
Piers have empty rows in its table of growth curves, starting levels and
equipment. Jenna's row is filled, with other numbers than The Lost Age
gives her: she would start at level 1, not 5
([character_table.c](../tbs/src/game/character_table.c), and
[The Lost Age's](src/game/character_table.c)).

One battle effect is in The Broken Seal's table with no ability to play
it: the user leaps at the target and sparks burst beneath it
(`BattleEffect_RunUnusedSevereBlowLeap`, in
[battle_effect_thrash.c](../tbs/src/battle_effect/battle_effect_thrash.c)).
The Broken Seal's Severe Blow plays a
[casting impact](src/battle_effect/README.md#effect-families) instead. In
The Lost Age, Severe Blow is the leap
([battle_effect_thrash.c](src/battle_effect/battle_effect_thrash.c)). The
Broken Seal's casting impacts also test for one kind of impact that nothing
in that game ever asks them for, and The Lost Age's do get asked for it
([battle_effect_casting_impact.c](../tbs/src/battle_effect/battle_effect_casting_impact.c)).

## What both keep and never use

Some of what the games carry unused is the same in both. No ability in
either game plays the triangle strike, Bind's effect without its Djinni,
ending in a run at the first target (`BattleEffect_RunUnusedTriangleStrike`,
in [battle_effect_bind.c](src/battle_effect/battle_effect_bind.c) and
[The Broken Seal's](../tbs/src/battle_effect/battle_effect_bind.c)).

Three routines set aside room on the stack that they never touch, the same
amount in both games: the link battle's handshake after each turn, 20 bytes
(`LinkBattle_SyncTurn`), Lucky Dice, 32 (`LuckyDice_Run`), and Move cast
with no target, 12 (`FieldPsynergy_Move`).

Once it runs the other way. The Broken Seal's `Unit_Get`, which finds a
fighter's [unit](glossary.md#unit), reads the address it was called from
and never uses it
([game_units.c](../tbs/src/game/game_units.c)). The Lost Age's does not
read it ([system_flags.c](src/system/system_flags.c)).

## The same idea, reached another way

Both games keep each part's [work](glossary.md#work) in a
[heap slot](glossary.md#heap-slot). The Broken Seal's code finds the slots
through a table it links by name, `gHeapSlots`, and the field's work is
`gFieldWork`, one of its entries. The Lost Age keeps its slots at a fixed
address and reads them as the fields of one struct,
`Ram_HeapSlots->fieldWork`, in 384 of its source files. So a routine that
does the same job in both games is often written, and compiled, differently
in each.
