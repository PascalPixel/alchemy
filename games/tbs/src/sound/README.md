# Sound

This folder holds The Broken Seal's sound: the game's cue layer over the
sound driver, and the driver itself, Nintendo's MusicPlayer2000 as this game
links it.

See also: [Runtime](../runtime/README.md), whose VBlank handler steps the
music's ramps and enters the mixer.

## Files

| File | What it holds |
| --- | --- |
| `sound.c` | the cue layer: playing a cue (a song, a sound effect, a fanfare or a fade), the music's volume, tempo and pitch ramps, the fanfare's return to the music, and the audio test |
| `engine.c` | the driver: the sound hardware, the players and their tracks, the song commands and the CGB channels |
| `audio.c` | the driver's library calls: starting, stopping, fading and resuming songs on the players, and the PCM key-to-frequency step |
| `tables.c` | the driver's tables: the song command handlers and the frequency tables |
| `empty_callback.c` | the driver's do-nothing callback |
| `mixer.s`, `channel_update_entry.s`, `clear_buffer.s`, `umul_high.s` | the driver's handwritten ARM and Thumb routines, each proven in its header |

`engine.c`, `audio.c` and `tables.c` are compiled with pret's `old_agbcc`, the
sound route of the Makefile (`SOUND_C`), as pret builds its MusicPlayer2000 C;
the rest with the game's compiler. The songs and samples are in
`games/tbs/sound`, and the songs' names in `songs.h` (`MUS_`, `SE_`).
Link order is in `games/tbs/objects.tsv`. The unit headers are in
`games/tbs/include/unit/sound/`, the driver's structures in
`games/tbs/include/sound`.

## Words

- **cue**: the number the game plays (`AudioCommand_Play`): a song, a sound
  effect, a fanfare that pauses the music and returns to it, or a fade
  control.
- **player** and **track**: a player (`struct SoundPlayer`) plays one song,
  whose tracks (`struct SoundTrack`) each run a stream of song commands.
- **note**: a sound a track starts, on a PCM channel the mixer renders or on
  one of the four CGB channels of the hardware.
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **lock**: `work->lock`, `SOUND_LOCK` while the driver may run (see the
  design below).

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `AudioCommand_`, `Audio_`, `AudioTest_` | the cue layer and the audio test |
| `AudioEngine_` | the sound hardware and the driver's work |
| `MusicPlayer_`, `MusicTrack_`, `MusicCommand_` | players, tracks and song commands |
| `Cgb_`, `CgbChannel_`, `CgbNote_` | the CGB channels |
| `Sound_`, `Runtime_` | things the engine names elsewhere |

The spellings some sites keep to match, and the comment markers, are in
[Matching](../MATCHING.md).

## Design

### What a cue number means

The game plays every sound through one call, `AudioCommand_Play(id)`, and
the number decides what kind of sound it is. Its low 12 bits are the cue and
its top four bits flags (`AUDIO_CUE_START_SILENT` starts music at volume 0).
A cue from 100 up (`AUDIO_SONG_FIRST`) is a song of the song table played on
the player its row names; a row that names player 7 takes the first free
player counting down from 7 to 4, and player 7 again if none is free, so up
to four such effects sound at once and a fifth cuts off player 7's. A cue
from 80 to 99 (`AUDIO_JINGLE_FIRST`) is a fanfare. Below 80 is music. Three
cues are controls instead: 17 fades the music out, 18 keeps the music that
is playing, and 0x121 fades player 3. Asking for the music that is
already playing does nothing, so a scene can name its music on every entry
without restarting it.

### The fanfare's return to the music

A fanfare does not stop the music. `AudioCommand_Play` sets the music's
volume to 0 at once and plays the fanfare, and sets a delay of ten frames.
Once a VBlank, `MusicPlayer_StepVolumeAndPitchTowardTargets` (called from
`System_VBlankHandler`) counts the delay down; when it reaches 1 it waits for
the fanfare's player to stop, and then sets the music's target volume back to
full. The same routine steps the volume, and the combined tempo and pitch,
towards their targets by a fixed step each VBlank, and stops on the target
when a step would cross it. So the music resumes where it was, fading back in,
and nothing in the waiting code has to know a fanfare played.

### The lock

The driver is called from two places that can interrupt each other: the game
calls the library (`audio.c`, `engine.c`) from its frame loop, and the VBlank
handler enters the mixer last of all (`SoundDriver_EnterMixer`). They share
one word, `work->lock`, and each player has one of its own.

The word is both a signature and a lock. `SOUND_LOCK` is 0x68736d53, the
bytes `Smsh`; `AudioEngine_Initialize` sets it to 0 while it clears the
driver's work and to `SOUND_LOCK` when the driver is ready. Every library
call that changes the driver's state checks that the word is exactly
`SOUND_LOCK`, adds 1 for the length of its work, and puts `SOUND_LOCK` back.
The mixer's entry runs only when the word is exactly `SOUND_LOCK`, and its
exit stores the signature again. So:

- a VBlank that arrives while a library call is changing the state skips
  that frame's driver work instead of reading half-changed state;
- a library call made while the driver is not ready, or while another call
  holds the lock, returns without doing anything: it is dropped, not
  delayed;
- `AudioEngine_SuspendDirectSound` adds 10, not 1, so a suspended driver is
  told apart from a busy one: it refuses to suspend twice (more than 1 above
  the signature), and `AudioEngine_ResumeDirectSound` takes the 10 off again.

XXX: whether a dropped call ever matters in the game is not known.

### The mixer's code is chosen per song

The PCM mixer is copied into IWRAM (`Ram_SoundMixer`, 0x03007000) so that it
runs from fast memory, and part of it is replaced per song:
`Sound_LoadPresetParameters` copies one of five 152-byte records of mixer
code over it. Music uses record 3 for three songs (`MUS_067`, `MUS_070` and
`MUS_075`) and record 2 for the rest. XXX: what the records change in the
mix, and why those three songs, is not known.

## Evidence and open questions

The driver's names follow pret's for MusicPlayer2000 where its structure is
the same; the cue layer's come from what `AudioCommand_Play` does with each
range of numbers, and the songs' from `songs.h`, where a song without a
proven name keeps its number (`MUS_070`, `SE_188`).

Still unknown:

- XXX: what the five mixer records differ in, and why three songs take
  record 3.
- XXX: whether a library call dropped by the lock is ever noticeable.
- XXX: why the sound effects that name player 7 search only players 4 to 7.
