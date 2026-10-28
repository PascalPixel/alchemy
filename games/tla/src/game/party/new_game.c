#include "types.h"
#include "main_routine_work.h"
#include "dma.h"

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000
#define START_DMA(source_, destination_, control_)                             \
  Dma_Set((const void *)(source_), (void *)(destination_), (control_),         \
          (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel) {
  while ((channel->control & DMA3_BUSY) != 0) {
  }
}

extern u8 gData_02001000[];
extern u8 gGameFlagBytes[];
extern u8 gOwnerStates[];
extern u32 gFrameWaitCount;
/* FAKEMATCH(volatile): the auto-sleep flag read again after the play time
 * store, as The Broken Seal declares its gIdleSleepEnabled; as plain u8, as
 * frame.h declares it, 108 of 322 instructions differ in tla-en. */
extern volatile u8 gAutoSleepEnabled;
extern volatile u16 gIdleFrameCount;
extern s16 gData_020036d0;
extern u16 gData_02003860;
extern const u8 Data_080b1f2c[4][5];

void Func_080af7ac(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Unit_LearnPsynergy(s32 owner, s32 ability);
u32 BuildStamp_GetTime(void);
u32 Random16(void);

/// Starts a new game's state: clears gMainRoutineWork, the work block, the
/// flags and the owners by DMA, builds the characters, sets the defaults (the
/// button layout, Felix and Jenna in the party, their first Psynergy, 200
/// coins, the message settings and the play time clock) and fills the work
/// block's random tables.
void Func_080addf0(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  zero = 0;
  START_DMA(&zero, &gMainRoutineWork, 0x850000b8);
  zero = 0;
  START_DMA(&zero, gData_02001000, 0x850007c8);
  // The Japanese release waits through a copy of the channel register the
  // transfer above left in r3 where alchemy-gcc rebuilds the address (one
  // instruction); no form tried reproduces it, so that edition keeps this
  // function on its scaffold.
  WaitDma(DMA3);
  zero = 0;
  START_DMA(&zero, gGameFlagBytes, 0x85000080);
  gData_02001000[4] = 0xff;
  zero = 0;
  START_DMA(&zero, gOwnerStates, 0x85000298);
  Func_080af7ac();
  gMainRoutineWork.keys230 = 1;
  gMainRoutineWork.unk232 = 2;
  gMainRoutineWork.keys234 = 4;
  gMainRoutineWork.keys236 = 8;
  gMainRoutineWork.keys238 = 0x200;
  gMainRoutineWork.keys23a = 0x100;
  *(u16 *)gMainRoutineWork.unk23c = 2;
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gMainRoutineWork.currentOwner = 4;
  Party_AddActiveOwner(4);
  Party_AddActiveOwner(5);
  Unit_LearnPsynergy(4, 149);
  Unit_LearnPsynergy(4, 140);
  Unit_LearnPsynergy(6, 141);
  Unit_LearnPsynergy(7, 33);
  Unit_LearnPsynergy(0, 149);
  Unit_LearnPsynergy(0, 140);
  Unit_LearnPsynergy(1, 140);
  Unit_LearnPsynergy(2, 141);
  Unit_LearnPsynergy(2, 144);
  gMainRoutineWork.coins = 200;
  *(u32 *)&gMainRoutineWork.unk284[8] = 0;
  gMainRoutineWork.forgeItem = 0;
  gMainRoutineWork.counter274 = 0;
  gMainRoutineWork.eventMode = 1;
  gMainRoutineWork.battleFxEnabled = 1;
  gMainRoutineWork.unk22b = 1;
  gMainRoutineWork.paletteGlow[0] = 0;
  gMainRoutineWork.paletteGlow[1] = 8;
  *(u32 *)gMainRoutineWork.unk000 = 0;
  *(u32 *)((u8 *)&gMainRoutineWork + 0x2d8) = BuildStamp_GetTime();
  gFrameWaitCount = 0;
  gAutoSleepEnabled = 0;
  gMainRoutineWork.playTime = 0;
  gMainRoutineWork.unk244[6] = gAutoSleepEnabled;
  gIdleFrameCount = 0;
  gData_020036d0 = -1;
  gMainRoutineWork.unk284[2] = gData_02003860;
  gMainRoutineWork.unk13d[0] = 4;
  gMainRoutineWork.unk13d[1] = 4;
  gMainRoutineWork.unk13d[2] = 4;
  gMainRoutineWork.unk13d[3] = 8;
  gMainRoutineWork.unk13d[4] = 8;
  gMainRoutineWork.unk13d[5] = 8;
  gMainRoutineWork.unk13d[6] = 16;
  gMainRoutineWork.unk13d[7] = 16;
  gMainRoutineWork.unk13d[8] = 16;
  gMainRoutineWork.unk13d[9] = 32;
  gMainRoutineWork.unk13d[10] = 32;
  gMainRoutineWork.unk13d[11] = 32;
  gMainRoutineWork.unk13d[12] = 64;
  gMainRoutineWork.unk13d[13] = 64;
  gMainRoutineWork.unk13d[14] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gData_02001000[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gData_02001000[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}
