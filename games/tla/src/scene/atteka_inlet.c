/// @file
/// Atteka Inlet, where the party's ship lies on the southern continent, in two
/// scenes that share this overlay: the harbour with the ship
/// (SceneId_AttekaInlet) and the shore beside it (SceneId_AttekaInletGeode).
/// Piers asks whether to set sail (AttekaInlet_AskPiersToSetSail). After
/// Jupiter Lighthouse the shipwrights stop the party to see their handiwork fly
/// (AttekaInlet_ShipwrightsStopTheParty), Hamma shows the ship's new wings
/// (AttekaInlet_HammaShowsTheWings), and in a game linked with The Broken Seal
/// a messenger brings Master Hammet's gift
/// (AttekaInlet_HammetsMessengerArrives). Cyclone clears the shore's bushes and
/// uncovers Geode, a Venus Djinni (AttekaInlet_CycloneUncoversGeode).

#include "flags.h"
#include "djinn_ids.h"
#include "system/io_reg.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_event.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "scene/event_layer.h"
#include "text/msg_ids.h"

/// The inlet's own story, which no other scene reads.
#define FLAG_SHIPWRIGHTS_STOPPED_PARTY                                         \
  0x9b0 ///< "We worked really hard on that ship"
#define FLAG_HAMMA_SHOWED_THE_WINGS 0x9b1 ///< Hamma showed the ship's wings
#define FLAG_HAMMETS_GIFT_DELIVERED 0x9ad ///< Master Hammet's messenger came

extern struct ActorPlacement gAttekaInlet_ActorPlacements[];
extern struct ActorPlacement gAttekaInlet_HarbourActorPlacements[];
extern struct ActorPlacement gAttekaInlet_HarbourActorPlacementsOnceWinged[];
extern struct ActorPlacement gAttekaInlet_HarbourActorPlacementsOnceFlown[];
extern struct ActorPlacement gAttekaInlet_ShoreActorPlacementsOnceWinged[];
extern struct ActorPlacement gAttekaInlet_ShoreActorPlacements[];
extern struct Trigger gAttekaInlet_Triggers[];
extern struct Trigger gAttekaInlet_ShoreTriggers[];
void AttekaInlet_HammetsMessengerArrives(void);
void AttekaInlet_SetUpTheHarbour(void);
void AttekaInlet_SetUpTheShore(void);

/// The harbour's people, numbering gAttekaInlet_HarbourActorPlacements and
/// the tables after it.
enum AttekaInletHarbourActorId {
  /// "I've been watching the skies, hoping I could catch another glimpse of
  /// the flying ship."
  ACTOR_YOUNG_MAN_WATCHING_THE_SKIES = ACTOR_FIRST_PLACED + 5,
  /// "I wonder what's on the other side of the ocean..."
  ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
  /// "We brought these wings from Contigo, and now we're guarding them."
  ACTOR_WOMAN_GUARDING_THE_WINGS,
  /// "I wonder if these giant wings will really make it fly..."
  ACTOR_BOY_ON_THE_WINGS,
  /// The cannon on the dock.
  ACTOR_CANNON,
  /// Kraden, who asks whether to set sail.
  ACTOR_KRADEN,
  /// Once the ship has wings: Contigo's scholars come to see it fly, and
  /// Hamma.
  ///@{
  ACTOR_SCHOLAR_A,
  ACTOR_SCHOLAR_B,
  ACTOR_SCHOLAR_C,
  ACTOR_SCHOLAR_D,
  ///@}
  ACTOR_HAMA,
  /// Master Hammet's messenger, in a game linked with The Broken Seal.
  ACTOR_HAMMETS_MESSENGER
};

/// The shore beside the harbour (gAttekaInlet_ShoreActorPlacements).
enum AttekaInletShoreActorId {
  /// "All of our myths and legends have taught us that the sea is a
  /// dangerous, fearsome power."
  ACTOR_WOMAN_FEARING_THE_SEA = ACTOR_FIRST_PLACED,
  /// The shipwrights who stop the party at the pier: "Where do you think
  /// you're going?"
  ///@{
  ACTOR_SHIPWRIGHT,
  ACTOR_SECOND_SHIPWRIGHT,
  ///@}
  /// A scholar from Contigo on the shore.
  ACTOR_SHORE_SCHOLAR,
  /// The round boulder Lift cannot lift.
  ACTOR_ROUND_BOULDER = ACTOR_FIRST_PLACED + 5,
  /// Geode, the Venus Djinni Cyclone uncovers.
  ACTOR_GEODE
};

