// Draft, differ 34 of 76: overlay 6b0's copy of resource_6ab_c_02003f9c.c, with
// this overlay's symbols; that header says what is left.
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

extern struct SpotState Data_02004458;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002d40(struct SpotState *state, struct Spot *spot);
void Func_02002db0(struct SpotState *state, struct Spot *spot);

s32 Func_020030e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct SpotState *state = &Data_02004458;
  struct Spot *spot = state->spots;
  s32 i;
  s32 first;
  s32 count;

  first = state->count;
  __asm__("" : : : "memory");
  count = (u16)state->count;
  i = 0;
  if (i < first) {
  next:
    if (spot->x == x && spot->z == z) {
      if (!GameFlag_Test(spot->flag)) {
        Func_02002d40(state, spot);
        GameFlag_SetBit(spot->flag);
        state->current = i;
        goto done;
      }
      if (i == state->current)
        goto done;
      state->current = i;
      Func_02002db0(state, spot);
      GameFlag_SetByte(state->flagByte, x);
      GameFlag_SetByte(state->flagByte + 8, z);
      return 1;
    }
    i++;
    spot++;
    if (i < (s16)count)
      goto next;
  }
done:
  return 0;
}
