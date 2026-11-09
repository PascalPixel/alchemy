#include "types.h"
#include "actor.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern s32 gData_02003584[];

void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_020032e0(struct Actor *object, s32 mode);
void Func_020032e8(struct Actor *object, const void *script);

static __inline__ void Vector_AddPolarOffsetFar_(s32 radius, s32 angle,
                                                 s32 *vector) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 48 of 83
   * instructions differ in tla-en. */
  Vector_AddPolarOffsetFar(radius, angle, vector);
}

void Func_020009b8(s32 x, s32 y, s16 part, s16 a3) {
  s32 i;

  for (i = 0; i != 16;) {
    struct Actor *object =
        (FieldActor_Create(0x2a0, x, y, 0x980000));
    s32 angle = i << 12;

    Vector_AddPolarOffsetFar_(0x200000, angle, &object->x);
    object->action = angle;
    object->unk66 = a3;
    BYTE_AT(object, 98) = i;
    BYTE_AT(object, 99) = part;
    object->bounce = x;
    object->unk4c = 0x200000;
    object->gravity = y;
    object->motionFlags = 0;
    Animation_SetStateFlags(object, 1);
    Actor_SetSpriteColor(object, (u16)part);
    Scene_SetActorSpritePriority(object, 2);
    Func_020032e0(object, 7);
    Actor_SetSingleChildFlags(object, 0);
    Func_020032e8(object, gData_02003584);
    i += 2;
  }
}