/// The chest Master Hammet's messenger leaves, holding his gift: hidden until
/// he comes.
#define ACTOR_HAMMETS_GIFT_CHEST 65

/// Geode, the Venus Djinni uncovered on the shore, joins the party.
void AttekaInlet_GeodeJoins(void) {
  Djinn_JoinParty(ACTOR_GEODE, DJINNI_GEODE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_GEODE));
  GameFlag_Clear(FLAG_AREA_TEMP_00);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ATTEKA_INLET) {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      return gAttekaInlet_HarbourActorPlacementsOnceFlown;
    if (GameFlag_Get(FLAG_REUNITED_AT_CONTIGO))
      return gAttekaInlet_HarbourActorPlacementsOnceWinged;
    return gAttekaInlet_HarbourActorPlacements;
  }
  if (scene == SCENE_ATTEKA_INLET_GEODE) {
    if (GameFlag_Get(FLAG_REUNITED_AT_CONTIGO))
      return gAttekaInlet_ShoreActorPlacementsOnceWinged;
    return gAttekaInlet_ShoreActorPlacements;
  }
  return gAttekaInlet_ActorPlacements;
}

extern struct Trigger gAttekaInlet_HarbourTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ATTEKA_INLET)
    return gAttekaInlet_HarbourTriggers;
  if (scene == SCENE_ATTEKA_INLET_GEODE)
    return gAttekaInlet_ShoreTriggers;
  return gAttekaInlet_Triggers;
}

/// Piers asks whether to set out: a yes boards the ship, with its wings once
/// the parties are reunited (FLAG_SHIP_FLIES, FLAG_SET_OUT_WITH_WINGS); a no is
/// counted and he walks back to the leader.
void AttekaInlet_AskPiersToSetSail(void) {
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAreWeSetting);
  // "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
  // don't rush on my account. Let's enjoy..."
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
    // "Then, to sea we go!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(FLAG_REUNITED_AT_CONTIGO)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(4);
  } else {
    (gFieldWork->message)++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetAnimationById(ACTOR_KRADEN, 2);
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (actor != NULL)
      Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    GameFlag_Set(FLAG_SCENE_TEMP_01);
    Event_End();
  }
}

