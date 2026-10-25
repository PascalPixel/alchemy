// Draft, differ 93 of 200: a bouncing object's update: it stops animating
// while any of the event work's four busy counters is set; with its action
// count at zero it is removed (or parked at 0, 0); otherwise it bounces back
// off the leader (and records 201 in the event work's counter 0x16a), off
// object 8 in scene 0x57 or off objects 12 to 14 in scene 0x5c, moves by
// its speeds and grows. Left: registers throughout (the native keeps the
// action field's address in r9 and the object's position address in r10)
// and the native's goto loop over objects 12 to 14.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Func_02005d7c(struct ObjectRuntime *object);
s32 Func_02000cec(s32 *a, s32 *b);

void Func_02001458(struct ObjectRuntime *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *other;
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
  if (Func_02000cec(&leader->x, &object->x) <= 8 &&
      ((s16 *)&leader->y)[1] == ((s16 *)&object->y)[1]) {
    *(s16 *)(work + 0x16a) = 201;
    object->unk4c = 0;
    object->speedLimit = -object->speedLimit >> 1;
    object->z = leader->z - 0x80000;
  }
  if (gPartyState.scene == 0x57) {
    other = Object_GetById(8);
    if (Func_02000cec(&other->x, &object->x) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - 0x80000;
    }
  } else if (gPartyState.scene == 0x5c) {
    for (i = 0; i != 3; i++) {
      other = Object_GetById(i + 12);
      if (Func_02000cec(&other->x, &object->x) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - 0x80000;
      }
    }
  }
  object->action--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->scaleX += 0xccc;
  object->speedLimit -= object->unk4c;
  object->scaleY += 0xccc;
}
