/// @file
/// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears the sway
/// state and places each person, then starts the sway's task.
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/actor.h"
#include "event_inline.h"

extern u16 gSceneState[];
extern const s16 gData_0200533c[][2];

void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);

void Func_02002cb4(u8 mode) {
  u16 *state = gSceneState;
  u16 *pending = state + 1;

  if (!GameFlag_Test(265)) {
    s32 i;
    s32 id;

    *state = 0;
    *pending = 0;
    Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
    for (i = 0, id = 8; id <= 16; i++, id++) {
      Actor_SetHorizontalPositionWithTerrain(id, gData_0200533c[i][0] << 19, gData_0200533c[i][1] << 19);
      Func_02002b04(id, mode);
    }
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_02002b8c, priority);
  }
}