/// After the reunion, until the ship sets out with its wings, two shipwrights
/// jump in to stop the party at the pier.
void AttekaInlet_ShipwrightsStopTheParty(void) {
  if (GameFlag_Get(FLAG_REUNITED_AT_CONTIGO) &&
      !GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(96, CAMERA_KEEP, 308, 1);
    {
      s32 angle = 208;

      Actor_SetPositionAndFacing(ACTOR_SHIPWRIGHT, toFIXED(60), toFIXED(362),
                                 angle << 8);
    }
    wait(1);
    Actor_SetSpeed(ACTOR_SHIPWRIGHT, 2);
    Actor_Jump(ACTOR_SHIPWRIGHT, 6, 0);
    Actor_MoveToAndWait(ACTOR_SHIPWRIGHT, 64, 336);
    Actor_WalkToAndWait(ACTOR_SHIPWRIGHT, 70, 308);
    Actor_FaceDirectionById(ACTOR_SHIPWRIGHT, FACING_EAST, 0);
    {
      s32 angle = 160;

      Actor_SetPositionAndFacing(ACTOR_SECOND_SHIPWRIGHT, toFIXED(94),
                                 toFIXED(272), angle << 7);
    }
    wait(1);
    Actor_SetSpeed(ACTOR_SECOND_SHIPWRIGHT, 2);
    Actor_Jump(ACTOR_SECOND_SHIPWRIGHT, 6, 0);
    Actor_MoveToAndWait(ACTOR_SECOND_SHIPWRIGHT, 90, 288);
    Actor_WalkToAndWaitById(ACTOR_SECOND_SHIPWRIGHT, 80, 316);
    Actor_FaceDirectionAndRefreshById(ACTOR_SECOND_SHIPWRIGHT, FACING_EAST);
    Actor_GetById(ACTOR_SHIPWRIGHT)->velocityY = toFIXED(4);
    Event_Wait(10);
    Event_SetMessageId((s32)MsgWhereDoYouThink);
    // "Where do you think you're going?"
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
    Actor_ShowEmoteById(ACTOR_SECOND_SHIPWRIGHT,
                        EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    // "We worked really hard on that ship of yours!"
    Event_ShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
    Actor_FaceDirection(ACTOR_SHIPWRIGHT, FACING_SOUTH_SOUTHEAST, 0);
    // "I want to see our handiwork take flight. Don't you, buddy?"
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
    Actor_FaceDirectionAndWait(ACTOR_SECOND_SHIPWRIGHT, FACING_NORTH_NORTHWEST);
    Actor_SetAnimationById(ACTOR_SECOND_SHIPWRIGHT, 3);
    // "I want to! Boy, do I want to!"
    Event_ShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
    Actor_FaceDirectionById(ACTOR_SECOND_SHIPWRIGHT, FACING_EAST, 0);
    Actor_ShowEmoteById(ACTOR_SHIPWRIGHT, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionById(ACTOR_SHIPWRIGHT, FACING_EAST, 20);
    // "So hurry up! We want to see the boat fly!"
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHIPWRIGHT, 0);
    // "Thanks, buddy!"
    Event_ShowMessageAndWait(ACTOR_SECOND_SHIPWRIGHT, 0);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    GameFlag_Set(FLAG_SHIPWRIGHTS_STOPPED_PARTY);
    Event_End();
  }
}

/// After the reunion, Hamma greets the party at the ship and shows the wings.
void AttekaInlet_HammaShowsTheWings(void) {
  if (GameFlag_Get(FLAG_REUNITED_AT_CONTIGO)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(520, CAMERA_KEEP, 372, 1);
    FieldCamera_WaitForArrival();
    Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    // "I've been waiting for you, Felix."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_NORTH_NORTHWEST);
    // "Take a look at your ship."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMA, 0);
    FieldCamera_GetActor()->motionFlags = 0;
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(440, 32, 368, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    FieldCamera_MoveTo(432, 32, 232, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(40);
    FieldCamera_SetSpeed(4.8);
    FieldCamera_MoveTo(520, 32, 372, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
    // "Now that you have been given wings, all obstacles in your path will..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "However, it is not the wings alone that grant your ship the power of..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAnimationById(ACTOR_HAMA, 4);
    // "Psynergy is the force that powers the Wings of Anemos."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    // "If the ship is to fly, you will need to focus the power of your..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_NORTH_NORTHWEST);
    // "You have visited Shaman Village recently, have you not?"
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST);
    // "Now is the time to make use of the powers you acquired there."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
    // "Now... Use the power of Hover to raise your ship!"
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAnimationById(ACTOR_HAMA, 4);
    // "At first, you may not want to move your ship too high or too far..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_ShowEmoteById(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "It will be difficult at first, but that is to be expected... After..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_ShowEmoteById(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    // "But your quest should not demand much more of you..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
    // "Now, be confident and set sail!"
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    Actor_SetSpeed(ACTOR_HAMA, 0.8);
    Actor_GetById(ACTOR_HAMA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWaitById(ACTOR_HAMA, 504, 360);
    Event_Wait(1);
    Actor_GetById(ACTOR_HAMA)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirectionAndRefreshById(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
    // "All of Contigo is watching... Show them your power, and make your..."
    Event_ShowMessageAndWait(ACTOR_HAMA, 0);
    GameFlag_Set(FLAG_HAMMA_SHOWED_THE_WINGS);
    Event_End();
  }
}

void Actor_MoveToObject(s32 id, s32 target);

/// Kraden comes to the party leader and asks whether to set out, as in
/// AttekaInlet_AskPiersToSetSail; after a no the leader steps back.
void AttekaInlet_PiersAsksToSetSail(void) {
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 532, 248);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  Event_SetMessageId(MsgAreWeSetting);
  // "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
  // don't rush on my account. Let's enjoy..."
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_KRADEN, 3);
    // "Then, to sea we go!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    gMainRoutineWork.ownerOverride = 1;
    if (Event_GetFlag(FLAG_REUNITED_AT_CONTIGO)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(4);
  } else {
    (gFieldWork->message)++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetAnimationById(ACTOR_KRADEN, 2);
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (actor != NULL)
      Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 528, 276);
    Event_End();
  }
}

/// With Dodonpa locked up in The Broken Seal's data and the wings shown, Master
/// Hammet's messenger comes, once.
void AttekaInlet_CheckForHammetsMessenger(void) {
  if (GameFlag_Get(FLAG_TRANSFERRED_DODONPA_LOCKED_UP) &&
      GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS) &&
      !GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    AttekaInlet_HammetsMessengerArrives();
}

/// Cyclone on the shore's bush by Geode: the bush is cleared and, unless the
/// party has him, Geode hops out, the leader stepping aside if he stood there.
void AttekaInlet_CycloneUncoversGeode(void) {
  struct Actor *actor;
  s32 moved;

  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Get(FLAG_DJINNI(DJINNI_GEODE))) {
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    Event_Start();
    Event_ResolvePendingActions(0);
    moved = 0;
    if (FIELD_CELL(actor->x.fixed) == 7 && FIELD_CELL(actor->z.fixed) == 23) {
      Actor_Jump(gMainRoutineWork.leaderActorId, 6, 0);
      Actor_SetDestination(gMainRoutineWork.leaderActorId, 104, 376);
      moved = 1;
    }
    Actor_SetPosition(ACTOR_GEODE, 120, 376);
    wait(1);
    Actor_Jump(ACTOR_GEODE, 4, 10);
    GameFlag_Set(FLAG_AREA_TEMP_00);
    if (moved)
      Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                        FACING_EAST);
    Event_End();
  }
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneFirstBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 4, 10, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneSecondBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 6, 9, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneThirdBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 6, 6, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneFourthBush(void) {
  FieldTileCopies_CopyTouchedCells(12, 5, 1, 1, 12, 6, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneFifthBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 13, 7, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneSixthBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 14, 6, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneSeventhBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 21, 6, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneEighthBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 22, 6, 0);
}

/// Cyclone on one of the shore's nine bushes clears its cell.
void AttekaInlet_CycloneNinthBush(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 23, 7, 0);
}

