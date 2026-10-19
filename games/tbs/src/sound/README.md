# Sound

This folder holds The Broken Seal's sound: the game's cue layer over the
sound driver, and the driver itself, Nintendo's MusicPlayer2000 as this game
links it.

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
`games/common/include/sound`.

## Words used here

- **cue**: the number the game plays (`AudioCommand_Play`): a song, a sound
  effect, a fanfare that pauses the music and returns to it, or a fade
  control.
- **player** and **track**: a player (`struct SoundPlayer`) plays one song,
  whose tracks (`struct SoundTrack`) each run a stream of song commands.
- **note**: a sound a track starts, on a PCM channel the mixer renders or on
  one of the four CGB channels of the hardware.
- **ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **lock**: `work->lock`, `SOUND_LOCK` while the driver may run; the driver
  adds to it while it changes its state.

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `AudioCommand_`, `Audio_`, `AudioTest_` | the cue layer and the audio test |
| `AudioEngine_` | the sound hardware and the driver's work |
| `MusicPlayer_`, `MusicTrack_`, `MusicCommand_` | players, tracks and song commands |
| `Cgb_`, `CgbChannel_`, `CgbNote_` | the CGB channels |
| `Sound_`, `Runtime_` | things the engine names elsewhere |

## Spellings

- The driver's command table is called through a pointer read from the
  table's entry (`*(&gSound_CommandTable[n])`); indexing it directly adds an
  instruction with `old_agbcc`.
- `engine.c` keeps the labelled jumps its matching forms need.
