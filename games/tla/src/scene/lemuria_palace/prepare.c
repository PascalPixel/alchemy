#include "types.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"

void Task_Add(void (*callback)(void), s32 priority);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_020000f4(void);
void Func_02002024(s32 actor, s32 a, s32 b);

/// The room's setup: it schedules the palette cycle Func_020000f4.
s32 Func_02000158(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Value2((s32 (*)())Task_Add, (s32)Func_020000f4, 0xc80);
  Actor_SetSpritePriority(8, 3);
  if (gAdventure.entrance == 6)
    Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Test(0x96a)) {
    Func_02002024(7, 0, 0);
    Func_02002024(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
