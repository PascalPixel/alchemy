/// @file
/// The caves in Champa's cliff, home of Briggs' grandmother Obaba, in four
/// rooms of this overlay: Obaba waits for her grandson
/// (ChampaCliffCaves_ObabaAsksForBriggs), the party fights Briggs' Avimander
/// (ChampaCliffCaves_FightAvimander) and Briggs surrenders, and Obaba forges
/// the three prongs into the Trident (ChampaCliffCaves_ObabaForgesTrident).
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "resource_ids.h"
#include "types.h"
#include "system/io_reg.h"
#include "item_ids.h"
#include "flags.h"
#include "field/actor_script.h"

/// Set once Avimander is beaten and Briggs gives in.
#define FLAG_CHAMPA_AVIMANDER_BEATEN 0x97e
/// Obaba hands the party pieces of the trident one by one: set as she sees the
/// first prong, the second, and as she forges the three into the Trident.
#define FLAG_OBABA_SAW_FIRST_PRONG 0x975
#define FLAG_OBABA_SAW_SECOND_PRONG 0x976
#define FLAG_TRIDENT_FORGED 0x977
/// The Trident has been picked up from Obaba's forge.
#define FLAG_TRIDENT_TAKEN 0x978
/// Set as Briggs first stops the party at Avimander's room ("Hold it right
/// there!"); on a later visit he greets them with "Back for more?".
#define FLAG_CHAMPA_BRIGGS_STOPPED_PARTY 0x974

/// Where the party comes back to the Avimander's room after losing to it
/// (Event_SetDefeatDestination) or beating it, as The Broken Seal's Altin
/// Peak names its own.
enum { ENTRANCE_AFTER_DEFEAT = 98, ENTRANCE_AFTER_BATTLE = 99 };

#include "field/scene_tables.h"

/// Obaba's room, numbering gChampaCliffCaves_AvimanderPlacementsBriggsHome and
/// the room's other tables to ACTOR_HELD_ITEM.
enum ChampaCliffCavesObabasRoomActorId {
  ACTOR_OBABA = ACTOR_FIRST_PLACED,
  /// The prong or the Trident shown as an item icon in Obaba's hands.
  ACTOR_HELD_ITEM = ACTOR_FIRST_PLACED + 2,
  ACTOR_BRIGGS,
  /// Briggs's pet salamander, and Avimander as it grows to fight.
  ACTOR_SALAMANDER,
  ACTOR_AVIMANDER,
  ACTOR_CHAUCHA,
  ACTOR_KRADEN
};

/// Briggs's home once Avimander is beaten
/// (gChampaCliffCaves_ObabaPlacementsAfterAvimander).
enum ChampaCliffCavesBriggsHomeActorId {
  /// Briggs, lying in bed.
  ACTOR_BRIGGS_IN_BED = ACTOR_FIRST_PLACED
};
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/scene_call.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "graphics/resource.h"
#include "system/system.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_ids.h"
#include "version.h"

#include "field/call.h"
#include "field/actor.h"
#include "field/flag_call.h"
#include "system/dma.h"
#include "system/task.h"
#include "macros.h"
#include "text/msg_ids.h"

/// A door's opening: the map edit that draws it and the cell it plays at.
struct DoorEdit {
  const s16 *frames;
  u16 x;
  u16 z;
};

extern LATE_DATA struct DoorEdit gChampaCliffCaves_FirstDoor;
extern LATE_DATA struct DoorEdit gChampaCliffCaves_SecondDoor;

void ChampaCliffCaves_OpenDoorAndEnter(const struct DoorEdit *edit, s32 door,
                                       s32 mode);
void Event_PickUpItem(s32 a, s32 b, s32 c);
void Actor_WatchActor(struct Actor *actor, struct Actor *target);

extern struct ActorPlacement gChampaCliffCaves_BriggsPlacements[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacements[];
extern struct ActorPlacement gChampaCliffCaves_BriggsPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_ObabaPlacementsBriggsHome[];
extern struct ActorPlacement gChampaCliffCaves_BriggsPlacementsAfterAvimander[];
extern struct ActorPlacement gChampaCliffCaves_HousesPlacementsAfterAvimander[];
extern struct ActorPlacement gChampaCliffCaves_ObabaPlacementsAfterAvimander[];
extern struct Trigger gChampaCliffCaves_HousesTriggers[];
extern struct Trigger gChampaCliffCaves_ObabaTriggers[];
extern struct Trigger gChampaCliffCaves_AvimanderTriggers[];

void ChampaCliffCaves_AskIfBriggsLearned(void);
void ChampaCliffCaves_BriggsMournsSalamander(void);
void ChampaCliffCaves_HandPieceToObaba(s32 value);
void ChampaCliffCaves_ObabaStepsForward(s32 mode);

void ChampaCliffCaves_FlareAndDieDown(void);
void ChampaCliffCaves_ObabaSeesAWeapon(s32 item);
void ChampaCliffCaves_ObabaAsksForSecondPiece(s32 item);
void ChampaCliffCaves_StartFlash(s32 id, s32 mode);
/// A scale of the Champa forge's glow, 16.16; a union, so that a store to it
/// keeps its place after the sprite's (ChampaCliffCaves_LoadDisc).
union ChampaCliffCavesScale {
  FIXED value;
  u16 half;
};

/// The work of the glow over Obaba's forge (heap slot HEAP_SLOT_MENU): the
/// pictures of the glowing disc and the column of fire, their sprites at the
/// forge's spot, and their scales, each one's height trailing its width.
struct ChampaCliffCavesWork {
  u8 tiles[64 * 64]; ///< the larger picture, the disc's 64 by 64 pixels
  s32 position[3];
  struct EffectSprite disc;
  struct EffectSprite column;
  s16 discEntry;
  u16 discVram;
  s16 columnEntry;
  u16 columnVram;
  union ChampaCliffCavesScale discScale;
  union ChampaCliffCavesScale discTrailingScale;
  union ChampaCliffCavesScale columnScale;
  union ChampaCliffCavesScale columnTrailingScale;
  s16 frame;
};

/// The state the forge glow's steps end on.
enum { GLOW_DONE = 999 };

void ChampaCliffCaves_LoadDisc(void);
void ChampaCliffCaves_DrawGlow(void);
void ChampaCliffCaves_LoadColumn(void);

void ChampaCliffCaves_ObabaAsksForBriggs(void);

void ChampaCliffCaves_OpenWall(void);

void ChampaCliffCaves_CloseWall(void);
void ChampaCliffCaves_FightAvimander(void);

extern s16 gChampaCliffCaves_DoorFrames[];
extern s16 gChampaCliffCaves_OtherDoorFrames[];

void FieldEffect_StartBlankOverlay(s32 a);

#define DEFINE_CHAMPA_CLIFF_CAVES_GROW(name, n1)                               \
  void name(struct Actor *actor) {                                             \
    actor->scaleX += n1;                                                       \
    if (actor->scaleX >= FIXED_ONE) {                                          \
      actor->scaleX = FIXED_ONE;                                               \
      actor->update = 0;                                                       \
    }                                                                          \
    actor->scaleY = actor->scaleX;                                             \
  }

DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrown, FIXED_ONE / 16)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gChampaCliffCaves_ObabaPlacements[];
extern struct ActorPlacement gChampaCliffCaves_AvimanderPlacements[];
extern s16 gChampaCliffCaves_AvimanderPlacementsBriggsHome[];
extern s16 gChampaCliffCaves_AvimanderPlacementsAfterAvimander[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene;

  if (GameFlag_Get(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
    scene = gMainRoutineWork.next;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
      return gChampaCliffCaves_BriggsPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
      return gChampaCliffCaves_HousesPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
      return gChampaCliffCaves_ObabaPlacementsAfterAvimander;
    if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
      return gChampaCliffCaves_AvimanderPlacementsAfterAvimander;
  } else {
    if (GameFlag_Get(FLAG_BRIGGS_SAILED_FROM_ALHAFRA)) {
      scene = gMainRoutineWork.next;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
        return gChampaCliffCaves_BriggsPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
        return gChampaCliffCaves_HousesPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
        return gChampaCliffCaves_ObabaPlacementsBriggsHome;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
        return gChampaCliffCaves_AvimanderPlacementsBriggsHome;
    } else {
      scene = gMainRoutineWork.next;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
        return gChampaCliffCaves_BriggsPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
        return gChampaCliffCaves_HousesPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
        return gChampaCliffCaves_ObabaPlacements;
      if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
        return gChampaCliffCaves_AvimanderPlacements;
    }
  }
  return gChampaCliffCaves_BriggsPlacements;
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void ChampaCliffCaves_OpenDoorAndEnter(const struct DoorEdit *edit, s32 door,
                                       s32 mode) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  id = gMainRoutineWork.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Stepping into the cliff's first door: it opens and the leader walks through.
void ChampaCliffCaves_EnterDoor(s32 door) {
  ChampaCliffCaves_OpenDoorAndEnter(&gChampaCliffCaves_FirstDoor, door, 0);
}

/// Knocking at Obaba's door before Briggs is home: she comes out, asks whether
/// her grandson has returned, and goes back in.
void ChampaCliffCaves_ObabaAsksForBriggs(void) {
  const struct DoorEdit *edit;
  s32 msg;

  edit = &gChampaCliffCaves_FirstDoor;
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 168, 424);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_EAST);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Actor_GetById(ACTOR_OBABA)->motionFlags = 0;
  Actor_SetPosition(ACTOR_OBABA, 184, 408);
  Actor_SetSpeed(ACTOR_OBABA, 1.8);
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 16);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  msg = (s32)MsgHasMyGrandson;
  Event_SetMessageId(msg);
  // "Has my grandson returned? Has my Briggs come home?"
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_OBABA, 0);
  Actor_SetSpeed(ACTOR_OBABA, 0.8);
  Actor_WalkByAndWait(ACTOR_OBABA, 0, -14);
  Actor_SetPosition(ACTOR_OBABA, 0, 0);
  edit = &gChampaCliffCaves_SecondDoor;
  MapGrid_PlayMapEdit(edit->frames, edit->x, edit->z);
  Sound_Play(SE_DOOR_OPEN);
}

