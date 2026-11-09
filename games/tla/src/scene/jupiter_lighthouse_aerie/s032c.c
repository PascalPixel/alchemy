// Jupiter Lighthouse aerie's two pushed objects: object 13 on cell (6, 12) or
// object 14 on cell (57, 12) sets its flag (2570, 2571), falls into place
// (waited on for at most 30 frames), copies cells (Func_0802b450,
// Map_CopyCellAttributeRect) with sounds 134 and 167 and runs its part
// (Func_020001b4, Func_02000264); off its cell it only marks the cell it stands
// on (Func_02002a78). With both flags set, flag 2591 is set. The fall wait is
// an if around a do-while.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

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
      object->motionFlags = 0;
      Actor_Stop(gMainRoutineWork.currentOwner);
      Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      Sound_Play(134);
      Func_0802b450(66, 39, 1, 1, 70, 10);
      Func_0802b450(5, 75, 1, 1, 6, 75);
      Map_CopyCellAttributeRect(5, 12, 1, 1, 6, 12);
      Event_Wait(30);
      Sound_Play(167);
      Func_0802b450(70, 39, 1, 1, 71, 10);
      Event_Wait(20);
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
      object->motionFlags = 0;
      Actor_Stop(gMainRoutineWork.currentOwner);
      Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      Sound_Play(134);
      Func_0802b450(68, 39, 1, 1, 121, 10);
      Func_0802b450(58, 75, 1, 1, 57, 75);
      Map_CopyCellAttributeRect(58, 12, 1, 1, 57, 12);
      Event_Wait(30);
      Sound_Play(167);
      Func_0802b450(72, 39, 1, 1, 120, 10);
      Event_Wait(20);
      Func_02000264();
    } else {
      Func_02002a78(object->x, object->z, 0, 6);
    }
  }
  if (GameFlag_Test(2570) && GameFlag_Test(2571))
    GameFlag_SetBit(2591);
}
