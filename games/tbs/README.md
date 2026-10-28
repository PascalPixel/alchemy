# The Broken Seal

In Japan, Golden Sun carries the subtitle 開かれし封印, The Broken Seal. These
pages call the game by that subtitle, to keep it apart from
[The Lost Age](glossary.md#the-two-games).

## The main routine

At power-on `System_Initialize` sets up the hardware, the interrupt handlers,
the sound and the task table, and then calls `MainRoutine_RunFar`. The
[main routine](glossary.md#main-routine), `MainRoutine_Run`, never returns:
everything the game does outside its interrupts runs under it.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  loop(["Main routine"]) --> scene(["Field sequence: a scene"]) & battle(["Battle sequence"]) & dice(["Dice sequence: Lucky Dice"]) & wheels(["Slots sequence: Lucky Wheels"])
  scene & battle & dice & wheels --> back(["Next sequence"])
  back --> loop
  classDef mainRoutine fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  class loop mainRoutine
  class scene,battle,dice,wheels sequence
  class back request
```

The main routine is a loop, and each pass runs one
[sequence](glossary.md#sequence) from its start to its end. A pass begins by
tearing down whatever the last sequence left behind: its
[tasks](glossary.md#task) (`Task_ResetAll`), its HBlank and VCount interrupt
handlers (`Irq_SetHandler`) and every block it took from the
[heap](glossary.md#heap-and-pools) (`Heap_Initialize`). Then it reads one id,
`gMainRoutineWork.sceneId`, from the [main routine's
work](glossary.md#main-routines-work), which every sequence reads and writes.

Most ids are [scenes](glossary.md#scene). A scene is a row of the scene table: a
place, such as a room of Vault's inn, or a screen, such as the Title or the File
Screen. Camelot labels each row with a scene code (`PLOGV00` is Vale on the
stormy night of the prologue) and gives each scene an area name. For a scene, the main
routine clears the [flags](glossary.md#game-flag) the last scene kept for itself
(`Scene_Arrive`), starts the music, unpacks the scene's code, its
[overlay](glossary.md#overlay), into memory (`Scene_LoadOverlay`) and runs the
[field sequence](glossary.md#field-sequence), `FieldSequence_Run`. When the
party leaves, the field sequence returns the [exit](glossary.md#exit) it took,
and `Scene_TakeExit` turns that into the next scene and entrance.

The other ids are the other sequences: `SEQUENCE_BATTLE` runs a battle
(`Battle_RunFar`), `SEQUENCE_DICE` Lucky Dice and `SEQUENCE_SLOTS` Lucky
Wheels. Each returns a result, and `Scene_ReturnFromSequence` turns the result
into the scene to go back to.

Going from one scene to the next is a
[transition](glossary.md#transition-and-next-sequence) through the main
routine, just as going into a battle is: the last sequence writes the next id
and returns, and the next pass tears down and starts it
([System](system.md#the-main-routine)).

## Frames, waiting and tasks

Waiting and the task table belong to the System and work the same under every
sequence. Every sequence is straight-line code: it does one thing, then the
next, and where time must pass it calls [`wait(n)`](glossary.md#wait), which
returns to the next line once n [frames](glossary.md#frame) have run.
Simplified, from [frame.c](src/system/system/frame.c):

```c
void wait(s32 frames) {
  while (frames--) {
    Task_RunPass(TASK_PASS_FRAME);  /* every frame-pass task, once */
    Oam_BuildBuffer(...);           /* this frame's sprites */
    VBlankIntrWait();               /* halt until the next VBlank */
  }
}
```

A task is a function in the task table, `gTasks`, which the frame pass calls
once a frame until it is removed. The tasks are what keeps moving while the code
in charge waits: the [actors](glossary.md#actor), the map's scrolling, the
animated tiles, the open windows. Each [VBlank](glossary.md#vblank), about 60
times a second, the VBlank interrupt mixes the next slice of sound, reads the
buttons and runs the tasks of the table's other pass, `TASK_PASS_VBLANK`,
whatever the game is doing at that moment.

When a frame's work overruns one VBlank, everything counted in frames runs
slow, while the music, mixed at every VBlank, keeps its tempo: that is
[slowdown](glossary.md#slowdown) ([System](system.md#slowdown)).

Each sequence adds its own tasks when it starts, and the transition removes
them. The field's come through its actors, its map and its windows
(`Actor_UpdateAllThumb`, `MapGrid_UpdateLayerScroll`, `TilesetAnim_Update`,
`WindowSystem_Update`); `Battle_Run` adds `BattleCamera_Update` and
`BattleObject_UpdateSpritePriorities`; Lucky Wheels adds two to the VBlank
pass. What differs is what each sequence's own loop does between its waits:
the field answers what the player asks for, the battle runs turns.

## From a conversation to a battle and back

![In the loft of Vault's inn, the three thieves attack once Isaac has freed the villager; the battle opens, and at its end the party falls; back in the loft, the thieves take the party hostage](https://alchemy.pascalpixel.com/guides/front/vault-thieves.gif)

In Vault, the party pushes a crate aside with Move and climbs to the inn's
loft, where a villager lies tied up. All of it happens in one scene,
`SceneId_LogRoomsA`: a single map holding every room inside Vault's buildings,
all of them walled in logs, each room reached by its own entrance
([log_rooms_a.c](src/scene/log_rooms_a.c)).

### Talking to the villager

The main routine is running the field sequence for that scene. The field
sequence's loop calls `wait(1)` once a frame while the player walks. Pressing
A does not interrupt anything: the leader's control routine, which a task runs
during the wait, only sets a [request](glossary.md#field-request) in the field
work (`Field_CheckConfiguredKeys` sets `aButton`). When `wait(1)` returns,
the loop sees the request, finds the villager ahead
(`Field_FindNearbyTalkTarget`) and looks him up in the scene's
[trigger](glossary.md#trigger) table, which holds this row:

```c
{ TRIGGER_TALK, ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, EVENT(LogRoomsA_FreeRooferAndConfrontThieves) },
```

The row names an [event](glossary.md#event), and the loop calls it.

### The event

`LogRoomsA_FreeRooferAndConfrontThieves` runs straight through, from
`Event_Start` to `Event_End`. Isaac cuts the villager free, the three thieves
come up the ladder to `MUS_OMINOUS`, they speak, and they attack. Every pause
between those steps is an `Event_Wait`, which calls `wait()` unless the
debug fast-forward is on, and the tasks keep the actors moving during each
one. The event does not start the battle itself. At its end it sets a story
flag, writes into the main routine's work where the story goes after a win and
after a loss and that the battle is scripted, and asks the field for the battle
(one line, which picks the bars that close the screen, is left out):

```c
GameFlag_Set(FLAG_VAULT_THIEVES_ATTACKED);
base = (s32)&SceneId_LogRoomsA;
Event_SetAfterBattleDestination(base, LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
Event_SetDefeatDestination(base, LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
Battle_SetType(BATTLE_TYPE_SCRIPTED);
Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
```

A scene id is defined in the scene table itself: `scenes.s` defines
`SceneId_<name>` as the row's number, so no separate list of numbers can
drift from the table, and C reaches a number the assembler defines only as a
symbol's address. The last line only asks for the battle: it records the
thieves' [formation](glossary.md#formation) as the field's encounter request
and picks the battle music. The battle is answered like the A press, as a
request the loop sees once the event has returned.

### The battle

Back in the field sequence's loop, the encounter request is answered by
writing `SEQUENCE_BATTLE` as the next id and the formation as the entrance,
and the field sequence returns. The main routine's next pass tears down the
scene, its tasks and its heap, and calls `Battle_RunFar` with the formation.
The battle never reads the scene: no scene id and no overlay. Besides the
formation it is given, it reads the party, the battle type, the background
chosen for the scene (`gMainRoutineWork.special`) and the battle music from
the main routine's work. When it ends, `Scene_ReturnFromSequence` reads its result: a win
goes to the after-battle [destination](glossary.md#destination) the event
wrote, a loss to the defeat destination.

### Back in the loft

The next pass runs the field sequence for `SceneId_LogRoomsA` again, at the
[entrance](glossary.md#entrance) the event chose. The scene's
`Scene_Initialize` switches on that entrance. At
`LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED`, `LogRoomsA_DefeatThieves` stages
the beaten thieves, and `LogRoomsA_ReturnStolenGoods` sets
`FLAG_VAULT_THIEVES_JAILED` and hands back the stolen goods. At
`LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES` it plays `LogRoomsA_LoseToThieves`
and sends the party to the Vault Sanctum. Nothing on the map decides this:
the entrance tells the scene which event to play.

## Why every scene and battle starts cold

Since every sequence starts from the main routine's work and the game flags
alone, any scene or battle can follow any other, and Camelot's
[debug menus](glossary.md#debug-menus) can start any of them directly: the
debug scene selector writes a scene and entrance as the destination and lets
the scene exit ([System](system.md#design)).

## The four divisions

Camelot's credits divide the game's programming into System, Battle, Field,
User Interface and Support. The first four each have a page:

- [System](system.md): start-up, the main routine, waiting and tasks, memory,
  banks, overlays, saving and sound.
- [Battle](battle.md): a battle from the first menu to the spoils, and the
  game's rules.
- [Field](field.md): scenes and their requests, actors, maps, the World Map
  and field Psynergy.
- [User Interface](user_interface.md): windows and messages, the menus,
  shops and inns.

Beside them, [Places](https://alchemy.pascalpixel.com/tbs/places) has every scene in story order,
[Source](src/README.md) every file of the code, the [Glossary](glossary.md)
the words these pages share, and
[Unused](https://alchemy.pascalpixel.com/tbs/unused) what the game keeps but
never plays.
