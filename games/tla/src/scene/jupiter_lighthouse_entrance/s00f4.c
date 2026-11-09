#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Func_02001b1c();
s32 GameFlag_Test();
s32 Actor_SetHorizontalPositionWithTerrain();
s32 Func_02001a7c();
s32 Event_ResolvePendingActions();
s32 Func_080d489c();
s32 Actor_OffsetPositionAndResetMotion();
s32 Actor_WaitForMoveById();
s32 Func_02001a84();
extern u8 gMainRoutineWork[];

void Func_020000f4(void) {
  s32 t0;
  s32 t2;

  t0 = Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532));
  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  t2 = GameFlag_Test(0xfe2);
  if (t2 == 0) {
    Call3(Actor_SetHorizontalPositionWithTerrain, 0x40, 0x1180000, 0x1b80000);
    if ((FIELD_AT(t0, s32, 8) >> 20) == 0x11) {
      if ((FIELD_AT(t0, s32, 16) >> 20) == 0x1b) {
        Func_02001a7c();
        Event_ResolvePendingActions(0);
        Call2(Func_080d489c, FIELD_AT(gMainRoutineWork, s32, 532), 0x102);
        Call3(Actor_OffsetPositionAndResetMotion,
              FIELD_AT(gMainRoutineWork, s32, 532), -16, 0);
        FIELD_AT(t0, s32, 40) = 0x60000;
        Actor_WaitForMoveById(FIELD_AT(gMainRoutineWork, s32, 532));
        Func_02001a84();
      }
    }
  }
}
