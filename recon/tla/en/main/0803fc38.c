// Draft, differ 8 of 101 (2026-10-24): a one-pass block around the clearing
// DMA takes one reordered pair off. Left: instruction order only.
// Near miss, differ 10 of 101 (five swapped pairs, all else identical): The
// Broken Seal's OptionMenu_InitializeWork on this game's party state. For each
// option the native reads the saved byte one step earlier than here: movs r0,
// #160; ldrb r2, [r3]; lsls r0, r0, #3, where this draft finishes the shift
// before the load (and ldr r1, =gPartyState before movs r0, #147 at the top). A
// one-pass block cannot give that order: it would pull the load above the movs
// that starts the next offset. The party state fields are used by their header
// names.
#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "party_state.h"

struct OptionMarkerOutput {
  u8 unk00[14];
  u8 index;
};

union OptionCursor {
  struct OptionMarkerOutput *output;
  u8 data[0x10];
};

struct OptionWork {
  u8 unk000[0x574];
  u16 page;
  u16 tint[3];
  u16 cursorFrame;
  u16 preset;
  u8 unk580[0x14];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  union OptionCursor cursor;
  union OptionCursor marker[2];
  u8 unk5d4[0x18];
  void *frame[3][3];
};

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

void OptionMenu_InitializeWork(void) {
  struct OptionWork *work;
  volatile u32 zero;

  work = (struct OptionWork *)Runtime_AllocateBlock(208, 0x628);
  zero = 0;
  Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
  work->option[0] = gPartyState.paletteGlow[0];
  work->optionCount[0] = 24;
  work->option[1] = gPartyState.paletteGlow[1];
  work->optionCount[1] = 15;
  work->option[2] = gPartyState.eventMode;
  work->optionCount[2] = 3;
  work->option[3] = gPartyState.battleFxEnabled;
  work->optionCount[3] = 2;
  work->option[4] = gPartyState.unk244[6];
  work->optionCount[4] = 2;
  Scheduler_AddOrUpdateCallback(
      GraphicsPalette_LoadSelectionResourcesAndAdvance, 0x480);
}
