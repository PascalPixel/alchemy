/// @file
/// Champa, the pirates' village on the coast of the Great Western Sea, whose
/// people are fed once Briggs returns with the Ankohl jewels. Two of its
/// sailors spot Felix and run to warn Briggs (Champa_SailorsSpotFelix); at the
/// cliff where Obaba lives the party catches up with him
/// (Champa_CatchUpWithBriggs), Jenna and Sheba glaring at each other on the way
/// (Champa_JennaAndShebaGlare). In a game linked with The Broken Seal, Feizhi
/// arrives with a gift (Champa_FeizhiArrives). The getters pick later tables
/// once Briggs has sailed from Alhafra.

#include "shared/tables.h"
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
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "event_work.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/window/window.h"
#include "actor.h"
#include "call.h"
#include "version.h"
#include "system/frame.h"

struct Actor;
extern s16 gChampa_ActorPlacementsOnceBriggsSailed[];
extern s32 gChampa_Doors[];
s32 Event_PickUpItem();
extern struct Trigger gChampa_Triggers[];
extern struct Trigger gChampa_TriggersOnceBriggsSailed[];
void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s16 gChampa_NoDoorEdit[];
extern s16 gChampa_DoorEdit[];
s32 Tightrope_Place();
s32 Window_CloseMatchingId();

/// Sends an actor walking to where another stands, if it is on the map.
static inline void Actor_HeadToward(s32 id, s32 target) {
  struct Actor *actor = Actor_Get(target);

  if (actor != NULL)
    Actor_SetDestination(id, ((s16 *)&actor->x)[1], ((s16 *)&actor->z)[1]);
}

// The town's own story, which no other scene reads.
#define FLAG_NEAR_THE_CLIFF 0x970 ///< the party walked up to Obaba's cliff
#define FLAG_CAUGHT_UP_WITH_BRIGGS 0x971 ///< Briggs ran from them inside
#define FLAG_TOLD_OF_OBABA 0x972         ///< a villager told of Obaba
#define FLAG_SAILORS_SPOTTED_FELIX 0x973 ///< two sailors ran to warn Briggs
#define FLAG_FEIZHIS_GIFT_WAITING 0xa74  ///< Feizhi holds out her gift
#define FLAG_FEIZHI_CAME 0xa75 ///< Feizhi came, taking Felix for a friend

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s16 gChampa_ActorPlacements[];

const s16 *Scene_GetPlacements(void) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampa_ActorPlacementsOnceBriggsSailed;
  return gChampa_ActorPlacements;
}

