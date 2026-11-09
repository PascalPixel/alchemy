#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Func_02001b3c();

s32 Func_02000198(void) {
  s32 clear;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  clear = Event_GetFlag(0x109);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Func_02001b3c(gMainRoutineWork.currentOwner);
  }
  gMainRoutineWork.savedScene = (s32)&SceneId_Ov694Room2;
  gMainRoutineWork.savedEntrance = 6;
  return 0;
}
