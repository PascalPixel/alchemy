// Draft, differ 47 of 76: overlay 699's copy of the function drafted in
// resource_698_c_0200180c.c (overlay 698's Func_0200180c, differ 47 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct Spot {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk06;
};

struct SpotState {
  u8 unk00[2];
  s16 flagByte;
  u8 unk04[6];
  s16 count;
  s16 current;
  u8 unk0e[0x12];
  struct Spot spots[1];
};

extern struct SpotState gData_020024f8;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02001654(struct SpotState *state, struct Spot *spot);
void Func_020016c4(struct SpotState *state, struct Spot *spot);

s32 Func_020019f4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct SpotState *state = &gData_020024f8;
  struct Spot *spot = state->spots;
  s32 i;
  s16 count = state->count;

  for (i = 0; i < count; i++, spot++) {
    if (spot->x == x && spot->z == z) {
      if (!GameFlag_Test(spot->flag)) {
        Func_02001654(state, spot);
        GameFlag_SetBit(spot->flag);
        state->current = i;
        break;
      }
      if (i == state->current)
        break;
      state->current = i;
      Func_020016c4(state, spot);
      GameFlag_SetByte(state->flagByte, x);
      GameFlag_SetByte(state->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}
