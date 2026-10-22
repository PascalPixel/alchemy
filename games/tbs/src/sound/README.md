# Sound

Every sound in Golden Sun, from the battle music to the menu cursor moving, is
played through one call, `AudioCommand_Play`, with a number. That number is a
cue. The game's own cue layer (`sound.c`) decides what each number means,
keeps the music's fades moving, and brings the music back after a fanfare.
Under it sits the sound driver that turns songs into sound on the hardware:
Nintendo's MusicPlayer2000, as this game links it.

The two meet once a frame in the VBlank handler, which runs in the short pause
after each frame is drawn (see [Runtime](../runtime/README.md)): it steps the
music's ramps and then enters the mixer.

## Design

### What a cue number means

The number alone decides what kind of sound `AudioCommand_Play(id)` makes.
Only its low 16 bits count. The low 12 of those hold the cue itself, so cues
run from 0 to 4,095, and the four above them are flags. The only flag read is
`AUDIO_CUE_START_SILENT`, which starts music at volume 0 and fades it
in over 64 frames.

The sound driver plays songs on players (`struct SoundPlayer`), eight of them,
each playing one song at a time (`games/tbs/sound/players.s`). Player 0 plays
fanfares, player 1 the music with up to 10 tracks, and players 2 to 7 have two
tracks each. What a cue plays depends on where its number falls:

- **From 100 up** (`AUDIO_SONG_FIRST`), a cue is a song of the song table,
  played on the player its row names; the shop's purchase sound
  (`SE_SHOP_PURCHASE`, 101) and the menu cursor (`SE_MENU_CURSOR_MOVE`, 111)
  are two of them. A row that names player 7 takes the first free player
  counting down from 7 to 4, and player 7 again if none is free, so up to four
  such sounds play at once and a fifth cuts off player 7's.
- **From 80 to 99** (`AUDIO_JINGLE_FIRST`), a cue is a fanfare, one of the
  short jingles such as the treasure chest's (`MUS_TREASURE_FOUND`, 83) or a
  solved puzzle's (`MUS_PUZZLE_SOLVED`, 80).
- **Below 80**, a cue is music.

Three cues are controls instead. Cue 17 fades the music out over 112 frames
(16 steps, 7 frames apart), only once until the next music starts, and records
`MUS_SILENCE`, a song with no tracks, as what's playing, so a later cue for the
faded music starts it again. Cue 18 keeps the music that's playing. And cue
289 fades player 3 out over 48 frames; it's also `SE_289`'s number,
so `SE_289` never plays.

Asking for the music that's already playing does nothing, even when that song
has ended, so a scene can name its music every time it's entered without
restarting it.

### The fanfare's return to the music

Open a chest and the music drops away for the treasure jingle, then fades back
in where it left off. A fanfare doesn't stop the music. `AudioCommand_Play`
sets the music's volume to 0 at once, plays the fanfare on player 0, and sets
a delay of ten frames (`MUSIC_RESTORE_DELAY_FRAMES`); the music keeps playing,
silently.

Once a VBlank, `MusicPlayer_StepVolumeAndPitchTowardTargets` (called from
`System_VBlankHandler`) counts the delay down. When it reaches 1, it waits for
the fanfare's player to stop, and then sets the music's target volume back to
full. The same routine steps the volume, and the combined tempo and pitch,
towards their targets by a fixed step each VBlank, and stops on the target
when a step would cross it. Full volume is 256, which is 1.0 in the ramps' fixed point (the low eight
bits are the fraction), and the
usual step is 4, so a full fade in or out takes 64 frames. That way the music
resumes where it was, fading back in, and nothing in the waiting code has to
know a fanfare played.

The music commands here touch only tracks 0 to 7 (`MUSIC_ALL_TRACKS` sets eight bits,
one bit a track), though the music player has 10. A few songs use 9 or more
(`MUS_006`, `MUS_022`, `MUS_028`, `MUS_029`, `MUS_040` to `MUS_043` and
`MUS_074`), and their tracks 8 and 9 keep their volume and pitch through the
ramps and under a fanfare.

### The driver's lock

The driver is called from two places that can interrupt each other: the game
calls the library (`audio.c`, `engine.c`) from its frame loop, and the VBlank
handler enters the mixer last of all (`SoundDriver_EnterMixer`). They share
one word (four bytes), `work->lock`, the first field of the driver's work, and
each player has a lock word of its own.

The word is both a signature and a lock. `SOUND_LOCK` is a number whose four
bytes spell `Smsh` in memory order. `AudioEngine_Initialize`
sets the word to 0 while it clears the driver's work, and to `SOUND_LOCK` once
the driver is ready. Every library call that changes the driver's state checks
that the word is exactly `SOUND_LOCK`, adds 1 for the length of its work, and
puts `SOUND_LOCK` back. The mixer's entry runs only when the word is exactly
`SOUND_LOCK`, and its exit stores the signature again.

