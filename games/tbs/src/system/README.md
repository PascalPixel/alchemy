# System code

This folder is the base the rest of the game runs on: start-up, the main
routine, the frame, tasks, the heap and the save.
[System](../../system.md) gives the four big ideas. This page goes one step
closer, so you know what each file is for before you open it.

## Start-up

Switching on and the reset with A, B, Select and Start are the same thing:
both run the cartridge's first instruction. Start-up clears memory, copies
the hottest routines into fast RAM, switches on the interrupts, and calls
the main routine, which never returns. It is `System_Initialize` in
`system_start_up.c`.

## The main routine

`MainRoutine_Run` in `main_routine.c` goes round three steps for as long
as the console is on: clear away the last sequence, call the next one as a
function, and when it returns work out what follows. One trip is a whole
sequence, not a frame. The loop that goes round once a frame is
[inside `wait()`](#the-frame-loop).

### Clearing away the last sequence

Before every sequence the task list is emptied, the whole heap is taken
back, looping sounds are cut off and any line-by-line screen effect is
switched off. This is what makes each sequence start cold. Only what lies
outside the heap crosses from one to the next: the main routine's state, the
game flags and the actor snapshot, each a fixed block of memory
(`gMainRoutineState` and `gActorSnapshot` are declared in `ewram.s`).

A scene's overlay is loaded here too, just before the field sequence is
called. A battle, Lucky Dice and Lucky Wheels load none.

### Running another sequence

A scene ends with an exit, which the main routine turns into a destination
through the scene's exit table. Anything else ends with a result, and
`Scene_ReturnFromSequence` picks the destination from it: back where the
party stood, to the restart point after a defeat, or wherever an event
named beforehand.

## The frame loop

![Isaac talks to Garet's sister in Vale: a window opens at the top of the screen beside her portrait, `You guys are so lucky, Garet.` is typed out letter by letter with a bobbing arrow at the end, the next page `I wish I could go with you.` follows, and the window closes.](https://alchemy.pascalpixel.com/guides/window/talk.gif)

The frame loop is the inside of `wait()`, in `system_frame.c`. One trip
round it is one frame.

The rule that shapes everything here: during a frame the game never touches
the screen. It only writes down, in ordinary memory, what the screen should
become. The console signals when it has finished drawing a picture, the
[VBlank](../../glossary.md#vblank), and in that gap everything written down
goes up at once.

```mermaid
%%{init: {"theme": "base", "themeVariables": {"lineColor": "#9AA3AE", "edgeLabelBackground": "#FFFFFF"}, "flowchart": {"curve": "basis", "padding": 12}}}%%
flowchart LR
  code(["The caller's code, up to its wait"]) --> tasks(["Tasks run"])
  tasks --> list(["The frame's sprites are put in one list"])
  list --> done(["Marked finished"])
  done --> vblank(["VBlank: it all goes up at once"])
  vblank -- "frames left" --> tasks
  vblank -- "done: wait returns" --> code
  classDef code fill:#DDE3EC,stroke:#8FA0B8,color:#2E3C50
  classDef frame fill:#D2EEF3,stroke:#7FC3D0,color:#1E5966
  classDef hardware fill:#FBE7C6,stroke:#E0B46A,color:#7A5212
  class code code
  class tasks,list,done frame
  class vblank hardware
```

One trip, in order:

1. Every task of the frame pass runs, highest rank first.
2. The sprites that were asked for are written into one list
   ([The frame's sprites](#the-frames-sprites)).
3. The frame is marked finished.
4. The loop sleeps until the VBlank handler has run.
5. The sprite slots are emptied, the frame count goes up by one and held
   buttons are turned into repeats.

The caller's own work is part of the frame as well. What it did between its
last `wait()` and this one came before step 1, and is shown with what the
tasks do. So a frame's time is the caller's work and the tasks' together.

### What VBlank does

The VBlank handler does two kinds of work. If the frame was marked
finished, it copies the frame to the screen. Whether or not it was, it runs
the tasks of its own pass, reads the buttons and prepares the next stretch
of sound.

That split is the game's [slowdown](../../glossary.md#slowdown). A frame
that runs long is simply shown one VBlank later, whole. Everything counted
in frames stretches together, nothing is ever shown half moved, and the
music keeps its tempo.

### Buttons and sleep

The frame loop also turns held buttons into repeats for the menus, and puts
the console to sleep when L and R are held for three seconds, or when
nothing is pressed for three minutes and idle sleep is on.

### Waiting from anywhere

Any routine may call `wait()`, however deep in its own calls it is, but
for a [task](../../glossary.md#task) and the VBlank handler: a task is
called from inside `wait()` and must return for the frame to end. Code that
a task might reach checks first (`gTasksRunning`).

There is one line of execution throughout: `wait()` starts no second one.
It does move the stack. The loop always runs from the same fixed place in
memory, so when the caller's stack has grown past that place, the part in
use is copied aside first and put back when `wait()` returns.

## The task runner

A [task](../../glossary.md#task) is a function put on a list once and
called every frame until it is taken off. The list holds 20. Each task has
a rank, which the code calls its `priority`, and higher ranks run first.
That is how one task gets to see another's finished work in the same frame.

There are two passes. Most tasks run once a frame from the frame loop. A
few run once a VBlank from the handler: the ones that copy something to the
screen. When a frame runs long, those few run more than once in it. The
rank says which pass as well: `TASK_PASS_FRAME + 10` is rank 10 of the
frame pass, and `TASK_PASS_VBLANK` is rank 0 of the other.
[The field's tasks](../field/README.md#the-fields-tasks) lists the ones a
town keeps on the list.

### A task's life

`Task_Add` puts a function on the list with a rank, and sorts the list. If
the function is already there it only gets the new rank, so nothing is on
the list twice. `Task_Remove` takes it off.

A task can also be suspended and resumed: it stays on the list and is
passed over. Opening the field menu suspends every task whose function is
in the overlay, so the scene holds still behind the menu, and a scene can
flag one of its tasks to keep running through that.

The main routine empties the list before every sequence, and the field
sequence empties it again as it sets a scene up. So each sequence adds what
it needs when it starts, and a scene adds its own on top.

An entry of the list is the function, the rank and that one flag. A task
gets no arguments and no memory of its own, so what it has to remember is
in its owner's [state](../../glossary.md#state). No task adds a task, and no
task calls `wait()` ([Who may wait](../../system.md#who-may-wait)). It is
all in `system_task.c`.

## The frame's sprites

The console draws sprites in the order of one list, and has no idea of
"in front". So the game rebuilds that list from scratch every frame.
Whatever wants a sprite shown drops it into one of 256 depth slots, and the
list is written from the highest slot down.

Because the slots are emptied every frame, a sprite shows only while
something keeps asking for it. Nothing ever has to take one down. It is
`system_oam.c`.

Screen settings follow the same rule as sprites. A change to a hardware
register is queued, and the VBlank handler carries the queue out with the
frame it belongs to (`system_io_write_queue.c`).

## The heap

The heap hands out memory like a stack of plates: each block goes on top of
the last, and giving one back gives back everything above it. That makes
it nearly free, and it means blocks go back in reverse order. The main
routine empties the lot between sequences.

A block that other code has to find again is kept in a numbered
[heap slot](../../glossary.md#heap-slot). Such a block is its owner's
[state](../../glossary.md#state): the menu's state, the battle's state.
Asking for a slot that is already filled just returns it, so the code that
sets a state up and the code that reads it make the same call. It is
`system_heap.c`.

## Memory map

The console has two working memories: 256 KB that is large and slower, and
32 KB that is small and fast. The game keeps most of its state in the first
and what must be quick in the second, each with a heap of its own. Where
everything sits is fixed, and is declared in `ewram.s` and `iwram.s`.

## Saving

![Isaac opens the Pause menu by Vale's river and chooses Save your quest: the File Screen lists the three files, and `Your game was saved.` confirms it](https://alchemy.pascalpixel.com/guides/runtime/save.gif)

A save is never overwritten in place. Each file is two parts, each part
fills one sector of the flash chip, and there are ten spare sectors. The
new copy of a part goes into a spare sector with a higher count, is read
back, and only then is the old copy retired. Every sector carries a label
and a checksum, so a half-written one is recognised and ignored.

The parts hold the game flags, the main routine's state and where the
scene's actors stand. It is `system_save.c`, and the Save command that
uses it is in [Windows](../window/README.md#saving).

## The link cable

Two consoles trade one small checked packet at every VBlank. Anything
bigger, such as a whole party for the Battle Arena, is cut into numbered
pieces and resent from the first one that goes missing. It is
`system_serial.c`.

## How an overlay reaches the rest of the game

![Isaac climbs the steps to the Sanctum's door in Vale: Vale fades out, and the Sanctum's hall fades in with Isaac just inside its door](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

An [overlay](../../glossary.md#overlay) is a scene's code, unpacked into
one fixed spot when the party arrives. It and the always-loaded
[main image](../../glossary.md#main-image) find each other through two
tables:

- The [entry table](../../glossary.md#entry-table-and-import-table) at the
  overlay's start: six functions the field calls to ask the scene about
  itself ([Scenes](../scene/README.md#the-entry-table) explains each).
- The import table after its code: one line for each main-image function
  the overlay calls, which the build writes from one list of them,
  `imports.inc`.

Scenes share overlays, 96 for 201 scenes. An overlay that holds several
scenes still has one entry table. Each of its six functions looks at which
scene the party is in and answers for that one: Crossbone Isle's hands
over one list of actors for the isle, another for the floating logs and
another for the nine floors of the cave
([crossbone_isle.c](../scene/crossbone_isle.c)).

It works the other way too, and one scene can serve several towns: the
Sanctums of five towns are one scene that looks at which way in the party
took.

## Where the rest is

The cartridge itself, and how its contents are found, is
[The Game Pak](../gamepak/README.md).
