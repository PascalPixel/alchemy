/// @file
/// Suhalla Gate and Gateway Cave, the rocky pass Babi's soldiers guard on the
/// way from the Suhalla Desert to Lalivero and Venus Lighthouse.
///
/// Three scenes share this overlay, in the order the pass is crossed:
///
/// - SuhallaGateCheckpoint (area name Suhalla Gate): the gate itself, a
///   palisade of logs across the path north from the desert, with a house
///   in the cliff just past it (rock_rooms.c). The same map holds, in its
///   own part to the east, the pass's way out toward Venus Lighthouse. Two
///   soldiers keep the gate.
/// - SuhallaGateDew (area name Suhalla Gate): the cliffs beyond the gate, a
///   west part and an east part joined by a path. In the west part the first
///   slide down leads to a chest with a Mint; in the east part the third
///   slide from the west leads down to Dew, the Mercury Djinni, and green
///   vines climb back up the cliffs.
/// - Gateway Cave (area name Gateway Cave): a cave entered from the cliffs'
///   east part, two rooms that lead out onto the World Map, with a Psynergy
///   Stone.
///
/// One exit table serves the three scenes. The other getters choose by
/// gMainRoutineState.next, and SuhallaGateCheckpoint's actor placements
/// also by FLAG_BABI_SPOKE_AFTER_COLOSSO. Its trigger table answers any scene
/// not otherwise named.
///
/// Before Babi's audience after Colosso the guards turn the party away for want
/// of travel papers: stepping up to the gate (cells (7, 26) to (9, 27)) brings
/// them together in front of its opening (SuhallaGate_GuardsBlockPassage).
///
/// After the audience the party comes with Iodem and finds them lying wounded
/// before the gate, and a third soldier lying on the path out toward Venus
/// Lighthouse. Iodem's actor waits out of sight. Stepping up to the gate then
/// plays SuhallaGate_IodemQuestionsGuards once (FLAG_SUHALLA_GATE_ATTACK_SEEN),
/// after which a defeat returns the party to the house past the gate.
///
/// The third soldier, talked to or read, gasps that Venus Lighthouse was
/// attacked and fades away for good (SuhallaGate_SoldierVanishes,
/// FLAG_SUHALLA_GATE_SOLDIER_VANISHED).
///
/// Scene_Initialize remembers which side of the gate the party came in by
/// (FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH, FLAG_SUHALLA_GATE_ENTERED_FROM_EAST),
/// so that Retreat from the house leads back out that way. Leaving for the
/// World Map forgets it.
///
/// From TBS_PATCH_3 on, the editions lay the wounded soldiers out without
/// collision and patch the gate's cells around them, and Retreat always leads
/// back to SuhallaGateCheckpoint.
///
/// Gateway Cave's Psynergy Stone is kept in an area flag
/// (FLAG_GATEWAY_CAVE_PSYNERGY_STONE), so it is back whenever the party
/// returns from another area.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gSuhallaGateEntrancesOther[];
extern struct SceneEntrance gSuhallaGateDewEntrances[];
extern struct SceneEntrance gGatewayCaveEntrances[];
extern u32 gSuhallaGateExits[];
extern struct ActorPlacement gSuhallaGateActorPlacementsOther[];
extern struct ActorPlacement gSuhallaGateDewActorPlacements[];
extern struct ActorPlacement gSuhallaGateCheckpointActorPlacements[];
extern struct ActorPlacement
    gSuhallaGateCheckpointActorPlacementsAfterColosso[];
extern struct Trigger gSuhallaGateTriggersOther[];
extern struct Trigger gSuhallaGateDewTriggers[];
extern struct Trigger gGatewayCaveTriggers[];

/// Gateway Cave's Psynergy Stone has been found.
///
/// It is an area flag, so the stone is back once the party returns from
/// another area.
#define FLAG_GATEWAY_CAVE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/// The cells before Suhalla Gate, (7, 26) to (9, 27), where the guards block
/// the way or Iodem finds them wounded. The other cells' behaviors are the
/// numbers of the exits they lead to.
enum SuhallaGateCellBehavior { CELL_BEHAVIOR_SUHALLA_GATE_APPROACH = 10 };

/// SPRITE_SOLDIER's animation lying on his side, held (soldier.tsv).
enum { SOLDIER_ANIMATION_LIE_WOUNDED = 5 };

