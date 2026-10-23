# Sound

A town's music plays while the cursor clicks through a menu, a door creaks
open and, when a chest is opened, a short fanfare plays and the music comes
back by itself once it has finished. All of that is the sound folder's work.
The game asks for every sound the same way, with one call and a number, the
song number; the song layer (`sound.c`) decides what each number means and keeps the
music, the fanfares and the sound effects out of each other's way. Under it is
the driver that plays songs on the hardware: Nintendo's MusicPlayer2000, the
sound library many Game Boy Advance games ship, which [pret](../../glossary.md#pret)'s decompilations
call m4a, and its code here keeps pret's names for it (`m4aSongNumStart`,
`MPlayMain`, `ply_note`). In it every sound, a sound effect included, is a
song: a header and tracks of commands, played on one of the driver's music
players.

The driver has eight music players, each of which plays one song at a time,
and each kind of sound has music players of its own, so a sound effect never
stops the music. The song layer and the driver meet once a [frame](../../glossary.md#frame) in the VBlank handler,
which steps the music's volume towards its target and then runs the mixer
([Runtime](../runtime/README.md#the-frame-loop)).

## What a song number means

A song number says whether it is music, a fanfare, a sound from the song
table or a control.

Code anywhere in the game plays a sound by calling `Sound_PlaySong(s32
id)` with a song number, and it returns nothing. Every caller outside the sound [bank](../../glossary.md#bank) uses its
[far-call](../../glossary.md#far-call) name, `Sound_PlaySongFar`, as the field does when a chest
holds a Psynergy Stone (`Field_SearchCell`):

```c
Sound_PlaySongFar(MUS_TREASURE_FOUND);
```

The number's range picks what happens: below 80 it is music, from 80 to 99 a
fanfare, and from 100 up a row of the song table, which is how sound effects
play. A few numbers are controls rather than sounds, such as the one that
fades the music out.

Only a song number's low 16 bits count: the low 12 (`SONG_NUM_MASK`) are the song,
0 to 4,095, and the four above them are flags, of which only
`SONG_FLAG_FADE_IN` is read; it starts music at volume 0 and fades it
in over 64 frames.

The driver has eight music players (`gMPlayTable`): `MUSIC_PLAYER_FANFARE`
(0) with four tracks, `MUSIC_PLAYER_BGM` (1) with ten, and music players 2 to
7 with two each. What a song number plays depends on its range:

- From `SONG_SE_FIRST` (100) up, a number is a row of the song table
  (`gSongTable`), played on the music player the row names; sound effects
  such as `SE_SHOP_PURCHASE` and `SE_MENU_CURSOR_MOVE` are rows. A row that
  names music player 7, `MUSIC_PLAYER_SE_LAST`, means any free music player
  from 7 down to 4 (`MUSIC_PLAYER_SE_FIRST`): a floating effect. It takes the
  first idle one counting down, and 7 again if none is idle, so four floating
  effects play at once and a fifth cuts off the one on music player 7.
- From `SONG_FANFARE_FIRST` (80) to 99, a number is a fanfare, such as
  `MUS_TREASURE_FOUND` or `MUS_PUZZLE_SOLVED`, played on the fanfare's music
  player.
- Below 80, a number is music, played on the BGM's music player.

Three numbers are controls. `SONG_FADE_OUT_BGM` fades the music out over 112
frames (16 steps 7 frames apart); a second fade does nothing until new
music has started. The fade records `MUS_SILENCE`, a song with no tracks, as
the music chosen, so a later request for the faded music starts it again. `SONG_KEEP_BGM` keeps
the music that is playing. `SONG_FADE_OUT_ACTION_SE` fades music player 3,
`MUSIC_PLAYER_ACTION_SE`, out over 48 frames. The three values are also rows of the song table (17, 18 and 289),
and those rows never play, since the controls are tested first.

Asking for the music already chosen, which `gCurrentBgm` records, does nothing.
That lets a [map](../../glossary.md#map) name its music every time the [party](../../glossary.md#the-party) enters without
restarting it, but it also holds when that song has ended: a song that
doesn't loop plays again only after another piece of music or a fade.

`Sound_PlaySong` reads the song table, `gSongTable` (`games/tbs/sound/song_table.s`), whose row holds the
song's header and its music player, and the music players, `gMPlayTable`
(`games/tbs/sound/music_player_table.s`). The ranges and controls are named in
`audio_engine_symbols.h` and the songs in `songs.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SONG_NUM_MASK` | `0xfff` | the bits that are the song number |
| `SONG_FLAG_FADE_IN` | `0x1000` | flag: music starts at volume 0 and fades in over 64 frames |
| `SONG_FADE_OUT_BGM` | 17 | fade the music out over 112 frames, once |
| `SONG_KEEP_BGM` | 18 | keep the music that is playing |
| `SONG_FANFARE_FIRST` | 80 | first fanfare; below it, music |
| `SONG_SE_FIRST` | 100 | first song number that is a song table row |
| `SONG_FADE_OUT_ACTION_SE` | `0x121` (289) | fade `MUSIC_PLAYER_ACTION_SE` out over 48 frames |
| `MUSIC_PLAYER_FANFARE` | 0 | the fanfares' music player, 4 tracks |
| `MUSIC_PLAYER_BGM` | 1 | the music's music player, 10 tracks |
| `MUSIC_PLAYER_MENU_SE` | 2 | the interface's sound effects |
| `MUSIC_PLAYER_ACTION_SE` | 3 | the field's and battle's actions |
| `MUSIC_PLAYER_SE_FIRST` | 4 | the lowest music player a floating effect may take |
| `MUSIC_PLAYER_SE_LAST` | 7 | named by a row, a floating effect: the first idle music player from 7 down to 4 |

## The fanfare's return to the music

A fanfare silences the music without stopping it, and the music comes back by
itself once the fanfare has ended.

When the party finds an item, the treasure fanfare plays and the town's music
falls silent; a moment after the fanfare ends, the music fades back in from
where it had got to. The music never stopped: only its volume went to zero.
Once a frame the song layer checks whether the fanfare has finished and, when
it has, turns the music back up. The code that played the fanfare does
nothing more.

`Sound_PlaySong` sets the music's volume to 0, plays the fanfare on
`gMPlayInfo_Fanfare` and sets `gBgmRestoreDelay` to `BGM_RESTORE_DELAY_FRAMES`;
the music keeps playing, silently.

Once a VBlank, `Sound_UpdateBgmRamps` (called from
`System_VBlankHandler`) counts the delay down to 1, then waits until
`gMPlayInfo_Fanfare` has no track playing and sets the music's target volume back
to full. The same routine steps the volume, and the combined tempo and
pitch, towards their targets by a fixed step each VBlank, and stops on the
target when a step would cross it. Nothing in this game moves the tempo and
pitch target: its setter, `Sound_SetBgmTempoPitchTarget`, is reached only through a
far-call entry no code names. Full volume is `BGM_UNITY_SCALE`, 1.0
in the ramps' 8.8 fixed point (a number whose low 8 bits are a fraction in
256ths, so 1.0 is 256), and the usual step `BGM_DEFAULT_RAMP_STEP`
is 4, so a full fade takes 64 frames.

Songs with more than eight tracks are not fully silenced by a fanfare. The
music commands here select only tracks 0 to 7 (`TRACKS_ALL`, one bit a
track), though `gMPlayInfo_BGM` has 10, so tracks 8 and 9 keep their volume
and pitch through the ramps and under a fanfare. Nine songs use them: music
6, 22, 28, 29, 40 to 43 (`MUS_040` to `MUS_VENUS_LIGHTHOUSE`) and 74. The
original game does the same, since this is its code.

The ramps are globals of the song layer: `gBgmVolume`, `gBgmVolumeTarget`
and `gBgmVolumeStep`, and `gBgmTempoPitch`, `gBgmTempoPitchTarget` and
`gBgmTempoPitchStep` for tempo and pitch together.

| Name | Value | Meaning |
| --- | --- | --- |
| `BGM_RESTORE_DELAY_FRAMES` | 10 | VBlanks after a fanfare starts before the music waits for it to end |
| `BGM_UNITY_SCALE` | `0x100` | full volume, and the song's own tempo and pitch: 1.0 in 8.8 fixed point |
| `BGM_DEFAULT_RAMP_STEP` | 4 | a ramp's step a VBlank: a full fade takes 64 frames |
| `TRACKS_ALL` | `0xff` | the track mask of every music command: tracks 0 to 7 |
| `BGM_WAIT_TIMEOUT_FRAMES` | 300 | frames `Sound_WaitForSong` waits before giving up, about five seconds |

## The driver's ident

One word in the driver's memory, its ident, keeps the game's calls and the
mixer from changing the driver's state at the same time: it works as a lock.

The game starts and stops songs during the frame, and the mixer runs in the
blank, which can arrive in the middle of one of those calls. If the mixer read
the driver while a call was halfway through changing it, it could play
nonsense. So the two share a lock: a call takes it while it works, and the
mixer, finding it taken, leaves that frame's driver work for the next one.

The game calls the driver's library (`audio.c`, `engine.c`) from its frame
loop, and the VBlank handler enters the mixer last of all
(`Mixer_Enter`). They share one word, `SoundInfo.ident`, the first field of
the driver's work, and each music player has an ident of its own
(`MusicPlayerInfo.ident`).

The word is both a signature and a lock. `ID_NUMBER`'s four bytes spell
`Smsh` in memory order. `SoundInit` sets the word to 0 while it
clears the driver's work and to `ID_NUMBER` once the driver is ready. Every
library call that changes the driver's state checks that the word is exactly
`ID_NUMBER`, adds 1 while it works, and puts `ID_NUMBER` back. The mixer's
entry runs only when the word is exactly `ID_NUMBER`, and its exit stores
the signature again.

A VBlank that arrives during a library call therefore skips that frame's
driver work instead of reading half-changed state, and a library call made
while the driver isn't ready, or while another call holds the lock, returns
without doing anything: it is dropped, not delayed.

Suspending the driver's DirectSound, the hardware's two channels that play
recorded samples, uses the same word. `m4aSoundVSyncOff` adds
10 rather than 1, so a suspended driver can be told from a busy one, and
`m4aSoundVSyncOn` takes the 10 off again. It suspends only a
driver that is free or held once; `m4aSoundMode` is what holds it
once, around a change of the mixing rate.

| Name | Value | Meaning |
| --- | --- | --- |
| `ID_NUMBER` | `0x68736D53` | the driver is ready and free; its bytes spell `Smsh` |
| `ID_NUMBER` + 1 | | a library call holds the driver |
| `ID_NUMBER` + 10 | | `m4aSoundVSyncOff` has suspended the driver |
| 0 | | `SoundInit` is clearing the driver's work |

## The mixer's code is chosen per song

Part of the code that mixes the recorded sounds is swapped each time music
starts, so a song chooses how much of each frame's sound carries into the
next: in effect its echo, which MusicPlayer2000 calls reverb.

Some of the game's sounds are recordings, samples, which the driver mixes in
software, a frame's worth at a time, into the buffers the hardware plays
from. The last step of that mixing decides what the next frame's mix starts
from, and the game keeps five versions of that step. Starting a piece of
music copies one of them into place: the title and ending music use one,
most music another, a link battle a third.
Each keeps a different share of what has already played.

Each VBlank, `Mixer_Enter` jumps into the resident sound code at
`IwramSoundMixEntry`, which start-up copies to [IWRAM](../../glossary.md#iwram-and-ewram), the console's small,
fast memory inside the processor, with the rest of the resident runtime
([System](../system/README.md)).

`Sound_LoadPresetParameters` copies one of `SOUND_PRESET_COUNT` presets of
`SOUND_PRESET_SIZE` bytes of mixer code (`IwramCodeRecords`, in
`system/entry.s`) over `IwramSoundPresetCode`, in the middle of the code
that renders the mix. Music loads preset 3 for `MUS_ENDING`, `MUS_TITLE` and
`MUS_ENDING_LOGO` and preset 2 for the rest. A link battle loads preset 4 with its
music (`Battle_Run`), the Battle Arena preset 2, and the audio test
steps through all five with Select. What each preset does is under
[Evidence and open questions](#evidence-and-open-questions).

The driver's own mixer, `SoundMainRAM` (`mixer.s`), is separate code at
`Ram_SoundMixer`, which `m4aSoundInit` copies there; the presets never
touch it.

| Preset | Loaded by | What it carries into the next frame |
| --- | --- | --- |
| 0 | the audio test | one value for both channels, from the two buffers together |
| 1 | the audio test | each channel's old samples crossed with the other's |
| 2 | most music, the Battle Arena | a quarter of the mix and an eighth of each buffer's old samples |
| 3 | `MUS_ENDING`, `MUS_TITLE`, `MUS_ENDING_LOGO` | an eighth of the mix and a quarter of the old samples |
| 4 | a link battle | nothing: the mix buffer is cleared |

## Design

The driver is Nintendo's library, compiled the way pret compiles its copy, so
it keeps pret's names from `include/gba/m4a_internal.h`, `src/m4a.c` and
`src/m4a_1.s`: its structures (`SoundInfo`, `MusicPlayerInfo`,
`MusicPlayerTrack`, `SoundChannel`, `CgbChannel`, `ToneData`, `SongHeader`,
`MusicPlayer`, `Song`), its tables (`gMPlayTable`, `gSongTable`,
`gMPlayJumpTable`, `gXcmdTable`) and its routines (`m4aSoundInit`,
`MPlayStart`, the song commands `ply_goto` to `ply_xswee`), so a reader who
knows pret's games knows these. This copy is older than pret's: it has no
cries, its extended commands stop at `ply_xswee` (pret's add `ply_xwait` and
`ply_xcmd_0D`), and its jump-table indices are named here as `MPLAY_JUMP_`
constants after the commands `MPlayDef.s` names. What Camelot added keeps a
plain name in the project's style: the VBlank's way into the game's own IWRAM
mixer (`Mixer_Enter`, `Mixer_StartFrame`, `Mixer_EnterCallViaR3`), the
fanfare's music player and the sound-effect music players
(`gMPlayInfo_Fanfare`, `gMPlayInfo_MenuSE`, `gMPlayInfo_ActionSE`,
`gMPlayInfo_SE4` to `gMPlayInfo_SE7`), and the song layer in `sound.c`.

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

## Files

| File | What it holds |
| --- | --- |
| `sound.c` | the song layer: playing a song number, the music's ramps, the fanfare's return to the music, and the audio test |
| `engine.c` | the driver: the sound hardware, the music players and their tracks, the song commands and the CGB channels |
| `audio.c` | the driver's library calls: starting, stopping, fading and resuming songs on the music players, and the PCM key-to-frequency step |
| `tables.c` | the driver's tables: the song command handlers and the frequency tables |
| `empty_callback.c` | the driver's do-nothing callback |
| `mixer.s`, `channel_update_entry.s`, `clear_buffer.s`, `umul_high.s` | the driver's handwritten ARM and Thumb routines, each header naming the instruction that shows no compiler wrote it |

`engine.c`, `audio.c` and `tables.c` are compiled with pret's [`old_agbcc`](../../glossary.md#the-compilers)
(the Makefile's `SOUND_C`), as pret builds its MusicPlayer2000 C; the rest
with the game's compiler. The song layer's code is the sound bank; the
driver follows the banks in the ROM's tail, with `mixer.s` loaded into IWRAM.
The songs and samples are in `games/tbs/sound`, with the music players in
`games/tbs/sound/music_player_table.s`, and the songs' names in `songs.h` (`MUS_`,
`SE_`). The unit headers are in `games/tbs/include/unit/sound/`, the
driver's structures in `games/tbs/include/sound`.

## Words used here

- **song number**: the number `Sound_PlaySong` takes: music, a fanfare, a song of
  the song table, or a control.
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

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `Sound_`, `SoundTest_` | the song layer and the audio test (`sound.c`) |
| `m4a`, `MPlay`, `Sound`, `Cgb`, `ply_` | the driver, under pret's MusicPlayer2000 names: `m4aSongNumStart` and `m4aMPlay` calls, `MPlayStart` and `MPlayMain` on a music player, `SoundInit` and `SoundMain` on the driver's work, `CgbSound` and `CgbModVol` on the CGB channels, `ply_` for a song command |
| `Mixer_` | Camelot's way from the VBlank into the game's own IWRAM mixer, and the mixer's call helper |

## Evidence and open questions

The driver's names are pret's for MusicPlayer2000, each checked against the
same routine in pokeemerald's `src/m4a.c`, `src/m4a_1.s` and
`src/m4a_tables.c`. Where pret has no name the name is ours: `MPlayStartFade`
(the fade setup pret writes out in each fade call), `ld_r3_tp_adr_i_r2` (the
second entry of `ld_r3_tp_adr_i`), `Mixer_CallViaR3` (the `bx r3` at the end
of `SoundMainRAM`), `SoundMainRAM_Rom` (the mixer's ROM copy), the
`MPLAY_JUMP_` indices and the music players beyond `gMPlayInfo_BGM`. The song layer's come from what `Sound_PlaySong` does with each
range of numbers, and the songs' from `songs.h`, where a song without a
proven name keeps its number (`MUS_009`, `SE_188`).

The mixer presets are the last step of `IwramSoundRenderFrame`'s loop. Each
pass loads four words of the mix buffer, two 16-bit samples to a word, and
writes them out to the two hardware buffers, reading back first what those
buffers held from the previous time round (`fp` from the first, `ip` from the
second, `0x630` bytes on). The preset then works out what the mix buffer
starts the next frame with, from this frame's mix and those older samples,
and stores it. Preset 2, which most music uses, keeps a quarter of the mix
plus an eighth of each buffer's old samples, each channel from its own buffer.
Preset 3
swaps the weights, an eighth of the mix and a quarter of the old samples, so
more of what has already played carries into the next frame. Preset 0 makes
both channels one value from the two buffers together, preset 1 crosses each
channel's old samples with the other's, and preset 4 clears the mix buffer, so
nothing carries over. Preset 3 is the title's music and the ending's: `MUS_TITLE`
over the title intro, `MUS_ENDING_LOGO` over the Golden Sun logo shown after the ending, and
`MUS_ENDING` through the credits and as the Lemurian ship sails off
(`menu/title.c`, `map/lemurian_ship.c`). Preset 4 is the link battle's.

A library call is dropped only when it meets another that holds the lock, and
library calls all run on the main thread, one after another, except from a
VBlank [task](../../glossary.md#task). A call graph of the English edition's build finds one VBlank task that
reaches the library: `LuckyWheels_RunFrame`, the Lucky Wheels frame, which
plays its sounds from the VBlank. Its sounds can be dropped only if the VBlank
arrives while a main-loop task is inside a library call; while Lucky
Wheels runs, the main thread only draws and waits, and no message types, so
in practice nothing is dropped.

A sound effect that names `MUSIC_PLAYER_SE_LAST` searches only music players
4 to 7 because music players 2 and 3 have sounds of their own in the song
table: `MUSIC_PLAYER_MENU_SE` the interface's (`SE_MENU_CONFIRM`,
`SE_MENU_CANCEL`, `SE_MENU_ERROR`, `SE_SHOP_PURCHASE`) and
`MUSIC_PLAYER_ACTION_SE` the field's and battle's
actions (`SE_CAST`, `SE_IMPACT`, `SE_HEAVY_IMPACT`, `SE_DOOR_OPEN`). A
floating effect therefore never cuts those off.

- XXX: what the bytes no C names in the driver's structures hold (`SoundChannel` and
  `CgbChannel` `unk0e`, `unk15` and `unk38`, `MusicPlayerTrack.unk30`,
  `MusicPlayerInfo.unk10` and `unk2a`, `SoundInfo.unk09`, `unk0d` and `unk40`); a
  pass over the mixer and channel assembly (`mixer.s`, `clear_buffer.s`,
  `channel_update_entry.s`) for loads and stores at those offsets.
