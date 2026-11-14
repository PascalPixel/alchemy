/// @file
/// Atteka Inlet, where the party's ship lies on the southern continent, in two
/// scenes that share this overlay: the harbour with the ship
/// (SceneId_AttekaInlet1) and the shore beside it (SceneId_AttekaInlet2). Piers
/// asks whether to set sail (AttekaInlet_AskPiersToSetSail). After Jupiter
/// Lighthouse the shipwrights stop the party to see their handiwork fly
/// (AttekaInlet_ShipwrightsStopTheParty), Hamma shows the ship's new wings
/// (AttekaInlet_HammaShowsTheWings), and in a game linked with The Broken Seal
/// a messenger brings Master Hammet's gift
/// (AttekaInlet_HammetsMessengerArrives). Cyclone clears the shore's bushes and
/// uncovers Geode, a Venus Djinni (AttekaInlet_CycloneUncoversGeode).

#include "flags.h"
#include "system/io_reg.h"
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
#include "scene_tables.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "field_event.h"
#include "field_sprite.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "text/msg_ids.h"

// The inlet's own story, which no other scene reads.
#define FLAG_SHIPWRIGHTS_STOPPED_PARTY                                         \
  0x9b0 ///< "We worked really hard on that ship"
#define FLAG_HAMMA_SHOWED_THE_WINGS 0x9b1 ///< Hamma showed the ship's wings
#define FLAG_SET_OUT_WITH_WINGS 0x9b2 ///< the party set out on the winged ship
#define FLAG_HAMMETS_GIFT_DELIVERED 0x9ad ///< Master Hammet's messenger came

extern struct ActorPlacement gAttekaInlet_ActorPlacements[];
extern u32 gAttekaInlet_HarbourActorPlacements[];
extern struct ActorPlacement gAttekaInlet_HarbourActorPlacementsOnceWinged[];
extern u32 gAttekaInlet_HarbourActorPlacementsOnceFlown[];
extern struct ActorPlacement gAttekaInlet_ShoreActorPlacementsOnceWinged[];
extern struct ActorPlacement gAttekaInlet_ShoreActorPlacements[];
extern struct Trigger gAttekaInlet_Triggers[];
extern struct Trigger gAttekaInlet_ShoreTriggers[];
void AttekaInlet_HammetsMessengerArrives(void);
void AttekaInlet_SetUpTheHarbour(void);
void AttekaInlet_SetUpTheShore(void);

void AttekaInlet_GeodeJoins(void) {
  Djinn_JoinParty(14, 0, 15);
  GameFlag_Clear(FLAG_AREA_TEMP_00);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AttekaInlet1) {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      return gAttekaInlet_HarbourActorPlacementsOnceFlown;
    if (GameFlag_Get(0x9d0))
      return gAttekaInlet_HarbourActorPlacementsOnceWinged;
    return gAttekaInlet_HarbourActorPlacements;
  }
  if (scene == (s32)&SceneId_AttekaInlet2) {
    if (GameFlag_Get(0x9d0))
      return gAttekaInlet_ShoreActorPlacementsOnceWinged;
    return gAttekaInlet_ShoreActorPlacements;
  }
  return gAttekaInlet_ActorPlacements;
}

extern struct Trigger gAttekaInlet_HarbourTriggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AttekaInlet1)
    return gAttekaInlet_HarbourTriggers;
  if (scene == (s32)&SceneId_AttekaInlet2)
    return gAttekaInlet_ShoreTriggers;
  return gAttekaInlet_Triggers;
}

/// Object 18 asks whether to set out: a yes sets the owner override and, once
/// flag 0x9d0 is set, flags FLAG_SHIP_FLIES and FLAG_SET_OUT_WITH_WINGS; a no
/// counts the refusal and object 18 walks back to the party leader.
void AttekaInlet_AskPiersToSetSail(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(0x9d0)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(4);
  } else {
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 1.6);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_SetDestinationById(18, *(s16 *)((u8 *)object + 10),
                               *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    GameFlag_Set(FLAG_SCENE_TEMP_01);
    Event_End();
  }
}

