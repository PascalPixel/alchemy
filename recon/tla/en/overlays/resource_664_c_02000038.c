// Near misses of overlay 664.
// Func_02000038 (51 instructions, 10 differ) and Func_02000908 (58, 28): entry
// lists of three halfwords ended by 0xffff. The native tests the first entry
// through r0 before it copies the cursor and rebuilds 0xffff on every pass; a
// while loop moves the constant out, a goto loop copies the cursor first.
// Func_02000128 (75, 8): the native loads the priority flags into r2 before it
// clears the cell byte and builds the bit 2 in r3; the draft holds the flags in
// r3. Func_020004d4 (53, 28): register names of the random offsets (r2 for the
// base, the leader coordinate loaded before the mask).
#include "../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"

struct AirsRockInsideEntry {
  u16 id;
  s16 kind;
  s16 flag;
};

u32 Random16Far(void);

static inline u32 Random16Far_(void) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer. */
  return Random16Far();
}
s32 Func_02005f64(struct FieldActor *object);
void Func_020006cc(struct FieldActor *object);
void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02000038(s16 *entry) {
  if ((u16)entry[0] == 0xffff)
    return;
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
    {
      u8 *flags = &object->priorityFlags;
      s32 bit = 2; /* FAKEMATCH: the native builds the bit in the register it
                      ORs into */
      s32 old = *flags;

      cell[2] = 0;
      *flags = bit | old;
    }
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

  if (Random16Far() & 255) {
    object->x = ((Random16Far_() & 255) << 16) + 0xb00000;
    object->y = leader->y + ((Random16Far_() & 31) << 16);
    object->z = leader->z + ((Random16Far_() & 127) << 16);
  }
  *(s16 *)((u8 *)object + 102) = *(s16 *)((u8 *)object + 100);
  if (*(s16 *)((u8 *)object + 100) == 212)
    Func_02005d5c(object, 9);
  else
    Func_02005d5c(object, 10);
}

void Func_02000908(s16 *entry) {
  if ((u16)entry[0] == 0xffff)
    return;
next:
  {
    struct FieldActor *object = Actor_GetById((u16)entry[0]);
    s32 flag = entry[2];
    s32 kind = entry[1];

    Actor_SetSingleChildFlags(object, 0);
    Func_02005d5c(object, (u16)kind * 3 + GameFlag_Test((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    *(s16 *)((u8 *)object + 100) = flag;
    Func_020006cc(object);
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}
