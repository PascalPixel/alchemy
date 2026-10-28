#include "scene.h"
#include "object_runtime.h"

void Func_02001c5c(void);
void Func_02002208(void);

/// Readies actors 16 and 17 and the room's callbacks; actor 17 becomes the
/// followed actor (gData_0200480c).
void Func_02002318(void) {
  struct ObjectRuntime *leader = (struct ObjectRuntime *)Object_GetById(16);
  struct ObjectRuntime *object;
  struct FieldActor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  /* FAKEMATCH: through the pointer, taken here, gData_0200480c's address loads
   * before the call that fetches actor 17, as in the native (differ 2 naming
   * the global at each use, or the pointer taken at the top). */
  followed = &gData_0200480c;
  object = (struct ObjectRuntime *)Object_GetById(17);
  object->priorityFlags |= 2;
  *followed = (struct FieldActor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(17);
  (*followed)->motionFlags = 3;
  ObjectMotion_SetActionVariant(17, 3);
  Scene_SetSpeed(16, 0x19999, 0xcccc);
  Object_SetPartAttribute((struct FieldActor *)leader, 15);
  ObjectMotion_SetActionVariant(16, 3);
  Func_020040f8(16);
  Object_GetById(16)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    *((u8 *)leader + 98) = 0;
    leader->update = Func_02002208;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, 0x480);
}
