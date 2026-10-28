# Sound

A town's music plays while the cursor clicks through a menu, a door creaks open
and, after a night at the inn, a short jingle plays and the music comes back by
itself. The game asks for every sound the same way, with one call and a number,
the sound number; the sound layer (`sound.c`) decides what each number means and
keeps the music, the jingles and the sound effects out of each other's way.
Under it is the driver that plays them on the hardware: Nintendo's
MusicPlayer2000, the sound library many Game Boy Advance games ship, which
[pret](../../glossary.md#pret)'s decompilations call m4a, and its code here
keeps pret's names. To the driver every sound, a sound effect included, is a
song in the driver's terms: a header and tracks of notes and commands, played on
one of eight music players.

## What a sound number means

Code anywhere plays a sound with one call and a number, as the field does
when a chest holds a Psynergy Stone. The number's range picks what happens:
below 80 it is music, from 80 to 99 a jingle, and from 100 up a sound
effect. Each kind has music players of its own, so a sound effect never stops
the music. A few numbers are orders rather than sounds: one fades the music
out over about two seconds, one keeps whatever is playing, one fades out the
sound of the field's and battle's actions. Asking for the music already
chosen does nothing, which lets a [scene](../../glossary.md#scene) name its music
on every arrival without restarting it, but also means a piece that doesn't
loop plays again only after other music or a fade.

The music has a player with ten tracks and the jingles one with four. Of the
six players left, two belong to particular kinds of sound effect, the menus'
clicks and the actions' impacts and doors, so nothing cuts those off. The
other four are shared: a sound effect takes the first of them that is idle,
so four can play at once, and a fifth cuts off one of them.

The call is `Sound_Play`, or `Sound_PlayFar` from outside the sound
[bank](../../glossary.md#bank); its ranges are `SOUND_JINGLE_FIRST` and
`SOUND_SE_FIRST`, the players `gMPlayTable`, and the sound effects' rows
`gSongTable`. The sounds are named in `sounds.h`.

## The jingle's return to the music

![At Vale's inn the innkeeper welcomes the party and Isaac agrees to stay; the screen closes to black in a shrinking circle, stays dark while the inn's jingle plays, opens again, and the innkeeper says "I hope you slept well. Have a nice trip!"](https://alchemy.pascalpixel.com/guides/sound/inn-fanfare.gif)

During a night at the inn the town's music falls silent, the inn's short tune
plays over the black screen, and the music fades back in once it has ended,
from where it had got to. The music never stopped: only its volume went to
zero. Once a VBlank the sound layer checks whether the jingle has finished
and, when it has, turns the music back up over about a second, so the code
that played the jingle does nothing more. The inn waits for it before
opening the screen again.

The silence reaches only the music's first eight tracks. Nine pieces of music
use more, among them the world map's and Kalay's, and their extra tracks keep
playing under a jingle.

The volume is stepped by `Sound_UpdateBgmRamps`, which the blank runs every
VBlank; the inn waits with `Sound_WaitForJingle`.

## The driver's ident

The game starts and stops sounds during the frame, and the mixer runs in the
blank, which can arrive in the middle of one of those calls. If the mixer read
the driver while a call was halfway through changing it, it could play
nonsense, so the two share a lock: a call takes it while it works, and the
mixer, finding it taken, leaves that VBlank's work for the next. A second call
that finds the lock taken is simply dropped, not delayed; in practice no two
calls ever meet (see [Evidence](#evidence)).

The lock is one word, also a signature: its value when free, `ID_NUMBER`,
spells `Smsh` in memory. It is the first field of the driver's work,
`SoundInfo.ident`, and of each music player's.

## The mixer's code is chosen per piece of music

Some of the game's sounds are recordings, samples, which the driver mixes in
software a VBlank at a time into the buffers the hardware plays from. The last
step of that mixing decides how much of each VBlank's sound carries into the
next, in effect an echo, which MusicPlayer2000 calls reverb, and the game
keeps five versions of it. Starting a piece of music copies one into place:
the title and ending music carry the most, most music a little less, and a
link battle none at all. The audio test can step through all five.

The versions are `enum SoundReverbCode`, copied into place by
`Sound_LoadReverbCode`. When the mixer clips a sample that comes out too loud,
it also marks it in `gSoundMixerClipped`, which only the audio test looks at.

## Design

The driver is Nintendo's library, compiled the way pret compiles its copy, so
it keeps pret's names for its structures, tables and routines (`SoundInfo`,
`MusicPlayerInfo`, `gMPlayTable`, `m4aSoundInit`, the song commands `ply_goto`
to `ply_xswee`), and a reader who knows pret's games knows these. This copy
is older than pret's: it has no cries, and its extended commands stop at
`ply_xswee`. What Camelot added takes a plain name in the project's style: the
blank's way into the game's own mixer (`Mixer_Enter`), the jingle's and the
sound effects' music players (`gMPlayInfo_Jingle` and the rest), and the
sound layer in `sound.c`.

`engine.c`, `audio.c` and `tables.c` are compiled with pret's
[`old_agbcc`](../../glossary.md#the-compilers), as pret builds its
MusicPlayer2000 C; the rest with the game's compiler. The sound layer is the
sound bank; the driver follows the banks in the ROM's tail, and its mixer
runs from [IWRAM](../../glossary.md#iwram-and-ewram).

## How it fits with the rest of the game

Every part of the game plays sound through `Sound_PlayFar`: scenes name their
music on entry, menus click, battle effects play their impacts. The blank's code
in [Runtime](../runtime/README.md#the-frame-loop) steps the music's volume and
enters the mixer every VBlank, and the mixer's resident code is part of what
start-up copies into IWRAM
([Cartridge and memory](../cartridge/README.md#memory-map)). The sounds and
samples themselves are data in `games/tbs/sound`.

## Limits

- Eight music players; the music's player has 10 tracks, of which a jingle
  silences 8.
- Four shared sound effects play at once.
- Sound numbers run from 0 to 4,095.

## Words used here

- **sound number**: the number `Sound_Play` takes: music, a jingle, a sound
  effect from the song table, or an order. The Lost Age's sound test sorts the
  same sounds as Music, Jingle and SE.
- **song**: the driver's word for one sound's data, a header and its tracks
  (`struct Song`, `gSongTable`), whatever kind of sound it is.
- **music player** and **track**: a music player plays one song, whose tracks
  each run a stream of notes and commands.
- **note**: a sound a track starts, either a sample the mixer renders or one
  of the four tone generators built into the hardware.
- **floating effect**: a sound effect that takes the first idle one of the
  four shared music players.
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **ident**: the driver's and each music player's signature and lock word.
- **reverb code**: one of the five versions of the mixer's last step, copied
  in when music starts.

## Evidence

The driver's names are pret's for MusicPlayer2000, each checked against the
same routine in pokeemerald's `src/m4a.c`, `src/m4a_1.s` and
`src/m4a_tables.c`. Where pret has no name the name is ours: `MPlayStartFade`
(the fade setup pret writes out in each fade call), `ld_r3_tp_adr_i_r2` (the
second entry of `ld_r3_tp_adr_i`), `Mixer_CallViaR3` (the `bx r3` at the end of
`SoundMainRAM`), `SoundMainRAM_Rom` (the mixer's ROM copy), the `MPLAY_JUMP_`
indices and the music players beyond `gMPlayInfo_BGM`. The sound layer's names
come from what `Sound_Play` does with each range, and the sounds' from
`sounds.h`: music by the place or moment the
player knows it from (`MUS_DJINN_LESSON`), a sound effect by what it sounds
like or the moment it marks (`SE_THUMP`).

`gSoundMixerClipped` is named from the store that sets it: the last
instruction of `IwramSoundNormalizeChannels` before it returns to the render
loop is `str lr, [pc]`, which in the link lands on that word, the one after
the routine, with the clip mask `0x80008000` the routine has just built.

A library call is dropped only when it meets another that holds the lock, and
library calls all run outside the VBlank interrupt except from a VBlank
[task](../../glossary.md#task). A call graph of the English edition's build
finds one VBlank task that reaches the library: `LuckyWheels_Update`, which
runs Lucky Wheels once a VBlank. While Lucky Wheels runs, the code outside the
interrupt only draws and waits, and no message types, so in practice nothing
is dropped.

The driver's structures keep pret's names for the bytes nothing uses: the
notes' `dummy1` to `dummy5`, `xpi` and `xpc`, a track's `gap`, `timer` and
`unk_3C`, a music player's `gap`, and the driver work's `mode`, `gap` and
`gap2`. No C in the game names them, and the library assembly (`mixer.s`,
`clear_buffer.s`, `channel_update_entry.s`) loads and stores at none of their
offsets. pret's later driver uses two of them (a note's `xpi` for the last
compressed-sample block decoded, and a track's `unk_3C` copied into a new
note); the game's driver is the older one, with neither step.