/// A villager at a door asks whether the party wants to come in, and answers
/// the choice.
void ChampaCliffCaves_AskToComeIn(s32 actor) {
  s32 msg = (s32)MsgWhatYouWant;

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

/// One of Briggs's crew gives in ("I'll go back to Alhafra...but only if Briggs
/// is going") and keeps watching the leader.
void ChampaCliffCaves_CrewmanSurrenders(s32 actorId) {
  struct Actor *actor;

  Event_SetMessageId((s32)MsgAllRightI);
  // "All right, I surrender! I'll go back to Alhafra...but only if Briggs..."
  Event_ShowMessageAndWait(actorId, 0);
  actor = Actor_GetById(actorId);
  Actor_WatchActor(actor, Actor_GetById(gMainRoutineWork.leaderActorId));
}

/// One of Briggs's crew at Obaba's forge tells the party to leave him alone,
/// turns away and squashes and stretches for ever.
void ChampaCliffCaves_CrewmanTurnsAway(s32 actor) {
  Event_SetMessageId((s32)MsgAwComeOn);
  // "Aw, come on! I'm not doing anything! Leave me alone!"
  Event_ShowMessageAndWait(actor, 0);
  Actor_FaceDirectionAndWait(actor, FACING_NORTH_NORTHEAST);
  Actor_SetMovementType(Actor_GetById(actor), MOVEMENT_TYPE_SQUASH);
}

/// Opens the wall into Avimander's room: its pictures and its walkable cells.
void ChampaCliffCaves_OpenWall(void) {
  MapGrid_CopyCellPictureCellsRect(41, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(41, 33, 1, 4, 41, 13);
}

/// Closes the wall again, moving a leader who stands in its gap out of the way.
void ChampaCliffCaves_CloseWall(void) {
  struct Actor *actor;
  s32 x;
  s32 z;

  MapGrid_CopyCellPictureCellsRect(50, 97, 1, 4, 41, 77);
  Map_CopyCellAttributeRect(46, 33, 1, 4, 41, 13);
  actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  if (gMainRoutineWork.next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER && x == 41 &&
      (u32)(z - 13) <= 4)
    Actor_SetPosition(gMainRoutineWork.leaderActorId, 664, 280);
}

extern struct Trigger gChampaCliffCaves_BriggsTriggers[];

/// Picks up the forged Trident from beside Obaba's forge.
void ChampaCliffCaves_PickUpTrident(s32 a) {
  Event_PickUpItem(a, ITEM_TRIDENT, FLAG_TRIDENT_TAKEN);
}

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
    return gChampaCliffCaves_BriggsTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_HOUSES)
    return gChampaCliffCaves_HousesTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_OBABA)
    return gChampaCliffCaves_ObabaTriggers;
  if (scene == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER)
    return gChampaCliffCaves_AvimanderTriggers;
  return gChampaCliffCaves_BriggsTriggers;
}

s32 Scene_Initialize(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

#if !TLA_PATCH_1
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
#else
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
#endif
  if (party->next != SCENE_CHAMPA_CLIFF_CAVES_BRIGGS)
    Actor_GetById(party->leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (party->next == SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER) {
    if (party->entrance == ENTRANCE_AFTER_DEFEAT)
      ChampaCliffCaves_AskIfBriggsLearned();
    if (party->entrance == ENTRANCE_AFTER_BATTLE) {
      party->returnEntrance = 23;
      party->entrance = 23;
      ChampaCliffCaves_BriggsMournsSalamander();
    }
    if (GameFlag_Get(FLAG_TRIDENT_FORGED) &&
        !GameFlag_Get(FLAG_TRIDENT_TAKEN)) {
      FieldActor_ShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
      Actor_SetPosition(ACTOR_HELD_ITEM, 664, 232);
      Actor_GetById(ACTOR_HELD_ITEM)->motionFlags = ACTOR_FLOATS;
    }
    if (GameFlag_Get(FLAG_TRIDENT_FORGED))
      Actor_SetPosition(ACTOR_OBABA, 664, 200);
  } else if (party->next == SCENE_CHAMPA_CLIFF_CAVES_OBABA) {
    if (GameFlag_Get(FLAG_CHAMPA_AVIMANDER_BEATEN)) {
      Actor_SetAnimationById(ACTOR_BRIGGS_IN_BED, 11);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BRIGGS_IN_BED), 0);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The leader walks to Obaba, holds up the given item over the counter and
/// steps back.
void ChampaCliffCaves_HandPieceToObaba(s32 actor) {
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 648, 200);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_EAST);
  Event_Wait(5);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_LEADER_SPRING);
  Event_Wait(7);
  Actor_ShowItemIconById(ACTOR_HELD_ITEM, actor);
  Actor_SetPosition(ACTOR_HELD_ITEM, 648, 196);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(gMainRoutineWork.leaderActorId, -16, -8);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_EAST);
  Event_Wait(10);
}

