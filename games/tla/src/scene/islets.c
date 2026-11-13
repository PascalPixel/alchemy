/// @file
#include "flags.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "version.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "shared/push.h"
#include "system/heap_release.h"
#include "graphics/palette_snapshots.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "system/vector3.h"
#include "shared/tables.h"
#include "objcmd.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/shrine_of_the_sea_god.h"

struct Actor;

void Func_020000d8(void);

void Func_02000210(void);
void Func_020002ac(void);
void Func_02000380(void);
void Func_02000440(void);

void Func_02000568(void);

void Func_020014e0(void);
void Func_02001a68(void);

void Func_02001bf4(void);

void Func_02001c38(void);
void Func_0200208c(void);
void Leader_SetLayer(void);
void Func_020020d0(void);
void Func_02002144(void);
void Func_020024c8(void);
void Func_020024f0(void);

void Func_0200253c(void);

void Func_020025a4(void);
void Func_020025b0(void);

void Func_020025bc(void);
void Func_020025dc(void);
void Func_0200261c(void);
void Func_0200265c(void);

void Func_0200267c(void);
void Func_0200274c(void);

void Func_02003620(void);
void Func_0200367c(void);

extern u32 gData_020040d4[];
extern u32 gData_02004198[];
extern u32 gData_02004640[];
extern s16 gData_020046a4[];
extern s16 gData_020046ba[];

void Func_020034fc(void);

void Field_TryJumpForward();

void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);

void Func_02001c4c(void);

struct Actor;

struct ActionScript {
  u32 words[9];
};

struct IsletsMapWork {
  u8 unk000[0x190];
  s32 value190;
  s32 value194;
};

void Actor_Release(struct Actor *object);
void FieldPsynergy_GreyMindReadPalette(void);
void FieldPsynergy_RestoreMindReadPalette(void);
void FieldCamera_WaitForArrival(void);
void TileCopies_CopyTouchedCells();

s32 Func_02003684(s32 id);

void Actor_DisableById(s32 actor);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"
#ifndef TLA_ISLETS_H
#define TLA_ISLETS_H

#include "types.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "map.h"
#include "scene_tables.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "system/frame.h"
#include "system/vector3.h"

struct IsletsEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[0x50];
  s32 field1ac;
};

void Actor_SelectAnimation(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const void *script);
void Func_020034ec(void);

void Func_02001cac(void);
void Func_02001cfc(void);

extern u32 gSmokeScript[];
extern struct Trigger gIslets_Triggers[];
extern struct Trigger gIslets_Room1Triggers[];
extern struct Trigger gIslets_Room2Triggers[];
extern struct Trigger gIslets_Room3Triggers[];
extern struct Trigger gIslets_Room4Triggers[];
extern struct Trigger gIslets_Room5Triggers[];
extern u16 gSceneState[];
extern char SceneId_Ov674Room1;
extern char SceneId_Ov674Room2;
extern char SceneId_Ov674Room3;
extern char SceneId_Ov674Room4;
extern char SceneId_Ov674Room5;

void Islets_InitializeRoom1(void);
void Islets_InitializeRoom2(void);
void Islets_InitializeRoom3(void);
void Islets_InitializeRoom4(void);
void Islets_InitializeRoom5(void);

struct IsletsMapFlags {
  u8 unk00[0x17];
  u8 unused : 2;
  u8 mode : 2;
};

struct IsletsExit {
  s32 value;
  struct {
    u16 first;
    u16 second;
  } pair;
};

struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

extern struct ActorPlacement gIslets_Placements[];
extern struct ActorPlacement gIslets_Room1Placements[];
extern struct ActorPlacement gIslets_Room2Placements[];
extern struct ActorPlacement gIslets_Room3Placements[];
extern struct ActorPlacement gIslets_Room4Placements[];
extern struct ActorPlacement gIslets_Room5Placements[];
extern s32 gData_02005190;
extern s32 gData_02005194;
extern s32 gData_02005198;
extern s32 gData_0200519c;
extern s32 gData_02004028[];
/* VIEW(first): routine gData_020046bc as this part declared it; the joined file
 * has another; tried as the definition's extern u32 gData_020046bc[], it does
 * not compile in tla-ja: games/tla/src/scene/islets.c:1569: `gData_020046bc'
 * undeclared (first use in this function). */
extern const struct IsletsExit
    gData_020046bc__first[] __asm__("gData_020046bc");
#define gData_020046bc gData_020046bc__first
extern u16 gData_02004070[];

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value);
void Func_020021d0(void);
void Func_02002bd8(void);
void Leader_PushObjectAhead(void);
s32 MapGrid_CheckCollision(struct Actor *object, struct Vector3 *position);
struct Actor *Actor_FindAtCell(struct Vector3 *position, s32 mode);
s32 Func_020037d8(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void Func_02001f58(void);
void Func_02001278(struct Actor *object);
void Actor_EmitSmoke(struct Actor *object);
void Func_020034d8(void);
void Actor_MoveToObject(s32 actor, s32 target);
void FieldPsynergy_StopMindReadWave(void);

#endif

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gIslets_Room1Placements;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gIslets_Room2Placements;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gIslets_Room3Placements;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gIslets_Room4Placements;
  if (scene == (s32)&SceneId_Ov674Room5) {
    if (Event_GetFlag(0xaa5))
      gIslets_Room5Placements[1].talkFacing = 1;
    return gIslets_Room5Placements;
  }
  return gIslets_Placements;
}

void Func_020000d8(void) {
  struct PushTarget target;
  struct Actor *object = Actor_Get(10);

  if (Func_020037d8(&target)) {
    PushTarget_Push(target);
    System_Wait(5);
    object->gravity = 0x8000;
    Actor_SetAnimation(10, 3);
    Actor_SetDestinationOffset(10, 0, 8);
    FieldEvent_Wait(22);
    Event_PlaySound(SE_LOG_FALL);
    Actor_SetAnimation(10, 8);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->velocityY = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 8, 4, 1, 9, 9);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
    Event_SetFlag(0xa2c);
  }
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gIslets_Room1Triggers;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gIslets_Room2Triggers;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gIslets_Room3Triggers;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gIslets_Room4Triggers;
  if (scene == (s32)&SceneId_Ov674Room5)
    return gIslets_Room5Triggers;
  return gIslets_Triggers;
}

#include "text/msg_ids.h"

void Func_02000210(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 != 0) {
    result1 = Event_GetFlag(0x200);
    if (result1 != 0) {
      Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPen);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen2);
    }
    Event_ShowMessage(9, 0);
  } else {
    FieldActor_Stop(9);
    System_Wait(1);
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgPenPen3);
    Event_ShowMessage(9, 0);
    FieldActor_WatchAndSetScript(9, 19);
  }
  Event_End();
  return;
}

void Func_020002ac(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 != 0) {
    result1 = Event_GetFlag(0x200);
    if (result1 != 0) {
      FieldEvent_SetMessageId((s32)MsgThereIsNothing);
    } else {
      FieldEvent_SetMessageId((s32)MsgIMSoHappy);
    }
    Event_ShowMessage(9, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgMyDarlingPengulina);
    Event_ShowMessage(9, 0);
  }
  Event_End();
  return;
}

void Func_0200030c(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 != 0) {
    result1 = Event_GetFlag(0x200);
    if (result1 != 0) {
      Actor_FaceActor(19, gMainRoutineWork.currentOwner, 0);
      FieldEvent_SetMessageId((s32)MsgPenPenPehnnnn);
    } else {
      FieldEvent_SetMessageId((s32)MsgPenPen2);
    }
    Event_ShowMessage(19, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgPenPenPehnnn);
    Event_ShowMessage(19, 0);
  }
  Event_End();
  return;
}

void Func_02000380(void) {
  s32 result0;
  s32 result1;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa2);
  if (result0 != 0) {
    result1 = Event_GetFlag(0x200);
    if (result1 != 0) {
      FieldEvent_SetMessageId((s32)MsgPengusIsPretty);
    } else {
      FieldEvent_SetMessageId((s32)MsgIMSoGlad);
    }
    Event_ShowMessage(19, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgOhhMyLeg);
    Event_ShowMessage(19, 0);
  }
  return;
}

DEFINE_LEADER_IS_FACING_NORTH(Leader_IsFacingNorth)

void Islets_TalkToHealer(void) {
  s32 result0;

  result0 = Leader_IsFacingNorth();
  if (result0 != 0) {
    Sanctum_Run(21);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgIAmHere);
    Event_ShowMessage(21, 0);
    Event_End();
  }
  return;
}

void Func_02000440(void) {
  s32 result0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0x214);
  if (result0 == 0) {
    FieldEvent_SetMessageId((s32)MsgWeLikeThis);
    Event_ShowMessage(8, 0);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    FieldActor_Jump(9, 4, 10);
    Event_ShowMessage(9, 0);
    Actor_SetAttachedEffect(9, 0x102);
    FieldEvent_Wait(20);
    Event_SetFlag(0x214);
  }
  Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgLookWeRe);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Event_End();
  return;
}

