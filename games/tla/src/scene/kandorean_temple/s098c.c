#include "local.h"

s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_02000930(struct Actor *object, s32 priority);
s32 Func_02000864(struct Actor *object);

void Func_0200098c(s32 *position, s32 value) {
  struct Actor *object =
      Func_02005140_(0x11d, position[0], position[1], position[2] - 0x20000);

  if (object) {
    s32 timer;

    object->bounce = Math_Cosine(((Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() * 2) + value;
    timer = (u16)Random16Far() - 0x8000;
    object->action = timer;
    Func_02000930(object, 3);
    object->update = (void (*)(void))Func_02000864;
    Animation_SetStateFlags(object, 1);
  }
}

s32 Func_02000824(struct Actor *a, struct Actor *b);

void Func_02000a0c(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *other = Actor_GetById(12);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH(dead): an empty once-only block, so the Z speed is stored before
   * the scale rate loads, as in Func_02000864; without it, 2 of 97 instructions
   * differ in tla-en. */
  do {
  } while (0);
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->animation;
  object->scaleX = scale + rate;
  {
    /* FAKEMATCH(pin): the call's first argument pinned to r0, so it is copied
     * before the Y scale store; as a plain local, copied after the action's
     * halfword load, 4 of 97 instructions differ in tla-en. */
    register struct Actor *first asm("r0") = object;

    object->scaleY += object->acceleration;
    /* FAKEMATCH(dead): an empty once-only block, so the Y scale is stored
     * before the action is addressed, as in Func_02000864; without it, 2 of 97
     * instructions differ in tla-en. */
    do {
    } while (0);
    sprite->rotation += object->action;
    if (Func_02000824(first, leader)) {
      ((struct FieldSprite *)object->animation)->priority = 3;
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
  if (Func_02000824(object, other)) {
    ((struct FieldSprite *)object->animation)->priority = 3;
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 3;
    object->unk4c = 0;
  }
}
