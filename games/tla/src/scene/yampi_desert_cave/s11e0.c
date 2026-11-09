#include "yampi_desert_cave.h"
#include "actor.h"
#include "event_inline.h"

extern const s16 gData_020019cc[];

void GameFlag_SetByte(s32 flag, s32 value);

static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return ((s32)GameFlag_GetByte(flag));
}

void Func_020011e0(void) {
  s32 step = GameFlag_GetByte_(0x278);
  s32 index = (step / 32) * 2;
  s32 column = gData_020019cc[index];
  s32 row = gData_020019cc[index + 1];

  if (Actor_GetById(16)->movementState == 0) {
    Actor_SetHorizontalPositionWithTerrain(16, column << 19, row << 19);
    if (GameFlag_Test(0x104) == 0)
      step++;
    GameFlag_SetByte(0x278, step);
  }
}