void Func_020004f4(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgTheEasternSea);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(9, 0);
  Event_End();
  return;
}

void Func_02000520(void) {
  s32 result0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  result0 = Event_GetFlag(0xaa4);
  if (result0 != 0) {
    FieldActor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
  }
  FieldEvent_SetMessageId((s32)MsgChirpChirp2);
  Event_ShowMessage(10, 0);
  Event_End();
  return;
}

/// Object 10's talk, and its exchange of the party's item 0x1c4 for item 0x1c5.
void Func_02000568(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *spawned;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(0x1c4) == -1) {
    if (Event_GetFlag(0xaa4)) {
      FieldActor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      FieldEvent_SetMessageId((s32)MsgWhatDoYou);
    } else {
      FieldEvent_SetMessageId((s32)MsgILoveFeeling);
    }
    Event_ShowMessage(10, 0);
  } else {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    object = Actor_Get(10);
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldActor_Jump(10, 4, 10);
    FieldEvent_SetMessageId((s32)MsgOhMyThat);
    Event_ShowMessage(10, 0);
    Actor_SetAnimation(10, 4);
    FieldActor_SetPositionAndResetMotion(object, (leader->x + object->x) / 2,
                                         leader->y,
                                         (leader->z + object->z) / 2);
    System_Wait(20);
    FieldActor_SetPositionAndResetMotion(object, PIXELS(120), PIXELS(128),
                                         PIXELS(312));
    Actor_SetAnimation(10, 5);
    System_Wait(10);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    spawned = FieldActor_Create(0x1e9, leader->x, 0x980000, leader->z);
    if (spawned != 0) {
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
      FieldActor_SetItemIcon(spawned, 0x1c5);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->motionFlags = 0;
    }
    FieldEvent_Wait(10);
    FieldPsynergy_RestoreMindReadPalette();
    Message_ShowCentered((s32)MsgOhMyThat + 1, 1);
    PartyInventory_Remove(0x1c4);
    Party_GiveItem(0x1c5, 0);
    FieldPsynergy_GreyMindReadPalette();
    if (spawned != 0) {
      Actor_Release(spawned);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    }
    Actor_FaceDirectionAndWait(10, FACING_EAST);
    Event_SetFlag(0xaa4);
  }
  Event_End();
}

// The Islets' trade of item 453 with object 10. With the item in the party,
// if flag 513 is set (asked before) the camera moves and object 10 asks again
// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
// steps up and shows it (object type 489, its picture set to the item by
// Actor_SetItemIcon) and object 10 asks. A yes hands the item across step by
// step, takes item 453 and gives item 454 (object 10 hands it back over the
// leader's head), sets flag 0xaa3 and counts the answer in the event work's
// halfword 0x1c4 the first time; a no sets flag 513. Without the item, object
// 10 says MsgYupANice after the trade or MsgIVeBeenFeeling before it.

// zero is set only inside the first spawn check: set where CSE can see it, the
// store reuses the second answer, which then stays in r7. HAND_BACK is a plain
// block: a do-while(0) ends CSE's block and loses the owner address and
// message bases in the refusals.

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   Event_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

/// Object 10 walks round and hands item 454 back over the leader's head.
#define HAND_BACK(leader, given, off)                                          \
  {                                                                            \
    Actor_SetSpeed(10, 0xcccc, 0x6666);                                        \
    Actor_WalkToAndWait(10, 0x80, 164);                                        \
    Actor_FaceDirectionById(10, 0x8000);                                       \
    Event_Wait(20);                                                            \
    Actor_SetSpeed(10, 0x19999, 0xcccc);                                       \
    STEP(10, 108, 168);                                                        \
    given =                                                                    \
        FieldActor_Create(489, leader->x, leader->y + 0x180000, leader->z);    \
    if (given != 0) {                                                          \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 28);                    \
      given->motionFlags = off;                                                \
      Actor_SetItemIcon(given, 454);                                           \
      Event_Wait(20);                                                          \
    }                                                                          \
    STEP(10, 116, 164);                                                        \
    FieldPsynergy_RestoreMindReadPalette();                                    \
    PartyInventory_Remove(453);                                                \
    PartyInventory_GiveItem(454, 0);                                           \
    if (given != 0) {                                                          \
      Actor_Release(given);                                                    \
      Actor_SetModeById(gMainRoutineWork.currentOwner, 1);                     \
    }                                                                          \
    FieldPsynergy_GreyMindReadPalette();                                       \
    GameFlag_Set(0xaa3);                                                       \
  }

/// The Islets' trade of item 453 with object 10. With the item in the party, if
/// flag 513 is set (asked before) the camera moves and object 10 asks again
/// (MsgAreYouGiving); otherwise it notices the item (MsgOhThatRed), the leader
/// steps up and shows it (object type 489, its picture set to the item by
/// Actor_SetItemIcon) and object 10 asks. A yes hands the item across step by
/// step, takes item 453 and gives item 454 (object 10 hands it back over the
/// leader's head), sets flag 0xaa3 and counts the answer in the event work's
/// halfword 0x1c4 the first time; a no sets flag 513. Without the item, object
/// 10 says MsgYupANice after the trade or MsgIVeBeenFeeling before it.
void Func_020006f4(void) {
  struct Actor *item, *leader, *given;
  const u8 *asked, *noticed;
  /* FAKEMATCH(temporary): zero wrapped in a struct of one u16, so it stays a
   * HImode pseudo and its 0 is loaded from the pool; as s32 zero, 25 of 687
   * instructions differ in tla-en. */
  struct {
    u16 v;
  } zero;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(453) != -1) {
    if (Event_GetFlag(513)) {
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_FaceActorById(10, gMainRoutineWork.currentOwner, 0);
      asked = MsgAreYouGiving;
      Event_SetMessageId(asked);
      Event_ShowMessageAndWait(10, 0);
      FieldCamera_WaitForArrival();
      FieldPsynergy_RestoreMindReadPalette();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
        Actor_FaceDirectionById(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        item = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
        // Default: the object handed back is set for real in HAND_BACK.
        given = 0;
        if (item != 0) {
          Actor_SetItemIcon(item, 453);
          Actor_SetSpriteFlags(item, 0);
          item->motionFlags = 0;
          item->unk5c[0] = 1;
        }
        Event_Wait(10);
        FieldPsynergy_GreyMindReadPalette();
        Actor_Stop(10);
        Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
        Actor_ShowEmoteById(10, 264, 20);
        Event_SetMessageId(asked + 1);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(leader, given, 0)
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(10, 265, 20);
        Event_SetMessageId(asked + 4);
        Event_ShowMessageAndWait(0x800a, 0);
      }
    } else {
      noticed = MsgOhThatRed;
      Event_SetMessageId(noticed);
      Event_ShowMessageAndWait(10, 0);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_SetSpeed(0xcccc, 0x1999);
      FieldCamera_MoveTo(0x8c0000, -1, 0xbe0000, 1);
      Actor_FaceDirection(10, 0x5000, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 120, 190);
      Actor_FaceDirectionById(gMainRoutineWork.currentOwner, 0);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      item = FieldActor_Create(489, 0x8c0000, 0, 0xbe0000);
      if (item != 0) {
        Actor_SetItemIcon(item, 453);
        Actor_SetSpriteFlags(item, 0);
        item->motionFlags = 0;
        zero.v = 0;
        item->unk5c[0] = 1;
      }
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Actor_ArmCallback(10, 0x3000, 0);
      Actor_Jump(10, 4, 10);
      Event_ShowMessageAndWait(10, 0);
      Actor_WatchAndSetScript(10, gMainRoutineWork.currentOwner);
      STEP(gMainRoutineWork.currentOwner, 132, 190);
      FieldActor_SetPositionAndResetMotion(item, 0xc30000, 0, 0x730000);
      STEP(gMainRoutineWork.currentOwner, 120, 190);
      Event_Wait(20);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 98, 168);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
      Actor_SetPositionAndResetMotion(item, 0x680000, 0x180000, 0xa00000);
      Event_Wait(10);
      FieldPsynergy_RestoreMindReadPalette();
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
      FieldMessage_Show(noticed + 2, 1);
      (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
      FieldPsynergy_GreyMindReadPalette();
      Actor_Jump(10, 6, 10);
      Event_SetMessageId(noticed + 3);
      Event_ShowMessageAndWait(10, 0);
      Actor_Stop(10);
      Actor_FaceDirectionById(gMainRoutineWork.currentOwner, 0);
      FieldPsynergy_RestoreMindReadPalette();
      if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                                0) == 0) {
        item = Actor_GetById(gMainRoutineWork.currentOwner);
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(10, 264, 20);
        Event_SetMessageId(noticed + 4);
        Event_ShowMessageAndWait(0x800a, 0);
        HAND_BACK(item, item, zero.v)
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(10, 265, 20);
        Event_SetMessageId(noticed + 7);
        Event_ShowMessageAndWait(0x800a, 0);
        Actor_FaceDirectionById(gMainRoutineWork.currentOwner, 0xc000);
        Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
        if (item != 0)
          Actor_Release(item);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
        Event_SetFlag(513);
      }
    }
  } else {
    if (Event_GetFlag(0xaa3))
      Event_SetMessageId(MsgYupANice);
    else
      Event_SetMessageId(MsgIVeBeenFeeling);
    Event_ShowMessageAndWait(10, 0);
  }
  Actor_SetModeById(10, 4);
  Event_End();
}
#undef STEP