/// Where Retreat leads out of the checkpoint house: from patch 3 the
/// checkpoint, before it the scene the party is in.
#if TBS_PATCH_3
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (SCENE_SUHALLA_GATE_CHECKPOINT)
#else
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (currentMap)
#endif

/// The actors of the gate itself (SuhallaGateCheckpoint), numbered in
/// the order of gSuhallaGateCheckpointActorPlacements and, after Babi's
/// audience, of gSuhallaGateCheckpointActorPlacementsAfterColosso.
enum SuhallaGateCheckpointActorId {
  /// The two of Babi's soldiers who keep the gate, standing before Babi's
  /// audience and lying wounded after it. The west guard says only those with
  /// travel papers may pass, and later asks the party to send help from Babi
  /// Lighthouse.
  ACTOR_WEST_GUARD = ACTOR_FIRST_PLACED,
  /// Asks whether the party wants to go to Babi Lighthouse.
  ACTOR_EAST_GUARD,
  /// After Babi's audience, the soldier lying on the path out toward Venus
  /// Lighthouse, who tells of the attack on it and vanishes.
  ACTOR_VANISHING_SOLDIER,
  /// After Babi's audience, out of sight until SuhallaGate_IodemQuestionsGuards
  /// brings him out of the party.
  ACTOR_IODEM
};

/// The actors of the second scene (SuhallaGateDew), numbered in the order
/// of gSuhallaGateDewActorPlacements.
enum SuhallaGateDewActorId {
  /// Five green vines up the cliffs, drawn with the sprout Growth grows
  /// (SPRITE_SPROUT), three fully grown and two half grown.
  ACTOR_FIRST_SPROUT = ACTOR_FIRST_PLACED,
  /// A Mercury Djinni.
  ACTOR_DEW = ACTOR_FIRST_SPROUT + 5,
  /// The eighth placed actor, where the scene places six: Scene_Initialize
  /// stretches it all the same.
  ACTOR_EIGHTH_PLACED = ACTOR_FIRST_PLACED + 7
};

/// Talking to Dew, the Mercury Djinni at the foot of the third slide in the
/// cliffs' east part beyond Suhalla Gate (cell (16, 55)), starts its battle.
static void SuhallaGate_FightDew(void) {
  Field_RequestDjinnBattle(ACTOR_DEW, DJINNI_DEW);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_SUHALLA_GATE_DEW) {
    return gSuhallaGateDewEntrances;
  }
  if (sceneId == SCENE_GATEWAY_CAVE) {
    return gGatewayCaveEntrances;
  }
  return gSuhallaGateEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSuhallaGateExits; }

/// Suhalla Gate's guards, standing or wounded, or Dew and the vines of the
/// cliffs beyond.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_SUHALLA_GATE_DEW) {
    return gSuhallaGateDewActorPlacements;
  }
  if (sceneId == SCENE_SUHALLA_GATE_CHECKPOINT) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gSuhallaGateCheckpointActorPlacementsAfterColosso;
    }
    return gSuhallaGateCheckpointActorPlacements;
  }
  return gSuhallaGateActorPlacementsOther;
}

/// The events of the cliffs beyond the gate, of Gateway Cave, or of the gate
/// itself for any other scene.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_SUHALLA_GATE_DEW) {
    return gSuhallaGateDewTriggers;
  }
  if (sceneId == SCENE_GATEWAY_CAVE) {
    return gGatewayCaveTriggers;
  }
  return gSuhallaGateTriggersOther;
}

/// Sets up whichever of the pass's scenes is entered: after the attack a defeat
/// leads back to the house past the gate, the gate remembers which way the
/// party came in, and the cliffs' vines are drawn grown.
s32 Scene_Initialize(void) {
  s32 sceneId;

  if (GameFlag_Get(FLAG_SUHALLA_GATE_ATTACK_SEEN)) {
    gMainRoutineState.restartScene = SCENE_ROCK_ROOMS;
    gMainRoutineState.restartEntrance = ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT;
  }
  sceneId = gMainRoutineState.next;
  if (sceneId == SCENE_SUHALLA_GATE_CHECKPOINT) {
    if (GameFlag_Get(FLAG_SUHALLA_GATE_SOLDIER_VANISHED))
      Actor_SetPosition(ACTOR_VANISHING_SOLDIER, 0, 0);
#if TBS_PATCH_3
    // The wounded soldiers lie without collision here, so the two cells under
    // each are taken off the path, copied from the empty cell (0, 0) at height
    // 0: the third soldier's (29, 18) and (30, 18), the west guard's (8, 25)
    // and (9, 25), the east guard's (9, 26) and (10, 26).
    else if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO))
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 29, 18);
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 8, 25);
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 9, 26);
    }