/// Obaba steps up to take the item from the counter, and unless she is to forge
/// it steps back.
void ChampaCliffCaves_ObabaStepsForward(s32 value) {
  Actor_SetSpeed(ACTOR_OBABA, 0.8);
  Actor_GetById(ACTOR_OBABA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 6);
  Event_Wait(1);
  Actor_GetById(ACTOR_OBABA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_SetAnimationById(ACTOR_OBABA, 8);
  Event_Wait(15);
  Actor_SetPosition(ACTOR_HELD_ITEM, 0, 0);
  Event_Wait(15);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  if (value == 0) {
    Actor_GetById(ACTOR_OBABA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(ACTOR_OBABA, 0, -6);
    Event_Wait(1);
    Actor_GetById(ACTOR_OBABA)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_Wait(10);
  }
}

/// Fades five background colours, from palette entry 114, from one set to
/// another over the given number of frames, one step a frame.
void ChampaCliffCaves_FadeColours(u16 *from, u16 *to, s32 frames) {
  s32 step;

  for (step = 0; step <= frames; step++) {
    s32 offset;
    s32 left;

    for (offset = 0, left = 4; left >= 0; left--) {
      s32 red, green, blue, toRed, toGreen, toBlue;

      red = *(u16 *)(offset + (s32)from) & 31;
      green = *(u16 *)(offset + (s32)from) >> 5 & 31;
      blue = *(u16 *)(offset + (s32)from) >> 10 & 31;
      toRed = *(u16 *)(offset + (s32)to) & 31;
      toGreen = *(u16 *)(offset + (s32)to) >> 5 & 31;
      toBlue = *(u16 *)(offset + (s32)to) >> 10 & 31;
      red += step * (toRed - red) / frames;
      green += step * (toGreen - green) / frames;
      blue += step * (toBlue - blue) / frames;
      // A once-only block: its end keeps the step after the colour's store.
      do {
        s32 color = blue << 10 | green << 5 | red;

        *(u16 *)((u32)&BG_PLTT_COLOR(7, 2) + offset) = color;
      } while (0);
      offset += 2;
    }
    wait(1);
  }
}

/// Obaba is handed the first prong: she knows it for part of the trident of
/// Ankohl and offers to keep it until the party brings the other two.
void ChampaCliffCaves_ObabaSeesAWeapon(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgThisLooksLike);
  ChampaCliffCaves_HandPieceToObaba(actor);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "This looks like a weapon!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "It looks like part of a trident, but it's only one third of the..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "Why... Could it be? Is this the legendary trident of Ankohl?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(8);
  // "I'll hold on to this for you. It won't do you any good like this..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(0);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "You seem unsure... You don't think I'll pull a fast one on you, do you?"
  // Yes: "Kids today. So mistrustful." No: "Ah, that's nice. Don't you worry!"
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "Kids today. So mistrustful."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    gFieldWork->message++;
    // "Ah, that's nice. Don't you worry!"
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
  // "If you bring me the other two pieces, I'll be happy to rebuild it..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_End();
}

/// Obaba is handed the second prong and keeps it with the first.
void ChampaCliffCaves_ObabaAsksForSecondPiece(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAhHaveYou);
  ChampaCliffCaves_HandPieceToObaba(actor);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "Ah, have you found the second piece, dear?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Event_Wait(15);
  // "It's magnificent!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(0);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "I'll hold on to this as well. Wouldn't want it getting lost, now..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_End();
}

#include "text/msg_ids.h"

/// The trident's three prongs, in the order Obaba throws them into the forge.
struct TridentProngs {
  s32 items[3];
};

extern struct TridentProngs gChampaCliffCaves_Prongs;

/// Obaba is handed the last prong and throws the three into her forge, which
/// flares, shakes the room and lifts out the Trident.
void ChampaCliffCaves_ObabaForgesTrident(s32 value) {
  struct Actor *thrown = Actor_GetById(ACTOR_HELD_ITEM);
  u16 bright[5];
  u16 dim[5];
  s32 glow[3];
  s32 i;
  struct Actor *actor;
  u16 *palette;

  glow[0] = toFIXED(662);
  glow[1] = 0;
  glow[2] = toFIXED(238);
  palette = (u16 *)&BG_PLTT_COLOR(7, 2);
  bright[0] = *palette++;
  bright[1] = *palette++;
  bright[2] = *palette++;
  bright[3] = *palette++;
  bright[4] = *palette;
  palette = (u16 *)&BG_PLTT_COLOR(6, 2);
  dim[0] = *palette++;
  dim[1] = *palette++;
  dim[2] = *palette++;
  dim[3] = *palette++;
  dim[4] = *palette;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellDoneYou);
  ChampaCliffCaves_HandPieceToObaba(value);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Well done! You found all three pieces of the trident!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "That means I can begin work reforging the ancient trident of Ankohl!..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  ChampaCliffCaves_ObabaStepsForward(1);
  Actor_WalkToAndWait(ACTOR_OBABA, 664, 200);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "We'll just toss all three pieces in here, shall we?" Yes: "Just leave it
  // to me..." No: "There's nothing to be afraid of, dear. Just..."
  FieldMessage_ShowAtActor(ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "Just leave it to me..."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    gFieldWork->message++;
  } else {
    Event_Wait(40);
    gFieldWork->message++;
    // "There's nothing to be afraid of, dear. Just leave it to me."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  }
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  FieldCamera_MoveTo(664, CAMERA_KEEP, 232, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  // "Heart of the earth, fury of the forge's flames, grant me power!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  for (i = 0; i <= 2; i++) {
    struct TridentProngs prongs;
    FIXED landing[3];
    prongs = gChampaCliffCaves_Prongs;
    Actor_SetAnimationById(ACTOR_OBABA, 9);
    Event_Wait(3);
    Actor_ShowItemIconById(ACTOR_HELD_ITEM, prongs.items[i]);
    Actor_SetPosition(ACTOR_HELD_ITEM, 668, 200);
    thrown->gravity = toFIXED(1) / 4;
    Actor_Jump(ACTOR_HELD_ITEM, 4, 2);
    Actor_SetSpritePriority(ACTOR_HELD_ITEM, 1);
    Actor_SetSpeed(ACTOR_HELD_ITEM, 1);
    Actor_SetDestinationOffsetById(ACTOR_HELD_ITEM, 0, 40);
    Sound_Play(SE_FALL);
    Event_Wait(34);
    landing[0] = thrown->x.fixed;
    landing[1] = thrown->y.fixed;
    landing[2] = thrown->z.fixed;
    Actor_SetPosition(ACTOR_HELD_ITEM, 0, 0);
    Event_Wait(20);
    Sound_Play(SE_HEAVY_IMPACT);
    ChampaCliffCaves_FadeColours(bright, dim, 12);
    ChampaCliffCaves_FadeColours(dim, bright, 48);
    Event_Wait(10);
  }
  actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(gMainRoutineWork.leaderActorId, 12, 12);
  Event_Wait(1);
  Actor_GetById(gMainRoutineWork.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_SOUTHEAST);
  Event_Wait(10);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_OBABA, 7);
  Event_Wait(60);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "That's funny... Nothing happened."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Event_Wait(20);
  Sound_Play(SE_SHIMMER);
  ChampaCliffCaves_FadeColours(bright, dim, 48);
  Actor_StartRepeatedMotion(gMainRoutineWork.leaderActorId, 3);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Call2((void (*)())Field_StartColorTransition, 16, RGB(16, 4, 4));
  MapGrid_SetShake(2, 2, 1);
  ChampaCliffCaves_LightDisc(glow);
  Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
  Actor_Jump(ACTOR_FELIX, 6, 0);
  Sound_Play(SE_TABLET_RISE);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_FELIX, -12, -12);
  Event_Wait(1);
  Actor_GetById(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_OBABA, 3);
  FieldEffect_SetAttached(ACTOR_OBABA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(30);
  // "What's going on?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  MapGrid_SetShake(4, 4, 1);
  ChampaCliffCaves_RaiseColumn(glow);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(10);
  Actor_Jump(ACTOR_OBABA, 6, 15);
  Actor_Jump(ACTOR_OBABA, 6, 23);
  Sound_Play(SE_TABLET_RISE);
  Event_Wait(10);
  // "Something's happening! Is that...the legendary trident?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  FieldActor_ShowItemIconById(ACTOR_HELD_ITEM, ITEM_TRIDENT);
  Actor_SetPosition(ACTOR_HELD_ITEM, 664, 232);
  Actor_SetSpritePriority(ACTOR_HELD_ITEM, 1);
  thrown->motionFlags = ACTOR_FALLS;
  thrown->y.fixed = toFIXED(24);
  thrown->update = ChampaCliffCaves_GrowThrown;
  thrown->groundHeight = 0;
  thrown->scaleX = 0;
  thrown->scaleY = 0;
  Actor_Jump(ACTOR_HELD_ITEM, 5, 0);
  Field_StartPaletteTransition(8);
  ChampaCliffCaves_FlareAndDieDown();
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  while (thrown->y.fixed > 0)
    wait(1);
  thrown->gravity = toFIXED(1);
  thrown->y.fixed = 0;
  thrown->motionFlags = ACTOR_FLOATS;
  Scene_PlayMusic();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(5);
  // "After all these years, the trident is complete again. But what's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  // "Regardless, the trident has been fixed. It belongs to you now..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_HELD_ITEM, 2);
  Event_End();
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

/// Talking to Obaba at her counter: she takes the next prong the party carries,
/// right, left then center, or talks of the trident when it has none.
void ChampaCliffCaves_ObabaOffersHelp(s32 actor) {
  s32 item;
  // DEAD: each owner is stored and never read; the ROM keeps the copies of
  // the first two into sl and r8, which later stand for -1.
  s32 owner;

  if (FIELD_CELL(Actor_GetById(gMainRoutineWork.leaderActorId)->x.fixed) <= 41)
    goto first;
  gFieldWork->fieldMenu = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = ITEM_CENTER_PRONG;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = ITEM_RIGHT_PRONG;
  if ((owner = PartyInventory_FindMember(item)) != -1)
    goto remove;
  item++;
  if ((owner = PartyInventory_FindMember(item)) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!GameFlag_Test_(FLAG_OBABA_SAW_FIRST_PRONG)) {
      GameFlag_Set(FLAG_OBABA_SAW_FIRST_PRONG);
      ChampaCliffCaves_ObabaSeesAWeapon(item);
    } else if (!GameFlag_Test_(FLAG_OBABA_SAW_SECOND_PRONG)) {
      GameFlag_Set(FLAG_OBABA_SAW_SECOND_PRONG);
      ChampaCliffCaves_ObabaAsksForSecondPiece(item);
    } else if (!GameFlag_Test_(FLAG_TRIDENT_FORGED)) {
      GameFlag_Set(FLAG_TRIDENT_FORGED);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
      ChampaCliffCaves_ObabaForgesTrident(item);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, 0);
      FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, 0);
    }
  } else {
    Actor_FaceActorById(actor, gMainRoutineWork.leaderActorId, 5);
    if (!GameFlag_Test_(FLAG_OBABA_SAW_FIRST_PRONG))
      Event_SetMessageId((s32)MsgIMeanIt);
    else if (!GameFlag_Test_(FLAG_OBABA_SAW_SECOND_PRONG))
      Event_SetMessageId((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Test_(FLAG_TRIDENT_FORGED))
      Event_SetMessageId((s32)MsgYesThereS);
    else
      Event_SetMessageId((s32)MsgTheTridentSeems);
    // "The trident seems to have some amazing power... The legends must be..."
    Event_ShowMessageAndWait(actor, 0);
    Actor_FaceDirection(actor, FACING_SOUTH_SOUTHWEST, 0);
  }
}

