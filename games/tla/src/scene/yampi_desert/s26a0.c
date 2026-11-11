#include "scene.h"
#include "actor.h"
#include "task.h"

void Func_02001c5c(void);
void Func_020023cc(void);

/// Func_02002318's counterpart for actors 21 and 22: actor 22 becomes the
/// followed actor, and until flag 0x109 actor 21 is placed and given the
/// update Func_020023cc.
void Func_020026a0(void) {
  struct Actor *leader = (struct Actor *)Actor_GetById(21);
  struct Actor *object;
  struct FieldActor **followed;

  Scene_TaskAdd(Func_02002180, 0x480);
  /* FAKEMATCH(temporary): a pointer to gData_0200480c, taken here, loads its
   * address before the call that fetches actor 22, as in the native and in
   * Func_02002318; as the global named at each use. */
  followed = &gData_0200480c;
  object = (struct Actor *)Actor_GetById(22);
  object->priorityFlags |= 2;
  *followed = (struct FieldActor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Func_020040f8(22);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(22, 3);
  Scene_SetSpeed(21, 0x19999, 0xcccc);
  Actor_SetPartAttribute((struct FieldActor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Func_020040f8(21);
  Actor_GetById(21)->motionFlags = 3;
  if (!GameFlag_Test(0x109)) {
    Scene_Call4098(21, 232 << 16, 664 << 16, 0);
    *((u8 *)leader + 98) = 0;
    leader->update = Func_020023cc;
  }
  Func_020021d4();
  Task_Add(Func_02001c5c, TASK_PASS_VBLANK);
}
