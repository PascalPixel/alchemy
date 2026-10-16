// Draft, differ 41 of 102: the native keeps the id list pointer in r8 and the
// leader in r7; this form swaps them (global.c ranks the pointer, 10 refs over
// 64 insns, above the leader, 8 over 86), and the rest follows from the swap.
// The native rematerializes 0xffff at both tests, so its loop has no hoisting;
// the goto form here is a measure of that, not an adoptable form. Tried: a
// while loop or a do-while inside an if (differ 57: loop.c hoists 0xffff into
// sl), the parameter itself as the pointer (differ 42), permuter 5 minutes
// (only changes of behaviour).
#include "../../../../games/tla/src/scene/caverns/caverns.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Object_AttachWorkTargetToObject(s32 actor, s32 target);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 Func_020003b4(u32 layer, s32 x, s32 y);
s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value);

void Func_02000454(const u8 *data) {
  struct ObjectRuntime *leader =
      (struct ObjectRuntime *)Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object;
  s32 height;
  s32 cell;
  const u16 *ids;

  ids = (const u16 *)data;
  if (*ids == 0xffff)
    goto done;
loop:
  object = (struct ObjectRuntime *)Object_GetById(*ids);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->bounce = 0;
  ObjectDispatch_SetSingleChildFlags((struct FieldActor *)object, 0);
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  cell = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = cell;
  Func_020003f0(0, object->x >> 20, object->z >> 20, cell | 0x80);
  ids++;
  if (*ids != 0xffff)
    goto loop;
done:
  if (GameFlag_Test(0x109)) {
    WaitFrames(1);
    leader->y = leader->terrainHeight =
        Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    Object_AttachWorkTargetToObject(gPartyState.currentOwner, 0);
  }
}