DEFINE_CHAMPA_CLIFF_CAVES_GROW(ChampaCliffCaves_GrowThrownSlowly,
                               FIXED_ONE * 3 / 64)

#define PALETTE ((u16 *)&BG_PLTT_COLOR(0, 0))

/// Entering Avimander's room: Briggs stops the party and Obaba asks Felix to
/// back down; refused, she throws Briggs's salamander into her forge and it
/// comes out as Avimander to fight.
void ChampaCliffCaves_FightAvimander(void) {
  u16 current[5];
  u16 target[5];
  struct Vector3 position;
  const u16 *colour;
  struct Actor *actor = Actor_GetById(ACTOR_AVIMANDER);

  if (Flag_IsSet(FLAG_CHAMPA_AVIMANDER_BEATEN))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 584, 248);
  if (Flag_IsSet(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY)) {
    Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
    Event_SetMessageId((s32)MsgBackForMore);
    // "Back for more?"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Sound_Play(MUS_CONFRONTATION);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 256, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    // "I wouldn't be doing this if it weren't for Eoleo, you know."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "I'm sorry. You look like quite a nice boy, but I just can't allow..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  } else {
    Flag_Set(FLAG_CHAMPA_BRIGGS_STOPPED_PARTY);
    Event_SetMessageId((s32)MsgHoldItRight);
    // "Hold it right there!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Sound_Play(MUS_CONFRONTATION);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 256, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(5);
    Actor_Jump(ACTOR_BRIGGS, 6, 15);
    Actor_Jump(ACTOR_BRIGGS, 6, 23);
    // "So, Felix, you've come."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Don't assume I'll go so easy on you this time!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
    Event_Wait(10);
    // "Is that boy him? The "mighty warrior" you told me about?"
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    // "I told you he didn't look like much, Grandma..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Weren't you even listening to me?"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
    // "All you told me is that his name is Felix and that he is a "mighty..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    // "He doesn't look like the sort of person who's interested in stealing..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(30);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "But that's not the point, Grandma!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
    Event_Wait(5);
    // "If you don't destroy him now, he's going to take me far away, and..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Oh, now he doesn't look like the kind of boy who would do that..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 45);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST + 10);
    Event_Wait(10);
    // "If he takes me away, poor Eoleo will be so sad! What do you say..."
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST);
    Event_Wait(10);
    // "Oh, that's just not fair. You know I can't deny my darling..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
    Event_Wait(5);
    Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
    Event_Wait(5);
    // "I don't see why I should be the one to get you out of this mess, but..."
    FieldEvent_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
    Event_Wait(5);
    // "Grandma, please!"
    FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(5);
  }
  Event_SetMessageId((s32)MsgOhAllRightListen);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_NORTH_NORTHWEST);
  Event_Wait(20);
  Actor_GetById(ACTOR_OBABA)->facing = FACING_SOUTH_SOUTHWEST;
  Actor_SetAnimationById(ACTOR_OBABA, 5);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_OBABA, 6);
  Actor_SetPosition(ACTOR_SALAMANDER, 724, 252);
  Actor_GetById(ACTOR_SALAMANDER)->scaleX = FIXED_ONE * 6 / 5;
  Actor_GetById(ACTOR_SALAMANDER)->scaleY = FIXED_ONE * 6 / 5;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SALAMANDER), 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Scene_PlayMusic();
    Event_Wait(15);
    Actor_SetPosition(ACTOR_SALAMANDER, 0, 0);
    Actor_SetAnimationById(ACTOR_OBABA, ANIM_STAND);
    Actor_GetById(ACTOR_OBABA)->facing = FACING_WEST;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    gFieldWork->message++;
    Actor_WalkByAndWaitById(ACTOR_FELIX, -16, 0);
    Event_Wait(5);
  } else {
    Event_Wait(25);
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
    Sound_Play(SOUND_FADE_OUT_BGM);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(208), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(224), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(240), 0, -4);
    FieldMapGrid_SetCellHeight(toFIXED(656), toFIXED(256), 0, -4);
    actor->gravity = toFIXED(1) / 4;
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = -toFIXED(999);
    Actor_Jump(ACTOR_SALAMANDER, 10, 0);
    Sound_Play(SE_ENEMY_CRY);
    Actor_SetSpritePriority(ACTOR_SALAMANDER, 1);
    Actor_SetSpeed(ACTOR_SALAMANDER, 3);
    Actor_WalkToById(ACTOR_SALAMANDER, 644, 248);
    Event_Wait(21);
    Actor_SetPosition(ACTOR_SALAMANDER, 0, 0);
    Event_Wait(50);
    MapGrid_SetShake(2, 2, 1);
    Event_Wait(70);
    Sound_Play(SE_BOSS_ROAR);
    Event_Wait(25);
    colour = (const u16 *)&BG_PLTT_COLOR(7, 2);
    current[0] = *colour++;
    current[1] = *colour++;
    current[2] = *colour++;
    current[3] = *colour++;
    current[4] = *colour++;
    colour = (const u16 *)&BG_PLTT_COLOR(13, 2);
    target[0] = *colour++;
    target[1] = *colour++;
    target[2] = *colour++;
    target[3] = *colour++;
    target[4] = *colour++;
    ChampaCliffCaves_FadeColours(current, target, 48);
    Event_Wait(20);
    Sound_Play(SE_TABLET_RISE);
    position.x = toFIXED(662);
    position.y = 0;
    position.z = toFIXED(238);
    ChampaCliffCaves_StartForgeFire(&position);
    Sound_Play(SE_BEAST_CRY);
    Actor_SetPosition(ACTOR_AVIMANDER, 664, 248);
    actor->gravity = toFIXED(1);
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = 0;
    actor->y.fixed = toFIXED(32);
    actor->scaleX = FIXED_ONE / 4;
    actor->update = (s32)ChampaCliffCaves_GrowThrownSlowly;
    actor->scaleY = FIXED_ONE / 4;
    Actor_Jump(ACTOR_AVIMANDER, 11, 0);
    Sound_Play(SE_BEAST_CRY);
    Actor_SetSpeed(ACTOR_AVIMANDER, 3.2);
    Actor_WalkToAndWait(ACTOR_AVIMANDER, 584, 248);
    Event_SetDefeatDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
                               ENTRANCE_AFTER_DEFEAT);
    Event_SetAfterBattleDestination(SCENE_CHAMPA_CLIFF_CAVES_AVIMANDER,
                                    ENTRANCE_AFTER_BATTLE);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(12, 5);
  }
  Event_End();
}

