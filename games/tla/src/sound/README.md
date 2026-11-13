# Sound

Everything you hear goes through one call. Code asks for a sound by its
number and says nothing else, and the number decides the rest.

## What a sound number means

There are three kinds of [sound](../../glossary.md#sound), and a number's
range says which one it is:

- **Music**, below 80, plays until something changes it. Asking for the
  music that is already playing changes nothing, which is why a tune
  carries on as you walk from a town into one of its houses.
- **A jingle**, 80 to 99, is a short tune that silences the music and then
  gives it back. Finding an item plays one.
- **A sound effect**, from 100 to 699, plays over both.
- **Music again**, from 700 up, where The Lost Age keeps 31 more tunes.

A few numbers are not sounds but controls: fade the music out, keep
whatever is playing, or fade out the effects of one or two players. The
Lost Age has no list of names for its sounds yet, so its code writes the
numbers, and the one routine that takes them all is `Sound_Play`.

Every sound played is also marked as heard, one bit for each number. In
the link lobby the player can ask for a tune by number, and only the ones
already heard are offered, unless L, R and Select are held as it opens.

Two sounds are heard together only if something plays each of them. There
are eight players, each playing one sound at a time. Music has one, jingles
have one, and sound effects share the other six. A sound's number also
fixes its player, and that is the whole reason some sounds cut each other
off: a new sound on a busy player replaces the one it was playing. The last
player is kinder. A sound meant for it looks back through three more
effect players for a free one, and cuts off its own sound only when all
four are busy.

## How a place chooses its music

Music is not started by a scene's code. When the party arrives somewhere,
the main routine picks the music from a table of rules before the scene is
even loaded (`Scene_ChooseMusic`). A rule names a scene or a whole area,
optionally an entrance and a [game flag](../../glossary.md#game-flag), and
a sound. The first rule that fits wins.

It is the same pattern the field uses for triggers and exits. Daila has
two rules: one way into its houses gets a tune of its own, and the area's
rule gives every other part of Daila the town's. The World Map has six,
and two of them change its tune as game flags are set.

Battles have a short table of their own for the fights with a special tune.
Most other battles take their tune from who is fighting: one when Felix is
among the four in front, another when Isaac is and Felix is not, and a
third when Jenna is there without either.

## How the music comes back after a jingle

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
sounding note together.

The songs and samples are ordinary files you can open, in
[`games/tla/sound`](../../sound): each song is a MIDI file in `songs/`,
such as `mus_007.mid`, and each sample a WAV in `direct_sound_samples/`. A
sequence The Broken Seal holds note for note takes its name there, such as
`se_jump.mid`; the rest keep their row in `gSongTable`.

## Where the code is

- The game's own layer is `Sound_Play`, `Sound_UpdateBgmRamps` and their
  neighbours: the volume ramps and the music's return.
- The rest of this folder is the driver: its C, its handwritten routines
  in `driver/`, and the mixer the game copies into IWRAM.
- [`games/tla/sound`](../../sound) holds the songs, the samples and the
  table that gives each sound its player.