/// Once flag 0x9d0 is set and while flag FLAG_SET_OUT_WITH_WINGS is clear,
/// objects 9 and 10 are placed, jump in and talk, four cells of the map change
/// and flag FLAG_SHIPWRIGHTS_STOPPED_PARTY is set.
void AttekaInlet_ShipwrightsStopTheParty(void) {
  if (GameFlag_Get(0x9d0) && !GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveWithinMapBounds(96 << 16, -1, 308 << 16, 1);
    {
      s32 angle = 208;

      Actor_SetPositionAndFacing(9, 60 << 16, 362 << 16, angle << 8);
    }
    wait(1);
    Actor_SetSpeed(9, 2);
    Actor_Jump(9, 6, 0);
    Actor_MoveToAndWait(9, 64, 336);
    Actor_WalkToAndWait(9, 70, 308);
    Actor_ArmCallback(9, FACING_EAST, 0);
    {
      s32 angle = 160;

      Actor_SetPositionAndFacing(10, 94 << 16, 272 << 16, angle << 7);
    }
    wait(1);
    Actor_SetSpeed(10, 2);
    Actor_Jump(10, 6, 0);
    Actor_MoveToAndWait(10, 90, 288);
    Actor_WalkToAndWaitById(10, 80, 316);
    Actor_FaceDirectionById(10, FACING_EAST);
    Actor_GetById(9)->velocityY = px(4);
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhereDoYouThink);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 9, 0);
    Actor_ShowEmoteById(10, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Event_ShowMessageAndWait(10, 0);
    Actor_FaceDirection(9, FACING_SOUTH_SOUTHEAST, 0);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 9, 0);
    Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHWEST);
    Actor_SetModeById(10, 3);
    Event_ShowMessageAndWait(10, 0);
    Actor_ArmCallback(10, FACING_EAST, 0);
    Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_ArmCallback(9, FACING_EAST, 20);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 9, 0);
    Event_ShowMessageAndWait(10, 0);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    GameFlag_Set(FLAG_SHIPWRIGHTS_STOPPED_PARTY);
    Event_End();
  }
}

