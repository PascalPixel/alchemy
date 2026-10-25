// Near miss, differ 28 of 322 (206 before, 2026-10-25), written from the
// listing and The Broken Seal's SaveBlock_InitializeDefaults
// (games/tbs/src/game/game/units.c): clears the party state, the work
// block, the flags and the owners by DMA, builds the characters, sets a new
// game's defaults (the button layout, Felix and Jenna in the party, their
// first Psynergy, 200 coins, the message settings, the play time clock and
// the slot machine prize odds) and fills the work block's random tables.
// The party state written by field rather than through a byte pointer taken
// at the top (206 to 132: the native loads its address after the
// characters), gAutoSleepEnabled and gIdleFrameCount volatile as The Broken
// Seal declares its gIdleSleepEnabled (132 to 46: the native rereads the
// flag after the play time store; frame.h would need them volatile on
// adoption) and Unit_LearnPsynergy returning s32 as add_action.c defines it
// (46 to 28: its arguments are then set r1 first). Left: the native loads the
// pooled byte constants 1 and 8 a slot or two from where this does, and the
// pools sit 4 bytes apart from that.
#include "types.h"
#include "party_state.h"
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
extern volatile u8 gAutoSleepEnabled;
extern volatile u16 gIdleFrameCount;
extern s16 gData_020036d0;
extern u16 gData_02003860;
extern const u8 Data_080b1f2c[4][5];

void Func_080af7ac(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Unit_LearnPsynergy(s32 owner, s32 ability);
u32 Runtime_GetBuildStampTime(void);
u32 Random16(void);

void Func_080addf0(void) {
  volatile s32 zero;
  s32 i;
  s32 j;

  zero = 0;
  START_DMA(&zero, &gPartyState, 0x850000b8);
  zero = 0;
  START_DMA(&zero, gData_02001000, 0x850007c8);
  WaitDma(DMA3);
  zero = 0;
  START_DMA(&zero, gGameFlagBytes, 0x85000080);
  gData_02001000[4] = 0xff;
  zero = 0;
  START_DMA(&zero, gOwnerStates, 0x85000298);
  Func_080af7ac();
  gPartyState.keys230 = 1;
  gPartyState.unk232 = 2;
  gPartyState.keys234 = 4;
  gPartyState.keys236 = 8;
  gPartyState.keys238 = 0x200;
  gPartyState.keys23a = 0x100;
  *(u16 *)gPartyState.unk23c = 2;
  gPartyState.packedEvents[0] = 0;
  gPartyState.packedEvents[1] = 0;
  gPartyState.currentOwner = 4;
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
  gPartyState.coins = 200;
  *(u32 *)&gPartyState.unk284[8] = 0;
  gPartyState.forgeItem = 0;
  gPartyState.counter274 = 0;
  gPartyState.eventMode = 1;
  gPartyState.battleFxEnabled = 1;
  gPartyState.unk22b = 1;
  gPartyState.paletteGlow[0] = 0;
  gPartyState.paletteGlow[1] = 8;
  *(u32 *)gPartyState.unk000 = 0;
  *(u32 *)((u8 *)&gPartyState + 0x2d8) = Runtime_GetBuildStampTime();
  gFrameWaitCount = 0;
  gAutoSleepEnabled = 0;
  gPartyState.playTime = 0;
  gPartyState.unk244[6] = gAutoSleepEnabled;
  gIdleFrameCount = 0;
  gData_020036d0 = -1;
  gPartyState.unk284[2] = gData_02003860;
  gPartyState.unk13d[0] = 4;
  gPartyState.unk13d[1] = 4;
  gPartyState.unk13d[2] = 4;
  gPartyState.unk13d[3] = 8;
  gPartyState.unk13d[4] = 8;
  gPartyState.unk13d[5] = 8;
  gPartyState.unk13d[6] = 16;
  gPartyState.unk13d[7] = 16;
  gPartyState.unk13d[8] = 16;
  gPartyState.unk13d[9] = 32;
  gPartyState.unk13d[10] = 32;
  gPartyState.unk13d[11] = 32;
  gPartyState.unk13d[12] = 64;
  gPartyState.unk13d[13] = 64;
  gPartyState.unk13d[14] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gData_02001000[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gData_02001000[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}
