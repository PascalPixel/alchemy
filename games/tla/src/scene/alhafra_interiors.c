/// @file
/// The scene code of overlay 0x666, in ROM order.

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
#include "module/scene/scene.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"
#include "field/field.h"
#include "frame.h"

struct AlhafraInteriorsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

void Event_End(void);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Event_StartScreenTransition(void);

// ---- this overlay

extern u8 gData_02000cbc[];
extern u8 gData_02000f14[];
extern u8 gData_0200116c[];
extern u8 gData_020013dc[];
extern u8 gData_02001730[];
extern u8 gData_020019dc[];
#include "event_inline.h"

u8 *Scene_GetPlacements(void) {
  if (Event_GetFlag(0x8a7))
    return gData_0200116c;
  if (Event_GetFlag(0x8ab))
    return gData_02000f14;
  return gData_02000cbc;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000084(void) {
  Event_SetFlag(0x8b2);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors31);
  FieldCamera_MoveToActorById(0x1f, 1);
  Actor_WalkToAndWait(4, 452, 335);
  Actor_FaceDirection(4, 0x8000, 0);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(31, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 4);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_RunRepeatedMotion(31, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x801f, 0, 0xa);
  Actor_FaceDirection(31, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

void Func_020001ac(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(9, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors14);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_020001fc(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(2, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors16);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_0200024c(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors49);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000298(s32 a0) {
  Actor_ShowEmote(a0, 0x107, 0x1e);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors06);
  Event_ShowMessage(a0, 0);
}

void Func_020002bc(void) {
  Event_SetFlag(0x8b1);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(FIELD_AT(&gMainRoutineWork, s32, 532), 152, 328);
  Actor_FaceDirection(FIELD_AT(&gMainRoutineWork, s32, 532), 0xc000, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors07);
  Actor_FaceDirection(14, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Actor_FaceDirection(15, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xf, 0, 0xa);
  Actor_SetSpeed(14, 0x8000, 0x4000);
  Actor_WalkByAndWait(14, -32, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xe, 0, 0xa);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(13, 0, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(0xd, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0xd, 0, 0xa);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(0xe, 0x102);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(0xf, 0x102);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(13, 0xb000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0xd, 0, 0xa);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(14, 152, 312);
  FieldEvent_Wait(10);
  Actor_SetAnimation(15, 4);
  Actor_SetAnimationAndWait(14, 4);
  FieldEvent_Wait(10);
  Actor_FaceDirection(14, 0x3000, 0);
  Actor_FaceDirection(15, 0x4000, 0);
  Event_End();
}

void Func_02000444(s32 a0) {
  Actor_FaceEachOther(a0, FIELD_AT(&gMainRoutineWork, s32, 532), 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors08);
  Event_ShowMessage(a0, 0);
  Actor_FaceDirection(14, 0x3000, 0);
}

void Func_0200047c(s32 a0, s32 a1) {
  Actor_FaceEachOther(a1, FIELD_AT(&gMainRoutineWork, s32, 532), 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors48);
  Event_ShowMessage(a1, 0);
  Actor_FaceDirection(a1, 0xe000, 0);
}

void Func_020004b8(s32 a0) {
  Event_SetFlag(0x8b8);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors55);
  Event_ShowMessage(a0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(a0, 0x108, 0x1e);
  Event_ShowMessage(a0, 0);
}

void Func_020004f4(s32 actor) {
  s32 msg = (s32)MsgAlhafraInteriors74;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

u8 *Scene_GetEvents(void) {
  if (Event_GetFlag(0x8a7))
    return gData_020019dc;
  if (Event_GetFlag(0x8ab))
    return gData_02001730;
  return gData_020013dc;
}

s32 BattleEffect_DrawSpriteAtWorldPosition();

s32 Func_02000578(s32 a0) {
  BattleEffect_DrawSpriteAtWorldPosition(
      (s32)gData_02001c90, FIELD_AT(a0, s32, 8),
      (FIELD_AT(a0, s32, 12) + 0xc0000), FIELD_AT(a0, s32, 16));
  return 0;
}

s32 Func_02000598(s32 a0) {
  BattleEffect_DrawSpriteAtWorldPosition(
      (s32)gData_02001c60, FIELD_AT(a0, s32, 8),
      (FIELD_AT(a0, s32, 12) + 0x80000), FIELD_AT(a0, s32, 16));
  return 0;
}

#include "actor.h"
#include "ram_buffer.h"
#include "call.h"
#include "version.h"

// The Japanese edition sets no flag 0x200 by the leader's position and places
// no actors 31 and 32.
#if TLA_PATCH_1
void BattleEffect_FillOverlayTiles(s32 mode);
void Func_02000bb4();

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  BattleEffect_FillOverlayTiles(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_Get(14)->update = Func_02000578;
  Actor_Get(15)->update = Func_02000598;
  if (Event_GetFlag(0x8a7)) {
    if (Actor_Get(gMainRoutineWork.currentOwner)->x >= 0x2000000)
      Event_SetFlag(0x200);
    else
      Event_ClearFlag(0x200);
    if (Event_GetFlag(0x200) && Event_GetFlag(0x823) && !Event_GetFlag(0x903)) {
      Actor_SetPosition(31, PIXELS(376), PIXELS(316));
      Actor_SetPosition(32, PIXELS(392), PIXELS(344));
    } else {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x971))
      Actor_SetPosition(31, 0, 0);
  } else {
    FieldActor_SetSpritePriority(24, 2);
    FieldActor_SetSpritePriority(19, 3);
    FieldActor_SetSpritePriority(28, 2);
    FieldActor_SetSpritePriority(25, 3);
  }
  return 0;
}
#else
void BattleEffect_FillOverlayTiles(s32 mode);
void Func_02000bb4();

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  BattleEffect_FillOverlayTiles(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_Get(14)->update = Func_02000578;
  Actor_Get(15)->update = Func_02000598;
  if (Event_GetFlag(0x8a7)) {
    if (!Event_GetFlag(0x823) || Event_GetFlag(0x903)) {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x971))
      Actor_SetPosition(31, 0, 0);
  } else {
    FieldActor_SetSpritePriority(24, 2);
    FieldActor_SetSpritePriority(19, 3);
    FieldActor_SetSpritePriority(28, 2);
    FieldActor_SetSpritePriority(25, 3);
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

extern u8 gData_02000be4[];

void Func_02000738(void) {
  s32 t0;
  s32 t1;

  t0 = Event_GetFlag(0x903);
  if (t0 != 0) {
  } else {
#if TLA_PATCH_1 /* The Japanese edition does not check 0x200. */
    t1 = Event_GetFlag(0x200);
    if (t1 == 0) {
    } else
#endif
    {
      Event_SetFlag(0x903);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgAlhafraInteriors120);
      Actor_WalkToAndWait(4, 504, 328);
      Actor_FaceDirectionAndWait(4, FACING_WEST);
      FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(328), 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x100, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_ShowEmote(0x1f, 0x109, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x102, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      FieldActor_Jump(32, 6, 15);
      FieldActor_Jump(32, 6, 23);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_ShowEmote(0x20, 0x109, 0x1e);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_FaceDirectionAndWait(31, 0xd000);
      FieldEvent_Wait(15);
      FieldEvent_ShowMessageThenWait(0x1f, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x20, 0, 5);
      Actor_FaceDirectionAndWait(31, 0x3000);
      FieldEvent_Wait(15);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(20);
      Actor_SetSpeed(31, 0x10000, 0x8000);
      Actor_SetSpeed(32, 0x10000, 0x8000);
      FieldActor_SetSpritePriority(31, 2);
      FieldActor_SetSpritePriority(32, 2);
      FieldCamera_FollowActor(32, 1);
      Actor_WalkTo(31, 394, 342);
      FieldEvent_Wait(10);
      Actor_WalkTo(32, 450, 346);
      FieldActor_WaitForMoveById(31);
      Actor_WalkTo(31, 450, 346);
      FieldActor_WaitForMoveById(32);
      Actor_WalkTo(32, 470, 328);
      FieldActor_WaitForMoveById(31);
      Actor_WalkTo(31, 470, 328);
      FieldActor_WaitForMoveById(32);
      Actor_WalkTo(32, 486, 328);
      FieldActor_WaitForMoveById(32);
      Actor_SetAnimation(32, 1);
      Actor_FaceDirectionAndWait(32, FACING_EAST);
      FieldActor_WaitForMoveById(31);
      Actor_SetAnimation(31, 1);
      Actor_FaceDirectionAndWait(31, FACING_EAST);
      Actor_WalkByAndWait(4, 0, -16);
      Actor_FaceDirectionAndWait(4, FACING_SOUTH);
      FieldEvent_Wait(10);
      Actor_SetScript(32, (s32)gData_02000be4);
      FieldEvent_Wait(10);
      Actor_SetScript(31, (s32)gData_02000be4);
      FieldEvent_Wait(10);
      FieldActor_WatchAndSetScript(4, 32);
      FieldEvent_Wait(60);
      FieldActor_SetSpritePriority(31, 3);
      FieldActor_SetSpritePriority(32, 3);
      FieldActor_RefreshSelectorById(32);
      FieldActor_Stop(4);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Actor_SetPosition(32, 0, 0);
      Event_PlaySound(0x7b);
      FieldActor_RefreshSelectorById(31);
      Actor_SetPosition(31, 0, 0);
      Event_PlaySound(0x7b);
      FieldEvent_Wait(10);
      Event_End();
    }
  }
}
