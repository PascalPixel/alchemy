#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "event_work.h"
#include "call.h"

struct EventFlags {
  u8 unk000[0xd36];
  u8 unkd36;
  u8 unkd37;
};

void Actor_SetModeById(s32 id, s32 mode);
void Func_020005e0(void);
void Func_020005e8(s32 a, s32 b, s32 c);

s32 Func_02000104(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  ((struct EventFlags *)work)->unkd37 = 0;
  ((struct EventFlags *)work)->unkd36 = 0;
  gSequenceParentWork.eventMode = 2;
  Func_020005e0();
  Value3((s32 (*)())Func_020005e8, 0, 10, 11);
  Actor_SetModeById(9, 4);
  return 0;
}