#endif
    // Out of the house, Retreat leads back to the side the party came in by,
    // and the area's name is not shown again.
    if (gMainRoutineState.entrance == SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS) {
      if (GameFlag_Get(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH)) {
        gMainRoutineState.retreatScene = SUHALLA_GATE_RETREAT_MAP(sceneId);
        gMainRoutineState.retreatEntrance =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH;
      }
      if (GameFlag_Get(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST)) {
        gMainRoutineState.retreatScene = SUHALLA_GATE_RETREAT_MAP(sceneId);
        gMainRoutineState.retreatEntrance =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST;
      }
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }
    if (gMainRoutineState.entrance ==
        SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH) {
      GameFlag_Set(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
      // Coming up from the desert before the audience, the top of the slope
      // below the gate, (8, 27) and (9, 27), loses the guards' approach
      // behavior for the plain slope kept at (6, 0); the gate's own cells
      // still bring them out.
      if (!GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO))
        MapGrid_CopyCellAttributes(6, 0, 2, 1, 8, 27);
    }
    if (gMainRoutineState.entrance ==
        SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST)
      GameFlag_Set(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  } else if (sceneId == SCENE_SUHALLA_GATE_DEW) {
    Actor_SetAnimation(ACTOR_FIRST_SPROUT, SPROUT_ANIMATION_GROWN);
    Actor_SetAnimation(ACTOR_FIRST_SPROUT + 1, SPROUT_ANIMATION_GROWN);
    Actor_SetAnimation(ACTOR_FIRST_SPROUT + 2, SPROUT_ANIMATION_HALF_GROWN);
    Actor_SetAnimation(ACTOR_FIRST_SPROUT + 3, SPROUT_ANIMATION_GROWN);
    Actor_SetAnimation(ACTOR_FIRST_SPROUT + 4, SPROUT_ANIMATION_HALF_GROWN);
    // UB: the scene places only actors up to ACTOR_DEW, so there is no
    // ACTOR_EIGHTH_PLACED and Actor_Get returns NULL; the store goes to the
    // BIOS, which ignores it.
#ifndef UBFIX
    Actor_Get(ACTOR_EIGHTH_PLACED)->scaleY = 8 * FIXED_ONE / 5;
#endif
    // Map layer 1, whose cells begin at the map's column 64, takes Gateway
    // Cave's exit (SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, from its spare cell
    // (44, 38)) at its cell (38, 56), just north of the exit at the cave mouth
    // on the ground, (38, 57).
    MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  }
  return 0;
}

/// Walking north into the door in the cliff just past Suhalla Gate (cell
/// (8, 18)) opens it and takes the party into the house.
static void SuhallaGate_EnterHouse(void) {
  s32 exitTrigger;

  exitTrigger = gFieldState->stepTrigger;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, 2, 2);
  wait(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Import_EventSetExitRequest(exitTrigger);
}

