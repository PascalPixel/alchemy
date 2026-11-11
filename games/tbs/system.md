# System

![Isaac climbs the steps to the Sanctum's door in Vale. Watch the fade: Vale goes dark, and the Sanctum's hall comes up with Isaac just inside its door.](https://alchemy.pascalpixel.com/guides/front/sanctum-door.gif)

Isaac climbs the steps to Vale's Sanctum and walks through its door. The
town fades out, the hall fades in, and he is standing just inside.

To the player that is one walk with a fade in the middle. Underneath, one
part of the game has ended and another has begun. While the screen was dark
the game cleared away everything it had set up for Vale, and fetched from
the cartridge everything the Sanctum can do.

The System is the part of the game under every moment like that one. It
starts the game, decides which part of it runs next, and lets time pass. It
finds things on the cartridge, plays the sound and writes the save.

Three words come up all the way down this page. The game is code: long
lists of instructions for the console to carry out. Memory is where the
console keeps whatever the game is working on at the moment, and it is
emptied when the power goes. And the console draws the screen afresh about
sixty times a second. Each of those pictures, with the work that goes into
it, is a [frame](glossary.md#frame).

The System's code is in five folders, and each has a guide that goes
through properly what this page introduces:
[System code](src/system/README.md), [The Game Pak](src/gamepak/README.md),
which is the cartridge itself, [Sound](src/sound/README.md),
[Graphics](src/graphics/README.md) and
[Debug scenes](src/debug/README.md).

## Start-up

![Isaac stands in Vale. Then A, B, Select and Start are pressed together, and the game starts over from its opening logos.](https://alchemy.pascalpixel.com/guides/runtime/soft-reset.gif)

Hold A, B, Select and Start anywhere in the game and the opening logos come
up, just as they do when the console is switched on.

As far as the game can tell, it has been switched on. Switching on runs the
first instruction on the cartridge, and the four buttons send the game back
to that same instruction. So a reset goes through start-up in full.

Start-up runs once, from top to bottom, before anything moves. It wipes the
memory, which after a reset is still full of the old game. It loads the
pictures the windows are drawn from, and starts the sound.

Then it waits ten frames and hands over to the main routine, which never
hands back. The logos are the first thing the main routine shows.
[Start-up](src/system/README.md#start-up) goes through it step by step.

## The main routine

![Isaac walks through Goma Cave until a random battle starts. Watch what comes back afterwards: the same cave, with the party standing where the battle found it.](https://alchemy.pascalpixel.com/guides/overworld/battle-return.gif)

Isaac walks through Goma Cave, a battle breaks out, and afterwards the cave
is back with the party where it stood.

The game runs those as three separate stretches of play: the cave, the
battle, the cave again. It calls each stretch a
[sequence](glossary.md#sequence), and there are four kinds. The
[field sequence](glossary.md#field-sequence) runs a
[scene](glossary.md#scene), which is one town, one dungeon floor, a few
rooms or the World Map. The other three run a battle, a game of Lucky Dice
and a game of Lucky Wheels.

The [main routine](glossary.md#main-routine) runs them, one after another,
for as long as the console is on.

Between two sequences it makes a
[transition](glossary.md#transition-and-next-sequence). It stops whatever
the last sequence left going and clears out the memory it used. The battle
starts with nothing of the cave's still running under it.

Very little crosses over. The party, its coins and its Djinn are kept in
one place the clearing never touches, together with a note of what runs
next. The game's own name for a part's private notes is its work, so this
is the [main routine's work](glossary.md#main-routines-work). Beside it are
the [game flags](glossary.md#game-flag). A flag is one yes-or-no fact the
game remembers, such as a chest being open.

A scene ends when the party takes one of its [exits](glossary.md#exit), a
door or the edge of a map, and the exit says where it leads. A battle ends
with a result instead. After this battle in Goma Cave, won or fled, the
party goes back to the scene it left, standing where it stood. A party that
falls in a random battle is sent to its restart point, usually the last
Sanctum. A story battle can name other places beforehand.

[The main routine](src/system/README.md#the-main-routine) follows one turn
of this, from the clearing away to the exit.

## Waiting and tasks

![A villager by Vale's river says `I don't blame Felix.` and waits for the A Button. Watch the river behind him: it keeps flowing the whole time he waits.](https://alchemy.pascalpixel.com/guides/engine/waiting-talk.gif)

A villager by Vale's river says his line and waits for the A Button. Behind
him the river keeps flowing.

Here is the odd part. The code that showed his line is standing still
until the window has closed.

It stands still at one small piece of code named
[`wait()`](glossary.md#wait). Whatever has to let time pass hands over to
it and says for how many frames. And while `wait()` waits, it keeps the
rest of the game going.

The whole game is written this way. A night at an inn, in the code, is a
plain list: wait 20 frames, close the screen to black, play the inn's tune
and wait for it to end, wait 10 frames, open the screen again, wait 30
more. Nothing in that list has to keep the game running. The waits do it.

In every frame it waits through, `wait()` runs a short list of chores, each
of which does one frame's share of a job. The game calls a chore a
[task](glossary.md#task). Typing the next letters into a window is a
task, which is how the villager's line types itself out while his own code
stands still.

Then `wait()` rests until the console has finished drawing the picture. In
the short pause before the next picture starts, the
[VBlank](glossary.md#vblank), the finished frame is put on screen all at
once, the buttons are read, and the next stretch of sound is made ready.

[The frame loop](src/system/README.md#the-frame-loop) has the frame's order
in full, [The task runner](src/system/README.md#the-task-runner) says which
task goes first, and
[The stack switch in `wait()`](src/system/README.md#the-stack-switch-in-wait)
is the housekeeping that makes waiting safe.

### Slowdown

Sometimes a frame has too much to do and is still being worked on when the
VBlank comes. Nothing of it is shown. The screen keeps the last picture,
whole, and the late frame goes up at the VBlank after.

Every walk, wait and animation is counted in frames, so they all take
longer together. The music doesn't. Its sound is made ready at every
VBlank, finished frame or not, so the tune keeps its tempo while the
picture drags.

That is the game's [slowdown](glossary.md#slowdown).
[What VBlank does](src/system/README.md#what-vblank-does) shows why the
screen never shows half a frame.

## Overlays

![Isaac leaves a house in Vale. While the screen is dark between the room and the town, the code for one is swapped for the code of the other.](https://alchemy.pascalpixel.com/guides/map/arrive.gif)

Every scene has code of its own: what its people say, what its doors and
switches do. That code matters only while the party is there, and the game
has 201 scenes.

So the game keeps each scene's code packed small on the cartridge, and
unpacks it into memory when the party arrives. Such a packed piece is an
[overlay](glossary.md#overlay).

Every overlay is unpacked into the same spot in memory, over whichever one
was there before. So only one is ever loaded.

Scenes share overlays. The rooms of one place often have their code in one
piece, so the 201 scenes need only 96 overlays. But one overlay cannot
borrow from another, since the two are never loaded together. Code that
two of them need is built into both.

[How an overlay reaches the rest of the game](src/system/README.md#how-an-overlay-reaches-the-rest-of-the-game)
describes the unpacking, and the two lists through which a scene's code and
the rest of the game find each other.

## Saving

![Isaac opens the Pause menu by Vale's river and chooses `Save your quest`. The File Screen lists the three files, and `Your game was saved.` confirms it.](https://alchemy.pascalpixel.com/guides/runtime/save.gif)

Choose `Save your quest`, and before `Your game was saved.` appears the game
has been careful. It has written the new save beside the old one, read it
back to check it, and only then crossed the old one out.

The saves live in a [flash chip](glossary.md#flash-chip) on the cartridge, a
small memory that keeps what is written to it with the power off. The chip
is written in 16 pieces called sectors.

Each of the three save [files](glossary.md#file-and-part) is two parts, and
a part fills one sector. Between them the two parts hold the game flags,
the main routine's work and where the scene's people stand. Three files of
two parts need six sectors, so ten are spare.

A save uses the spare ones. Each part's new copy goes into an empty sector,
and the copy carries a count one higher than the old copy's. If the
batteries die halfway, the old copy is still whole. If they die just after,
the game finds two copies and takes the one with the higher count.

[Saving](src/system/README.md#saving) follows a save onto the chip, and
[The flash chip](src/gamepak/README.md#the-flash-chip) is about the chip
itself.

## Sound

![Isaac stays the night at an inn. Watch the screen close to black around him: that dark moment is when the inn's short tune plays.](https://alchemy.pascalpixel.com/guides/sound/inn-fanfare.gif)

Stay at an inn and the town's music stops, a short tune plays over the
black screen, and the music comes back by itself.

The inn did almost none of that. It asked for
[sound](glossary.md#sound) number 86 and waited for it to end. It never
said that 86 is a short tune, or that the music should make way for it.

The number says so. Every sound in the game is asked for the same way, by
its number, and the number's range decides the kind. Below 80 is music.
From 80 to 99 is a jingle, a short tune like the inn's. The music is turned
silent for it, carries on unheard, and is faded back in when the jingle
ends. From 100 up is a sound effect, which plays over both. A few numbers
are orders and not sounds, such as the one that fades the music out.

Asking for the music that is already playing changes nothing, which is
why walking from room to room leaves the tune playing on.

[What a sound number means](src/sound/README.md#what-a-sound-number-means)
has the kinds in full, and
[The sound driver](src/sound/README.md#the-sound-driver) is about the code
that turns notes into what the speaker plays.

## Images

![The title intro taken apart. Look at how many separate pictures it is: the clouds, Mt. Aleph and the sun are spread out one behind another, then put back together.](https://alchemy.pascalpixel.com/guides/graphics/title-intro-layers-3d.gif)

Everything the player sees is a picture from the cartridge: the tiles a
[map](glossary.md#map) is built from, Isaac, a monster, the letters of a
message.

In this project each one is an ordinary image file that any image editor
opens. `battle_backgrounds/grassland.png`, for instance, is a battle's
backdrop, 256 dots wide and 120 high. When the cartridge's contents are put
together from these files, every picture is packed into the form the
console reads and placed where the code will look for it.

[How an image is built and placed](src/graphics/README.md#how-an-image-is-built-and-placed)
follows a picture from its file to the cartridge, and
[The resource directory and table order](src/gamepak/README.md#the-resource-directory-and-table-order)
explains the cartridge's one numbered list, where a picture or a scene's
overlay is found by its number.

## Debug scenes

![A small window the player never sees, opened in Vale. Each press steps to another scene and shows its name, and A takes the party there.](https://alchemy.pascalpixel.com/guides/debug/map-select.gif)

The developers left their test rooms on the cartridge. The last ten of the
201 scenes are theirs: eight test rooms that hand out Djinn, one called
Increase item and level, and one called Menu test.

They also left the small window in the recording, which jumps to any scene
in the game.

A player never sees any of it. The window opens only when a hidden switch
the developers used, debug mode, is on, and start-up turns it off.
[How the debug scenes are reached](src/debug/README.md#how-the-debug-scenes-are-reached)
shows the window and says why the switch stays off.

## Design

Every sequence starts cold. The cave after a battle is not the old cave
kept waiting. It is built again from what the main routine's work and the
game flags say. So any scene or battle can follow any other, and that is
what lets the developers' window jump from Vale to any scene in the game:
the [debug menus](glossary.md#debug-menus) rely on it
([The Broken Seal](README.md#why-every-sequence-starts-fresh)).

## For readers of the code

### The heap and work

Memory the game uses for a while and then gives up is borrowed from the
[heap](glossary.md#heap-and-pools). Before a game of Lucky Dice, for
instance, the main routine borrows a little to keep the player's window
colours safe, and puts the colours back afterwards. The main routine
empties the whole heap between sequences.

A part of the game that keeps notes to come back to, its
[work](glossary.md#work), holds them in a numbered place of the heap, a
[heap slot](glossary.md#heap-slot). [The heap](src/system/README.md#the-heap)
explains how the borrowing is done.

### Banks and far calls

Most of the game's code runs straight from the cartridge. That code is the
[main image](glossary.md#main-image), and it is cut into fourteen
[banks](glossary.md#bank): one for the field, one for the battle, one for
the shops, one for the sound, and so on.

When the inn, in the shops' bank, wants its tune, it does not go to the
sound's code directly. It goes to a fixed signpost at the front of the
sound's bank, a [veneer](glossary.md#veneer), which points on to the code.
Going by the signpost is a [far call](glossary.md#far-call).
[Why the main image is cut into banks](src/gamepak/README.md#why-the-main-image-is-cut-into-banks)
says what the signposts are for.
