# The Lost Age: sound

The Lost Age's [sound](../../../tbs/glossary.md#sound) layer and its MusicPlayer2000 sound driver,
with the driver's handwritten routines under `driver/`, and the mixer the game copies into IWRAM
(`resident_mixer*.s`).
[Sound](../../../tbs/src/sound/README.md) explains, for The Broken Seal, what
a sound number means, how a jingle returns to the music and the driver's lock.

## What differs from The Broken Seal

- The driver's C (`sound.c`, `tables.c`, `driver.c` and
  `player_work_copy.c`) is compiled with `old_agbcc` through the
  `Makefile`'s `SOUND_C`, as The Broken Seal's driver is.
- `driver.c` holds the handlers of the song commands, as The Broken Seal's
  does; the handwritten ARM and Thumb routines are one file each under
  `driver/`.
- This game has its own song table (`games/tla/sound/songs.s`), and it has no
  `sounds.h` of names yet.
- A few copies kept beside the driver carry older prefixes (`MusicTrack_`,
  `Audio_`).
- `midi_key_to_freq_work_copy.s` and `sound_work_copy.s` hold copies of
  routines from `sound.c`, 0x1a70 bytes before them: the same bytes, but with
  their pool words left 0 and their calls keeping the originals' offsets, so
  they land short of the routines the originals call. Nothing refers to them.
- `Sound_SetFixedSampleFreq`, a fixed-rate variant of `SampleFreqSet`, and
  the `SoundFlag_` functions, which set, clear and test one bit per sound
  number (`gSoundFlags`), have no twin.
