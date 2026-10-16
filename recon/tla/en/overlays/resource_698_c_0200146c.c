// Draft, differ 2 of 49 (one reordered pair): in 14 overlays of the 698 family.
// The listing loads the leader's x (ldr r6, [r0, #8]) between building the 64
// mask and copying it; this draft copies the mask first. Tried: operands of |
// either way, compound |=, the mask as a variable (6), 0x40 with u16 fields
// (12), x as a local, an asm barrier between the two values (4), a minute of
// alchemy permute (45,329 candidates, none better).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct Tile {
  u8 unk00[6];
  s16 x;
  s16 z;
};

struct ObjectRuntime *Object_GetById(s32 id);
void FieldEvent_WaitFrames(s32 frames);
void Func_020013e4(void);
void Func_02001b44(s32 sound);
void Func_02001a44(s32 x, s32 z, s32 width, s32 height, s32 from_x, s32 from_z);

void Func_0200146c(struct Tile *tile) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 fromX = (leader->x >> 20) | (s16)(tile->x & 64);
  s32 fromZ = (leader->z >> 20) | (s16)(tile->z & 64);

  Func_020013e4();
  Func_02001b44(161);
  Func_02001a44(tile->x, tile->z + 1, 1, 1, fromX, fromZ);
  FieldEvent_WaitFrames(12);
}