struct Actor *Func_02000da0(struct Actor *object, s32 value) {
  struct Actor *spawned;

  spawned = FieldActor_Create(489, object->x, object->y + 0x180000, object->z);
  if (spawned != 0) {
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
    spawned->motionFlags = 0;
    FieldActor_SetItemIcon(spawned, value);
    Actor_SetSpriteFlags(spawned, 0);
    spawned->unk5c[0] = 1;
  }
  return spawned;
}

DEFINE_ACTOR_EMIT_SMOKE(Actor_EmitSmoke, 3, Actor_SelectAnimation, gSmokeScript)

extern s32 gData_020043f8[];
extern s32 gData_02004300[];

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1, Actor_WalkToAndWait(actor, x, z),  \
   FieldEvent_Wait(1), Actor_GetById(actor)->actionFlags |= 1)

void Func_02000e38(void) {
  struct Actor *leader, *trader, *item, *gift;
  s32 right;
  s32 giftItem;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (FieldPartyInventory_FindMember(454) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    trader = Actor_GetById(11);
    // Default: the trader's zero update stores read it.
    gift = 0;
    right = 0;
    Actor_FaceActor(11, gMainRoutineWork.currentOwner, 0);
    trader->update = Actor_EmitSmoke;
    FieldActor_Jump(11, 4, 10);
    trader->update = 0;
    Event_SetMessageId(MsgSniffSniffI);
    Event_ShowMessage(11, 0);
    if (leader->x > trader->x)
      right = 1;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_SetSpeed(11, 0x19999, 0xcccc);
    if (right) {
      Actor_FaceDirection(11, 0, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Actor_EmitSmoke;
      Actor_RunScriptAndWait(11, gData_020043f8);
      trader->update = 0;
    } else {
      Actor_FaceDirection(11, 0x8000, 0);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 856);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      item = (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, 454);
      trader->update = Actor_EmitSmoke;
      Actor_RunScriptAndWait(11, gData_02004300);
      trader->update = 0;
    }
    Event_ShowMessage(11, 0);
    FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
    if (item != 0) {
      void (*hook2)(void);
      item->collisionFlags = 9;
      FieldActor_SetPositionAndResetMotion(item, 0x680000, 0, 0x3480000);
      hook2 = Actor_EmitSmoke;
      if (right) {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x120, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 296, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 292, 844);
      } else {
        leader->update = hook2;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
        leader->update = hook2;
        STEP(gMainRoutineWork.currentOwner, 0x110, 840);
        FieldActor_SetPositionAndResetMotion(item, 0x1180000, 0, 0x3480000);
        STEP(gMainRoutineWork.currentOwner, 264, 840);
        leader->update = 0;
        FieldActor_Stop(gMainRoutineWork.currentOwner);
        System_Wait(1);
        trader->update = hook2;
        STEP(11, 268, 844);
      }
    }
    giftItem = 455;
    gift =
        (struct Actor *)Value2((s32 (*)())Func_02000da0, (s32)leader, giftItem);
    STEP(11, 280, 856);
    trader->update = 0;
    FieldPsynergy_RestoreMindReadPalette();
    Message_ShowCentered((s32)MsgTradedHisMilk, 1);
    PartyInventory_Remove(454);
    Party_GiveItem(giftItem, 0);
    FieldPsynergy_GreyMindReadPalette();
    Actor_GetById(gMainRoutineWork.currentOwner)->facing = 0x4000;
    if (gift != 0) {
      Actor_Release(gift);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    }
    Actor_FaceDirectionAndWait(11, 0xc000);
    Event_SetFlag(0xaa1);
    Event_SetFlag(534);
  } else {
    if (Event_GetFlag(0xaa1)) {
      FieldActor_WatchAndSetScript(11, gMainRoutineWork.currentOwner);
      if (Event_GetFlag(534))
        Event_SetMessageId(MsgIDonTLike);
      else
        Event_SetMessageId(MsgIDonTWantTo);
    } else {
      Event_SetMessageId(MsgTheWaterHere);
    }
    Event_ShowMessage(11, 0);
  }
  Event_End();
}

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_Get(9);

  FieldActor_SetPositionAndResetMotion(object, source->x,
                                       source->y + PIXELS(16), source->z);
  object->facing = source->facing;
}

#if !TLA_PATCH_1
#define ISLETS_GIFT_TRANSITION 521
#else
#define ISLETS_GIFT_TRANSITION 513
#endif

/* VIEW(first): routine gData_02004004 as this part declared it; the joined file
 * has another; tried as the definition's extern u32 gData_02004004[], it does
 * not compile in tla-ja: games/tla/src/scene/islets.c:851: `gData_02004004'
 * undeclared (first use in this function). */
extern struct ActionScript gData_02004004__first __asm__("gData_02004004");
#define gData_02004004 gData_02004004__first

void Func_0200129c(struct Actor *gift) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *guide = Actor_Get(9);
  struct ActionScript script = gData_02004004;

  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_ShowEmote(9, 264, 20);
  guide->update = Actor_EmitSmoke;
  Actor_RunScriptAndWait(9, &script);
  guide->update = 0;
  FieldEvent_SetMessageId(MsgAtLongLast);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, 0x5000);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  guide->update = Actor_EmitSmoke;
  Actor_WalkToAndWait(9, 296, 452);
  guide->update = 0;
  Actor_FaceDirection(9, 0x5000, 0);
  gift->speedLimit = 0x19999;
  gift->acceleration = 0xcccc;
  gift->update = Actor_EmitSmoke;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1160000, 0, 0x1c40000);
  FieldActor_WaitForMove(gift);
  Actor_SelectAnimation(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(455);
  Event_ShowMessage(9, 0);
  ((u8 *)HeapSlot_AllocEwram(88, 664))[661] = 99;
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  leader->update = Actor_EmitSmoke;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 312, 446);
  leader->update = 0;
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
  leader->update = Func_02001278;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  gift->speedLimit = 0xcccc;
  gift->acceleration = 0x6666;
  guide->update = Actor_EmitSmoke;
  Actor_SetDestination(9, 288, 538);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Func_020034ec();
  gift->update = Actor_EmitSmoke;
  Actor_SelectAnimation(gift, 2);
  FieldActor_SetMoveTarget(gift, 0x1200000, 0, 0x21a0000);
  FieldEvent_Wait(20);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  Event_SetFlag(0xaa5);
}

// The value stored at the event work's 0x1ac: 521 in the Japanese release,
// 513 from patch 1.
#if TLA_PATCH_1
#define STORED_AT_1AC 513
#else
#define STORED_AT_1AC 521
#endif

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = FieldActor_Create(163, 0x1280000, 0, 0x1a80000);                       \
    b = FieldActor_Create(14, 0x1280000, 0, 0x1a80000);                        \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSpriteFlags(a, 0);                                              \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = 0x20000;                                                     \
      Actor_SetSpriteFlags(b, 0);                                              \
      Actor_SelectAnimation(b, 1);                                             \
      Actor_StartScript(b, gSmokeScript);                                      \
    }                                                                          \
  }

