// Near miss, differ 2: the listing loads the object x (ldr r6, [r0, #8]) before
// it copies the mask 64 (adds r3, r2, #0); here the copy comes first. A barrier
// naming column, mask and x changes the allocation (differ 25). Also measured:
// one-pass blocks before, between and after x and z (2 to 8), x and z shifted
// first and ORed after (2), the mask as a local (6), s16 locals for the masked
// cells (2), five minutes of alchemy permute (2). Its copy group is led by
// 698's Func_0200146c in another lane.
#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"

struct MarsStarCell {
  u8 unk00[6];
  s16 column;
  s16 row;
};

void Func_02003b74(void);

void Func_02003bfc(struct MarsStarCell *cell) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = (object->x >> 20) | (s16)(cell->column & 64);
  s32 z = (object->z >> 20) | (s16)(cell->row & 64);

  Func_02003b74();
  Func_0200446c(161);
  Func_02004234(cell->column, cell->row + 1, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
}
