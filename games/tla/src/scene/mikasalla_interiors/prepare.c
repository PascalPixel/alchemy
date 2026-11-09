#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_02000454(void);

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000454(void) {
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_ClearFlag(0x12f);
  return;
}
