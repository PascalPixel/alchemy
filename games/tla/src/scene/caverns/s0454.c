#include "../../../../games/tla/src/scene/caverns/caverns.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Camera_FollowActor(s32 actor, s32 target);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 Func_020003b4(u32 layer, s32 x, s32 y);
s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value);

void Func_02000454(const u8 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader =
      (struct Actor *)Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 height;
  s32 cell;

  if (*ids == 0xffff)
    goto done;
loop:
  object = (struct Actor *)Actor_GetById(*ids);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->bounce = 0;
  Actor_SetSingleChildFlags((struct FieldActor *)object, 0);
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
    wait(1);
    leader->y = leader->terrainHeight =
        Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}
