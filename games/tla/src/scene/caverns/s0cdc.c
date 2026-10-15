#include "caverns.h"

s32 Func_02000c38(struct FieldActor *object, s16 *position) {
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 blocked = Func_020010b4(object, position);

  if (blocked == 0) {
    s32 mask;

    Func_020010dc();
    Func_0200113c(0);
    Func_02001074(object, 6);
    WaitFrames(6);
    Func_02001164(152);
    Func_02001074(object, 7);
    object->speed = 0x30000;
    object->acceleration = 0x20000;
    object->velocityY = 0x40000;
    mask = 0x7e;
    mask &= *flags;
    *flags = mask;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    {
      struct PartyState *party = &gPartyState;
      s32 x = position[1];
      s32 z = position[5];
      s32 owner = party->currentOwner;

      /* FAKEMATCH: the one-pass block keeps the party state's address loaded
       * before the position (plain call: differ 2; owner read without the party
       * pointer: differ 12). */
      do {
        ObjectMotion_SetPositionAndCommit(owner, x, z);
      } while (0);
    }
    Func_02001074(object, 6);
    ObjectDispatch_SetSingleChildFlags(object, 1);
    *flags = saved;
    Func_020010e4();
    return 1;
  }
  return 0;
}

void Func_02000cdc(void) {
  struct FieldActor *leader = Object_GetById(gPartyState.currentOwner);
  s32 position[3];
  s32 x;

  /* FAKEMATCH: the one-pass block loads x before the offset is built (differ 2
   * without it). */
  do {
    x = leader->x;
  } while (0);
  x += 0x200000;
  position[0] = x;
  position[1] = leader->y;
  position[2] = leader->z;
  Func_02000c38(leader, (s16 *)position);
}
