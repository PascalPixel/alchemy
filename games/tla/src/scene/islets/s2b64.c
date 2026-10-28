#include "islets.h"

s32 Func_02002b64(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  s16 scene;

  work->field1ac = 0x100;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov674Room1)
    Func_02002c38();
  else if (scene == (s32)&SceneId_Ov674Room2)
    Func_02002f1c();
  else if (scene == (s32)&SceneId_Ov674Room3)
    Func_020030b4();
  else if (scene == (s32)&SceneId_Ov674Room4)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov674Room5)
    Func_0200329c();
  return 0;
}

void Func_02002bd8(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;

  asm volatile(
      ""
      : "+l"(y)); /* FAKEMATCH: the listing loads y before it builds 0x80000 */
  if (y > 0x80000) {
    Actor_GetById(9)->collisionFlags &= ~8;
    Actor_GetById(10)->collisionFlags &= ~8;
  } else {
    Actor_GetById(9)->collisionFlags |= 8;
    Actor_GetById(10)->collisionFlags |= 8;
  }
}
