#include "scene.h"
#include "actor.h"

// Const: the cell helper only computes from its argument.
s32 Func_020012d4(s32 cell) __attribute__((const));

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineWork *work = &gMainRoutineWork;                          \
    s32 x = Func_020012d4(column);                                             \
    s32 z = Func_020012d4(row);                                                \
    Func_02004090(work->currentOwner, x << 16, z << 16);                       \
  }

void Func_020013f8(void) {
  struct Actor *leader =
      (struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner);
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
    MOVE_LEADER_TO(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_TO(19, 41)
    else if (row == 42)
      MOVE_LEADER_TO(18, 42)
  }
}