So a VBlank that arrives while a library call is changing the state skips that
frame's driver work instead of reading half-changed state. A library call made
while the driver isn't ready, or while another call holds the lock, returns
without doing anything: it's dropped, not delayed. And
`AudioEngine_SuspendDirectSound` adds 10, not 1, so a suspended driver can be
told apart from a busy one. It suspends only a driver that's free, or held
once, as `AudioEngine_SetMode` holds it around a rate change. So it refuses to
suspend twice (more than 1 above the signature) and leaves a driver that isn't
ready alone, and `AudioEngine_ResumeDirectSound` takes the 10 off again.

XXX: whether a dropped call ever matters in the game isn't known.

### The mixer's code is chosen per song

The PCM channels play recorded samples, and the driver mixes them in software
into the buffers the hardware plays from. That mixing runs from IWRAM, the
fast memory: each VBlank, `SoundDriver_EnterMixer` jumps into the resident
sound code at `IwramSoundMixEntry`, which is copied to IWRAM at
power-up with the rest of the resident runtime
([System](../system/README.md)).

Part of that code changes with the music. `Sound_LoadPresetParameters` copies
one of five 152-byte records of mixer code (`IwramCodeRecords`, kept in
`system/entry.s`) over `IwramSoundPresetCode`, in the middle of
the code that renders the mix. Music uses record 3 for three songs (`MUS_067`,
`MUS_070` and `MUS_075`) and record 2 for the rest. A link battle loads record
4 with its music (`Battle_RunEncounter`), the link lobby record 2, and the
audio test steps through all five with Select.

The driver's own mixer, `SoundDriver_Mixer` (`mixer.s`), is separate code at
`Ram_SoundMixer`, which `Audio_Initialize` copies there, and the
records never touch it.

XXX: what the records change in the mix, and why those three songs, isn't
known.

## What's in the folder

| File | What it holds |
| --- | --- |
| `sound.c` | the cue layer: playing a cue (a song, a sound effect, a fanfare or a fade), the music's volume, tempo and pitch ramps, the fanfare's return to the music, and the audio test |
| `engine.c` | the driver: the sound hardware, the players and their tracks, the song commands and the CGB channels |
| `audio.c` | the driver's library calls: starting, stopping, fading and resuming songs on the players, and the PCM key-to-frequency step |
| `tables.c` | the driver's tables: the song command handlers and the frequency tables |
| `empty_callback.c` | the driver's do-nothing callback |
| `mixer.s`, `channel_update_entry.s`, `clear_buffer.s`, `umul_high.s` | the driver's handwritten ARM and Thumb routines, each proven in its header |

`engine.c`, `audio.c` and `tables.c` are compiled with pret's `old_agbcc`, the
sound route of the Makefile (`SOUND_C`), as pret builds its MusicPlayer2000
C; the rest with the game's compiler. The songs and samples are in
`games/tbs/sound`, and the songs' names in `songs.h` (`MUS_`, `SE_`). Link
order is in `games/tbs/objects.tsv`. The unit headers are in
`games/tbs/include/unit/sound/`, the driver's structures in
`games/tbs/include/sound`. The spellings some sites keep in order to match,
and the comment markers, are explained in [Matching](../MATCHING.md).

## Words used here

- **Cue**: the number the game plays (`AudioCommand_Play`): a song, a sound
  effect, a fanfare that pauses the music and returns to it, or a fade
  control.
- **Player** and **track**: a player (`struct SoundPlayer`) plays one song,
  whose tracks (`struct SoundTrack`) each run a stream of song commands.
- **Note**: a sound a track starts, on a PCM channel the mixer renders or on
  one of the four CGB channels, the sound generators built into the hardware
  (channel 4 is noise).
- **Ramp**: the music's volume, or its combined tempo and pitch, stepped
  towards a target once a VBlank.
- **Lock**: `work->lock`, `SOUND_LOCK` while the driver may run (see the
  driver's lock above).

## Function prefixes

| Prefix | What it acts on |
| --- | --- |
| `AudioCommand_`, `Audio_`, `AudioTest_` | the cue layer and the audio test |
| `AudioEngine_` | the sound hardware and the driver's work |
| `MusicPlayer_`, `MusicTrack_`, `MusicCommand_` | players, tracks and song commands |
| `SoundNote_`, `Pcm_` | a note, and its PCM pitch |
| `Cgb_`, `CgbAudio_`, `CgbChannel_`, `CgbNote_` | the CGB channels |
| `SoundDriver_`, `MusicDriver_`, `Mixer_` | the handwritten routines: entering the mix, the mixer and their call helpers |
| `Sound_`, `Runtime_` | things the engine names elsewhere |

## Evidence and open questions

The driver's names follow pret's for MusicPlayer2000 where its structure is
the same. The cue layer's come from what `AudioCommand_Play` does with each
range of numbers, and the songs' from `songs.h`, where a song without a proven
name keeps its number (`MUS_070`, `SE_188`).

Still unknown:

- XXX: what the five mixer records differ in, and why three songs take
  record 3.
- XXX: whether a library call dropped by the lock is ever noticeable.
- XXX: why the sound effects that name player 7 search only players 4 to 7.
