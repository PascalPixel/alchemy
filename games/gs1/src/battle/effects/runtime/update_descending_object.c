#include "types.h"
struct DescendingObjectState {
  u8 padding_00[6];
  u16 angle;
  u8 padding_08[4];
  u32 scale;
  u8 padding_10[8];
  u32 primary_position;
  u32 secondary_position;
  u8 padding_20[0x34];
  s8 active;
};
void BattleEffect_UpdateDescendingObject(struct DescendingObjectState *state) {
  u32 next_position;
  state->secondary_position -= 0x280;
  next_position = state->primary_position - 0x280;
  state->angle = state->angle + 0x2000;
  state->scale = state->scale + 0x10000;
  state->primary_position = next_position;
  if ((s32)next_position < 0x3000) {
    state->active = 0;
  }
}