void Func_020014e0(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (PartyInventory_FindMember(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Get(513) != 0) {
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      /* FAKEMATCH(temporary): the form kept; as leader->update =
       * Actor_EmitSmoke, 94 of 569 instructions differ in tla-en. */
      a = (struct Actor *)Actor_EmitSmoke;
      leader->update = (void (*)(void))a;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      message = MsgOhYouRe;
      Event_SetMessageId(message);
      Event_ShowMessage(9, 0);
      FieldPsynergy_RestoreMindReadPalette();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        leader->update = (void (*)(void))a;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 0);
        Func_0200129c(a);
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Event_SetMessageId(message - 1);
        goto refused;
      }
    } else {
      Event_SetMessageId(MsgIMSoVery);
      Event_ShowMessage(9, 0);
      Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 262, 20);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
      leader->update = Actor_EmitSmoke;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      leader->update = Actor_EmitSmoke;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x4000, 0);
      Actor_ShowEmoteById(9, 256, 20);
      Actor_FaceDirectionAndWait(9, 0xb000);
      other = Actor_GetById(9);
      other->update = Actor_EmitSmoke;
      FieldActor_Jump(9, 4, 20);
      other->update = 0;
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0xd000);
      Event_ShowMessage(9, 0);
      FieldPsynergy_RestoreMindReadPalette();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        Func_0200129c(a);
      } else {
        FieldPsynergy_GreyMindReadPalette();
        Actor_ShowEmoteById(9, 265, 0);
        Event_ShowMessage(9, 0);
        Actor_FaceDirection(9, 0x5000, 0);
        Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Actor_EmitSmoke;
        Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 302, 424);
        leader->update = 0;
        if (a != 0)
          Actor_Release(a);
        Event_SetFlag(513);
      }
    }
  } else if (GameFlag_Get(0xaa5)) {
    Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId(MsgWantToGo);
    Event_ShowMessage(9, 0);
    FieldPsynergy_RestoreMindReadPalette();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      FieldPsynergy_GreyMindReadPalette();
      (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
      Event_ShowMessage(9, 0);
      ((u8 *)HeapSlot_AllocEwram(88, 664))[0x295] = 99;
      FieldActor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
      Actor_FaceDirectionAndWait(9, 0x5000);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 296, 452);
      leader->update = Func_02001278;
      FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner),
                                15);
      Actor_SetAnimation(9, 3);
      FieldEvent_Wait(20);
      Event_ShowMessage(9, 0);
      FieldPsynergy_RestoreMindReadPalette();
      Actor_SetSpeed(9, 0xcccc, 0x6666);
      Actor_SetSpeed(10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Actor_EmitSmoke;
      Actor_SetDestination(9, 288, 538);
      FieldEvent_Wait(40);
      FieldPsynergy_RestoreMindReadPalette();
      Func_020034ec();
      Actor_GetById(10)->update = Actor_EmitSmoke;
      Actor_WalkTo(10, 288, 538);
      FieldEvent_Wait(20);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
          STORED_AT_1AC;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      FieldPsynergy_GreyMindReadPalette();
      FieldEvent_SetExitRequest(3);
    } else {
      FieldPsynergy_GreyMindReadPalette();
    refused:
      Event_ShowMessage(9, 0);
      Actor_FaceDirectionAndWait(9, 0x5000);
    }
  } else {
    Event_SetMessageId(MsgIMSoVeryVery);
    Event_ShowMessage(9, 0);
  }
  Event_End();
}

extern u32 gData_020045a4[];
extern u32 gData_020045f0[];

void Func_02001a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);
  void (*update)(void);
  s32 result;
  u8 *done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgYouAllSet);
  Event_ShowMessage(9, 0);
  FieldPsynergy_RestoreMindReadPalette();
  result =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result == 0) {
    FieldPsynergy_GreyMindReadPalette();
    Event_ShowMessage(9, 0);
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(88, 0x298))
        ->unk295 = 99;
    FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 768);
    leader->update = (void (*)(void))Func_02001278;
    FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
    Actor_SetAnimation(9, 3);
    FieldEvent_Wait(20);
    FieldPsynergy_RestoreMindReadPalette();
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_SetSpeed(10, 0x19999, 0xcccc);
    update = (void (*)(void))Actor_EmitSmoke;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetScript(9, gData_020045a4);
    FieldEvent_Wait(30);
    Actor_Get(10)->update = update;
    Actor_SetScript(10, gData_020045f0);
    do {
      System_Wait(1);
    } while (*done == 0);
    Func_020034ec();
    System_Wait(1);
    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetExitRequest(4);
  } else {
    FieldPsynergy_GreyMindReadPalette();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  }
  Event_End();
}

void Func_02001bf4(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgMyPopS);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_End();
}

void Func_02001c38(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  work->field15a = 1;
}

void Func_02001c4c(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y < 0x280000) {
    Actor_Get(11)->collisionFlags |= 8;
    Actor_Get(12)->collisionFlags |= 8;
  } else {
    Actor_Get(11)->collisionFlags &= ~8;
    Actor_Get(12)->collisionFlags &= ~8;
  }
}

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

/// Applies BG1 horizontal line scrolling with HBlank DMA.
void Func_02001cac(void) {
  s16 *lines = gData_02004f10[gData_02004f04];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH(barrier): an asm reading the first line keeps its load before the
   * register address is built; as no asm, 6 of 34 instructions differ in
   * tla-en. */
  __asm__("" : : "r"(first));
  scroll = &REG_BG1HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= 0xc5ff; // Clear start timing and repeat.
  dma[5] &= 0x7fff; // Stop.
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}

void Func_02001cfc(void) {
  s16 *layer = (s16 *)&Ram_HeapSlots->mapWork->layers[2];
  s16 *lines = gData_02004f10[gData_02004f04 ^ 1];
  s32 v[2];
  s32 *count = &v[1];
  s32 value;
  s32 sine;

  for (*count = 0; *count != 160; (*count)++) {
    /* FAKEMATCH(dead): the sine's call and store in a once-only block; as plain
     * statements, 2 of 65 instructions differ in tla-en. */
    do {
      sine = Math_Sin((*count + *(u8 *)&gFrameCount + layer[3]) << 9);
      v[0] = sine;
    } while (0);
    value = layer[1] + (sine >> 15);
    value += *count % 3;
    *lines++ = value - 1;
  }
  gData_02004f04 ^= 1;
}

void Func_02001d90(void) {
  Task_AddFrame(Func_02001cfc, 0x480);
  Task_AddFrame(Func_02001cac, 0xc80);
}

/// When object 19 stands in its target area, actor 9 and the leader react and
/// the party is given item 0x1c4.
void Func_02001db0(void) {
  struct Actor *object = Actor_GetById(19);
  struct Actor *leader;
  struct Actor *item;
  s32 z;

  if (object->x < 0xe40000 || object->x > 0xec0000)
    return;
  z = object->z;
  if (z < 0xa40000)
    return;
  if (z > 0xac0000)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(19, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 216, 184);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgThankYouSo);
  FieldActor_Jump(9, 2, 10);
  FieldActor_Jump(9, 4, 10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 10);
  Event_ShowMessage(9, 0);
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 222, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_GetById(9)->actionFlags &= ~1;
  Actor_WalkToAndWait(9, 232, 186);
  FieldEvent_Wait(1);
  Actor_GetById(9)->actionFlags |= 1;
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  item = Actor_GetById(20);
  item->motionFlags = 0;
  FieldActor_SetItemIcon(item, 0x1c4);
  FieldActor_SetPositionAndResetMotion(item, leader->x, leader->y + PIXELS(24),
                                       leader->z);
  FieldEvent_Wait(20);
  FieldPsynergy_RestoreMindReadPalette();
  Party_GiveItem(0x1c4, 0);
  FieldPsynergy_GreyMindReadPalette();
  Actor_SetPosition(20, 0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Event_SetFlag(0xaa2);
  Event_End();
}

void Func_02001f58(void) {
  if ((gFrameCount & 15) == 0)
    Event_PlaySound(SE_FOOTSTEP);
}

void Func_02001f74(void) {
  struct Actor *object = Actor_Get(10);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(10);
  System_Wait(1);
  object->motionFlags = 0;
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  FieldActor_SetPositionAndResetMotion(object, PIXELS(268), PIXELS(64),
                                       PIXELS(390));
  System_Wait(1);
  object->terrainHeight = PIXELS(128);
  System_Wait(1);
  object->y = PIXELS(160);
  System_Wait(1);
  FieldTask_AddFrame(Func_02001f58, 0x480);
  FieldActor_SetMoveTarget(object, PIXELS(132), PIXELS(128), PIXELS(320));
  FieldActor_WaitForMove(object);
  FieldTask_Remove(Func_02001f58);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 3;
  FieldActor_SetSpritePriority(10, 2);
  Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(10, 4, 0);
  Actor_MoveToAndWait(10, 120, 312);
  Actor_SetAnimation(10, 5);
  object->motionFlags = 0;
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  Event_SetFlag(0x215);
  System_Wait(1);
  Event_End();
}

void Func_0200208c(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 1);
  Event_End();
  return;
}

DEFINE_LEADER_SET_LAYER(Leader_SetLayer, 2)

DEFINE_LEADER_SET_LAYER(Func_020020d0, 0)

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells0 = map->layers[0].cells;
  struct MapCell *cells2 = map->layers[2].cells;
  s32 index = x / 0x100000 + (z / 0x100000) * 128;

  if (layers & 1) {
    cells0 += index;
    cells0->collisionCode = value;
  }
  if (layers & 2) {
    cells2 += index;
    cells2->collisionCode = value;
  }
}

/// Records objects 12 and 13's positions; with them on cells (7, 29) and
/// (7, 28), clears layer 2's collision under object 12, and with them on
/// (12, 25) and (12, 26), under object 13.
void Func_02002144(void) {
  struct Actor *object;
  s32 *x0;
  s32 *z0;
  s32 x1;
  s32 z1;

  object = Actor_Get(12);
  x0 = &gData_0200519c;
  z0 = &gData_02005194;
  *x0 = object->x;
  *z0 = object->z;
  object = Actor_Get(13);
  gData_02005198 = x1 = object->x;
  {
    /* FAKEMATCH(pin): object 13's z in r2, as the native keeps it for its
     * test; as a plain local, 31 of 58 instructions differ in tla-en. */
    register s32 z asm("r2");

    z = object->z;
    gData_02005190 = z;
    z1 = z;
  }
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Func_020020ec(*x0, *z0, 2, 0);
  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 25 &&
      gData_02005198 >> 20 == 12 && gData_02005190 >> 20 == 26)
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
}

