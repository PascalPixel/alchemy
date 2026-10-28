// Draft, differ 10 of 83: spawns the eight objects of a ring around (x, y),
// each at its own angle, with the turn rate and part that overlay 6b0's
// Func_02000660 (the orbit callback) reads back: radius in unk4c, angle in
// action, turn in unk66, centre in bounce and gravity. Written from the listing
// (no twin). The loop is a goto loop: as a for loop loop.c hoists 0x200000 and
// the part conversion (62 as a counting loop, 45 as a do-while). The polar
// offset goes through an inline so its 0x200000 is not shared with the radius
// store (as the native rebuilds it). Left: the native converts the part to u16
// with lsls/lsrs inside the loop; (u16)part (or a u16 prototype) has gcse's PRE
// hoist the lsls before the loop, which takes fp and spills the turn (37); part
// & 0xffff stays in the loop but builds a mask (12); the part passed as it
// is, used here, leaves out only the two conversion instructions (10). Measured:
// a copy of the part behind an in-out barrier (24), an in-out barrier on the
// part itself (36, part and the index swap registers), the conversion through
// an inline (37).

#include "types.h"
#include "actor.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern s32 gData_02003584[];

struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 part);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020032e0(struct Actor *object, s32 mode);
void Func_020032e8(struct Actor *object, const void *script);

static __inline__ void Vector_AddPolarOffsetFar_(s32 radius, s32 angle,
                                                 s32 *vector) {
  Vector_AddPolarOffsetFar(radius, angle, vector);
}

void Func_020009b8(s32 x, s32 y, s16 part, s16 a3) {
  s32 i = 0;

loop:
  {
    struct Actor *object = Import_ActorCreate(0x2a0, x, y, 0x980000);
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
    Actor_SetPartAttribute(object, part);
    Scene_SetActorSpritePriority(object, 2);
    Func_020032e0(object, 7);
    Actor_SetSingleChildFlags(object, 0);
    i += 2;
    Func_020032e8(object, gData_02003584);
  }
  if (i != 16)
    goto loop;
}
