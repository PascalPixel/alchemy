// Draft, differ 126 of 342, skeleton gap 56 (tla-en, 2026-11-01)
// The push itself: the leader leans in, the object slides to its end, and its
// footprint moves between the map layers. Takes the six-word block
// Func_020001d4 fills, as yampi_desert_cave.h declares it: words 0 to 2 the
// push's end (16.16 x, y, z), 3 the footprint kind, 4 the object's slot, 5 a
// callback or 0. The calls, arguments, fields, widths and frame (64 bytes, the
// block at sp+96, from and cell at sp+40 and sp+52) follow the native. Imported
// calls go through the scene layer's forwarded wrappers (scene_call.h): that
// gives each speed call its own constants and each stack argument its own r3.
// Left: the do/while below is a stand-in, not the source. The native has no
// back branch, yet behaves as if a loop stood there through both CSE passes:
// from the target call on it rebuilds the owner address into r5 and holds it to
// the second mode call, keeps the zero index unfolded in r9 (set early, shifted
// at its use), and rebuilds kind * 4 + 1 at the end. Only a real loop gives
// that here; it leaves the cmp and beq, and its exit test is false for a
// nonzero step only, so it is wrong behaviour if a step of 0 can occur. Its
// other costs: i folds and its zero is set at the loop's top instead of early;
// cse2 copies the owner address (mov r6, sl) instead of rebuilding it.
// The rest is allocation: object is in r7 where the native has r6, and the
// native stores all four argument words before the heap slot load.
// Measured: no loop at all, 312 (gap 134); a loop on i with i < 1, 134 (gap
// 62), which strength-reduces the table read to ldmia; a separate counter j
// tested j < 1, 108 (gap 62) with i still folded; i = 0 just before the loop,
// which unrolls it and cse2 then merges everything, 301 (gap 129); do/while(0),
// for(;;) with break and an unused label, no loop to the optimizer, 319;
// exit on the callback word, 194 (gap 70); footprint table flat with kind * 4
// kept, with the loop, 163 (gap 103); map read as s32, no change; empty asm
// does not flush CSE. Worth trying: a source shape whose back edge goes only
// after cse2 (combine or the post-reload jump pass).
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

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_SetModeById_(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetModeById(id, mode);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                       s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Actor_WaitForMoveById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_WaitForMoveById(id);
}

static __inline__ void Actor_SetPositionAndResetMotion_(struct Actor *actor,
                                                       s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndResetMotion(actor, x, y, z);
}

static __inline__ void Map_FillCellRectByte2_(s32 layer, s32 x, s32 z, s32 w,
                                             s32 h, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Map_FillCellRectByte2(layer, x, z, w, h, value);
}

static __inline__ void wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  wait(frames);
}

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
  Map_FillCellRectByte2_(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeedParameters_(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById_(gMainRoutineWork.currentOwner, 8);
  wait_(15);
  Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner,
                                     (rect.word[0] - from[0]) >> 17,
                                     (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02000754;
  wait_(4);
  Func_02001828(object, 2);
  Func_02001960(239);
  Actor_SetSpeedParameters_(rect.word[4], 0x8000, 0x1999);
  Func_02001838(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(one-pass): a stand-in loop after the target call, so the code
   * after it starts afresh as the native's does; as plain statements, 312 of
   * 342 instructions differ in tla-en. */
  do {
  Actor_WaitForMoveById_(gMainRoutineWork.currentOwner);
  Actor_SetModeById_(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters_(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  offset = gData_0200198c[i] << 16;
  Actor_OffsetPositionAndResetMotion_(gMainRoutineWork.currentOwner,
                                     offset >> 31, offset >> 17);
  if (rect.word[5] != 0)
    ((void (*)(void))rect.word[5])();
  Actor_WaitForMoveById_(gMainRoutineWork.currentOwner);
  Actor_SetModeById_(gMainRoutineWork.currentOwner, 1);
  } while (offset == 0);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_02001840(object);
  Func_02001960(298);
  Func_02001960(213);
  Actor_SetPositionAndResetMotion_(object, rect.word[0], object->y,
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
  Map_FillCellRectByte2_(0, rect.word[0], rect.word[2], w, h, 255);
  Map_FillCellRectByte2_(2, rect.word[0], rect.word[2], w, h, 255);
  cell[0] = (from[0] + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02001974[kind][1] << 16)) >> 20;
  YampiDesertCave_Call1848(ax + cell[0], az + cell[2], w, h, cell[0], cell[2]);
  YampiDesertCave_Call1848(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                           cell[2] + bz);
  Func_02001938();
}
