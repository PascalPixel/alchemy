#include "../../../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
#include "map.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct Actor *Actor_GetById(s32 id);
void Func_02000754(void);
void Func_02001828(struct Actor *object, s32 mode);
void Func_02001838(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02001840(struct Actor *object);
void Func_02001938(void);
void Func_02001960(s32 song);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 w, s32 h, s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void wait(s32 frames);

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters_ forwarded inline; as direct
   * calls to Actor_SetSpeedParameters, 61 of 342 instructions differ in tla-en.
   */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  /* FAKEMATCH(wrapper): Actor_OffsetPositionAndResetMotion_ forwarded inline;
   * as direct calls to Actor_OffsetPositionAndResetMotion, 62 of 342
   * instructions differ in tla-en. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

struct HeapView {
  s32 pad[8];
  s32 word;
};

void Func_02000458(struct YampiDesertCaveRect rect) {
  // Read as s32: the load then keeps its place after the argument stores.
  struct MapWork *map =
      (struct MapWork *)((struct HeapView *)Ram_HeapSlots)->word;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(rect.word[4]);
  // Always 0: angle is a u16. Direction 0 of the table of steps.
  s32 i = leader->angle >> 16;
  s32 kind = rect.word[3];
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  s32 from[3];
  s32 j;
  s32 offset;
  s32 ax, az, bx, bz;

  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeedParameters_(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner,
                                      (rect.word[0] - from[0]) >> 17,
                                      (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02000754;
  wait(4);
  Func_02001828(object, 2);
  Func_02001960(239);
  Actor_SetSpeedParameters(rect.word[4], 0x8000, 0x1999);
  Func_02001838(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(one-pass): a loop of one pass with an untaken two-test branch, so
   * the second half starts afresh in cse; as plain statements, 277 of 342
   * instructions differ in tla-en. */
  for (j = 0; j < 1; j++) {
    if (j != 0 && j != 5)
      w = 1;
  }
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  offset = gData_0200198c[i] << 16;
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     offset >> 31, offset >> 17);
  if (rect.word[5] != 0)
    ((void (*)(void))rect.word[5])();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02001840(object);
  Func_02001960(298);
  Func_02001960(213);
  Actor_SetPositionAndResetMotion(object, rect.word[0], object->y,
                                  rect.word[2]);
  Func_02001828(object, 1);
  rect.word[0] = (rect.word[0] + (gData_02001974[kind][0] << 16)) >> 20;
  rect.word[2] = (rect.word[2] + (gData_02001974[kind][1] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0] >> 20;
  bz = *(s32 *)&map->layers[2].unk08[4] >> 20;
  YampiDesertCave_Call1848(rect.word[0], rect.word[2], w, h, ax + rect.word[0],
                           az + rect.word[2]);
  YampiDesertCave_Call1848(rect.word[0] + bx, rect.word[2] + bz, w, h,
                           rect.word[0] + 64, rect.word[2] + 64);
  Map_FillCellRectByte2(0, rect.word[0], rect.word[2], w, h, 255);
  Map_FillCellRectByte2(2, rect.word[0], rect.word[2], w, h, 255);
  cell[0] = (from[0] + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02001974[kind][1] << 16)) >> 20;
  YampiDesertCave_Call1848(ax + cell[0], az + cell[2], w, h, cell[0], cell[2]);
  YampiDesertCave_Call1848(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                           cell[2] + bz);
  Func_02001938();
}
