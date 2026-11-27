# Sound

Everything you hear goes through one call. Code asks for a sound by its
number and says nothing else, and the number decides the rest.

## What a sound number means

![Felix scoops at the spot under a puddle in Daila and digs up a Psy Crystal, with the treasure jingle: `Felix got a Psy Crystal.`](https://alchemy.pascalpixel.com/tla/guides/field/scoop.gif)

There are three kinds of [sound](../../../../docs/tla/glossary.md#sound), and a number's
range says which one it is:

- **Music**, below 80, plays until something changes it. Asking for the
  music that is already playing changes nothing, which is why a tune
  carries on as you walk from a town into one of its houses.
- **A jingle**, 80 to 99, is a short tune that silences the music and then
  gives it back. Finding an item plays one.
- **A sound effect**, from 100 to 699, plays over both.
- **Music again**, from 700 up, where The Lost Age keeps 31 more tunes.

A few numbers are not sounds but controls: fade the music out, keep
whatever is playing, or fade out the effects of one or two players. Every
sound has a name in [sounds.h](../../include/sounds.h), one for each row
of the song table; a row of the empty song, which plays nothing, has none.
A sound whose notes are The Broken Seal's own takes that game's name (the
message voices, `SE_BELL`, `MUS_VILLAGE_THEME`), a tune takes the title of
its track where the two are matched
(`MUS_TO_THE_OPEN_SEA`) and otherwise the place it is the music of
(`MUS_PROX`) or the moment it marks (`MUS_SAVED`), and an effect
for what it plays with (`SE_DOLL_PLACED`) or what it sounds like
(`SE_LANDING_THUD`). A sound that repeats another's
notes higher, lower or on another voice is named for it
(`SE_RISE_WITH_SPARKS_HIGH`), and one nothing plays says so
(`SE_UNUSED_WHIRLWIND_LOW_A`). The one function that takes them all is
`sySndPlay`.

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
even loaded (`fdChooseMusic`). A rule names a scene or a whole area,
optionally an entrance and a [game flag](../../../../docs/tla/glossary.md#game-flag), and
a sound. The first rule that fits wins.

It is the same pattern the field engine uses for cues and exits. Daila
has two rules: one way into its houses gets a tune of its own, and the
area's rule gives every other part of Daila the town's. The World Map has
six, and two of them change its tune as game flags are set.

Battles have a short table of their own for the fights with a special tune.
Most other battles take their tune from who is fighting: one when Felix is
among the four in front, another when Isaac is and Felix is not, and a
third when Jenna is there without either.

## How the music comes back after a jingle

![Felix stays at Daila's inn: the price, Yes, the screen closes to black for the night while the inn's tune plays, and the morning's line](https://alchemy.pascalpixel.com/tla/guides/sound/inn-fanfare.gif)

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

Under the game's own sound code is the standard driver of the console's
development kit. To the driver every sound is a song: tracks of notes with
instructions among them, read a little further at every VBlank. A note
plays either a sample, a short recording pitched up or down, or a tone on
one of the console's four built-in tone generators. A mixer adds every
sounding note together.

The songs and samples are ordinary files you can open, in
[`games/tla/sound`](../../sound): each song is a MIDI file in `songs/`,
such as `mus_madra.mid`, and each sample a WAV in `direct_sound_samples/`. A
sequence takes its sound's name, such as `se_jump.mid`; those nothing
names keep their row in `SongTable`. A music sample is named for the
instrument General MIDI gives its voice slot (slot 47, `timpani.pcm8.wav`),
an effect's sample for the sounds that play it (`rumble.pcm8.wav`), and a
drum for the key General MIDI gives its row of the drum kit (`side_stick.pcm8.wav`).
A song or sample that The Broken Seal has byte for byte, and the wave
channel's patterns, are one copy in
[`games/common/sound`](../../../common/README.md) that both games build.

## Where the code is

- `sound_play.c` and `sound_test.c` here are the game's own layer:
  `sySndPlay`, `sySndUpdateBGMRamps` and their neighbours, the volume ramps
  and the music's return.
- [`games/common/sdk/sound`](../../../common/sdk/sound) is the driver both
  games build: its C, its handwritten functions and the mixer the game
  copies into IWRAM.
- [`system/mixer_render.s`](../system/mixer_render.s) is the one part of that
  mixer that uses r4 as the system library's functions do, and so is
  counted the system library's own.
- [`games/tla/sound`](../../sound) holds the songs, the samples and the
  table that gives each sound its player.
