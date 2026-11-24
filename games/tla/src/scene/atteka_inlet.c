/// @file
/// Atteka Inlet, where Felix ties up the Lemurian ship on the way to Contigo,
/// in two scenes that share this overlay: the harbor, a sandy beach where the
/// ship lies and the Wings of Anemos wait on their trestles
/// (SceneId_AttekaInlet), and the grassy heights above it, with a house, the
/// path north towards Contigo and the way west out to the world map
/// (SceneId_AttekaInletGeode). On the deck, walking towards the ship's wheel
/// brings Kraden up to ask whether to set sail
/// (AttekaInlet_KradenAsksToSetSail). Cyclone on the bush beside the round
/// boulder on the heights uncovers Geode, a Venus Djinni
/// (AttekaInlet_CycloneUncoversGeode), and on eight other clumps of weeds only
/// scatters them. Once Isaac's party has joined Felix at Contigo, two
/// shipwrights block the way west, wanting to see their handiwork fly
/// (AttekaInlet_ShipwrightsStopTheParty); Hamma waits at the foot of the
/// gangplank to show the ship its new wings (AttekaInlet_HammaShowsTheWings);
/// and in a game linked with The Broken Seal, a messenger runs up the
/// gangplank with Master Hammet's gift for Isaac
/// (AttekaInlet_HammetsMessengerArrives).

#include "flags.h"
#include "djinn_ids.h"
#include "system/io_reg.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "system/main_routine_state.h"
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
#define FLAG_GEODE_UNCOVERED FLAG_AREA_TEMP_00 ///< Cyclone uncovered Geode
/// The party told Kraden not to set out yet: "Please don't rush on my account."
#define FLAG_TOLD_KRADEN_NOT_YET FLAG_SCENE_TEMP_01

/// The map cells the inlet's step and Cyclone triggers answer.
enum AttekaInletCellBehavior {
  /// The harbor: the grass at the foot of the gangplank, where Hamma waits;
  /// the deck before the ship's wheel; the deck at the head of the gangplank.
  CELL_BEHAVIOR_GANGPLANK_FOOT = 10,
  CELL_BEHAVIOR_BEFORE_THE_WHEEL = 11,
  CELL_BEHAVIOR_GANGPLANK_HEAD = 12,
  /// The heights: the way west out to the world map, where the shipwrights
  /// stop the party.
  CELL_BEHAVIOR_WAY_WEST = 10,
  /// The bush beside the round boulder, where Geode hides, and the nine clumps
  /// of weeds, in the order of AttekaInlet_CycloneFirstWeeds and the rest.
  CELL_BEHAVIOR_GEODES_BUSH = 20,
  CELL_BEHAVIOR_FIRST_WEEDS,
  CELL_BEHAVIOR_SECOND_WEEDS,
  CELL_BEHAVIOR_THIRD_WEEDS,
  CELL_BEHAVIOR_FOURTH_WEEDS,
  CELL_BEHAVIOR_FIFTH_WEEDS,
  CELL_BEHAVIOR_SIXTH_WEEDS,
  CELL_BEHAVIOR_SEVENTH_WEEDS,
  CELL_BEHAVIOR_EIGHTH_WEEDS,
  CELL_BEHAVIOR_NINTH_WEEDS
};

extern struct ActorPlacement gAttekaInlet_ActorPlacements[];
extern struct ActorPlacement gAttekaInlet_HarborActorPlacements[];
extern struct ActorPlacement gAttekaInlet_HarborActorPlacementsOnceWinged[];
extern struct ActorPlacement gAttekaInlet_HarborActorPlacementsOnceSetOut[];
extern struct ActorPlacement gAttekaInlet_HeightsActorPlacementsOnceWinged[];
extern struct ActorPlacement gAttekaInlet_HeightsActorPlacements[];
extern struct Trigger gAttekaInlet_Triggers[];
extern struct Trigger gAttekaInlet_HeightsTriggers[];
void AttekaInlet_HammetsMessengerArrives(void);
void AttekaInlet_SetUpTheHarbor(void);
void AttekaInlet_SetUpTheHeights(void);

