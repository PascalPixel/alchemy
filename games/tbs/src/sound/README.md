# Sound

Everything you hear goes through one call. Code asks for a sound by its
number and says nothing else, and the number decides the rest.

## What a sound number means

![Isaac checks the chest in Sol Sanctum: `Isaac checked the chest...`, the Small Jewel rises from it with the treasure jingle, and `Isaac got a Small Jewel.`](https://alchemy.pascalpixel.com/guides/front/chest.gif)

There are three kinds of [sound](../../glossary.md#sound), and a number's
range says which one it is:

- **Music**, below 80, plays until something changes it. Asking for the
  music that is already playing changes nothing, which is why a tune
  carries on as you walk from a town into one of its houses.
- **A jingle**, 80 to 99, is a short tune that silences the music and then
  gives it back. Opening a chest plays one.
- **A sound effect**, from 100 up, plays over both.

A few numbers are not sounds but controls: fade the music out, or keep
whatever is playing. Every number has a name in
[sounds.h](../../include/sounds.h), `MUS_` for music and jingles and
`SE_` for effects, and the one routine that takes them all is
`Sound_Play` in `sound.c`.

Two sounds are heard together only if something plays each of them. There
are eight players, each playing one sound at a time. Music has one, jingles
have one, and sound effects share the other six. A sound's number also
fixes its player, and that is the whole reason some sounds cut each other
off: every blip of a spoken line is on the same player, so each replaces
the last.

## How a place chooses its music

Music is not started by a scene's code. When the party arrives somewhere,
the main routine picks the music from a table of rules before the scene is
even loaded. A rule names a scene or a whole area, optionally a
[game flag](../../glossary.md#game-flag), and a sound. The first rule that
fits wins.

It is the same pattern the field uses for triggers and exits. Vale has five
rules, latest first, and between them they tell its whole story: silence on
the stormy night, then one tune after another as flags are set. Because the
rules name the area, every house in Vale gets the town's answer.

Battles have a short table of their own for the fights with a special tune.
Every other battle gets the usual one.

## How the music comes back after a jingle

![Isaac stays at an inn: `Welcome to our inn.`, the price of a room and `Would you like to stay?`, Yes, the screen closes to black around him for the night, and `I hope you slept well. Have a nice trip!` Watch how long the screen stays black: it is waiting for the inn's tune to end.](https://alchemy.pascalpixel.com/guides/sound/inn-fanfare.gif)

An inn asks for one sound, its jingle, and never asks for the town's music
again. The music comes back by itself.

A jingle does not stop the music. It turns its volume to zero and lets it
play on unheard. The volume has a target, and steps toward it at every
VBlank. When the jingle is over the target goes back to full, and the
music swells in over about a second, having moved on as if the band had
played through the night.

Nothing counts frames here. All of the sound's timing runs on VBlanks,
which is why the music keeps its tempo when the picture slows down.

## The sound driver

Under the game's own sound code is Nintendo's standard driver for the
console. To the driver every sound is a song: tracks of notes with
instructions among them, read a little further at every VBlank. A note
plays either a sample, a short recording pitched up or down, or a tone on
one of the console's four built-in tone generators. A mixer adds every
sounding note together, with a little echo.

The songs and samples are ordinary files you can open, in
[`games/tbs/sound`](../../sound): each song is a MIDI file, such as
`songs/mus_vale.mid`, and each sample a WAV.

## Where the code is

- `sound.c` is the game's own layer: `Sound_Play`, the volume ramps and
  the music's return.
- `audio.c`, `driver.c`, `tables.c` and the mixer files are the driver.
- [`games/tbs/sound`](../../sound) holds the songs, the samples and the
  table that gives each sound its player.