/// Master Hammet's messenger runs up with the gift he was to bring Isaac and
/// leaves it in a chest; Hamma tells how Hammet sent it in thanks
/// (FLAG_HAMMETS_GIFT_DELIVERED).
void AttekaInlet_HammetsMessengerArrives(void) {
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  angle = FACING_NORTH_NORTHEAST;
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      ACTOR_HAMMETS_MESSENGER, toFIXED(504), toFIXED(460), angle);
  wait(1);
  Event_SetMessageId((s32)MsgHeyWait2);
  // "Hey, wait!"
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, CAMERA_KEEP, 380, 1);
  Actor_SetSpeed(ACTOR_HAMMETS_MESSENGER, 1.6);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 432);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 388);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 0);
  // "Master Hammet requested that I deliver this to Isaac, and I totally..."
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_ShowEmoteById(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  // "Well, you'd better hurry."
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_ShowEmote(ACTOR_HAMMETS_MESSENGER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                  0);
  // "I'm supposed to deliver this to Isaac... It's not too late, is it?"
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMMETS_MESSENGER, angle);
  Actor_GetById(ACTOR_HAMMETS_MESSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 384);
  Event_Wait(1);
  Actor_GetById(ACTOR_HAMMETS_MESSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_RunRepeatedMotion(ACTOR_HAMMETS_MESSENGER, 2);
  Actor_SetPosition(ACTOR_HAMMETS_GIFT_CHEST, 520, 380);
  wait(1);
  Actor_GetById(ACTOR_HAMMETS_MESSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 388);
  Event_Wait(1);
  Actor_GetById(ACTOR_HAMMETS_MESSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_HAMMETS_MESSENGER, FACING_NORTH_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_HAMMETS_MESSENGER, 3);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 434);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 348, 1);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 504, 460);
  Actor_SetPosition(ACTOR_HAMMETS_MESSENGER, 0, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMA, angle);
  // "On my way here, I stopped in Kalay briefly to speak with Master Hammet."
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_ShowEmoteById(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "He was quite upset that you hadn't returned to visit him so that he..."
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAnimationById(ACTOR_HAMA, 3);
  // "So he requested that I take this with me to Atteka."
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMA, 3);
  Actor_ShowEmoteById(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "It is a gift of thanks from Master Hammet. Take it, please."
  Event_ShowMessageAndWait(ACTOR_HAMA, 0);
  GameFlag_Set(FLAG_HAMMETS_GIFT_DELIVERED);
  Event_End();
}

