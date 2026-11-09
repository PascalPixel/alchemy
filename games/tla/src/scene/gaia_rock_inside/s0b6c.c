#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Event_Wait(s32 frames);
s32 GetMapCellCollision(s32 a, s32 x, s32 z);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 destX, s32 destY);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_080d489c(s32 id, s32 emote);
void Import_EventSetExitRequest(s32 value);
void Event_ResolvePendingActions(s32 value);
void Func_02004fdc(void);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

/// Unless GetMapCellCollision(2, ...) holds, copies map cell (36, 23) to (38,
/// 25), moves a leader standing on that cell aside, and sets flag 0x30a, or
/// 0x9bf once flag 0x927 is set.
void Func_02000b6c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Call7(Func_02004ff4, 37, 26, 0, 3, 3, 61, 0);
  if (Value3(GetMapCellCollision, 2, 152 << 18, 200 << 17) == 0) {
    Call6(Map_CopyCellAttributeRect, 36, 23, 1, 1, 38, 25);
    Func_02004e5c();
    Event_ResolvePendingActions(0);
    if (leader->x >> 20 == 38 && leader->z >> 20 == 25) {
      Func_080d489c(gMainRoutineWork.currentOwner, 258);
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16, 0);
      leader->velocityY = PIXELS(6);
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    }
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x30a);
      Event_Wait(30);
      Func_02004fdc();
      Import_EventSetExitRequest(20);
    } else {
      GameFlag_SetBit(0x9bf);
      Func_02004e64();
    }
  }
}