void Champa_ShrinkIntoDoorway(struct Actor *object) {
  object->scaleX += -1536;
  object->scaleY += -1536;
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_SetDestinationOffsetById(id, dx, dz);
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void Champa_OpenDoorAndEnter(const void *warp, s32 door, s32 mode) {
  s32 id;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  id = gMainRoutineWork.currentOwner;
  if (mode == 0)
    Actor_CenterOnColumn_(id, 2, -8);
  else
    Actor_OffsetPositionAndResetMotion_(id, 0, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Map_ClearLayerEntryFlag(s32 a);

void Champa_EnterDoor(s32 door) {
  s32 *table = gChampa_Doors;

  if ((u32)(door - 1) <= 1)
    *(void **)((u8 *)Actor_Get(gMainRoutineWork.currentOwner) + 108) =
        Champa_ShrinkIntoDoorway;
  if (door == 7) {
    Map_ClearLayerEntryFlag(8);
    Champa_OpenDoorAndEnter(table, 7, 1);
  } else {
    Champa_OpenDoorAndEnter(table + 2, door, 0);
  }
}

void Champa_RevealTreasureSpot(void) {

  TreasureSpot_SetPosition(0x40, 0x1880000, 0x2380000);
  Actor_Get(64)->motionFlags = 0;
  Actor_Get(64)->y = 0x200000;
}

void Champa_HideTreasureSpot(void) { TreasureSpot_SetPosition(0x40, 0, 0); }

#include "text/msg_ids.h"

void Champa_SailorsSpotFelix(void) {

  Event_SetFlag(FLAG_SAILORS_SPOTTED_FELIX);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgChampa49);
  Actor_FaceDirection(32, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_Jump(32, 6, 20);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  FieldCamera_MoveTo(176, CAMERA_KEEP, 256, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(32, 3);
  Actor_SetAttachedEffect(0x20,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(33, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x21, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x23);
  Actor_FaceDirectionAndWait(33, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x21, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  FieldEvent_ShowMessageThenWait(0x21, 0, 5);
  FieldActor_Jump(32, 6, 0);
  FieldActor_Jump(33, 6, 0);
  Actor_SetSpeed(32, 1.8);
  Actor_SetSpeed(33, 1.8);
  Actor_Get(32)->actionFlags &= 0xfe;
  Actor_WalkBy(32, 0, -16);
  Actor_Get(33)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(33, 0, -16);
  FieldEvent_Wait(1);
  Actor_Get(33)->actionFlags |= 1;
  FieldEvent_Wait(15);
  Actor_ShowEmote(0x20, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  FieldEvent_ShowMessageThenWait(0x20, 0, 5);
  Actor_FaceDirection(32, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(33, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 33, 0, 5);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_WatchAndSetScript(32, 34);
  FieldActor_WatchAndSetScript(33, 34);
  Actor_SetSpeed(34, 1.2);
  Actor_WalkByAndWait(34, 0, 32);
  Actor_FaceDirectionAndWait(34, FACING_SOUTH_SOUTHEAST);
  FieldCamera_MoveTo(208, CAMERA_KEEP, 296, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(15);
  FieldActor_Jump(34, 6, 15);
  FieldActor_Jump(34, 6, 23);
  FieldEvent_ShowMessageThenWait(0x22, 0, 5);
  FieldActor_Jump(34, 6, 20);
  Actor_SetSpeed(34, 2);
  Actor_WalkByAndWait(34, 0, -48);
  Actor_FaceDirection(32, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(33, FACING_NORTH_NORTHEAST, 0);
  Actor_SetPosition(34, 0, 0);
  Actor_SetSpeed(32, 2);
  Actor_SetSpeed(33, 2);
  Actor_WalkByAndWait(32, -16, -16);
  Actor_WalkBy(33, 16, -16);
  Actor_WalkByAndWait(32, 0, -16);
  Actor_SetPosition(32, 0, 0);
  Actor_WalkByAndWait(33, 0, -16);
  Actor_SetPosition(33, 0, 0);
  Event_End();
}

void Champa_TalkToPracticingBoy(s32 actor) {
  struct Actor *object = Actor_Get(actor);

  object->scaleY = 0x10000;
  object->scaleX = 0x10000;
  FieldEvent_SetMessageId((s32)MsgChampa07);
  Event_ShowMessage(actor, 0);
  Actor_FaceDirectionAndWait(actor, FACING_EAST);
  Actor_SetActionCallback(object, 8);
}

void Champa_TellOfBriggsTreasure(s32 actor) {
  s32 msg = (s32)&MsgChampa29;

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

void Champa_AskAboutAnkohlRuins(s32 actor) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldEvent_SetMessageId((s32)MsgChampa30);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa30 + 2);
    }
    Event_ShowMessage(actor, 0);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa05);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
        0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgChampa05 + 2);
    }
    Event_ShowMessage(actor, 0);
  }
}

void Champa_TellOfObaba(s32 a0) {

  if (!Event_GetFlag(FLAG_TOLD_OF_OBABA)) {
    Event_SetFlag(FLAG_TOLD_OF_OBABA);
    FieldEvent_SetMessageId((s32)MsgChampa35);
  } else {
    FieldEvent_SetMessageId((s32)MsgChampa36);
  }
  Event_ShowMessage(a0, 0);
}

void Champa_SailorsNotWelcome(s32 actor) {
  s32 msg = (s32)&MsgChampa11;

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

void Champa_TakeFeizhisGift(s32 a0) {

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Value3(Event_PickUpItem, a0, 0x19f, -1)) {
    Event_ClearFlag(FLAG_FEIZHIS_GIFT_WAITING);
  }
  Event_End();
}

void Champa_FeizhiHoldsHerGift(void) {
  FieldActor_ShowItemIconById(31, 415);
  {
    s32 facing = FACING_NORTH;

    Actor_SetPositionAndFacing(31, PIXELS(312), PIXELS(593), facing);
  }
  Event_SetFlag(FLAG_FEIZHIS_GIFT_WAITING);
}

void Champa_FeizhiArrives(void) {
#if TLA_PATCH_1
#endif

  if (!Event_GetFlag(0x822)) {
  } else {
#if TLA_PATCH_1
    // The Japanese edition has no flag 0 test.
    if (Event_GetFlag(0)) {
    } else
#endif
    {
      Event_SetFlag(FLAG_FEIZHI_CAME);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      FieldEvent_SetMessageId((s32)MsgChampa52);
      {
        s32 angle = 0xc000;

        Actor_SetPositionAndFacing(29, PIXELS(314), PIXELS(679), angle);
      }
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_WalkToAndWait(4, 314, 580);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(10);
      EventCamera_FollowActor(29, 1);
      FieldCamera_WaitForArrival();
      FieldEvent_Wait(40);
      Actor_WalkToAndWait(29, 314, 624);
      Actor_ShowEmote(0x1d, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
      Event_OpenMessage(0x1d, 0);
      if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
        FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
            (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
      } else {
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
            (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
        FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      }
      Actor_ShowEmote(0x1d, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
      FieldActor_LaunchFromLeader(0x1e, 0x10, 0, 0x4000);
      FieldActor_WaitForMoveById(30);
      FieldEvent_Wait(20);
      {
        /* FAKEMATCH(pin): the emote pinned to r1. */
        register s32 emote asm("r1") = 0x101;

        Actor_ShowEmote(0x1e, emote, 0x1e);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(20);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_FaceEachOther(30, 4, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      Actor_SetAnimationAndWait(30, 4);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_ShowEmote(0x1d, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
      FieldActor_Jump(29, 2, 0);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_ShowEmote(0x1e, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH(barrier): the emote and the frames held at an empty asm. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Actor_ShowEmote(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_ShowEmote(0x1d, EMOTE_IN_FRONT | EMOTE_WINCE, 0x14);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_FaceDirectionAndWait(29, FACING_SOUTH);
      FieldEvent_Wait(10);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH(barrier): the emote and the frames held at an empty asm. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Actor_ShowEmote(0x1e, b, c);
      }
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_FaceDirectionAndWait(29, FACING_NORTH);
      FieldEvent_Wait(10);
      Actor_ShowEmote(0x1d, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_RunRepeatedMotion(29, 2);
      FieldEvent_Wait(5);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Champa_FeizhiHoldsHerGift();
      Actor_FaceEachOther(30, 4, 0);
      FieldEvent_Wait(40);
      Actor_FaceDirection(30, FACING_SOUTH, 0);
      Actor_FaceDirection(4, FACING_SOUTH, 0);
      FieldEvent_Wait(20);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_ShowEmote(0x1e, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
      FieldEvent_ShowMessageThenWait(0x1e, 0, 5);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      FieldEvent_ShowMessageThenWait(0x1d, 0, 5);
      Actor_SetAnimation(4, 3);
      Actor_SetAnimationAndWait(30, 3);
      FieldEvent_Wait(10);
      Actor_SetAnimationAndWait(29, 3);
      FieldEvent_Wait(10);
      EventCamera_FollowActor(4, 1);
      Actor_SetSpeed(29, 1.2);
      Actor_WalkBy(29, 0, 100);
      FieldEvent_Wait(10);
      Actor_FaceEachOther(4, 30, 0);
      FieldEvent_Wait(30);
      Actor_SetAnimation(30, 3);
      Actor_SetAnimationAndWait(4, 3);
      FieldEvent_Wait(30);
      Actor_SetAnimation(30, 2);
      Actor_HeadToward(30, 4);
      FieldActor_WaitForMoveById(30);
      Actor_SetPosition(30, 0, 0);
      Actor_SetPosition(29, 0, 0);
      Event_End();
    }
  }
}

const struct Trigger *Scene_GetEvents(void) {
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA))
    return gChampa_TriggersOnceBriggsSailed;
  return gChampa_Triggers;
}

void Champa_RotatePaletteColors(void) {
  if ((gFrameCount & 1) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = *(volatile u16 *)0x0500007c;
    index = 14;
  first:
    palette = (volatile u16 *)0x05000060;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto first;
    *(volatile u16 *)0x05000062 = saved;
    saved = *(volatile u16 *)0x0500009c;
    index = 14;
  second:
    palette = (volatile u16 *)0x05000080;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto second;
    *(volatile u16 *)0x05000082 = saved;
    saved = *(volatile u16 *)0x050000bc;
    index = 14;
  third:
    palette = (volatile u16 *)0x050000a0;
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 1)
      goto third;
    *(volatile u16 *)0x050000a2 = saved;
  }
}

s32 Tightrope_Initialize();

s32 Scene_Initialize(void) {

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Task_AddFrame((s32)Champa_RotatePaletteColors, 0xc80);
  Map_SetLayerEntryFlag(8);
  Actor_SetSpriteFlags(((s32)Actor_Get(10)), 0);
  Actor_SetSpriteFlags(((s32)Actor_Get(11)), 0);
  FieldActor_SetSpritePriority(13, 3);
  Actor_Get(23)->scaleX = 0x8000;
  Actor_Get(23)->scaleY = 0x8000;
  Tightrope_Initialize();
  Tightrope_Place(0, 8, 9);
  Map_SetLayerEntryFlag(3);
  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    FieldActor_SetSpritePriority(20, 2);
    FieldActor_SetSpritePriority(21, 3);
    FieldActor_SetSpritePriority(22, 3);
    FieldActor_SetSpritePriority(19, 3);
    Actor_SetActionCallback(Actor_Get(18), 9);
    if (Event_GetFlag(FLAG_SAILORS_SPOTTED_FELIX)) {
      Actor_SetPosition(32, 0, 0);
      Actor_SetPosition(33, 0, 0);
      Actor_SetPosition(34, 0, 0);
    }
  }
  if (Event_GetFlag(FLAG_FEIZHIS_GIFT_WAITING)) {
    Champa_FeizhiHoldsHerGift();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {

  if (Event_GetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
    MapGrid_CopyCellsLayered(0xb, 0x33, 5, 4, 0x10, 0x17);
    MapGrid_CopyCellAttributes(11, 51, 5, 6, 16, 23);
  }
  return 0;
}

extern u32 gChampa_GlareSparkScript[];

void Actor_StartScript(struct Actor *object, const void *script);

void Champa_SpawnGlareSpark(s32 x, s32 y, s32 z, s32 angle, s32 frames) {
  s16 value = angle;
  struct Actor *object;
  struct FieldSprite *sprite;

  object = FieldActor_Create(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Actor_SelectAnimation(object, 1);
    Actor_StartScript(object, gChampa_GlareSparkScript);
    object->motionFlags = 0;
    ((struct {
      u8 unk00[94];
      u16 frames;
    } *)object)
        ->frames = frames;

    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

void Champa_JennaAndShebaGlare(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_Get(firstId);
  struct Actor *second = Actor_Get(secondId);
  s32 rise = 0x1c0000;
  s32 y;

  y = first->y;
  Champa_SpawnGlareSpark(first->x, y + rise, first->z, 0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  y = second->y;
  Champa_SpawnGlareSpark(second->x, y + rise, second->z, 0xf000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnGlareSpark(first->x, first->y + rise, first->z, 0x1000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
  Champa_SpawnGlareSpark(second->x, second->y + rise, second->z, 0xf000, 15);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(15);
}

void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
void Portrait_OpenWindow(s32 object, s32 a, s32 b, s32 c);

void Champa_ShowTwoMessages(s32 firstId, s32 firstX, s32 firstY, s32 firstA,
                            s32 firstB, s32 secondId, volatile s32 secondX,
                            s32 secondY, s32 secondA, s32 secondB,
                            volatile s32 frames) {
  /* FAKEMATCH(volatile): the frames volatile, read from their stack slot at
   * each use. */
  struct ChampaEventWork *work = Ram_HeapSlots->eventWork;
  s32 first = Actor_GetSpriteById(firstId);
  s32 second = Actor_GetSpriteById(secondId);

  Message_OpenWindow(work->message++, firstX, firstY,
                     Speaker_GetVoice(first) << 16);
  Portrait_OpenWindow(first, 0, firstA, firstB);
  if (frames != 0) {
    while (!Message_AllTyped())
      System_Wait(frames);
  }
  /* FAKEMATCH(volatile): the counter's second store and the second x both
   * volatile. */
  Message_OpenWindow((*(volatile s16 *)&work->message)++, secondX, secondY,
                     Speaker_GetVoice(second) << 16);
  Portrait_OpenWindow(second, 0, secondA, secondB);
  while (!Message_AllTyped())
    System_Wait(1);
  System_Wait(1);
}

void Champa_CloseTwoMessages(s32 a0, s32 a1) {
  s32 t0;
  s32 t1;

  t0 = Actor_GetSpriteById(a0);
  t1 = Actor_GetSpriteById(a1);
  Window_CloseMatchingId(t0);
  Window_CloseMatchingId(t1);
}

void Champa_ApproachTheCliff(void) { GameFlag_Set(FLAG_NEAR_THE_CLIFF); }

static __inline__ void Champa_MessageOpen(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActor(speaker, flags);
}

void Champa_CatchUpWithBriggs(void) {

  if (!Event_GetFlag(FLAG_NEAR_THE_CLIFF)) {
    return;
  }
  Event_SetFlag(FLAG_CAUGHT_UP_WITH_BRIGGS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgChampa25);
  Actor_SetSpeed(25, 0.8);
  Actor_SetSpeed(26, 1);
  Actor_SetSpeed(27, 1);
  Actor_SetPosition(25, 312, 520);
  Actor_SetPosition(26, 456, 616);
  Actor_SetPosition(27, 168, 616);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_ShowMessage(0x19, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x14);
  Actor_WalkToAndWait(4, 312, 616);
  FieldCamera_MoveTo(312, CAMERA_KEEP, 600, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(5);
  Event_PlaySound(MUS_039);
  Actor_WalkToAndWait(25, 304, 576);
  FieldEvent_Wait(5);
  FieldActor_LaunchFromLeader(7, -16, 0, 0xc000);
  FieldActor_LaunchFromLeader(5, 0, 0x10, 0xc000);
  FieldActor_LaunchFromLeader(6, -16, 0x10, 0xc000);
  FieldActor_LaunchFromLeader(0x1c, -32, 0, 0xd000);
  FieldActor_WaitForMoveById(28);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(10);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirection(28, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Champa_MessageOpen(0x2019, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(15);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
  } else {
    FieldEvent_Wait(35);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  }
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x32);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_FaceDirection(28, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  FieldEvent_Wait(5);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(6, 3);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(6, FACING_NORTH);
    FieldEvent_Wait(10);
    Event_ShowMessage(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(6, 3);
    FieldEvent_Wait(10);
    Actor_FaceDirectionAndWait(6, FACING_NORTH);
    FieldEvent_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    Event_ShowMessage(6, 0);
  }
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  Event_ShowMessage(0x1a, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  Actor_WalkToAndWait(26, 344, 616);
  FieldEvent_Wait(5);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessage(6, 0);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Event_ShowMessage(0x1b, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(28, FACING_WEST);
  Actor_WalkToAndWait(27, 232, 616);
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x3c);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirectionAndWait(26, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(0x1a, 0);
  Actor_WalkByAndWait(27, 16, 0);
  Event_ShowMessage(0x1b, 0);
  FieldActor_Jump(27, 4, 25);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(27, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(0x1b, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(15);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(6, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0x32);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(5);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0x2a);
  Actor_WalkByAndWait(28, 0, -16);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirectionAndWait(25, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_SetSpeed(28, 1);
  Actor_WalkByAndWait(28, 8, -8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(0x1c, 0);
  Actor_SetSpeed(25, 1);
  Actor_Get(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, -7, 7);
  FieldEvent_Wait(1);
  Actor_Get(25)->actionFlags |= 1;
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x28);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x32);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(0x19,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(26, 2);
  Actor_FaceDirectionAndWait(26, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(0x1a, 0);
  Actor_SetSpeed(25, 0.6);
  Actor_FaceDirectionAndWait(25, FACING_EAST);
  Actor_WalkBy(25, 8, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1b, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1a, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1b, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  FieldEvent_Wait(5);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  FieldEvent_Wait(30);
  Actor_SetSpeed(28, 0.6);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(2);
  Actor_Get(28)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(28, -8, 8);
  FieldEvent_Wait(1);
  Actor_Get(28)->actionFlags |= 1;
  Actor_FaceDirectionAndWait(28, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 28, 0);
  Actor_ShowEmote(0x1a, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0x2a);
  FieldActor_Jump(26, 6, 15);
  FieldActor_Jump(26, 6, 23);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Event_ShowMessage(0x1a, 0);
  FieldActor_Jump(27, 6, 15);
  FieldActor_Jump(27, 6, 23);
  Event_ShowMessage(0x1b, 0);
  Actor_SetSpeed(26, 1.8);
  Actor_WalkByAndWait(26, -12, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(0x1a, 0);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_WINCE, 0x2d);
  Actor_SetSpeed(27, 1.8);
  Actor_WalkByAndWait(27, 12, 0);
  FieldEvent_Wait(5);
  Actor_ShowEmote(0x1b, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(0x1b, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x14);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 6, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(26, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1a, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1b, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0x2a);
  FieldActor_Jump(27, 6, 15);
  FieldActor_Jump(27, 6, 23);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(26, 2);
  Actor_FaceDirectionAndWait(26, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(15);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(26, 2);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(25, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(15);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(27, FACING_EAST);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(28, FACING_WEST);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(4, 27);
  FieldActor_WatchAndSetScript(7, 27);
  FieldActor_WatchAndSetScript(5, 27);
  FieldActor_WatchAndSetScript(6, 27);
  FieldActor_WatchAndSetScript(28, 27);
  Actor_SetSpeed(27, 1);
  Actor_WalkByAndWait(27, 20, 0);
  Actor_WalkByAndWait(27, 2, 32);
  Actor_WalkByAndWait(27, 32, 32);
  Actor_WalkByAndWait(27, 0, 32);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(4, 26);
  FieldActor_WatchAndSetScript(7, 26);
  FieldActor_WatchAndSetScript(5, 26);
  FieldActor_WatchAndSetScript(6, 26);
  FieldActor_WatchAndSetScript(28, 26);
  Actor_SetSpeed(26, 1);
  Actor_WalkByAndWait(26, 0, 32);
  Actor_WalkByAndWait(26, -20, 24);
  Actor_WalkByAndWait(26, 0, 48);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(28, FACING_NORTH_NORTHEAST);
  FieldEvent_Wait(20);
  Actor_SetPosition(26, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  FieldActor_WatchAndSetScript(4, 25);
  FieldActor_WatchAndSetScript(7, 25);
  FieldActor_WatchAndSetScript(5, 25);
  FieldActor_WatchAndSetScript(6, 25);
  FieldActor_WatchAndSetScript(28, 25);
  Actor_SetSpeed(25, 1);
  Actor_WalkByAndWait(25, 22, 24);
  EventCamera_FollowActor(25, 1);
  Actor_WalkByAndWait(25, 0, 32);
  Actor_WalkByAndWait(25, -16, 16);
  Actor_WalkByAndWait(25, 0, 8);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x28);
  Actor_FaceDirectionAndWait(25, FACING_NORTH_NORTHWEST);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 648, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(28, 1);
  Actor_WalkByAndWait(28, 0, 32);
  Actor_FaceDirectionAndWait(28, FACING_SOUTH_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(0x1c, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(0x1c, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  FieldEvent_Wait(25);
  Actor_SetSpeed(28, 0.4);
  Actor_Get(28)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(28, 0, -24);
  FieldEvent_Wait(1);
  Actor_Get(28)->actionFlags |= 1;
  FieldEvent_Wait(10);
  Actor_SetAnimation(28, 8);
  FieldEvent_Wait(30);
  Event_ShowMessage(0x1c, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(5, 2);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(0x19, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 25, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_WalkByAndWait(25, 0, 80);
  Actor_SetPosition(25, 0, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 624, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Event_PlaySound(MUS_003);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0x1e);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(28, 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(28, 0.8);
  Actor_WalkByAndWait(28, 0, 16);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(5);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(7, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(45);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 6, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(6, 1);
  FieldEvent_Wait(25);
  Call2(Champa_JennaAndShebaGlare, 5, 6);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x28);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_SetAnimation(5, 4);
  Actor_SetAnimationAndWait(6, 4);
  FieldEvent_ShowTwoMessagesAndWait(5, 5, 5, 0x12, 8, 6, 6, 0xe, 8, 9, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(6, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x1e);
  Call11(Champa_ShowTwoMessages, 5, 8, 5, 0x12, 8, 6, 8, 0xe, 8, 9, 0);
  Actor_FaceDirectionAndWait(4, FACING_SOUTHWEST);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Champa_CloseTwoMessages(5, 6);
    FieldEvent_Wait(15);
    Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
    Event_ShowMessage(0x1c, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
  } else {
    Champa_CloseTwoMessages(5, 6);
    FieldEvent_Wait(35);
    Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0x1e);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId =
        (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId + 1);
    Event_ShowMessage(0x1c, 0);
  }
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SCOWL, 0x2d);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  FieldActor_Jump(5, 6, 15);
  FieldActor_Jump(5, 6, 23);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  FieldActor_Jump(6, 6, 15);
  FieldActor_Jump(6, 6, 23);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 6, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x28);
  Actor_WalkByAndWait(28, 0, -8);
  Actor_FaceDirectionAndWait(28, FACING_EAST);
  FieldEvent_Wait(5);
  Event_ShowMessage(0x1c, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_SMILE, 0x1e);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_WINCE, 0x1e);
  Event_ShowMessage(0x1c, 0);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  FieldEvent_Wait(5);
  Actor_ShowEmote(0x1c, EMOTE_IN_FRONT | EMOTE_QUESTION, 0x2d);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(15);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 1.2);
  Actor_SetAnimation(5, 2);
  Actor_HeadToward(5, gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 1.2);
  Actor_SetAnimation(6, 2);
  Actor_HeadToward(6, gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(7, 1.2);
  Actor_SetAnimation(7, 2);
  Actor_HeadToward(7, gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(28, 1.2);
  Actor_SetAnimation(28, 2);
  Actor_HeadToward(28, gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(28);
  Actor_SetPosition(28, 0, 0);
  Event_End();
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 280, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(182),
    SCENE_EXIT_IF(1, 183, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 183, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 183, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 183, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 183, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 183, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 184, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 185, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 2, 40, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 186, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 187, 19, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

// clang-format off
s16 gChampa_ActorPlacements[] = {
    407, -1, 1, 0, 0, 200, 0, 0,
    0, 408, FACING_SOUTH, 2, 407, -1, 1, 0,
    0, 344, 0, 0, 0, 408, FACING_SOUTH, 258,
    461, -1, 1, 0, 0, 240, 0, 0,
    0, 456, FACING_SOUTH, 2, 461, -1, 1, 0,
    0, 368, 0, 0, 0, 456, FACING_SOUTH, 258,
    115, -1, 2, 0, 0, 296, 0, 0,
    0, 408, FACING_WEST, 0, 116, -1, 1, 0,
    0, 168, 0, 0, 0, 392, 0x3000, 1,
    114, -1, 2, 0, 0, 328, 0, 0,
    0, 312, 0x5000, 0, 128, -1, 2, 0,
    0, 432, 0, 0, 0, 312, FACING_WEST, 0,
    112, -1, 1, 0, 0, 328, 0, 0,
    0, 280, FACING_WEST, 1, 113, -1, 1, 0,
    0, 312, 0, 0, 0, 280, 0, 1,
    136, -1, 8, 0, 0, 440, 0, 0,
    0, 392, 0, 1, 138, -1, 1, 0,
    0, 440, 0, 0, 0, 408, 0, 0,
    39, -1, 1, 0, 0, 216, 0, 0,
    0, 280, 0, 1, 39, -1, 1, 0,
    0, 216, 0, 0, 0, 296, 0, 1,
    39, -1, 1, 0, 0, 248, 0, 0,
    0, 280, FACING_WEST, 1, 255, -1, 1, 0,
    0, 456, 0, 0, 0, 392, FACING_WEST, 1,
    116, -1, 1, 0, 0, 904, 0, 0,
    0, 344, 0x3000, 1, 57, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    23, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 1, 33, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    56, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_NORTH, 2, 5, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    6, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, 7, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_NORTH, 2,
    63, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_NORTH, 2,
    489, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

s16 gChampa_ActorPlacementsOnceBriggsSailed[] = {
    407, -1, 1, 0, 0, 200, 0, 0,
    0, 408, FACING_SOUTH, 2, 407, -1, 1, 0,
    0, 344, 0, 0, 0, 408, FACING_SOUTH, 258,
    461, -1, 1, 0, 0, 240, 0, 0,
    0, 456, FACING_SOUTH, 2, 461, -1, 1, 0,
    0, 368, 0, 0, 0, 456, FACING_SOUTH, 258,
    115, -1, 1, 0, 0, 264, 0, 0,
    0, 424, 0x3000, 1, 116, -1, 1, 0,
    0, 168, 0, 0, 0, 392, 0x3000, 1,
    114, -1, 2, 0, 0, 328, 0, 0,
    0, 312, 0x5000, 0, 128, -1, 2, 0,
    0, 432, 0, 0, 0, 312, FACING_WEST, 0,
    112, -1, 1, 0, 0, 328, 0, 0,
    0, 280, FACING_WEST, 1, 113, -1, 1, 0,
    0, 312, 0, 0, 0, 280, 0, 1,
    136, -1, 1, 0, 0, 328, 0, 0,
    0, 400, 0x5000, 0, 138, -1, 1, 0,
    0, 424, 0, 0, 0, 392, FACING_WEST, 0,
    39, -1, 1, 0, 0, 264, 0, 0,
    0, 392, 0x3000, 1, 39, -1, 1, 0,
    0, 272, 0, 0, 0, 472, 0xb000, 1,
    39, -1, 1, 0, 0, 320, 0, 0,
    0, 472, 0xd000, 1, 255, -1, 1, 0,
    0, 456, 0, 0, 0, 392, FACING_WEST, 1,
    116, -1, 1, 0, 0, 904, 0, 0,
    0, 344, 0x3000, 1, 57, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    23, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 1, 33, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    56, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_NORTH, 2, 5, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    6, -1, 1, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, 7, -1, 1, 0,
    0, -56, 0, 0, 0, -56, FACING_NORTH, 2,
    63, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, FACING_NORTH, 2,
    489, -1, 1, 0, 0, 0, 0, 0,
    0, 0, FACING_NORTH, 2, 39, -1, 1, 0,
    0, 184, 0, 0, 0, 296, 0xb000, 3,
    39, -1, 1, 0, 0, 152, 0, 0,
    0, 296, 0xd000, 3, 16, -1, 1, 0,
    0, 168, 0, 0, 0, 264, 0x5000, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gChampa_NoDoorEdit[] = {
    -1,
};

s16 gChampa_DoorEdit[] = {
    36, 2, 2, 2, 6, 36, 4, 2,
    2, 6, -1,
};
// clang-format on

DEFINE_G_DATA_0200064C(gChampa_Doors, gChampa_DoorEdit, 0x9001d,
                       gChampa_NoDoorEdit, 0)

struct Trigger gChampa_Triggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 11, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc403, 24, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgChampa53)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, 100, FLAG_FEIZHI_CAME, EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, 19, FLAG_NEAR_THE_CLIFF, EVENT(Champa_ApproachTheCliff)},
    {TRIGGER_STEP, 20, FLAG_CAUGHT_UP_WITH_BRIGGS,
     EVENT(Champa_CatchUpWithBriggs)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgChampa01)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgChampa02)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgChampa03)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgChampa04)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Champa_AskAboutAnkohlRuins)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgChampa06)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Champa_TalkToPracticingBoy)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgChampa08)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgChampa09)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgChampa10)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Champa_SailorsNotWelcome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgChampa12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgChampa13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgChampa14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgChampa15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgChampa16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgChampa17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgChampa18)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgChampa19)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgChampa20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgChampa21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgChampa22)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgChampa23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgChampa24)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Champa_TakeFeizhisGift)},
    {TRIGGER_TABLE_END},
};

struct Trigger gChampa_TriggersOnceBriggsSailed[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc602, 11, CONDITION_ALWAYS, EVENT(Champa_EnterDoor)},
    {0xc403, 24, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgChampa53)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Champa_RevealTreasureSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Champa_HideTreasureSpot)},
    {TRIGGER_STEP, 100, FLAG_FEIZHI_CAME, EVENT(Champa_FeizhiArrives)},
    {TRIGGER_STEP, 19, FLAG_NEAR_THE_CLIFF, EVENT(Champa_ApproachTheCliff)},
    {TRIGGER_STEP, 20, FLAG_CAUGHT_UP_WITH_BRIGGS,
     EVENT(Champa_CatchUpWithBriggs)},
    {TRIGGER_STEP, 26, FLAG_SAILORS_SPOTTED_FELIX,
     EVENT(Champa_SailorsSpotFelix)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgChampa26)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgChampa27)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgChampa28)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Champa_TellOfBriggsTreasure)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Champa_AskAboutAnkohlRuins)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgChampa31)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgChampa32)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgChampa33)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgChampa34)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Champa_TellOfObaba)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgChampa37)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgChampa38)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgChampa39)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgChampa40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgChampa41)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgChampa42)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgChampa43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgChampa44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgChampa45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgChampa46)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgChampa47)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgChampa48)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgChampa50)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgChampa51)},
    {TRIGGER_TALK, 31, CONDITION_ALWAYS, EVENT(Champa_TakeFeizhisGift)},
    {TRIGGER_TABLE_END},
};

u32 gChampa_GlareSparkScript[] = {
    38,
};