/// Each frame in the harbour: the boy by the wings and actors 8 and 10 take the
/// leader's sprite priority.
void AttekaInlet_ShareLeadersPriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.leaderActorId)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(ACTOR_BOY_ON_THE_WINGS, variant);
  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}

/// Each frame on the shore: the round boulder takes the leader's sprite
/// priority.
void AttekaInlet_ShareLeadersPriorityWithRock(void) {
  Actor_SetSpritePriority(
      ACTOR_ROUND_BOULDER,
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.leaderActorId)
           ->sprite)
          ->priority);
}

/// Rotates palette colours 5-9 and 13-15 by one entry every eight frames.
void AttekaInlet_RotateWaterColors(void) {
  if ((*Ram_FrameCount & 7) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(11, 9);
    for (index = 9; index != 4; index--)
      BG_PLTT_COLOR(11, index) = BG_PLTT_COLOR(11, index - 1);
    BG_PLTT_COLOR(11, 4) = saved;
  }
  if ((*Ram_FrameCount & 7) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(11, 15);
    for (index = 15; index != 12; index--)
      BG_PLTT_COLOR(11, index) = BG_PLTT_COLOR(11, index - 1);
    BG_PLTT_COLOR(11, 12) = saved;
  }
}

/// Stepping into a door: it opens, the leader walks through and the scene
/// leaves by the exit given.
void AttekaInlet_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -4);
  Event_Wait(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ATTEKA_INLET)
    AttekaInlet_SetUpTheHarbour();
  else if (scene == SCENE_ATTEKA_INLET_GEODE)
    AttekaInlet_SetUpTheShore();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ATTEKA_INLET && GameFlag_Get(FLAG_REUNITED_AT_CONTIGO)) {
    MapGrid_CopyCellPictureIndicesRect(54, 47, 4, 15, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 38, 73, 7, 10, 4);
    MapGrid_CopyCellPictureIndicesRect(54, 43, 21, 55, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(65, 43, 21, 20, 10, 3);
    MapGrid_CopyCellPictureIndicesRect(4, 47, 4, 50, 10, 3);
  }
  return 0;
}

/// The harbour on entry: the ship is here, the water's colours turn, Hammet's
/// chest hides until his gift comes, and the scholars and Hamma stand by the
/// wings until the ship has flown.
void AttekaInlet_SetUpTheHarbour(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;
  struct MainRoutineWork *party;

  GameFlag_Set(FLAG_SHIP_AT_ATTEKA_INLET);
  if (!GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    Actor_SetPosition(ACTOR_HAMMETS_GIFT_CHEST, 0, 0);
  map->wading = MAP_WADING_UPPER_LEVEL;
  party = &gMainRoutineWork;
  Actor_GetById(party->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldTask_AddFrame(AttekaInlet_RotateWaterColors, TASK_PASS_VBLANK);
  FieldTask_AddFrame(AttekaInlet_ShareLeadersPriority, TASK_PASS_VBLANK);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_GetById(ACTOR_CANNON)->motionFlags = 0;
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetPosition(ACTOR_SCHOLAR_A, 0, 0);
    Actor_SetPosition(ACTOR_SCHOLAR_B, 0, 0);
    Actor_SetPosition(ACTOR_SCHOLAR_C, 0, 0);
    Actor_SetPosition(ACTOR_SCHOLAR_D, 0, 0);
    Actor_SetPosition(ACTOR_HAMA, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS)) {
      s32 facing = 192;

      Actor_SetPositionAndFacing(ACTOR_HAMA, toFIXED(504), toFIXED(360),
                                 facing << 6);
    }
    Actor_WatchAndSetScript(ACTOR_SCHOLAR_A, party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_SCHOLAR_B, party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_SCHOLAR_C, party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_SCHOLAR_D, party->leaderActorId);
  }
}

