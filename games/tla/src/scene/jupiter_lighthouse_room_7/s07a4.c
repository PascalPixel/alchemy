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
#include "scene_tables.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02000194(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_020002f0(void);
s32 Func_02000290(void);
void Func_02002b6c(void);
void Func_02002b74(void);
void Func_02002bd4(void);
void Func_02002ad4(struct Actor *object, s32 mode);
void Func_020005e8(s32 value);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c4c(s32 x, s32 z, s32 a, s32 b);

void Func_020007a4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 spot;
  s32 i;

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
    Event_PlaySound(188);
    object->motionFlags = 0;
  }
  if ((u32)(id - 18) <= 1) {
    spot = Func_02000194();
    if (spot == -1)
      return;
    Actor_SetSingleChildFlags(Actor_GetById(id), 0);
    Event_Wait(30);
    Func_020002f0();
    if (spot == 0)
      Event_PlaySound(167);
    if (spot == 1)
      Event_PlaySound(167);
    if (spot == 3 && Event_GetFlag(2576))
      Event_PlaySound(167);
    if (spot == 4 && Event_GetFlag(2577))
      Event_PlaySound(167);
    Actor_Stop(gMainRoutineWork.currentOwner);
    Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
    if (Func_02000290() != 2 || !Event_GetFlag(2582))
      return;
    object = Actor_GetById(17);
    GameFlag_ClearBit(2582);
    Event_Wait(10);
    Func_02002b6c();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds(166 << 18, -1, PIXELS(136), 1);
    Func_02002bd4();
    Event_Wait(20);
    Sound_Play(151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    Func_080dbdd4(object->x, object->z, object->terrainId, 255);
    Event_Wait(30);
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
    Event_Wait(30);
    Sound_Play(167);
    Func_020005e8(1);
    if (!Event_GetFlag(2583))
      return;
    object = Actor_GetById(9);
    GameFlag_ClearBit(2583);
    Event_Wait(20);
    Sound_Play(151);
    Func_02002ad4(object, 3);
    object->priorityFlags &= ~2;
    Func_080dbdd4(object->x, object->z, object->terrainId, 255);
  } else {
    Func_02002b54(object->x, object->z, 0, 8);
    Func_02002c4c(object->x, object->z, 0, 128);
  }
}
