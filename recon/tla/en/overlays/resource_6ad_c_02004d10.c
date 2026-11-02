// Draft, differ 225 of 305, skeleton gap 77 (tla-en, 2026-11-01)
// One line from differ 0. This is the listing of jupiter_lighthouse_room_8's
// matched Func_02000ec8 (games/tla/src/scene/jupiter_lighthouse_room_8/s0ec8.c),
// whose compiled output is this function instruction for instruction, and the
// text below is that source. Left: the second Actor_SetSpeedParameters, which
// the twin writes as
//   Call3((void (*)())Actor_SetSpeedParameters, move.id, 0x8000, 0x1999);
// With that one line and #include "call.h" the draft measures differ 0, gap 0.
// It stays out because this round adds no device and S2 lists `wrapper`; call.h
// is untagged at about 14,400 sites in games/tla/src, so whether it counts as a
// device at all is Pascal's call (S2, C2). Measured: without it the third call
// reuses the second's 0x1999 (no rebuild there) and the frame slots sit 4 bytes
// off. A reading, not measured: a direct call splits the constants into pseudos
// at expansion, and through the inline they arrive only at integration, too late
// to be shared. That one shared value would explain nearly all of the 225.
// This replaces the old stepIndex recipe; the one-pass loop and the ABS sums are
// gone. r9's zero is direction: angle is a u16, so `>> 16` is always 0. The test
// on it folds away, and without it the owner address and the second call's
// constants are not rebuilt (differ 309 with the rest in place). The test is the
// twin's text, untagged there, and mode 3 appears nowhere in this overlay's
// listing: a reviewer should judge it against S2's `dead`.
// toX and toZ are plain locals; they replace the twin's Call6 and give gap 0.
// No gain over 225 from: no prototype on the callee, a cast call, an id local,
// pushSpeed/pushAcceleration block locals at each of the three calls.
// Overlay 668's Func_020044c4 and 674's Func_02003a5c are the same listing.
//
// Overlay 6ad's copy of resource_668_c_020044c4.c, with this overlay's
// symbols: carries out a push measured as overlay 668's Func_02004240
// measures one (this overlay's Func_02004a8c has the same shape): clears
// the cells under the object's box, walks the leader up to it (mode 8),
// slides the object to the push's end with the leader following (cue 239,
// Func_020054b8), runs the optional callback, restores the leader's speed,
// plays cues 298 and 213, places the object exactly, copies the spare cells
// into its new place and back over its old place (Func_020054e8) and marks
// layer 0 and 2 there. The same listing as overlay 674's Func_02003a5c.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into gData_02005f64, four bytes each
  s32 id;
  void (*callback)(void);
};

struct HeapSlotWords {
  s32 unk00[8];
  s32 map;
};

extern s8 gData_02005f64[];
extern s32 gData_02005f7c[];
struct Actor *Actor_GetById(u32 id);
void Func_02004a44(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02004fc0(void);
void Func_02005480(struct Actor *object, s32 mode);
void Func_020056c0(s32 cue);
void Func_020054b8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020054c0(struct Actor *object);
void Func_020054e8(s32 x, s32 z, s32 width, s32 height, s32 fromX, s32 fromZ);
void Func_02005668(void);

#define ABS(x) ((x) < 0 ? -(x) : (x))

void Func_02004d10(struct PushMove move) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->map;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(move.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  // BUG: angle is 16 bits, so direction is always 0.
  direction = leader->angle >> 16;
  footprint = move.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02005f64[footprint * 4] << 16)) >> 20;
  cell[2] = (object->z + (gData_02005f64[footprint * 4 + 1] << 16)) >> 20;
  width = (ABS(gData_02005f64[footprint * 4]) +
           ABS(gData_02005f64[footprint * 4 + 2])) >>
          4;
  height = (ABS(gData_02005f64[footprint * 4 + 1]) +
            ABS(gData_02005f64[footprint * 4 + 3])) >>
           4;
  Func_02004a44(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  {
    s32 deltaX = (move.x - from[0]) >> 17;
    s32 deltaZ = (move.z - from[2]) >> 17;

    Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02004fc0;
  wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02005480(object, 3);
  else
    Func_02005480(object, 2);
  Func_020056c0(239);
  Actor_SetSpeedParameters(move.id, 0x8000, 0x1999);
  Func_020054b8(object, move.x, move.y, move.z);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     (gData_02005f7c[direction] << 16) >> 31,
                                     (gData_02005f7c[direction] << 16) >> 17);
  if (move.callback != 0)
    move.callback();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Func_020054c0(object);
  Func_020056c0(298);
  Func_020056c0(213);
  Actor_SetPositionAndResetMotion(object, move.x, object->y, move.z);
  Func_02005480(object, 1);
  move.x = move.x + (gData_02005f64[footprint * 4] << 16);
  move.z = move.z + (gData_02005f64[footprint * 4 + 1] << 16);
  move.x >>= 20;
  move.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  {
    s32 toX = originX + move.x;
    s32 toZ = originZ + move.z;

    Func_020054e8(move.x, move.z, width, height, toX, toZ);
  }
  Func_02004a44(0, move.x, move.z, width, height, 255);
  Func_02004a44(2, move.x, move.z, width, height, 255);
  cell[0] = (from[0] + (gData_02005f64[footprint * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02005f64[footprint * 4 + 1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  Func_020054e8(originX, originZ, width, height, cell[0], cell[2]);
  Func_02004a44(2, cell[0], cell[2], width, height, 0);
  Func_02005668();
}
