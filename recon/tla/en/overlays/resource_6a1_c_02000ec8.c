// Draft, differ 288 of 305 (was 337): overlay 6a1's copy of the function
// drafted in resource_69c_c_02001050.c (overlay 69c's Func_02001050), whose
// header says what it is and what was tried. Forwarding calls through inline
// wrappers (FAKEMATCH(wrapper) on adoption) gained 49.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
};

extern const s8 gData_020022dc[][4];
extern const s32 gData_020022f4[];

struct Actor *Actor_GetById(s32 id);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void WaitFrames(s32 frames);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Func_02002160(struct Actor *object, s32 mode);
void Func_020022c8(s32 sound);
void Func_02002190(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02002198(struct Actor *object);
void Func_020021b0(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_020022a0(void);
void Func_02001178(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static __inline__ void Func_020021b0_(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ) {
  Func_020021b0(x, z, width, height, toX, toZ);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_02000ec8(struct PushTarget target, s32 id, void (*callback)(void)) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor *object = Actor_GetById(id);
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
  rect = gData_020022dc[target.footprint];
  cell[0] = (object->x + (rect[0] << 16)) >> 20;
  cell[2] = (object->z + (rect[1] << 16)) >> 20;
  width = (ABS(rect[0]) + ABS(rect[2])) >> 4;
  height = (ABS(rect[1]) + ABS(rect[3])) >> 4;
  Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedParameters_(gAdventure.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gAdventure.currentOwner, 8);
  WaitFrames(15);
  Actor_OffsetPositionAndResetMotion(gAdventure.currentOwner,
                                            (target.x - from[0]) >> 17,
                                            (target.z - from[2]) >> 17);
  Actor_GetById(gAdventure.currentOwner)->update = Func_02001178;
  WaitFrames(4);
  Func_02002160(object, 2);
  Func_020022c8(239);
  Actor_SetSpeedParameters_(id, 0x8000, 0x1999);
  Func_02002190(object, target.x, target.y, target.z);
  Actor_WaitForMoveById(gAdventure.currentOwner);
  Actor_SetModeById(gAdventure.currentOwner, 2);
  Actor_SetSpeedParameters_(gAdventure.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(
      gAdventure.currentOwner, (gData_020022f4[step] << 16) >> 31,
      (gData_020022f4[step] << 16) >> 17);
  if (callback != 0)
    callback();
  Actor_WaitForMoveById(gAdventure.currentOwner);
  Actor_SetModeById(gAdventure.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02002198(object);
  Func_020022c8(298);
  Func_020022c8(213);
  Actor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02002160(object, 1);
  target.x = (target.x + (rect[0] << 16)) >> 20;
  target.z = (target.z + (rect[1] << 16)) >> 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  Func_020021b0_(target.x, target.z, width, height, originX + target.x,
                originZ + target.z);
  Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);
  Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (rect[0] << 16)) >> 20;
  cell[2] = (from[2] + (rect[1] << 16)) >> 20;
  Func_020021b0_(originX + cell[0], originZ + cell[2], width, height, cell[0],
                cell[2]);
  Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);
  Func_020022a0();
}
