// Draft, differ 30 of 124 (was 48): the twin of Func_020012dc for the other
// room (row 89): resets the stone, copies the four blocks and moves the
// leader off the cells the blocks cover. The last three moves shift the
// column in place (row <<= 16), as the native keeps the shifted value in r6.
// Left: the native forms &gSequenceParentWork.currentOwner in r2 at the start (r1
// here) and, for the last two moves, in r5 from the pool with the offset in
// r3 (here r3 and r1).
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "actor.h"

s32 Func_020012d4(s32 cell);

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = Func_020012d4(column);                                            \
    s32 z_ = Func_020012d4(row);                                               \
    Func_02004090(gSequenceParentWork.currentOwner, x_ << 16, z_ << 16);               \
  }

#define MOVE_LEADER_AGAIN(column, row_) \
  { \
    row = Func_020012d4(column); \
    row <<= 16; \
    Func_02004090(gSequenceParentWork.currentOwner, row, \
                  Func_020012d4(row_) << 16); \
  }

void Func_020013f8(void) {
  struct Actor *leader =
      (struct Actor *)Actor_GetById(gSequenceParentWork.currentOwner);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (Actor_GetById(64) != NULL)
    Func_02004090(64, 0, 0);
  Func_02003ff0(82, 89, 15, 40, 5, 6);
  Scene_Call4000(82, 89, 5, 6, 15, 40);
  Func_02003ff0(88, 89, 36, 25, 5, 5);
  Scene_Call4000(88, 89, 5, 5, 36, 25);
  if (column == 15 && row == 44)
    MOVE_LEADER_TO(15, 43)
  else if (column == 17 && row == 44)
    MOVE_LEADER_AGAIN(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_AGAIN(19, 41)
    else if (row == 42)
      MOVE_LEADER_AGAIN(18, 42)
  }
}