/// After losing to Avimander: Briggs asks Felix whether he has learned his
/// lesson, Obaba scolds, and the party is sent back.
void ChampaCliffCaves_AskIfBriggsLearned(void) {
  struct FieldWork *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgHaveYouLearned);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 568, 248);
  Actor_SetPosition(ACTOR_OBABA, 584, 224);
  Actor_SetPosition(ACTOR_AVIMANDER, 600, 240);
  Actor_SetPosition(ACTOR_BRIGGS, 584, 264);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 41);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Have you learned your lesson, Felix?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Ah, such a bright lad. Now then, don't you ever try to take my..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 20);
  work = Ram_HeapSlots->fieldWork;
  work->transitionFrames = 40;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetDestination((s16)gMainRoutineWork.restartScene,
                       (s16)gMainRoutineWork.restartEntrance);
}

extern u32 gChampaCliffCaves_ChaseToNorthScript[];
extern u32 gChampaCliffCaves_ChaseToEastScript[];
extern u32 gChampaCliffCaves_ChaseToWestScript[];
extern u32 gChampaCliffCaves_ChaseToSouthScript[];

/// Briggs mourns his beaten salamander ("My salamander! You... beat my
/// salamander..."), talks with the party through three answers, each counted
/// in the event work, and the party gathers again; the Avimander stays beaten
/// (FLAG_CHAMPA_AVIMANDER_BEATEN).
void ChampaCliffCaves_BriggsMournsSalamander(void) {
  const u8 *scriptA, *scriptB, *scriptC, *scriptD;
  s32 *owner;
  u8 *party;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgMySalamanderYou);
  owner = (s32 *)&gMainRoutineWork;
  owner = (s32 *)((u8 *)owner + 532);
  Actor_SetPosition(*owner, 568, 256);
  Actor_SetPosition(ACTOR_AVIMANDER, 592, 240);
  Actor_SetAnimationById(ACTOR_AVIMANDER, 3);
  Actor_FaceDirectionById(*owner, FACING_EAST, 0);
  FieldCamera_MoveTo(656, CAMERA_KEEP, 248, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  // "My salamander! You... beat my salamander..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_AVIMANDER, 2);
  Event_Wait(5);
  ChampaCliffCaves_StartFlash(13, 1);
  Actor_SetSpeed(ACTOR_OBABA, 1);
  Actor_WalkByAndWait(ACTOR_OBABA, -12, -18);
  Actor_WalkByAndWait(ACTOR_OBABA, -10, -14);
  Actor_WalkByAndWait(ACTOR_OBABA, -18, -12);
  Actor_WalkToAndWait(ACTOR_OBABA, 664, 200);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  // "I thought he was unbeatable... How could this be?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 0);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 8);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 16);
  Actor_WalkByAndWait(ACTOR_OBABA, -8, 16);
  Actor_WalkToAndWait(ACTOR_OBABA, 608, 248);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST);
  Event_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "How did you do that? What... What are you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.7);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, 0);
  Event_Wait(5);
  // "Grandma, what's going on!? I thought you were going to protect me!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(5);
  // "What do you mean, what am I doing? I've done everything I can!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_Jump(ACTOR_BRIGGS, 6, 15);
  Actor_Jump(ACTOR_BRIGGS, 6, 23);
  // "Grandma!!! He's going to take me away! He's a bad guy! You have to..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "I'm sorry, Briggs, but you'll have to fight him yourself."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(38);
  // "Wha-What!?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "You heard me. I'm through pampering you. You have to get out of this..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(37);
  // "But...Grandma... Look at him! I mean, look what he can do!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST - 10);
  Event_Wait(25);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(10);
  // "I know, dear, but there's nothing I can do to help. You don't want..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "But...what about poor Eoleo?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "*Sigh* All right, all right."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 0);
  // "Um. Excuse me... There seems to be a small misunderstanding."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_WEST - 10);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 0, -16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -16, -16, FACING_EAST);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, -16, FACING_EAST);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Event_Wait(10);
  FieldCamera_MoveTo(640, CAMERA_KEEP, 248, 1);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "Yeah! I don't like standing here listening to a pirate call us the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_OBABA, 3);
  Actor_SetAttachedEffect(ACTOR_OBABA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "Pirate! My Briggs?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "He stole a ship from Alhafra and escaped from their jail! A ship we..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_OBABA, FACING_EAST);
  Event_Wait(5);
  // "Briggs, is this true?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  FieldEffect_SetAttached(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(35);
  // "Aw... But, Grandma, they..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "We should tell you, we're not here to take Briggs back to Alhafra."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(5);
  // "So you've decided you don't want to run errands for the mayor of..." Yes:
  // "Good. That greedy bag of hot air can do his..." No: "Felix, I know you
  // can't mean that. We didn't..."
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    // "Good. That greedy bag of hot air can do his own dirty work, for all..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_SOUTHEAST);
    Event_Wait(5);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
    // "Felix, I know you can't mean that. We didn't come to Champa for that..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0, 5);
  }
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
  // "Briggs... Piracy? Why?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetSpeed(ACTOR_OBABA, 2.8);
  Actor_SetSpeed(ACTOR_BRIGGS, 3.4);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  scriptA = gChampaCliffCaves_ChaseToNorthScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptA);
  Event_Wait(20);
  Actor_WalkToById(ACTOR_BRIGGS, 664, 290);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  scriptB = gChampaCliffCaves_ChaseToEastScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptB);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  scriptC = gChampaCliffCaves_ChaseToWestScript;
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptC);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  scriptD = gChampaCliffCaves_ChaseToSouthScript;
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptD);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptA);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_FaceDirection(ACTOR_OBABA, FACING_NORTH_NORTHWEST, 0);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  // "Grandma, it's not what you think! These guys, they..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Briggs, you just hush up!!! ...It all makes sense now. The gems, the..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "What do you mean? What makes sense?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "They're telling the truth! And you, Briggs! You've been lying to me..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptC);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptB);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_SetMovementTypeById(ACTOR_OBABA, scriptA);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_SetMovementTypeById(ACTOR_BRIGGS, scriptD);
  Actor_WaitForScriptEndById(ACTOR_OBABA);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "You're going to take their word over your own grandson's?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "You got that right! I should have known better than to believe you..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  // "He only did it for Champa!"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_SetSpritePriority(ACTOR_FELIX, 3);
  Actor_SetSpritePriority(ACTOR_JENNA, 3);
  Actor_SetSpritePriority(ACTOR_SHEBA, 3);
  Actor_SetSpritePriority(ACTOR_PIERS, 3);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  Event_Wait(10);
  FieldCamera_MoveTo(568, CAMERA_KEEP, 248, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  Actor_GetById(ACTOR_CHAUCHA)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_GetById(ACTOR_CHAUCHA)->motionFlags = 0;
  Actor_SetPosition(ACTOR_CHAUCHA, 520, 200);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
  Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
  // "Chaucha!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_CHAUCHA);
  Actor_WatchAndSetScript(ACTOR_OBABA, ACTOR_CHAUCHA);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 16);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 3);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 0, 32);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 8, 8);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 64, 0);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 256, 1);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -32, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, -16);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "You came!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "I heard Felix had shown up looking for you. I was worried..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_OBABA, 1);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 0);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 8);
  Actor_WalkByAndWait(ACTOR_OBABA, -16, 16);
  Actor_WalkByAndWait(ACTOR_OBABA, -8, 8);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  // "What did you mean, Briggs did it for Champa?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(15);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "All of that food... Stolen goods?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "No, we paid for everthing we brought back with us."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "But you stole a boat to do it, didn't you?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  // "Grandma, we found those jewels ourselves, and we paid Alhafra well..."
  // Yes: "Your story has the ring of truth to it." No: "So he's lying again?"
  FieldMessage_ShowAtActor(ACTOR_BRIGGS, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_OBABA, 2);
    Event_Wait(5);
    // "Your story has the ring of truth to it."
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(10);
    Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Thank you, Felix. Your honesty speaks well of you."
    Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    gFieldWork->message += 2;
  } else {
    Event_Wait(35);
    gFieldWork->message += 2;
    // "So he's lying again?"
    Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
    Actor_FaceDirectionAndRefreshById(ACTOR_BRIGGS, FACING_WEST);
    Event_Wait(5);
    Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Dang it, Felix, I'm not lying!"
    Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
    Actor_Jump(ACTOR_BRIGGS, 6, 15);
    Actor_Jump(ACTOR_BRIGGS, 6, 23);
  }
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
  Event_Wait(15);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "We...were pirates, for a short while. Until we found the jewels."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(5);
  // "But if not for our efforts, Champa would have starved away long ago."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "But that doesn't excuse resorting to piracy!"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "Hey, it's not like we were doing it for the money! We wanted to save..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(5);
  // "Those jewels we found are very valuable. We intend to repay each..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(5);
  // "Isn't that true?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What's the matter with you!? You said on the boat that we would..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  FieldEffect_SetAttached(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "Oh yeah... You're right... We really ought to try to make amends for..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_NORTH_NORTHEAST);
  Event_Wait(10);
  // "That's the truth, Grandmother. Can you find it in your heart to..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_OBABA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  // "All right, all right. I'm too old for grudges. But what about Felix?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "What do you say, Felix? Can you forgive my husband?"
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0);
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(15);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    // "I agree, Felix. If each town is repaid what was stolen, I see no..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_Wait(10);
    gFieldWork->message++;
    // "I don't know, Felix. We didn't come to capture Briggs. Why should we..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 5);
  }
  Actor_FaceDirection(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(10);
  // "Do you hear that? They forgive you, too. That means you can leave..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  Event_Wait(10);
  // "I... I don't know what to say..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You know, you look a little strange..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  // "Oh... Oh, really?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 15);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "You haven't caught a cold, have you? Do you have a fever?"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  FieldActor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_WEST);
  Event_Wait(5);
  // "What are you talking about?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
  // "No, I'm positive... You look flush, too... You'd better lie down..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_FaceDirectionAndWait(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST);
  Event_Wait(10);
  // "She...worries about me."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  // "Hurry up and get into bed so you can get some rest!"
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -8, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -8, 0);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, -56, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST);
  FieldActor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAUCHA, FACING_EAST);
  Event_Wait(10);
  // "Oh, for goodness' sake! You don't want to give them your cold, do..."
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 5);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(5);
  Actor_Jump(ACTOR_BRIGGS, 6, 20);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_BRIGGS);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_BRIGGS);
  Actor_SetSpeed(ACTOR_BRIGGS, 2);
  Actor_WalkByAndWaitById(ACTOR_BRIGGS, -80, 0);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 3);
  Actor_SetSpritePriority(ACTOR_BRIGGS, 3);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, -64);
  Actor_WalkByAndWait(ACTOR_BRIGGS, -16, 0);
  Actor_WalkByById(ACTOR_BRIGGS, 0, -64);
  Event_Wait(15);
  Actor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  Actor_SetSpritePriority(ACTOR_BRIGGS, 2);
  Actor_WaitForMoveById(ACTOR_CHAUCHA);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Actor_WaitForMoveById(ACTOR_BRIGGS);
  Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Event_Wait(60);
  Actor_WalkByAndWaitById(ACTOR_OBABA, 0, 16);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_WEST);
  // "So...what brings you to Champa?"
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_NOD);
  Event_Wait(10);
  // "I mean, you didn't come here to catch Briggs, from what I gathered..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_OBABA, ANIM_SHAKE_HEAD);
  // "Ah, well... To be young and foolish again. If you ever need to..."
  Event_ShowMessageThenWait(ACTOR_OBABA, 0, 5);
  Actor_SetSpeed(ACTOR_BRIGGS, 0.7);
  Actor_SetMovementTypeById(ACTOR_OBABA, gChampaCliffCaves_ChaseToNorthScript);
  Event_Wait(30);
  Actor_Stop(ACTOR_OBABA);
  Actor_WalkToAndWait(ACTOR_OBABA, 648, 188);
  Actor_FaceDirectionAndWait(ACTOR_OBABA, FACING_SOUTH_SOUTHWEST);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_FELIX, 2);
  Actor_GetById(ACTOR_FELIX)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  party = (u8 *)&gMainRoutineWork;
  owner = (s32 *)(party + 532);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(*owner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_CHAMPA_AVIMANDER_BEATEN);
  Event_End();
}

