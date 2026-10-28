# The Lost Age: sound

The Lost Age's [sound](../../../tbs/glossary.md#sound) layer and its MusicPlayer2000 sound driver,
with the driver's handwritten routines under `driver/`, and the mixer the game copies into IWRAM
(`resident_mixer*.s`).
[Sound](../../../tbs/src/sound/README.md) explains, for The Broken Seal, what
a sound number means, how a jingle returns to the music and the driver's lock.

## What differs from The Broken Seal

- The driver's C (`sound.c`, `tables.c`, `driver.c`, `tail.c` and the
  `music_track_*.c` files) is compiled with `old_agbcc` through the
  `Makefile`'s `SOUND_C`, as The Broken Seal's driver is.
- The driver is split into one file per routine, where The Broken Seal keeps
  `driver.c` and `audio.c` whole: the handlers of the song commands are the
  `music_track_*.c` files, and the handwritten ARM and Thumb routines are one
  file each under `driver/`. One file is still named by its address
  (`f1c0cac.c`).
- This game has its own song table (`games/tla/sound/songs.s`), and it has no
  `sounds.h` of names yet.
- A few copies kept beside the driver carry older prefixes (`MusicTrack_`,
  `Audio_`).
- `Sound_SetFixedSampleFreq`, a fixed-rate variant of `SampleFreqSet`, and
  the `SoundFlag_` functions, which set, clear and test one bit per sound
  number (`gSoundFlags`), have no twin.
