// Jupiter Lighthouse room 7's push handler for objects 18 to 20: an object not
// on the ground falls into place (sound 188). Object 18 or 19 then checks its
// spot (Func_02000194), resets the room, plays sound 167 for the spots that
// count, and when all is set (Func_02000290 returns 2 with flag 2582) clears
// that flag and shows object 17 changing (sound 151) from the camera at (664,
// 136). Object 20 on cell (8, 12) sets flag 2586, plays sound 167 and with flag
// 2583 shows object 9 changing; off that cell it marks the cell it stands on.
// The landing wait is the rotated loop (an if around a do-while), and the flag
// and sound calls go through the call layer, each as the native has it.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test();
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002c5c();
s32 Func_02000194(void);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020002f0(void);
void Actor_Stop(s32 id);
void Func_02002ba4(s32 id, s32 a, s32 b);
s32 Func_02000290(void);
void Func_02002b6c(void);
void Func_02002b74(void);
void Func_02002c0c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02002bd4(void);
void Func_02002ad4(struct Actor *object, s32 mode);
void Func_02002c3c(s32 x, s32 z, s32 layer, s32 value);
void Func_020005e8(s32 value);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c4c(s32 x, s32 z, s32 a, s32 b);

void Func_020007a4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 spot;
  s32 i;
  s32 y;
  s32 height;

  if (object->terrainHeight !=
      Map_GetTerrainHeight(object->terrainId, object->x, object->z)) {
    object->motionFlags = 3;
    wait(2);
    i = 0;
    if (object->y > object->terrainHeight || object->velocityY != 0) {
      do {
        wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y > object->terrainHeight || object->velocityY != 0);
    }
    Call1(Func_02002c5c, 188);
    object->motionFlags = 0;
  }
  if ((u32)(id - 18) <= 1) {
    spot = Func_02000194();
    if (spot == -1)
      return;
    Actor_SetSingleChildFlags(Actor_GetById(id), 0);
    FieldEvent_Wait(30);
    Func_020002f0();
    if (spot == 0)
      Call1(Func_02002c5c, 167);
    if (spot == 1)
      Call1(Func_02002c5c, 167);
    if (spot == 3 && Value1(GameFlag_Test, 2576))
      Call1(Func_02002c5c, 167);
    if (spot == 4 && Value1(GameFlag_Test, 2577))
      Call1(Func_02002c5c, 167);
    Actor_Stop(gMainRoutineWork.currentOwner);
    Func_02002ba4(id, 0, 0);
    if (Func_02000290() != 2 || !Value1(GameFlag_Test, 2582))
      return;
    object = Actor_GetById(17);
    GameFlag_ClearBit(2582);
    FieldEvent_Wait(10);
    Func_02002b6c();
    Func_02002c0c(0);
    Camera_MoveWithinMapBounds(166 << 18, -1, PIXELS(136), 1);
    Func_02002bd4();
    FieldEvent_Wait(20);
    Call1(Func_02002c5c, 151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    Func_02002c3c(object->x, object->z, object->terrainId, 255);
    FieldEvent_Wait(30);
    Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
    Func_02002bd4();
    Func_02002b74();
    return;
  }
  if (id != 20)
    return;
  /* FAKEMATCH(volatile): z read for the test as volatile, so the failing test
   * reloads it with the other; as object->z, 6 of 230 instructions differ in
   * tla-en. */
  if (object->x >> 20 == 8 && *(volatile s32 *)&object->z >> 20 == 12) {
    GameFlag_SetBit(2586);
    Actor_SetSingleChildFlags(Actor_GetById(20), 0);
    FieldEvent_Wait(30);
    Call1(Func_02002c5c, 167);
    Func_020005e8(1);
    if (!Value1(GameFlag_Test, 2583))
      return;
    object = Actor_GetById(9);
    GameFlag_ClearBit(2583);
    FieldEvent_Wait(20);
    Call1(Func_02002c5c, 151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    Func_02002c3c(object->x, object->z, object->terrainId, 255);
  } else {
    Func_02002b54(object->x, object->z, 0, 8);
    Func_02002c4c(object->x, object->z, 0, 128);
  }
}
