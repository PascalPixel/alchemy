#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_ids.h"

void GameFlag_ClearBit();
void Func_02000454(void);

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000454(void) {
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Call1((void (*)())GameFlag_ClearBit, 0x12f);
  return;
}
