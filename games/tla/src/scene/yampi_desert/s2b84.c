#include "scene.h"
#include "object_runtime.h"

void Func_02001c5c(void);
void Func_02002764(void);

// Func_02002318's counterpart for actors 21 and 22 in Room 5: after flag
// 0x88b actor 21 is placed facing 0x4000; otherwise, until flag 0x109, it
// is placed and given the update Func_02002764.
void Func_02002b84(void) {
  struct ObjectRuntime *leader = (struct ObjectRuntime *)Object_GetById(21);
  struct ObjectRuntime *object;
  struct FieldActor **followed;
  s32 angle;

  /* FAKEMATCH: through the pointer, taken here, gData_0200480c's address loads
   * before the call that fetches actor 22, as in the native (differ 2 naming
   * the global at each use). */
  followed = &gData_0200480c;
  object = (struct ObjectRuntime *)Object_GetById(22);
  object->priorityFlags |= 2;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  *followed = (struct FieldActor *)object;
  Func_020040f8(22);
  ObjectMotion_SetActionVariant(22, 3);
  (*followed)->motionFlags = 3;
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Object_SetPartAttribute((struct FieldActor *)leader, 15);
  ObjectMotion_SetActionVariant(21, 3);
  Func_020040f8(21);
  Object_GetById(21)->motionFlags = 3;
  if (GameFlag_Test(0x88b)) {
    angle = 0x4000;
    Scene_Call4098(21, 232 << 16, 168 << 16, angle);
  } else {
    Scene_TaskAdd(Func_02002180, 0x480);
    if (!GameFlag_Test(0x109)) {
      Scene_Call4098(21, 200 << 16, 264 << 16, 0);
      *((u8 *)leader + 98) = 0;
      leader->update = Func_02002764;
    }
  }
  Func_020021d4();
  Scheduler_AddOrUpdateCallback(Func_02001c5c, 0x480);
}
