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

void Sound_LoadReverbCode(s32 preset);

/// The three cues the test starts with.
static const struct SoundTestCues sSoundTestCues = {{0, 80, 100}};

/// Runs the sound test's three cue selectors and five mixer presets forever,
/// from preset 3, with the sound VBlank state's byte 2 set, a clip timer of 2
/// and the commands 0, 78 and 390.
void SoundTest_Run(void) {
  struct SoundTestCues cues;
  s32 preset;
  s32 row;
  s32 timer;

  cues = sSoundTestCues;
  row = 0;
  preset = 3;
  Io_Write32(0, &gBuildStamp[0]);
  gSoundVBlankState.soundTestActive = 1;
  timer = 20;

  for (;;) {
    if (timer != 0)
      timer--;

    if (gSoundMixerClipped != 0) {
      gSoundMixerClipped = 0;
      timer = 2;
    }

    if (gInput.repeat & KEY_SELECT) {
      preset = (preset + 1) % 5;
      Sound_LoadReverbCode(preset);
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

/// Replaces the audio command mask, or toggles its requested bits when
/// flagged.
void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gSoundCommandMask ^= command;
  else
    gSoundCommandMask = command;
}

/// Returns the music Sound_Play last chose: 0xff before any, MUS_SILENCE after
/// a fade, else the last music.
u16 Sound_GetCurrentBgm(void) { return gCurrentBgm; }

/// Returns sound `sound`'s bit in gSoundFlags, one bit for each of 1024 sound
/// ids.
s32 SoundFlag_Test(s32 sound) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & sound);
  sound = (sound & 0x3ff) >> 3;
  bytes = gSoundFlags;
  value = bytes[sound] & mask;
  return NONZERO(value);
}

/// Sets sound `sound`'s bit in gSoundFlags.
void SoundFlag_Set(s32 sound) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bits = 0x300;
  bytes = gSoundFlags;
  sound = (sound & (bits + 0xff)) >> 3;
  bytes[sound] = (u8)(bytes[sound] | mask);
}

/// Clears sound `sound`'s bit in gSoundFlags.
void SoundFlag_Clear(s32 sound) {
  s32 mask;
  s32 bits;
  u8 *bytes;

  mask = 1 << (7 & sound);
  bits = 0x300;
  bytes = gSoundFlags;
  sound = (sound & (bits + 0xff)) >> 3;
  bytes[sound] = (u8)(bytes[sound] & ~mask);
}

s32 __divsi3(s32 numerator, s32 denominator);

/// Sets direct sound to a fixed 31536 Hz output with 528 samples a VBlank,
/// keeping mode's rate nibble, clears the two PCM buffers and points DMA 1
/// and 2 at them, then restarts timer 0 at line 159.
void Sound_SetFixedSampleFreq(u32 mode) {
  struct SoundInfo *work = SOUND_INFO_PTR;
  u32 samples;
  u8 *buffer;
  u32 control;
  volatile u8 *vcount;

  mode = (mode >> 16) & 15;
  work->transferPeriod = 3;
  work->outputRate = 31536;
  work->rate = mode;
  samples = 528;
  work->pcmSamplesPerVBlank = samples;
  work->rateStep = 266;
  control = 0;
  Io_Write16(control, &REG_TM0CNT_H);
  Io_Write16(__divsi3(-140448, samples), &REG_TM0CNT_L);
  buffer = gMixerPcmBuffer;
  Iwram_ClearWords(buffer, 0x18c0);
  m4aSoundVSyncOn();
  REG_DMA1SAD = (u32)buffer;
  buffer += 0xc60;
  Io_Write32((u32)buffer, &REG_DMA2SAD);
  vcount = (volatile u8 *)&REG_VCOUNT;
  while (*vcount == 159)
    ;
  while (*vcount != 159)
    ;
  control = TIMER_ENABLE;
  REG_TM0CNT_H = control;
}