/// Walking off Suhalla Gate's map, south down the path to the desert or east
/// toward Venus Lighthouse, leaves for the World Map.
///
/// The gate forgets which way the party came in.
static void SuhallaGate_LeaveForWorldMap(void) {
  s16 exitTrigger = gFieldState->stepTrigger;

  Sound_Play(SE_MAP_EXIT);
  GameFlag_Clear(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
  GameFlag_Clear(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  Import_EventSetExitRequest(exitTrigger);
}

/// Before Babi's audience, stepping up to Suhalla Gate (cells (7, 26) to
/// (9, 27)) makes the two guards, who stand either side of its opening, step
/// together in front of it and face the party, blocking the way north.
///
/// Then the trigger cells at (7, 26), (8, 26) and (7, 27) take the attributes
/// of their neighbours.
static void SuhallaGate_GuardsBlockPassage(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
  Actor_WalkTo(ACTOR_WEST_GUARD, 136, 384);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 152, 384);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
  // The gate's approach cells go back to plain ground until the party comes
  // again. From TBS_PATCH_3 they are copied from (11, 26), clear of the east
  // guard's cells, which are taken off the path once he lies wounded.
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  MapGrid_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  Event_End();
}

/// After Babi's audience, stepping up to Suhalla Gate (cells (7, 26) to
/// (9, 27)) finds the two guards lying wounded before it; Iodem steps out
/// of the party and demands to know what happened.
///
/// They tell of a strange group with no papers and strange powers, Iodem
/// says it must have been Felix, and the guards send the party on after them.
static void SuhallaGate_IodemQuestionsGuards(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 438);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
  }
  wait(1);
  Actor_SetSpeed(ACTOR_IODEM, 1.6);
  Actor_WalkToAndWait(ACTOR_IODEM, 108, 431);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 20);
  Actor_StartRepeatedMotion(ACTOR_IODEM, 2);
  Event_SetMessageId(MsgSuhallaMeaning);
  // "What is the meaning of this!?"
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  // The guards describe the attack before Iodem asks who was responsible.
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  // "Uh! Is... Is that Iodem...?"
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_WalkToAndWait(ACTOR_IODEM, 132, 420);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 138, 416);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 10);
  Actor_StartRepeatedMotion(ACTOR_IODEM, 2);
  // "What has happened to the gate?"
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  // "There was a strange group... No papers... They wanted to pass..."
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
  Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  // "Uhh... Iodem..."
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 144, 420);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  // "We tried to hold them back...but they were too powerful!"
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_SetAttachedEffect(ACTOR_EAST_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 3);
  // "They had strange powers... We were no match for them."
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 20);
  // The party answers Iodem, who, once he has heard the wounded soldiers in
  // Suhalla, adds that they must find Sheba, then track down the monsters.
  // "It must have been Felix!"
  // Yes: "Yeah... No one else is capable of this."
  // No: "Don't you know the work of your own foe's hand? Who else..."
  Event_AskYesNo(ACTOR_IODEM, 0);
  if (GameFlag_Get(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 40);
    Actor_SetAttachedEffect(ACTOR_IODEM,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    // "After we find Sheba, we must track down the monsters that..."
    Event_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  } else {
    // Skips "After we find Sheba...".
    gFieldState->message += 1;
  }
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 10);
  // "We must get to Babi Lighthouse as soon as possible!"
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 10);
  // "Yes, but we cannot leave the soldiers like this..."
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  Actor_WalkToAndWait(ACTOR_IODEM, 138, 416);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHWEST, 20);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  // "We're okay. Don't worry about us! Get after them, please..."
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 1);
  Event_Wait(20);
  // "But that would be..."
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 1);
  Event_Wait(20);
  // "We were bested in battle, but we will survive... Now go!"
  Event_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  // "If they have done anything at the lighthouse... It would be..."
  Event_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 1);
  Event_Wait(20);
  // "He's right. We've got to stop them!"
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHWEST, 10);
  // "We're going after them, right, Isaac!"
  Event_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  Scene_WalkIntoLeader(ACTOR_IODEM);
  // The gate's approach cells go back to plain ground, as after the guards
  // block the way.
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
  MapGrid_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  GameFlag_Set(FLAG_SUHALLA_GATE_ATTACK_SEEN);
  Event_End();
}

/// Talking to the east guard at Suhalla Gate: before Babi's audience he asks
/// whether the party wants to go to Babi Lighthouse, and a yes gets a demand
/// for travel papers, a no a scoff. After it, lying wounded, he says it is a
/// pity a Colosso victor should end up like this and swears to settle the
/// score.
static void SuhallaGate_TalkToEastGuard(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_SUHALLA_GATE_ATTACK_SEEN) != 0) {
    Event_SetMessageId(MsgSuhallaPityColossoVictor);
    // "It's a pity a Colosso victor should end up like this... Next..."
    Event_ShowMessage(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgSuhallaWantGoBabi);
    // "Do you want to go to Babi Lighthouse?"
    Event_OpenMessage(ACTOR_EAST_GUARD, 0);

    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      // Yes: "Show me your travel papers, and I'll let you pass!"
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
      Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_SHAKE_HEAD);
      // "What! You don't have travel papers? Then I cannot let you pass!"
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
      gFieldState->message += 2;
      // No: "Hmmph! You don't want to see Babi Lighthouse? You're a strange
      // one!"
      Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
  }

  Event_End();
}

