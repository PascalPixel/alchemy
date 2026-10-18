#include "shrine_of_the_sea_god.h"
#include "macros.h"

extern const u8 gData_02003cbc[];

void Func_02000f18(s32 actor) {
  Func_02003aa0(actor);
  Object_GetById(actor)->priorityFlags |= 0x20;
}

void Func_02000f34(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  GameFlag_SetBit(0x200);
}

void Func_02000f58(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  GameFlag_SetBit(0x201);
}

void Func_02000f80(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  GameFlag_SetBit(0x202);
}

void Func_02000fa8(void) { Func_02003b38(gData_02003cbc); }

void Func_02000fb8(void) {
  struct ShrineOfTheSeaGodObject *object;

  Func_02003b40();
  object = Object_GetById(8);
  if (object->x >> 20 == 21 && object->z >> 20 == 13)
    GameFlag_SetBit(0x207);
  else
    GameFlag_ClearBit(0x207);
}

void Func_02000fec(void) {
  struct ShrineOfTheSeaGodObject *object = Object_GetById(8);

  Func_02003b40();
  if (object->x >> 20 == 12)
    GameFlag_SetBit(0x84c);
}

void Func_02001010(struct ShrineOfTheSeaGodObject *object) {
  object->motionFlags = 3;
  do {
    WaitFrames(1);
  } while (object->speed != 0);
  Func_02003b48(188);
  WaitFrames(10);
  object->motionFlags = 0;
}

void Func_02001038(void) {
  struct ShrineOfTheSeaGodObject *object = Object_GetById(8);
  s32 z = object->z >> 20;

  if (z == 16) {
    Func_020039d8();
    Func_02003b08(0);
    Func_02001010(object);
    ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, z);
    ObjectMotion_SetActionVariant(8, 3);
    Func_020039e0();
    GameFlag_SetBit(0x301);
  }
}

void Func_02001088(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  u32 id;

  for (id = 9; id <= 13;) {
    u8 *flags = &Object_GetById(id)->priorityFlags;
    u32 next = id + 1;

    if (leader->y > 0x100000) {
      ObjectMotion_SetActionVariant(id, 2);
      /* FAKEMATCH: the native ties the flag or to the constant's register. */
      *flags = *flags + 2 - (*flags & 2);
    } else {
      ObjectMotion_SetActionVariant(id, 1);
      *flags &= ~2;
    }
    id = next;
  }
}

void Func_020010e8(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  s32 y;

  Func_02003b28();
  y = leader->y;
  /* FAKEMATCH: loads the leader height before it builds the threshold. */
  __asm__("");
  if (y < 0x100000)
    ((u8 *)&gPartyState)[0x2c3] = 0;
  else
    ((u8 *)&gPartyState)[0x2c3] = 1;
}

void Func_02001128(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Object_GetById(gPartyState.currentOwner);
  s32 y;
  s32 z;

  y = leader->y;
  /* FAKEMATCH: loads the leader height before it builds the threshold. */
  __asm__("");
  if (y < 0x100000) {
    z = leader->z;
    /* FAKEMATCH: loads the leader depth before it builds the threshold. */
    __asm__("");
    if (z < 0x1280000) {
      ObjectMotion_SetActionVariant(10, 1);
      ObjectMotion_SetActionVariant(11, 1);
      ObjectMotion_SetActionVariant(12, 2);
    } else {
      ObjectMotion_SetActionVariant(10, 2);
      ObjectMotion_SetActionVariant(11, 2);
      ObjectMotion_SetActionVariant(12, 3);
    }
  } else {
    ObjectMotion_SetActionVariant(10, 2);
    ObjectMotion_SetActionVariant(11, 2);
    ObjectMotion_SetActionVariant(12, 3);
  }
}

void Func_020011a0(void) {
  struct ShrineOfTheSeaGodObject *first = Object_GetById(8);
  struct ShrineOfTheSeaGodObject *second = Object_GetById(9);

  SET_POSITION(second, first->x, 0x140000, first->z);
}

void Func_020011c0(void) {
  Object_GetById(gPartyState.currentOwner)->priorityFlags &= ~0x20;
}

void Func_020011e0(void) {
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 0x20;
}
