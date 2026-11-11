# Scenes

![Isaac talks to his mother Dora outside their house in Vale: she turns to him, says `It looks like it won't rain for some time... so I guess I'll just have you fix it later.`, and turns back the way she faced](https://alchemy.pascalpixel.com/places/vale/events/Event_DoraFretsAboutTheRoof.gif)

Walking around Vale, the player meets what is particular to Vale. Dora
stands outside her house with something to say about the roof. A door opens
into the house of Jenna's grandparents. There is a jar with coins in it,
and a pillar to push.

The code that knows all this is Vale's own, and every place in the game has
code of its own in the same way. A place is a
[scene](../../glossary.md#scene), one row of the scene table. Its code is an
[overlay](../../glossary.md#overlay), unpacked into memory when the
[party](../../glossary.md#the-party) arrives and replaced by the next scene's
when it leaves.

The [field sequence](../../glossary.md#field-sequence), which runs the walk,
knows only how to ask a scene about itself. The scene's answers are what the
player sees there, and they come in the order the field asks for them:

1. The [entry table](#the-entry-table) is the six questions the field asks
   every scene, and when it asks them.
2. The [tables](#the-tables) that answer them say where the party arrives,
   who stands where, where it can leave, and what each thing the player does
   starts.
3. [How a trigger is chosen](#how-a-trigger-is-chosen) from those tables
   when the player talks, steps or searches. A
   [trigger](../../glossary.md#trigger) is one row of them.
4. [Events](#events) are the code a trigger runs. An
   [event](../../glossary.md#event) is the C routine itself.
5. The [flags that clean themselves up](#state-that-cleans-itself-up), which
   let a scene keep notes without putting anything back, and how a scene
   [draws its room to match](#drawing-the-room-to-match) them.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  field(["🏡 The field sequence"]) -- "asks, through the entry table" --> overlay(["The scene's overlay"])
  overlay -- "answers with" --> tables(["Its tables: entrances, exits, actor placements, triggers"])
  tables -- "a trigger row names" --> event(["An event: code that waits"])
  event -- "sets" --> flags(["🚩 Game flags, which the tables read"])
  classDef sequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef overlay fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef data fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  class field sequence
  class overlay,event overlay
  class tables,flags data
```

Every scene has a page of its own under
[Places](https://alchemy.pascalpixel.com/tbs/places), in
[story order](../../glossary.md#the-places-in-story-order), with its map,
its triggers and a recording of each event.

## The entry table

![Isaac climbs the steps to the Sanctum's door in Vale: Vale fades out, and the Sanctum's hall fades in with Isaac just inside its door](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

When Isaac walks from Vale into the Sanctum, the field asks the Sanctum's
overlay where the party arrives and who stands there. Later, as he leaves,
it asks where the door he took leads.

Those questions come from the entry table, the fixed set of six questions
the field asks a scene. Each is answered by a function of the scene's own,
so the field can run any scene without knowing anything about it.

Here are the six, in the order the field asks them while a scene runs:

| Question | The scene answers with | When the field asks |
| --- | --- | --- |
| What can happen here? | its trigger table | as the scene starts, before anything else |
| Where does the party arrive? | its entrance table | as the scene starts, for the entrance the party came in by |
| Who stands where? | its actor placement table | as the scene starts, once the party is placed |
| Is there anything to set up? | its setup function | as the scene starts, last of all |
| Has the leader stepped into a box on the ground that leads out? | its exit region table | at every step onto a marked cell, while the party walks |
| Where does this exit lead? | its exit table | as the scene ends, with the exit the party took |

Each answer comes from a function, so a scene can answer differently as the
story moves on. Vale answers "who stands where" with one table before the
party has set out, another after it has left and a third once it has
reached Kalay, each chosen by a [game flag](../../glossary.md#game-flag).
Vault answers "what can happen here" with one table before its thieves are
jailed and another after.

The setup function is where a scene does whatever its tables cannot. It
restores what the flags say has happened, so a pushed pillar stands where it
was left. It starts work of its own that runs every frame, plays an arrival
event, or sends the party straight on to another scene.

A setup that sends the party on leaves an exit request behind, and the field
ends the scene before it has faded in.

When the party comes back to where it stood, as after a battle, the scene's
own setup decides what to skip. The field tells it so through
`FLAG_KEEP_PARTY_POSITION`, which stays set until the setup has run.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  load(["The overlay is unpacked into the overlay area"]) --> triggers(["triggers(): what can happen here"])
  triggers --> entrances(["entrances(): the party is placed"])
  entrances --> placements(["actorPlacements(): the people are placed"])
  placements --> init(["initialize(): the scene's own setup"])
  init --> walk(["🏡 The walk: regions() at each marked step"])
  walk -- "the party takes an exit" --> exits(["exits(): where it leads"])
  classDef overlay fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef data fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  class load overlay
  class triggers,entrances,placements,init,exits data
  class walk sequence
```

The six functions sit at the very start of every overlay, always in the
same order (`struct SceneEntryPoints`), each behind an 8-byte
[veneer](../../glossary.md#veneer). So the field finds each one at a fixed
place, whatever the overlay holds. Only the order is fixed, and a scene
names its six functions as it likes.

[System code](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
explains the overlay's other table, the imports through which the scene calls
the main image back.

## The tables

The answers are tables, one row to a thing: an entrance the party can arrive
at, an exit it can leave by, a person standing somewhere, a box that takes
the leader out, and a trigger for each thing the player can do.

The field is drawn in three dimensions. x runs east, z runs south across the
ground, and y is height. So a position in these tables is three numbers in
pixels, and a facing is one of eight headings
([Words used here](#words-used-here) gives the units).

Every table ends with an end row, and the rows are declared in
`scene_tables.h`.

### Conditions

Most rows carry a condition, which says whether the row holds now.

The usual case is "until this has happened". The condition names a game
flag, and the row holds while that flag is clear. A row can also hold only
while a flag is set, or always.

| Condition | The row holds |
| --- | --- |
| `CONDITION_ALWAYS` | always |
| a game flag, such as `FLAG_VALE_NUT_CAUGHT` | while the flag is clear |
| `IF_FLAG_SET(flag)` | while the flag is set |
| `CONDITION_IN_VIEW`, in actor placements only | while the actor stands inside the camera's box, on a sheet of rooms ([Design](#design)) |

### Entrances

An entrance is where the party appears and which way it faces. It is named
for where the party comes from, so `VALE_FROM_SANCTUM` is Vale's doorstep of
the Sanctum.

An entrance row holds the entrance's number, a position, a facing, and a box
the camera may move in. A room on a shared map uses that box to show only
itself ([Design](#design) explains the sharing).

A row can also require a flag to be set. Then a second row for the same
number can place the party elsewhere once the story has moved on. The party
arrives at the first row for its entrance whose flag is set, or at the
table's first row when none matches.

The camera box is read even when the party keeps its position after a
battle, so the view stays the room's. A camera box smaller than the screen
is widened to it, so the view is always at least 240 by 160 pixels.

`scene_exits.h` names every entrance and every exit.

### Exits

An exit is a way out of a scene, named for where it leads.
`SOL_ENTRANCE_TO_VALE` is the path down from Sol Sanctum's entrance.

The exit table turns the exit into the scene and entrance that run next. A
row can hold only until a flag is set, and the first row that holds wins.
The front page's [Sol Sanctum path](../../README.md#exits-and-entrances)
shows three rows for one exit, each holding until its flag is set.

A door is named the same from both sides, so each side names the other.
Vale's `VALE_TO_COTTAGE_ROOMS_A_NORTH` leads to
`COTTAGE_ROOMS_A_FROM_VALE_NORTH`, and that room's
`COTTAGE_ROOMS_A_TO_VALE_NORTH` leads back to
`VALE_FROM_COTTAGE_ROOMS_A_NORTH`.

An exit table opens with a header naming the scene whose exits follow. An
overlay that serves several scenes lists each scene's exits under its own
header in the one table. One kind of exit leads into a battle.

| Word | Meaning |
| --- | --- |
| `SCENE_EXITS(Scene)` | the header: the exits of this scene follow |
| `SCENE_EXIT(exit, Scene, entrance)` | this exit leads to that scene, at that entrance |
| `SCENE_EXIT_UNLESS(exit, Scene, entrance, flag)` | the same, until the flag is set |
| `EXIT_TO_BATTLE(exit, formation)` | this exit starts a battle against that [formation](../../glossary.md#formation) |
| `SCENE_EXITS_END` | the end of the table |

When a scene ends, the field sequence hands the main routine the exit the
party took. The main routine finds the scene's header in the table, and then
the first row of that exit whose flag is clear. A row of exit 0, or the end
of the scene's rows, sends the party to the first row's scene at its first
entrance.

An exit that is also how the leader leaves, with a sound and an animation up
or down stairs, is chosen by its trigger row
([below](#what-a-trigger-row-says)).

### Who stands where

![Vale's gate at three points in the story: on the stormy night, with villagers gathered in the rain; after the party has left, with the gate quiet; and after the party has reached Kalay](https://alchemy.pascalpixel.com/guides/front/vale-plaza-story.gif)

Each row of a scene's actor placement table puts one person or thing in the
scene, as an [actor](../../glossary.md#actor). The row gives its sprite, its
condition, how it moves, its position and facing, and how it turns when
spoken to. It can move by standing still, by wandering near its spot, or by
an [actor script](../../glossary.md#actor-script) of its own. The rows'
conditions read the story flags, so the field places only the rows that
hold.

Every row owns an [actor id](../../glossary.md#actor-id), whether or not its
actor appears. The party takes ids 0 to 7, and the rows take the ids from
`ACTOR_FIRST_PLACED` in table order. A row whose condition fails puts nobody
in the scene, and its id stays empty. That way every actor keeps one number
in its scene, and the scene's code can name it once.

On Mercury Lighthouse's upper floor the blue statue has one row for its
place beside a jet of water and another for where it lands after it falls.
Only one of the two appears.

A row whose sprite is one of the party's characters takes that character's
own id instead, so Garet in Vale after Kalay is `ACTOR_GARET` wherever his
row sits.

A placed actor stands on the ground under it, and a row with a height
floats at that height instead.

A Djinni's row holds while its caught flag is clear and, away from the World
Map, while its hiding flag is clear too. So a Djinni the party fled from
stays hidden until the scene is entered afresh.

Each scene names its actors in row order from `ACTOR_FIRST_PLACED`.
`ACTOR_PARTY_LEADER` is Isaac, the one the player moves everywhere but
Colosso. Up to four tables can be placed in one scene, since an event can
add a table of its own after the scene's.

### Exit regions

![In Kolima's tunnel Isaac walks down from the foot of a ladder, looks along the ledge and comes back, then climbs the ladder; near its top the screen fades to black and Kolima fades in, Isaac standing beside a great tree stump with a door.](https://alchemy.pascalpixel.com/guides/scene/exit-region.gif)

An exit region is a box that takes the leader out through an exit as soon
as the leader steps into it. It is for the few places where a map edge or a
wide opening is easier to draw as a box than to mark cell by cell.

A row holds the box's two corners, a condition and the exit it leads to. The
field tests the boxes at every step onto a marked cell, and the first box
that holds the leader's position takes its exit.

A box has a height as well. In Kolima's tunnel each one sits at the top of a
ladder, so climbing it is the way out.

Fourteen scenes have regions, Kolima's tunnel and the Tolbi ship's deck
among them. The rest answer with none.

### What a trigger row says

![Isaac walks into the door of his house in Vale after the party has left, and Dora, behind it, refuses to believe it is him: `My Isaac would keep his promise... Go away.`](https://alchemy.pascalpixel.com/places/vale/events/Event_EnterIsaacsHouse.gif)

A trigger is one row of the scene's trigger table, for one thing the player
can do there. The row says what starts it, whom or where it answers, when it
holds, and what runs.

Walking north into the doorstep of Isaac's house is one row. After the party
has left Vale, that row's event has Dora refuse to open the door.

Here are four rows of Vale's table on the day the story begins:

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

Talking to Dora runs an event. Walking north into the doorstep of Isaac's
house, a [step trigger](../../glossary.md#step-trigger), takes him in. Catch
on the nut works until the nut has been caught. And A facing the locked door,
an [examine trigger](../../glossary.md#examine-trigger), searches it.

The first column says what starts the row. That also settles what the second
column, the subject, names: an actor for talking, a cell for stepping and
searching, a number the scene's own code raises.

| Kind | Started by | The subject is |
| --- | --- | --- |
| `TRIGGER_TALK` | the A button on someone | that actor's id |
| `TRIGGER_TALK_HALTED` | talking to someone Halt has stopped | that actor's id |
| `TRIGGER_STEP` and its forms | stepping onto a cell, walking into it facing a way, or pushing against it | the cell's [cell behavior](../../glossary.md#cell-behavior) |
| `TRIGGER_EXIT` | stepping onto a cell no step trigger answers | the cell behavior; the value is the exit |
| `EXAMINE_TRIGGER(target)` | the A button on what is ahead, when it is nobody | the cell behavior |
| `USE_ITEM(item)`, `USE_ITEM_ON_ACTOR(item)` | using an item | a cell behavior or an actor |
| `PSYNERGY_ON_ACTOR(ability)`, `PSYNERGY_ANYWHERE(ability, phase)` | using [Psynergy](../../glossary.md#psynergy-and-pp) on someone, or anywhere | that actor, or anywhere |
| `TRIGGER_RAISED` | the scene's own code, or the menus while `FLAG_MENUS_RAISE_TRIGGER` is set | a number the code gives |

A step trigger's form says how firmly the leader must mean it. A plain
`TRIGGER_STEP` runs on the first step onto the cell. `STEP_TRIGGER_WALKING`
runs once the leader has pushed against it for more than 12
[frames](../../glossary.md#frame), or more than 2 in its quick form. The
`_FACING` and `_WALKING_INTO` forms run only while the leader faces within
an eighth of a turn of the heading the row names. That is how a doorstep
opens only to someone walking north into it.

A `TRIGGER_EXIT` row also says how the leader leaves: plainly with
`SE_MAP_EXIT`, silently, or up or down stairs with `SE_STAIRS_UP` or
`SE_STAIRS_DOWN` and an animation that the next scene's arrival answers
(`enum ExitStyle`).

An examine trigger's target, `EXAMINE_CHEST`, `EXAMINE_JAR` and the rest of
`enum ExamineTarget`, names the line the leader says as the search starts
(`Isaac checked the chest...`), and the matching line when nothing comes of
it.

A Psynergy row says when it runs around the Psynergy: before it plays, after
it, or from inside it as Force strikes, Frost freezes or Reveal shows what
is hidden (`enum PsynergyPhase`). Reveal's scenes pair that row with one
that hides everything again as the colours come back. A row can also run
when Psynergy that runs out by itself, such as Cloak, has ended.
[Field code](../field/README.md#how-field-psynergy-reaches-its-target)
follows a cast through these.

The last column is what runs: a message, an event, or for a search what is
found there. One word holds either a message or an event, because events sit
at addresses above any message number.

| Value | What runs |
| --- | --- |
| `MSGNO(name)` | that message, shown and waited for |
| `EVENT(function)` | that event |
| `FIND_ITEM(item)`, `FIND_COINS(n)`, `FIND_PSYNERGY_STONE` | a search finds that: the item goes to the first party member with room, the coins to the purse, the stone fills everyone's PP |
| `FIND_MESSAGE(message)` | a search shows that line |
| `FIND_MIMIC` | the chest is a mimic, and its battle starts |

The row is `struct Trigger`. Its kinds, their forms and the `FIND_` values
are in `scene_tables.h`.

### Which triggers a cell starts

![Isaac walks north into the foot of the ladder beside his house in Vale and climbs it onto the roof](https://alchemy.pascalpixel.com/places/vale/events/Event_ClimbLadderOntoRoof.gif)

Some spots in a scene do something when the leader reaches them. A doorstep
leads into a house, the edge of a village leads on to the next scene, the
foot of a ladder climbs to a roof, and a chest answers the A button.

The map marks each such spot with a number, the cell behavior of its
[cell](../../glossary.md#cell), and the scene's trigger table says what that
number does there. The same number means different things in different
scenes, since each scene reads it through its own table.

Only the numbers from `CELL_BEHAVIOR_EVENT_FIRST` to
`CELL_BEHAVIOR_EVENT_LAST` belong to the scene. The ones above them, walls,
ledges and the jars anyone can search, mean the same on every map
([Maps](../map/README.md#cells)).

Stepping onto or walking into a marked cell runs the first step trigger for
its number whose condition holds, or failing one the first such exit.
Pressing A facing it runs the first such examine trigger. The cell a press
of A searches is the one 16 pixels ahead of the leader.

In Vale on the day the story begins, each door is a step trigger. Walking
north into the doorstep runs an event that opens the door, walks the leader
in and only then asks for the exit. That is how Dora can refuse at Isaac's
door.

The ladder in the recording is a step trigger too. Walking north into its
foot runs the event that climbs the leader and changes the roof's cells so
that it can be walked on. Another step trigger puts the bridge's cells back
when the leader steps off it.

## How a trigger is chosen

![In Vault's mayor's house after Kalay, Isaac talks to the mayor twice: the first talk welcomes him back, `Oh, Isaac! You've come back to Vault!`, and the second warns him, `Those guys really wanted revenge on you, so take good care of yourselves.`](https://alchemy.pascalpixel.com/guides/map/talk-twice.gif)

When the player talks to someone, the game reads the scene's trigger table
from the top and runs the first row that fits: the right kind, the right
subject, a condition that holds.

That one rule is how a person says something new as the story moves on. A
scene lists a person's rows in story order, each holding until its flag is
set, with one that always holds last. The event a row runs sets the flag, so
the next talk falls through to the row below.

In Vault's houses after Kalay, the mayor has a row that holds until he has
welcomed Isaac back, and below it one that always holds. So the first talk
welcomes Isaac and every later one warns of the thieves
([`log_rooms_b.c`](log_rooms_b.c)):

```c
{ TRIGGER_TALK, ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT(LogRoomsB_WelcomeIsaacBack) },
{ TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsB_WarnOfThievesRevenge) },
```

A row behind one that always fits is unreachable. Rows these rules make
unreachable keep a [`// Unused:`](../../glossary.md#unused) tag in their
table, and [Unused](https://alchemy.pascalpixel.com/tbs/unused) describes
every such case.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  press(["A is pressed facing the mayor: a request"]) --> find(["🏡 The field sequence reads the trigger table from the top"])
  find --> first{"The first talk row for the mayor whose condition holds"}
  first -- "FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK clear" --> welcome(["The welcome event, which sets 🚩 the flag"])
  first -- "the flag set" --> warn(["The warning event, which always holds"])
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef sequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef overlay fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  class press,first request
  class find sequence
  class welcome,warn overlay
```

### Talking

Pressing A with someone ahead is a talk only when that person has a talk
row. With a person who has none, the A press goes on to search the cell
ahead, or to open the field menu.

Once the row is found, the person stops. Unless the row's placement says to
keep its heading, the leader stops too and the two turn to face each other.
Then the row's message is shown, or its event runs. Afterwards a person
whose placement says so turns back to its old heading, or keeps turning to
watch the leader.

A person Halt has stopped answers with a `TRIGGER_TALK_HALTED` row where the
scene has one. Otherwise a talk row that is a message is still shown, and
one that is an event gives one of two lines of dots instead.

### Searching

![Isaac checks the chest in Sol Sanctum: `Isaac checked the chest...`, the Small Jewel rises from it with the treasure jingle, and `Isaac got a Small Jewel.`](https://alchemy.pascalpixel.com/guides/front/chest.gif)

An opened chest still answers. Searching something with a target, a chest, a
jar, a bookcase, is the one case where a row is found whatever its
condition, and that is why.

The game finds the row, shows the `checked the chest...` line, and only then
tests the condition. An unopened chest gives what it holds and sets its
flag, and an opened one adds `but the chest was empty.`

A mimic's row keeps the ordinary rule, so an opened mimic chest stops
answering. The jars, barrels, wooden boxes, graves, books and wells that
every scene shares have no row: their two lines are the same everywhere.

What a chest holds decides what happens next. An item stays in the chest
when no party member can carry it, and the chest closes again. A Psynergy
Stone restores every party member's PP and is gone once taken. A mimic's
chest jumps up and the battle starts, and the chest is gone once the mimic
is beaten.

### Stepping and raising

Stepping onto or walking into a marked cell runs its step trigger, or takes
its exit when the scene has none for that cell. The exit's sound plays, and
the scene ends with that exit.

A scene's own code can raise a trigger by number, as a stop in a chase
raises the row that reads the cornered suspect's mind. The menus raise one
too while `FLAG_MENUS_RAISE_TRIGGER` is set, so that a scene can answer a
menu press with a refusal instead.

## Events

![In Vault's inn, Isaac talks to the innkeeper from beside the counter: `Everyone thinks our guests are the thieves, but I think it's unfair to accuse them without any evidence.`, and on the next page `Sure, they look suspicious, but they've been quite polite. And they paid cash!`](https://alchemy.pascalpixel.com/places/log-rooms-a/events/LogRoomsA_TalkToInnkeeper.gif)

An event is the code a trigger row runs: a conversation that does more than
show one line, or a cutscene. It is written as a list of
[event commands](../../glossary.md#event-command), one to a line, which the
game plays from top to bottom while the party waits.

The door of Jenna's grandparents' house is one of the shortest
([vale.c](vale.c)). It plays the door's sound, opens the door, starts the
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
wait, holds the event until it is done. One that takes no time, such as
starting a walk, lets the next line run in the same frame. That is why the
leader is still walking when the exit is asked for.

A request the event raises, like this exit, waits until the event has
returned, as the front page's [Vault thieves](../../README.md#the-event)
wait for their battle.

While an event runs it is standing still at one command, so whatever has to
keep moving moves on its own. It can do that in three ways:

- as an actor script, the commands an actor runs each frame;
- as a [task](../../glossary.md#task), a function run once a frame, added
  with `Task_AddFrame`;
- as an update routine, a function one actor runs each frame.

The thieves climb the ladder on their actor scripts while the event waits
for them.

Most events open with `Event_Start`, which hides the
[area](../../glossary.md#area)'s name, stops the leader and resets the
message state, and close with `Event_End`. The door above runs without
either. The innkeeper's talk in the recording is the other usual shape:
`Event_Start`, a message chosen by a flag, shown and waited for,
`Event_End`.

An event is plain C written in the calls of `event.h`, and a command that
takes time lets frames pass before it returns. `Event_Wait` calls `wait()`,
which runs one frame after another
([System](../../system.md#waiting-and-tasks) explains it).

## State that cleans itself up

![In Kolima Forest Isaac walks into a log on the river bank; it rolls, tips into the river and floats there. He runs to and fro until a battle with three Skeletons opens; after its end the forest comes back with the log still in the river, and Isaac crosses it and walks away. The screen fades out and in, and on his return the river is empty and the log stands on the far bank again.](https://alchemy.pascalpixel.com/guides/scene/log-state.gif)

Two ranges of game flags are cleared for scenes as the party moves on, so a
scene can keep a note without any code to put things back.

Scene temp flags last until the party next enters a scene. Kolima Forest
keeps the log the party tips into its river in one, so the log is back on
the bank the next time the party comes in.

Area temp flags last while the party stays anywhere in the area, so they
hold what must survive moving between its scenes. That is how Fuchin Falls
Cave keeps where its logs float.

Coming back from a battle clears neither, since the party is returning to
where it stood. The log is still in the river after one.

Because the numbers are reused everywhere, each scene names the ones it
uses for what they hold there, beside its includes:

```c
#define FLAG_VALE_ON_ROOF FLAG_SCENE_TEMP_04
#define FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST FLAG_AREA_TEMP_01
```

The main routine clears them before each scene, unless the party is coming
back to where it stood. [Game rules](../game/README.md#the-game-flags)
describes the game flags as a whole.

| Flags | How many | Cleared |
| --- | --- | --- |
| `FLAG_SCENE_TEMP_00` to `FLAG_SCENE_TEMP_FF` | 256 | on every ordinary arrival in a scene |
| `FLAG_AREA_TEMP_00` to `FLAG_AREA_TEMP_FF` | 256 | on entering another area |

### Drawing the room to match

A flag only remembers that something happened. The room is drawn fresh from
its map every time the party arrives, so a scene must change the map to
match what its flags say, and it does that by copying cells. A map's grid is
far larger than most rooms, and its unused corners hold spare cells: the
same patch of the room in its other states.

In Kalay's tunnel ([`kalay.c`](kalay.c)) a pool lies under a waterfall, and
the map keeps its floor a second time, drained. When the party pushes a
statue into the arch the water falls from, the event copies the drained
floor over the pool, a little at a time with waits between, so the water is
seen to sink, and sets a flag. The next time the party arrives, the scene's
setup finds the flag set and makes the same copy at once. The pool is
already dry, and nothing had to be saved but the flag.

A copy can carry a cell's picture, its ground, or both. Copying the ground
alone changes where the party can walk and leaves the picture as it was. In
Mercury Lighthouse ([`mercury_lighthouse.c`](mercury_lighthouse.c)) a statue
stands in front of a door; once it is moved aside, one cell near it is given
the ground of a cell the leader can jump from, and from then on a jump
reaches the door.

The three layers of a map are windows onto the one grid
([Maps](../map/README.md#one-grid-three-layers)), so a copy reaches a layer
by where it writes. In Sol Sanctum's Elemental Star Room
([`sol_sanctum_elemental_star_room.c`](sol_sanctum_elemental_star_room.c))
taking a star raises three pillars out of the dark as stepping stones: each
pillar's picture is copied onto the overhead layer, at the pillar's own cell
plus the place where that layer's window starts.

## The Title, the File Screen and the Battle Arena

Three scenes are screens. Each is a row of the scene table with an overlay
of its own, run by the field sequence like any other.

- The Title ([`title.c`](title.c)) is the start screen, and the logo shown
  after the ending.
- The File Screen ([`file_screen.c`](file_screen.c)) is new game, continue,
  copy and delete, the door to the Battle Arena, and the transfer of a
  save's data by link or as a password.
- The Battle Arena ([`battle_arena.c`](battle_arena.c)) is the third.

Their screens are drawn by the window code
([Windows](../window/README.md)), and the title intro and the credits the
Title starts are in [Menus](../menu/README.md#the-title-intro).

The World Map is a place, but the only scene of its overlay
([`world_map.c`](world_map.c)), and the one drawn in perspective
([Field](../../field.md#the-world-map)).

## Code shared between overlays

Code several scenes need is written once in `shared/` and compiled into each
overlay that uses it ([System](../../system.md#overlays) explains why).

The three Colosso finals share the friends' cheering spots, the stage
guides, the race gauge and the opponent's recorded run. Vault and its houses
share the walking villagers. Mercury Lighthouse's scenes share the pool's
lights and the passages between floors. And many scenes share the small
field effects and the helpers that move actors.

The ROM holds a copy in each overlay, alike but for the addresses they hold.

## From Vale's doorstep into the house

![Isaac walks north into the door of Jenna's grandparents' house in Vale; the door opens, he walks in, and the room inside fades in](https://alchemy.pascalpixel.com/places/vale/events/Event_EnterJennasGrandparentsHouse.gif)

Isaac walks north into the doorstep of Jenna's grandparents' house, the door
opens, and the room inside fades in with him just inside it. Each step of
that is one of the mechanisms above.

### The step

The field sequence is in its walk, and in each frame it looks at the cell
under the leader. The doorstep's cell carries the number Vale's table gives
the grandparents' door, within the scene's own range, so the walk notes a
step request for it and stops.

The field answers the request by reading Vale's trigger table from the top
for the first step row with that subject whose condition holds. Vale's table
has this one:

```c
{ STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Vale_EnterJennasGrandparentsHouse) },
```

The form asks for more than a step. The leader must have pushed against the
doorstep for more than 12 frames and be facing north. Isaac is, so the row
fits. Its value is an event, which the field calls.

### The event

`Vale_EnterJennasGrandparentsHouse` runs its four lines. The door's sound
plays. The cell animation opens the door and holds the event until it has.
Isaac starts walking to a spot inside the doorway, which takes no time. And
the last line asks for the exit `VALE_TO_COTTAGE_ROOMS_A_NORTH`.

That is a request, so it waits in the field's work until the event has
returned. When it has, the field finds the exit request first of all, since
leaving comes before anything else. It ends the scene and hands the exit to
the main routine.

### The exit

The main routine asks Vale's overlay for its exit table and finds, under
`SCENE_EXITS(Vale)`, this row:

```c
SCENE_EXIT(VALE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
```

So what runs next is `SceneId_CottageRoomsA` at the entrance
`COTTAGE_ROOMS_A_FROM_VALE_NORTH`. The main routine clears Vale's scene
temp flags, keeps the area temp flags, since the houses are in Vale's area,
and unpacks the cottage rooms' overlay over Vale's.

### The room

The field sequence starts again and asks the new overlay its questions.
Its trigger table comes first.

Then the entrance table gives the row for
`COTTAGE_ROOMS_A_FROM_VALE_NORTH`. Isaac is placed at (337, 0, 199) facing
north, and the camera is boxed in from (252, 4) to (548, 236), which is the
one room of the sheet the house is drawn on.

The overlay chooses its actor placement table by the story flags and marks
each row as in or out of that box, so only the grandparents' room is
peopled.

Last, the scene's setup runs, and once the party has left Vale it lays the
grandmother in her sickbed for this entrance.

The scene fades in.

## Design

The scene table, `gScenes` in [`field/scenes.s`](../field/scenes.s), gives
each scene its overlay, area, scene type and map, and names it
`SceneId_<Scene>`. Several scenes may share one overlay, and then they share
its file.

The four are chosen independently. The Bilibin Barricade runs on Bilibin's
overlay but is an area of its own, and Vault before and after the thieves is
two scenes drawn on one map.

An overlay is one file, laid out by kind: its entry points, then every
function, then every scene's tables.

Rooms are packed several to a map, a
[sheet of rooms](../../glossary.md#sheet-of-rooms). The rooms drawn in one
tile style sit side by side on one map, each with a camera box of its own,
and the entrance the party arrives at chooses the room by its box.

The rooms of one sheet can belong to different places, so its area name
follows the entrance too. Vale's inn and the scholars' room in Babi's Palace
share `SceneId_CottageRoomsB`. A sheet is therefore named for the style its
rooms are drawn in, and its file lists its rooms and the places they belong
to.

A sheet's actor placements use `CONDITION_IN_VIEW`, so that the field
peoples only the room the camera's box holds.

A scene's functions, its events among them, take its name (`Vault_`,
`VaultRevisit_`) and say what the player sees there. Shared helpers take
their family's (`Colosso_`, `MercuryLighthouse_`, and `FieldEffect_` for the
field effects).

## How it fits with the rest of the game

The main routine unpacks a scene's overlay and the field sequence calls its
entries.
[System code](../system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes the entry and import tables and how the overlay is loaded.

The scene table and how scenes depend on the story are in
[Field](../../field.md#scenes-and-areas). The requests the field answers
between a trigger and its event are in
[Field code](../field/README.md#the-field-sequence-serves-requests). The maps
the scenes are drawn on are in [Maps](../map/README.md), and the debug
scenes, built the same way, in [Debug scenes](../debug/README.md).

To find what someone says, search `games/tbs/text/en.po` for the line; its
`msgctxt` is the name the scene writes (`MsgVaultWelcomeToVaultTheTownAt`).

## Limits

- An overlay's code, tables and RAM variables must fit in `gOverlayArea`,
  32 KB of [EWRAM](../../glossary.md#iwram-and-ewram).
- An overlay calls the [main image](../../glossary.md#main-image) only through
  its import table, so code two scenes need is copied into each.
- 66 actor ids (`ACTOR_ID_COUNT`): the party's 8, then the placed actors.
- Four actor placement tables in one scene (`FieldWork.tables`).
- 256 scene temp flags and 256 area temp flags, reused by every scene and
  area.

## Words used here

- position: pixels, x east, y up and z south, written `PIXELS(n)`; a cell
  is 16 by 16 pixels.
- facing: `enum Facing`, eight headings an eighth of a turn apart, from
  `FACING_EAST` through `FACING_SOUTH` to `FACING_NORTHEAST`.
- condition: a table row's test ([Conditions](#conditions)).
- camera box: the corners an entrance row gives, within which the camera
  moves; a room on a sheet of rooms is the box its entrance gives.
- scene temp flags and area temp flags: `FLAG_SCENE_TEMP_xx` and
  `FLAG_AREA_TEMP_xx`, the two ranges cleared for scene code
  ([above](#state-that-cleans-itself-up)).
- messages: each line of game text has a name, and `MSGNO(name)` is its
  number. Showing a message chooses the one after it as the next, so a line
  that follows a named one has no name of its own.
- update routine: an actor's `update`, run each frame with the actor.

## Evidence

A scene's name is its area name from `gArea_NameRules`, or for a room the
game leaves unnamed, what the player sees there.

A sheet of rooms is named for the style its rooms are drawn in, in a word
from building and furnishing (`CottageRoomsA`, `LogRoomsC`, `StuccoRoomsB`,
`StoneRooms`, `ShipRooms`), with a letter in scene-table order where several
share a style.

A scene's functions are named for what the player sees, and its messages are
named from their English text. Overlay numbers are their rows of the
resource directory.

The stairs exits of `enum ExitStyle` play the leader's leaving and arrival
animations, named in `enum ActorAnimation` from Isaac's sprite
(`spr_000.tsv`, animations 23 to 26).