/// The vanishing soldier's blinks: slow ones, four frames shown and four
/// hidden, then fast ones, two and two.
enum { SOLDIER_SLOW_BLINKS = 6, SOLDIER_FAST_BLINKS = 12 };

/// The wounded soldier on the path out toward Venus Lighthouse, talked to or
/// read with Mind Read, gasps that Venus Lighthouse was attacked by strange
/// wanderers, then blinks out and is gone (cell (29, 19)).
static void SuhallaGate_SoldierVanishes(void) {
  unsigned int beat;

  Event_Start();

  Event_SetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
  // "Venus Lighthouse was... attacked by strange... wanderers..."
  Event_ShowMessageThenWait(ACTOR_VANISHING_SOLDIER, 0, 10);

  beat = 0;
  do {
    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER),
                         SPRITE_FLAG_SHADOW);
    wait(4);

    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 0);
    beat++;
    wait(4);
  } while (beat < SOLDIER_SLOW_BLINKS);

  for (beat = 0; beat < SOLDIER_FAST_BLINKS; beat++) {
    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER),
                         SPRITE_FLAG_SHADOW);
    wait(2);
    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 0);
    wait(2);
  }

  Actor_SetPosition(ACTOR_VANISHING_SOLDIER, 0, 0);

  GameFlag_Set(FLAG_SUHALLA_GATE_SOLDIER_VANISHED);

  // The path the third soldier lay on is open again.
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
  Event_End();
}

// How the wounded soldiers lie, the west guard mirrored to lie the other way.
// In the Spanish, French and Italian editions each script first turns collision
// off and runs on into the one after it.
#if TBS_PATCH_3
s32 gSuhallaGateWoundedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWounded[] = {
    SC_ANIMATION(SOLDIER_ANIMATION_LIE_WOUNDED),
    SC_END,
};

#if TBS_PATCH_3
s32 gSuhallaGateWoundedFlippedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWoundedFlipped[] = {
    SC_SET(SW_SCALE_X, -FIXED_ONE),
    SC_ANIMATION(SOLDIER_ANIMATION_LIE_WOUNDED),
    SC_END,
};