/// The harbor's people, numbering gAttekaInlet_HarborActorPlacements and
/// the tables after it.
enum AttekaInletHarborActorId {
  /// Purple posts with glowing tops, along the shore and up the slope.
  ///@{
  ACTOR_GLOWING_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_GLOWING_POST_B,
  ACTOR_GLOWING_POST_C,
  ACTOR_GLOWING_POST_D,
  ACTOR_GLOWING_POST_E,
  ///@}
  /// "I've been watching the skies, hoping I could catch another glimpse of
  /// the flying ship."
  ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
  /// "I wonder what's on the other side of the ocean..."
  ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
  /// "We brought these wings from Contigo, and now we're guarding them."
  ACTOR_WOMAN_GUARDING_THE_WINGS,
  /// "I wonder if these giant wings will really make it fly..."
  ACTOR_BOY_ON_THE_WINGS,
  /// The cannon on the ship's deck, once the ship carries it.
  ACTOR_CANNON,
  /// Kraden, who asks whether to set sail.
  ACTOR_KRADEN,
  /// Once the ship has wings, four robed men come to see it fly, and Hamma.
  /// "It's almost time... Time to watch a legend become reality!"
  ACTOR_MAN_AWAITING_THE_LEGEND,
  /// "If everyone will just obey Hamma's request, the ship will surely fly."
  ACTOR_MAN_ON_HAMMAS_REQUEST,
  /// "When the winged ship of Anemos flies once again, the whole land shall be
  /// made free..."
  ACTOR_MAN_ON_THE_WINGED_SHIP,
  /// "Once you remove the stone anchors weighing the ship down, it should
  /// fly..."
  ACTOR_MAN_ON_THE_STONE_ANCHORS,
  ACTOR_HAMMA,
  /// Master Hammet's messenger, in a game linked with The Broken Seal.
  ACTOR_HAMMETS_MESSENGER
};

/// The heights above the harbor (gAttekaInlet_HeightsActorPlacements).
enum AttekaInletHeightsActorId {
  /// "All of our myths and legends have taught us that the sea is a
  /// dangerous, fearsome power. But on the day the ship left, the sea was so
  /// calm and romantic looking..."
  ACTOR_WOMAN_ON_THE_CALM_SEA = ACTOR_FIRST_PLACED,
  /// The shipwrights who stop the party on its way west out of the inlet:
  /// "Where do you think you're going?"
  ///@{
  ACTOR_SHIPWRIGHT,
  ACTOR_SECOND_SHIPWRIGHT,
  ///@}
  /// "We sent everyone back to the village so that their hopes would not impair
  /// your flight."
  ACTOR_MAN_WHO_SENT_EVERYONE_BACK,
  /// The round boulder in front of Geode's bush, which Lift raises.
  ACTOR_ROUND_BOULDER = ACTOR_FIRST_PLACED + 5,
  /// Geode, the Venus Djinni Cyclone uncovers.
  ACTOR_GEODE
};

/// The chest Master Hammet's messenger leaves, holding his gift: hidden until
/// he comes.
#define ACTOR_HAMMETS_GIFT_CHEST (FIELD_TREASURE_ACTOR + 1)

/// Talking to Geode, facing east towards him where Cyclone uncovered him by the
/// round boulder, makes him hop to the leader and join the party, the screen
/// washing green as he does.
void AttekaInlet_GeodeJoins(void) {
  Djinn_JoinParty(ACTOR_GEODE, DJINNI_GEODE / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_GEODE));
  GameFlag_Clear(FLAG_GEODE_UNCOVERED);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_ATTEKA_INLET) {
    if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS))
      return gAttekaInlet_HarborActorPlacementsOnceSetOut;
    if (GameFlag_Get(FLAG_REUNION_AT_CONTIGO))
      return gAttekaInlet_HarborActorPlacementsOnceWinged;
    return gAttekaInlet_HarborActorPlacements;
  }
  if (scene == SCENE_ATTEKA_INLET_GEODE) {
    if (GameFlag_Get(FLAG_REUNION_AT_CONTIGO))
      return gAttekaInlet_HeightsActorPlacementsOnceWinged;
    return gAttekaInlet_HeightsActorPlacements;
  }
  return gAttekaInlet_ActorPlacements;
}

extern struct Trigger gAttekaInlet_HarborTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_ATTEKA_INLET)
    return gAttekaInlet_HarborTriggers;
  if (scene == SCENE_ATTEKA_INLET_GEODE)
    return gAttekaInlet_HeightsTriggers;
  return gAttekaInlet_Triggers;
}