/// The shore on entry: the shipwrights and the scholar leave once the ship has
/// flown, or block the pier once they have stopped the party; Geode stands out
/// once uncovered.
void AttekaInlet_SetUpTheShore(void) {
  if (gMainRoutineWork.entrance == 3)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetPosition(ACTOR_SHIPWRIGHT, 0, 0);
    Actor_SetPosition(ACTOR_SECOND_SHIPWRIGHT, 0, 0);
    Actor_SetPosition(ACTOR_SHORE_SCHOLAR, 0, 0);
  } else if (GameFlag_Get(FLAG_SHIPWRIGHTS_STOPPED_PARTY)) {
    Actor_SetPositionAndFacing(ACTOR_SHIPWRIGHT, toFIXED(70), toFIXED(308),
                               FACING_EAST);
    Actor_SetPositionAndFacing(ACTOR_SECOND_SHIPWRIGHT, toFIXED(80),
                               toFIXED(316), FACING_EAST);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    wait(1);
  }
  if (GameFlag_Get(FLAG_AREA_TEMP_00))
    Actor_SetPosition(ACTOR_GEODE, 120, 376);
  Task_Add(AttekaInlet_ShareLeadersPriorityWithRock, TASK_PASS_VBLANK);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 72, 0, 295, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AttekaInlet),
    SCENE_EXIT(ATTEKA_INLET_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins,
               LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK),
    SCENE_EXIT(ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH, AttekaInletGeode,
               ATTEKA_INLET_GEODE_FROM_ATTEKA_INLET_SOUTH),
    SCENE_EXIT(ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH, AttekaInletGeode,
               ATTEKA_INLET_GEODE_FROM_ATTEKA_INLET_NORTH),
    SCENE_EXIT(ATTEKA_INLET_TO_WORLD_MAP_WEST, WorldMap,
               WORLD_MAP_FROM_ATTEKA_INLET_EAST),
    SCENE_EXITS(AttekaInletGeode),
    SCENE_EXIT(ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH, AttekaInlet,
               ATTEKA_INLET_FROM_ATTEKA_INLET_GEODE_SOUTH),
    SCENE_EXIT(ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_ATTEKA_INLET_GEODE_NORTH),
    SCENE_EXIT(ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS, ContigoShops,
               CONTIGO_SHOPS_FROM_ATTEKA_INLET_GEODE),
    SCENE_EXIT(4, AttekaInletGeode, 4),
    SCENE_EXIT(ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH, AttekaInlet,
               ATTEKA_INLET_FROM_ATTEKA_INLET_GEODE_NORTH),
    SCENE_EXIT(ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_ATTEKA_INLET_GEODE_NORTH),
    SCENE_EXITS_END,
};

/// The scene's other rooms' actors.
struct ActorPlacement gAttekaInlet_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

