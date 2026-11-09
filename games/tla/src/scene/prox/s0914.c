#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02004464();
s32 Func_0200446c();
extern u8 MsgProx47[];

void Func_02000914(void) {
  s32 t0;
  s32 t8;

  t0 = Event_GetFlag(0xa3d);
  if (t0 != 0) {
  } else {
    Func_02004464();
    Event_ResolvePendingActions(0);
    Sound_Play(158);
    MapGrid_CopyCellsTo(0x59, 0, 0x4d, 0x10, 1, 2);
    GameFlag_SetBit(0x200);
    Event_Wait(10);
    {
      s32 y = 0xc0;
      Actor_SetPositionAndFacing(8, PIXELS(520), PIXELS(360), y << 6);
    }
    t8 = ((s32)Func_080d4568());
    FIELD_AT((t8 + 0x55), u8, 0) = t0;
    Camera_SetSpeedParameters(0x19999, 0x3333);
    Camera_MoveWithinMapBounds(PIXELS(552), -1, PIXELS(388), 1);
    Event_Wait(10);
    Actor_SetSpeed(8, 0x13333, 0x9999);
    Actor_WalkToAndWait(8, 526, 384);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_WalkToAndWait(8, 540, 388);
    Actor_FaceDirectionAndWait(8, 0);
    Actor_SetModeById(8, 4);
    Event_SetMessageId((s32)MsgProx47);
    Event_ShowMessage(0x4008, 0);
    Func_080d47b4(9, 0x101, 0);
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(8, 3);
    Event_ShowMessage(0x4008, 0);
    Func_080d47b4(9, 0x100, 20);
    Actor_FaceDirection(9, 0xb000, 0);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(8, 0xb000, 40);
    Actor_FaceDirectionAndWait(8, 0);
    Actor_SetAnimation(8, 4);
    Event_ShowMessage(0x4008, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Func_080d47b4(8, 0x109, 20);
    Event_ShowMessage(0x4008, 0);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirectionAndWait(8, 0xb000);
    GameFlag_SetBit(0x9c0);
    Func_0200446c();
  }
}
