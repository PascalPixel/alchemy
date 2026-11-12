/// @file
/// Scene code of overlay 0x6a3, in ROM order, from s1c7c's offset up to the next object the overlay links.

#include "jupiter_lighthouse_room_m.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "types.h"
#include "version.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "field_event.h"

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
   * the pool at each call, as the native does; as direct calls, 23 of 160
   * instructions of Func_02001c7c differ in tla-en, the four uses sharing it
   * in r5. */
  Unit_AddHp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_AddPp(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): forwarding through the inline reloads -10000 from
   * the pool at each call, as the native does; as direct calls, 23 of 160
   * instructions of Func_02001c7c differ in tla-en, the four uses sharing it
   * in r5. */
  Unit_AddPp(owner, delta);
}

static __inline__ void JupiterLighthouseRoomM_Func_020048b4(s32 actor, s32 a) {
  /* FAKEMATCH(wrapper): forwarding through the inline builds the second
   * argument around the first, as the native does; as a direct call, 2 of 160
   * instructions of Func_02001c7c differ in tla-en. */
  Actor_ArmCallbackAndRefresh(actor, a);
}

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as ☀️'s scene layer does; as direct calls, 35 of 160
   * instructions of Func_02001c7c differ in tla-en. */
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so it is  \
     * built before x and z, as the native does; as a plain argument, 12 of    \
     * 160 instructions of Func_02001c7c differ in tla-en. */                  \
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
     * store of the owner below; without it, 31 of 160 instructions differ
     * in tla-en. */
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

extern u32 gData_02005548[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

#define PLACE(id, x, z, angle)                                                 \
  {                                                                            \
    /* FAKEMATCH(temporary): the angle passes through a block local, so the    \
     * first two angles are shared in r5 and the later ones built before z,    \
     * as the native does; as a plain argument, 24 of 53 instructions differ   \
     * in tla-en (as one function-wide angle local, 10 to 16). */              \
    s32 angle_ = (angle);                                                      \
    ObjPlace(id, (x) << 16, (z) << 16, angle_);                                \
  }

void Func_02001e38(void) {
  PLACE(0, 761, 219, FACING_NORTHWEST);
  PLACE(2, 741, 232, FACING_NORTHWEST);
  Actor_SetSingleChildFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 39);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 32);
  PLACE(5, 761, 203, FACING_SOUTH);
  PLACE(6, 725, 232, FACING_EAST);
  PLACE(28, 729, 203, FACING_SOUTHEAST);
  Actor_SetScript(3, gData_02005548);
}

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

extern const u8 MsgWeDidItAgatio[];
extern u32 gData_02004a6c[];
extern u32 gData_02004a90[];
extern u32 gData_02004ab4[];
extern u32 gData_02004ae8[];
extern u32 gData_02004b1c[];
extern u32 gData_02005548[];
extern u32 gData_020059a4[];
extern u32 gData_02005a00[];

void Func_020047d4(void);
void Party_RemoveActiveOwner(s32 owner);
s32 Party_AddActiveOwner(s32 owner);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020048f4(void);
void PartyInventory_Remove(s32 a);
s32 PartyInventory_Add(s32 item);
void *Unit_Get(s32 owner);
s32 Func_020047a4();
s32 Event_ClearInvalidPackedValues();
void Func_020047dc(void);

#define ACTOR_SET_MODE_AND_WAIT(id, mode, frames)                              \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                                               \
    FieldEvent_Wait(frames);                                                        \
  } while (0)

// The Japanese form differs and is still on its listing.

static __inline__ void Walk(s32 id, s32 speed, s32 frames) {
  /* FAKEMATCH(wrapper): the walk forwarded inline, so its constants stay out of
   * CSE; as direct calls, 103 of 1328 instructions differ in tla-en. */
  ((struct Actor *)Func_080d47b4(id, speed, frames));
}

