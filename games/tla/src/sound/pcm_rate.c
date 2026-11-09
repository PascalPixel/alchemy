#include "sound_driver.h"
#include "io_reg.h"
#include "iwram_call.h"

extern u8 gData_02003a90[];

s32 __divsi3(s32 numerator, s32 denominator);
void m4aSoundVSyncOn(void);

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
  /* FAKEMATCH: the clear goes through the inline wrapper Sound_ClearWords, as
   * ☀️'s IWRAM call layer, so the entry's pool load comes after the size's shift
   * (differ 2 called directly). */
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