/// Once flag 0x9d0 is set, the camera pans over object 23, who greets the
/// party, turns, talks and walks to its place; flag FLAG_HAMMA_SHOWED_THE_WINGS
/// is set.
void AttekaInlet_HammaShowsTheWings(void) {
  if (GameFlag_Get(0x9d0)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(520, CAMERA_KEEP, 372, 1);
    FieldCamera_WaitForArrival();
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(23, FACING_SOUTH_SOUTHEAST, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 23, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(440, 32, 368, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    FieldCamera_MoveTo(432, 32, 232, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveWithinMapBounds(px(520), px(32), px(372), 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Actor_FaceDirectionAndWait(23, FACING_SOUTH_SOUTHEAST);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmote(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Event_ShowMessageAndWait(23, 0);
    Actor_FaceDirectionAndWait(23, FACING_NORTH_NORTHWEST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 23, 0);
    Actor_FaceDirectionAndWait(23, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetModeById(23, 4);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_ShowMessageAndWait(23, 0);
    Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageAndWait(23, 0);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    Actor_SetSpeed(23, 0.8);
    Actor_GetById(23)->actionFlags &= ~1;
    Actor_WalkToAndWaitById(23, 504, 360);
    Event_Wait(1);
    Actor_GetById(23)->actionFlags |= 1;
    Actor_FaceDirectionById(23, FACING_SOUTH_SOUTHEAST);
    Motion_SetModeAndWaitAnimation(23, 3);
    Event_ShowMessageAndWait(23, 0);
    GameFlag_Set(FLAG_HAMMA_SHOWED_THE_WINGS);
    Event_End();
  }
}

void Actor_MoveToObject(s32 id, s32 target);

/// Object 18 comes to the party leader and asks whether to set out, as in
/// AttekaInlet_AskPiersToSetSail; after a no the leader steps back.
void AttekaInlet_PiersAsksToSetSail(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(18, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(18, 0.8);
  Actor_WalkToAndWaitById(18, 532, 248);
  Actor_FaceDirectionById(18, FACING_SOUTH_SOUTHEAST);
  Event_SetMessageId(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Event_ShowMessageAndWait(18, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(0x9d0)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(4);
  } else {
    (((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)++;
    Event_ShowMessageAndWait(18, 0);
    Actor_SetSpeed(18, 1.6);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != NULL)
      Actor_SetDestinationById(18, *(s16 *)((u8 *)object + 10),
                               *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 528, 276);
    Event_End();
  }
}

void TileCopies_CopyTouchedCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 layer);

static inline void MapCopy(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                           s32 destY, s32 layer) {
  TileCopies_CopyTouchedCells(srcX, srcY, width, height, destX, destY, layer);
}

void AttekaInlet_CheckForHammetsMessenger(void) {
  if (GameFlag_Get(0x820) && GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS) &&
      !GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    AttekaInlet_HammetsMessengerArrives();
}

void AttekaInlet_CycloneUncoversGeode(void) {
  struct Actor *object;
  s32 moved;

  MapCopy(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Get(63)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    Event_Start();
    Event_ResolvePendingActions(0);
    moved = 0;
    if ((object->x >> 20) == 7 && (object->z >> 20) == 23) {
      Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
      Actor_SetDestination(gMainRoutineWork.currentOwner, 104, 376);
      moved = 1;
    }
    Actor_SetHorizontalPositionWithTerrain(14, 120 << 16, 376 << 16);
    wait(1);
    Actor_Jump(14, 4, 10);
    GameFlag_Set(FLAG_AREA_TEMP_00);
    if (moved)
      Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_EAST);
    Event_End();
  }
}

void AttekaInlet_CycloneFirstBush(void) { MapCopy(9, 23, 1, 1, 4, 10, 0); }

void AttekaInlet_CycloneSecondBush(void) { MapCopy(9, 23, 1, 1, 6, 9, 0); }

void AttekaInlet_CycloneThirdBush(void) { MapCopy(9, 23, 1, 1, 6, 6, 0); }

void AttekaInlet_CycloneFourthBush(void) { MapCopy(12, 5, 1, 1, 12, 6, 0); }

void AttekaInlet_CycloneFifthBush(void) { MapCopy(9, 23, 1, 1, 13, 7, 0); }

void AttekaInlet_CycloneSixthBush(void) { MapCopy(9, 23, 1, 1, 14, 6, 0); }

void AttekaInlet_CycloneSeventhBush(void) { MapCopy(9, 23, 1, 1, 21, 6, 0); }

void AttekaInlet_CycloneEighthBush(void) { MapCopy(9, 23, 1, 1, 22, 6, 0); }

void AttekaInlet_CycloneNinthBush(void) { MapCopy(9, 23, 1, 1, 23, 7, 0); }

/// Object 24 calls out to the party, jumps down to it and talks with object
/// 23; flag FLAG_HAMMETS_GIFT_DELIVERED is set.
void AttekaInlet_HammetsMessengerArrives(void) {
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  angle = 0xd000;
  Actor_SetHorizontalPositionAndAngleWithTerrain(24, px(504), px(460), angle);
  wait(1);
  Event_SetMessageId((s32)MsgHeyWait2);
  Event_ShowMessageAndWait(24, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, CAMERA_KEEP, 380, 1);
  Actor_SetSpeed(24, 1.6);
  Actor_WalkToAndWait(24, 520, 432);
  Actor_WalkToAndWait(24, 520, 388);
  Actor_FaceDirection(23, FACING_SOUTH_SOUTHEAST, 0);
  Event_ShowMessageAndWait(24, 0);
  Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmote(24, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_ShowMessageAndWait(24, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Actor_FaceDirectionById(24, angle);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 384);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Motion_SetVarCbAndRefresh(24, 2);
  Actor_SetHorizontalPositionWithTerrain(65, px(520), px(380));
  wait(1);
  Actor_GetById(24)->actionFlags &= ~1;
  Actor_WalkToAndWait(24, 520, 388);
  Event_Wait(1);
  Actor_GetById(24)->actionFlags |= 1;
  Event_Wait(10);
  Actor_FaceDirection(24, FACING_NORTH_NORTHWEST, 10);
  Motion_SetModeAndWaitAnimation(24, 3);
  Actor_WalkToAndWait(24, 520, 434);
  FieldCamera_MoveWithinMapBounds(px(512), -1, px(348), 1);
  Actor_WalkToAndWait(24, 504, 460);
  Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Actor_FaceDirectionById(23, angle);
  Event_ShowMessageAndWait(23, 0);
  Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_ShowMessageAndWait(23, 0);
  Actor_SetModeById(23, 3);
  Event_ShowMessageAndWait(23, 0);
  Motion_SetModeAndWaitAnimation(23, 3);
  Actor_ShowEmoteById(23, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_ShowMessageAndWait(23, 0);
  GameFlag_Set(FLAG_HAMMETS_GIFT_DELIVERED);
  Event_End();
}

void AttekaInlet_ShareLeadersPriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(16, variant);
  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}

void AttekaInlet_ShareLeadersPriorityWithRock(void) {
  Actor_SetSpritePriority(
      13, ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
               ->sprite)
              ->priority);
}

/// Rotates palette colours 5-9 and 13-15 by one entry every eight frames.
void AttekaInlet_RotateWaterColors(void) {
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(11, 9);
    index = 9;
  first:
    palette = (u16 *)&BG_PLTT_COLOR(11, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 4)
      goto first;
    BG_PLTT_COLOR(11, 4) = saved;
  }
  if ((*Ram_FrameCount & 7) == 0) {
    volatile u16 *palette;
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(11, 15);
    index = 15;
  second:
    palette = (u16 *)&BG_PLTT_COLOR(11, 0);
    palette[index] = (palette - 1)[index];
    index--;
    if (index != 12)
      goto second;
    BG_PLTT_COLOR(11, 12) = saved;
  }
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void AttekaInlet_OpenDoorAndEnter(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
  Event_Wait(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(((struct EventWork *)work)->exit);
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AttekaInlet1)
    AttekaInlet_SetUpTheHarbour();
  else if (scene == (s32)&SceneId_AttekaInlet2)
    AttekaInlet_SetUpTheShore();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AttekaInlet1 && GameFlag_Get(0x9d0)) {
    MapGrid_CopyCellPictureIndicesRect(54, 47, 4, 15, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 38, 73, 7, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 43, 21, 55, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(65, 43, 21, 20, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(4, 47, 4, 50, 10, 3);
  }
  return 0;
}

static __inline__ void Task_Add_(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline. */
  Task_Add(func, priority);
}

void AttekaInlet_SetUpTheHarbour(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party;

  GameFlag_Set(0x9b4);
  if (!GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    Actor_SetHorizontalPositionWithTerrain(65, 0, 0);
  map->mode = 1;
  party = &gMainRoutineWork;
  Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  Task_Add_(AttekaInlet_RotateWaterColors, 0x480);
  Task_Add_(AttekaInlet_ShareLeadersPriority, 0x480);
  Actor_SetSpriteFlags(Actor_GetById(17), 0);
  *((u8 *)Actor_GetById(17) + 85) = 0;
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS)) {
      s32 speed = 0xc0;

      Actor_SetPositionAndFacing(23, px(504), px(360), speed << 6);
    }
    Actor_WatchAndSetScript(19, party->currentOwner);
    Actor_WatchAndSetScript(20, party->currentOwner);
    Actor_WatchAndSetScript(21, party->currentOwner);
    Actor_WatchAndSetScript(22, party->currentOwner);
  }
}

void AttekaInlet_SetUpTheShore(void) {
  if (gMainRoutineWork.entrance == 3)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  } else if (GameFlag_Get(FLAG_SHIPWRIGHTS_STOPPED_PARTY)) {
    Actor_SetPositionAndFacing(9, 70 << 16, 308 << 16, FACING_EAST);
    Actor_SetPositionAndFacing(10, 80 << 16, 316 << 16, FACING_EAST);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    wait(1);
  }
  if (GameFlag_Get(FLAG_AREA_TEMP_00))
    Actor_SetPosition(14, 120, 376);
  {
    s32 priority = 0x480;

    Task_Add(AttekaInlet_ShareLeadersPriorityWithRock, priority);
  }
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 72, 0, 295, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AttekaInlet1),
    SCENE_EXIT(1, LemurianShip1, 1),
    SCENE_EXIT(2, AttekaInlet2, 1),
    SCENE_EXIT(3, AttekaInlet2, 5),
    SCENE_EXIT(4, WorldMap, 52),
    SCENE_EXITS(AttekaInlet2),
    SCENE_EXIT(1, AttekaInlet1, 2),
    SCENE_EXIT(2, WorldMap, 53),
    SCENE_EXIT(3, Contigo2, 21),
    SCENE_EXIT(4, AttekaInlet2, 4),
    SCENE_EXIT(5, AttekaInlet1, 3),
    SCENE_EXIT(6, WorldMap, 53),
    SCENE_EXITS_END,
};

struct ActorPlacement gAttekaInlet_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

u32 gAttekaInlet_HarbourActorPlacements[] = {
    0xffff014d, 7, 0x580000,  0, 0x1780000, 0x24000,
    0xffff014d, 7, 0xd80000,  0, 0x1480000, 0x1024000,
    0xffff014d, 7, 0xa80000,  0, 0xb80000,  0x1024000,
    0xffff014d, 7, 0x1e80000, 0, 0x1980000, 0x1024000,
    0xffff014d, 7, 0x2280000, 0, 0x1a80000, 0x1024000,
    0xffff0073, 1, 0x980000,  0, 0x1280000, 0x12000,
    0xffff0073, 1, 0x500000,  0, 0x1300000, 0x1c000,
    0xffff0074, 1, 0xd00000,  0, 0x1180000, 0x18000,
    0xffff006a, 1, 0xb80000,  0, 0xe00000,  0x14000,
    0x190a01a4, 1, 0x1d00000, 0, 0x1080000, 0x20000,
    0xffff0038, 1, 0,         0, 0,         0x4000,
    0xffff,     0, 0,         0, 0,         0,
};

struct ActorPlacement gAttekaInlet_HarbourActorPlacementsOnceWinged[] = {
    ACTOR_PLACEMENT(333, CONDITION_ALWAYS, 7, 88, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(333, CONDITION_ALWAYS, 7, 216, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(333, CONDITION_ALWAYS, 7, 168, 0, 184, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(333, CONDITION_ALWAYS, 7, 488, 0, 408, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(333, CONDITION_ALWAYS, 7, 552, 0, 424, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 168,
                    FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 55, 0, 234,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 260, 0, 158,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(420, IF_FLAG_SET(0x90a), MOVEMENT_TYPE_STAND, 464, 0, 264,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 424,
                    FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 424,
                    FACING_WEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 408,
                    FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 408,
                    FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(59, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 516, 0, 360,
                    FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH,
                    0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

u32 gAttekaInlet_HarbourActorPlacementsOnceFlown[] = {
    0xffff014d, 7, 0x580000,  0, 0x1780000, 0x24000,
    0xffff014d, 7, 0xd80000,  0, 0x1480000, 0x1024000,
    0xffff014d, 7, 0xa80000,  0, 0xb80000,  0x1024000,
    0xffff014d, 7, 0x1e80000, 0, 0x1980000, 0x1024000,
    0xffff014d, 7, 0x2280000, 0, 0x1a80000, 0x1024000,
    0xffff0073, 1, 0xa20000,  0, 0x12c0000, 0x12000,
    0xffff0073, 1, 0x6f0000,  0, 0x1470000, 0x1c000,
    0xffff0074, 1, 0,         0, 0,         0x18000,
    0xffff006a, 3, 0xcc0000,  0, 0xcc0000,  0x4000,
    0x190a01a4, 1, 0x1d00000, 0, 0x1080000, 0x20000,
    0xffff0038, 1, 0,         0, 0,         0x4000,
    0xffff,     0, 0,         0, 0,         0,
};

struct ActorPlacement gAttekaInlet_ShoreActorPlacementsOnceWinged[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 264, 0,
                    352, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 380, 0, 280,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(420, IF_FLAG_SET(0x90a), MOVEMENT_TYPE_STAND, 464, 0, 264,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(329, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 392,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(243, 0x3f, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAttekaInlet_ShoreActorPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 0, 0, 0,
                    FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(420, IF_FLAG_SET(0x90a), MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(329, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 392,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(243, 0x3f, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAttekaInlet_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gAttekaInlet_HarbourTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(AttekaInlet_OpenDoorAndEnter)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 10, FLAG_HAMMA_SHOWED_THE_WINGS,
     EVENT(AttekaInlet_HammaShowsTheWings)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(AttekaInlet_AskPiersToSetSail)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(AttekaInlet_PiersAsksToSetSail)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CheckForHammetsMessenger)},
    {TRIGGER_TALK, 13, 0x9d0, MSGNO(MsgWeVeMade)},
    {TRIGGER_TALK, 13, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgHammaReturnedTo)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgIVeBeenWatching)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x9d0, MSGNO(MsgWeFirstHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgWeVeBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgWheneverILook)},
    {TRIGGER_TALK, 14, 0x9d0, MSGNO(MsgWeMustBe)},
    {TRIGGER_TALK, 14, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWasYourBoat)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIWonderWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x9d0, MSGNO(MsgTheseWingsAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgWhoBuiltThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgIWishI)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgWeBroughtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIDoubtAnyone)},
    {TRIGGER_TALK, 16, 0x9d0, MSGNO(MsgIWonderIfThese)},
    {TRIGGER_TALK, 16, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgAMassiveRock)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgYouReGoingNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x9d0, MSGNO(MsgItSKind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgIfThatBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIVeDecided)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgItSAlmost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgTheDayHas)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgIfEveryoneWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgTheAnemosWere)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWhenTheWinged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgTheseWarriorsWill)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgOnceYouRemove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIHearThatThe)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgUseYourPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgTravelSafelyThe)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAttekaInlet_ShoreTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 10, FLAG_SHIPWRIGHTS_STOPPED_PARTY,
     EVENT(AttekaInlet_ShipwrightsStopTheParty)},
    {TALK_TRIGGER_FACING(FACING_EAST), 14, CONDITION_ALWAYS,
     EVENT(AttekaInlet_GeodeJoins)},
    {TRIGGER_TALK, 8, FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgEveryoneSWaiting)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAllOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgHammaIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneInContigo)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheVillagersAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgItSAmazing)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHammaWouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgHammaSaysThat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWhenPeopleWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgWeCanT)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 20,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneUncoversGeode)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 21,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneFirstBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 22,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneSecondBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 23,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneThirdBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 24,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneFourthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 25,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneFifthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 26,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneSixthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 27,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneSeventhBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 28,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneEighthBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 29,
     CONDITION_ALWAYS, EVENT(AttekaInlet_CycloneNinthBush)},
    {TRIGGER_TABLE_END},
};
