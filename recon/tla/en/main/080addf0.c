// Near miss, differ 206 of 322, 2026-10-18, written from the listing and The
// Broken Seal's GameState_InitializeDefaults (games/tbs/src/game/game.c): clears the
// party state, the work block, the flags and the owners by DMA, builds the
// characters, sets a new game's defaults (the button layout, Felix and Jenna
// in the party, their first Psynergy, 200 coins, the message settings, the
// play time clock and the slot machine prize odds) and fills the work
// block's random tables. Left: the native loads gPartyState's address after
// the characters are built (this hoists it to the top); it builds the button
// layout's halfword constants with movs and pools only the byte constants
// 0, 1, 4 and 8 of the later stores (r8, r5, r10, r6), where agscc shares
// the first 1 and 2 with the later uses and pools those; the remaining
// differences follow from those registers.
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
extern u8 gAutoSleepEnabled;
extern u16 gIdleFrameCount;
extern s16 gData_020036d0;
extern u16 gData_02003860;
extern const u8 Data_080b1f2c[4][5];

void Func_080af7ac(void);
s32 Party_AddActiveOwner(s32 owner);
void Unit_LearnPsynergy(s32 owner, s32 ability);
u32 Runtime_GetBuildStampTime(void);
u32 Random16(void);

void Func_080addf0(void) {
  volatile s32 zero;
  u8 *state = (u8 *)&gPartyState;
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
  *(u16 *)(state + 0x230) = 1;
  *(u16 *)(state + 0x232) = 2;
  *(u16 *)(state + 0x234) = 4;
  *(u16 *)(state + 0x236) = 8;
  *(u16 *)(state + 0x238) = 0x200;
  *(u16 *)(state + 0x23a) = 0x100;
  *(u16 *)(state + 0x23c) = 2;
  *(u16 *)(state + 0x240) = 0;
  *(u16 *)(state + 0x242) = 0;
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
  *(u32 *)(state + 0x28c) = 0;
  *(u16 *)(state + 0x2bc) = 0;
  state[0x274] = 0;
  state[0x22c] = 1;
  state[0x22a] = 1;
  state[0x22b] = 1;
  state[0x225] = 0;
  state[0x226] = 8;
  *(u32 *)(state + 0) = 0;
  *(u32 *)(state + 0x2d8) = Runtime_GetBuildStampTime();
  gFrameWaitCount = 0;
  gAutoSleepEnabled = 0;
  gPartyState.playTime = 0;
  state[0x24a] = gAutoSleepEnabled;
  gIdleFrameCount = 0;
  gData_020036d0 = -1;
  state[0x286] = gData_02003860;
  state[0x13d] = 4;
  state[0x13e] = 4;
  state[0x13f] = 4;
  state[0x140] = 8;
  state[0x141] = 8;
  state[0x142] = 8;
  state[0x143] = 16;
  state[0x144] = 16;
  state[0x145] = 16;
  state[0x146] = 32;
  state[0x147] = 32;
  state[0x148] = 32;
  state[0x149] = 64;
  state[0x14a] = 64;
  state[0x14b] = 64;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 8; j++)
      gData_02001000[0x11b8 + i * 12 + j] = (Random16() * 5) >> 16;
  }
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 5; j++)
      gData_02001000[0x1338 + i * 5 + j] = Data_080b1f2c[i][j];
  }
}
