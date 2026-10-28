#include "scene.h"

s32 Func_020012d4(s32 cell);

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = Func_020012d4(column);                                            \
    s32 z_ = Func_020012d4(row);                                               \
    Func_02004090(gMainRoutineWork.currentOwner, x_ << 16, z_ << 16);               \
  }

void Func_020012dc(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldActor *object = Actor_GetById(64);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (object != NULL)
    Scene_Call4090(64, 154 << 18, 222 << 17);
  Func_02003ff0(82, 82, 15, 40, 5, 6);
  Scene_Call4000(82, 82, 5, 6, 15, 40);
  Func_02003ff0(88, 82, 36, 25, 5, 5);
  Scene_Call4000(88, 82, 5, 5, 36, 25);
  if (column == 15) {
    if (row == 41)
      MOVE_LEADER_TO(14, 41)
    else if (row == 43)
      MOVE_LEADER_TO(16, 43)
  } else if (column == 16) {
    if (row == 40)
      MOVE_LEADER_TO(16, 39)
    else if (row == 41)
      MOVE_LEADER_TO(16, 42)
  } else if (column == 18) {
    if (row == 40)
      MOVE_LEADER_TO(18, 41)
    else if (row == 43)
      MOVE_LEADER_TO(17, 43)
  }
}