/// Talking to Kraden on the deck has him ask "Felix, are we setting off now?":
/// yes sails the ship out of the inlet onto the world map, no has him say
/// "Please don't rush on my account" and walk back into the leader.
/// Once the parties have met again at Contigo, setting out takes the wings
/// along (FLAG_SHIP_FLIES, FLAG_SET_OUT_WITH_WINGS).
void AttekaInlet_AskKradenToSetSail(void) {
  struct Actor *actor;

  // The player never finds him to talk to: Kraden is placed on the harbor's
  // stage only by AttekaInlet_KradenAsksToSetSail, and answered No there he
  // walks back into Felix and is gone from the deck.
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId(MsgAreWeSetting);
  // "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
  // don't rush on my account. Let's enjoy..."
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "Then, to sea we go!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    gMainRoutineState.ownerOverride = 1;
    if (Event_GetFlag(FLAG_REUNION_AT_CONTIGO)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(ATTEKA_INLET_TO_WORLD_MAP_WEST);
  } else {
    (gFieldState->message)++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    if (actor != NULL)
      Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    GameFlag_Set(FLAG_TOLD_KRADEN_NOT_YET);
    Event_End();
  }
}

/// Walking west across the heights towards the way out to the world map (cells
/// (5, 18) to (6, 20)), once Isaac's party has joined Felix and before the ship
/// has flown, two shipwrights jump up from below: "Where do you think you're
/// going?" They want to see their handiwork take flight, and stay standing in
/// the way, so the party can only leave by the ship. Walls copied onto the
/// cells beside them (`Map_CopyCellAttributeRect`) close the path for good.
void AttekaInlet_ShipwrightsStopTheParty(void) {
  if (GameFlag_Get(FLAG_REUNION_AT_CONTIGO) &&
      !GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldCamera_MoveTo(96, CAMERA_KEEP, 308, 1);
    Actor_SetPositionAndFacing(ACTOR_SHIPWRIGHT, toFIXED(60), toFIXED(362),
                               FACING_NORTH_NORTHEAST);
    wait(1);
    Actor_SetSpeed(ACTOR_SHIPWRIGHT, 2);
    Actor_Jump(ACTOR_SHIPWRIGHT, 6, 0);
    Actor_MoveToAndWait(ACTOR_SHIPWRIGHT, 64, 336);
    Actor_WalkToAndWait(ACTOR_SHIPWRIGHT, 70, 308);
    Actor_FaceDirectionById(ACTOR_SHIPWRIGHT, FACING_EAST, 0);
    Actor_SetPositionAndFacing(ACTOR_SECOND_SHIPWRIGHT, toFIXED(94),
                               toFIXED(272), FACING_SOUTH_SOUTHWEST);
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
    Actor_SetAnimationById(ACTOR_SECOND_SHIPWRIGHT, ANIM_NOD);
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

/// Stepping off the gangplank onto the grass below the ship (cells (31, 24) to
/// (34, 24)), once Isaac's party has joined Felix, has Hamma greet them, "I've
/// been waiting for you, Felix", while the camera looks over the ship and its
/// new wings; she tells them to raise it with Hover and set sail.
void AttekaInlet_HammaShowsTheWings(void) {
  if (GameFlag_Get(FLAG_REUNION_AT_CONTIGO)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(520, CAMERA_KEEP, 372, 1);
    FieldCamera_WaitForArrival();
    Actor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 20);
    Event_SetMessageId((s32)MsgIVeBeenWaiting);
    // "I've been waiting for you, Felix."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHWEST);
    // "Take a look at your ship."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMA, 0);
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
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "Now that you have been given wings, all obstacles in your path will..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_ShowEmote(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "However, it is not the wings alone that grant your ship the power of..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
    // "Psynergy is the force that powers the Wings of Anemos."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    // "If the ship is to fly, you will need to focus the power of your..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_NORTH_NORTHWEST);
    // "You have visited Shaman Village recently, have you not?"
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMA, 0);
    Actor_FaceDirectionAndWait(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
    // "Now is the time to make use of the powers you acquired there."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "Now... Use the power of Hover to raise your ship!"
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAnimationById(ACTOR_HAMMA, ANIM_SHAKE_HEAD);
    // "At first, you may not want to move your ship too high or too far..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_ShowEmoteById(gMainRoutineState.leaderActorId,
                        EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_ShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "It will be difficult at first, but that is to be expected... After..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_ShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    // "But your quest should not demand much more of you..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "Now, be confident and set sail!"
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    Actor_SetSpeed(ACTOR_HAMMA, 0.8);
    Actor_GetById(ACTOR_HAMMA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWaitById(ACTOR_HAMMA, 504, 360);
    Event_Wait(1);
    Actor_GetById(ACTOR_HAMMA)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST);
    Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
    // "All of Contigo is watching... Show them your power, and make your..."
    Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
    GameFlag_Set(FLAG_HAMMA_SHOWED_THE_WINGS);
    Event_End();
  }
}

void Actor_PlaceOnActor(s32 id, s32 target);

/// Walking east along the deck towards the ship's wheel (cell (34, 16)) brings
/// Kraden out from the party to ask "Felix, are we setting off now?": yes sails
/// the ship out of the inlet onto the world map, no sends him back into the
/// party and Felix a step south, off the spot, so that Kraden asks again each
/// time he comes back to it.
void AttekaInlet_KradenAsksToSetSail(void) {
  struct Actor *actor;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  wait(1);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 532, 248);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  Event_SetMessageId(MsgAreWeSetting);
  // "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
  // don't rush on my account. Let's enjoy..."
  FieldMessage_ShowAtActor(ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
    // "Then, to sea we go!"
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    gMainRoutineState.ownerOverride = 1;
    if (Event_GetFlag(FLAG_REUNION_AT_CONTIGO)) {
      GameFlag_Set(FLAG_SHIP_FLIES);
      GameFlag_Set(FLAG_SET_OUT_WITH_WINGS);
    }
    Import_EventSetExitRequest(ATTEKA_INLET_TO_WORLD_MAP_WEST);
  } else {
    (gFieldState->message)++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    if (actor != NULL)
      Actor_SetDestinationById(ACTOR_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 528, 276);
    Event_End();
  }
}