/// ChampaCliffCaves_StartFlash's explosion: its tiles, decompressed into the
/// heap, and the sprite that shows them.
struct FlashWork {
  u8 tiles[1024];
  struct EffectSprite sprite;
};

/// The explosion that hides an actor's change: the actor spins (mode 0) or not,
/// is hidden, and the explosion's sprite plays over it, twice the size in mode
/// 1.
void ChampaCliffCaves_StartFlash(s32 id, s32 mode) {
  struct FlashWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct FlashWork));
  struct Actor *actor = Actor_GetById(id);
  s32 slot;
  s32 tile;
  struct EffectSprite *sprite;
  FIXED position[3];
  s32 i;
  FIXED z;

  if (mode == 0) {
    for (i = 15; i >= 0; i--) {
      s32 angle = actor->facing;
      angle += DEGtoANG(45);
      actor->facing = angle;
      wait(1);
    }
  }
  wait(2);
  actor->scaleX = 0;
  Actor_SetSpriteFlags(actor, 0);
  Dma_Set(Field_ExplosionPalette, (void *)&OBJ_PLTT_COLOR(15, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION),
                      work);
  slot = VramCache_FindFreeEntry();
  tile = Vram_Load(slot, 1024, work);
  sprite = &work->sprite;
  FieldEffectSprite_Init(sprite, 8, 16, OBJ_SIZE_16X16, tile);
  sprite->depth = 240;
  sprite->oam.priority = 1;
  sprite->oam.fullColor = 0;
  sprite->oam.palette = 15;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = z = position[2];
  if (mode == 1) {
    sprite->scaleX = FIXED_ONE * 2;
    sprite->scaleY = FIXED_ONE * 2;
    sprite->y = z + toFIXED(10);
  }
  Sound_Play(SE_CRASH);
  for (i = 0; i <= 71; i++) {
    sprite = &work->sprite;
    switch (i) {
    case 6:
      sprite->oam.index = tile + 4;
      break;
    case 12:
      sprite->oam.index = tile + 8;
      break;
    case 22:
      sprite->oam.index = tile + 12;
      break;
    case 32:
      sprite->oam.index = tile + 16;
      break;
    case 42:
      sprite->oam.index = tile + 20;
      break;
    case 52:
      sprite->oam.index = tile + 24;
      break;
    case 62:
      sprite->oam.index = tile + 28;
      break;
    }
    EffectSprite_Draw(sprite);
    wait(1);
  }
  Actor_SetPosition(id, 0, 0);
  VramCache_FreeEntry(slot);
  HeapSlot_Release(HEAP_SLOT_MENU);
}
#undef PALETTE

/// Loads the forge's glowing disc into the glow's work, its picture moved onto
/// the palette the glow cycles, and sets its sprite at no size.
void ChampaCliffCaves_LoadDisc(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuWork;
  struct EffectSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GLOW_DISC), work);
  for (i = 0, tile = work->tiles; i < (s32)sizeof(work->tiles); tile++, i++) {
    if ((*tile & 0xf0) == 0xf0)
      *tile = (*tile & 0xf) + 0x60;
  }
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 64 * 64, work);
  work->discEntry = entry;
  work->discVram = vram;
  sprite = &work->disc;
  FieldEffectSprite_Init((struct EffectSprite *)sprite, 31, 31, OBJ_SIZE_64X64,
                         vram);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = 2;
  sprite->depth = 1;
  work->discScale.value = 0;
  work->discTrailingScale.value = 0;
}

/// Loads the forge's column of fire into the glow's work as
/// ChampaCliffCaves_LoadDisc loads the disc.
void ChampaCliffCaves_LoadColumn(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuWork;
  struct EffectSprite *sprite;
  s32 i;
  u8 *tile;
  s32 entry;
  s32 vram;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIRE_COLUMN), work);
  for (i = 0, tile = work->tiles; i < (s32)sizeof(work->tiles); i++, tile++) {
    if ((*tile & 0xf0) == 0xf0)
      *tile = (*tile & 0xf) + 0x60;
  }
  entry = VramCache_FindFreeEntry();
  vram = VramCache_Load(entry, 32 * 64, work);
  work->columnEntry = entry;
  work->columnVram = vram;
  sprite = &work->column;
  FieldEffectSprite_Init((struct EffectSprite *)sprite, 15, 47, OBJ_SIZE_32X64,
                         vram);
  sprite->oam.fullColor = 1;
  sprite->oam.palette = 1;
  sprite->oam.priority = 1;
  sprite->depth = 1;
  work->columnScale.value = 0;
  work->columnTrailingScale.value = 0;
}

