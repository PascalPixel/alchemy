#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
void Func_02000778(struct Actor *object, s32 dz);
void Func_02005388(s32 cue);
extern s32 gData_02006298;
extern s32 gData_0200629c;

void Func_020019d4(void) {
  struct Actor *leader;
  s32 cellX, cellZ, onCell;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cellX = leader->x >> 20;
  cellZ = leader->z >> 20;
  onCell = 0;
  if (cellX == 13 && cellZ == 13) {
    Flag_Set(0x208);
    onCell = 1;
  } else {
    Flag_Clear(0x208);
  }
  if (cellX == 23 && cellZ == 15) {
    Flag_Set(0x209);
    onCell = 1;
  } else {
    Flag_Clear(0x209);
  }
  if (onCell)
    Flag_Set(0x17d);
  else
    Flag_Clear(0x17d);
  if (gData_02006298 != 0 && leader->y == 0 && leader->x > 0x11c0000 &&
      leader->x < 0x1340000 && leader->z < 0xc80000 && !Flag_IsSet(0x20b))
    Func_02000778(leader, 0x26666);
  if (gData_0200629c != 0 && leader->y == 0 && leader->x > 0x14a0000 &&
      leader->x <= 0x165ffff && leader->z < 0xd80000 && !Flag_IsSet(0x20b))
    Func_02000778(leader, 0x39999);
  if (Flag_IsSet(0x20e)) {
    if (!Flag_IsSet(0x20c) && !Flag_IsSet(0x20d)) {
      Func_02005388(-1);
      Flag_Clear(0x20e);
    }
  } else if (Flag_IsSet(0x20c) || Flag_IsSet(0x20d)) {
    Func_02005388(162);
    Flag_Set(0x20e);
  }
}
