# The Lost Age: sound

This folder holds The Lost Age's cue layer and its MusicPlayer2000 sound
driver, with the driver's handwritten routines under `driver/`. ☀️'s
[Sound](../../../tbs/src/sound/README.md) explains what a cue number means,
the fanfare's return to the music and the driver's lock; this page says only
what differs here.

## What differs from ☀️

- **Compilers.** The driver's C, `sound.c`, `tables.c`, `engine.c`, `tail.c`
  and the `music_track_*.c` files, is compiled with pret's `old_agbcc`, the
  sound route of the `Makefile` (`SOUND_C`), as ☀️'s driver is. The rest takes
  K2's options on top of ☀️'s (`CFLAGS_tla`): `-mthumb-split-constants`,
  `-mthumb-call-via-lr` and `-mtune=arm9tdmi`, without `-mthumb-interwork`.
- **Layout.** ⚓️ splits the driver into one file per routine where ☀️ keeps
  `engine.c` and `audio.c` whole; the song commands' handlers are the
  `music_track_*.c` files, and the handwritten ARM and Thumb routines are one
  file each under `driver/`.
- **Songs.** ⚓️ has its own song table (`games/tla/sound/songs.s`), so its
  cue numbers are not ☀️'s, and it has no `songs.h` of names yet.
- **Names.** Routines that do what their ☀️ twins do take their names.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address;
one file is still named by its address (`f1c0cac.c`). `make progress` gives
the counts.

## Function prefixes

Functions take their ☀️ twins' prefixes, which ☀️'s guides explain:
`Audio_`, `AudioCommand_`, `AudioEngine_`, `Cgb_`, `CgbAudio_`, `CgbChannel_`,
`CgbNote_`, `LzPack_`, `MusicCommand_`, `MusicPlayer_`, `MusicTrack_`, `Pcm_`,
`Runtime_` and `Sound_`. Only `SongFlag_`, setting, clearing and testing a
song's flags, is The Lost Age's own so far.
