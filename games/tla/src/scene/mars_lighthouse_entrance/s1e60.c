#include "room.h"

extern const u8 gData_020056a0[];

void Func_02001ae0(void);
void Func_02001cbc(void);
void Func_02001f90(struct Actor *object);

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020056a0. Overlays 6ab, 6ac and 6ad have the same function.
void Func_02001e1c(struct Actor *object, s32 part) {
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;
  {
    s32 three = 3;

    /* FAKEMATCH: an empty asm holding a 3 builds the part mask before the
     * sprite loads, and the zero stays a variable; as plain C, 4 instructions
     * differ in tla-en. */
    asm volatile("" : : "r"(three));
    ((struct SpriteParts *)object->animation)->part = part;
  }
  Actor_SetSingleChildFlags(object, 0);
  Func_02004bd0(object, gData_020056a0);
  object->action = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_02001e60(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001e1c(object, 3);
    object->update = Func_02001ae0;
    if (mode)
      object->unk5c[0] = 2;
    Func_02004bc0(object, 2);
  }
}

void Func_02001ea8(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001e1c(object, 3);
    object->update = Func_02001cbc;
    if (mode)
      object->unk5c[0] = 2;
    Func_02004bc0(object, 2);
  }
}

void Func_02001ef0(s32 *position, s32 speed, s32 mode) {
  struct Actor *object =
      Func_02004bd8_(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001e1c(object, 3);
    object->update = Func_02001cbc;
    if (mode)
      object->unk5c[0] = 2;
    Func_02004bc0(object, 2);
  }
}

s32 Func_02001f38(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)
      return 1;
  }
  return 0;
}

void Func_02001f90(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate */
  asm volatile("");
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->animation;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: ⚓️ stores the Y scale before it addresses the action */
  asm volatile("");
  parts->unk12 += object->action;
  if (leader->y == leader->terrainHeight && Func_02001f38(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

void Func_0200202c(struct Actor *object, s32 unused) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02004bc0(object, 2);
  Func_02004bd0(object, gData_020056a0);
  Actor_SetPartAttribute(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_0200206c(s32 *position, s32 speed) {
  struct Actor *object =
      Func_02004bd8_(285, position[0], position[1], position[2] + 0xfffe0000);
  s32 spin;

  if (object != NULL) {
    object->bounce = Math_Cosine((u16)(Random16Far() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + speed;
    spin = (u16)Random16Far() + 0xffff8000;
    object->action = spin;
    Func_0200202c(object, 2);
    object->update = (void (*)(void))Func_02001f90;
    Animation_SetStateFlags(object, 1);
  }
}
