/// @file
/// The sound test, which nothing in the game calls; the heard-sound flags the
/// link lobby offers its tunes by; and the fixed 31536 Hz output rate the game
/// sets at start-up. Linked after Sound_Play's object, with the BIOS sound
/// call between them.

#include "sound/sound.h"
#include "macros.h"
#include "sounds.h"
#include "types.h"
#include "sound/sound_driver.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/system.h"

/// Unused: the sound test's own empty function, as sound.c keeps
/// UnusedDummyFunc; nothing calls it.
void SoundTest_UnusedDummyFunc(void) {}

void Sound_LoadReverbCode(s32 code);

/// The sounds the test's three selectors start on: one each for music, jingles
/// and sound effects.
static const struct SoundTestCues sSoundTestCues = {
    {0, SOUND_JINGLE_FIRST, SOUND_SE_FIRST}};

/// Unused: the sound test, which never returns. Up and Down choose a selector
/// (music, jingles, sound effects), Left and Right step its sound by 1 and L
/// and R by 10, A plays it, B silences the music and Start fades it, and Select
/// fades the action sounds and steps the reverb code, from SOUND_REVERB_LONG,
/// which this game's Sound_LoadReverbCode ignores.
void SoundTest_Run(void) {
  struct SoundTestCues cues;
  s32 reverbCode;
  s32 row;
  s32 timer;

  cues = sSoundTestCues;
  row = 0;
  reverbCode = SOUND_REVERB_LONG;
  Io_Write32(0, &gBuildStamp[0]);
  gSoundVBlankState.soundTestActive = 1;
  timer = 20;

  for (;;) {
    // A countdown, restarted whenever the mixer has clipped a sample since the
    // last frame; nothing reads it.
    if (timer != 0)
      timer--;

    if (gSoundMixerClipped != 0) {
      gSoundMixerClipped = 0;
      timer = 2;
    }

    if (gInput.repeat & KEY_SELECT) {
      reverbCode = (reverbCode + 1) % 5;
      Sound_LoadReverbCode(reverbCode);
    }
    if (gInput.repeat & KEY_R)
      cues.cue[row] += 10;
    if (gInput.repeat & KEY_L)
      cues.cue[row] -= 10;
    if (gInput.repeat & KEY_RIGHT)
      cues.cue[row] += 1;
    if (gInput.repeat & KEY_LEFT)
      cues.cue[row] -= 1;
    if (gInput.repeat & KEY_UP) {
      if (row > 0)
        row--;
    }
    if (gInput.repeat & KEY_DOWN) {
      if (row <= 1)
        row++;
    }
    if (gInput.repeat & KEY_A)
      Sound_Play(cues.cue[row]);
    if (gInput.repeat & KEY_B)
      Sound_Play(MUS_SILENCE);
    if (gInput.repeat & KEY_START)
      Sound_Play(SOUND_FADE_OUT_BGM);
    if (gInput.repeat & KEY_SELECT)
      Sound_Play(SOUND_FADE_OUT_ACTION_SE);

    wait(1);
  }
}

/// Unused: replaces gSoundCommandMask, or flips the bits named with
/// AUDIO_COMMAND_TOGGLE. Nothing calls it and nothing reads the mask.
void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gSoundCommandMask ^= command;
  else
    gSoundCommandMask = command;
}

/// Returns the music Sound_Play last started: MUS_SILENCE before any and after
/// a fade.
u16 Sound_GetCurrentBgm(void) { return gCurrentBgm; }

/// The sound ids gSoundFlags has a bit for.
enum { SOUND_FLAG_COUNT = 1024 };

/// Returns sound `sound`'s bit in gSoundFlags, one bit for each of the
/// SOUND_FLAG_COUNT sound ids.
s32 SoundFlag_Test(s32 sound) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & sound);
  sound = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
  bytes = gSoundFlags;
  value = bytes[sound] & mask;
  return NONZERO(value);
}

/// Sets sound `sound`'s bit in gSoundFlags.
void SoundFlag_Set(s32 sound) {
  s32 mask;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bytes = gSoundFlags;
  sound = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
  bytes[sound] = (u8)(bytes[sound] | mask);
}

/// Clears sound `sound`'s bit in gSoundFlags.
void SoundFlag_Clear(s32 sound) {
  s32 mask;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bytes = gSoundFlags;
  sound = (sound & (SOUND_FLAG_COUNT - 1)) >> 3;
  bytes[sound] = (u8)(bytes[sound] & ~mask);
}

s32 __divsi3(s32 numerator, s32 denominator);

/// The fixed output rate Sound_Initialize sets for the whole game.
enum {
  SOUND_FIXED_RATE = 31536, ///< samples a second: SOUND_MODE_FREQ_GAME's rate
  /// SOUND_FIXED_RATE over the 59.7275 VBlanks a second, rounded.
  SOUND_FIXED_SAMPLES_PER_VBLANK = 528
};

/// Sets direct sound to the fixed SOUND_FIXED_RATE with
/// SOUND_FIXED_SAMPLES_PER_VBLANK samples a VBlank, keeping mode's rate
/// nibble, clears the two PCM buffers and points DMA 1 and 2 at them, then
/// restarts timer 0 as the screen's last line begins.
void Sound_SetFixedSampleFreq(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 samples;
  u8 *buffer;
  u32 control;
  volatile u8 *vcount;

  mode = (mode >> SOUND_MODE_FREQ_SHIFT) &
         (SOUND_MODE_FREQ >> SOUND_MODE_FREQ_SHIFT);
  work->transferPeriod = SOUND_PCM_BUFFER_SIZE / SOUND_FIXED_SAMPLES_PER_VBLANK;
  work->outputRate = SOUND_FIXED_RATE;
  work->rate = mode;
  samples = SOUND_FIXED_SAMPLES_PER_VBLANK;
  work->pcmSamplesPerVBlank = samples;
  // Half the rate's reciprocal in 24-bit fixed point, as SampleFreqSet's.
  work->rateStep = SOUND_RATE_STEP_ONE / 2 / SOUND_FIXED_RATE;
  control = 0;
  Io_Write16(control, &REG_TM0CNT_H);
  // Timer 0 overflows twice a sample: half a VBlank's cycles over its samples.
  Io_Write16(__divsi3(-(SOUND_CYCLES_PER_VBLANK / 2), samples), &REG_TM0CNT_L);
  buffer = gMixerPcmBuffer;
  // Each channel's buffer here is twice the driver's own.
  Iwram_ClearWords(buffer, 4 * SOUND_PCM_BUFFER_SIZE);
  m4aSoundVSyncOn();
  REG_DMA1SAD = (u32)buffer;
  buffer += 2 * SOUND_PCM_BUFFER_SIZE;
  Io_Write32((u32)buffer, &REG_DMA2SAD);
  vcount = (volatile u8 *)&REG_VCOUNT;
  while (*vcount == DISPLAY_HEIGHT - 1)
    ;
  while (*vcount != DISPLAY_HEIGHT - 1)
    ;
  control = TIMER_ENABLE;
  REG_TM0CNT_H = control;
}
