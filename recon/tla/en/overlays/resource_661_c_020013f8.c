// Differ 4 of 124 (tla-en, 2026-11-08, -mtune=gba), unchanged this round: a
// function-level work pointer (65), the owner read into a local (4), the
// pointer to currentOwner itself (46), and an 8-minute permute found nothing.
// From 30: the last three moves take the
// work's address between the two cell calls, so it crosses the second call
// and lands in r5 as the native's does; that also gives the start its r2.
// Left: the pool load of &gMainRoutineWork sits before the second
// Func_020012d4 call here (2 rows in each of the two 19 moves) and just after
// it natively. Thumb has no first scheduling pass and sched2 keeps a pool load
// before a call, so the native's load was after the call in RTL while the
// pointer still crossed a call. Tried without gain: the work pointer taken
// before the first call (13), after the second (56, the 15 move crossjumps
// too), at function level or per branch (65, 78), through Scene_Call4090
// (12 to 54), the shifts in place (8 to 26); a 3-minute permute reached 26
// only through a long long temporary.
// The twin of Func_020012dc for the other room (row 89): resets the stone,
// copies the four blocks and moves the leader off the cells the blocks cover.
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "actor.h"

s32 Func_020012d4(s32 cell);

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = Func_020012d4(column);                                            \
    s32 z_ = Func_020012d4(row);                                               \
    Func_02004090(gMainRoutineWork.currentOwner, x_ << 16, z_ << 16);               \
  }

#define MOVE_LEADER_AGAIN(column, row_)                                        \
  {                                                                            \
    struct MainRoutineWork *work;                                              \
    s32 x_;                                                                    \
    s32 z_;                                                                    \
    x_ = Func_020012d4(column);                                                \
    work = &gMainRoutineWork;                                                  \
    z_ = Func_020012d4(row_);                                                  \
    Func_02004090(work->currentOwner, x_ << 16, z_ << 16);                     \
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
    MOVE_LEADER_AGAIN(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_AGAIN(19, 41)
    else if (row == 42)
      MOVE_LEADER_AGAIN(18, 42)
  }
}
