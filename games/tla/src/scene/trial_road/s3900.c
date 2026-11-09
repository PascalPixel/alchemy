#include "trial_road.h"
#include "text/msg_ids.h"
#include "event_inline.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

extern const u8 gData_02005c88[];
extern const u8 gData_02005be8[];
extern const u8 gData_02005d28[];
extern const u8 gData_02005dc8[];
extern const u8 gData_02005e40[];
extern const u8 gData_02005ed4[];

void Event_StartScreenTransition(void);
void Func_02005880(void);
void Func_02005980(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 actor, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(actor, dx, dz);
}

void Func_02003900(void) {
  s32 i;

  Func_02005880();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, PIXELS(1016));
  Actor_SetPosition(8, PIXELS(1016), PIXELS(1016));
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gData_02005c88);
  Actor_SetMovementTypeById(14, gData_02005be8);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(184 << 16, -1, 600 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 552 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 408 << 16, 1);
  Func_02005980();
  FieldCamera_MoveTo(120 << 16, -1, 120 << 16, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  Event_ShowMessageAndWait(8, 0);
  Func_02005980();
  FieldCamera_MoveTo(216 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(4, 0);
  Actor_SetMovementTypeById(13, gData_02005d28);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 8);
  Event_Wait(1);
  /* FAKEMATCH: the bit is set through the 8-bit field, as in overlay 685: read
   * as a plain byte the store takes the order of a store that aliases what
   * follows. */
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(20);
  Actor_SetSpeed(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_OffsetPositionAndResetMotion_(13, 0, -40);
  Actor_WaitForMoveById(13);
  Sound_Play(103);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  MapGrid_SetShake(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate(13, 0, 16);
  Event_Wait(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  Event_Wait(60);
  Actor_ArmCallback(13, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, -16, -16);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Sound_Play(124);
  Actor_SetModeById(9, 6);
  Event_Wait(30);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetPositionAndReset(13, 200, 168);
  Actor_FaceDirection(13, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(9, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(13, gData_02005dc8);
  Actor_RefreshSelectorById(13);
  Sound_Play(123);
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005e40);
  FieldCamera_MoveTo(328 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Event_ShowMessageAndWait(8, 0);
  Actor_FaceDirection(14, FACING_NORTHEAST, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_Wait(10);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Actor_WalkByAndWait(14, 16, -16);
  Actor_ArmCallback(14, 0, 0);
  Sound_Play(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(14, 350, 168);
  Actor_FaceDirection(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(11, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(14, 364, 168);
  Actor_ArmCallback(14, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(125);
  Actor_SetModeById(12, 5);
  Event_Wait(20);
  Sound_Play(138);
  Func_0802b450(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_ArmCallback(14, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Event_ShowMessageAndWait(8, 0);
  Camera_MoveWithinMapBounds(408 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Event_ShowMessageAndWait(8, 0);
  Actor_SetMovementTypeById(14, gData_02005ed4);
  Actor_RefreshSelectorById(14);
  Event_ShowMessageAndWait(8, 0);
  Import_EventSetExitRequest(10);
}
