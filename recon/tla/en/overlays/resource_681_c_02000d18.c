// Draft, differ 47 of 188, skeleton gap 3/4 (tla-en, 2026-11-01)
// Once any of twelve cells is filled (flag 0x9c4 still clear), person 15 walks
// off, the doorway cells open, four boulders are placed and the flag is set.
// Everything outside the cell loop now matches: Func_02004ff4 takes seven
// arguments, the second boulder is at 175 << 18 and Func_02004f2c returns s32 (a
// guess at the type that moves its argument load). The set-once locals are the
// shape of the map-copy calls (source x, y, size, destination x, y), inferred, not
// confirmed from the game.
// Left, one cause: the loop and the frame (16 bytes, native 12).
// - Native loads the table inside the loop, and the index i * 4 + 2 is reduced
//   while i * 4 is rebuilt. loop.c hoists the load here (threshold 15 * life 7
//   against 24 insns), so both addresses become givs of table + i * 4, the table
//   lands in r4 and is saved around the call at sp+12.
// - Keeping the load in the loop gives native's shape: a local set twice in the
//   loop did it as a probe, but that is a reused local, so it is not kept.
// Tried, no change: flat s16[24], i + i, i << 1, s32 j = i, x and z locals, an
// inner or outer table local, a non-const table. Struct fields .x and .z are worse
// (79), because the +2 goes outside the index; s32 i loses the 16-bit steps. A
// `(u32 *)` pointer to currentOwner folds its offset into the pool and is worse.
// call.h closes the call constants too, but it is a tagged wrapper, so not used.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"

struct CellPoint {
  s16 position[2];
};

extern const struct CellPoint gData_020051d8[12];
extern const u8 gData_020051b8[];
extern const u8 gData_02005118[];
extern const u8 gData_02005168[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_SetMovementTypeById(u32 objectId, const u8 *script);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02004dec(s32 layer, s32 x, s32 z);
void Func_02004e5c(void);
void Func_02004fcc(s32 mode);
s32 Func_02004f2c(s32 id, s32 animation, s32 frames);
void Func_02004e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005014(s32 cue);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Func_02004fdc(void);
s32 Func_02004f6c(s32 id);

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02000d18(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;
  s32 height = 8;
  s32 sourceX = 117;
  s32 sourceY = 20;

  Func_02004ff4(104, sourceY, 1, height, height, sourceX, sourceY);
  if (GameFlag_Test(0x9c4))
    return;
  count = 0;
  for (i = 0; i <= 11; i++) {
    if (Func_02004dec(2, gData_020051d8[i].position[0] << 20,
                      gData_020051d8[i].position[1] << 20) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_GetById(15);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02004e5c();
  Func_02004fcc(0);
  if (leader->z >> 19 > 53 && leader->x >> 19 > 83 && leader->x >> 19 <= 91) {
    leader->speedLimit = 0x20000;
    Func_02004f2c(gMainRoutineWork.currentOwner, 0x100, 0);
    Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                            gData_020051b8);
  }
  Actor_SetMovementTypeById(15, gData_02005118);
  FieldEvent_Wait(60);
  Actor_SetMovementTypeById(15, gData_02005168);
  FieldEvent_Wait(30);
  Func_02004e04(117, 20, 7, 8, 104, 20);
  Func_02004e04(111, 12, 1, 6, 111, 20);
  {
    s32 destX = 40;
    s32 destY = 84;
    Func_02004dfc(49, destY, 8, 8, destX, destY);
  }
  FieldEvent_Wait(30);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1ea0000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  Func_02005014(204);
  guard->motionFlags = 3;
  ((struct BoulderSprite *)guard->animation)->layer = 3;
  FieldEvent_Wait(10);
  Func_02004ebc(15, 0, 0);
  Func_02004fdc();
  GameFlag_SetBit(0x9c4);
  Func_02004f6c(9);
}