void Func_02001ed0(void) {
  s32 index;
  s32 offset;
  s32 answer;

  Func_020047d4();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(0x2e10000, 0, 0xd10000, 0);
  gMainRoutineWork.currentOwner = 4;
  Party_RemoveActiveOwner(0);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(5);
  Party_RemoveActiveOwner(6);
  Party_RemoveActiveOwner(7);
  Party_RemoveActiveOwner(3);
  Party_RemoveActiveOwner(1);
  Party_AddActiveOwner(4);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(4, 0x2980000, 0x580000, angle);
  }
  {
    s32 angle = 0x2000;

    Actor_SetPositionAndFacing(0x1b, 0x2cd0000, 0xd00000, angle);
  }
  Actor_SetPositionAndFacing(0x1a, 0x2e10000, 0xc30000, 0x2000);
  {
    s32 angle = 0xa000;

    Actor_SetPositionAndFacing(0, 0x2f90000, 0xdb0000, angle);
  }
  Actor_SetPositionAndFacing(2, 0x2e50000, 0xe80000, 0xa000);
  Actor_SetScript(3, (s32)gData_02005548);
  Actor_SetSingleChildFlags(Actor_Get(0), 0);
  Actor_SetAnimation(0, 0x27);
  Actor_SetSingleChildFlags(Actor_Get(2), 0);
  Actor_SetAnimation(2, 0x20);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetMessageId((s32)MsgWeDidItAgatio);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAnimation(0x1a, 8);
  FieldEvent_Wait(0x28);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  FieldActor_StartRepeatedMotion(0, 2);
  Actor_RunRepeatedMotion(2, 2);
  FieldEvent_Wait(5);
  Walk(0x1a, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0xe000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetAnimation(0x1a, 1);
  FieldEvent_Wait(0x1e);
  Actor_FaceDirectionAndWait(0x1a, 0x5000);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_FollowActor(4, 1);
  FieldEvent_Wait(0x14);
  FieldActor_LaunchFromLeader(6, 0, 0, 0x4000);
  FieldCamera_FollowActor(6, 1);
  FieldActor_LaunchFromLeader(7, 0x10, 0, 0x4000);
  FieldActor_LaunchFromLeader(0x1c, -16, 0x18, 0x2000);
  FieldActor_LaunchFromLeader(5, -16, 0, 0x2000);
  Actor_WalkByAndWait(4, 0x10, 0x18);
  Actor_FaceDirection(4, 0x4000, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x100, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  Actor_FaceDirectionAndWait(0x1c, 0);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(4, 0x8000, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0x23);
  Actor_SetAnimation(4, 3);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_WalkTo(4, 0x2ab, 0xba);
  Actor_WalkTo(0x1c, 0x29b, 0xca);
  Actor_SetScript(4, (s32)gData_02004a6c);
  Actor_SetScript(0x1c, (s32)gData_02004a90);
  Actor_FaceDirection(0x1a, 0x8000, 0);
  Actor_FaceDirection(0x1b, 0x8000, 0);
  Walk(6, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceActor(5, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(7, 5, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  ACTOR_SET_MODE_AND_WAIT(7, 3, 0x14);
  FieldEvent_Wait(0x14);
  Actor_WalkBy(7, 0, 0x28);
  Actor_WalkBy(6, 0, 0x28);
  Actor_WalkBy(5, 0, 0x28);
  Actor_SetScript(7, (s32)gData_02004ab4);
  Actor_SetScript(6, (s32)gData_02004ae8);
  Actor_SetScript(5, (s32)gData_02004b1c);
  FieldCamera_FollowActor(4, 1);
  Func_020048f4();
  FieldEvent_Wait(0x14);
  Walk(0x1a, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceDirectionAndWait(0x1a, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x4000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0xc000);
  FieldEvent_Wait(0xa);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_RunRepeatedMotion(0x1c, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x103, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(6, 4, 0x21);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1b, 0x107, 0x1e));
  Event_OpenMessage(0x1b, 0);
  Actor_FaceActor(0x1c, 4, 0);
  Actor_FaceActor(5, 4, 0);
  Actor_FaceActor(6, 4, 0);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(0xa);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ((struct Actor *)Actor_ShowEmote(0x1b, 0x102, 0x1e));
    FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ((struct Actor *)Actor_ShowEmote(0x1b, 0x102, 0x1e));
    EVENT_WORK->messageId++;
    FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  }
  Actor_FaceActor(7, 0x1a, 0);
  ((struct Actor *)Actor_ShowEmote(7, 0x107, 0x1e));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(5, 4, 0x21);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceEachOther(0x1a, 0x1b, 0);
  FieldActor_RefreshSelectorById(0x1a);
  FieldActor_RefreshSelectorById(0x1b);
  Walk(0x1a, 0x105, 0);
  ((struct Actor *)Actor_ShowEmote(0x1b, 0x105, 0x1e));
  Actor_FaceActor(0x1c, 0x1b, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 4, 0x21);
  Actor_FaceActor(0x1a, 0x1c, 0);
  Actor_FaceActor(0x1b, 0x1c, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  FieldEvent_Wait(0x3c);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_RunRepeatedMotion(0x1a, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 4, 0x21);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0x1a, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Walk(0x1b, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  FieldEvent_Wait(0x14);
  Actor_RunRepeatedMotion(0x1a, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1a, 0x8000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceDirectionAndWait(0x1b, 0xa000);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceActor(7, 4, 0);
  Actor_FaceEachOther(6, 5, 0);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldActor_RefreshSelectorById(0x1c);
  Walk(4, 0x108, 0);
  Walk(6, 0x108, 0);
  Walk(5, 0x108, 0);
  Walk(7, 0x108, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x108, 0x28));
  Actor_FaceActor(4, 0x1a, 0);
  Actor_FaceActor(5, 0x1a, 0);
  Actor_FaceActor(6, 0x1a, 0);
  Actor_FaceActor(7, 0x1a, 0);
  Actor_FaceActor(0x1c, 0x1a, 0);
  ACTOR_SET_MODE_AND_WAIT(0x1a, 3, 0x14);
  FieldEvent_Wait(0xa);
  Event_OpenMessage(0x1a, 0);
  answer = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
    EVENT_WORK->messageId++;
  } else {
    ACTOR_SET_MODE_AND_WAIT(0x1a, 4, 0x21);
    EVENT_WORK->messageId++;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  }
  Actor_FaceEachOther(0x1c, 4, 0);
  FieldActor_RefreshSelectorById(0x1c);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  Actor_FaceEachOther(4, 0x1c, 0);
  FieldActor_RefreshSelectorById(4);
  FieldEvent_Wait(0xa);
  ((struct Actor *)Actor_ShowEmote(4, 0x102, 0x1e));
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0x28);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0x1e);
  Actor_FaceEachOther(0x1a, 0x1b, 0);
  FieldActor_RefreshSelectorById(0x1a);
  FieldActor_RefreshSelectorById(0x1b);
  Walk(0x1b, 0x108, 0);
  Walk(0x1a, 0x108, 0x1e);
  Actor_FaceActor(0x1a, 4, 0);
  Actor_FaceActor(4, 0x1b, 0);
  Actor_WalkToAndWait(0x1b, 0x2bb, 0xca);
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Walk(0, 0x100, 0);
  Walk(4, 0x100, 0);
  Walk(2, 0x100, 0);
  Walk(5, 0x100, 0);
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x100, 0x1e));
  ((struct Actor *)Actor_ShowEmote(0x1c, 0x109, 0x1e));
  Actor_FaceDirection(0x1c, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x108, 0x1e));
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_FaceEachOther(4, 7, 0);
  Actor_FaceEachOther(6, 5, 0);
  FieldActor_RefreshSelectorById(6);
  FieldActor_RefreshSelectorById(5);
  FieldEvent_Wait(0x28);
  Actor_FaceActor(4, 0x1a, 0);
  Actor_FaceActor(7, 0x1a, 0);
  Actor_FaceActor(6, 0x1a, 0);
  Actor_FaceActor(5, 0x1a, 0);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x107, 0x1e));
  Actor_FaceDirection(0x1b, 0, 0);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_SetSpeed(7, 0x40000, 0x20000);
  Actor_SetSpeed(0x1b, 0x20000, 0x10000);
  Actor_WalkBy(7, 8, 0x18);
  Actor_WalkByAndWait(0x1b, 8, 4);
  FieldActor_WaitForMoveById(7);
  FieldEvent_Wait(5);
  ((struct Actor *)Actor_ShowEmote(0x1a, 0x103, 0x1e));
  Actor_FaceActor(0x1a, 7, 0);
  Actor_FaceActor(0x1b, 7, 0);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 5);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  Actor_FaceActor(0x1a, 0, 0);
  Actor_FaceActor(0x1b, 0, 0);
  Actor_RunRepeatedMotion(0, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0, 0, 5);
  FieldEvent_Wait(0x28);
  Actor_FaceDirectionAndWait(4, 0x2000);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldActor_WatchAndSetScript(0x1a, 4);
  FieldActor_WatchAndSetScript(0x1b, 4);
  Actor_WalkByAndWait(4, 5, 0x20);
  Actor_WalkByAndWait(4, 0x38, -2);
  FieldEvent_Wait(0x1e);
  FieldActor_StartRepeatedMotion(0, 2);
  FieldEvent_Wait(4);
  Actor_RunRepeatedMotion(4, 2);
  PartyInventory_Remove(0xdf);
  PartyInventory_Add(0xf5);
  Message_ShowCentered((s16)EVENT_WORK->messageId++, 5);
  index = 0;
  offset = 0xd8;
  for (index = 0; index <= 14; offset += 2, index++) {
    if ((*(u16 *)((u8 *)Unit_Get(4) + offset) & 0x1ff) == 245) {
      Func_020047a4(4, index);
      break;
    }
  }
  ACTOR_SET_MODE_AND_WAIT(0x1b, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 5);
  Actor_FaceActor(4, 0x1b, 0);
  FieldEvent_Wait(0x14);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0x14);
  Actor_SetSpeed(7, 0x20000, 0x10000);
  Actor_SetSpeed(0x1a, 0x8000, 0x4000);
  Actor_SetSpeed(0x1b, 0x8000, 0x4000);
  Actor_SetSpeed(4, 0x8000, 0x4000);
  Actor_SetScript(0x1a, (s32)gData_020059a4);
  FieldEvent_Wait(0x28);
  Actor_SetDestinationOffset(7, -16, 0);
  FieldEvent_Wait(0xa);
  Actor_SetScript(0x1b, (s32)gData_020059a4);
  FieldEvent_Wait(0x6e);
  Actor_SetScript(4, (s32)gData_020059a4);
  FieldActor_RefreshSelectorById(0x1a);
  Actor_SetScript(0x1a, (s32)gData_02005a00);
  FieldActor_RefreshSelectorById(0x1b);
  Actor_SetScript(0x1b, (s32)gData_02005a00);
  FieldActor_RefreshSelectorById(4);
  Actor_SetAnimation(4, 1);
  FieldActor_StartRepeatedMotion(7, 2);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(0x1c, 0xc000, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  FieldCamera_FollowActor(6, 1);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ((struct Actor *)Actor_ShowEmote(4, 0x101, 0x1e));
  ((struct Actor *)Actor_ShowEmote(7, 0x109, 0x1e));
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  ((struct Actor *)Actor_ShowEmote(4, 0x100, 0x1e));
  ((struct Actor *)Actor_ShowEmote(6, 0x106, 0x1e));
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(5, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_FaceDirection(5, 0xc000, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(4, 0x4000);
  FieldEvent_Wait(0xa);
  ACTOR_SET_MODE_AND_WAIT(0x1c, 3, 0x14);
  FieldEvent_Wait(0xa);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 5);
  ACTOR_SET_MODE_AND_WAIT(4, 3, 0x14);
  FieldEvent_Wait(0xa);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_SetScript(7, (s32)gData_020059a4);
  FieldActor_RefreshSelectorById(7);
  Party_AddActiveOwner(7);
  FieldCamera_FollowActor(4, 1);
  Actor_SetPosition(7, 0, 0);
  FieldEvent_Wait(0x14);
#if TLA_PATCH_1
  Event_ClearInvalidPackedValues();
#endif
  Actor_WalkToAndWait(4, 0x298, 0x46);
  Func_020047dc();
}

extern s32 gData_02005a3c;

void Func_02004994(s32 id, s32 mode);
void Func_02004984(void);
void Func_0200498c(s32 a, s32 b, s32 c, s32 d);
void Func_02000c08(void);
void Func_02001e38(void);
void Func_02000cac(s32 value);
void Func_02001c7c(void);
void Func_02001ed0(void);
void Func_02000b48(void);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): forwarded inline; as plain calls, 58 of 203
   * instructions differ in tla-en. */
  return GameFlag_Test(flag);
}