/// Stepping onto the deck at the head of the gangplank (cell (32, 20)) after
/// Hamma has shown the wings, in a game whose Broken Seal data has Dodonpa
/// locked up, brings Master Hammet's messenger running after the party, once.
void AttekaInlet_CheckForHammetsMessenger(void) {
  if (GameFlag_Get(FLAG_TRANSFERRED_DODONPA_LOCKED_UP) &&
      GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS) &&
      !GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    AttekaInlet_HammetsMessengerArrives();
}

/// Cyclone on the bush beside the round boulder, below the cliff on the
/// heights (cell (7, 23)), blows its leaves away and, unless the party has him
/// already, Geode hops out of it; a leader standing in the bush hops aside to
/// the west first.
void AttekaInlet_CycloneUncoversGeode(void) {
  struct Actor *actor;
  s32 moved;

  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 7, 23, 0);
  if (!GameFlag_Get(FLAG_DJINNI(DJINNI_GEODE))) {
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    Event_Start();
    Event_EndLastingPsynergy(0);
    moved = 0;
    if (FIELD_CELL(actor->x.fixed) == 7 && FIELD_CELL(actor->z.fixed) == 23) {
      Actor_Jump(gMainRoutineState.leaderActorId, 6, 0);
      Actor_SetDestination(gMainRoutineState.leaderActorId, 104, 376);
      moved = 1;
    }
    Actor_SetPosition(ACTOR_GEODE, 120, 376);
    wait(1);
    Actor_Jump(ACTOR_GEODE, 4, 10);
    GameFlag_Set(FLAG_GEODE_UNCOVERED);
    if (moved)
      Actor_FaceDirectionAndRefreshById(gMainRoutineState.leaderActorId,
                                        FACING_EAST);
    Event_End();
  }
}

/// Cyclone on the westmost clump of weeds on the heights, south-west of the two
/// pillars on the path north (cell (4, 14)), scatters it. Each clump's event
/// copies bare ground from a cell nearby over the clump's cells that the whirl
/// touched; on raised ground, as all nine are, a clump is drawn four rows north
/// of the cell it answers on.
void AttekaInlet_CycloneFirstWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 4, 10, 0);
}

/// Cyclone on the clump of weeds just north-east of the westmost one, west of
/// the two pillars on the path north (cell (6, 13)), scatters it.
void AttekaInlet_CycloneSecondWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 6, 9, 0);
}

/// Cyclone on the clump of weeds just north-west of the west pillar on the
/// path north (cell (6, 10)), scatters it.
void AttekaInlet_CycloneThirdWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 6, 6, 0);
}

