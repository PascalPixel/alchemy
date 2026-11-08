#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
#include "ram_buffer.h"

struct Actor *Actor_GetById(u32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_02005d7c(struct Actor *object);
s32 Func_02000cec(s32 *a, s32 *b);

void Func_02001458(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other;
  s32 *pos;
  u8 i;

  if (*(s8 *)((u8 *)Ram_HeapSlots->block32 + 0x2a01) != 0)
    return;
  if (*(u16 *)(work + 0x1b2) == 0)
    return;
  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  if (object->action == 0) {
    if (object->x == 0) {
      Func_02005d7c(object);
    } else {
      object->z = 0;
      object->x = 0;
    }
    return;
  }
  if (Func_02000cec(&leader->x, pos = &object->x) <= 8 &&
      ((s16 *)&leader->y)[1] == ((s16 *)&object->y)[1]) {
    {
      s16 *counter = (s16 *)(work + 0x16a);
      s32 value = 201;

      *counter = value;
    }
    object->unk4c = 0;
    /* FAKEMATCH(barrier): an empty asm holds the next statement after this
     * one; as no asm, 22 of 200 instructions differ in tla-en. */
    asm("");
    object->speedLimit = -object->speedLimit >> 1;
    object->z = leader->z - 0x80000;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3) {
    other = Actor_GetById(8);
    if (Func_02000cec(&other->x, pos) <= 8) {
      object->speedLimit = -object->speedLimit >> 1;
      object->unk4c = 0;
      object->z = other->z - 0x80000;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    for (i = 0; i != 3; i++) {
      struct Actor *other2 = Actor_GetById(i + 12);
#define other other2
      if (Func_02000cec(&other->x, pos) <= 8) {
        object->speedLimit = -object->speedLimit >> 1;
        object->unk4c = 0;
        object->z = other->z - 0x80000;
      }
    }
#undef other
  }
  object->action--;
  object->z += object->speedLimit;
  object->x += object->acceleration;
  object->speedLimit -= object->unk4c;
  object->scaleX += SPLIT_CONSTANT(0xccc);
  object->scaleY += 0xccc;
}
