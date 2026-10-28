// Jupiter Lighthouse aerie's two pushed objects: object 13 on cell (6, 12) or
// object 14 on cell (57, 12) sets its flag (2570, 2571), falls into place
// (waited on for at most 30 frames), copies cells (Func_02002a50,
// Func_02002a48) with sounds 134 and 167 and runs its part (Func_020001b4,
// Func_02000264); off its cell it only marks the cell it stands on
// (Func_02002a78). With both flags set, flag 2591 is set. The fall wait is an
// if around a do-while.
#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Actor_Stop(s32 id);
void Func_02002ab8(s32 id, s32 a, s32 b);
void Func_02002b68(s32 sound);
void Func_02002a50(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void Func_02002a48(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_020001b4(void);
void Func_02000264(void);

void Func_0200032c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  if (id == 13) {
    /* FAKEMATCH(volatile): z read for the test as volatile, so the failing test
     * reloads it with the other; as object->z, 6 of 222 instructions differ in
     * tla-en. */
    if (object->x >> 20 == 6 && *(volatile s32 *)&object->z >> 20 == 12) {
      GameFlag_SetBit(2570);
      object->motionFlags = 3;
      WaitFrames(2);
      i = 0;
      if (object->y > object->terrainHeight || object->velocityY != 0) {
        do {
          WaitFrames(1);
          i++;
          if (i > 29)
            break;
        } while (object->y > object->terrainHeight || object->velocityY != 0);
      }
      object->motionFlags = 0;
      Actor_Stop(gAdventure.currentOwner);
      Func_02002ab8(id, 0, 0);
      Func_02002b68(134);
      Func_02002a50(66, 39, 1, 1, 70, 10);
      Func_02002a50(5, 75, 1, 1, 6, 75);
      Func_02002a48(5, 12, 1, 1, 6, 12);
      FieldEvent_WaitFrames(30);
      Func_02002b68(167);
      Func_02002a50(70, 39, 1, 1, 71, 10);
      FieldEvent_WaitFrames(20);
      Func_020001b4();
    } else {
      Func_02002a78(object->x, object->z, 0, 6);
    }
  }
  if (id == 14) {
    /* FAKEMATCH(volatile): z read for the test as volatile, so the failing test
     * reloads it with the other; as object->z, 6 of 222 instructions differ in
     * tla-en. */
    if (object->x >> 20 == 57 && *(volatile s32 *)&object->z >> 20 == 12) {
      GameFlag_SetBit(2571);
      object->motionFlags = 3;
      WaitFrames(2);
      i = 0;
      if (object->y > object->terrainHeight || object->velocityY != 0) {
        do {
          WaitFrames(1);
          i++;
          if (i > 29)
            break;
        } while (object->y > object->terrainHeight || object->velocityY != 0);
      }
      object->motionFlags = 0;
      Actor_Stop(gAdventure.currentOwner);
      Func_02002ab8(id, 0, 0);
      Func_02002b68(134);
      Func_02002a50(68, 39, 1, 1, 121, 10);
      Func_02002a50(58, 75, 1, 1, 57, 75);
      Func_02002a48(58, 12, 1, 1, 57, 12);
      FieldEvent_WaitFrames(30);
      Func_02002b68(167);
      Func_02002a50(72, 39, 1, 1, 120, 10);
      FieldEvent_WaitFrames(20);
      Func_02000264();
    } else {
      Func_02002a78(object->x, object->z, 0, 6);
    }
  }
  if (GameFlag_Test(2570) && GameFlag_Test(2571))
    GameFlag_SetBit(2591);
}
