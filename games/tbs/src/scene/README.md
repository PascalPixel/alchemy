# Scenes

Each [scene](../../glossary.md#scene)'s own code says who stands where, what
each person says, what happens when the [party](../../glossary.md#the-party)
steps on a doorstep, searches a jar or uses an item or
[Psynergy](../../glossary.md#psynergy-and-pp), and the events it plays. It is
an [overlay](../../glossary.md#overlay): the
[field sequence](../field/README.md#the-field-sequence-serves-requests) asks
it questions through its entry table, and it calls the main image through its
[import table](../../glossary.md#entry-table-and-import-table). Every scene
has a generated page under [Places](../field/scenes.s), in
[story order](../../glossary.md#the-places-in-story-order), with its
[map](../../glossary.md#map), its [triggers](../../glossary.md#trigger)
and a recording of each event.

## The entry table

The six questions the field asks every scene, and when it asks them:

When the party arrives, the field asks what can happen here (the
triggers, kept until an event swaps them), where the party arrives (the
[entrances](../../glossary.md#entrance)), who stands where (the
[actor](../../glossary.md#actor) placements), and whether
anything needs setting up. At every step it asks whether the leader is
standing in an [exit](../../glossary.md#exit) area, and as the party leaves,
where the exit leads. Each answer comes from a function rather than a fixed
table, so a scene's answers can change with the story
([Field](../../field.md#scene-entries-and-triggers) gives Vault's example), and
an overlay that serves several scenes can answer for each.

The setup restores what the flags say has happened, starts the scene's own
timed work, or plays an arrival event, and can move the party straight on. It
skips arrival events when the party is coming back to where it stood, as
after a battle.

The six are `struct SceneEntryPoints`, in a fixed order at the start of
every overlay (`entry/<scene>.s`), each behind an 8-byte
[veneer](../../glossary.md#veneer). Vault names them `Scene_Initialize`,
`Scene_GetEntrances`, `Scene_GetExits`, `Scene_GetActorPlacements`,
`Scene_GetTriggers` and `Scene_GetExitRegions`; some overlays give them names of
their own (`BabiLighthouse_GetExits`), since only the order is fixed.

## The tables

The answers are tables: where the party can arrive and leave, who stands
where, and what answers each thing the player does. The field is 3D, with y
the height and x and z the ground, so a position in them is in pixels with x
east, y up and z south, and a facing is one of eight headings
([Words used here](#words-used-here) gives the units).

- Entrances: for each place the party can arrive, a flag that must be set, a
  position, a facing and how far the camera may go.
- Exits: for each way out, the scene and entrance it leads to, sometimes only
  while a flag is clear. One kind of exit leads into a battle. An exit also
  says how the leader leaves: plainly, silently, or up or down stairs with an
  animation and sound the next scene answers (`enum ExitStyle`).
- Actor placements: who stands where, with a
  [sprite](../../glossary.md#sprite-and-oam), a condition, how they move
  (standing, wandering, or an [actor script](../../glossary.md#actor-script) of
  their own), a position, a facing and how they turn when spoken to.
- Regions: boxes in which the leader takes an exit, each with a condition.
  Most scenes have none.
- Triggers: what answers each thing the player does
  ([below](#what-a-trigger-row-says)).

These are `struct SceneEntrance`, the `SCENE_EXIT` rows of the exit
table (named in `scene_exits.h`), `struct ActorPlacement`,
`struct SceneExitRegion` and `struct Trigger`, all in `scene_tables.h` and
`scene.h`.

### Conditions

The actor placements and the triggers carry a condition, which says
whether the row holds. Most often it is a [game flag](../../glossary.md#game-flag),
and the row holds until that flag is set: "until this has happened" is the
common case.

| Condition | The row holds |
| --- | --- |
| `CONDITION_ALWAYS` (-1) | always |
| a game flag, such as `FLAG_VALE_NUT_CAUGHT` | while the flag is clear |
| `IF_FLAG_SET(flag)` | while the flag is set |
| `CONDITION_IN_VIEW` (0), actor placements only | while the actor lies within the view, in scenes that call `Scene_AssignViewFlags` (so only the room entered is populated) |

### Who stands where

Each row of a scene's actor placements puts one person or thing in the
scene, an actor, and each row owns an
[actor id](../../glossary.md#actor-id), whether or not it appears.

A row whose condition fails puts nobody in the scene. Its id stays empty
rather than going to the next row, so every actor keeps one number in its
scene and the scene's code can name it once: on Mercury Lighthouse's upper
floor the blue statue has one row for its place on the ledge and another for
where it lands after it falls, and only one of the two appears.

The party takes ids 0 to 7, and the table's rows take the ids from
`ACTOR_FIRST_PLACED` in order. A row whose sprite is one of the party's
characters takes that character's own id instead.

Each scene names its actors in an enum in row order, starting from
`ACTOR_FIRST_PLACED`; `ACTOR_PARTY_LEADER` is Isaac, the one the player moves
everywhere but Colosso, and `Event_SpawnActorPlacements` places the rows.

### What a trigger row says

![Isaac talks to his mother Dora outside their house in Vale: "It looks like it won't rain for some time... so I guess I'll just have you fix it later."](https://alchemy.pascalpixel.com/places/vale/events/Event_DoraFretsAboutTheRoof.gif)

Each thing the player can do in a scene that the scene answers, talking to Dora
here, is one row of its trigger table: what starts it, whom or where it
answers, when it holds, and what runs. These are four rows of Vale's table on
the day the story begins:

```c
{ TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS,
  EVENT(Vale_DoraFretsAboutTheRoof) },
{ STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
  EVENT(Vale_EnterIsaacsHouse) },
{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_VALE_NUT_CAUGHT,
  EVENT(Vale_CatchNut) },
{ EXAMINE_TRIGGER(EXAMINE_SILENT), 24, CONDITION_ALWAYS,
  EVENT(Vale_SearchTheLockedDoor) },
```

Talking to Dora, Isaac's mother, runs an event; walking north into the
doorstep of Isaac's house, a [step trigger](../../glossary.md#step-trigger),
takes him in; Catch on the nut works until the nut has been caught; and A
facing the locked door, an [examine trigger](../../glossary.md#examine-trigger),
searches it. What starts a row also says what it answers:

| Kind | Started by | Answers |
| --- | --- | --- |
| `TRIGGER_TALK` | the A button on someone | that actor |
| `TRIGGER_TALK_HALTED` | talking to someone Halt has stopped | that actor |
| `TRIGGER_STEP` and its forms | stepping onto a spot, facing a way, or pushing against it | the spot's [cell behavior](../../glossary.md#cell-behavior) |
| `TRIGGER_EXIT` | stepping onto a spot no step trigger answers | the spot's cell behavior |
| `EXAMINE_TRIGGER(target)` | the A button on what is ahead, when it is nobody | the spot's cell behavior |
| `USE_ITEM(item)`, `USE_ITEM_ON_ACTOR(item)` | using an item | a spot or an actor |
| `PSYNERGY_ON_ACTOR(ability)`, `PSYNERGY_ANYWHERE(ability, phase)` | using Psynergy on someone, or anywhere | that actor, or anywhere |
| `TRIGGER_RAISED` | the scene's code, or the menus while `FLAG_MENUS_RAISE_TRIGGER` is set | a number the code gives |

A Psynergy row also says when it runs around the Psynergy: before it plays,
after it, or from inside it, as Force strikes, Frost freezes or Reveal shows
what is hidden. Reveal's scenes pair that row with one that hides everything
again as the colours come back, and a row can run when Psynergy that runs
out by itself, such as Cloak, has ended. [Field code](../field/README.md#how-field-psynergy-reaches-its-target) follows a
cast through these.

What runs is a message, an event, or for a search what is found there: an
item, coins, a Psynergy Stone, a line of text or a mimic's battle. One word
holds either a message or an event, since events live at addresses above any
message number.

The row is `struct Trigger`, its kinds and the `FIND_` values are in
`scene_tables.h`, an event is written `EVENT(function)` and a message
`MSGNO(name)`.

### Which triggers a cell starts

![Isaac walks into the door of his house in Vale after the party has left, and Dora, behind it, refuses to believe it is him: "My Isaac would keep his promise... Go away."](https://alchemy.pascalpixel.com/places/vale/events/Event_EnterIsaacsHouse.gif)

Some spots in a scene do something when the leader reaches them: a doorstep
leads into a house when the leader walks into it, the edge of a village leads
on to the next scene, a ladder climbs to a roof, and a chest or a bookcase
answers the A button. The map marks each spot by the cell behavior of its
[cell](../../glossary.md#cell), and the scene's trigger table says what that
number does. The same number means different things in different scenes.

In Vale on the day the story begins, each door is a step trigger rather than an
exit: walking north into the doorstep runs an event that opens the door,
walks the leader in and only then asks for the exit, and that event can refuse
instead, as Dora does at Isaac's door between the party leaving Vale and
reaching Kalay. Step triggers also climb the ladder beside Isaac's house and
put the bridge's cells back when the leader steps off it.

Stepping on or walking into a cell runs the first step trigger that answers
its number and whose condition holds, or failing one the first such exit;
pressing A facing it runs the first such examine trigger.

`Scene_RunStepTriggerOrExit` runs the stepping, and the numbers a scene owns
are `CELL_BEHAVIOR_EVENT_FIRST` to `CELL_BEHAVIOR_EVENT_LAST`; the numbers
above them mean the same in every scene ([Maps](../map/README.md#cells)).

## How a trigger is chosen

![Isaac talks to the mayor of Vault in his house, who welcomes him back](https://alchemy.pascalpixel.com/places/vault-interiors-revisit/events/Event_WelcomeIsaacBack.gif)

When the player talks to someone, the game reads the scene's trigger table from
the top and runs the first row that fits. That one rule is how a person says
something new as the story moves on. A scene lists a person's rows in story
order, each holding until its flag is set, with one that always holds last. In
Vault's houses after Kalay, the mayor has a row that holds until he has
welcomed Isaac back, and below it one that always holds, so the first talk
welcomes Isaac and every later one warns of the thieves
([`log_rooms_b.c`](log_rooms_b.c)):

```c
{ TRIGGER_TALK, ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT(LogRoomsB_WelcomeIsaacBack) },
{ TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsB_WarnOfThievesRevenge) },
```

A row behind one that always fits is never reached.

Searching something with a search target (a chest, a jar, a barrel...) is the
one exception, so that an opened chest still answers. The game finds the row
whatever its condition, shows the "checked the chest..." line, and only then
tests the condition: an unopened chest gives what it holds and is marked
opened, and an opened one adds "but the chest was empty." A mimic's row keeps
the ordinary rule, so an opened mimic chest no longer answers.

`Scene_FindTrigger` finds the row and `Field_ExamineCell` runs a
search. Rows these rules make unreachable keep a
[`// Unused:`](../../glossary.md#unused) tag in their table, and
[Unused](https://alchemy.pascalpixel.com/tbs/unused) describes every such
case.

## Events

![Isaac walks north into the door of Jenna's grandparents' house in Vale; the door opens, he walks in, and the room inside fades in](https://alchemy.pascalpixel.com/places/vale/events/Event_EnterJennasGrandparentsHouse.gif)

An [event](../../glossary.md#event) is written as a list of
[event commands](../../glossary.md#event-command), one to a line, and the game
plays it from top to bottom while the party waits. The door in the recording
is one of the shortest: it plays the door's sound, opens the door, starts the
leader walking in and asks for the exit into the house.

```c
static void Vale_EnterJennasGrandparentsHouse(void) {
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gVale_GrandparentsDoorOpens, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Import_EventSetExitRequest(VALE_TO_COTTAGE_ROOMS_A_NORTH);
}
```

A command that takes time, such as opening the door, a line of dialogue or a
wait, holds the event until it is done; one that takes no time, such as
starting a walk, lets the next line run in the same
[frame](../../glossary.md#frame). Whatever has to keep moving during an
event runs on its own: as an actor script (the commands someone runs each
frame), a [task](../../glossary.md#task) (a function run once a frame, added
with `Task_AddFrame`) or an update routine (a function one actor runs each
frame). A request the event raises, like this exit, waits until the event has
finished.

An event is plain C written in the calls of `event.h`, `event_inline.h` and
`system_call.h`. A command that takes time lets frames pass before it
returns: `Event_Wait` calls `wait()`, which runs one frame after another
([System](../../system.md#waiting-and-tasks) explains it).

## State that cleans itself up

Two ranges of game flags are cleared for scenes as the party moves on, so a
scene needs no code to put things back.

Scene temp flags last until the party next enters a scene, so a pushed stone
pillar or an opened door kept in one is back where it started the next time
the party comes in. [Area](../../glossary.md#area) temp flags last while the
party stays anywhere in the area, so they hold what must survive moving
between its scenes, as Fuchin Falls Cave keeps where its logs float
(`FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST` and the rest, in
[`imil_fuchin_falls.c`](imil_fuchin_falls.c)). Coming back from a battle
clears neither. Because the numbers are reused everywhere, each scene names
the ones it uses for what they hold there, in its own file
(`FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN` in Crossbone Isle,
`FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK` in Vault's houses).

`Scene_Arrive` clears them, and the flags are in `games/tbs/include/flags.h`;
[Game rules](../game/README.md#the-game-flags) describes the game flags as a
whole.

| Name | Value | Meaning |
| --- | --- | --- |
| `FLAG_SCENE_TEMP_00` to `FLAG_SCENE_TEMP_FF` | `0x200` to `0x2ff` | cleared on every ordinary arrival in a scene |
| `FLAG_AREA_TEMP_00` to `FLAG_AREA_TEMP_FF` | `0x300` to `0x3ff` | cleared on entering another area |

## The Title, the File Screen and the Battle Arena

Three scenes are screens rather than places, each a row of the scene table
with an overlay of its own: the Title ([`title.c`](title.c): the start screen,
and the logo shown after the ending), the File Screen
([`file_screen.c`](file_screen.c): new game, continue, copy and delete, the
door to the Battle Arena, and the transfer of a save's data by link or as a
password) and the Battle Arena ([`battle_arena.c`](battle_arena.c)). Their
screens are drawn by the window code
([Windows](../window/README.md)), and the title intro and the credits the
Title starts are in [Menus](../menu/README.md#the-title-intro).

## Code shared between overlays

Code several scenes need is written once in `shared/` and compiled into each
overlay that uses it ([System](../../system.md#overlays) explains why): the
three Colosso finals share the friends' cheering spots, the stage guides, the
race gauge and the opponent's recorded run; Vault and its houses share the
walking villagers; Mercury Lighthouse's scenes share the pool's lights and the
passages between floors; and many scenes share the small field effects and
the helpers that move actors. The ROM holds a copy in each overlay, alike but
for the addresses they hold.

A header in `shared/` is code, not declarations: each scene includes it where
the code lies in that overlay, since the include decides where its bytes land,
and including it twice in one scene would place it twice. A helper that some of
the scenes including its header never call is `UNUSED`, so that a current GCC
does not warn there. Renaming a function in `shared/` renames it in every scene
that includes the header; each header's opening comment says what it holds.

## Design

Each overlay is three objects, linked in this order into the overlay area:
`entry/<scene>`, `<scene>` and `import/<scene>`. Where an overlay's RAM
variables are still written in assembly, they are a fourth object,
`bss/<scene>.s`, on the overlay's `bss` row of `objects.tsv`. The scene table, `gScenes` in
[`field/scenes.s`](../field/scenes.s), gives each scene its overlay,
area, scene type and map, and names it `SceneId_<Scene>`; several scenes may
share one overlay, and then share its file. A scene overlay is never split into
parts, because its code is laid out by kind (entry points, then handlers,
then tables), not by scene.

Indoor rooms are packed several to a map, a
[sheet of rooms](../../glossary.md#sheet-of-rooms): the rooms drawn in
one tile style sit side by side on one map, each with a camera box of its
own, and the entrance the party arrives at chooses the room by its
box. The rooms of one sheet can belong to different places, so its area name
follows the entrance too; Vale's inn and the scholars' room in Babi's
Palace share `SceneId_CottageRoomsB`. A sheet is therefore named for the style
its rooms are drawn in, never for a place, and its file lists its rooms and the
places they belong to.

A scene's functions, its events among them, take its name (`Vault_`,
`VaultRevisit_`, `ColosseumNavampaMatch_`) and say what the player sees.
Shared helpers take their family's prefix (`Colosso_`, `MercuryLighthouse_`,
`KolimaPalette_`, `PushedActor_`, and `FieldEffect_` for the field effects),
and some keep older descriptive names (`Scene_`,
`Actor_SetFlagBitByRelativeDepth`). A run of functions inside one overlay that
is about one thing takes that thing's name, and a party member's or the
leader's part in an event takes that character's (`Ivan_StepOnDeck`,
`Leader_StopMoving`). The spellings some sites keep to match (fields reached
through `offsetof`, another struct's view of an actor, update routines
returning `s32`) are in [Matching](../MATCHING.md).

## How it fits with the rest of the game

`MainRoutine_Run` unpacks the overlay into `gOverlayArea`
(`Scene_LoadOverlay`) and `FieldSequence_Run` calls its entries;
[System](../../system.md#overlays) describes the entry and import tables and
how the overlay is loaded. The scene table and how scenes depend on the story
are in [Field](../../field.md#scenes-and-areas). The maps the scenes are drawn
on are in [Maps](../map/README.md), and the debug scenes, built the same way,
in [Debug scenes](../debug/README.md).

To find the code for a scene, find its row in `field/scenes.s`: the
overlay's resource number begins the rows of `objects.tsv` that name its
files. To find what someone says, search `games/tbs/text/en.po` for the line;
its `msgctxt` is the name the scene writes (`MsgVaultWelcomeToVaultTheTownAt`).

## Limits

- An overlay's code, tables and RAM variables must fit in `gOverlayArea`,
  32 KB of [EWRAM](../../glossary.md#iwram-and-ewram).
- An overlay calls the [main image](../../glossary.md#main-image) only through
  its import table, never another overlay, so code two scenes need is copied
  into each.
- 66 actor ids (`ACTOR_ID_COUNT`): the party's 8, then the placed actors.
- 256 scene temp flags and 256 area temp flags, reused by every scene and
  area.

## Words used here

- position: pixels, x east, y up and z south; the field keeps them in
  16.16 fixed point (`PIXELS(n)`), and a cell is 16 by 16 pixels.
- facing: `enum Facing`, eight headings in 1/65536 turns from `FACING_EAST`
  through `FACING_SOUTH` to `FACING_NORTHEAST`.
- condition: a table row's test ([Conditions](#conditions)).
- scene temp flags and area temp flags: `FLAG_SCENE_TEMP_xx` and
  `FLAG_AREA_TEMP_xx`, the two ranges cleared for scene code
  ([above](#state-that-cleans-itself-up)).
- messages: each line of game text has a name (its `msgctxt` in
  `games/tbs/text/en.po`, such as `MsgVaultWelcomeToVaultTheTownAt`);
  `MSGNO(name)` is its number. `Event_ShowMessageAndWait` chooses the next
  message after showing one, so a line that follows a named one has no name
  of its own.
- update routine: an actor's `update`, run each frame with the actor.

## Evidence

A scene's name is its area name from `gArea_NameRules`, or for a
room the game leaves unnamed, what the player sees there. A sheet of rooms
is named for the style its rooms are drawn in, in a word from building and
furnishing rather than a place (`CottageRoomsA`, `LogRoomsC`, `StuccoRoomsB`,
`StoneRooms`, `ShipRooms`), with a letter in scene-table order where several
share a style. A scene's functions are named for what the player sees; its
messages are named from their English text. Overlay numbers are their rows of
the resource directory.

The stairs exits of `enum ExitStyle` play the leader's leaving and arrival
animations, named in `enum ActorAnimation` from Isaac's sprite
(`spr_000.tsv`, animations 23 to 26): an arrival grows from a small figure
seen side-on to full size facing the player, and a leaving turns from his
back to side-on as he shrinks and is then hidden. The alternate pair, which
goes with `SE_STAIRS_DOWN`, has its own drawings of the same turns.
