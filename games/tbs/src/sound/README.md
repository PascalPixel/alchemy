# Sound

A town's music plays while the cursor clicks through a menu and, after a night
at the inn, a short jingle plays and the music comes back by itself. The game
asks for every sound the same way, with one call and a number, the sound
number; the sound layer (`sound.c`) decides what each number means and keeps
the music, the jingles and the sound effects out of each other's way. Under it
is the driver that plays them on the hardware, Nintendo's MusicPlayer2000. To
the driver every sound, a sound effect included, is a song: a header and
tracks of notes and commands, played on one of eight music players.

## What a sound number means

The number's range picks what happens: below 80 it is music, from 80 to 99 a
jingle, and from 100 up a sound effect. Each kind has music players of its
own, so a sound effect never stops the music. A few numbers are orders rather
than sounds: one fades the music out over about two seconds, one keeps
whatever is playing, and one fades out the sounds of the field and of battle,
such as footsteps and impacts. Asking for the music already chosen does
nothing, which lets a [scene](../../glossary.md#scene) name its music on every
arrival without restarting it, but also means a piece that doesn't loop plays
again only after other music or a fade. New music starts at full volume, or,
when the call asks for it, fades in over about a second.

The music has a player with ten tracks and the jingles one with four. Of the
six players left, one belongs to the menus' sounds and one to footsteps,
tremors, water and impacts in the field and in battle
(`MUSIC_PLAYER_ACTION_SE`), so nothing else cuts those off. The other four are
shared. A sound effect whose row names the last of the four takes the first
idle one, counting down from the last; with all four busy, it cuts off the
last one's sound. Others play on the player their row names, cutting off what
was there. At most four play on these players at once.

The call is `Sound_Play`, or `Sound_PlayFar` from outside the sound
[bank](../../glossary.md#bank). Its ranges are `SOUND_JINGLE_FIRST` and
`SOUND_SE_FIRST`, its orders `SOUND_FADE_OUT_BGM`, `SOUND_KEEP_BGM` and
`SOUND_FADE_OUT_ACTION_SE`, its one flag `SOUND_FLAG_FADE_IN`, and the
players' ids `MUSIC_PLAYER_JINGLE` to `MUSIC_PLAYER_SE_LAST`. The players are
listed in `gMPlayTable` and the sounds, each with its player, in
`gSongTable`; `sounds.h` names them.

## How the music comes back after a jingle

During a night at the inn the town's music falls silent, the inn's short tune
plays over the black screen, and the music fades back in once the tune has
ended. The music never stopped: only its volume went to zero, so it comes back
at the point it has played on to. Once a
[VBlank](../../glossary.md#vblank) the sound layer checks whether the jingle
has finished and, when it has, turns the music back up over about a second, so
the code that played the jingle does nothing more. The inn waits for it before
opening the screen again, as do the sanctum, the Lucky Dice winnings at Tolbi
and the Lemurian ship.

The silence reaches only the music's first eight tracks. Nine pieces of music
use more, among them the world map's and Kalay's, and their extra tracks keep
playing under a jingle.

`Sound_Play` sets the jingle's countdown, `gBgmRestoreDelay`, to ten VBlanks;
`Sound_UpdateBgmRamps`, which the VBlank handler runs, counts it down to 1,
holds it there until the jingle's player falls silent, then clears it and
steps the music's volume back to full. The inn and the others wait a frame at
a time with `Sound_WaitForJingle` until the jingle has ended and the music
starts to come back. Vale's Sanctum, after the jingle for accepting the Great
Healer's quest, waits the same way with `Sound_WaitForBgmRestore`, which gives
up after `BGM_WAIT_TIMEOUT_FRAMES`, about five seconds.

## The driver's lock

The game starts and stops sounds outside the interrupt, and the mixer runs at
VBlank, which can arrive in the middle of one of those calls. The VBlank
handler also calls the driver itself: it steps the music's volume, tempo and
pitch through the same driver calls (`Sound_UpdateBgmRamps`), Lucky Wheels
plays its sounds from a VBlank [task](../../glossary.md#task)
(`LuckyWheels_Update`), and the mixer reads every player. If one of these
read the driver while a call was halfway through changing it, it could play
nonsense, so they share a lock: a call takes it while it works, and the mixer,
finding it taken, skips mixing for that VBlank. A call that finds the lock
taken is dropped, not delayed. Since the interrupt finishes before the code it
interrupted goes on, only a call made inside the interrupt can be dropped
(see [Evidence](#evidence)).

The lock is one word that is also a signature: its value when free,
`ID_NUMBER`, spells `Smsh` in memory. A call adds 1 to it while it works and
puts it back after. The driver's work has one, its first field
`SoundInfo.ident`, and each music player its own, `MusicPlayerInfo.ident`;
`Mixer_Enter` checks the driver's before mixing.

## The reverb code for each piece of music

Some of the game's sounds are recordings, samples, which the driver mixes in
software a VBlank at a time into the buffers the hardware plays from. The last
step of that mixing decides how much of each VBlank's sound carries into the
next, in effect an echo, which MusicPlayer2000 calls reverb, and the game
keeps five versions of that step's machine code in ROM. Starting a piece of
music copies one over the mixer's last step in
[IWRAM](../../glossary.md#iwram-and-ewram): the title and ending music carry
the most and other music a little less. A link battle then loads the version
that carries nothing, and the Battle Arena the usual one. The other two
versions only the sound test (`SoundTest_Run`) loads, and nothing in the game
calls the sound test.

The versions are `enum SoundReverbCode`. `Sound_LoadReverbCode` copies the
chosen one, 152 bytes, from `IwramSoundReverbCodes` over
`IwramSoundReverbCode`. When the mixer clips a sample that comes out too loud,
it also marks it in `gSoundMixerClipped`, which only the sound test reads.

## Design

The sound layer decides and the driver plays. The sound layer is the sound
bank, compiled with the game's compiler. The driver follows the banks in the
ROM's tail; `driver.c`, `audio.c` and `tables.c` are compiled with
[`old_agbcc`](../../glossary.md#the-compilers), and its mixer runs from IWRAM.

## How it fits with the rest of the game

Every part of the game plays sound through `Sound_PlayFar`: scenes name their
music on entry, menus click, battle effects play their impacts. The VBlank
handler in [Runtime](../runtime/README.md#the-frame-loop) steps the music's
volume and enters the mixer every VBlank, and the mixer's resident code is
part of what start-up copies into IWRAM
([Cartridge and memory](../cartridge/README.md#memory-map)). The sounds and
samples themselves are data in `games/tbs/sound`.

## Limits

- Eight music players; the music's player has 10 tracks, of which a jingle
  silences 8.
- Four sound effects on the shared players play at once.
- Sound numbers run from 0 to 311, one for each row of `gSongTable`; the call
  reads the low 12 bits as the number, and the bit above them,
  `SOUND_FLAG_FADE_IN`, makes new music fade in over about a second.

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
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **lock** (`ident`): the driver's and each music player's signature word,
  raised while a call changes them.
- **reverb code**: one of the five versions of the mixer's last step, copied
  in when music starts.

## Evidence

The driver is Nintendo's, so its code keeps the m4a spellings
[pret](../../glossary.md#pret) gives its structures, tables and routines
(`SoundInfo`, `MusicPlayerInfo`, `gMPlayTable`, `m4aSoundInit`, the song
commands `ply_goto` to `ply_xswee`), each checked against the same routine in
pret's m4a sources. Where pret has no name the name is ours: `MPlayStartFade`
(the fade setup each fade call shares), `ld_r3_tp_adr_i_r2` (the second entry
of `ld_r3_tp_adr_i`), `Mixer_CallViaR3` (the `bx r3` at the end of
`SoundMainRAM`), `SoundMainRAM_Rom` (the mixer's ROM copy) and the
`MPLAY_JUMP_` indices. What Camelot added takes a plain name in the project's
style: the VBlank handler's way into the mixer (`Mixer_Enter`), the music
players beyond `gMPlayInfo_BGM` (`gMPlayInfo_Jingle` and the rest), and the
sound layer in `sound.c`. The sound layer's names come from what `Sound_Play`
does with each range, and the sounds' from `sounds.h`: music by the place or
moment the player knows it from (`MUS_DJINN_LESSON`), a sound effect by what
it sounds like or the moment it marks (`SE_THUMP`).

`gSoundMixerClipped` is named from the store that sets it: near the end of
`IwramSoundNormalizeChannels`, `str lr, [pc]` writes the clip mask the routine
has just built into the word that follows the routine.

A dropped driver call costs little. Each of the VBlank handler's ramp steps
sets the whole volume, or the whole tempo and pitch, again, so a dropped step
is made up by the next. Only a ramp's last step can be lost, which leaves the
music one step short of its target: with the usual step,
`BGM_DEFAULT_RAMP_STEP`, a sixty-fourth of full volume (`BGM_UNITY_SCALE`). A
sound `LuckyWheels_Update` asks for while its player's lock is taken does not
play.

The driver's structures keep their m4a names for the bytes nothing uses: the
notes' `dummy1` to `dummy5`, `xpi` and `xpc`, a track's `gap`, `timer` and
`unk_3C`, a music player's `gap`, and the driver work's `mode`, `gap` and
`gap2`. No C in the game names them, and the driver's assembly (`mixer.s`,
`clear_buffer.s`, `channel_update_entry.s`) loads and stores at none of their
offsets.
