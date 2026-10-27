// Draft, differ 345 of 342: the push itself: the leader leans in, the object
// slides to its end, and its footprint moves between the map layers. Left:
// the frame (the native spills all four argument words) and register choice.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"
#include "party_state.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct PushMove {
  s32 x, y, z;
  s32 kind;
  s32 slot;
  void (*callback)(void);
};

extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02000754(void);
void Func_02001828(struct ObjectRuntime *object, s32 mode);
void Func_02001838(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02001840(struct ObjectRuntime *object);
void Func_02001848(s32 x, s32 z, s32 w, s32 h, s32 toX, s32 toZ);
void Func_02001938(void);
void Func_02001960(s32 song);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 w, s32 h, s32 value);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void WaitFrames(s32 frames);

void Func_02000458(struct PushMove move) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object = Object_GetById(move.slot);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 from[3];
  s32 cell[3];
  s32 i = 0;
  s32 w, h, offset;
  s32 ax, az, bx, bz;

  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02001974[move.kind][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_02001974[move.kind][1] << 16)) >> 20;
  w = (ABS(gData_02001974[move.kind][0]) + ABS(gData_02001974[move.kind][2])) >>
      4;
  h = (ABS(gData_02001974[move.kind][1]) + ABS(gData_02001974[move.kind][3])) >>
      4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x1999);
  Object_SetModeById(gPartyState.currentOwner, 8);
  WaitFrames(15);
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner,
                                            (move.x - from[0]) >> 17,
                                            (move.z - from[2]) >> 17);
  Object_GetById(gPartyState.currentOwner)->update = Func_02000754;
  WaitFrames(4);
  Func_02001828(object, 2);
  Func_02001960(239);
  ObjectMotion_SetSpeedParameters(move.slot, 0x8000, 0x1999);
  Func_02001838(object, move.x, move.y, move.z);
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 2);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x4ccc, 0x1999);
  offset = gData_0200198c[i] << 16;
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner,
                                            offset >> 31, offset >> 17);
  if (move.callback != NULL)
    move.callback();
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 1);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02001840(object);
  Func_02001960(298);
  Func_02001960(213);
  Object_SetPositionAndResetMotion(object, move.x, object->y, move.z);
  Func_02001828(object, 1);
  move.x = (move.x + (gData_02001974[move.kind][0] << 16)) >> 20;
  move.z = (move.z + (gData_02001974[move.kind][1] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0] >> 20;
  bz = *(s32 *)&map->layers[2].unk08[4] >> 20;
  Func_02001848(move.x, move.z, w, h, ax + move.x, az + move.z);
  Func_02001848(move.x + bx, move.z + bz, w, h, move.x + 64, move.z + 64);
  Map_FillCellRectByte2(0, move.x, move.z, w, h, 255);
  Map_FillCellRectByte2(2, move.x, move.z, w, h, 255);
  cell[0] = (from[0] + (gData_02001974[move.kind][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02001974[move.kind][1] << 16)) >> 20;
  Func_02001848(ax + cell[0], az + cell[2], w, h, cell[0], cell[2]);
  Func_02001848(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx, cell[2] + bz);
  Func_02001938();
}