/// Cyclone on the clump of weeds just north-east of the east pillar on the
/// path north (cell (12, 10)), scatters it.
void AttekaInlet_CycloneFourthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(12, 5, 1, 1, 12, 6, 0);
}

/// Cyclone on the clump of weeds east of the east pillar on the path north
/// (cell (13, 11)), scatters it.
void AttekaInlet_CycloneFifthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 13, 7, 0);
}

/// Cyclone on the clump of weeds north-east of the east pillar, by the cliff's
/// edge (cell (14, 10)), scatters it.
void AttekaInlet_CycloneSixthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 14, 6, 0);
}

/// Cyclone on the west clump of the three on the clifftop above the house
/// (cell (21, 10)) scatters it.
void AttekaInlet_CycloneSeventhWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 21, 6, 0);
}

/// Cyclone on the middle clump of the three on the clifftop above the house
/// (cell (22, 10)) scatters it.
void AttekaInlet_CycloneEighthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 22, 6, 0);
}

/// Cyclone on the east clump of the three on the clifftop above the house
/// (cell (23, 11)) scatters it.
void AttekaInlet_CycloneNinthWeeds(void) {
  FieldTileCopies_CopyTouchedCells(9, 23, 1, 1, 23, 7, 0);
}

/// Master Hammet's messenger runs up the gangplank calling "Hey, wait!", sets
/// down a chest with the gift he was to bring Isaac and runs off again; Hamma
/// tells how Hammet, hurt that Isaac never came back to Kalay, sent it in
/// thanks (FLAG_HAMMETS_GIFT_DELIVERED).
void AttekaInlet_HammetsMessengerArrives(void) {
  s32 angle;

  Event_Start();
  Event_EndLastingPsynergy(0);
  angle = FACING_NORTH_NORTHEAST;
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      ACTOR_HAMMETS_MESSENGER, toFIXED(504), toFIXED(460), angle);
  wait(1);
  Event_SetMessageId((s32)MsgAttekaInletHeyWait);
  // "Hey, wait!"
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(512, CAMERA_KEEP, 380, 1);
  Actor_SetSpeed(ACTOR_HAMMETS_MESSENGER, 1.6);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 432);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 388);
  Actor_FaceDirection(ACTOR_HAMMA, FACING_SOUTH_SOUTHEAST, 0);
  // "Master Hammet requested that I deliver this to Isaac, and I totally..."
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_ShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  // "Well, you'd better hurry."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_ShowEmote(ACTOR_HAMMETS_MESSENGER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                  0);
  // "I'm supposed to deliver this to Isaac... It's not too late, is it?"
  Event_ShowMessageAndWait(ACTOR_HAMMETS_MESSENGER, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
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
  Actor_SetAnimationAndWait(ACTOR_HAMMETS_MESSENGER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 520, 434);
  FieldCamera_MoveTo(512, CAMERA_KEEP, 348, 1);
  Actor_WalkToAndWait(ACTOR_HAMMETS_MESSENGER, 504, 460);
  Actor_SetPosition(ACTOR_HAMMETS_MESSENGER, 0, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_HAMMA, angle);
  // "On my way here, I stopped in Kalay briefly to speak with Master Hammet."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_ShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "He was quite upset that you hadn't returned to visit him so that he..."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationById(ACTOR_HAMMA, ANIM_NOD);
  // "So he requested that I take this with me to Atteka."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMA, ANIM_NOD);
  Actor_ShowEmoteById(ACTOR_HAMMA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "It is a gift of thanks from Master Hammet. Take it, please."
  Event_ShowMessageAndWait(ACTOR_HAMMA, 0);
  GameFlag_Set(FLAG_HAMMETS_GIFT_DELIVERED);
  Event_End();
}

/// Nothing visible happens: each frame in the harbor, the boy by the wings and
/// two of the glowing posts are layered with the leader, so that he passes in
/// front of or behind them as he does the scenery around them. Each takes the
/// leader's sprite priority.
void AttekaInlet_ShareLeadersPriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineState.leaderActorId)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(ACTOR_BOY_ON_THE_WINGS, variant);
  Actor_SetSpritePriority(ACTOR_GLOWING_POST_A, variant);
  Actor_SetSpritePriority(ACTOR_GLOWING_POST_C, variant);
}

