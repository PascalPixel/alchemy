#include "types.h"
#include "field_event.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

extern s16 gData_02000d80[];

void Func_02000d18(void);
void Func_02000d50(s16 *data);
void Func_02000d68(s32 a, s32 b);
void Actor_SetWideSprite(s32 actor, s32 flag);

void Func_02000d58(s16 *data);

void Func_02000a9c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 flags;

  Func_02000d58(gData_02000d80);
  object = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= object->priorityFlags;
  flags &= 0xfd;
  object->priorityFlags = flags;
}

s32 Func_02000ac8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila1)
    Func_02000d68(12, 0);
  else if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    Func_02000d50(gData_02000d80);
  Actor_SetSpritePriority(21, 1);
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2 &&
      GameFlag_Test(0x300) == 0)
    Actor_SetWideSprite(66, 0);
  return 0;
}

s32 Func_02000b44(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2) {
    if (GameFlag_Test(0x210))
      MapGrid_CopyCellAttributes(10, 19, 1, 1, 11, 19);
    if (GameFlag_Test(0x211))
      MapGrid_CopyCellAttributes(25, 20, 1, 1, 26, 20);
    if (GameFlag_Test(0x212))
      MapGrid_CopyCellAttributes(28, 23, 1, 1, 29, 23);
    if (GameFlag_Test(0x213))
      MapGrid_CopyCellAttributes(17, 11, 1, 1, 18, 11);
    if (GameFlag_Test(0x214))
      MapGrid_CopyCellAttributes(30, 17, 1, 1, 31, 17);
    if (GameFlag_Test(0x215))
      MapGrid_CopyCellAttributes(16, 23, 1, 1, 17, 23);
  }
  return 0;
}

void Func_02000c30(void) { Func_02000d18(); }