/// Cycles thirty-two colours of the object palettes through a wave of greens
/// and blues, places the glow's two sprites at its spot with their scales, and
/// lets each sprite's second scale trail its first at 31/40.
void ChampaCliffCaves_DrawGlow(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuWork;
  s32 position[3];
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i <= 31; i++) {
    s32 red = 31;
    s16 *frame = &work->frame;
    s32 wave = Trig_Sin((*frame + i) << 11) * 6;
    s32 blue = Trig_Sin(((*frame / 2 + i) << 11) + TRIG_TURN * 9 / 32) * 3;
    s32 colour;
    wave >>= 16;
    blue >>= 16;
    wave += 20;
    blue += 3;
    if (i <= 15) {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)&OBJ_PLTT_COLOR(0, 0))[111 - i] = colour;
    } else {
      colour = blue << 10 | wave << 5 | red;
      ((u16 *)&OBJ_PLTT_COLOR(0, 0))[255 - i] = colour;
    }
  }
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->disc;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = work->discScale.value;
  sprite->scaleY = work->discTrailingScale.value;
  EffectSprite_Draw(sprite);
  position[0] = work->position[0];
  position[1] = work->position[1];
  position[2] = work->position[2];
  sprite = &work->column;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = work->columnScale.value;
  sprite->scaleY = work->columnTrailingScale.value;
  EffectSprite_Draw(sprite);
  work->discTrailingScale.value = work->discScale.value * 31 / 40;
  work->columnTrailingScale.value = work->columnScale.value * 31 / 40;
  work->frame++;
}

