#include "jupiter_lighthouse_room_m.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 MsgIMSorry[];

void Func_020047d4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020048f4(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Party_RemoveActiveOwner(s32 owner);
s32 Party_AddActiveOwner(s32 owner);
void Unit_Calculate(s32 owner);
s32 Unit_AddPp(s32 owner, s32 delta);
void Func_02004904(s32 a, s32 b);

static __inline__ void JupiterLighthouseRoomM_AddHp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from
   * the pool at each call, as the native does; as direct calls, the four
   * uses share it in r5 and 23 of 160 instructions differ. */
  Unit_AddHp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_AddPp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from
   * the pool at each call, as the native does; as direct calls, the four
   * uses share it in r5 and 23 of 160 instructions differ. */
  Unit_AddPp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_Func_020048b4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline builds the second
   * argument around the first, as the native does; as a direct call, 2 of 160
   * instructions differ. */
  Actor_ArmCallbackAndRefresh(actor, a);
}

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 35 of 160
   * instructions differ. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so it is  \
     * built before x and z, as the native does; as a plain argument, 12 of    \
     * 160 instructions differ. */                                             \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

/// Places actors 0, 2, 27 and 26 and shows an apology; then the party is left
/// with owners 2 and 0, whose first values and PP drop by 10000. "I'm sorry"
void Func_02001c7c(void) {
  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  PLACE(0, 776, 202, FACING_WEST);
  PLACE(2, 731, 232, FACING_NORTHWEST);
  PLACE(27, 731, 213, FACING_SOUTH);
  PLACE(26, 737, 202, FACING_EAST);
  Actor_SetAnimation(0, 1);
  Actor_SetAnimation(2, 1);
  Actor_SetAnimation(26, 1);
  Actor_SetAnimation(27, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId(MsgIMSorry);
  FieldEvent_ShowMessageThenWait(2, 0, 5);
  (struct Actor *)Actor_ShowEmote(4, 0x100, 30);
  FieldCamera_MoveTo(PIXELS(705), 0, PIXELS(177), 1);
  Func_020048f4();
  FieldEvent_Wait(30);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  FieldEvent_Wait(10);
  JupiterLighthouseRoomM_Func_020048b4(0, FACING_SOUTHWEST);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  (struct Actor *)Actor_ShowEmote(26, 0x107, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 5);
  Actor_WalkBy(26, 24, 0);
  FieldEvent_Wait(20);
  do {
    /* FAKEMATCH(dead): the store kept in a once-only block, whose end ends
     * CSE's block, so the byte store's dead zero is not kept in r8 for the
     * store of the owner below; without it, 31 of 160 instructions differ. */
    gMainRoutineWork.battleRule24b = 6;
  } while (0);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6a3, 77);
  Event_SetDefeatDestination((s32)&SceneId_Ov6a3, 77);
  Party_RemoveActiveOwner(4);
  Party_RemoveActiveOwner(7);
  Party_RemoveActiveOwner(5);
  Party_RemoveActiveOwner(6);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(0);
  Party_AddActiveOwner(2);
  Party_AddActiveOwner(0);
  Party_RemoveActiveOwner(3);
  Party_RemoveActiveOwner(1);
  JupiterLighthouseRoomM_AddHp(2, -10000);
  JupiterLighthouseRoomM_AddHp(0, -10000);
  Unit_Calculate(0);
  Unit_Calculate(2);
  FieldUnit_AddHp(0, 173);
  JupiterLighthouseRoomM_AddPp(2, -10000);
  Unit_AddPp(0, -10000);
  gMainRoutineWork.currentOwner = 0;
  Func_02004904(102, 4);
}
