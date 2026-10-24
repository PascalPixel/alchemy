# Sound

A town's music plays while the cursor clicks through a menu, a door creaks
open and, after a night at the inn, a short fanfare plays and the music comes
back by itself. The game asks for every sound the same way, with one call and
a number, the song number; the song layer (`sound.c`) decides what each number
means and keeps the music, the fanfares and the sound effects out of each
other's way. Under it is the driver that plays songs on the hardware:
Nintendo's MusicPlayer2000, the sound library many Game Boy Advance games
ship, which [pret](../../glossary.md#pret)'s decompilations call m4a, and its
code here keeps pret's names (`m4aSongNumStart`, `MPlayMain`, `ply_note`). To
the driver every sound, a sound effect included, is a song: a header and
tracks of commands, played on one of eight music players.

## What a song number means

Code anywhere plays a sound with `Sound_PlaySong(s32 id)`, through its
[far-call](../../glossary.md#far-call) name `Sound_PlaySongFar` from outside
the sound [bank](../../glossary.md#bank), as the field does when a chest holds
a Psynergy Stone (`Field_SearchCell`):

```c
Sound_PlaySongFar(MUS_TREASURE_FOUND);
```

The number's range picks what happens: below 80 it is music, from 80 to 99 a
fanfare, and from 100 up a row of the song table, which is how sound effects
play. Each kind has music players of its own, so a sound effect never stops
the music. A few numbers are controls, such as the one that fades the music
out. Asking for the music already chosen (`gCurrentBgm`) does nothing, which
lets a [map](../../glossary.md#map) name its music on every arrival without
restarting it, but also means a song that doesn't loop plays again only after
another piece of music or a fade.

Only the low 16 bits count: the low 12 (`SONG_NUM_MASK`) are the song, and of
the four flags above them only `SONG_FLAG_FADE_IN` is read, which starts
music at volume 0 and fades it in over 64 frames. The music players are
`gMPlayTable`: `MUSIC_PLAYER_FANFARE` (0) with four tracks, `MUSIC_PLAYER_BGM`
(1) with ten, and 2 to 7 with two each.

- From `SONG_SE_FIRST` up, a number is a row of the song table (`gSongTable`,
  `games/tbs/sound/song_table.s`), played on the music player the row names. A
  row that names `MUSIC_PLAYER_SE_LAST` is a floating effect: it takes the
  first idle music player from 7 down to 4, or 7 again if none is idle, so four
  play at once and a fifth cuts off the one on 7.
- From `SONG_FANFARE_FIRST` to 99, a fanfare (`MUS_TREASURE_FOUND`,
  `MUS_INN`), on the fanfare's music player.
- Below 80, music, on the BGM's music player.

`SONG_FADE_OUT_BGM` fades the music out over 112 frames and records
`MUS_SILENCE`, a song with no tracks, as the music chosen, so a later request
for the faded music starts it again. `SONG_KEEP_BGM` keeps what is playing,
and `SONG_FADE_OUT_ACTION_SE` fades `MUSIC_PLAYER_ACTION_SE` out over 48
frames. Their numbers are also song table rows, which never play, since the
controls are tested first. The ranges and controls are named in
`audio_engine_symbols.h` and the songs in `songs.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SONG_NUM_MASK` | `0xfff` | the bits that are the song number |
| `SONG_FLAG_FADE_IN` | `0x1000` | music starts at volume 0 and fades in over 64 frames |
| `SONG_FADE_OUT_BGM` | 17 | fade the music out over 112 frames, once |
| `SONG_KEEP_BGM` | 18 | keep the music that is playing |
| `SONG_FANFARE_FIRST` | 80 | first fanfare; below it, music |
| `SONG_SE_FIRST` | 100 | first song number that is a song table row |
| `SONG_FADE_OUT_ACTION_SE` | 289 | fade `MUSIC_PLAYER_ACTION_SE` out over 48 frames |
| `MUSIC_PLAYER_FANFARE` | 0 | the fanfares' music player, 4 tracks |
| `MUSIC_PLAYER_BGM` | 1 | the music's music player, 10 tracks |
| `MUSIC_PLAYER_MENU_SE` | 2 | the interface's sound effects |
| `MUSIC_PLAYER_ACTION_SE` | 3 | the field's and battle's actions |
| `MUSIC_PLAYER_SE_FIRST` | 4 | the lowest music player a floating effect may take |
| `MUSIC_PLAYER_SE_LAST` | 7 | named by a row, a floating effect |

## The fanfare's return to the music

![At Vale's inn the innkeeper welcomes the party and Isaac agrees to stay; the screen closes to black in a shrinking circle, stays dark while the inn's fanfare plays, opens again, and the innkeeper says "I hope you slept well. Have a nice trip!"](https://alchemy.pascalpixel.com/guides/sound/inn-fanfare.gif)

During a night at the inn the town's music falls silent, the inn's short tune
plays over the black screen, and the music fades back in once it has ended,
from where it had got to. The music never stopped: only its volume went to
zero. Once a frame the song layer checks whether the fanfare has finished
and, when it has, turns the music back up, so the code that played the
fanfare does nothing more. The inn waits for it (`Sound_WaitForFanfare`)
before opening the screen again.

`Sound_PlaySong` sets the music's volume to 0, plays the fanfare on
`gMPlayInfo_Fanfare` and sets `gBgmRestoreDelay` to `BGM_RESTORE_DELAY_FRAMES`.
Once a VBlank, `Sound_UpdateBgmRamps` (called from `System_VBlankHandler`)
counts the delay down, waits until the fanfare's player has no track playing
and sets the music's target volume back to full. The same routine steps the
volume, and the combined tempo and pitch, towards their targets by a fixed
step each VBlank. Full volume is `BGM_UNITY_SCALE`, 1.0 in 8.8 fixed point
(the low 8 bits a fraction in 256ths), and the usual step is 4, so a full fade
takes 64 frames. Nothing in this game moves the tempo and pitch target: its
setter, `Sound_SetBgmTempoPitchTarget`, is reached only through a far-call
entry no code names.

The music commands select only tracks 0 to 7 (`TRACKS_ALL`), though
`gMPlayInfo_BGM` has 10, so tracks 8 and 9 keep their volume under a fanfare.
Nine songs use them: `MUS_KALAY`, `MUS_ELEMENTAL_STARS`, `MUS_DUNGEON`,
`MUS_CONFRONTATION`, `MUS_UNUSED`, `MUS_TOLBI_BOUND_SHIP`, `MUS_WORLD_MAP`,
`MUS_VENUS_LIGHTHOUSE` and `MUS_MAIN_MENU`. The ramps are the song layer's
`gBgmVolume`, `gBgmVolumeTarget` and `gBgmVolumeStep`, and `gBgmTempoPitch`,
`gBgmTempoPitchTarget` and `gBgmTempoPitchStep`.

| Name | Value | Meaning |
| --- | --- | --- |
| `BGM_RESTORE_DELAY_FRAMES` | 10 | VBlanks after a fanfare starts before the music waits for it to end |
| `BGM_UNITY_SCALE` | `0x100` | full volume, and the song's own tempo and pitch: 1.0 in 8.8 fixed point |
| `BGM_DEFAULT_RAMP_STEP` | 4 | a ramp's step a VBlank: a full fade takes 64 frames |
| `TRACKS_ALL` | `0xff` | the track mask of every music command: tracks 0 to 7 |
| `BGM_WAIT_TIMEOUT_FRAMES` | 300 | frames `Sound_WaitForSong` waits before giving up |

## The driver's ident

The game starts and stops songs during the frame, and the mixer runs in the
blank, which can arrive in the middle of one of those calls. If the mixer read
the driver while a call was halfway through changing it, it could play
nonsense, so the two share a lock: a call takes it while it works, and the
mixer, finding it taken, leaves that frame's driver work for the next.

The lock is one word, `SoundInfo.ident`, the first field of the driver's work
(each music player has its own, `MusicPlayerInfo.ident`). It is also a
signature: `ID_NUMBER`'s bytes spell `Smsh`. `SoundInit` sets it to 0 while it
clears the driver's work and to `ID_NUMBER` once the driver is ready. Every
library call that changes the driver's state checks the word is exactly
`ID_NUMBER`, adds 1 while it works and puts `ID_NUMBER` back; the mixer's
entry (`Mixer_Enter`, last in the VBlank handler) runs only on `ID_NUMBER`. A
call made while another holds the lock returns without doing anything: it is
dropped, not delayed. `m4aSoundVSyncOff` suspends DirectSound, the hardware's
two channels for recorded samples, by adding 10 rather than 1, so a suspended
driver can be told from a busy one, and `m4aSoundVSyncOn` takes the 10 off.

| Name | Value | Meaning |
| --- | --- | --- |
| `ID_NUMBER` | `0x68736D53` | the driver is ready and free; its bytes spell `Smsh` |
| `ID_NUMBER` + 1 | | a library call holds the driver |
| `ID_NUMBER` + 10 | | `m4aSoundVSyncOff` has suspended the driver |
| 0 | | `SoundInit` is clearing the driver's work |

## The mixer's code is chosen per song

Some of the game's sounds are recordings, samples, which the driver mixes in
software a frame at a time into the buffers the hardware plays from. The last
step of that mixing decides how much of each frame carries into the next, in
effect an echo MusicPlayer2000 calls reverb, and the game keeps five versions
of it. Starting a piece of music copies one into place: the title and ending
music use one, most music another, a link battle a third.

`Sound_LoadReverbCode` copies one of `SOUND_REVERB_CODE_COUNT` versions
(`IwramSoundReverbCodes`, in `system/entry.s`) over `IwramSoundReverbCode`, in
the middle of the resident code that renders the mix in
[IWRAM](../../glossary.md#iwram-and-ewram), which `Mixer_Enter` jumps into
each VBlank at `IwramSoundMixEntry`. A link battle loads `SOUND_REVERB_NONE`
with its music (`Battle_Run`), the Battle Arena `SOUND_REVERB_NORMAL`, and the
audio test steps through all five with Select. The driver's own mixer,
`SoundMainRAM` (`mixer.s`), is separate code at `Ram_SoundMixer`, which the
reverb codes never touch.

| Name | Value | Loaded by | What it carries into the next frame |
| --- | --- | --- | --- |
| `SOUND_REVERB_MONO` | 0 | the audio test | one value for both channels, from the two buffers together |
| `SOUND_REVERB_CROSSED` | 1 | the audio test | each channel's old samples crossed with the other's |
| `SOUND_REVERB_NORMAL` | 2 | most music, the Battle Arena | a quarter of the mix and an eighth of each buffer's old samples |
| `SOUND_REVERB_LONG` | 3 | `MUS_ENDING`, `MUS_TITLE`, `MUS_ENDING_LOGO` | an eighth of the mix and a quarter of the old samples |
| `SOUND_REVERB_NONE` | 4 | a link battle | nothing: the mix buffer is cleared |

## Design

The driver is Nintendo's library, compiled the way pret compiles its copy, so
it keeps pret's names from `include/gba/m4a_internal.h`, `src/m4a.c` and
`src/m4a_1.s`: its structures (`SoundInfo`, `MusicPlayerInfo`,
`MusicPlayerTrack`, `SoundChannel`, `CgbChannel`, `ToneData`, `SongHeader`,
`MusicPlayer`, `Song`), its tables (`gMPlayTable`, `gSongTable`,
`gMPlayJumpTable`, `gXcmdTable`) and its routines (`m4aSoundInit`,
`MPlayStart`, the song commands `ply_goto` to `ply_xswee`), so a reader who
knows pret's games knows these. This copy is older than pret's: it has no
cries, its extended commands stop at `ply_xswee`, and its jump-table indices
are named here as `MPLAY_JUMP_` constants. What Camelot added keeps a plain
name in the project's style: the VBlank's way into the game's own IWRAM mixer
(`Mixer_Enter`, `Mixer_StartFrame`, `Mixer_EnterCallViaR3`), the fanfare's and
the sound effects' music players (`gMPlayInfo_Fanfare`, `gMPlayInfo_MenuSE`,
`gMPlayInfo_ActionSE`, `gMPlayInfo_SE4` to `gMPlayInfo_SE7`), and the song
layer in `sound.c`.

`engine.c`, `audio.c` and `tables.c` are compiled with pret's
[`old_agbcc`](../../glossary.md#the-compilers) (the Makefile's `SOUND_C`), as
pret builds its MusicPlayer2000 C; the rest with the game's compiler. The song
layer is the sound bank; the driver follows the banks in the ROM's tail, with
`mixer.s` loaded into IWRAM.

## How it fits with the rest of the game

Every part of the game plays sound through `Sound_PlaySongFar`: maps name
their music on entry, menus click, battle animations play their impacts. The
VBlank handler in [Runtime](../runtime/README.md#the-frame-loop) steps the
music's ramps and enters the mixer every frame, and the mixer's resident code
is part of what start-up copies into IWRAM
([System](../system/README.md#memory-map)). The songs and samples themselves
are data in `games/tbs/sound`.

## Limits

- Eight music players; `gMPlayInfo_BGM` has 10 tracks, of which the song
  layer's ramps and fanfare silence reach 8.
- Four sound effects that name `MUSIC_PLAYER_SE_LAST` play at once.
- Song numbers are 12 bits, 0 to 4,095.

## Words used here

- **song number**: the number `Sound_PlaySong` takes: music, a fanfare, a song
  of the song table, or a control.
- **music player** and **track**: a music player (`struct MusicPlayerInfo`)
  plays one song, whose tracks (`struct MusicPlayerTrack`) each run a stream
  of song commands.
- **note**: a sound a track starts, on a PCM channel the mixer renders into
  DirectSound or on one of the four CGB channels, the tone generators built
  into the hardware (channel 4 is noise).
- **floating effect**: a song table row that names music player 7, played on
  the first idle music player from 7 down to 4.
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **ident**: `SoundInfo.ident` and `MusicPlayerInfo.ident`, the driver's and
  each music player's signature and lock word (`ID_NUMBER`).
- **reverb code**: one of the five versions of the IWRAM mixer's last step
  (`enum SoundReverbCode`), copied in when music starts.

## Evidence and open questions

The driver's names are pret's for MusicPlayer2000, each checked against the
same routine in pokeemerald's `src/m4a.c`, `src/m4a_1.s` and
`src/m4a_tables.c`. Where pret has no name the name is ours: `MPlayStartFade`
(the fade setup pret writes out in each fade call), `ld_r3_tp_adr_i_r2` (the
second entry of `ld_r3_tp_adr_i`), `Mixer_CallViaR3` (the `bx r3` at the end of
`SoundMainRAM`), `SoundMainRAM_Rom` (the mixer's ROM copy), the `MPLAY_JUMP_`
indices and the music players beyond `gMPlayInfo_BGM`. The song layer's names
come from what `Sound_PlaySong` does with each range, and the songs' from
`songs.h`, named as pret names its songs: music by the place or moment the
player knows it from (`MUS_DJINN_LESSON`), a sound effect by what it sounds
like or the moment it marks (`SE_THUMP`).

The reverb codes are the last step of `IwramSoundRenderFrame`'s loop. Each
pass loads four words of the mix buffer and writes them out to the two
hardware buffers, reading back first what those buffers held the previous time
round (`fp` from the first, `ip` from the second, `0x630` bytes on); the reverb
code then works out what the mix buffer starts the next frame with, from this
frame's mix and those older samples. `SOUND_REVERB_LONG` is the title's music
and the ending's: `MUS_TITLE` over the title intro, `MUS_ENDING_LOGO` over the
Golden Sun logo after the ending, and `MUS_ENDING` through the credits and as
the Lemurian ship sails off (`menu/title.c`, `map/lemurian_ship.c`).

A library call is dropped only when it meets another that holds the lock, and
library calls all run on the main thread except from a VBlank
[task](../../glossary.md#task). A call graph of the English edition's build
finds one VBlank task that reaches the library: `LuckyWheels_RunFrame`, the
Lucky Wheels frame. While Lucky Wheels runs, the main thread only draws and
waits, and no message types, so in practice nothing is dropped.

A floating effect searches only music players 4 to 7 because 2 and 3 have
sounds of their own in the song table: `MUSIC_PLAYER_MENU_SE` the interface's
(`SE_MENU_CONFIRM`, `SE_MENU_CANCEL`, `SE_MENU_ERROR`, `SE_SHOP_PURCHASE`) and
`MUSIC_PLAYER_ACTION_SE` the field's and battle's actions (`SE_CAST`,
`SE_IMPACT`, `SE_HEAVY_IMPACT`, `SE_DOOR_OPEN`), so a floating effect never
cuts those off.

The driver's structures keep pret's names for the bytes nothing uses: the
notes' `dummy1` to `dummy5`, `xpi` and `xpc`, a track's `gap`, `timer` and
`unk_3C`, a music player's `gap`, and the driver work's `mode`, `gap` and
`gap2`. No C in the game names them, and the library assembly (`mixer.s`,
`clear_buffer.s`, `channel_update_entry.s`) loads and stores at none of their
offsets. pret's later driver uses two of them (a note's `xpi` for the last
compressed-sample block decoded, and a track's `unk_3C` copied into a new
note); the game's driver is the older one, with neither step.
