// Draft, differ 8 of 58, skeleton gap 2 (tla-en, 2026-11-01): Func_02000908,
// the worst of four; measure each with --symbol.
// Matches: differ 0 for Func_02000038, Func_02000128 and Func_020004d4, all
// plain C. s0550.c holds Func_020004d4 with two pins and two barriers that the
// form here does without, so its adoption can drop them.
// Func_02000908 is left with two pairs. The native clears motionFlags before it
// copies the actor for collisionFlags; the draft's copy schedules ahead of the
// store (the same pair took a barrier in aqua_rock_inside/s2e04.c), and neither
// a pointer to the field, BYTE_AT nor struct Actor moves it. The native loads
// the flag with its index in r3 and the value in r2 before mov r8; the draft
// swaps them, through s16 locals and an extra s16 local alike.
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"
#include "actor.h"

u32 Random16Far(void);
s32 Func_02005f64(struct FieldActor *object);
void Func_020006cc(struct FieldActor *object);
void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02000038(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct FieldActor *object = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02000128(id, (u16)kind, (u16)flag, BYTE_AT(object, 34));
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = Actor_GetById(id);
  u8 *cell = ((struct AirsRockInsideMapWork *)Ram_HeapSlots->mapWork)
                 ->layers[layer]
                 .cells;

  cell += Func_02005f64(object) * 4;
  if (GameFlag_Test(flag)) {
    u8 flags = object->priorityFlags;
    s32 bits;

    cell[2] = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 2);
      break;
    case 1:
      Func_02005d5c(object, 4);
      break;
    case 2:
      Func_02005d5c(object, 6);
      break;
    }
  } else {
    cell[2] = 255;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 1);
      break;
    case 1:
      Func_02005d5c(object, 3);
      break;
    case 2:
      Func_02005d5c(object, 5);
      break;
    }
  }
}

void Func_020004d4(struct FieldActor *object) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *self = (struct Actor *)object;

  if (Random16Far() & 255) {
    object->x = ((Random16Far() & 255) << 16) + (176 << 16);
    object->y = leader->y + ((Random16Far() & 31) << 16);
    object->z = leader->z + ((Random16Far() & 127) << 16);
  }
  self->unk66 = self->action;
  if (self->action == 212)
    Func_02005d5c(object, 9);
  else
    Func_02005d5c(object, 10);
}

void Func_02000908(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    struct FieldActor *object = Actor_GetById((u16)entry[0]);
    s32 flag = entry[2];
    s32 kind = entry[1];

    Actor_SetSingleChildFlags(object, 0);
    Func_02005d5c(object, (u16)kind + (u16)kind * 2 + GameFlag_Test((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    *(s16 *)((u8 *)object + 100) = flag;
    Func_020006cc(object);
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}
