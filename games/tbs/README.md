# The Broken Seal

Golden Sun was released in Japan as Golden Sun: The Broken Seal. These pages
use the Japanese subtitle, or TBS, to keep it apart from
[The Lost Age](glossary.md#the-two-games).

## How the game is built

Every sequence, event, menu and battle animation in The Broken Seal is code
that runs from its start to its end and waits where it stands whenever time
must pass, while short tasks run once a frame to keep the world moving:

- **One sequence at a time.** The [parent sequence](#the-parent-sequence)
  runs one [sequence](glossary.md#sequence) (a scene, a battle, a minigame)
  from start to finish.
- **Sequences hand over by themselves.** A scene that meets a random battle
  writes the battle down as the next sequence and ends; the battle ends by
  reporting a result, and the game works out where to go back to.
- **Code waits in place.** An event or a menu is an ordinary function that
  runs top to bottom and pauses when time must pass. While it pauses, a short
  list of [tasks](glossary.md#task) keeps the world moving
  ([Waiting in place](#waiting-in-place)).
- **The scene's loop only listens.** The scene sequence goes round once a
  frame while the player walks, and acts on what the tasks noted down.
- **A battle's rules and animations run together.** Damage appears while the
  attack is still on screen ([Battles](#battles)).
- **A frame that runs long slows the game instead of skipping work.** The
  picture and every movement slow down; the music doesn't
  ([When a frame runs long](#when-a-frame-runs-long)).

## Architecture

The game runs in four layers, each inside the one above it.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "fontSize": "14px"}, "flowchart": {"padding": 10}}}%%
flowchart TB
  subgraph seqm ["Parent sequence"]
    direction TB
    c1["One sequence at a time, for as long as the console is on"]
    subgraph scene ["Scene sequence"]
      direction TB
      c2["Loads one scene: its code, its map, its people"]
      subgraph mgr ["Its loop"]
        direction TB
        c3["Runs the scene a frame at a time until the party leaves"]
        subgraph frame ["Frame"]
          direction LR
          c4["The queued tasks run, then requests are handled"]
        end
      end
    end
    others["Battle · Dice · Slots sequences"]
  end
  c1 ~~~ scene
  c2 ~~~ mgr
  c3 ~~~ frame
  scene ~~~ others
  classDef caption fill:transparent,stroke:transparent,color:#6B7280
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class c1,c2,c3,c4 caption
  class others step
  style seqm fill:#EEF1F6,stroke:#8FA0B8,color:#2E3C50,rx:16,ry:16
  style scene fill:#E6F6F9,stroke:#7FC3D0,color:#1E5966,rx:16,ry:16
  style mgr fill:#E4F3E8,stroke:#8DC49E,color:#25603A,rx:16,ry:16
  style frame fill:#FFFFFF,stroke:#9AA3AE,rx:16,ry:16
```

- **[Parent sequence](#the-parent-sequence).** Runs one
  [sequence](glossary.md#sequence) at a time, for as long as the console is
  on: the scene sequence, the battle sequence, the dice sequence (Lucky Dice)
  or the slots sequence (Lucky Wheels).
- **[Scene sequence](#the-scene-sequence).** Loads one
  [scene](glossary.md#scene): a place such as Xian, or a screen such as the
  Title.
- **[Its loop](#the-scene-sequences-loop).** Runs that scene, one
  [frame](glossary.md#frame) at a time, until the party leaves.
- **Frame.** Every queued [task](glossary.md#task) runs once, then the scene
  sequence handles what the player or the scene asked for.

A box keeps its name and colour in every chart below. Xian, the example
throughout, is red.

## Start-up

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  power(["Power on"]) --> memory["Clear the fast memory"] --> irq["Install the resident code and the interrupt handlers"]
  irq --> services["Start the sound and the task queue"] --> seq(["Parent sequence"])
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class seq parentSequence
  class power,memory,irq,services step
```

Power-on runs once. It clears the fast memory, installs the code that stays
resident and the interrupt handlers, loads the window graphics, starts the
sound and empties the task queue. Then it hands control to the parent
sequence, which never returns.

**Reference:** `System_Initialize` · [Engine: Start-up](engine.md#start-up)

## The parent sequence

![Isaac walks up into the Sanctum's doorway in Vale; the village fades to black and the Sanctum's hall fades in around him, its people standing round the altar](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  loop(["Parent sequence"]) --> scene(["Scene sequence"]) & battle(["Battle sequence"]) & dice["Dice sequence: Lucky Dice"] & wheels["Slots sequence: Lucky Wheels"]
  scene & battle & dice & wheels --> back(["Next sequence"])
  back --> loop
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class loop parentSequence
  class scene sceneSequence
  class back request
  class battle battle
  class dice,wheels step
```

Each pass runs one sequence to its end. The sequence then names the next one,
and in the transition the parent sequence clears everything the last one left
behind (its tasks, its screen effects, the memory it borrowed) before starting
it.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  subgraph s1 ["Scene sequence"]
    xian(["Xian"])
  end
  s1 --> ask1(["Next sequence: the World Map"])
  ask1 --> s2
  subgraph s2 ["Scene sequence"]
    world(["World Map"])
  end
  s2 --> ask2(["Next sequence: a random battle"])
  ask2 --> battle(["Battle"]) --> ask3(["Next sequence: back to the World Map, where the party stood"])
  ask3 --> s3
  subgraph s3 ["Scene sequence"]
    again(["World Map"])
  end
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef place fill:#D93A2B,stroke:#B02A1D,color:#FFFFFF
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class xian place
  class world,again step
  class ask1,ask2,ask3 request
  class battle battle
  style s1 fill:#E6F6F9,stroke:#7FC3D0,color:#1E5966,rx:16,ry:16
  style s2 fill:#E6F6F9,stroke:#7FC3D0,color:#1E5966,rx:16,ry:16
  style s3 fill:#E6F6F9,stroke:#7FC3D0,color:#1E5966,rx:16,ry:16
```

A battle replaces the scene for a turn. Its code is always loaded, so there is
nothing to unpack, and afterwards the party stands where it stood. After a
defeat the game goes where an [event](glossary.md#event) chose for losing that
battle, or else to the last Sanctum.

### How a sequence finds its way back

Sequences pass nothing to each other directly. Everything goes through common,
`gCommon`, the one struct every sequence shares:

| When                       | What is written                                                                              |
| -------------------------- | -------------------------------------------------------------------------------------------- |
| a scene starts             | the return point (this scene and entrance), and empty after-battle and defeat destinations   |
| an event prepares a battle | the after-battle or defeat destination, where the story should continue                      |
| a random battle starts     | the battle as the next scene, its enemy group as the entrance, every actor's position        |
| the battle ends            | its result: won, fled or lost                                                                |

`Scene_ReturnFromSequence` reads them back. A win or a flight goes to the
after-battle destination, or else to the return point with everyone where they
stood; a defeat goes to the defeat destination, or else to the last Sanctum
with the leader at 1 HP. Lucky Dice and Lucky Wheels return the same way. The
result stays in `gCommon.sequenceResult` for the scene that comes back.
The Title and the File Screen need none of this: they are ordinary scenes.

**Reference:** `ParentSequence_Run` · `Scene_ReturnFromSequence`

## Waiting in place

The screen refreshes about 60 times a second, at each
[VBlank](glossary.md#vblank). The game moves in
[frames](glossary.md#frame), and every frame happens inside one function,
`WaitFrames`. Whatever code is in charge (the scene sequence, an event, a
menu, a battle animation) calls it when time must pass:

```c
void WaitFrames(s32 frames) {         /* simplified */
  while (frames--) {
    Task_RunPass(TASK_PASS_FRAME);    /* every queued task, once */
    Oam_BuildBuffer(...);             /* this frame's picture */
    VBlankIntrWait();                 /* halt until the next VBlank */
  }
}
```

The tasks are the world's moving parts: the actors, the camera, the animated
water, the open windows. They run whoever is waiting, so the world keeps
moving through a conversation, a menu or a cutscene without any of that code
knowing about it.

An event therefore reads in the order things happen. Showing the water
carrier's line, `Event_ShowMessageAndWait` simplified:

```c
window = FieldMessage_ShowAtActor(actorId, flags);  /* open her window */
while (!Window_IsClosedFar(window))
  WaitFrames(1);                                    /* the water keeps moving */
```

The window itself is drawn by a task, which types the line out and closes the
window once the player presses A.

| Frame | Code in charge         | Tasks during its wait                         | After the wait                                |
| ----- | ---------------------- | --------------------------------------------- | --------------------------------------------- |
| 1     | scene sequence         | the carrier steps, the water moves            | nothing asked for: next frame                 |
| 2     | scene sequence         | player control sees A and notes a request     | a talk request: run her event                 |
| 3     | her event              | the window types her line, the water moves    | window still open: wait again                 |
| …     | her event              | A pressed: the window task closes the window  | window closed: return                         |
| next  | scene sequence         | the carrier walks on                          | nothing asked for: next frame                 |

The conversation is the function itself, paused inside `WaitFrames` and
resumed on the next line, so nothing has to note how far it has got: the place
it has reached in its own code says so. To allow a wait from deep inside any
function, `WaitFrames` copies the caller's stack aside while the frame runs
([Runtime](src/runtime/README.md#the-stack-switch-in-waitframes)).

### When a frame runs long

Each VBlank interrupts the game, whatever it is doing. The interrupt always
mixes the next slice of sound and reads the buttons, and it puts up a new
picture only if `WaitFrames` has finished one. A frame whose tasks take longer
than one refresh still runs to the end, while the screen shows the old picture
until the next VBlank. Every wait, walk and animation is counted in frames, so
they all slow down together, while the music, counted in VBlanks, keeps its
tempo. This is the game's slowdown. The VBlanks a frame took are counted in
`gFrameVBlanks`.

**Reference:** [Engine: Waiting and tasks](engine.md#waiting-and-tasks) ·
[Runtime: The frame loop](src/runtime/README.md#the-frame-loop) ·
`Event_WaitFrames`

## The scene sequence

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  pick(["Scene sequence"]) --> arrive["Clear the last scene's flags, and the area's on entering a new area"]
  arrive --> music["Start the area's music"] --> code["Unpack the scene's code"]
  code --> map["Load the map"] --> place["Place the party at its entrance, or where it stood before a battle"]
  place --> actors["Create the actors from the scene's list"] --> own["Run the scene's set-up"]
  own --> fade["Fade in"] --> run(["The scene sequence's loop"])
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class pick sceneSequence
  class run sceneLoop
  class arrive,music,code,map,place,actors,own,fade step
```

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  subgraph seq ["Scene sequence"]
    direction TB
    pick(["Xian"]) --> arrive["From the World Map: a new area, so its flags are cleared and its name will show"]
    arrive --> music["MUS_FUCHIN_TEMPLE, Xian's field music"] --> code["xian.c's overlay, in place of the World Map's"]
    code --> map["map_074, with its animated water"] --> place["Isaac at the entrance from the World Map"]
    place --> actors["The villagers, the water carrier, Feizhi, the kung fu student, Mist"]
    actors --> own["Xian_Initialize: Feizhi gone and the student moved once Xian has heard of Hsu"]
    own --> fade["Fade in, Xian's name at the top"]
  end
  fade --> mgr
  subgraph mgr ["The scene sequence's loop"]
    run(["Xian"])
  end
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef place fill:#D93A2B,stroke:#B02A1D,color:#FFFFFF
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class pick,run place
  class arrive,music,code,map,place,actors,own,fade step
  style seq fill:#E6F6F9,stroke:#7FC3D0,color:#1E5966,rx:16,ry:16
  style mgr fill:#E4F3E8,stroke:#8DC49E,color:#25603A,rx:16,ry:16
```

Between scenes the parent sequence clears the
[game flags](glossary.md#game-flag) the last scene kept for itself, and the
[area](glossary.md#area)'s flags on entering a new area. It starts the area's music and unpacks the scene's code, its
[overlay](glossary.md#overlay). The scene sequence then loads the
[map](glossary.md#map), places the party at its
[entrance](glossary.md#entrance) and creates every
[actor](glossary.md#actor) on the scene's list. Last, the scene's own set-up
arranges the place as the story has left it, and the screen fades in.

**Reference:** `Scene_Arrive` · `Scene_ChooseMusic` · `Scene_LoadOverlay` ·
`SceneSequence_Run` · `Xian_Initialize` in [xian.c](src/scene/xian.c)

## The scene sequence's loop

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  frame(["The scene sequence's loop"]) --> queue
  subgraph queue ["Queued tasks, highest priority first"]
    direction TB
    t1["15 · Palette fade · moves the colours toward their target"] ~~~ t2["10 · Actors · each actor runs a step of its script; player control notes a pressed button as a request"]
    t2 ~~~ t3["5 · Camera · scrolls the map after the leader"] ~~~ t4["0 · Sprites · places each actor on the screen"]
    t4 ~~~ t5["0 · Tile animation · steps the map's animated tiles"] ~~~ t6["0 · Windows · draws each open window"]
  end
  queue --> ground["Check the ground under the leader"]
  ground --> check{"A request?"}
  check -->|"no: next frame"| frame
  check -->|"yes"| handle["Handle it to the end"]
  handle -->|"next frame"| frame
  handle -->|"an exit or a battle"| next(["Next sequence"])
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class frame sceneLoop
  class next request
  class t1,t2,t3,t4,t5,t6,ground,handle,check step
  style queue fill:#FFFFFF,stroke:#9AA3AE
```

Every frame, each queued task runs once, highest priority first. The camera
ranks below the actors so that it follows where they are now. The scene
sequence then checks the ground under the leader and handles any
[field request](glossary.md#field-request) to the end before the next frame.

### A frame in Xian

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart TD
  subgraph mgr ["The scene sequence's loop"]
    direction TB
    xian(["Xian"]) --> queue
    subgraph queue ["Queued tasks, this frame"]
      direction TB
      t1["15 · Palette fade · the colours held, the fade-in over"] ~~~ t2["10 · Actors · the water carrier steps; Isaac's player control: a button pressed? no, so no request"]
      t2 ~~~ t3["5 · Camera · stays on Isaac"] ~~~ t4["0 · Sprites · the villagers placed"]
      t4 ~~~ t5["0 · Tile animation · the water moves"] ~~~ t6["0 · Windows · none open"]
    end
    queue --> ground["Check the ground under Isaac, at the foot of the stairs: nothing there"]
    ground --> check{"A request?"}
    check -->|"no: next frame"| queue
  end
  classDef parentSequence fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef sceneSequence fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef battle fill:#F6D5D2,stroke:#D9928B,color:#8A2A22
  classDef sceneLoop fill:#D3EBD9,stroke:#8DC49E,color:#25603A
  classDef request fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  classDef place fill:#D93A2B,stroke:#B02A1D,color:#FFFFFF
  classDef step fill:#F4F4F2,stroke:#CFCFCB,color:#3A3A38
  class xian place
  class t1,t2,t3,t4,t5,t6,ground,check step
  style mgr fill:#E4F3E8,stroke:#8DC49E,color:#25603A,rx:16,ry:16
  style queue fill:#FFFFFF,stroke:#9AA3AE
```

Isaac stands still and the player presses nothing, yet the village moves: the
water carrier walks her round and the water flows.

### Talking to the water carrier

```mermaid
%%{init: {"theme": "base", "themeVariables": {"actorBkg": "#F4F4F2", "actorBorder": "#CFCFCB", "actorTextColor": "#3A3A38", "signalColor": "#9AA3AE", "signalTextColor": "#3A3A38", "noteBkgColor": "#FBE7C6", "noteBorderColor": "#E0B46A", "noteTextColor": "#7A5212", "loopTextColor": "#3A3A38", "labelBoxBkgColor": "#E4F3E8", "labelBoxBorderColor": "#8DC49E", "labelTextColor": "#25603A"}}}%%
sequenceDiagram
  participant P as Player
  participant C as Player control
  participant M as Scene sequence
  participant E as Her event
  participant T as Other tasks
  P->>C: presses A
  C->>M: notes an A request
  M->>M: finds the water carrier ahead and her talk trigger
  M->>E: runs her event, and she stops and turns to Isaac
  E->>E: opens a window with her line
  loop each frame while the window waits for A
    T->>T: the water moves, the text types out
  end
  P->>E: presses A
  E->>M: closes the window and returns
  M->>C: hands Isaac back to the player
```

A request can take as long as it needs. Her event waits for A inside an
ordinary function, and the tasks keep the village moving around the window.

### Requests

| Request        | Raised by                                                                          |
| -------------- | ---------------------------------------------------------------------------------- |
| exit           | stepping onto a way out, or an event that sends the party on                       |
| raised trigger | the scene's own code, the world map's L and R, or a party member falling to poison |
| random battle  | walking where battles happen, once enough steps have passed                        |
| step trigger   | stepping onto a cell the scene watches                                             |
| push           | walking into a stone pillar or anything else Move or a shove can shift             |
| A              | the A button: talk to whoever is ahead, else search, else open the field menu      |
| field menu     | Select                                                                             |
| Psynergy, item | the L and R shortcuts                                                              |
| Pause menu     | Start                                                                              |

Requests are handled in this order. An exit ends the scene. A random battle
names the battle as the next sequence and ends the scene too, which is why the
party returns from a battle to the same spot.

**Reference:** `SceneSequence_Run` in
[scene_sequence.c](src/overworld/overworld/scene_sequence.c) ·
`PaletteFade_Update` · `Actor_UpdateAllThumb` · `MapGrid_UpdateLayerScroll` ·
`ActorSystem_PlaceSprites` · `TilesetAnim_Update` · `WindowSystem_Update` ·
`Field_CheckConfiguredKeys` · `Field_UpdateWalking` ·
[Running a scene](src/overworld/README.md#the-scene-sequence-serves-requests)

## Battles

![Isaac and Garet against two Gryphons: both attack, then the first Gryphon casts Impact and its partner's Attack rises, and the second casts Tornado on the pair; the next round's menu opens.](https://alchemy.pascalpixel.com/guides/battle/round.gif)

A battle is a sequence of its own with an ordinary loop, in `Battle_Run`:

```c
for (;;) {                                  /* one round */
  actionCount = Battle_GatherActions(...);  /* menus and enemy habits */
  for (i = 0; i < actionCount; i++) {       /* in Agility order */
    BattleStaging_DispatchAction(...);      /* announce, animate, apply */
    /* stop here if a side has fled or been beaten */
  }
  Battle_ProcessRoundEnd();                 /* statuses count down */
}
```

An attack's animation is an ordinary function that waits frame by frame.
Its damage comes from a task, playback, which stays idle until the animation
signals that the blow has landed and then applies it target by target. So
Quake's damage messages appear while the rock is still rising, rather than
after it: the rules decide how much, the animation only when. The battle's
code also stays loaded between battles, so starting one unpacks nothing.

**Reference:** [Battle](battle.md) ·
[Rules and animations run together](battle.md#rules-and-animations-run-together)

## Scenes and overlays

Only one scene's code is in memory at a time. Its people, their lines and its
events matter nowhere else, so each scene's code is packed on the cartridge as
an overlay and unpacked into the same 32 KB. Overlays never call each other;
code that several scenes need is copied into each.

The rest of the game reaches a scene through six functions,
`struct SceneEntryPoints` at the start of every overlay:

1. Set up the scene.
2. Where can the party arrive?
3. Where do the exits lead?
4. Who stands here?
5. What happens when the player talks, searches or steps somewhere?
6. Which parts lead out?

Each answer is a function rather than a fixed table, so a scene can answer
differently as the story moves on.

**Reference:** [Engine: Overlays](engine.md#overlays) ·
[Scenes](src/scene/README.md) · [trigger](glossary.md#trigger)

## The scene table and areas

![Isaac steps from the world map onto Vale's marker; the village's gate fades in with the name Vale in a small window at the top of the screen, and he walks up through the gate](https://alchemy.pascalpixel.com/guides/front/area-name.gif)

Every scene is a row of `gScenes`, named in code by its row's symbol,
`SceneId_<Place>`. A row picks four things independently:

| Part | What it decides                                         |
| ---- | ------------------------------------------------------- |
| code | what happens there                                      |
| area | the music, the area name, the flags kept while inside   |
| map  | the picture and the ground                              |
| type | whether it is a town                                    |

The Bilibin Barricade runs on Bilibin's code but is an area of its own, and
several scenes can share a map. Walking between Vale's houses shows no name,
since the party stays in Vale's area; arriving from the world map shows "Vale"
for 90 frames, a second and a half.

In a town the party regains no [PP](glossary.md#psynergy-and-pp) and recovers
no [Djinn](glossary.md#djinn) as it walks, poison does no damage, Retreat
doesn't work and Avoid doesn't wear off.

| What     | Count | Note                               |
| -------- | ----- | ---------------------------------- |
| scenes   | 201   |                                    |
| overlays | 96    | some shared by several scenes      |
| areas    | 71    |                                    |

## Exits and entrances

![Isaac walks down the path from Sol Sanctum's entrance and arrives at the foot of the steps in Vale; then the same walk again, and this time the exit leads to Vale erupting, the villagers crowded beside the bridge while the river and the cliffs glow red](https://alchemy.pascalpixel.com/guides/front/sol-entrance-warp.gif)

An [exit](glossary.md#exit) only names where it goes. The scene's exit table
turns that name into a scene and an entrance: the spot where the party appears
and the way it faces. A row can last only until a flag is set, and the first
row that holds wins. The path in the recording has three rows:

| Until                    | The path leads to |
| ------------------------ | ----------------- |
| the Sanctum erupts       | Vale              |
| the party leaves Vale    | Vale erupting     |
| from then on             | Vale              |

A scene's set-up often reads the entrance too, to play the part of an event
the last scene sent the party to.

Every way between scenes is named in [scene_exits.h](include/scene_exits.h):
entrances for where the party comes from (`VALE_PLAZA_FROM_SANCTUM`), exits for
where they lead (`VALE_PLAZA_TO_WORLD_MAP`), so a door has the same name from
both sides.

**Reference:** `gSolSanctumEntrance_Exits` in
[sol_sanctum_entrance.c](src/scene/sol_sanctum_entrance.c)

## Game flags

![In a small room of Sol Sanctum, Isaac searches the chest in front of him: a green Small Jewel rises out of it, he holds it over his head and a window reads "Isaac got a Small Jewel."](https://alchemy.pascalpixel.com/guides/front/chest.gif)

A scene keeps nothing between visits. Whatever the game must remember, a chest
or the end of a chapter, is a game flag saved with the game. The chest's
trigger row counts only while `FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND` is
clear; opening it sets the flag, and from then on the chest is drawn open and
empty.

[flags.h](include/flags.h) holds 4,096 flags in a fixed order: the party and
the Djinn, the engine's, the story's, then the chests and other finds, each
area's together. Two ranges are scratch space:

| Range           | Size | Cleared                          | Example                                   |
| --------------- | ---- | -------------------------------- | ----------------------------------------- |
| a scene's flags | 256  | each time the party arrives      | a stone pillar pushed out of place is back |
| an area's flags | 256  | on entering another area         |                                           |

Neither is cleared on returning from a battle, which comes back to the same
moment. Eight flags in a row can be read as a small number, which is how
counters live among them.

Because trigger rows and flags depend on each other, some of the game's code
can never run. Its comment opens with [`Unused:`](glossary.md#unused), and
[Unused](https://alchemy.pascalpixel.com/tbs/unused) collects every case.

**Reference:** [Game rules: The game flags](src/game/README.md#the-game-flags)

## How scenes depend on the story

![Vale's gate three times: on the stormy night, rain falling and the villagers gathered at the top of the screen; after the party has left, a guard standing by the path; and later in the journey, more villagers about](https://alchemy.pascalpixel.com/guides/front/vale-plaza-story.gif)

The story is the order the flags are set in. Each time the party arrives, a
scene asks the flags which version of itself to be and returns that version's
tables. Vale's gate is `ValePlaza_GetTriggers` in
[vale_plaza.c](src/scene/vale_plaza.c).

The other forms it takes, in
[story order](glossary.md#the-places-in-story-order):

| Form                               | Example                                                                                                                       |
| ---------------------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| a scene per point in the story     | the stormy night in Vale ([vale_stormy_night.c](src/scene/vale_stormy_night.c))                                               |
| a scene with no triggers           | Vale erupting plays its event and leaves by its one exit ([vale_eruption.c](src/scene/vale_eruption.c))                       |
| a row that lasts until its flag    | each pedestal of the Elemental Stars offers its star once ([sol_sanctum_elemental_star_room.c](src/scene/sol_sanctum_elemental_star_room.c)) |
| a flag set by arriving             | reaching the World Map from Vale marks that the party has left                                                                |
| one event cut into parts           | the top of Venus Lighthouse, broken by two battles and two trips to the World Map ([venus_lighthouse_aerie.c](src/scene/venus_lighthouse_aerie.c)) |
| the last scene                     | the Lemurian ship at Lalivero sends the party to the Title ([lemurian_ship.c](src/scene/lemurian_ship.c))                     |

**Reference:** [Places](src/field/scenes.s), every scene in story order

## The world map

![The whole world map of The Broken Seal laid out flat: the land of mountains, rivers and forest that the story crosses, two small islands in the sea, and plain blocks where the map's data leaves squares blank](https://alchemy.pascalpixel.com/story/world.png)

The World Map is a scene like the others, drawn in perspective on the two
background layers the hardware can scale and rotate. Its world is stored in
squares 512 pixels on a side, and only the four around the party are unpacked
at a time. Each town or dungeon is a figure that takes its exit when touched.
The map also holds three Djinn and a storm, and plays the story's moments
between places, chosen by where the last scene sent the party.

**Reference:** [world_map.c](src/scene/world_map.c) ·
[World: The world map](world.md#the-world-map)

## How the code is organised

The main image runs straight from the cartridge in fourteen
[banks](glossary.md#bank), about 1 MB of the English cartridge, followed by
the sound code, the music and the packed pictures and maps.

| Folder                                    | Holds                                         |
| ----------------------------------------- | --------------------------------------------- |
| `runtime`, `field`, `window`, `game`, `overworld`, `menu`, `shop`, `battle`, `battle_anim` | the nine large banks, in cartridge order; the five small banks' code sits with the folder it belongs to |
| `graphics`, `map`, `sound`, `system`      | code grouped by topic, linked wherever needed |
| `scene`, `debug`                          | overlays                                      |

`overworld` runs a scene from arrival to exit, and `field` moves and draws its
actors.

Each bank starts at a fixed place, and code in one bank reaches another only
through a short list of entrances at that bank's start, so a bank can grow
without moving the others. An entrance is the function's name with `Far`
added: the parent sequence starts a battle with `Battle_RunFar`, which leads
to `Battle_Run` in the battle bank.

**Reference:** [far calls](glossary.md#far-call) ·
[Why the main image is cut into banks](src/system/README.md#why-the-main-image-is-cut-into-banks) ·
[`objects.tsv`](objects.tsv) · [Source](src/README.md)

## Section pages

| Page                                                 | Covers                                                               |
| ---------------------------------------------------- | -------------------------------------------------------------------- |
| [World](world.md)                                    | the field engine, the maps, the request loop and field Psynergy      |
| [Battle](battle.md)                                  | a battle from the first menu to the spoils, damage, its animations   |
| [Menus](menus.md)                                    | the field menu and its screens, shops and inns, windows              |
| [Engine](engine.md)                                  | start-up, frames and tasks, banks and far calls, overlays, songs       |
| [Places](src/field/scenes.s)                         | every scene in story order, with its actor templates and triggers    |
| [Game rules](src/game/README.md)                     | stats, items, Djinn, classes, levels and the flags                   |
| [Unused](https://alchemy.pascalpixel.com/tbs/unused) | what the game keeps but never plays, and why                         |
| [Source](src/README.md)                              | every file of the code, with its comments                            |
| [Glossary](glossary.md)                              | the words every page uses                                            |
