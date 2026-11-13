/// @file
/// Alhafra's houses, inn and sanctum, whose people are taking in their
/// neighbors after the tidal wave. Their tables follow the story: before Briggs
/// is caught, after (FLAG_BRIGGS_CAUGHT), and once the boat's mast is cleared
/// (FLAG_ALHAFRA_MAST_CLEARED).

#include "system/main_routine_work.h"
#include "event_work.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "module/scene/scene.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "field_sprite.h"
#include "system/frame.h"
#include "field/event_inline.h"
#include "actor.h"
#include "call.h"
#include "version.h"

extern u8 gAlhafraInteriors_FirstGuestSprite[];
extern u8 gAlhafraInteriors_SecondGuestSprite[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

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

extern struct ActorPlacement gActorPlacements[];
extern struct ActorPlacement
    gAlhafraInteriors_ActorPlacementsOnceBriggsIsCaught[];
extern struct ActorPlacement
    gAlhafraInteriors_ActorPlacementsOnceTheMastIsCleared[];
extern struct Trigger gAlhafraInteriors_Triggers[];
extern struct Trigger gAlhafraInteriors_TriggersOnceBriggsIsCaught[];
extern struct Trigger gAlhafraInteriors_TriggersOnceTheMastIsCleared[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraInteriors_ActorPlacementsOnceTheMastIsCleared;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gAlhafraInteriors_ActorPlacementsOnceBriggsIsCaught;
  return gActorPlacements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// Stepping into Alhafra's inn the first time: Felix walks up to Alex, who
/// says he too is resting here while he looks for a boat
/// (FLAG_ALHAFRA_MET_ALEX).
void AlhafraInteriors_MeetAlex(void) {
  Event_SetFlag(FLAG_ALHAFRA_MET_ALEX);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors31);
  FieldCamera_MoveToActorById(31, 1);
  Actor_WalkToAndWait(4, 452, 335);
  Actor_FaceDirection(4, FACING_WEST, 0);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(31, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 31, 0, 10);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 31, 0, 10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 31, 0, 10);
  Actor_RunRepeatedMotion(31, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 31, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 31, 0, 10);
  Actor_FaceDirection(31, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The merchant: across the counter, his shop; else what he says.
void AlhafraInteriors_TalkToItemMerchant(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(9, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors14);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The innkeeper: across the counter, the inn; else what he says.
void AlhafraInteriors_TalkToInnkeeper(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Inn_CheckIn(2, actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors16);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The healer: across the counter, the sanctum; else what he says.
void AlhafraInteriors_TalkToHealer(s32 actor) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Sanctum_Run(actor);
  } else {
    Event_SetMessageId((s32)MsgAlhafraInteriors49);
    Event_ShowMessageAndWait(actor, 0);
  }
}

/// The lady of the house, worn out by her houseguests.
void AlhafraInteriors_TalkToHostess(s32 a0) {
  Actor_ShowEmote(a0, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors06);
  Event_ShowMessage(a0, 0);
}

/// The old couple who lost their house in the wave ask the lady of the house
/// for tea, once (flag 0x8b1).
void AlhafraInteriors_GuestsAskForTea(void) {
  Event_SetFlag(0x8b1);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 152, 328);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors07);
  Actor_FaceDirection(14, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_FaceDirection(15, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(15, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetSpeed(14, 0.5);
  Actor_WalkByAndWait(14, -32, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(14, 0, 10);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(13, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  FieldActor_StartRepeatedMotion(14, 3);
  Actor_SetAttachedEffect(14, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_RunRepeatedMotion(13, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(13, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(14, 152, 312);
  FieldEvent_Wait(10);
  Actor_SetAnimation(15, 4);
  Actor_SetAnimationAndWait(14, 4);
  FieldEvent_Wait(10);
  Actor_FaceDirection(14, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Event_End();
}

/// The old houseguest, put out by the lady of the house.
void AlhafraInteriors_TalkToDemandingGuest(s32 a0) {
  Actor_FaceEachOther(a0, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors08);
  Event_ShowMessage(a0, 0);
  Actor_FaceDirection(14, FACING_SOUTH_SOUTHEAST, 0);
}

/// Mind Read on Alex: he turns to the party and asks them to stop.
void AlhafraInteriors_ReadAlexsMind(s32 a0, s32 a1) {
  Actor_FaceEachOther(a1, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors48);
  Event_ShowMessage(a1, 0);
  Actor_FaceDirection(a1, FACING_NORTHEAST, 0);
}

/// The master of the house tells that the town has started rebuilding, once
/// (flag 0x8b8).
void AlhafraInteriors_HostTellsOfRepairs(s32 a0) {
  Event_SetFlag(0x8b8);
  FieldEvent_SetMessageId((s32)MsgAlhafraInteriors55);
  Event_ShowMessage(a0, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(a0, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessage(a0, 0);
}

/// Alex asks whether the party caught Briggs.
void AlhafraInteriors_AlexHearsOfBriggs(s32 actor) {
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

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraInteriors_TriggersOnceTheMastIsCleared;
  if (Event_GetFlag(FLAG_BRIGGS_CAUGHT))
    return gAlhafraInteriors_TriggersOnceBriggsIsCaught;
  return gAlhafraInteriors_Triggers;
}

s32 BattleEffect_DrawSpriteAtWorldPosition();

s32 AlhafraInteriors_DrawFirstGuestSprite(s32 a0) {
  BattleEffect_DrawSpriteAtWorldPosition(
      (s32)gAlhafraInteriors_FirstGuestSprite, FIELD_AT(a0, s32, 8),
      (FIELD_AT(a0, s32, 12) + 0xc0000), FIELD_AT(a0, s32, 16));
  return 0;
}

s32 AlhafraInteriors_DrawSecondGuestSprite(s32 a0) {
  BattleEffect_DrawSpriteAtWorldPosition(
      (s32)gAlhafraInteriors_SecondGuestSprite, FIELD_AT(a0, s32, 8),
      (FIELD_AT(a0, s32, 12) + 0x80000), FIELD_AT(a0, s32, 16));
  return 0;
}

// The Japanese edition sets no flag 0x200 by the leader's position and places
// no actors 31 and 32.
#if TLA_PATCH_1
void BattleEffect_FillOverlayTiles(s32 mode);
void EffectSprite_InitFromSlot94();

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  BattleEffect_FillOverlayTiles(0);
  Call4(EffectSprite_InitFromSlot94, (s32)gAlhafraInteriors_FirstGuestSprite, 8,
        15, 0x40000000);
  Call4(EffectSprite_InitFromSlot94, (s32)gAlhafraInteriors_SecondGuestSprite,
        8, 15, 0x40000000);
  *(s32 *)(gAlhafraInteriors_SecondGuestSprite + 24) = 0x18000;
  Actor_Get(14)->update = AlhafraInteriors_DrawFirstGuestSprite;
  Actor_Get(15)->update = AlhafraInteriors_DrawSecondGuestSprite;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (Actor_Get(gMainRoutineWork.currentOwner)->x >= px(512))
      Event_SetFlag(FLAG_SCENE_TEMP_00);
    else
      Event_ClearFlag(FLAG_SCENE_TEMP_00);
    if (Event_GetFlag(FLAG_SCENE_TEMP_00) &&
        Event_GetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) &&
        !Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
      Actor_SetPosition(31, 376, 316);
      Actor_SetPosition(32, 392, 344);
    } else {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_ALEX_LEFT_ALHAFRA))
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
void EffectSprite_InitFromSlot94();

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  BattleEffect_FillOverlayTiles(0);
  Call4(EffectSprite_InitFromSlot94, (s32)gAlhafraInteriors_FirstGuestSprite, 8,
        15, 0x40000000);
  Call4(EffectSprite_InitFromSlot94, (s32)gAlhafraInteriors_SecondGuestSprite,
        8, 15, 0x40000000);
  *(s32 *)(gAlhafraInteriors_SecondGuestSprite + 24) = 0x18000;
  Actor_Get(14)->update = AlhafraInteriors_DrawFirstGuestSprite;
  Actor_Get(15)->update = AlhafraInteriors_DrawSecondGuestSprite;
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    if (!Event_GetFlag(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE) ||
        Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    if (Event_GetFlag(FLAG_ALEX_LEFT_ALHAFRA))
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

extern u32 gAlhafraInteriors_TravelersWalkOffScript[];

/// Two guests of the inn tell that Isaac picked Crossbone Isle clean, once
/// (FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE), while
/// FLAG_ISAAC_CLEARED_CROSSBONE_ISLE is set.
void AlhafraInteriors_OverhearCrossboneIsleTale(void) {

  if (Event_GetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE)) {
  } else {
#if TLA_PATCH_1 /* The Japanese edition does not check 0x200. */
    if (!Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    } else
#endif
    {
      Event_SetFlag(FLAG_ALHAFRA_HEARD_OF_CROSSBONE_ISLE);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgAlhafraInteriors120);
      Actor_WalkToAndWait(4, 504, 328);
      Actor_FaceDirectionAndWait(4, FACING_WEST);
      FieldCamera_MoveTo(408, CAMERA_KEEP, 328, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      Actor_ShowEmote(32, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_ShowEmote(31, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      Actor_ShowEmote(32, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      FieldActor_Jump(32, 6, 15);
      FieldActor_Jump(32, 6, 23);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      Actor_ShowEmote(32, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_RunRepeatedMotion(31, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_FaceDirectionAndWait(31, FACING_NORTH_NORTHEAST);
      FieldEvent_Wait(15);
      FieldEvent_ShowMessageThenWait(31, 0, 5);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(32, 0, 5);
      Actor_FaceDirectionAndWait(31, FACING_SOUTH_SOUTHEAST);
      FieldEvent_Wait(15);
      Actor_SetAnimationAndWait(31, 3);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(32, 3);
      FieldEvent_Wait(20);
      Actor_SetSpeed(31, 1);
      Actor_SetSpeed(32, 1);
      FieldActor_SetSpritePriority(31, 2);
      FieldActor_SetSpritePriority(32, 2);
      EventCamera_FollowActor(32, 1);
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
      Actor_SetScript(32, (s32)gAlhafraInteriors_TravelersWalkOffScript);
      FieldEvent_Wait(10);
      Actor_SetScript(31, (s32)gAlhafraInteriors_TravelersWalkOffScript);
      FieldEvent_Wait(10);
      FieldActor_WatchAndSetScript(4, 32);
      FieldEvent_Wait(60);
      FieldActor_SetSpritePriority(31, 3);
      FieldActor_SetSpritePriority(32, 3);
      FieldActor_RefreshSelectorById(32);
      FieldActor_Stop(4);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(32, 0, 0);
      Event_PlaySound(SE_MAP_EXIT);
      FieldActor_RefreshSelectorById(31);
      Actor_SetPosition(31, 0, 0);
      Event_PlaySound(SE_MAP_EXIT);
      FieldEvent_Wait(10);
      Event_End();
    }
  }
}

// clang-format off
u32 gAlhafraInteriors_TravelersWalkOffScript[] = {
    39, 2, 4, 0x2080000, 0, 0x1480000,
    1, 4, 0x2080000, 0, 0x1380000, 1,
    4, 0x2280000, 0, 0x1380000, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafraInteriors),
    SCENE_EXIT_UNLESS(1, Alhafra1, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Alhafra1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Alhafra1, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Alhafra1, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Alhafra1, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(20, AlhafraInteriors, 20, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(21, AlhafraInteriors, 21, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, AlhafraInteriors, 22, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, AlhafraInteriors, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(24, AlhafraInteriors, 24, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(25, AlhafraInteriors, 25, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 280, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(211, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 152, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 520, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 536, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 544, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 344, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraInteriors_ActorPlacementsOnceBriggsIsCaught[] = {
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(211, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraInteriors_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 544, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 312, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(199, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 84, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 152, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 864, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(211, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 762, 0, 84, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 536, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 552, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(224, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 316, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(225, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 344, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gAlhafraInteriors_Triggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 16, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToHealer)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors121))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors122))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors123))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors124))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGER_STEP, 30, FLAG_ALHAFRA_MET_ALEX, EVENT(AlhafraInteriors_MeetAlex)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors01)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors04)},
    {TRIGGER_TALK, 14, 0x8b1, EVENT(AlhafraInteriors_GuestsAskForTea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x8b1,
     EVENT(AlhafraInteriors_GuestsAskForTea)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors05)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(AlhafraInteriors_TalkToHostess)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToDemandingGuest)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors13)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors15)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors16)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors17)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors18)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors19)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors20)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors21)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors22)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors23)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors24)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors25)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors26)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors27)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors28)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors29)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors30)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors33)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors36)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors41)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors42)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1055, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_ReadAlexsMind)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors50)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafraInteriors_TriggersOnceBriggsIsCaught[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 16, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToHealer)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors121))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors122))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors123))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors124))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGER_TALK, 30, IF_FLAG_SET(FLAG_ALEX_LEFT_ALHAFRA),
     MSGNO(MsgAlhafraInteriors84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30,
     IF_FLAG_SET(FLAG_ALEX_LEFT_ALHAFRA), MSGNO(MsgAlhafraInteriors85)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors51)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors52)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors53)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors54)},
    {TRIGGER_TALK, 12, 0x8b8, EVENT(AlhafraInteriors_HostTellsOfRepairs)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors56)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors57)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors58)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors60)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors61)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors62)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors63)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors64)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors65)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors66)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors67)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors68)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors69)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors70)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors71)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors72)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors73)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_AlexHearsOfBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors75)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors76)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors77)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors78)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors79)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors80)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors81)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors82)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 31, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors83)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors86)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors87)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafraInteriors_TriggersOnceTheMastIsCleared[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToItemMerchant)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 16, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToInnkeeper)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS,
     EVENT(AlhafraInteriors_TalkToHealer)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 214, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors121))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 215, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors122))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 216, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors123))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 217, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors124))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGER_STEP, 31, IF_FLAG_SET(FLAG_ISAAC_CLEARED_CROSSBONE_ISLE),
     EVENT(AlhafraInteriors_OverhearCrossboneIsleTale)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors88)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors89)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors90)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors91)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors92)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors93)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors94)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors95)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors96)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors97)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors98)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors99)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors100)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors101)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors102)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors103)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors104)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors105)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors106)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors107)},
    {TRIGGER_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors108)},
    {TRIGGER_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors109)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors110)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors111)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors112)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors113)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors114)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors115)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors116)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors117)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraInteriors118)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraInteriors119)},
    {TRIGGER_TABLE_END},
};
