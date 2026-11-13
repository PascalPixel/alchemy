/// @file
/// The scene code of overlay 0x665, in ROM order.

#include "system/main_routine_work.h"

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/alhafra.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02001e48[];
extern const u8 gData_02002100[];
extern const u8 gData_02002358[];
extern struct ActorPlacement gActorPlacements[];
extern struct DoorStep gData_0200277c[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (GameFlag_Test(0x8a7)) {
    if (GameFlag_Test(0x90f) == 0)
      return gActorPlacements;
    return gData_02002358;
  }
  if (GameFlag_Test(0x8ab))
    return gData_02002100;
  return gData_02001e48;
}

void Func_02000098(struct DoorStep *step, s32 countdown, s32 unused) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Map_PlayMapEdit(step->script, step->x, step->z);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_0200010c(s32 index) {
  struct DoorStep *table = gData_0200277c;

  Func_02000098(&table[index], index, 0);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Shop_Run();

void Func_02000124(s32 a0) { Shop_Run(7, a0); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000130(s32 a0) { Shop_Run(8, a0); }
#undef FIELD_AT

void Func_0200013c(s32 actor) {
  s32 msg;

  if (Event_GetFlag(0x8b0) == 0) {
    msg = (s32)&MsgAlhafra03;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_SetMessageId(msg + 1);
      GameFlag_SetBit(0x8b0);
    } else {
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  } else {
    Event_SetMessageId((s32)&MsgAlhafra04);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_020001a4(s32 actor) {
  s32 msg = (s32)&MsgAlhafra16;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_020001f0(s32 actor) {
  s32 msg = (s32)&MsgAlhafra33;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200023c(void) { Func_02001958(1); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000248(void) { Func_02001958(0); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000254(void) { FieldActor_SetSpritePriority(25, 2); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000260(void) { FieldActor_SetSpritePriority(25, 3); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(alhafra): routine Func_02001d2c as s32 Func_02001d2c();
 * module/scene/scene.h has s32 Func_02001d2c(s32 x, s32 z, s32 layer); tried as
 * the rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/alhafra.c:194: too few arguments to function
 * `Func_02001d2c'. */
extern s32 Func_02001d2c__alhafra() __asm__("Func_02001d2c");
extern u8 gData_02001d54[];

void Func_0200026c(s32 a0, s32 a1) {

  Func_02001d2c__alhafra((s32)gData_02001d54);
  Actor_GetById(a1)->priorityFlags = 0;
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000288(void) {
  Event_SetMessageId((s32)MsgAlhafra50);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  Actor_ArmCallback(19, -256, 0);
  Event_Wait(10);
  Func_080d47b4(0x13, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x13, 0, 0xa);
  FieldActor_StartRepeatedMotion(24, 3);
  FieldEffect_SetAttached(0x18, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(19, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0);
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020002f8(void) {
  GameFlag_SetBit(0x8b5);
  PartyInventory_Remove(0x1c9);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra69);
  Event_Wait(10);
  Actor_SetAngleToward(27, gMainRoutineWork.currentOwner, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1b, 0, 0xa);
  Event_ShowMessageThenWait(-1, 0, 0xa);
  Actor_SetModeById(27, 1);
  Actor_Jump(27, 6, 15);
  Actor_Jump(27, 6, 23);
  Event_ShowMessageThenWait(0x1b, 0, 0);
  Func_02000398(0x1b);
  Actor_FaceDirection(27, 0x4000, 0);
  Event_End();
}
#undef FIELD_AT

#include "ram_buffer.h"

void Func_02000398(s32 id) {
  if (!GameFlag_Test(0x8b5)) {
    ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
    Event_SetMessageId(MsgAlhafra65);
    Event_ShowMessageAndWait(id, 0);
  } else if (Event_GetFlag(0x8b6)) {
    Event_SetMessageId(MsgAlhafra71);
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId(MsgAlhafra70);
    Event_Wait(10);
    Motion_SetModeAndWaitAnimation(id, 3);
    Event_ShowMessageThenWait(id, 0, 10);
    Motion_SetVarCbAndRefresh(id, 2);
    FieldMessage_ShowAtActor(id, 0);
    Event_Wait(10);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      GameFlag_SetBit(0x8b6);
      Event_Wait(20);
      Event_ShowMessageThenWait(id, 0, 10);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
    } else {
      Event_Wait(30);
      ((struct AlhafraEventWork *)Ram_HeapSlots->eventWork)->value1c4++;
      Event_ShowMessageThenWait(id, 0, 10);
    }
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/* VIEW(alhafra): routine FieldCamera_WaitForArrival as void
 * FieldCamera_WaitForArrival(void); module/scene/scene.h has void
 * FieldCamera_WaitForArrival(s32 actor, s32 value); tried as the rest of the
 * object declares it, Func_0200047c moves: 3 of 189 instructions, Func_02000aa0
 * moves: 11 of 1456 instructions in tla-ja. */
extern void Func_02001ccc__alhafra(void) __asm__("FieldCamera_WaitForArrival");

void Func_0200047c(void) {
  GameFlag_SetBit(0x8b7);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra74);
  Camera_MoveWithinMapBounds(0x2300000, -1, 0x1f00000, 1);
  Func_02001ccc__alhafra();
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_FaceDirection(28, 0, 0);
  Actor_FaceDirection(31, 0x6000, 0);
  Actor_FaceDirection(30, 0x6000, 0);
  Actor_SetModeById(29, 5);
  Sound_Play(0x59);
  gMainRoutineWork.currentOwner = 0x1d;
  BattleEffect_StartEffectObject22(0, 3);
  gMainRoutineWork.currentOwner = 4;
  Event_Wait(50);
  FieldActor_Jump(28, 6, 15);
  FieldActor_Jump(28, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_ArmCallback(29, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  Func_080d47b4(0x1e, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Func_080d47b4(0x1d, 0x102, 0x28);
  Actor_FaceDirection(29, 0x4000, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_ArmCallback(29, 0, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(20);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Actor_FaceDirection(30, 0, 0);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_SetAnimation(31, 6);
  Actor_FaceDirection(30, 0xc000, 0);
  Actor_SetAnimation(30, 6);
  Actor_FaceDirection(29, 0x4000, 0);
  Actor_FaceDirection(28, 0xc000, 0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02001ccc__alhafra();
  Event_End();
}
#undef FIELD_AT

/// Talking to the actor: once flag 0x8b9 is set it says its last line;
/// otherwise it asks a yes-or-no question. A nonzero answer ends with another
/// line; the other sets the flag, and the actor and the leader step aside.
void Func_0200066c(s32 actor) {
  s32 message = (s32)MsgAlhafra92;

  if (Event_GetFlag(0x8b9) != 0) {
    Event_SetMessageId(message + 4);
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) != 0) {
      Event_Wait(20);
      Event_SetMessageId(message + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      GameFlag_SetBit(0x8b9);
      Event_Wait(20);
      Event_SetMessageId(message + 2);
      Event_ShowMessage(actor, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 344);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
      Event_Wait(10);
      Actor_SetSpeed(actor, 0x10000, 0x8000);
      Actor_WalkToAndWait(actor, 56, 328);
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
      Actor_ArmCallback(actor, FACING_SOUTHEAST, 0);
      Event_Wait(20);
      Motion_SetModeAndWaitAnimation(actor, 3);
      Event_Wait(10);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000760(void) {
  TreasureSpot_SetPosition(0x40, 0xb80000, 0x2280000);
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000774(void) { TreasureSpot_SetPosition(0x40, -1, -1); }
#undef FIELD_AT

extern struct Trigger gData_020027f4[];
extern struct Trigger gData_02002ad0[];
extern struct Trigger gData_02002e48[];
extern struct Trigger gData_02003154[];

const struct Trigger *Scene_GetEvents(void) {
  if (GameFlag_Test(0x8a7)) {
    if (GameFlag_Test(0x90f) == 0)
      return gData_02003154;
    return gData_02002e48;
  }
  if (GameFlag_Test(0x8ab))
    return gData_02002ad0;
  return gData_020027f4;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Scene_Initialize(void) {
  s32 t1, t2, t9, t23, t37, t39, t41, t44, t51, t53, t56, t58, t60;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  t1 = GameFlag_Test(0x8a7);
  if (t1 == 0)
    goto L_020088aa;
  t2 = GameFlag_Test(0x90f);
  if (t2 != 0)
    goto L_02008836;
  Actor_GetById(24)->update = (void (*)(void))Func_02000a98;
  Actor_GetById(19)->update = (void (*)(void))Func_02000a98;
  Actor_SetSpritePriority(20, 2);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(16, 3);
  Actor_SetSpritePriority(10, 1);
  goto L_02008a0a;
L_02008836:
  t9 = (s32)Actor_Get(14);
  FIELD_AT(t9, s32, 108) = (s32)Func_02000a98;
  Actor_GetById(20)->update = (void (*)(void))Func_02000a98;
  Actor_GetById(22)->update = (void (*)(void))Func_02000a98;
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(19, 3);
  Actor_SetSpritePriority(23, 2);
  Actor_SetSpritePriority(24, 3);
  Actor_SetSpritePriority(27, 2);
  Actor_SetSpritePriority(28, 3);
  Actor_SetSpritePriority(29, 1);
  Actor_SetSpritePriority(30, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(12, 3);
  Actor_SetSpritePriority(10, 3);
  goto L_02008a0a;
L_020088aa:
  t23 = GameFlag_Test(0x8ab);
  if (t23 == 0)
    goto L_0200898a;
  Actor_GetById(14)->update = (void (*)(void))Func_02000a98;
  Actor_GetById(20)->update = (void (*)(void))Func_02000a98;
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(19, 3);
  Actor_SetSpritePriority(23, 3);
  Actor_SetSpritePriority(24, 3);
  Actor_SetSpritePriority(22, 2);
  Actor_SetSpritePriority(27, 2);
  Actor_SetSpritePriority(28, 3);
  Actor_SetSpritePriority(29, 1);
  Actor_SetSpritePriority(30, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(12, 3);
  t37 = ((s32)Actor_GetById(26));
  Actor_SetActionCallback(t37, 9);
  t39 = GameFlag_Test(0x8b5);
  if (t39 != 0)
    goto L_02008944;
  Actor_SetModeById(27, 5);
L_02008944:
  t41 = GameFlag_Test(0x8b7);
  if (t41 == 0)
    goto L_02008962;
  Actor_SetModeById(30, 6);
  Actor_SetModeById(31, 6);
L_02008962:
  t44 = GameFlag_Test(0x8b9);
  if (t44 == 0)
    goto L_02008a0a;
  Actor_SetPosition(12, PIXELS(56), PIXELS(328));
  Actor_ArmCallback(12, FACING_EAST, 0);
  goto L_02008a0a;
L_0200898a:
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(23, 2);
  Actor_SetSpritePriority(10, 3);
  Actor_GetById(24)->update = (void (*)(void))Func_02000a98;
  t51 = GameFlag_Test(0x8b3);
  if (t51 == 0)
    goto L_020089c8;
  Actor_SetPosition(14, PIXELS(536), PIXELS(760));
L_020089c8:
  t53 = GameFlag_Test(0x8b4);
  if (t53 == 0)
    goto L_020089e0;
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
L_020089e0:
  Func_02001d24((s32)&gData_02001d54);
  t56 = ((s32)Actor_GetById(26));
  Actor_SetActionCallback(t56, 9);
  t58 = ((s32)Actor_GetById(27));
  Actor_SetActionCallback(t58, 9);
  t60 = ((s32)Actor_GetById(28));
  Actor_SetActionCallback(t60, 9);
L_02008a0a:
  return 0;
}
#undef FIELD_AT

s32 Scene_PrepareMap(void) {
  if (GameFlag_Test(0x8ab)) {
    u16 *src = (u16 *)0x5000116;

    u32 c;

    /* FAKEMATCH(dead): the four copies in a once-only block, whose end holds
     * the order around it; as plain statements, 4 of 57 instructions differ in
     * tla-en. */
    do {
      c = *src;
      *(u16 *)0x50000f6 = c;
      src++;
      c = *src;
      *(u16 *)0x50000f8 = c;
      src++;
      c = *src;
      *(u16 *)0x50000fa = c;
      {
        u32 last = src[1];

        *(u16 *)0x50000fc = last;
      }
    } while (0);
    MapGrid_CopyCellAttributes(37, 18, 2, 2, 37, 33);
    MapGrid_CopyCellAttributes(37, 18, 2, 3, 39, 31);
    MapGrid_CopyCellAttributes(50, 80, 7, 5, 35, 95);
  }
  return 0;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000a98(void) { Func_02001d3c(); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_020033a0[];
extern u8 gData_020033f8[];
extern u8 gData_02003434[];
extern u8 gData_02003550[];
extern u8 gData_020035e8[];
extern u8 gData_020034cc[];
extern u8 gData_0200374c[];
extern u8 gData_0200366c[];
extern u8 gData_020036c8[];
extern u8 gData_020037f8[];
extern u8 gData_02003890[];
extern u8 gData_02003970[];
extern u8 gData_020039cc[];
extern u8 gData_02003914[];
extern u8 gData_02003a28[];
extern u8 gData_02003b08[];
extern u8 gData_02003b78[];
extern u8 gData_02003a98[];

void Func_02000aa0(s32 a0) {
  GameFlag_SetBit(0x8b3);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafra10);
  Actor_SetSpeed(29, 0x10000, 0x8000);
  Actor_SetSpeed(30, 0x10000, 0x8000);
  Actor_SetSpeed(31, 0x10000, 0x8000);
  Actor_SetSpeed(32, 0x10000, 0x8000);
  Actor_SetSpeed(33, 0x10000, 0x8000);
  Actor_SetSpeed(34, 0x10000, 0x8000);
  Actor_SetSpeed(35, 0x10000, 0x8000);
  Actor_SetSpeed(17, 0x8000, 0x4000);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  if (a0 == 0x14) {
    Actor_WalkByAndWait(4, 0, -8);
  }
  Actor_SetPosition(31, 0x1980000, 0x3680000);
  Actor_SetPosition(32, 0x1880000, 0x3780000);
  Event_ShowMessageThenWait(0x1f, 0, 0xa);
  if (a0 == 0x14) {
    Actor_FaceDirection(4, 0x4000, 0);
  } else {
    Actor_ArmCallback(4, 0, 0);
  }
  Event_Wait(20);
  FieldCamera_MoveTo(0x1900000, -1, 0x3280000, 1);
  Func_02001ccc__alhafra();
  Actor_WalkBy(31, 0, -96);
  Actor_WalkByAndWait(32, 0, -96);
  Actor_SetModeById(31, 1);
  Event_Wait(10);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0xa000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0xa000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_FaceDirection(31, 0, 0);
  Actor_SetPosition(33, 0x1880000, 0x3680000);
  Actor_SetPosition(35, 0x1980000, 0x3680000);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_FaceDirection(32, 0x3000, 0);
  Event_Wait(20);
  Actor_WalkBy(35, 0, -48);
  Actor_WalkByAndWait(33, 0, -48);
  Actor_WaitForMoveById(35);
  Actor_ArmCallback(35, 0xb000, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(31, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  {
    s32 y = 0x102, z = 0x1e;
    /* FAKEMATCH(barrier): the empty asm claims to change y and z, so the second
     * and third arguments are set before the first, as the ROM does; as the
     * call with constant arguments, 37 of 1456 instructions differ in tla-en.
     */
    __asm__ volatile("" : "+l"(y), "+l"(z));
    Func_080d47b4(0x21, y, z);
  }
  Actor_FaceDirection(33, 0x3000, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(33, 2);
  Event_Wait(20);
  Actor_FaceDirection(33, 0xc000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(20);
  Actor_SetAnimation(31, 3);
  Motion_SetModeAndWaitAnimation(32, 3);
  Event_Wait(10);
  Actor_WalkBy(35, 32, -48);
  Actor_WalkByAndWait(33, 32, -48);
  Actor_WaitForMoveById(35);
  Actor_FaceDirection(35, 0x5000, 0);
  Actor_FaceDirection(33, 0x4000, 0);
  Actor_SetSpritePriority(35, 3);
  Event_Wait(20);
  Actor_SetPosition(34, 0x1900000, 0x3680000);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_FaceDirection(31, 0x4000, 0);
  Actor_FaceDirection(32, 0x3000, 0);
  Actor_FaceDirection(33, 0x4000, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(34, 0, -32);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(90);
  Motion_SetVarCbAndRefresh(34, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_CommitPositionAndActivate(34, 0, -16);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(60);
  Actor_WalkByAndWait(34, 8, -32);
  Actor_FaceDirection(34, 0x4000, 0);
  Event_Wait(1);
  Actor_SetModeById(34, 5);
  Event_Wait(30);
  Actor_SetPosition(30, 0x1880000, 0x3680000);
  Actor_SetPosition(29, 0x1980000, 0x3680000);
  Actor_WalkBy(30, 0, -32);
  Actor_WalkByAndWait(29, 0, -32);
  Actor_SetModeById(30, 1);
  Event_Wait(1);
  Actor_SetAnimation(29, 5);
  FieldActor_WatchAndSetScript(31, 30);
  FieldActor_WatchAndSetScript(32, 30);
  FieldActor_WatchAndSetScript(33, 30);
  FieldActor_WatchAndSetScript(34, 30);
  Actor_WatchAndSetScript(35, 30);
  Event_Wait(20);
  Actor_ArmCallback(30, 0xa000, 0);
  Event_Wait(20);
  Actor_ArmCallback(30, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(30, 0xc000, 0);
  Event_Wait(10);
  FieldActor_Jump(30, 6, 15);
  FieldActor_Jump(30, 6, 23);
  Actor_SetAnimation(34, 1);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetMovementTypeById(29, (s32)gData_020033a0);
  Event_Wait(20);
  Actor_WalkByAndWait(30, 0, -32);
  Actor_CommitPositionAndActivate(30, -32, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetAnimationAndWait(29, 4);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_ShowEmote(0x1e, 0x102, 0x1e);
  Actor_ArmCallback(30, 0, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetAnimation(29, 5);
  Actor_WalkByAndWait(30, 64, 0);
  Actor_FaceDirection(29, 0x5000, 0);
  Actor_FaceDirection(30, 0xe000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Actor_SetModeById(29, 1);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_Stop(31);
  Actor_Stop(32);
  Actor_Stop(33);
  Actor_Stop(34);
  Actor_Stop(35);
  Actor_SetModeById(29, 5);
  Event_Wait(50);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_FaceDirection(32, 0xe000, 0);
  Event_Wait(40);
  Actor_FaceDirection(31, 0x3000, 0);
  Actor_FaceDirection(32, 0x2000, 0);
  Event_Wait(20);
  Actor_SetMovementTypeById(14, (s32)gData_020033f8);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  Event_ShowMessageThenWait(0x1f, 0, 0xa);
  Event_Wait(10);
  Actor_ArmCallback(32, 0xc000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x4000, 0);
  Event_Wait(20);
  Actor_FaceDirection(32, 0x8000, 0);
  Event_Wait(20);
  Actor_ArmCallback(32, 0x1000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x20, 0, 0xa);
  Actor_SetModeById(29, 1);
  Event_Wait(10);
  Func_080d47b4(0x1d, 0x107, 0x1e);
  Actor_FaceDirection(29, 0x8000, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  FieldCamera_FollowActor(29, 1);
  Func_02001ccc__alhafra();
  Actor_WalkToAndWait(29, 488, 808);
  Actor_FaceDirection(31, 0, 0);
  Actor_FaceDirection(32, 0, 0);
  Actor_FaceDirection(33, 0, 0);
  Actor_FaceDirection(34, 0, 0);
  Actor_FaceDirection(35, 0x3000, 0);
  Actor_FaceDirection(30, 0, 0);
  Actor_FaceDirection(29, 0xd000, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(29, 4);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(0x2880000, -1, 0x2180000, 1);
  Func_02001ccc__alhafra();
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  FieldCamera_MoveTo(0x1b80000, -1, 0x3280000, 1);
  Func_02001ccc__alhafra();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  Event_Wait(20);
  Actor_WalkByAndWait(33, 32, 36);
  Actor_WalkByAndWait(33, 32, 0);
  Actor_WalkToAndWait(33, 504, 808);
  Actor_ArmCallback(33, 0xd000, 0);
  Event_Wait(30);
  FieldActor_StartRepeatedMotion(33, 3);
  FieldEffect_SetAttached(0x21, 0x102);
  Event_Wait(50);
  Actor_FaceDirection(33, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(33, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x21, 0, 0xa);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(20);
  Actor_ArmCallback(29, 0, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x201d, 0, 0xa);
  Func_080d47b4(0x22, 0x100, 0x28);
  FieldEvent_ShowMessageThenWait(0x22, 0, 0xa);
  Actor_FaceDirection(29, 0x8000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(29, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetVarCbAndRefresh(30, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_FaceDirection(31, 0x5000, 0);
  Actor_ArmCallback(32, 0xe000, 0);
  Event_Wait(30);
  FieldActor_Jump(31, 6, 15);
  FieldActor_Jump(31, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1f, 0, 0xa);
  Motion_SetModeAndWaitAnimation(32, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x20, 0, 0xa);
  Motion_SetVarCbAndRefresh(29, 2);
  Event_Wait(10);
  Actor_FaceDirection(31, 0x3000, 0);
  Actor_FaceDirection(32, 0x2000, 0);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1e, 0, 0xa);
  Actor_SetPosition(17, 0x1980000, 0x2b80000);
  Actor_WalkToAndWait(17, 408, 760);
  Actor_WalkToAndWait(17, 448, 792);
  Actor_WalkToAndWait(17, 448, 808);
  Actor_WalkByAndWait(30, 8, 0);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Func_080d47b4(0x11, 0x101, 0x32);
  Actor_FaceDirection(17, 0x8000, 0);
  Event_Wait(20);
  Func_080d47b4(0x1e, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x101e, 0, 0xa);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->waitMode = 0;
  FieldEvent_ShowMessageThenWait(0x11, 0, 0xa);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_SetSpeed(29, 0x8000, 0x4000);
  Actor_SetSpeed(30, 0x8000, 0x4000);
  Actor_SetSpeed(31, 0x8000, 0x4000);
  Actor_SetSpeed(32, 0x8000, 0x4000);
  Actor_SetSpeed(33, 0x8000, 0x4000);
  Actor_SetSpeed(34, 0x8000, 0x4000);
  Actor_SetSpeed(35, 0x8000, 0x4000);
  Actor_SetSpeed(17, 0x8000, 0x4000);
  FieldCamera_FollowActor(30, 1);
  Func_02001ccc__alhafra();
  if (a0 == 0x15) {
    FieldActor_SetSpritePriority(30, 2);
    FieldActor_WatchAndSetScript(31, 29);
    FieldActor_WatchAndSetScript(32, 29);
    FieldActor_WatchAndSetScript(34, 29);
    Actor_SetMovementTypeById(17, (s32)gData_02003434);
    Actor_SetMovementTypeById(29, (s32)gData_02003550);
    Actor_SetMovementTypeById(33, (s32)gData_020035e8);
    Event_Wait(80);
    FieldActor_SetSpritePriority(29, 3);
    FieldActor_SetSpritePriority(30, 3);
    FieldActor_SetSpritePriority(31, 3);
    FieldActor_SetSpritePriority(32, 3);
    FieldActor_SetSpritePriority(33, 3);
    FieldActor_SetSpritePriority(34, 3);
    FieldActor_SetSpritePriority(35, 3);
    FieldActor_SetSpritePriority(17, 3);
    FieldActor_WatchAndSetScript(35, 29);
    Actor_SetMovementTypeById(30, (s32)gData_020034cc);
    Event_Wait(170);
    Actor_SetMovementTypeById(32, (s32)gData_0200374c);
    Event_Wait(10);
    Actor_SetMovementTypeById(31, (s32)gData_0200366c);
    Actor_SetMovementTypeById(34, (s32)gData_020036c8);
    Event_Wait(60);
    Actor_SetMovementTypeById(35, (s32)gData_020037f8);
  } else {
    FieldActor_SetSpritePriority(30, 2);
    FieldActor_WatchAndSetScript(30, 17);
    FieldActor_WatchAndSetScript(31, 30);
    FieldActor_WatchAndSetScript(32, 30);
    FieldActor_WatchAndSetScript(34, 30);
    Actor_SetMovementTypeById(17, (s32)gData_02003890);
    Event_Wait(60);
    Actor_SetMovementTypeById(29, (s32)gData_02003970);
    Actor_SetMovementTypeById(33, (s32)gData_020039cc);
    Event_Wait(70);
    FieldActor_WatchAndSetScript(35, 29);
    Actor_SetMovementTypeById(30, (s32)gData_02003914);
    FieldActor_SetSpritePriority(29, 3);
    FieldActor_SetSpritePriority(30, 3);
    FieldActor_SetSpritePriority(31, 3);
    FieldActor_SetSpritePriority(32, 3);
    FieldActor_SetSpritePriority(33, 3);
    FieldActor_SetSpritePriority(34, 3);
    FieldActor_SetSpritePriority(35, 3);
    Actor_SetSpritePriority(17, 3);
    Event_Wait(120);
    Actor_SetMovementTypeById(34, (s32)gData_02003a28);
    Event_Wait(40);
    Actor_SetMovementTypeById(31, (s32)gData_02003b08);
    Actor_SetMovementTypeById(35, (s32)gData_02003b78);
    Event_Wait(30);
    Actor_SetMovementTypeById(32, (s32)gData_02003a98);
  }
  Event_Wait(300);
  Actor_RefreshSelectorById(34);
  Event_Wait(50);
  FieldCamera_MoveTo(0xf80000, -1, 0x2980000, 1);
  Func_02001ccc__alhafra();
  FieldEvent_ShowMessageThenWait(0x11, 0, 0xa);
  Actor_ArmCallback(17, 0x3000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(30, 3);
  Event_Wait(20);
  Actor_WalkByAndWait(17, -16, 0);
  Actor_ArmCallback(17, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(29, 0x8000, 0);
  Actor_FaceDirection(31, 0x8000, 0);
  Actor_FaceDirection(32, 0x8000, 0);
  Actor_FaceDirection(30, 0, 0);
  Actor_FaceDirection(33, 0, 0);
  Actor_ArmCallback(34, 0, 0);
  Event_Wait(20);
  Actor_SetAnimation(30, 3);
  Actor_SetAnimation(29, 3);
  Actor_SetAnimation(33, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(32, 3);
  Actor_SetAnimation(35, 3);
  Motion_SetModeAndWaitAnimation(34, 3);
  Event_Wait(10);
  Actor_FaceDirection(29, 0xc000, 0);
  Actor_FaceDirection(31, 0xc000, 0);
  Actor_FaceDirection(32, 0xc000, 0);
  Actor_FaceDirection(30, 0xc000, 0);
  Actor_FaceDirection(33, 0xc000, 0);
  Actor_FaceDirection(34, 0xc000, 0);
  Actor_SetSpeed(29, 0x1e666, 0xf333);
  Actor_SetSpeed(30, 0x1e666, 0xf333);
  Actor_SetSpeed(31, 0x1e666, 0xf333);
  Actor_SetSpeed(32, 0x1e666, 0xf333);
  Actor_SetSpeed(33, 0x1e666, 0xf333);
  Actor_SetSpeed(34, 0x1e666, 0xf333);
  Actor_SetSpeed(35, 0x1e666, 0xf333);
  Actor_WalkBy(29, 0, -150);
  Actor_WalkBy(30, 0, -150);
  Actor_WalkBy(31, 0, -150);
  Actor_WalkBy(32, 0, -150);
  Actor_WalkBy(33, 0, -150);
  Actor_WalkBy(35, 0, -150);
  Actor_WalkByAndWait(34, 0, -150);
  Actor_SetPosition(29, 0, 0);
  Actor_SetPosition(30, 0, 0);
  Actor_SetPosition(31, 0, 0);
  Actor_SetPosition(32, 0, 0);
  Actor_SetPosition(33, 0, 0);
  Actor_SetPosition(34, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(35, 0, 0);
  Event_Wait(15);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02001ccc__alhafra();
  Event_End();
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001958(s32 a0) {

  Event_SetFlag(0x8b4);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafra28);
  if (a0 == 1) {
    FieldEvent_Wait(10);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 736);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  }
  Actor_ShowEmote(0x19, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  if (a0 != 1) {
  } else {
    Actor_CommitPositionAndActivate(25, -8, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(25, 3);
    FieldEffect_SetAttached(0x19, 0x102);
    Event_Wait(40);
    Actor_GetById(25)->actionFlags &= 0xfe;
    Actor_SetPositionAndReset(25, 192, 736);
    Event_Wait(1);
    Actor_GetById(25)->actionFlags |= 1;
    Event_Wait(10);
    Event_ShowMessageThenWait(0x19, 0, 0xa);
    {
      s32 b = 0x107, c = 0x1e;
      Func_080d47b4(0x19, b, c);
    }
    Event_OpenMessage(0x19, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      Event_Wait(30);
      Motion_SetVarCbAndRefresh(25, 2);
      Event_Wait(20);
      Event_ShowMessageThenWait(0x19, 0, 0xa);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
          (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    } else {
      Event_Wait(30);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
          (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
      Event_ShowMessageThenWait(0x19, 0, 0xa);
    }
    Actor_GetById(4)->actionFlags &= 0xfe;
    Actor_WalkToAndWait(4, 152, 752);
    Event_Wait(1);
    Actor_GetById(4)->actionFlags |= 1;
    Actor_FaceDirection(4, 0xe000, 0);
    Event_Wait(20);
    Motion_SetModeAndWaitAnimation(25, 3);
    Event_Wait(10);
    Actor_ArmCallback(25, 0xe000, 0);
    Event_Wait(70);
    {
      s32 b = 0x100, c = 0x32;
      Func_080d47b4(0x19, b, c);
    }
    Event_ShowMessageThenWait(0x19, 0, 0xa);
  }
  Actor_WalkToAndWait(25, 184, 728);
  Actor_WalkByAndWait(25, 0, -110);
  if (a0 == 0) {
    Event_Wait(50);
    Actor_SetPosition(25, 0x1080000, 0x2880000);
    Actor_CommitPositionAndActivate(25, 0, 32);
    Actor_CommitPositionAndActivate(25, 150, 0);
  }
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_End();
}
