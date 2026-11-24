# How the two games relate

The Lost Age is The Broken Seal's program, grown bigger. Most of its code
works as the first game's does, and reading the two side by side shows
more than that: which Broken Seal the sequel grew from, what the first game
already holds for the second, and what both carry without ever using it.

Everything on this page is read from the code of the two games. It says
what each one holds, not when or why Camelot made it so.

## It grew from the Japanese Broken Seal

The clearest sign is a test room. **Increase item and level** is one of the
developers' [debug scenes](../../games/tla/src/debug/README.md), and it is in both games.
The Broken Seal has it in two forms. In the Japanese edition there are four
testers, the level editor is drawn at 1.7 times its size, and casting Force
at the portrait browser runs a small motion test. The
[international editions](glossary.md#editions) add an ability browser and
two testers for emotes, draw the level editor at 1.6 times, and give Force
a different test. Every edition of The Lost Age has the Japanese room
([level.c](../../games/tla/src/debug/level.c), and
[The Broken Seal's](../../games/tbs/src/debug/level.c)).

The spring game says the same from the other side. The Japanese Lost Age
writes Lemuria's spring the way the Japanese Broken Seal writes Tolbi's:
one loop around the choice of coin or medal and the toss. The international
editions of both games write it another way
([lemuria_spring.c](../../games/tla/src/scene/lemuria_spring.c) and
[tolbi_games.c](../../games/tbs/src/scene/tolbi_games.c)). The Japanese Lost Age
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
([character_ids.h](../../games/tbs/include/game/character_ids.h)). Felix, Sheba and
Piers have empty rows in its table of growth curves, starting levels and
equipment. Jenna's row is filled, with other numbers than The Lost Age
gives her: she would start at level 1, not 5
([character_table.c](../../games/tbs/src/game/character_table.c), and
[The Lost Age's](../../games/tla/src/game/character_table.c)).

One battle effect is in The Broken Seal's table with no ability to play
it: the user leaps at the target and sparks burst beneath it
(`BattleEffect_RunUnusedSevereBlowLeap`, in
[battle_effect_thrash.c](../../games/tbs/src/battle_effect/battle_effect_thrash.c)).
The Broken Seal's Severe Blow plays a
[casting impact](../../games/tla/src/battle_effect/README.md#effect-families) instead. In
The Lost Age, Severe Blow is the leap
([battle_effect_thrash.c](../../games/tla/src/battle_effect/battle_effect_thrash.c)). The
Broken Seal's casting impacts also test for one kind of impact that nothing
in that game ever asks them for, and The Lost Age's do get asked for it
([battle_effect_casting_impact.c](../../games/tbs/src/battle_effect/battle_effect_casting_impact.c)).

## What both keep and never use

Some of what the games carry unused is the same in both. No ability in
either game plays the triangle strike, Bind's effect without its Djinni,
ending in a run at the first target (`BattleEffect_RunUnusedTriangleStrike`,
in [battle_effect_bind.c](../../games/tla/src/battle_effect/battle_effect_bind.c) and
[The Broken Seal's](../../games/tbs/src/battle_effect/battle_effect_bind.c)).

Three routines set aside room on the stack that they never touch, the same
amount in both games: the link battle's handshake after each turn, 20 bytes
(`LinkBattle_SyncTurn`), Lucky Dice, 32 (`LuckyDice_Run`), and Move cast
with no target, 12 (`FieldPsynergy_Move`).

Once it runs the other way. The Broken Seal's `Unit_Get`, which finds a
fighter's [unit](glossary.md#unit), reads the address it was called from
and never uses it
([game_units.c](../../games/tbs/src/game/game_units.c)). The Lost Age's does not
read it ([system_flags.c](../../games/tla/src/system/system_flags.c)).

## The same idea, reached another way

Both games keep each part's [state](glossary.md#state) in a
[heap slot](glossary.md#heap-slot). The Broken Seal's code finds the slots
through a table it links by name, `gHeapSlots`, and the field's state is
`gFieldState`, one of its entries. The Lost Age keeps its slots at a fixed
address and reads them as the fields of one struct,
`Ram_HeapSlots->fieldState`, in 384 of its source files. So a routine that
does the same thing in both games is often written, and compiled,
differently in each.

## Time passes the same way

Both games keep their frame loop inside `wait()` and run a short list of
[tasks](glossary.md#task) from it ([System](system.md#waiting-and-tasks)).
The differences are small, and each can be read in
[system_frame.c](../../games/tla/src/system/system_frame.c) beside
[The Broken Seal's](../../games/tbs/src/system/system_frame.c).

- The list holds 24 tasks. The Broken Seal's holds 20.
- The Broken Seal's `wait()` moves the stack to a fixed spot before it runs
  the tasks and puts it back after. The Lost Age's runs them on the
  caller's stack.
- The two passes have changed numbers. A task's pass is the high byte of
  its priority. The Broken Seal's `wait()` runs the tasks whose high byte
  is 12 and its VBlank handler those with 4. The Lost Age's `wait()` runs
  the 4s and its handler the 12s. The list is sorted highest first, so here
  the handler's tasks come first, and the handler stops at the first task
  of the other pass.
- The field keeps one task more. Actors that ride other actors are moved by
  `Actor_MoveWithCarriers`, at rank 10, and the actors' own task ranks 11
  to run before it ([Field code](../../games/tla/src/field/README.md#the-fields-tasks)).
- The field answers a battle request after a step and a push, where The
  Broken Seal answers it before them
  ([Field code](../../games/tla/src/field/README.md#the-twelve-requests)).