void Func_020021d0(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);
  s32 firstX = first->x >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondZ = second->z >> 20;
  s32 both;

  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 24) {
    Func_020020ec(gData_0200519c, gData_02005194, 1, 21);
    Func_020020ec(gData_0200519c, gData_02005194, 2, 0);
  } else {
    s32 *z = &gData_02005194;

    Func_020020ec(gData_0200519c, *z, 3, 0);
  }
  if (gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 27) {
    Func_020020ec(gData_02005198, gData_02005190, 1, 21);
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
  } else {
    s32 *z = &gData_02005190;

    Func_020020ec(gData_02005198, *z, 3, 0);
  }
  Func_020020ec(first->x, first->z, 3, 255);
  Func_020020ec(second->x, second->z, 3, 255);
  if (!Event_GetFlag(0x210) && firstX == 12 && firstZ == 26) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (secondX == 12 && secondZ == 26) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    first->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(SE_RATTLE);
    System_Wait(10);
    first->motionFlags = 0;
    Event_SetFlag(0x210);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 12, 26);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Event_End();
  }
  if (!Event_GetFlag(0x211) && secondX == 7 && secondZ == 29) {
    both = 0;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (firstX == 7 && firstZ == 29) {
      MapGrid_CopyCellAttributes(7, 2, 1, 1, firstX, firstZ);
      both = 1;
    }
    second->motionFlags = 3;
    System_Wait(10);
    Event_PlaySound(SE_RATTLE);
    System_Wait(10);
    second->motionFlags = 0;
    Event_SetFlag(0x211);
    if (both) {
      Func_020020ec(first->x, first->z, 3, 0);
      Func_020020ec(second->x, second->z, 3, 0);
      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, 29);
      Event_SetFlag(0x212);
      ((u8 *)first)[0x62] = 1;
      ((u8 *)second)[0x62] = 1;
    }
    Event_End();
  }
}

void Func_02002430(void) {
  struct Actor *first = Actor_Get(12);
  struct Actor *second = Actor_Get(13);

  gData_0200519c = first->x;
  gData_02005194 = first->z;
  gData_02005198 = second->x;
  gData_02005190 = second->z;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gData_0200519c >> 20 == 7 && gData_02005194 >> 20 == 29 &&
      gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 28) {
    ((u8 *)first)[0x62] = 0;
    ((void (*)())Func_020020ec)(gData_0200519c, gData_02005194, 2);
  }
  Leader_PushObjectAhead();
  Func_020021d0();
  Event_End();
}

void Func_020024c8(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    object->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 280);
    object->motionFlags = 3;
    Event_End();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if ((u16)(*state + 0xfff6) > 200) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      object->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Event_End();
    }
  }
}

void Func_020025a4(void) {
  Event_SetFlag(0x218);
  return;
}

void Func_020025b0(void) {
  Event_ClearFlag(0x218);
  return;
}

// The following six are from patch 1.
#if TLA_PATCH_1
void Func_020025bc(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 4, 4, 65, 10, 1);
  return;
}

void Func_020025dc(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 2, 2, 71, 10, 1);
  return;
}

void Func_020025fc(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 1, 1, 68, 15, 1);
  return;
}

void Func_0200261c(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 2, 3, 75, 20, 1);
  return;
}

void Func_0200263c(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 4, 1, 78, 22, 1);
  return;
}

void Func_0200265c(void) {
  Call7((void (*)())TileCopies_CopyTouchedCells, 65, 0, 1, 1, 79, 23, 1);
  return;
}
#endif

void Func_0200267c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  s16 *exit;
  s32 index;

  object = Actor_Get(party->currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object,
                           ((8 - (object->x >> 16) % 16) << 16) + object->x,
                           object->y, object->z);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  exit = &work->exit;
  index = *exit;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gData_020046bc[index].value,
                       gData_020046bc[index].pair.first,
                       gData_020046bc[index].pair.second);
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(*exit);
}

void Func_0200274c(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_020034ec();
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002780(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;

  object = Actor_Get(party->currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(party->currentOwner, 2);
  Actor_SetSpeed(party->currentOwner, 0x8000, 0x4000);
  FieldActor_SetMoveTarget(object,
                           ((8 - (object->x >> 16) % 16) << 16) + object->x,
                           object->y, object->z);
  FieldActor_WaitForMove(object);
  object->facing = FACING_NORTH;
  Actor_SetScript(party->currentOwner, gData_02004028);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);

  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(273), PIXELS(520));
  System_Wait(1);
  object->facing = FACING_NORTH;
  owner->update = (void (*)(void))Func_02001278;
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0x9999, 0x4ccc);
  Actor_SetSpeed(10, 0x9999, 0x4ccc);
  Actor_Get(10)->update = (void (*)(void))Actor_EmitSmoke;
  object->update = (void (*)(void))Actor_EmitSmoke;
  Actor_WalkTo(10, 280, 468);
  Actor_MoveToAndWait(9, 296, 452);
  Actor_SetAnimation(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_Get(10)->update = 0;
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId((s32)MsgHereWeAre);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 312, 440);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_ShowMessage(9, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct Actor *)((s32)Actor_Get((id))))

extern u32 gData_02004508[];
extern u32 gData_02004554[];

/// The Islets' arrival by entrance 4: actor 9 joins the leader and actor 10
/// is placed, the screen fades in, both run their action scripts
/// (Actor_EmitSmoke updates) until actor 9's byte 0x63 is set, then actor 9
/// speaks (MsgWellHereWe) and the leader is launched up to (152, 742) and
/// turned; ends with Func_020034d8. The Japanese edition does not set the
/// event work's transition word 0x1ac.
void Func_020029c4(void) {
  struct Actor *leader = OBJECT(gMainRoutineWork.currentOwner);
  struct Actor *guide = OBJECT(9);
  u8 *done;

  Actor_MoveToObject(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(240), PIXELS(540));
  System_Wait(1);
  guide->facing = FACING_SOUTH;
  leader->update = Func_02001278;
  FieldActor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                            15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  OBJECT(10)->update = Actor_EmitSmoke;
  guide->update = Actor_EmitSmoke;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Actor_StartScript((s32)guide, gData_02004508);
  Actor_SetScript(10, gData_02004554);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  FieldActor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)), 0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Func_020034d8();
#if TLA_PATCH_1
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
#endif
}

s32 Scene_Initialize(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;
  s16 scene;

  work->field1ac = 0x100;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov674Room1)
    Islets_InitializeRoom1();
  else if (scene == (s32)&SceneId_Ov674Room2)
    Islets_InitializeRoom2();
  else if (scene == (s32)&SceneId_Ov674Room3)
    Islets_InitializeRoom3();
  else if (scene == (s32)&SceneId_Ov674Room4)
    Islets_InitializeRoom4();
  else if (scene == (s32)&SceneId_Ov674Room5)
    Islets_InitializeRoom5();
  return 0;
}

void Func_02002bd8(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y > 0x80000) {
    Actor_Get(9)->collisionFlags &= ~8;
    Actor_Get(10)->collisionFlags &= ~8;
  } else {
    Actor_Get(9)->collisionFlags |= 8;
    Actor_Get(10)->collisionFlags |= 8;
  }
}

void Islets_InitializeRoom1(void) {
  struct IsletsMapFlags *map = (struct IsletsMapFlags *)Ram_HeapSlots->mapWork;
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  map->mode = 1;
  Actor_SetSpriteFlags(Actor_Get(11), 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_Get(10)->collisionFlags |= 0x80;
  if (Event_GetFlag(0xaa1)) {
    spawned = FieldActor_Create(489, 0x1180000, 0, 0x3480000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, 454);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->collisionFlags = 9;
      spawned->unk5c[0] = 1;
    }
    Actor_Get(11)->facing = FACING_NORTH + FACING_STEP;
  } else {
    Actor_SetAnimation(11, 5);
  }
  Task_AddFrame(Func_02002bd8, 0x480);
}

void Func_02002d0c(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSpriteFlags(object, 0);
  object->unk5c[0] = 3;
  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x2666;
  object->speedLimit = 0x4ccc;
}

void Func_02002d48(s32 id, s32 column, s32 row) {
  struct Actor *object = Actor_Get(id);
  s32 x = (column * 32 + object->timer) << 16;
  s32 z = (row * 32 + (s16)object->unk66) << 16;

  FieldActor_SetMoveTarget(object, x, object->y, z);
}

