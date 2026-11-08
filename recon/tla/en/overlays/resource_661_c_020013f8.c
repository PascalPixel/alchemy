// Matches: differ 0 (tla-en, 2026-11-08)
// Two things close it. Func_020012d4 is declared const, which it is (it reads
// no memory): a call to it no longer flushes pending loads in sched2, so the
// pool load of &gMainRoutineWork drifts past the second cell call as the
// native's does. The original likely had the helper static in this file,
// where mark_constant_function infers the same. And the move takes the work's
// address at the top of its block, so the pointer crosses both cell calls and
// takes r5 (x then r6), as in the native. One macro serves all four moves: in
// the first, CSE reuses the owner address read at the top (r8).
// Without const: the pointer taken between the two calls stayed at differ 4
// (the load sat before the second call); the twin's plain macro is 56; a
// function-level work pointer is 65; an inline MoveLeader helper is 13.
// The twin of Func_020012dc for the other room (row 89): resets the stone,
// copies the four blocks and moves the leader off the cells the blocks cover.
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
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