/// Sets up Jupiter Lighthouse room M: objects 8 to 15 take mode 1, objects 24
/// and 25 stand at heights 0 and 32, the cells flags 514 and 2591 opened are
/// stamped again, and with 2591 set the arrival is chosen by flags 2595, 2593
/// and 2596 and the entrance.
s32 Func_02002c54(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Func_02004994(8, 1);
  Func_02004994(9, 1);
  Func_02004994(10, 1);
  Func_02004994(11, 1);
  Func_02004994(12, 1);
  Func_02004994(13, 1);
  Func_02004994(14, 1);
  Func_02004994(15, 1);
  Func_02004984();
  Func_0200498c(0, 512, 21, 22);
  gData_02005a3c = 0;
  object = Actor_Get(24);
  object->motionFlags = 0;
  object->y = 0;
  object->terrainHeight = 0;
  object = Actor_Get(25);
  object->motionFlags = 0;
  object->y = PIXELS(32);
  object->terrainHeight = PIXELS(32);
  if (Event_GetFlag(514)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
  }
  if (Event_GetFlag(2591)) {
    MapGrid_CopyCellsLayered(49, 106, 4, 3, 49, 76);
    MapGrid_CopyCellAttributes(49, 45, 4, 1, 49, 44);
    Actor_SetPosition(23, 0, 0);
    if (GameFlag_Test_(2595)) {
      if (!Event_GetFlag(2593)) {
        Func_02000c08();
        Func_02001e38();
      }
    } else if (!GameFlag_Test_(2596)) {
      if (gMainRoutineWork.entrance == 3) {
        Func_02000c08();
        Event_SetFlag(2596);
        Func_02000cac(gMainRoutineWork.entrance == 3);
      }
    } else {
      Func_02000c08();
      if (gMainRoutineWork.entrance == 5) {
        Func_02001c7c();
      } else if (gMainRoutineWork.entrance == 77) {
        Func_02001ed0();
        Event_SetFlag(2595);
        Event_ClearFlag(2596);
      } else {
        Func_02000b48();
      }
    }
  }
  Actor_Get(23)->motionFlags = 0;
  if (Event_GetFlag(2573))
    Actor_SetPosition(23, 0, 0);
  if (gMainRoutineWork.entrance == 99)
    Func_02001ed0();
  return 0;
}

void Scene_SetArrivalFlags();
void Func_02002fc4();
void Func_02003a38();
void Func_02003ac0();
void Func_02004774(s32 x, s32 z, s32 a, s32 b);

/// The scene's map preparation.
s32 Func_02002e3c(void) {
  struct FieldActor *object;

  Scene_SetArrivalFlags(0);
  Func_02002fc4(1);
  Func_02003a38();
  Func_02003ac0(20, 0);
  object = ((struct FieldActor *)Actor_Get(20));
  if (Event_GetFlag(0xa0c)) {
    Actor_SetPosition(20, PIXELS(1016), PIXELS(248));
    MapGrid_CopyCellsLayered(104, 42, 5, 11, 61, 42);
    MapGrid_CopyCellsLayered(106, 52, 1, 1, 63, 84);
    MapGrid_SetCellBehavior(0x4100000, 0xa00000, 2, 236);
  }
  object->motionFlags = 0;
  {
    s32 zero = 0;

    /* FAKEMATCH(barrier): the last call's zero is built before the position
     * loads; as a plain 0 argument, 2 of 62 instructions differ in tla-en. */
    __asm__("" : : "r"(zero));
    Func_02004774(object->x, object->z, zero, -6);
  }
  return 0;
}