// The Islets' marching guards (objects 11 to 18): while the event work is busy
// or flag 0x218 is set they stand still; once the controlled character drops
// below height -12 they stand and the event work's counter 0x16a becomes 200;
// otherwise, for each frame owed (gSceneState's halfword 2) while no event
// runs, a 260-frame routine sets them walking with their facing steps
// (Func_02002d48) at 0, still at 100 and 220, and walking in place at 130.
void Func_02002d7c(void) {
  u16 *step = (u16 *)((u8 *)gSceneState);
  u8 *work = Ram_HeapSlots->eventWork;
  u16 *owed = (u16 *)(((u8 *)gSceneState) + 2);
  s32 i;
  s32 count;

  if (*(s16 *)(work + 0x166) != 0 || GameFlag_Get(0x218)) {
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    return;
  }
  for (i = 11; i <= 18; i++)
    Actor_GetById(i)->movementState = 0;
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0xc0000) {
    work = Ram_HeapSlots->eventWork;
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    {
      s16 *countdown = (s16 *)(work + 0x16a);
      s32 ticks = 200;

      *countdown = ticks;
    }
    return;
  }
  count = *owed + 1;
  *owed = count;
  if (*(s16 *)(work + 0x156) != 0 || (u16)count == 0)
    return;
  do {
    switch (*step) {
    case 0:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 2);
      Func_02002d48(11, 0, 1);
      Func_02002d48(12, 0, -1);
      Func_02002d48(13, 0, 1);
      Func_02002d48(14, 0, 1);
      Func_02002d48(15, -1, 0);
      Func_02002d48(16, 0, 1);
      Func_02002d48(17, 1, 0);
      Func_02002d48(18, 0, -1);
      break;
    case 100:
    case 220:
      for (i = 11; i <= 18; i++)
        Actor_SetAnimation(i, 1);
      break;
    case 130:
      for (i = 11; i <= 18; i++) {
        Actor_SetAnimation(i, 2);
        Func_02002d48(i, 0, 0);
      }
      break;
    }
    if (++*step == 260)
      *step = 0;
  } while (--*owed != 0);
}

extern u32 gData_02004274[];