/// Nothing visible happens: each frame on the heights, the round boulder is
/// layered with the leader, as the harbor's posts are, by taking his sprite
/// priority.
void AttekaInlet_ShareLeadersPriorityWithBoulder(void) {
  Actor_SetSpritePriority(
      ACTOR_ROUND_BOULDER,
      ((struct FieldSprite *)Actor_GetById(gMainRoutineState.leaderActorId)
           ->sprite)
          ->priority);
}

/// Turns the blues of the band along the ship's hull and the surf's colours on
/// one step every eight frames.
void AttekaInlet_RotateHullBandAndSurfColors(void) {
  if ((*Ram_FrameCount & 7) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_LAST_COLOR);
    for (index = HULL_BAND_LAST_COLOR; index != HULL_BAND_FIRST_COLOR; index--)
      BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index) =
          BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index - 1);
    BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_FIRST_COLOR) = saved;
  }
  if ((*Ram_FrameCount & 7) == 0) {
    s16 saved;
    s32 index;

    saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_LAST_COLOR);
    for (index = SURF_LAST_COLOR; index != SURF_FIRST_COLOR; index--)
      BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index) =
          BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, index - 1);
    BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_FIRST_COLOR) = saved;
  }
}

/// Walking north into the door of the cabin on the ship's main deck (cell
/// (27, 18)) opens it; Felix walks in and the screen fades out to the cabins
/// below. The exit taken is the one the door's cell names.
void AttekaInlet_OpenDoorAndEnter(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -4);
  Event_Wait(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(state->stepTrigger);
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_ATTEKA_INLET)
    AttekaInlet_SetUpTheHarbor();
  else if (scene == SCENE_ATTEKA_INLET_GEODE)
    AttekaInlet_SetUpTheHeights();
  return 0;
}

s32 Scene_PrepareMap(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_ATTEKA_INLET && GameFlag_Get(FLAG_REUNION_AT_CONTIGO)) {
    MapGrid_CopyCellTilesIndicesRect(54, 47, 4, 15, 10, 4);
    MapGrid_CopyCellTilesIndicesRect(54, 38, 73, 7, 10, 4);
    MapGrid_CopyCellTilesIndicesRect(54, 43, 21, 55, 10, 3);
    MapGrid_CopyCellTilesIndicesRect(65, 43, 21, 20, 10, 3);
    MapGrid_CopyCellTilesIndicesRect(4, 47, 4, 50, 10, 3);
  }
  return 0;
}

/// The harbor on entry: the ship is moored here, the waves along its hull and
/// the surf on the beach move, Hammet's chest stays hidden until his messenger
/// brings it, and once the ship has its wings the four robed men and Hamma
/// stand watching the party until it has set out.
void AttekaInlet_SetUpTheHarbor(void) {
  struct MapStateGroundEffects *map =
      (struct MapStateGroundEffects *)Ram_HeapSlots->mapGrid;
  struct MainRoutineState *party;

  GameFlag_Set(FLAG_SHIP_AT_ATTEKA_INLET);
  if (!GameFlag_Get(FLAG_HAMMETS_GIFT_DELIVERED))
    Actor_SetPosition(ACTOR_HAMMETS_GIFT_CHEST, 0, 0);
  map->wading = MAP_WADING_UPPER_LEVEL;
  party = &gMainRoutineState;
  Actor_GetById(party->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldTask_AddFrame(AttekaInlet_RotateHullBandAndSurfColors, TASK_PASS_FRAME);
  FieldTask_AddFrame(AttekaInlet_ShareLeadersPriority, TASK_PASS_FRAME);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_GetById(ACTOR_CANNON)->motionFlags = 0;
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetPosition(ACTOR_MAN_AWAITING_THE_LEGEND, 0, 0);
    Actor_SetPosition(ACTOR_MAN_ON_HAMMAS_REQUEST, 0, 0);
    Actor_SetPosition(ACTOR_MAN_ON_THE_WINGED_SHIP, 0, 0);
    Actor_SetPosition(ACTOR_MAN_ON_THE_STONE_ANCHORS, 0, 0);
    Actor_SetPosition(ACTOR_HAMMA, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_HAMMA_SHOWED_THE_WINGS))
      Actor_SetPositionAndFacing(ACTOR_HAMMA, toFIXED(504), toFIXED(360),
                                 FACING_SOUTH_SOUTHEAST);
    Actor_WatchAndSetScript(ACTOR_MAN_AWAITING_THE_LEGEND,
                            party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_MAN_ON_HAMMAS_REQUEST, party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_MAN_ON_THE_WINGED_SHIP, party->leaderActorId);
    Actor_WatchAndSetScript(ACTOR_MAN_ON_THE_STONE_ANCHORS,
                            party->leaderActorId);
  }
}

