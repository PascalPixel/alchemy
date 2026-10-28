# The Lost Age: sound

This folder holds The Lost Age's sound layer and its MusicPlayer2000 sound
driver, with the driver's handwritten routines under `driver/`. ☀️'s
[Sound](../../../tbs/src/sound/README.md) explains what a sound number means,
the jingle's return to the music and the driver's lock; this page says only
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
- **Sounds.** ⚓️ has its own song table (`games/tla/sound/songs.s`), so its
  sound numbers are not ☀️'s, and it has no `sounds.h` of names yet.
- **Names.** Routines that do what their ☀️ twins do take their names.

## Status

Functions not yet matched are drafts under `recon/tla/en/main`, by address;
one file is still named by its address (`f1c0cac.c`). `make progress` gives
the counts.

## Function prefixes

Functions take their ☀️ twins' names, which ☀️'s guides explain: the
driver's routines pret's MusicPlayer2000 names (`m4aSongNumStart`, `MPlayMain`,
`ply_note`), the sound layer `Sound_`. A few copies The Lost Age keeps beside
the driver still carry older prefixes (`MusicTrack_`, `Audio_`, `Pcm_`) with
their own suffixes, and `LzPack_` and `Runtime_` remain. Only `SoundFlag_`,
setting, clearing and testing a sound's flags, and
`Sound_SetFixedSampleFreq`, a fixed-rate variant of `SampleFreqSet`, are
The Lost Age's own so far.
