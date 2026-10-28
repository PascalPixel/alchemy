#include "types.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "task.h"
#include "sequence_parent_work.h"

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

void Func_0803fc38(void) {
  struct OptionWork *work;
  volatile u32 zero;

  work = (struct OptionWork *)HeapSlot_AllocEwram(208, 0x628);
  zero = 0;
  Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
  work->option[0] = gSequenceParentWork.paletteGlow[0];
  work->optionCount[0] = 24;
  work->option[1] = gSequenceParentWork.paletteGlow[1];
  work->optionCount[1] = 15;
  work->option[2] = gSequenceParentWork.eventMode;
  work->optionCount[2] = 3;
  work->option[3] = gSequenceParentWork.battleFxEnabled;
  work->optionCount[3] = 2;
  work->option[4] = gSequenceParentWork.unk244[6];
  work->optionCount[4] = 2;
  Task_Add(
      GraphicsPalette_LoadSelectionResourcesAndAdvance, TASK_PASS_VBLANK);
}