struct SceneEntrance gSuhallaGateEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0,
     520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST, CONDITION_ALWAYS, 248,
     0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS, CONDITION_ALWAYS, 136, 0, 304,
     FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST, CONDITION_ALWAYS, 360,
     0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 664, 0, 424,
     FACING_WEST, 0, 336, 32, 736, 496, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaGateDewEntrances[] = {
    {SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST, CONDITION_ALWAYS, 40,
     0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0},
    {SUHALLA_GATE_DEW_WEST_FROM_EAST, CONDITION_ALWAYS, 392, 0, 368,
     FACING_WEST, 0, 16, 32, 416, 480, 0},
    {SUHALLA_GATE_DEW_EAST_FROM_WEST, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST,
     0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, 680,
     0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0},
    {SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE, CONDITION_ALWAYS, 616, 0, 920,
     FACING_SOUTH, 0, 16, 608, 720, 1008, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gGatewayCaveEntrances[] = {
    {GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW, CONDITION_ALWAYS, 72, 0, 264,
     FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {GATEWAY_CAVE_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH,
     0, 24, 24, 280, 256, 0},
    {GATEWAY_CAVE_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH,
     0, 208, 288, 704, 640, 0},
    {GATEWAY_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH,
     0, 208, 288, 704, 640, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaGateExits[] = {
    SCENE_EXITS(SuhallaGateCheckpoint),
    SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH, WorldMap,
               WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH),
    SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST, SuhallaGateDew,
               SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST),
    SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_ROCK_ROOMS, RockRooms,
               ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT),
    SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST, SuhallaGateDew,
               SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
    SCENE_EXIT(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
    SCENE_EXITS(SuhallaGateDew),
    SCENE_EXIT(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST,
               SuhallaGateCheckpoint,
               SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST),
    SCENE_EXIT(SUHALLA_GATE_DEW_WEST_TO_EAST, SuhallaGateDew,
               SUHALLA_GATE_DEW_EAST_FROM_WEST),
    SCENE_EXIT(SUHALLA_GATE_DEW_EAST_TO_WEST, SuhallaGateDew,
               SUHALLA_GATE_DEW_WEST_FROM_EAST),
    SCENE_EXIT(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST,
               SuhallaGateCheckpoint,
               SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST),
    SCENE_EXIT(SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, GatewayCave,
               GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW),
    SCENE_EXITS(GatewayCave),
    SCENE_EXIT(GATEWAY_CAVE_TO_SUHALLA_GATE_DEW, SuhallaGateDew,
               SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE),
    SCENE_EXIT(GATEWAY_CAVE_NORTH_TO_SOUTH, GatewayCave,
               GATEWAY_CAVE_SOUTH_FROM_NORTH),
    SCENE_EXIT(GATEWAY_CAVE_SOUTH_TO_NORTH, GatewayCave,
               GATEWAY_CAVE_NORTH_FROM_SOUTH),
    SCENE_EXIT(GATEWAY_CAVE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_GATEWAY_CAVE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSuhallaGateActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gSuhallaGateDewActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        264, 0, 400, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 368, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        472, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        536, 0, 912, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        552, 0, 880, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_DEW), MOVEMENT_TYPE_STAND,
        264, 0, 888, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaGateCheckpointActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaGateCheckpointActorPlacementsAfterColosso[] = {
#if !TBS_PATCH_3
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlipped,
        136, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        168, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        472, 0, 304, FACING_SOUTH, TALK_FACING_PARTY, 0),
#else
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlippedNoCollision,
        140, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        164, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        476, 0, 300, FACING_SOUTH, TALK_FACING_PARTY, 0),
#endif
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gSuhallaGateTriggersOther[] = {
    { TRIGGER_STEP, SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForWorldMap) },
    { TRIGGER_EXIT, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST },
    { TRIGGER_EXIT, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST },
    { TRIGGER_STEP, SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForWorldMap) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_GATE_CHECKPOINT_TO_ROCK_ROOMS, CONDITION_ALWAYS, EVENT(SuhallaGate_EnterHouse) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SUHALLA_GATE_APPROACH, FLAG_BABI_SPOKE_AFTER_COLOSSO, EVENT(SuhallaGate_GuardsBlockPassage) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SUHALLA_GATE_APPROACH, FLAG_SUHALLA_GATE_ATTACK_SEEN, EVENT(SuhallaGate_IodemQuestionsGuards) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaTravelPapersRequired) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
    { TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, EVENT(SuhallaGate_TalkToEastGuard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaObeyBabiOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaSeeColossoNextYear) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
    { TRIGGER_TALK, ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT(SuhallaGate_SoldierVanishes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT(SuhallaGate_SoldierVanishes) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gSuhallaGateDewTriggers[] = {
    {TRIGGER_EXIT, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST,
     CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST},
    {TRIGGER_EXIT, SUHALLA_GATE_DEW_WEST_TO_EAST, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_WEST_TO_EAST},
    {TRIGGER_EXIT, SUHALLA_GATE_DEW_EAST_TO_WEST, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_EAST_TO_WEST},
    {TRIGGER_EXIT, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST,
     CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST},
    {TRIGGER_EXIT, SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_TO_GATEWAY_CAVE},
    {TRIGGER_TALK, ACTOR_DEW, CONDITION_ALWAYS, EVENT(SuhallaGate_FightDew)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_NORTH, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_EAST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGatewayCaveTriggers[] = {
    {TRIGGER_EXIT, GATEWAY_CAVE_TO_SUHALLA_GATE_DEW, CONDITION_ALWAYS,
     GATEWAY_CAVE_TO_SUHALLA_GATE_DEW},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, GATEWAY_CAVE_NORTH_TO_SOUTH,
     CONDITION_ALWAYS, GATEWAY_CAVE_NORTH_TO_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, GATEWAY_CAVE_SOUTH_TO_NORTH,
     CONDITION_ALWAYS, GATEWAY_CAVE_SOUTH_TO_NORTH},
    {TRIGGER_EXIT, GATEWAY_CAVE_TO_WORLD_MAP, CONDITION_ALWAYS,
     GATEWAY_CAVE_TO_WORLD_MAP},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100, FLAG_GATEWAY_CAVE_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {TRIGGER_TABLE_END},
};