/// The heights on entry: the shipwrights and the robed man are gone once the
/// ship has flown, and until then, once they have stopped the party, the
/// shipwrights stand blocking the way west; Geode stands by the round boulder
/// once uncovered, until he joins.
void AttekaInlet_SetUpTheHeights(void) {
  if (gMainRoutineState.entrance == ATTEKA_INLET_GEODE_FROM_CONTIGO_SHOPS)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  if (GameFlag_Get(FLAG_SET_OUT_WITH_WINGS)) {
    Actor_SetPosition(ACTOR_SHIPWRIGHT, 0, 0);
    Actor_SetPosition(ACTOR_SECOND_SHIPWRIGHT, 0, 0);
    Actor_SetPosition(ACTOR_MAN_WHO_SENT_EVERYONE_BACK, 0, 0);
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
  if (GameFlag_Get(FLAG_GEODE_UNCOVERED))
    Actor_SetPosition(ACTOR_GEODE, 120, 376);
  Task_Add(AttekaInlet_ShareLeadersPriorityWithBoulder, TASK_PASS_FRAME);
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

/// The harbor's actors, in AttekaInletHarborActorId's order.
struct ActorPlacement gAttekaInlet_HarborActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
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

/// The harbor's actors once the party has met again at Contigo and the ship
/// has its wings.
struct ActorPlacement gAttekaInlet_HarborActorPlacementsOnceWinged[] = {
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
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
    ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 516, 0,
                    360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH,
                    0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_BLOND_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The harbor's actors once the ship has set out with its wings.
struct ActorPlacement gAttekaInlet_HarborActorPlacementsOnceSetOut[] = {
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 376, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 216, 0, 328, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 168, 0, 184, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 488, 0, 408, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_GLOWING_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 552, 0, 424, FACING_SOUTH,
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
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER_NEAR, 204, 0, 204, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CANNON, IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
                    MOVEMENT_TYPE_STAND, 464, 0, 264, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The heights' actors once the ship has its wings.
struct ActorPlacement gAttekaInlet_HeightsActorPlacementsOnceWinged[] = {
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

/// The heights' actors, in AttekaInletHeightsActorId's order.
struct ActorPlacement gAttekaInlet_HeightsActorPlacements[] = {
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

/// The harbor's exits, the cabin door, Hamma's and Kraden's spots, talks and
/// the messenger's check.
struct Trigger gAttekaInlet_HarborTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     ATTEKA_INLET_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_OpenDoorAndEnter)},
    {TRIGGER_EXIT, ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH, CONDITION_ALWAYS,
     ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_SOUTH},
    {TRIGGER_EXIT, ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH, CONDITION_ALWAYS,
     ATTEKA_INLET_TO_ATTEKA_INLET_GEODE_NORTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_GANGPLANK_FOOT, FLAG_HAMMA_SHOWED_THE_WINGS,
     EVENT(AttekaInlet_HammaShowsTheWings)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(AttekaInlet_AskKradenToSetSail)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_THE_WHEEL, CONDITION_ALWAYS,
     EVENT(AttekaInlet_KradenAsksToSetSail)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GANGPLANK_HEAD, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CheckForHammetsMessenger)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_REUNION_AT_CONTIGO,
     MSGNO(MsgWeVeMade)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgHammaReturnedTo)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WATCHING_THE_SKIES, CONDITION_ALWAYS,
     MSGNO(MsgIVeBeenWatching)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     FLAG_REUNION_AT_CONTIGO, MSGNO(MsgWeFirstHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWeVeBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WATCHING_THE_SKIES,
     CONDITION_ALWAYS, MSGNO(MsgWheneverILook)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
     FLAG_REUNION_AT_CONTIGO, MSGNO(MsgWeMustBe)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgWasYourBoat)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_YOUNG_MAN_WONDERING_ABOUT_THE_OCEAN, FLAG_REUNION_AT_CONTIGO,
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
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_REUNION_AT_CONTIGO,
     MSGNO(MsgIWonderIfThese)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgAMassiveRock)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WINGS, CONDITION_ALWAYS,
     MSGNO(MsgYouReGoingNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     FLAG_REUNION_AT_CONTIGO, MSGNO(MsgItSKind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgIfThatBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WINGS,
     CONDITION_ALWAYS, MSGNO(MsgIVeDecided)},
    {TRIGGER_TALK, ACTOR_MAN_AWAITING_THE_LEGEND, CONDITION_ALWAYS,
     MSGNO(MsgItSAlmost)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AWAITING_THE_LEGEND,
     CONDITION_ALWAYS, MSGNO(MsgTheDayHas)},
    {TRIGGER_TALK, ACTOR_MAN_ON_HAMMAS_REQUEST, CONDITION_ALWAYS,
     MSGNO(MsgIfEveryoneWill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HAMMAS_REQUEST,
     CONDITION_ALWAYS, MSGNO(MsgTheAnemosWere)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_WINGED_SHIP, CONDITION_ALWAYS,
     MSGNO(MsgWhenTheWinged)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_WINGED_SHIP,
     CONDITION_ALWAYS, MSGNO(MsgTheseWarriorsWill)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_STONE_ANCHORS, CONDITION_ALWAYS,
     MSGNO(MsgOnceYouRemove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STONE_ANCHORS,
     CONDITION_ALWAYS, MSGNO(MsgIHearThatThe)},
    {TRIGGER_TALK, ACTOR_HAMMA, CONDITION_ALWAYS, MSGNO(MsgUseYourPsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMA, CONDITION_ALWAYS,
     MSGNO(MsgTravelSafelyThe)},
    {TRIGGER_TABLE_END},
};

/// The heights' exits, the shipwrights' spot, talks, Geode and Cyclone on its
/// weeds.
struct Trigger gAttekaInlet_HeightsTriggers[] = {
    {TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH, CONDITION_ALWAYS,
     ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_SOUTH},
    {TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
     ATTEKA_INLET_GEODE_TO_WORLD_MAP_NORTH},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH,
     ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS, CONDITION_ALWAYS,
     ATTEKA_INLET_GEODE_TO_CONTIGO_SHOPS},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH, CONDITION_ALWAYS,
     ATTEKA_INLET_GEODE_TO_ATTEKA_INLET_NORTH},
    {TRIGGER_EXIT, ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS,
     ATTEKA_INLET_GEODE_TO_WORLD_MAP_SOUTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_WAY_WEST, FLAG_SHIPWRIGHTS_STOPPED_PARTY,
     EVENT(AttekaInlet_ShipwrightsStopTheParty)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_GEODE, CONDITION_ALWAYS,
     EVENT(AttekaInlet_GeodeJoins)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CALM_SEA, FLAG_SET_OUT_WITH_WINGS,
     MSGNO(MsgEveryoneSWaiting)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CALM_SEA, CONDITION_ALWAYS,
     MSGNO(MsgAllOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CALM_SEA,
     FLAG_SET_OUT_WITH_WINGS, MSGNO(MsgHammaIsThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CALM_SEA,
     CONDITION_ALWAYS, MSGNO(MsgEveryoneInContigo)},
    {TRIGGER_TALK, ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgTheVillagersAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgItSAmazing)},
    {TRIGGER_TALK, ACTOR_SECOND_SHIPWRIGHT, CONDITION_ALWAYS,
     MSGNO(MsgHammaWouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SHIPWRIGHT,
     CONDITION_ALWAYS, MSGNO(MsgHammaSaysThat)},
    {TRIGGER_TALK, ACTOR_MAN_WHO_SENT_EVERYONE_BACK, CONDITION_ALWAYS,
     MSGNO(MsgWhenPeopleWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_WHO_SENT_EVERYONE_BACK,
     CONDITION_ALWAYS, MSGNO(MsgWeCanT)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROUND_BOULDER, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_GEODES_BUSH, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneUncoversGeode)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIRST_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneFirstWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SECOND_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneSecondWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_THIRD_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneThirdWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FOURTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneFourthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIFTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneFifthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SIXTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneSixthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SEVENTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneSeventhWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_EIGHTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneEighthWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_NINTH_WEEDS, CONDITION_ALWAYS,
     EVENT(AttekaInlet_CycloneNinthWeeds)},
    {TRIGGER_TABLE_END},
};