/// The room's setup: flags, the objects' places and the scene state.
void Islets_InitializeRoom2(void) {
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 i;

  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(0x12f);
  i = 11;
  do {
    Func_02002d0c(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    *state = 0;
    *next = 0;
  }
  FieldTask_AddFrame(Func_02002d7c, 0x480);
  if (Event_GetFlag(0xa2c)) {
    object = Actor_Get(10);
    Actor_SetPosition(10, PIXELS(176), PIXELS(168));
    Actor_SetAnimation(10, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = PIXELS(-8);
    object->terrainHeight = PIXELS(-8);
    MapGrid_CopyCellAttributes(9, 1, 4, 1, 9, 10);
    MapGrid_CopyCellAttributes(9, 64, 4, 1, 9, 74);
  } else {
    Func_02003684(10);
  }
  if (!Event_GetFlag(0xaa2)) {
    FieldActor_WatchAndSetScript(9, 19);
    Actor_SetAnimation(19, 0);
    Actor_Get(19)->actionFlags &= ~1;
  } else if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    other = Actor_Get(9);
    other->facing = FACING_SOUTH + FACING_STEP;
    other->collisionFlags |= 0x80;
    {
      s32 facing = FACING_SOUTHEAST + FACING_STEP;

      Actor_SetPositionAndFacing(19, PIXELS(234), PIXELS(204), facing);
    }
    Actor_Get(19)->collisionFlags |= 0x80;
    Actor_SetSpeed(9, 0x20000, 0x10000);
    Actor_SetSpeed(19, 0x19999, 0xcccc);
    Actor_SetScript(9, gData_02004274);
    Actor_SetScript(19, gData_02004274);
    Event_SetFlag(0x200);
  }
}

void Islets_InitializeRoom3(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    Event_ClearFlag(0x12f);
  Actor_SetSpriteFlags(Actor_Get(10), 0);
  Actor_SetAnimation(10, 4);
  if (Event_GetFlag(0xaa3)) {
    spawned = FieldActor_Create(489, 0x680000, 0x180000, 0xa00000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      FieldActor_SetItemIcon(spawned, 453);
      Actor_SetSpriteFlags(spawned, 0);
      spawned->unk5c[0] = 1;
    }
    Actor_SetPosition(10, PIXELS(104), PIXELS(204));
  }
  FieldActor_SetSpritePriority(11, 1);
  FieldActor_SetSpriteColor(Actor_Get(11), 15);
  Actor_Get(11)->priorityFlags |= 2;
}

// A room's setup.

/// Sets up the room's actors and map work.
void Islets_InitializeRoom4(void) {
  struct IsletsMapWork *map = (struct IsletsMapWork *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(10);
  struct Actor *last;
  s32 set;
  s32 zero = 0;

  map->value190 = zero;
  map->value194 = zero;
  if (gMainRoutineWork.entrance == 11)
    Event_ClearFlag(0x12f);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    object->motionFlags = zero;
    FieldActor_SetSpritePriority(10, 1);
  }
  Actor_SetPosition(11, 0, 0);
  Actor_DisableById(12);
  Actor_DisableById(13);
  set = Event_GetFlag(0x212);
  if (set) {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = zero;
    second->motionFlags = zero;
    ((u8 *)first)[0x62] = 1;
    ((u8 *)second)[0x62] = 1;
    {
      s32 held = 29;

      MapGrid_CopyCellAttributes(7, 3, 1, 1, 7, held);
    }
  } else {
    struct Actor *first = Actor_Get(12);
    struct Actor *second = Actor_Get(13);

    first->motionFlags = 0;
    second->motionFlags = 0;
    Func_020020ec(first->x, first->z, 3, 255);
    Func_020020ec(second->x, second->z, 3, 255);
  }
  Actor_DisableById(14);
  last = Actor_Get(14);
  {
    // A local that holds 0, as the native keeps its own register for it.
    s32 still = 0;

    ((u8 *)last)[0x62] = 1;
    last->motionFlags = still;
  }
  Func_020020ec(last->x, last->z, 1, 255);
  if (Event_GetFlag(0x213))
    MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
}

#if TLA_PATCH_1

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003410(void);
/* VIEW(islets): routine PaletteCycle_Add as void PaletteCycle_Add(s32 a, s32 b,
 * s32 c, s32 d); graphics/palette_snapshots.h has s32 PaletteCycle_Add(s16
 * bank, s16 index, s16 delay, s16 count); tried as the definition's s32
 * PaletteCycle_Add(s16 bank, s16 index, s16 delay, s16 count), which the object
 * already declares, Islets_InitializeRoom5 moves: 2 of 145 instructions in
 * tla-en. */
void PaletteCycle_Add__islets(s32 a, s32 b, s32 c,
                              s32 d) __asm__("PaletteCycle_Add");

void Islets_InitializeRoom5(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  {
    s32 a1 = 12;
    s32 a2 = 5;
    s32 a3 = 4;
    s32 a0 = 3;

    /* FAKEMATCH(barrier): the native loads the first argument last; as the
     * plain call, 2 of 145 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2), "+l"(a3));
    PaletteCycle_Add__islets(a0, a1, a2, a3);
  }
  PaletteCycle_Add__islets(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) &&
      (done = Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSpriteFlags(Actor_Get(9), 0);
  Actor_SetSpriteFlags(Actor_Get(11), 0);
  Actor_SetSpriteFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else if (entrance != 5) {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Event_End();
}

#else

// The Japanese release's older form.

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003410(void);
s32 PaletteCycle_Add__islets(s16 bank, s16 index, s16 delay,
                             s16 count) __asm__("PaletteCycle_Add");

void Islets_InitializeRoom5(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gMainRoutineWork.entrance == 2)
    Event_ClearFlag(303);
  Func_02003410();
  PaletteCycle_Add__islets(3, 12, 5, 4);
  PaletteCycle_Add__islets(4, 12, 5, 4);
  if (Event_GetFlag(0xaa5) &&
      (done = Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) == 0) {
    Actor_SetPosition(10, 0x1180000, 0x1d40000);
    Actor_Get(10)->motionFlags = done;
    {
      s32 a1 = 0x1280000;

      Actor_SetPosition(9, a1, 0x1c40000);
    }
  }
  Actor_SetSpriteFlags(Actor_Get(9), 0);
  Actor_SetSpriteFlags(Actor_Get(11), 0);
  Actor_SetSpriteFlags(Actor_Get(12), 0);
  FieldTask_AddFrame(Func_02001c4c, 0x480);
  if ((u16)(gMainRoutineWork.entrance - 4) <= 1) {
    Event_SetFlag(0x217);
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      {
        s32 a3 = 0x4000;

        Actor_SetPositionAndFacing(9, 0x980000, 0x3000000, a3);
      }
      Actor_SetPosition(10, 0xb00000, 0x3090000);
      System_Wait(1);
    }
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Event_End();
}

#endif

s32 Scene_PrepareMap(void) { return 0; }

void Func_02003410(void) {
  struct PaletteSnapshotWork *queue;
  struct PaletteSnapshot *entry;
  s16 i, j;

  queue = HeapSlot_AllocEwram(112, sizeof(struct PaletteSnapshotWork));
  entry = queue->entries;
  i = 0;
outer:
  entry->source = 0;
  entry->position = 0;
  entry->timer = 0;
  entry->delay = 0;
  entry->count = 0;
  j = 0;
inner:
  entry->colors[(u16)j] = 0;
  {
    s32 next = ((u16)j + 1) << 16;

    j = next >> 16;
    if (next != 16 << 16)
      goto inner;
  }
  entry++;
  {
    s32 next = (i << 16) + 0x10000;

    i = next >> 16;
    if (next != 4 << 16)
      goto outer;
  }
  queue->count = 0;
}

// Overlay 674 keeps its own palette queue between its allocation and step
// functions; its PaletteCycle_Add is the main image's.

#define gPaletteWork ((struct PaletteSnapshotWork *)Ram_HeapSlots->unk70[0])

#include "../field/palqueue_add.c"
#undef gPaletteWork

#include "text/msg_ids.h"

void Func_020034d8(void) {
  Task_AddFrame(&Func_020034fc, 0x480);
  return;
}

void Func_020034ec(void) {
  FieldTask_Remove(Func_020034fc);
  return;
}

/* VIEW(s3410): tag struct PaletteCycleWork as struct PaletteCycleWork__s3410;
 * palette_snapshots.h has another definition; tried with the definition the
 * rest of the object has, it does not compile in tla-ja:
 * games/tla/src/scene/islets.c:2230: structure has no member named `count'. */
struct PaletteCycleWork__s3410 {
  struct PaletteCycle entries[4];
  u16 count;
};

#define gPaletteWork ((struct PaletteCycleWork *)Ram_HeapSlots->unk70[0])
#define EVENT_S16(offset) (*(s16 *)((u8 *)Ram_HeapSlots->eventWork + (offset)))

void Func_020034fc(void) {
  if (EVENT_S16(0x166) == 0 && EVENT_S16(0x15a) == 0 && EVENT_S16(0x15e) == 0) {
    struct PaletteCycleWork__s3410 *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
      struct PaletteCycle *cycle = &work->entries[i];
      if (cycle->timer == 0) {
        u16 start = cycle->position;
        u16 len = cycle->count;
        void *dest = cycle->source;
        u16 *src = cycle->colors;
        u8 j;
        for (j = len - start; j < len; j++)
          buf[j] = *src++;
        for (j = 0; j < len - start; j++)
          buf[j] = *src++;
        Dma_Set(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
        start++;
        if (start >= len)
          start = 0;
        cycle->position = start;
        cycle->timer = cycle->delay;
      } else {
        cycle->timer--;
      }
    }
  }
}

void Func_02003620(void) {
  s32 v5;
  struct Actor *object0;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_550);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_Get(*(s32 *)((u8 *)v5 + 0));
  Actor_SetSpriteFlags((s32)object0, 0);
  Actor_SetAnimation(*(s32 *)((u8 *)v5 + 0), 18);
  FieldEvent_Wait(40);
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(3);
  return;
}

void Func_0200367c(void) {
  Field_TryJumpForward();
  return;
}

// Rewrites the map cells under a pushable actor. The actor's sprite
// shape picks one of the six footprints in gPushFootprints (four signed bytes
// each, in sixteenths of a cell); the cells under it are copied from the
// map's spare area (Map_CopyCellAttributeRect) and byte 2 of both collision
// layers there is filled with 255 (Func_02003790). Returns 1, or 0 for a shape
// with no footprint. The twin of overlay 668's Func_020040ec.

extern s8 gPushFootprints[][4];

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

/// Rewrites the map cells under a pushable actor. The actor's sprite shape
/// picks one of the six footprints in gPushFootprints (four signed bytes each,
/// in sixteenths of a cell); the cells under it are copied from the map's spare
/// area (Map_CopyCellAttributeRect) and byte 2 of both collision layers there
/// is filled with 255 (Func_02003790). Returns 1, or 0 for a shape with no
/// footprint. The twin of overlay 668's Func_020040ec.
s32 Func_02003684(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_GetById(id);
  s16 kind = *((struct ObjectAnimationState *)actor->sprite)->value28;
  s16 i = 0;
  s32 next;
  u32 index;
  s32 a, b;
  s32 fromX, fromZ;
  s32 width, height, x, z;

  // A goto loop: a C loop is strength-reduced, which the original's is not.
  if ((u16)kind != gData_02004070[(u16)i]) {
  again:
    next = (i + 1) * 65536;
    index = (u32)next >> 16;
    i = next >> 16;
    if (index <= 5 && (u16)kind != gData_02004070[index])
      goto again;
  }
  // fromX and b are reused for the next values they hold.
  fromX = (u16)i;
  if (fromX == 6)
    return 0;
  a = gPushFootprints[fromX][0];
  width = (Abs(a) + Abs(gPushFootprints[fromX][2])) >> 4;
  b = gPushFootprints[fromX][1];
  height = (Abs(b) + Abs(gPushFootprints[fromX][3])) >> 4;
  x = actor->x + (a << 16);
  z = actor->z + (b << 16);
  b = *(s32 *)(map + 0x148) >> 20;
  x >>= 20;
  z >>= 20;
  fromZ = *(s32 *)(map + 0x14c) >> 20;
  fromX = (u16)b;
  Map_CopyCellAttributeRect(x, z, width, height, x + fromX, z + (u16)fromZ);
  Func_02003790(0, x, z, width, height, 255);
  Func_02003790(2, x, z, width, height, 255);
  return 1;
}

void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value) {
  struct MapCell *cells = Ram_HeapSlots->mapWork->layers[layer].cells;
  struct MapCell *cell;
  u32 i;
  u32 j;

  cells += x + z * 128;
  for (i = 0; i < height; i++) {
    cell = cells + i * 128;
    for (j = 0; j < width; j++) {
      cell->collisionCode = value;
      cell++;
    }
  }
}

extern s32 gPushLeaderSteps[];

static __inline__ s32 RowByte(s8 *row, s32 index) { return row[index]; }

static __inline__ s32 BoxSpan(s32 kind, s32 start) {
  return (Abs(gPushFootprints[kind][start]) +
          Abs(gPushFootprints[kind][start + 2])) >>
         4;
}

static __inline__ s32 CallFunc_02003e2c(struct Actor *object,
                                        struct Vector3 *cell) {
  return MapGrid_CheckCollision(object, cell);
}

s32 Func_020037d8(struct PushTarget *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->id, &push->footprint);
  if (object != 0) {
    kind = push->footprint;
    /* FAKEMATCH(dead): the address of kind taken and dropped; as kind =
     * push->footprint alone, 127 of 171 instructions differ in tla-en. */
    (void)&kind;
    {
      s32 width = BoxSpan(kind, 0);
      s32 height = BoxSpan(kind, 1);
      u8 *layer;
      s32 index;

      offset = gPushLeaderSteps[direction];
      dx = offset & 0xffff0000;
      dz = offset << 16;
      push->callback = 0;
      steps = 0;
      layer = &object->terrainId;
      *layer = 2;
      push->x = object->x + dx;
      push->z = object->z + dz;
      cell.y = object->y;
      for (;;) {
        s8 *table = gPushFootprints[0];

        index = kind * 4;
        /* FAKEMATCH(wrapper): the read through RowByte; as table[index + 1],
         * 27 of 171 instructions differ in tla-en. */
        cell.z = push->z + (RowByte(table, index + 1) << 16);
        for (j = 0; j < height; j++) {
          cell.x = push->x + (gPushFootprints[0][index] << 16);
          for (i = 0; i < width; i++) {
            /* FAKEMATCH(wrapper): the call through CallFunc_02003e2c; as a
             * direct call, 2 of 171 instructions differ in tla-en. */
            if (CallFunc_02003e2c(object, &cell) == 2)
              goto blocked;
            cell.x += 0x100000;
          }
          cell.z += 0x100000;
        }
        push->x += dx;
        steps++;
        push->z += dz;
      }
    blocked:
      *layer = 0;
    }
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      result = 1;
      push->y = object->y;
      push->z = object->z + dz * steps;
    }
  } else
    result = 0;
  return result;
}

/// The whole-pixel half of a 16.16 coordinate.
#define HI(v) (((s16 *)&(v))[1])

