// Draft, differ 47 of 76: in 17 overlays of the 698 family. The listing keeps
// the state in r6 and the index in r7 and loads the count's halfword once (ldrh
// r2), sign-extending it each pass; this draft swaps r6 and r7 and keeps the
// count sign-extended. Tried: continue or nested if (76), the table by name and
// index (96), a while loop, i declared first, the count as s16, u16 or s32
// local (47 to 49) or as a u16 field (76).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

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

extern struct SpotState gData_02002308;

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_0200146c(struct SpotState *state, struct Spot *spot);
void Func_020014dc(struct SpotState *state, struct Spot *spot);

s32 Func_0200180c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct SpotState *state = &gData_02002308;
  struct Spot *spot = state->spots;
  s32 i;
  s16 count = state->count;

  for (i = 0; i < count; i++, spot++) {
    if (spot->x == x && spot->z == z) {
      if (!GameFlag_Test(spot->flag)) {
        Func_0200146c(state, spot);
        GameFlag_SetBit(spot->flag);
        state->current = i;
        break;
      }
      if (i == state->current)
        break;
      state->current = i;
      Func_020014dc(state, spot);
      GameFlag_SetByte(state->flagByte, x);
      GameFlag_SetByte(state->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}
