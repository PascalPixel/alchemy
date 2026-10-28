// Draft of overlay 6aa's Func_02004490, the twin of overlay 698's
// Func_0200180c: its draft (resource_698_c_0200180c.c) with this overlay's
// symbols: differ 47 of 76. Left: what that draft's header records.
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"

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

extern struct SpotState Data_020068b4;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_020040f0(struct SpotState *state, struct Spot *spot);
void Func_02004160(struct SpotState *state, struct Spot *spot);

s32 Func_02004490(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct SpotState *state = &Data_020068b4;
  struct Spot *spot = state->spots;
  s32 i;
  s16 count = state->count;

  for (i = 0; i < count; i++, spot++) {
    if (spot->x == x && spot->z == z) {
      if (!GameFlag_Test(spot->flag)) {
        Func_020040f0(state, spot);
        GameFlag_SetBit(spot->flag);
        state->current = i;
        break;
      }
      if (i == state->current)
        break;
      state->current = i;
      Func_02004160(state, spot);
      GameFlag_SetByte(state->flagByte, x);
      GameFlag_SetByte(state->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}
