// Draft, differ 313 of 305: pushes an object to the stopping point
// Func_02000dcc found: clears byte 2 of the cells under its footprint, steps
// the leader back and lets it push (mode 8, sound 239), slides the object over
// (Func_02002310) while the leader follows, runs the optional callback, then
// sets the object down (sounds 298 and 213), copies the cells to its new place
// and fills byte 2 there. Shared with overlay 6a1 and others. It takes the
// six-word block by value, as room 3's local.h declares it (id and callback are
// its last two words, so r0 to r3 are stored above the frame with the stack
// words), and its speed calls go through the call layer. Left: the native keeps
// a zero step in r9 for the push offset's index where this folds it, spills the
// width where this spills the map, and register choice throughout.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern const s8 gData_02002484[][4];
extern const s32 gData_0200249c[];

struct ObjectRuntime *Object_GetById(s32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void WaitTicks(s32 ticks);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_020022e0(struct ObjectRuntime *object, s32 mode);
void Func_02002470(s32 sound);
void Func_02002310(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_02002318(struct ObjectRuntime *object);
void Func_02002330(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_02002430(void);
void Func_02001300(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02001050(struct PushTarget target) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object = Object_GetById(target.id);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 from[3];
  s32 cell[3];
  const s8 *rect;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 step = 0;

  from[0] = object->x;
  from[2] = object->z;
  rect = gData_02002484[target.footprint];
  cell[0] = (object->x + (rect[0] << 16)) >> 20;
  cell[2] = (object->z + (rect[1] << 16)) >> 20;
  width = (ABS(rect[0]) + ABS(rect[2])) >> 4;
  height = (ABS(rect[1]) + ABS(rect[3])) >> 4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, gPartyState.currentOwner,
        0x8000, 0x1999);
  Object_SetModeById(gPartyState.currentOwner, 8);
  WaitTicks(15);
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner,
                                            (target.x - from[0]) >> 17,
                                            (target.z - from[2]) >> 17);
  Object_GetById(gPartyState.currentOwner)->update = Func_02001300;
  WaitTicks(4);
  Func_020022e0(object, 2);
  Func_02002470(239);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, target.id, 0x8000, 0x1999);
  Func_02002310(object, target.x, target.y, target.z);
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 2);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, gPartyState.currentOwner,
        0x4ccc, 0x1999);
  ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner,
                                            (gData_0200249c[step] << 16) >> 31,
                                            (gData_0200249c[step] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
  Object_SetModeById(gPartyState.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02002318(object);
  Func_02002470(298);
  Func_02002470(213);
  Object_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_020022e0(object, 1);
  target.x = (target.x + (rect[0] << 16)) >> 20;
  target.z = (target.z + (rect[1] << 16)) >> 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Func_02002330(target.x, target.z, width, height, originX + target.x,
                originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (rect[0] << 16)) >> 20;
  cell[2] = (from[2] + (rect[1] << 16)) >> 20;
  Func_02002330(originX + cell[0], originZ + cell[2], width, height, cell[0],
                cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_02002430();
}