/// The forge's fire at a spot: the disc grows, holds, and the column rises
/// through it, while the room shakes.
void ChampaCliffCaves_StartForgeFire(const s32 *position) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesWork));
  s32 time;
  s32 state;

  work->position[0] = position[0];
  work->position[1] = position[1];
  work->position[2] = position[2];
  ChampaCliffCaves_LoadDisc();
  ChampaCliffCaves_LoadColumn();
  work->frame = 0;
  time = 0;
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_Add(ChampaCliffCaves_DrawGlow, priority);
  }
  MapGrid_SetShake(4, 4, 1);
  state = 0;
  do {
    switch (state) {
    case 0:
      if ((work->discScale.value += FIXED_ONE * 3 / 128) >=
          FIXED_ONE * 13 / 10) {
        work->discScale.value = FIXED_ONE * 13 / 10;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if (time == 10) {
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if ((work->columnScale.value += FIXED_ONE / 64) >= FIXED_ONE * 2 / 5) {
        work->columnScale.value = FIXED_ONE * 2 / 5;
        state = 3;
        time = -1;
      }
      break;
    case 3:
      if ((work->columnScale.value += FIXED_ONE * 3 / 16) >=
          FIXED_ONE * 13 / 10) {
        work->columnScale.value = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
      break;
    }
    System_Wait(1);
    time++;
  } while (state != GLOW_DONE);
}

/// Unused: puts out the forge's fire: the column and then the disc shrink
/// away and the glow's work is released.
UNUSED void ChampaCliffCaves_PutOutForgeFire(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuWork;
  FIXED position[3];
  s32 state;
  s32 time;

  position[0] = work->position[0];
  position[1] = work->position[1] - toFIXED(32);
  state = 0;
  time = 0;
  position[2] = work->position[2];
  do {
    switch (state) {
    case 0:
      if ((work->columnScale.value -= FIXED_ONE / 16) < 0) {
        work->columnScale.value = 0;
        time = -1;
        state = 1;
      }
      break;
    case 1:
      if ((work->discScale.value -= FIXED_ONE / 32) < 0) {
        work->discScale.value = 0;
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if (time == 1)
        state = GLOW_DONE;
      break;
    }
    wait(1);
    time++;
  } while (state != GLOW_DONE);
  Task_Remove(ChampaCliffCaves_DrawGlow);
  MapGrid_SetShake(0, 0, 0);
  VramCache_FreeEntry(work->discEntry);
  VramCache_FreeEntry(work->columnEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// The forge's first glow at a spot: the disc alone grows to full size.
void ChampaCliffCaves_LightDisc(const struct Vector3 *origin) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  work->position[0] = origin->x;
  work->position[1] = origin->y;
  work->position[2] = origin->z;
  ChampaCliffCaves_LoadDisc();
  work->columnScale.value = 0;
  work->columnTrailingScale.value = 0;
  work->frame = 0;
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_Add(ChampaCliffCaves_DrawGlow, priority);
  }
  Sound_Play(SE_DJINNI_SET);
  state = 0;
  do {
    if (state == 0) {
      work->discScale.value += FIXED_ONE / 16;
      if (work->discScale.value >= FIXED_ONE * 13 / 10) {
        work->discScale.value = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
    }
    wait(1);
  } while (state != GLOW_DONE);
}

/// The column of fire rises through the disc already burning.
void ChampaCliffCaves_RaiseColumn(void) {
  struct ChampaCliffCavesWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct ChampaCliffCavesWork));
  s32 state;

  ChampaCliffCaves_LoadColumn();
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_Add(ChampaCliffCaves_DrawGlow, priority);
  }
  Sound_Play(SE_CLATTER);
  state = 0;
  do {
    switch (state) {
    case 0:
      work->columnScale.value += FIXED_ONE / 16;
      if (work->columnScale.value >= FIXED_ONE * 2 / 5) {
        work->columnScale.value = FIXED_ONE * 2 / 5;
        state = 1;
      }
      break;
    case 1:
      work->columnScale.value += FIXED_ONE * 3 / 16;
      if (work->columnScale.value >= FIXED_ONE * 13 / 10) {
        work->columnScale.value = FIXED_ONE * 13 / 10;
        state = GLOW_DONE;
      }
      break;
    }
    wait(1);
  } while (state != GLOW_DONE);
}

/// The forge's last flare: the column flares to twice its size, then it and the
/// disc die away and the glow's work is released.
void ChampaCliffCaves_FlareAndDieDown(void) {
  struct ChampaCliffCavesWork *work = Ram_HeapSlots->menuWork;
  s32 state = 0;
  s32 time = 0;

  do {
    switch (state) {
    case 0:
      if ((work->columnScale.value += FIXED_ONE / 16) >= FIXED_ONE * 2) {
        work->columnScale.value = FIXED_ONE * 2;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if ((work->columnScale.value -= FIXED_ONE / 16) < FIXED_ONE * 13 / 10) {
        work->columnScale.value = FIXED_ONE * 13 / 10;
        state = 2;
        time = -1;
      }
      /* fall through */
    case 2:
      if ((work->columnScale.value -= FIXED_ONE / 16) < 0)
        work->columnScale.value = 0;
      if ((work->discScale.value -= FIXED_ONE / 16) < 0) {
        work->discScale.value = 0;
        state++;
        time = -1;
      }
      break;
    case 3:
      if (time == 1)
        state = GLOW_DONE;
      break;
    }
    wait(1);
    time++;
  } while (state != GLOW_DONE);
  Task_Remove(ChampaCliffCaves_DrawGlow);
  VramCache_FreeEntry(work->discEntry);
  VramCache_FreeEntry(work->columnEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

struct TridentProngs gChampaCliffCaves_Prongs = {
    {ITEM_RIGHT_PRONG, ITEM_LEFT_PRONG, ITEM_CENTER_PRONG}};

#undef DEFINE_CHAMPA_CLIFF_CAVES_GROW
#undef PALETTE

#include "text/msg_ids.h"
#include "field/effect_sprite.h"

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(ChampaCliffCavesBriggs),
    SCENE_EXIT_UNLESS(1, Champa, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, ChampaCliffCavesHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, ChampaCliffCavesBriggs, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, ChampaCliffCavesBriggs, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, ChampaCliffCavesBriggs, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, ChampaCliffCavesBriggs, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesHouses),
    SCENE_EXIT_UNLESS(7, Champa, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, ChampaCliffCavesHouses, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, ChampaCliffCavesHouses, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, ChampaCliffCavesHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, ChampaCliffCavesHouses, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, ChampaCliffCavesBriggs, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, ChampaCliffCavesObaba, 14, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesObaba),
    SCENE_EXIT_UNLESS(14, ChampaCliffCavesHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, ChampaCliffCavesObaba, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, ChampaCliffCavesObaba, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, ChampaCliffCavesObaba, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(18, ChampaCliffCavesObaba, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(19, Champa, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(20, ChampaCliffCavesAvimander, 21, EXIT_FLAG_NONE),
    SCENE_EXITS(ChampaCliffCavesAvimander),
    SCENE_EXIT_UNLESS(21, ChampaCliffCavesObaba, 20, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(22, ChampaCliffCavesAvimander, 23, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(23, ChampaCliffCavesAvimander, 22, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_BriggsPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0, 224,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 192, 0, 184, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_NORTH_NORTHEAST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_ObabaPlacements[] = {
    ACTOR_PLACEMENT(36, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 200, 0, 424, FACING_SOUTH_SOUTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_AvimanderPlacements[] = {
    ACTOR_PLACEMENT(36, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 616, 0, 216, FACING_SOUTH_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_BriggsPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, 8, 344, 0, 568,
                        FACING_WEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 504,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_HousesPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
            0, 728, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328,
            0, 728, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248,
                        FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gChampaCliffCaves_ObabaPlacementsBriggsHome[] =
    {
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520,
            0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
        ACTOR_PLACEMENT(
            SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184,
            0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
        {SCENE_TABLE_END},
};

LATE_DATA s16 gChampaCliffCaves_AvimanderPlacementsBriggsHome[] = {
    36,
    -1,
    1,
    0,
    0,
    736,
    0,
    0,
    0,
    248,
    FACING_WEST,
    2,
    115,
    -1,
    8,
    0,
    0,
    216,
    0,
    0,
    0,
    464,
    FACING_NORTH_NORTHEAST,
    1,
    489,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    16,
    -1,
    1,
    0,
    0,
    752,
    0,
    0,
    0,
    264,
    FACING_WEST,
    2,
    338,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    2,
    158,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    2,
    37,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_WEST,
    1,
    56,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    2,
    5,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    6,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    7,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_NORTH,
    2,
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_BriggsPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 568,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 216, 0,
                        224, FACING_SOUTH, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 208,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 600, 0, 216,
                        FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_WANDER, 576, 0, 248, FACING_SOUTH,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 504, 0, 264, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_HousesPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 192, 0, 184,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216,
                        FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 568, 0, 248, FACING_WEST,
                        TALK_FACING_PARTY, 0),
        ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS,
                        MOVEMENT_TYPE_STAND, 488, 0, 248, FACING_EAST,
                        TALK_FACING_PARTY, 0),
        {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement
    gChampaCliffCaves_ObabaPlacementsAfterAvimander[] = {
        ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        560, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
        ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                        FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
        ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        480, 0, 704, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH,
                        0),
        ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                        504, 0, 712, FACING_WEST, TALK_FACING_PARTY_AND_BACK,
                        0),
        {SCENE_TABLE_END},
};

LATE_DATA s16 gChampaCliffCaves_AvimanderPlacementsAfterAvimander[] = {
    36,
    -1,
    1,
    0,
    0,
    648,
    0,
    0,
    0,
    188,
    FACING_SOUTH_SOUTHWEST,
    2,
    115,
    -1,
    1,
    0,
    0,
    216,
    0,
    0,
    0,
    464,
    FACING_SOUTH_SOUTHWEST,
    3,
    489,
    -1,
    1,
    0,
    0,
    -56,
    0,
    0,
    0,
    -56,
    FACING_SOUTH,
    2,
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

LATE_DATA s16 gChampaCliffCaves_DoorFrames[] = {
    1, 64, 1, 2, 6, 2, 64, 1, 2, 6, -1,
};

LATE_DATA s16 gChampaCliffCaves_OtherDoorFrames[] = {
    1, 64, 1, 2, 6, 0, 64, 1, 2, 6, -1,
};

LATE_DATA struct DoorEdit gChampaCliffCaves_FirstDoor = {
    gChampaCliffCaves_DoorFrames, 11, 88};

LATE_DATA struct DoorEdit gChampaCliffCaves_SecondDoor = {
    gChampaCliffCaves_OtherDoorFrames, 11, 88};

LATE_DATA struct Trigger gChampaCliffCaves_BriggsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgArenTYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgIThinkThose)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReGoing)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgThereWerePlenty)},
    {TRIGGER_TALK, 12, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeReallyWant)},
    {TRIGGER_TALK, 13, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgItSAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThatSureComes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAllThoseSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgSupposeWeWent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsIsJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWasAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgAtLeastWe)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhyDoYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_CrewmanSurrenders)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_AskToComeIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgMaybeIfI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSAfter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgToughOrNot)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAllOfOurMen)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTheChampaSailors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgBriggsIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIfYouLive)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_HousesTriggers[] = {
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWhyDidYou)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgWeHaveSo)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBriggsCameDown)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgEoleoStillWon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIJustKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSaysHe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgBriggsSNever)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgEoleoIsYounger)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIDRatherFight)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgYouReAren)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgWhySEveryone)},
    {TRIGGER_TALK, 11, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgIfYouSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgItWouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsSaidThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgHeSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatSHe)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBeforeHeLeft)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMyHusbandWants)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSomedayIWant)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgMomSHad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgBriggsMayBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgWhenIMarried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgHasMomForgotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgEoleoIsThe)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_ObabaTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     EVENT(ChampaCliffCaves_EnterDoor)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS,
     EVENT(ChampaCliffCaves_ObabaAsksForBriggs)},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgOnceIM)},
    {TALK_TRIGGER_FACING(FACING_NORTH), 11,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIWonderWhatS)},
    {TRIGGER_TALK, 10, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgBabuBaBubabu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgIMCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgThereIsAbsolutely)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgOneOfPapa)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgLeaveTheWomen)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgDidYouReally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhatAreThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgWhyWouldBriggs)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgObabaSNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIVeNeverSeen)},
    {TRIGGER_EXAMINE, 40, FLAG_CHAMPA_AVIMANDER_BEATEN,
     EVENT(ChampaCliffCaves_ObabaAsksForBriggs)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gChampaCliffCaves_AvimanderTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_FightAvimander)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS,
     EVENT(ChampaCliffCaves_ObabaOffersHelp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_OBABA_SAW_FIRST_PRONG,
     MSGNO(MsgNowIVe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_OBABA_SAW_SECOND_PRONG,
     MSGNO(MsgAllOfMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_TRIDENT_FORGED,
     MSGNO(MsgSoonILl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAllOfMyTraining)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(ChampaCliffCaves_PickUpTrident)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN),
     MSGNO(MsgYouGuysDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_CHAMPA_AVIMANDER_BEATEN), MSGNO(MsgMustBeIncredibly)},
    {TRIGGER_TALK, 9, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     EVENT(ChampaCliffCaves_CrewmanTurnsAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA), MSGNO(MsgBriggsIsPretty)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgThatPitIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgIWonderHow)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(ChampaCliffCaves_OpenWall)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(ChampaCliffCaves_CloseWall)},
    {TRIGGER_TABLE_END},
};

/// Obaba chasing Briggs round her forge: the four quarters of the way round,
/// each walked to its end.
LATE_DATA u32 gChampaCliffCaves_ChaseToNorthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(616, 0, 220),
    SC_SYNC,
    SC_MOVE(632, 0, 206),
    SC_SYNC,
    SC_MOVE(648, 0, 200),
    SC_SYNC,
    SC_MOVE(664, 0, 198),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToEastScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(680, 0, 200),
    SC_SYNC,
    SC_MOVE(694, 0, 206),
    SC_SYNC,
    SC_MOVE(710, 0, 216),
    SC_SYNC,
    SC_MOVE(720, 0, 232),
    SC_SYNC,
    SC_MOVE(720, 0, 246),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToSouthScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(710, 0, 264),
    SC_SYNC,
    SC_MOVE(698, 0, 278),
    SC_SYNC,
    SC_MOVE(664, 0, 290),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gChampaCliffCaves_ChaseToWestScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(632, 0, 280),
    SC_SYNC,
    SC_MOVE(616, 0, 266),
    SC_SYNC,
    SC_MOVE(608, 0, 248),
    SC_SYNC,
    SC_MOVE(608, 0, 230),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(1),
    SC_END,
};
// clang-format on
