#include "caverns.h"
#include "macros.h"

s32 Func_02000c38(struct FieldActor *object, s16 *position) {
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 blocked = Func_020010b4(object, position);

  if (blocked == 0) {
    s32 mask;

    Func_020010dc();
    Func_0200113c(0);
    Func_02001074(object, 6);
    wait(6);
    Func_02001164(152);
    Func_02001074(object, 7);
    object->speed = 0x30000;
    object->acceleration = 0x20000;
    object->velocityY = 0x40000;
    mask = 0x7e;
    mask &= *flags;
    *flags = mask;
    Actor_SetSingleChildFlags(object, 0);
    {
      struct SequenceParentWork *party = &gSequenceParentWork;
      s32 x = position[1];
      s32 z = position[5];
      s32 owner = party->currentOwner;

      /* FAKEMATCH: the one-pass block keeps gSequenceParentWork's address loaded
       * before the position (plain call: differ 2; owner read without the party
       * pointer: differ 12). */
      do {
        Actor_SetPositionAndCommit(owner, x, z);
      } while (0);
    }
    Func_02001074(object, 6);
    Actor_SetSingleChildFlags(object, 1);
    *flags = saved;
    Func_020010e4();
    return 1;
  }
  return 0;
}

void Func_02000cdc(void) {
  struct FieldActor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 position[3];
  s32 x;

  /* FAKEMATCH: the one-pass block loads x before the offset is built (differ 2
   * without it). */
  do {
    x = leader->x;
  } while (0);
  x += 0x200000;
  SET_VECTOR(position, x, leader->y, leader->z);
  Func_02000c38(leader, (s16 *)position);
}