/// The harbour's actors, in AttekaInletHarbourActorId's order.
struct ActorPlacement gAttekaInlet_HarbourActorPlacements[] = {
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
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 152, 0, 296, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 80, 0, 304, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 208, 0, 280, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 184, 0, 224, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The harbour's actors once the party has met again at Contigo and the ship
/// has its wings.
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
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 216, 0, 168, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 55, 0, 234, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 260, 0, 158, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 504, 0, 424, FACING_EAST,
                    TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 536, 0, 424, FACING_WEST,
                    TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 504, 0, 408, FACING_EAST,
                    TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 536, 0, 408, FACING_EAST,
                    TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 516, 0,
                    360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH,
                    0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The harbour's actors once the ship has set out with its wings.
struct ActorPlacement gAttekaInlet_HarbourActorPlacementsOnceFlown[] = {
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
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 162, 0, 300, FACING_SOUTHEAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 111, 0, 327, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS, 3, 204, 0,
                    204, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The shore's actors once the ship has its wings.
struct ActorPlacement gAttekaInlet_ShoreActorPlacementsOnceWinged[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 264, 0, 352, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 380, 0, 280, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    104, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GEODE),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The shore's actors, in AttekaInletShoreActorId's order.
struct ActorPlacement gAttekaInlet_ShoreActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 0, 0, 0, FACING_WEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    104, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GEODE),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The scene's other rooms' triggers.
struct Trigger gAttekaInlet_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The harbour's exits, door, talks and the messenger's check.
struct Trigger gAttekaInlet_HarbourTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(AttekaInlet_OpenDoorAndEnter)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 10, FLAG_HAMMA_SHOWED_THE_WINGS,
     EVENT(AttekaInlet_HammaShowsTheWings)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(AttekaInlet_AskPiersToSetSail)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(AttekaInlet_PiersAsksToSetSail)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CheckForHammetsMessenger)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_REUNITED_AT_CONTIGO,
     MSGNO(MsgWeVeMade)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgHammaReturnedTo)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, CONDITION_ALWAYS,
     MSGNO(MsgIVeBeenWatching)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     FLAG_REUNITED_AT_CONTIGO, MSGNO(MsgWeFirstHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWeVeBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     CONDITION_ALWAYS, MSGNO(MsgWheneverILook)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
     FLAG_REUNITED_AT_CONTIGO, MSGNO(MsgWeMustBe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWasYourBoat)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, FLAG_REUNITED_AT_CONTIGO,
     MSGNO(MsgTheseWingsAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgWhoBuiltThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, CONDITION_ALWAYS,
     MSGNO(MsgIWishI)},
    {TRIGGER_TALK, ACTOR_WOMAN_GUARDING_THE_WINGS, CONDITION_ALWAYS,
     MSGNO(MsgWeBroughtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_GUARDING_THE_WINGS,
     CONDITION_ALWAYS, MSGNO(MsgIDoubtAnyone)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_REUNITED_AT_CONTIGO,
     MSGNO(MsgIWonderIfThese)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgAMassiveRock)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, CONDITION_ALWAYS,
     MSGNO(MsgYouReGoingNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     FLAG_REUNITED_AT_CONTIGO, MSGNO(MsgItSKind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgIfThatBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     CONDITION_ALWAYS, MSGNO(MsgIVeDecided)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_A, CONDITION_ALWAYS, MSGNO(MsgItSAlmost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_A, CONDITION_ALWAYS,
     MSGNO(MsgTheDayHas)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_B, CONDITION_ALWAYS, MSGNO(MsgIfEveryoneWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_B, CONDITION_ALWAYS,
     MSGNO(MsgTheAnemosWere)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_C, CONDITION_ALWAYS, MSGNO(MsgWhenTheWinged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_C, CONDITION_ALWAYS,
     MSGNO(MsgTheseWarriorsWill)},
    {TRIGGER_TALK, ACTOR_SCHOLAR_D, CONDITION_ALWAYS, MSGNO(MsgOnceYouRemove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_D, CONDITION_ALWAYS,
     MSGNO(MsgIHearThatThe)},
    {TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgUseYourPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS,
     MSGNO(MsgTravelSafelyThe)},
    {TRIGGER_TABLE_END},
};

/// The shore's exits, talks, Geode and Cyclone on its bushes.
struct Trigger gAttekaInlet_ShoreTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 10, FLAG_SHIPWRIGHTS_STOPPED_PARTY,
     EVENT(AttekaInlet_ShipwrightsStopTheParty)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_GEODE, CONDITION_ALWAYS,
     EVENT(AttekaInlet_GeodeJoins)},
    {TRIGGER_TALK, ACTOR_WOMAN_FEARING_THE_SEA, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgEveryoneSWaiting)},
    {TRIGGER_TALK, ACTOR_WOMAN_FEARING_THE_SEA, CONDITION_ALWAYS,
     MSGNO(MsgAllOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEARING_THE_SEA,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgHammaIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_FEARING_THE_SEA,
     CONDITION_ALWAYS, MSGNO(MsgEveryoneInContigo)},
    {TRIGGER_TALK, ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgTheVillagersAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgItSAmazing)},
    {TRIGGER_TALK, ACTOR_SECOND_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgHammaWouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SHIPWRIGHT,
     CONDITION_ALWAYS, MSGNO(MsgHammaSaysThat)},
    {TRIGGER_TALK, ACTOR_SHORE_SCHOLAR, CONDITION_ALWAYS,
     MSGNO(MsgWhenPeopleWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHORE_SCHOLAR,
     CONDITION_ALWAYS, MSGNO(MsgWeCanT)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROUND_BOULDER, CONDITION_ALWAYS, 0},
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
