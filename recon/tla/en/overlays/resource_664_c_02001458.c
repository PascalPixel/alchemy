// Draft, differ 22 of 200 (was 67): a bouncing object's update: it stops
// animating while any of the event work's four busy counters is set; with its
// action count at zero it is removed (or parked at 0, 0); otherwise it
// bounces back off the leader (and records 201 in the event work's counter
// 0x16a), off object 8 in scene 0x57 or off objects 12 to 14 in scene 0x5c,
// moves by its speeds and grows. The 201 goes through an s32 (a halfword
// constant would come from the pool and move the pool into the middle), the
// object's position pointer is taken just before the leader test (so the
// leader takes r8 and the pointer r10), and the speed's update is read before
// the scale's. Left: in scene 0x5c's loop the native keeps the count in r5
// and the object in r7 (here the reverse), and a few rows of the leader's
// bounce come in another order. Measured: the loop as goto or do, a u32
// count (64), the loop's object in a variable of its own (no change).
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Func_02005d7c(struct ObjectRuntime *object);
s32 Func_02000cec(s32 *a, s32 *b);

void Func_02001458(struct ObjectRuntime *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *other;
  s32 *pos;
  u8 i;

  if (*(s8 *)((u8 *)Ram_HeapSlots->block32 + 0x2a01) != 0)
    return;
  if (*(u16 *)(work + 0x1b2) == 0)
    return;
  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0) {
    ObjectDispatch_SetAnimationSpeed(object, 0);
    return;
  }
  ObjectDispatch_SetAnimationSpeed(object, 16);
  if (object->action == 0) {
    if (object->x == 0) {
      Func_02005d7c(object);
    } else {
      object->z = 0;
      object->x = 0;
    }
    return;
  }
  pos = &object->x;
  if (Func_02000cec(&leader->x, pos) <= 8 &&
      ((s16 *)&leader->y)[1] == ((s16 *)&object->y)[1]) {
    {
      s16 *counter = (s16 *)(work + 0x16a);
      s32 value = 201;

      *counter = value;
    }
    object->unk4c = 0;
    object->speedLimit = -object->speedLimit >> 1;
    object->z = leader->z - 0x80000;
  }
  if (gAdventure.scene == (s32)&SceneId_Ov664Room3) {
    other = Object_GetById(8);
    if (Func_02000cec(&other->x, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - 0x80000;
    }
  } else if (gAdventure.scene == (s32)&SceneId_Ov664Room8) {
    for (i = 0; i != 3; i++) {
      other = Object_GetById(i + 12);
      if (Func_02000cec(&other->x, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - 0x80000;
      }
    }
  }
  object->action--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->speedLimit -= object->unk4c;
  object->scaleX += 0xccc;
  object->scaleY += 0xccc;
}
