#include "types.h"
#include "event.h"
#include "module/sound/sound.h"
#include "system.h"
#include "sound.h"

/// An empty function in the sound driver.
// As ☀️'s UnusedDummyFunc (games/tbs/src/sound/audio.c).
void Func_081c1010(void) {}

#include "io_reg.h"
#include "input.h"

#include "sound_driver_symbols.h"

/* VIEW(noop_stub): variable gBuildStamp as s32 gBuildStamp; sound.h has s32 gBuildStamp[]; tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/src/sound/noop_stub.c:41: `gBuildStamp' has an incomplete type. */
extern s32 gBuildStamp__noop_stub __asm__("gBuildStamp");

void Sound_LoadReverbCode(s32 preset);

/// The three cues the test starts with: the object's .rodata, which the
/// object list places after DummyFunc.
// As ☀️'s sound.c has its .rodata in the sound tail.
static const struct SoundTestCues sSoundTestCues = {{0, 80, 100}};

/// Runs the sound test's three cue selectors and five mixer presets forever,
/// from preset 3, with the sound VBlank state's byte 2 set, a clip timer of 2
/// and the commands 0, 78 and 390.
// As ☀️'s SoundTest_Run (games/tbs/src/sound/sound.c).
void SoundTest_Run(void) {
  struct SoundTestCues cues;
  s32 preset;
  s32 row;
  s32 timer;

  cues = sSoundTestCues;
  row = 0;
  preset = 3;
  // The store in a once-only block, after preset's setup and before the
  // VBlank state's; as the plain store, 10 of 145 instructions differ in
  // tla-en.
  do {
    gBuildStamp__noop_stub = 0;
  } while (0);
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
      Sound_Play(0);
    if (gInput.repeat & KEY_START)
      Sound_Play(78);
    if (gInput.repeat & KEY_SELECT)
      Sound_Play(390);

    wait(1);
  }
}

#include "sound_driver.h"

void Sound_UpdateCommandMask(u32 command) {
  u32 toggle = command & AUDIO_COMMAND_TOGGLE;

  command &= AUDIO_COMMAND_MASK;
  if (toggle != 0)
    gSoundCommandMask ^= command;
  else
    gSoundCommandMask = command;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

u16 Sound_GetCurrentBgm(void) { return gCurrentBgm; }

s32 SoundFlag_Test(s32 sound) {
  s32 mask;
  u8 *bytes;
  s32 value;

  mask = 1 << (7 & sound);
  sound = (sound & 0x3ff) >> 3;
  bytes = gSoundFlags;
  value = bytes[sound] & mask;
  return (u32)(-value | value) >> 31;
}

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

#include "iwram_call.h"

s32 __divsi3(s32 numerator, s32 denominator);

static __inline__ s32 Sound_ClearWords(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

/// Sets direct sound to a fixed 31536 Hz output with 528 samples a VBlank,
/// keeping mode's rate nibble, clears the two PCM buffers and points DMA 1
/// and 2 at them, then restarts timer 0 at line 159.
void Sound_SetFixedSampleFreq(u32 mode) {
  struct SoundWork *work = SOUND_WORK;
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
  // control holds both of timer 0's control values, each built in a register;
  // as the constants written directly, 22 of 58 instructions differ in tla-en.
  control = 0;
  // The timer's stop in a once-only block keeps its register's address after
  // the work's stores; as the plain store, 8 of 58 instructions differ in
  // tla-en.
  do {
    REG_TM0CNT_H = control;
  } while (0);
  // The reload's store in a once-only block keeps it ahead of the buffer's
  // address; as the plain store, 6 of 58 instructions differ in tla-en.
  do {
    REG_TM0CNT_L = __divsi3(-140448, samples);
  } while (0);
  buffer = gData_02003a90;
  /* FAKEMATCH(wrapper): the clear forwarded through Sound_ClearWords, so the
   * entry's address loads after the size's shift; as Iwram_ClearWords(buffer,
   * 0x18c0), 2 of 58 instructions differ in tla-en. */
  Sound_ClearWords(buffer, 0x18c0);
  m4aSoundVSyncOn();
  REG_DMA1SAD = (u32)buffer;
  buffer += 0xc60;
  // The second source's store in a once-only block keeps it ahead of VCOUNT's
  // address; as the plain store, 4 of 58 instructions differ in tla-en.
  do {
    REG_DMA2SAD = (u32)buffer;
  } while (0);
  vcount = (volatile u8 *)&REG_VCOUNT;
  while (*vcount == 159)
    ;
  while (*vcount != 159)
    ;
  control = 0x80;
  REG_TM0CNT_H = control;
}
