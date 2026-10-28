// Draft, differ 34 of 76: overlay 6ab's copy of the spot check drafted in
// resource_698_c_0200180c.c, with this overlay's symbols, its loop as a goto
// loop. The listing tests the count once with ldrsh before the loop and loads
// it again unextended (ldrh) for the loop's own test, sign-extending it on
// every pass: here the second read follows an empty asm with a memory clobber
// (as plain reads CSE shares one ldrsh: 47). Left: the native keeps the state
// in r6 and the index in r7, here the other way round (an asm on the state,
// other declaration orders and permute for 4 minutes change nothing).
#include "types.h"
#include "actor.h"
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

extern struct SpotState gData_02004cf4;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_02003bfc(struct SpotState *state, struct Spot *spot);
void Func_02003c6c(struct SpotState *state, struct Spot *spot);

s32 Func_02003f9c(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct SpotState *state = &gData_02004cf4;
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
        Func_02003bfc(state, spot);
        GameFlag_SetBit(spot->flag);
        state->current = i;
        goto done;
      }
      if (i == state->current)
        goto done;
      state->current = i;
      Func_02003c6c(state, spot);
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