struct Actor *Func_02003940(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gData_02004070[k]) {
    x = (HI(leader->x) + (gPushLeaderSteps[*direction] >> 16)) >> 4;
    z = (HI(leader->z) + (s16)gPushLeaderSteps[*direction]) >> 4;
    left = (HI(object->x) + gPushFootprints[k][0]) >> 4;
    top = (HI(object->z) + gPushFootprints[k][1]) >> 4;
    right = (HI(object->x) + gPushFootprints[k][2]) >> 4;
    bottom = (HI(object->z) + gPushFootprints[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

void Actor_StopAtPushTarget(struct Actor *object);

#define ABS(x) ((x) < 0 ? -(x) : (x))

DEFINE_PUSH_TARGET_PUSH(PushTarget_Push, gPushFootprints, Func_02003790,
                        Actor_StopAtPushTarget, Actor_SelectAnimation,
                        gPushLeaderSteps)

void Actor_StopAtPushTarget(struct Actor *object) {
  struct Vector3 position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, &position);
  hit = Actor_FindAtCell(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gData_02004070[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (MapGrid_CheckCollision(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
}

#undef TLA_ISLETS_H
#undef STEP
#undef HAND_BACK
#undef ISLETS_GIFT_TRANSITION
#undef STORED_AT_1AC
#undef SPAWN
#undef EVENT_WORK
#undef OBJECT
#undef gPaletteWork
#undef EVENT_S16
#undef HI
#undef ABS
#undef gData_020046bc
#undef gData_02004004
#include "text/msg_ids.h"

void Func_0200030c(void);
void Islets_TalkToHealer(void);
void Func_020004f4(void);
void Func_02000520(void);
void Func_020006f4(void);
void Func_02001db0(void);
void Func_02001f74(void);
void Func_020021d0(void);
void Func_02002780(void);

// clang-format off
u32 gData_02004004[] = {
    SC_ADD(SW_FACING, FACING_STEP),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
    SC_END,
};

DEFINE_G_DATA_0200121C(gData_02004028)

u16 gData_02004070[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0}, {-8, -32, 8, 32},
    {-32, -8, 32, 8}, {-8, -32, 8, 32},
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gData_020040d4[] = {
#if !TLA_PATCH_1
    22, 30, 131, 22, 16, 0x6666,
#else
    22, 30, 129, 22, 16, 0x6666,
#endif
    22, 15, 0x6666, 39, 2, 4,
    0x1780000, 0, 0xd80000, 1, 4, 0x1880000,
    0, 0xe00000, 1, 39, 1, 0,
    1, 22, 3, 0x3000, 0, 80,
    39, 2, 4, 0x1780000, 0, 0xd80000,
    1, 4, 0x1780000, 0, 0xb80000, 1,
    39, 1, 0, 40, 13, 0xffff,
    0,
};

u32 gData_02004198[] = {
    SC_ARG(4),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_Y, PIXELS(64)),
    SC_LABEL(1),
    SC_MOVE(PIXELS(352), PIXELS(64), PIXELS(336)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), PIXELS(64), PIXELS(336)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), PIXELS(64), PIXELS(328)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), PIXELS(64), PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), PIXELS(64), PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(352), PIXELS(64), PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), PIXELS(64), PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), PIXELS(64), PIXELS(328)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_02004274[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(234), 0, PIXELS(240)),
    SC_SYNC,
    SC_MOVE(PIXELS(276), 0, PIXELS(222)),
    SC_SYNC,
    SC_MOVE(PIXELS(308), 0, PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(252), 0, PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(208)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(184)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gData_02004300[] = {
    SC_ARG(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(-16)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(-32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(280), 0, PIXELS(856)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gData_020043f8[] = {
    SC_ARG(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(-32)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(-32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(280), 0, PIXELS(856)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gSmokeScript[] = {
    0, 15, 3, 0, 1, 38,
};

DEFINE_G_DATA_02004508(gData_02004508, 686, 768, 152, 768)

DEFINE_G_DATA_020033C8(gData_02004554, 248, 686, 768, 176, 777)

DEFINE_G_DATA_02004508(gData_020045a4, 768, 686, 248, 584)

DEFINE_G_DATA_020033C8(gData_020045f0, 248, 768, 686, 240, 600)

u32 gData_02004640[] = {
    39, 2, 4, 0xb80000, 0, 0x3a80000,
    1, 39, 1, 1, 80, 39,
    2, 4, 0x580000, 0, 0x3a80000, 1,
    39, 1, 0, 80, 13, 0xffff,
    0,
};

s16 gData_020046a4[] = {
    92, 10, 1, 2, 6, 92, 8, 1,
    2, 6, -1,
};

s16 gData_020046ba[] = {
    -1,
};

DEFINE_G_DATA_0200064C(gData_020046bc, gData_020046ba, 0x400040, gData_020046a4,
    0xa0052)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(131),
    SCENE_EXIT(1, 2, 28),
    SCENE_EXIT(2, 183, 9),
    SCENE_EXITS(132),
    SCENE_EXIT(1, 2, 29),
    SCENE_EXIT(2, 57, 5),
    SCENE_EXIT(3, 132, 1),
    SCENE_EXITS(133),
    SCENE_EXIT(1, 2, 30),
    SCENE_EXIT(2, 2, 80),
    SCENE_EXIT(3, 11, 7),
    SCENE_EXIT(4, 2, 81),
    SCENE_EXITS(134),
    SCENE_EXIT(10, 2, 31),
    SCENE_EXIT(11, 183, 8),
    SCENE_EXITS(135),
    SCENE_EXIT(1, 2, 32),
    SCENE_EXIT(2, 156, 6),
    SCENE_EXIT(3, 135, 4),
    SCENE_EXIT(4, 135, 3),
    SCENE_EXIT(5, 136, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gIslets_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_Room1Placements[] = {
    {73, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(744), FACING_WEST, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(920), FACING_NORTHWEST, 2, 0},
    {77, CONDITION_ALWAYS, (s32)gData_02004640, PIXELS(88), 0, PIXELS(936), FACING_EAST, 1, 0},
    {189, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(856), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_Room2Placements[] = {
    {71, CONDITION_ALWAYS, (s32)gData_020040d4, PIXELS(376), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {399, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {258, CONDITION_ALWAYS, 7, PIXELS(176), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {402, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {402, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {400, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(280), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_Room3Placements[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(184), FACING_SOUTHEAST, 1, 0},
    {80, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(176), FACING_WEST, 1, 0},
    {303, CONDITION_ALWAYS, 7, PIXELS(326), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_Room4Placements[] = {
    {187, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(296), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {405, CONDITION_ALWAYS, (s32)gData_02004198, PIXELS(360), 0, PIXELS(312), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 0, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(472), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gIslets_Room5Placements[] = {
    {75, CONDITION_ALWAYS, 2, PIXELS(168), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {162, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(440), FACING_SOUTH, 0, 0},
    {163, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(202), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(266), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIslets_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02002780)},
    {0x602, 16, CONDITION_ALWAYS, EVENT(Func_0200367c)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgEverSinceThe)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02001bf4)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgYouReLucky)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgArffArff2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgTheWaterwaysNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgICanTWaitUntil)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgICanTWaitUntilThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     EVENT(Func_02000e38)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200267c)},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(Func_0200208c)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(Func_0200367c)},
    {0x4602, 30, 0xa2c, EVENT(Func_020000d8)},
    {TRIGGER_STEP, 20, 0xaa2, EVENT(Func_02001db0)},
    {TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(Func_020024f0)},
    {TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(Func_0200253c)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgMyMomSays)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02003620)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgWhoInWeyard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Func_020002ac)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     EVENT(Func_02000380)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Islets_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgMayTheseTravelers)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_020025a4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_020025b0)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIJustWant)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheSeaOf)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgMooMoo2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgDadAndGrandpa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIRememberOnce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Func_020006f4)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Leader_SetLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Func_020020d0)},
#if TLA_PATCH_1 /* From patch 1. */
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Func_020025bc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Func_020025dc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12,
     CONDITION_ALWAYS, EVENT(Func_020025fc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13,
     CONDITION_ALWAYS, EVENT(Func_0200261c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(Func_0200263c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 15,
     CONDITION_ALWAYS, EVENT(Func_0200265c)},
#endif
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_Room4Triggers[] = {
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0xce01, 11, CONDITION_ALWAYS, 11},
    {0x4602, 21, 0x212, EVENT(Func_02002430)},
    {8, 0, 0x212, EVENT(Func_02002144)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x212, EVENT(Func_02002144)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x212, EVENT(Func_02002144)},
    {9, 0, 0x212, EVENT(Func_020021d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x212, EVENT(Func_020021d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x212, EVENT(Func_020021d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x213, EVENT(Func_020024c8)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000440)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Func_020004f4)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000520)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIfWeRe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgSureThereAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(Func_02000568)},
    {TRIGGER_STEP, 20, 0x215, EVENT(Func_02001f74)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIslets_Room5Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_0200274c)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_0200274c)},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgYouUsedTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgIRememberHow)},
    {TRIGGER_TALK, 9, 0xaa5, MSGNO(MsgBlank3)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgYup2)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x217, EVENT(Func_020014e0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     EVENT(Func_02001a68)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02001c38)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_02001c38)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Leader_SetLayer)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Func_020020d0)},
    {TRIGGER_TABLE_END},
};
