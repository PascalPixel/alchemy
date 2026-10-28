#include "types.h"
#include "sequence_parent_work.h"
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

/// Starts a new game's state: clears gSequenceParentWork, the work block, the
/// flags and the owners by DMA, builds the characters, sets the defaults (the
/// button layout, Felix and Jenna in the party, their first Psynergy, 200
/// coins, the message settings and the play time clock) and fills the work
/// block's random tables.
void Func_080addf0(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  zero = 0;
  START_DMA(&zero, &gSequenceParentWork, 0x850000b8);
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
  gSequenceParentWork.keys230 = 1;
  gSequenceParentWork.unk232 = 2;
  gSequenceParentWork.keys234 = 4;
  gSequenceParentWork.keys236 = 8;
  gSequenceParentWork.keys238 = 0x200;
  gSequenceParentWork.keys23a = 0x100;
  *(u16 *)gSequenceParentWork.unk23c = 2;
  gSequenceParentWork.packedEvents[0] = 0;
  gSequenceParentWork.packedEvents[1] = 0;
  gSequenceParentWork.currentOwner = 4;
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
  gSequenceParentWork.coins = 200;
  *(u32 *)&gSequenceParentWork.unk284[8] = 0;
  gSequenceParentWork.forgeItem = 0;
  gSequenceParentWork.counter274 = 0;
  gSequenceParentWork.eventMode = 1;
  gSequenceParentWork.battleFxEnabled = 1;
  gSequenceParentWork.unk22b = 1;
  gSequenceParentWork.paletteGlow[0] = 0;
  gSequenceParentWork.paletteGlow[1] = 8;
  *(u32 *)gSequenceParentWork.unk000 = 0;
  *(u32 *)((u8 *)&gSequenceParentWork + 0x2d8) = BuildStamp_GetTime();
  gFrameWaitCount = 0;
  gAutoSleepEnabled = 0;
  gSequenceParentWork.playTime = 0;
  gSequenceParentWork.unk244[6] = gAutoSleepEnabled;
  gIdleFrameCount = 0;
  gData_020036d0 = -1;
  gSequenceParentWork.unk284[2] = gData_02003860;
  gSequenceParentWork.unk13d[0] = 4;
  gSequenceParentWork.unk13d[1] = 4;
  gSequenceParentWork.unk13d[2] = 4;
  gSequenceParentWork.unk13d[3] = 8;
  gSequenceParentWork.unk13d[4] = 8;
  gSequenceParentWork.unk13d[5] = 8;
  gSequenceParentWork.unk13d[6] = 16;
  gSequenceParentWork.unk13d[7] = 16;
  gSequenceParentWork.unk13d[8] = 16;
  gSequenceParentWork.unk13d[9] = 32;
  gSequenceParentWork.unk13d[10] = 32;
  gSequenceParentWork.unk13d[11] = 32;
  gSequenceParentWork.unk13d[12] = 64;
  gSequenceParentWork.unk13d[13] = 64;
  gSequenceParentWork.unk13d[14] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gData_02001000[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gData_02001000[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}
