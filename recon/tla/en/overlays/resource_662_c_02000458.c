// Draft, differ 251 of 342 (tla-en, 2026-10-31): the push itself: the leader
// leans in, the object slides to its end, and its footprint moves between
// the map layers. Takes the six-word block Func_020001d4 fills, as
// yampi_desert_cave.h declares it: words 0 to 2 the push's end (16.16 x, y,
// z), 3 the footprint kind, 4 the object's slot, 5 a callback or 0. The
// calls, their arguments, the control flow, every field and width and the
// frame (64 bytes, the block at sp+96, from and cell at sp+40 and sp+52)
// now follow the native. Left: the native's CSE keeps nothing across the
// target call in either pass (it rebuilds the owner address there, keeps
// the zero step in r9 and shifts it, rebuilds kind * 4 + 1 at the end); the
// one-pass block below breaks only the first CSE pass, and cse2 still
// copies the owner address (mov r5, r8), folds the step, keeps kind * 4 + 1
// from the start and turns the step into the later zeros. From that,
// register choice (the native keeps kind * 4 in r11 and spills both sizes;
// here kind * 4 is spilled and the height takes r11), the speed calls
// sharing 0x8000 and 0x1999 in r5 and r6 where the native builds them for
// each call, and the native storing all four argument words before the heap
// slot load, which this compiler hoists between them. Measured: kind read
// from the block each time, 305; the speed calls forwarded inline, 282 (the
// first alone, 241); Func_02001848 direct, 287; the footprint table flat
// with kind * 4 kept, 250; w and h declared after the speeds, 265; ABS as
// if statements, 379 (with the forwarded speed calls); the block before the
// 0x4ccc speed call, 255; before the leader's wait or around the call, the
// same as here; the heap slot read as an s32 through a cast, 243, with the
// four stores first as the native has them.
#include "../../../../games/tla/src/scene/yampi_desert_cave/yampi_desert_cave.h"
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

void Func_02000458(struct YampiDesertCaveRect rect) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(rect.word[4]);
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  s32 from[3];
  s32 i = 0;
  s32 kind = rect.word[3];
  s32 offset;
  s32 ax, az, bx, bz;

  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (rect.word[0] - from[0]) >> 17,
                                     (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02000754;
  wait(4);
  Func_02001828(object, 2);
  Func_02001960(239);
  Actor_SetSpeedParameters(rect.word[4], 0x8000, 0x1999);
  Func_02001838(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(one-pass): a one-pass block after the target call, so CSE
   * starts afresh there as the native's does; as the call alone, 319 of 342
   * instructions differ in tla-en. */
  do {
  } while (0);
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
  YampiDesertCave_Call1848(rect.word[0], rect.word[2], w, h,
                           ax + rect.word[0], az + rect.word[2]);
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
