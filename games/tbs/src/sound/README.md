# Sound

![The sound driver's eight players, each able to play one song at a time. Player 0 plays fanfares, such as the one for finding treasure, with four tracks. Player 1 plays the music, with ten tracks. Player 2 plays the menus' sounds: confirm, cancel, error and buying. Player 3 plays the sounds of actions in the field and in battle: casting, impacts, doors opening. Players 4 to 7 share the other sound effects: most take the first free player counting down from 7, and when none is free they cut off whatever player 7 was playing. Because each kind has its own players, a sound effect never stops the music.](https://alchemy.pascalpixel.com/illustrations/sound/players.svg)

A town's music plays while the cursor clicks through a menu, a door creaks
open and, when a chest is opened, a short fanfare plays and the music comes
back by itself once it has finished. All of that is the sound folder's work.
The game asks for every sound the same way, with one call and a number, the
cue; the cue layer (`sound.c`) decides what each number means and keeps the
music, the fanfares and the sound effects out of each other's way. Under it is
the driver that plays songs on the hardware: Nintendo's MusicPlayer2000, the
sound library many Game Boy Advance games ship, which [pret](../../glossary.md#pret)'s decompilations
call m4a. In it every sound, a sound effect included, is a song: a header and
tracks of commands, played on one of the driver's players.

The driver has eight players, each of which plays one song at a time, and
each kind of sound has players of its own, so a sound effect never stops the
music. The cue layer and the driver meet once a [frame](../../glossary.md#frame) in the VBlank handler,
which steps the music's volume towards its target and then runs the mixer
([Runtime](../runtime/README.md#the-frame-loop)).

## What a cue number means

A cue's number says whether it is music, a fanfare, a sound from the song
table or a control.

![Every sound starts as one call with a cue number, and the number's range decides what it is. Cues below 80 are music, played on the music player; asking for the music already chosen does nothing. Cues from 80 to 99 are fanfares, played on the fanfare player while the music is silenced. Cues from 100 up are rows of the song table, mostly sound effects, each played on the player its row names. Three cues are controls instead of sounds: cue 17 fades the music out, cue 18 keeps the music that is playing, and cue 289 fades out player 3.](https://alchemy.pascalpixel.com/illustrations/sound/cue-ranges.svg)

Code anywhere in the game plays a sound by calling `Sound_PlaySong(s32
id)` with a cue, and it returns nothing. Every caller outside the sound [bank](../../glossary.md#bank) uses its
[far-call](../../glossary.md#far-call) name, `Sound_PlaySongFar`, as the field does when a chest
holds a Psynergy Stone (`Field_SearchCell`):

```c
Sound_PlaySongFar(MUS_TREASURE_FOUND);
```

The cue's range picks what happens: below 80 it is music, from 80 to 99 a
fanfare, and from 100 up a row of the song table, which is how sound effects
play. A few numbers are controls rather than sounds, such as the one that
fades the music out.

Only a cue's low 16 bits count: the low 12 (`SONG_NUM_MASK`) are the cue,
0 to 4,095, and the four above them are flags, of which only
`SONG_FLAG_FADE_IN` is read; it starts music at volume 0 and fades it
in over 64 frames.

The driver has eight players (`gSound_PlayerSlots`):
`SOUND_PLAYER_FANFARE` (0) with four tracks, `SOUND_PLAYER_MUSIC` (1) with
ten, and players 2 to 7 with two each. What a cue plays depends on its
range:

- From `SONG_SE_FIRST` (100) up, a cue is a row of the song table
  (`gSound_SongTable`), played on the player the row names; sound effects
  such as `SE_SHOP_PURCHASE` and `SE_MENU_CURSOR_MOVE` are rows. A row that
  names player 7, `SOUND_PLAYER_EFFECT_LAST`, means any free player from 7
  down to 4 (`SOUND_PLAYER_EFFECT_FIRST`): a floating effect. It takes the
  first idle one counting down, and player 7 again if none is idle, so four
  floating effects play at once and a fifth cuts off the one on player 7.
- From `SONG_FANFARE_FIRST` (80) to 99, a cue is a fanfare, such as
  `MUS_TREASURE_FOUND` or `MUS_PUZZLE_SOLVED`, played on the fanfare player.
- Below 80, a cue is music, played on the music player.

Three cues are controls. `SONG_FADE_OUT_BGM` fades the music out over 112
frames (16 steps 7 frames apart); a second fade cue does nothing until new
music has started. The fade records `MUS_SILENCE`, a song with no tracks, as
the music chosen, so a later cue for the faded music starts it again. `SONG_KEEP_BGM` keeps
the music that is playing. `SONG_FADE_OUT_ACTION_SE` fades player 3 out over
48 frames. The three values are also rows of the song table (17, 18 and 289),
and those rows never play, since the controls are tested first.

A cue for the music already chosen, which `gCurrentBgm` records, does nothing.
That lets a [map](../../glossary.md#map) name its music every time the [party](../../glossary.md#the-party) enters without
restarting it, but it also holds when that song has ended: a song that
doesn't loop plays again only after another music cue or a fade.

`Sound_PlaySong` reads the song table, `gSound_SongTable` (`games/tbs/sound/songs.s`), whose row holds the
song's header and its player, and the players, `gSound_PlayerSlots`
(`games/tbs/sound/players.s`). The cues are named in
`audio_engine_symbols.h` and the songs in `songs.h`.

| Name | Value | Meaning |
| --- | --- | --- |
| `SONG_NUM_MASK` | `0xfff` | the bits that are the cue |
| `SONG_FLAG_FADE_IN` | `0x1000` | flag: music starts at volume 0 and fades in over 64 frames |
| `SONG_FADE_OUT_BGM` | 17 | fade the music out over 112 frames, once |
| `SONG_KEEP_BGM` | 18 | keep the music that is playing |
| `SONG_FANFARE_FIRST` | 80 | first fanfare cue; below it, music |
| `SONG_SE_FIRST` | 100 | first cue that is a song table row |
| `SONG_FADE_OUT_ACTION_SE` | `0x121` (289) | fade player 3 out over 48 frames |
| `SOUND_PLAYER_FANFARE` | 0 | the fanfare player, 4 tracks |
| `SOUND_PLAYER_MUSIC` | 1 | the music player, 10 tracks |
| `SOUND_PLAYER_EFFECT_FIRST` | 4 | the lowest player a floating effect may take |
| `SOUND_PLAYER_EFFECT_LAST` | 7 | named by a row, a floating effect: the first idle player from 7 down to 4 |

## The fanfare's return to the music

A fanfare silences the music without stopping it, and the music comes back by
itself once the fanfare has ended.

![The music's volume over time while a chest is opened. The music plays at full volume. When the treasure fanfare starts on its own player, the music's volume drops to nothing at once, but the music keeps playing silently underneath. When the fanfare ends, the music's volume climbs back to full over 64 frames, about a second, from the point the song has reached. The code that asked for the fanfare does nothing to bring the music back.](https://alchemy.pascalpixel.com/illustrations/sound/fanfare-return.svg)

When the party finds an item, the treasure fanfare plays and the town's music
falls silent; a moment after the fanfare ends, the music fades back in from
where it had got to. The music never stopped: only its volume went to zero.
Once a frame the cue layer checks whether the fanfare has finished and, when
it has, turns the music back up. The code that played the fanfare does
nothing more.

`Sound_PlaySong` sets the music's volume to 0, plays the fanfare on the
fanfare player and sets `gBgmRestoreDelay` to `BGM_RESTORE_DELAY_FRAMES`;
the music keeps playing, silently.

Once a VBlank, `Sound_UpdateBgmRamps` (called from
`System_VBlankHandler`) counts the delay down to 1, then waits until the
fanfare player has no track playing and sets the music's target volume back
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
track), though the music player has 10, so tracks 8 and 9 keep their volume
and pitch through the ramps and under a fanfare. Nine songs use them: music
6, 22, 28, 29, 40 to 43 (`MUS_040` to `MUS_VENUS_LIGHTHOUSE`) and 74. The
original game does the same, since this is its code.

The ramps are globals of the cue layer: `gBgmVolume`, `gBgmVolumeTarget`
and `gBgmVolumeStep`, and `gBgmTempoPitch`, `gBgmTempoPitchTarget` and
`gBgmTempoPitchStep` for tempo and pitch together.

| Name | Value | Meaning |
| --- | --- | --- |
| `BGM_RESTORE_DELAY_FRAMES` | 10 | VBlanks after a fanfare starts before the music waits for it to end |
| `BGM_UNITY_SCALE` | `0x100` | full volume, and the song's own tempo and pitch: 1.0 in 8.8 fixed point |
| `BGM_DEFAULT_RAMP_STEP` | 4 | a ramp's step a VBlank: a full fade takes 64 frames |
| `TRACKS_ALL` | `0xff` | the track mask of every music command: tracks 0 to 7 |
| `BGM_WAIT_TIMEOUT_FRAMES` | 300 | frames `Sound_WaitForSong` waits before giving up, about five seconds |

## The driver's lock

One word in the driver's memory keeps the game's calls and the mixer from
changing the driver's state at the same time.

![Two kinds of code change the sound driver's state: the game's calls, such as starting a song, which run during the frame, and the mixer, which the VBlank interrupt runs once a frame and which can arrive in the middle of a call. They share one lock word. A call takes the lock while it works and gives it back when done. If the VBlank arrives while the lock is taken, the mixer skips that frame's driver work rather than read half-changed state, and at the next VBlank, with the lock free, it works as usual. A call that finds the lock taken does nothing at all: it is dropped, not delayed.](https://alchemy.pascalpixel.com/illustrations/sound/lock.svg)

The game starts and stops songs during the frame, and the mixer runs in the
blank, which can arrive in the middle of one of those calls. If the mixer read
the driver while a call was halfway through changing it, it could play
nonsense. So the two share a lock: a call takes it while it works, and the
mixer, finding it taken, leaves that frame's driver work for the next one.

The game calls the driver's library (`audio.c`, `engine.c`) from its frame
loop, and the VBlank handler enters the mixer last of all
(`SoundDriver_EnterMixer`). They share one word, `work->lock`, the first field
of the driver's work, and each player has a lock word of its own.

The word is both a signature and a lock. `SOUND_LOCK`'s four bytes spell
`Smsh` in memory order. `AudioEngine_Initialize` sets the word to 0 while it
clears the driver's work and to `SOUND_LOCK` once the driver is ready. Every
library call that changes the driver's state checks that the word is exactly
`SOUND_LOCK`, adds 1 while it works, and puts `SOUND_LOCK` back. The mixer's
entry runs only when the word is exactly `SOUND_LOCK`, and its exit stores
the signature again.

A VBlank that arrives during a library call therefore skips that frame's
driver work instead of reading half-changed state, and a library call made
while the driver isn't ready, or while another call holds the lock, returns
without doing anything: it is dropped, not delayed.

Suspending the driver's DirectSound, the hardware's two channels that play
recorded samples, uses the same word. `AudioEngine_SuspendDirectSound` adds
10 rather than 1, so a suspended driver can be told from a busy one, and
`AudioEngine_ResumeDirectSound` takes the 10 off again. It suspends only a
driver that is free or held once; `AudioEngine_SetMode` is what holds it
once, around a change of the mixing rate.

| Name | Value | Meaning |
| --- | --- | --- |
| `SOUND_LOCK` | `0x68736D53` | the driver is ready and free; its bytes spell `Smsh` |
| `SOUND_LOCK` + 1 | | a library call holds the driver |
| `SOUND_LOCK` + 10 | | `AudioEngine_SuspendDirectSound` has suspended the driver |
| 0 | | `AudioEngine_Initialize` is clearing the driver's work |

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

Each VBlank, `SoundDriver_EnterMixer` jumps into the resident sound code at
`IwramSoundMixEntry`, which start-up copies to [IWRAM](../../glossary.md#iwram-and-ewram), the console's small,
fast memory inside the processor, with the rest of the resident runtime
([System](../system/README.md)).

`Sound_LoadPresetParameters` copies one of `SOUND_PRESET_COUNT` records of
`SOUND_PRESET_SIZE` bytes of mixer code (`IwramCodeRecords`, in
`system/entry.s`) over `IwramSoundPresetCode`, in the middle of the code
that renders the mix. Music loads record 3 for `MUS_ENDING`, `MUS_TITLE` and
`MUS_ENDING_LOGO` and record 2 for the rest. A link battle loads record 4 with its
music (`Battle_Run`), the link lobby record 2, and the audio test
steps through all five with Select. What each record does is under
[Evidence and open questions](#evidence-and-open-questions).

The driver's own mixer, `SoundDriver_Mixer` (`mixer.s`), is separate code at
`Ram_SoundMixer`, which `Audio_Initialize` copies there; the records never
touch it.

| Record | Loaded by | What it carries into the next frame |
| --- | --- | --- |
| 0 | the audio test | one value for both channels, from the two buffers together |
| 1 | the audio test | each channel's old samples crossed with the other's |
| 2 | most music, the link lobby | a quarter of the mix and an eighth of each buffer's old samples |
| 3 | `MUS_ENDING`, `MUS_TITLE`, `MUS_ENDING_LOGO` | an eighth of the mix and a quarter of the old samples |
| 4 | a link battle | nothing: the mix buffer is cleared |

## How it fits with the rest of the game

Every part of the game plays sound through `Sound_PlaySongFar`: maps name
their music on entry, menus click, battle effects play their impacts. The
VBlank handler in [Runtime](../runtime/README.md#the-frame-loop) steps the
music's ramps and enters the mixer every frame, and the mixer's resident code
is part of what start-up copies into IWRAM
([System](../system/README.md#memory-map)). The songs and samples themselves
are data in `games/tbs/sound`.

## Limits

- Eight players; the music player has 10 tracks, of which the cue layer's
  ramps and fanfare silence reach 8.
- Four sound effects that name `SOUND_PLAYER_EFFECT_LAST` play at once.
- Cues are 12 bits, 0 to 4,095.

## Files

| File | What it holds |
| --- | --- |
| `sound.c` | the cue layer: playing a cue, the music's ramps, the fanfare's return to the music, and the audio test |
| `engine.c` | the driver: the sound hardware, the players and their tracks, the song commands and the CGB channels |
| `audio.c` | the driver's library calls: starting, stopping, fading and resuming songs on the players, and the PCM key-to-frequency step |
| `tables.c` | the driver's tables: the song command handlers and the frequency tables |
| `empty_callback.c` | the driver's do-nothing callback |
| `mixer.s`, `channel_update_entry.s`, `clear_buffer.s`, `umul_high.s` | the driver's handwritten ARM and Thumb routines, each header naming the instruction that shows no compiler wrote it |

`engine.c`, `audio.c` and `tables.c` are compiled with pret's [`old_agbcc`](../../glossary.md#the-compilers)
(the Makefile's `SOUND_C`), as pret builds its MusicPlayer2000 C; the rest
with the game's compiler. The cue layer's code is the sound bank; the
driver follows the banks in the ROM's tail, with `mixer.s` loaded into IWRAM.
The songs and samples are in `games/tbs/sound`, with the players in
`games/tbs/sound/players.s`, and the songs' names in `songs.h` (`MUS_`,
`SE_`). The unit headers are in `games/tbs/include/unit/sound/`, the
driver's structures in `games/tbs/include/sound`.

## Words used here

- **cue**: the number `Sound_PlaySong` takes: music, a fanfare, a song of
  the song table, or a control.
- **player** and **track**: a player (`struct SoundPlayer`) plays one song,
  whose tracks (`struct SoundTrack`) each run a stream of song commands.
- **note**: a sound a track starts, on a PCM channel the mixer renders into
  DirectSound or on one of the four CGB channels, the tone generators built
  into the hardware (channel 4 is noise).
- **floating effect**: a song table row that names player 7, played on the
  first idle player from 7 down to 4.
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **lock**: `work->lock`, the driver's signature and lock word.

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `AudioCommand_`, `Audio_`, `AudioTest_` | the cue layer and the audio test |
| `AudioEngine_` | the sound hardware and the driver's work |
| `MusicPlayer_`, `MusicTrack_`, `MusicCommand_` | players, tracks and song commands |
| `SoundNote_`, `Pcm_` | a note, and its PCM pitch |
| `Cgb_`, `CgbAudio_`, `CgbChannel_`, `CgbNote_` | the CGB channels |
| `SoundDriver_`, `MusicDriver_`, `Mixer_` | the handwritten routines: entering the mix, the mixer and their call helpers |
| `Sound_`, `Runtime_` | functions the engine names elsewhere |

## Evidence and open questions

The driver's names follow pret's for MusicPlayer2000 where its structure is
the same. The cue layer's come from what `Sound_PlaySong` does with each
range of numbers, and the songs' from `songs.h`, where a song without a
proven name keeps its number (`MUS_009`, `SE_188`).

The mixer records are the last step of `IwramSoundRenderFrame`'s loop. Each
pass loads four words of the mix buffer, two 16-bit samples to a word, and
writes them out to the two hardware buffers, reading back first what those
buffers held from the previous time round (`fp` from the first, `ip` from the
second, `0x630` bytes on). The record then works out what the mix buffer
starts the next frame with, from this frame's mix and those older samples,
and stores it. Record 2, which most music uses, keeps a quarter of the mix
plus an eighth of each buffer's old samples, each channel from its own buffer
Record 3
swaps the weights, an eighth of the mix and a quarter of the old samples, so
more of what has already played carries into the next frame. Record 0 makes
both channels one value from the two buffers together, record 1 crosses each
channel's old samples with the other's, and record 4 clears the mix buffer, so
nothing carries over. Record 3 is the title's music and the ending's: `MUS_TITLE`
over the title intro, `MUS_ENDING_LOGO` over the Golden Sun logo shown after the ending, and
`MUS_ENDING` through the credits and as the Lemurian ship sails off
(`menu/title.c`, `map/lemurian_ship.c`). Record 4 is the link battle's.

A library call is dropped only when it meets another that holds the lock, and
library calls all run on the main thread, one after another, except from a
VBlank [task](../../glossary.md#task). A call graph of the English edition's build finds one VBlank task that
reaches the library: `LuckyWheels_RunFrame`, the slot machine's frame, which
plays its sounds from the VBlank. Its sounds can be dropped only if the VBlank
arrives while a main-loop task is inside a library call; while the slot
machine runs, the main thread only draws and waits, and no message types, so
in practice nothing is dropped.

A sound effect that names `SOUND_PLAYER_EFFECT_LAST` searches only players 4
to 7 because players 2 and 3 have sounds of their own in the song table:
player 2 the interface's (`SE_MENU_CONFIRM`, `SE_MENU_CANCEL`,
`SE_MENU_ERROR`, `SE_SHOP_PURCHASE`) and player 3 the field's and battle's
actions (`SE_CAST`, `SE_IMPACT`, `SE_HEAVY_IMPACT`, `SE_DOOR_OPEN`). A
floating effect therefore never cuts those off.

- XXX: what the bytes no C names in the driver's records hold (`SoundNote` and
  `CgbNote` `unk0e`, `unk15` and `unk38`, `SoundTrack.unk30`,
  `SoundPlayer.unk10` and `unk2a`, `SoundWork.unk09`, `unk0d` and `unk40`); a
  pass over the mixer and channel assembly (`mixer.s`, `clear_buffer.s`,
  `channel_update_entry.s`) for loads and stores at those offsets.
