# Engine

Under everything the player sees is code they never see directly: what
starts the game when the console is switched on, keeps time one frame at a
time, reads the buttons, hands out memory, plays the music and loads each
place's code from the cartridge. The field, the battles and the menus are all
built on it.

## What happens at power-on?

![The game from power-on: the Nintendo logo, the Camelot logo with its castle, and the Golden Sun logo](https://alchemy.pascalpixel.com/guides/engine/boot.gif)

The cartridge's header jumps to `System_Entry`, which sets up the stacks and
calls `System_Initialize`. That clears the console's fast internal RAM,
copies the game's most time-critical routines into it, installs the interrupt
handlers, starts the sound driver, waits ten frames and starts the game.

It never returns. The game doesn't have a main loop that calls everything
else once a frame. `Game_Run` goes from scene to scene for as long
as the console is on, and the first scene is the title, which shows the
Nintendo and Camelot logos. When there's a save file, the intro from
[the front page](README.md) plays next; with none, as here, the file select
screen brings up the Golden Sun logo and goes straight to naming the hero.
[Runtime](src/runtime/README.md) follows the whole boot, and
[Graphics](src/graphics/README.md) how the logos and the intro are drawn.

## How does time pass?

Every part of the game is written as ordinary functions that run until
they're done. A menu screen, a cutscene or a battle effect is a function with
a loop of its own, and when time has to pass it calls `WaitFrames` and
carries on from the same place once the frames are over. Inside, `WaitFrames`
runs the game's tasks and waits for the screen to finish drawing, and in the
short pause before the next frame its sprites and scroll positions are
copied to the hardware.

Anything that has to keep moving while such a function waits is a task: a
small routine the scheduler runs every frame, however deep the waiting
function is. The windows zooming open, the damage appearing during a battle
effect and the watch for Start in the menus are all tasks. The choice has a
cost. Every waiting function keeps its calls on the stack, and the game
keeps that in less than two kilobytes of fast RAM, so `WaitFrames` moves a deep
caller's stack aside before it runs the frame, and puts it back afterwards.
That way the tasks always start from the same place, with the same room.

## Why is the program cut into banks?

The code the console runs straight from the cartridge is cut into banks, one
for the runtime, one for the field, one for the windows and so on, each
starting at an address of its own with empty space after it. A bank calls a
function in another bank through a table of jumps at the start of that bank,
never at the function's own address. That isn't about distance, since a call
can reach the whole program. It's what lets a bank grow or shrink inside its
space without moving any address another bank relies on.

The places the party walks around in aren't in that program at all. Each town
and dungeon is an overlay: its code, compressed in the cartridge, unpacked
into the same 32 KB of RAM when its scene loads. An overlay reaches the main
program through its import table, which lists the jumps it needs in a fixed
order, and the main program reaches the overlay through six entries at its
start. Those two tables are the only places they meet.
[System](src/system/README.md) has the memory map, the banks and the
overlays.

## What does a sound number mean?

The game asks for every sound by number, from the battle music to the menu
cursor's click, and where the number falls decides what kind of sound it is:
music, a fanfare or a sound effect. A fanfare, like the jingle when a chest
opens, doesn't stop the music. It turns the music down to nothing, plays over
it, and once it has finished, the music fades back in where it would have
been, so the code that opened the chest never has to know. Every sound is
played by Nintendo's MusicPlayer2000, the driver the game links.
[Sound](src/sound/README.md) explains the cues, how a fanfare gets back to
the music, and the driver's lock.

## What's left of the developers' tools?

The finished game still holds three test rooms: one that hands out Djinn, one
for levels and for browsing items, icons and letters, and one full of shop and
message tests. Each is an ordinary scene, built like any town, so they're
also the shortest way to see how a scene is put together.
[Debug rooms](src/debug/README.md) goes through them.
