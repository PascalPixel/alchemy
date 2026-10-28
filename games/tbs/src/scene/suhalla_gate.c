/// @file
/// Suhalla Gate and Gateway Cave, the pass Babi's soldiers guard on the way
/// from the Suhalla Desert to Babi Lighthouse. Three scenes share this overlay,
/// in the order the pass is crossed:
///
/// - SuhallaGateCheckpoint (SceneId_SuhallaGateCheckpoint, area name Suhalla
///   Gate): the gate itself, entered from the World Map from the south and from
///   the east, each into its own part of the map, with the house beside it
///   (Gate to Suhalla, rock_rooms.c). Two soldiers keep the passage north.
/// - SuhallaGateDew (SceneId_SuhallaGateDew, area name Suhalla Gate): beyond
/// the
///   passage, in a west part and an east part, with Dew, a Mercury Djinni, and
///   a chest.
/// - Gateway Cave (SceneId_GatewayCave, area name Gateway Cave): two caves out
///   onto the World Map, with a Psynergy Stone.
///
/// One exit table serves the three scenes; the other getters choose by
/// gAdventure.sceneId, and SuhallaGateCheckpoint's actor templates also
/// by FLAG_BABI_SPOKE_AFTER_COLOSSO. Its trigger table answers any scene not
/// otherwise named.
///
/// Before Babi's audience after Colosso the guards turn the party away for want
/// of travel papers, and metatile behavior 10 before the passage brings them
/// together in front of it (SuhallaGate_GuardsBlockPassage). After it they lie
/// wounded, a third soldier lies east of the gate, and Iodem, who travels with
/// the party, has an actor out of sight: metatile behavior 10 then plays
/// SuhallaGate_IodemQuestionsGuards once (FLAG_SUHALLA_GATE_ATTACK_SEEN), after
/// which a defeat returns the party to the house beside the gate. The third
/// soldier, talked to or read, warns of the attack on Venus Lighthouse and
/// vanishes for good (SuhallaGate_SoldierVanishes,
/// FLAG_SUHALLA_GATE_SOLDIER_VANISHED).
///
/// Scene_Initialize remembers which side of the gate the party came in by
/// (FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH, FLAG_SUHALLA_GATE_ENTERED_FROM_EAST),
/// so that Retreat from the house leads back out that way, and leaving for the
/// World Map forgets it. From TBS_PATCH_3 on, the editions lay the wounded
/// soldiers out without collision and patch the gate's cells around them, and
/// Retreat always leads back to SuhallaGateCheckpoint. Gateway Cave's Psynergy
/// Stone is kept in an area flag (FLAG_GATEWAY_CAVE_PSYNERGY_STONE), so
/// it is back whenever the party returns from another area.

#include "adventure.h"
#include "version.h"
#include "unit/scene/suhalla_gate.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "suhalla_gate.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "types.h"
#include "event_layer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"
#include "fixed_math.h"

/// Gateway Cave's Psynergy Stone has been found; an area flag, so the
/// stone is back once the party returns from another area.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_GATEWAY_CAVE_PSYNERGY_STONE FLAG_AREA_TEMP_50

#if TBS_PATCH_3
#define SUHALLA_GATE_RETREAT_MAP(currentMap)                                   \
  ((s32)&SceneId_SuhallaGateCheckpoint)
#else
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (currentMap)
#endif

/// The actors of the gate itself (SuhallaGateCheckpoint), numbered in
/// the order of gSuhallaGateCheckpointActorTemplates and, after Babi's
/// audience, of gSuhallaGateCheckpointActorTemplatesAfterAudience.
enum SuhallaGateCheckpointActorId {
  /// The two of Babi's soldiers who keep the passage north, standing before
  /// Babi's audience and lying wounded after it.
  /// ★ Named from the game: "Only those with travel papers may pass through."
  /// (MsgSuhallaTravelPapersRequired)
  ACTOR_WEST_GUARD = ACTOR_FIRST_PLACED,
  /// Asks whether the party wants to go to Babi Lighthouse.
  /// ★ Named from the game: "Only those with travel papers may pass through."
  /// (MsgSuhallaTravelPapersRequired)
  ACTOR_EAST_GUARD,
  /// After Babi's audience, the soldier east of the gate who tells of the
  /// attack on Venus Lighthouse and vanishes.
  /// ★ Named from what the player sees: a wounded soldier lying east of the
  /// gate, who flickers and vanishes (its render)
  ACTOR_VANISHING_SOLDIER,
  /// After Babi's audience, out of sight until SuhallaGate_IodemQuestionsGuards
  /// brings him out of the party.
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  ACTOR_IODEM
};

/// The actors of the second scene (SuhallaGateDew), numbered in the order
/// of gSuhallaGateDewActorTemplates.
enum SuhallaGateDewActorId {
  /// Five sprouts (SPRITE_SPROUT, the sprout Growth grows in Suhalla), each
  /// drawn in the frame Scene_Initialize gives it.
  /// ★ Named from what the player sees: small green sprouts on the grassy
  /// ledges (its render)
  ACTOR_FIRST_SPROUT = ACTOR_FIRST_PLACED,
  /// A Mercury Djinni.
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  ACTOR_DEW = ACTOR_FIRST_SPROUT + 5
};

/// SPRITE_SPROUT's animations as the sprouts here stand: half grown or fully
/// grown (spr_238's frames 3 and 6).
enum { SPROUT_ANIMATION_HALF_GROWN = 3, SPROUT_ANIMATION_GROWN = 4 };

/// Suhalla Gate's second scene (SceneId_SuhallaGateDew): talking to Dew, a
/// Mercury Djinni, starts its battle.
static void SuhallaGate_FightDew(void) {
  Field_RequestDjinnBattle(ACTOR_DEW, DJINNI_DEW);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_SuhallaGateDew) {
    return gSuhallaGateDewEntrances;
  }
  if (sceneId == (s32)&SceneId_GatewayCave) {
    return gGatewayCaveEntrances;
  }
  return gSuhallaGateEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSuhallaGateExits; }

/// Suhalla Gate's guards, standing or, after Babi's audience, lying wounded
/// beside the vanishing soldier and Iodem; Dew and the second scene's sprouts.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_SuhallaGateDew) {
    return gSuhallaGateDewActorTemplates;
  }
  if (sceneId == (s32)&SceneId_SuhallaGateCheckpoint) {
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gSuhallaGateCheckpointActorTemplatesAfterAudience;
    }
    return gSuhallaGateCheckpointActorTemplates;
  }
  return gSuhallaGateActorTemplatesOther;
}

/// The events of Suhalla Gate's second scene (SceneId_SuhallaGateDew), of
/// Gateway Cave (SceneId_GatewayCave), or of the gate itself
/// (SceneId_SuhallaGateCheckpoint) for any other scene.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_SuhallaGateDew) {
    return gSuhallaGateDewTriggers;
  }
  if (sceneId == (s32)&SceneId_GatewayCave) {
    return gGatewayCaveTriggers;
  }
  return gSuhallaGateTriggersOther;
}

/// Sets up whichever of the pass's scenes is entered. Once Iodem has
/// questioned the guards, a defeat returns the party to the house beside the
/// gate. In Suhalla Gate (SceneId_SuhallaGateCheckpoint) the vanished soldier
/// stays hidden, the side the party came in by is remembered for Retreat, and
/// arriving from the south before Babi's audience changes cells by the passage;
/// the Spanish, French and Italian editions also change cells around the
/// wounded guards. In the second scene (SceneId_SuhallaGateDew) the five
/// sprouts take their frames and one cell changes; its stretch of actor 15 does
/// nothing, as the comment at it says.
s32 Scene_Initialize(void) {
  s32 sceneId;

  if (GameFlag_Get(FLAG_SUHALLA_GATE_ATTACK_SEEN)) {
    gAdventure.restartScene = (s32)&SceneId_RockRooms;
    gAdventure.restartEntrance = ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT;
  }
  sceneId = gAdventure.sceneId;
  if (sceneId == (s32)&SceneId_SuhallaGateCheckpoint) {
    if (GameFlag_Get(FLAG_SUHALLA_GATE_SOLDIER_VANISHED))
      Import_ActorSetPosition(ACTOR_VANISHING_SOLDIER, 0, 0);
#if TBS_PATCH_3
    else if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO))
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 29, 18);
    if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 8, 25);
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 9, 26);
    }
#endif
    // Out of the house, Retreat leads back to the side the party came in by,
    // and the area's name is not shown again.
    if (gAdventure.entrance == SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS) {
      if (GameFlag_Get(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH)) {
        gAdventure.retreatScene = SUHALLA_GATE_RETREAT_MAP(sceneId);
        gAdventure.retreatEntrance =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH;
      }
      if (GameFlag_Get(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST)) {
        gAdventure.retreatScene = SUHALLA_GATE_RETREAT_MAP(sceneId);
        gAdventure.retreatEntrance =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST;
      }
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }
    if (gAdventure.entrance == SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH) {
      GameFlag_Set(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
      if (!GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO))
        MapGrid_CopyCellAttributes(6, 0, 2, 1, 8, 27);
    }
    if (gAdventure.entrance == SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST)
      GameFlag_Set(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  } else if (sceneId == (s32)&SceneId_SuhallaGateDew) {
    Import_ActorSetAnimation(ACTOR_FIRST_SPROUT, SPROUT_ANIMATION_GROWN);
    Import_ActorSetAnimation(ACTOR_FIRST_SPROUT + 1, SPROUT_ANIMATION_GROWN);
    Import_ActorSetAnimation(ACTOR_FIRST_SPROUT + 2,
                             SPROUT_ANIMATION_HALF_GROWN);
    Import_ActorSetAnimation(ACTOR_FIRST_SPROUT + 3, SPROUT_ANIMATION_GROWN);
    Import_ActorSetAnimation(ACTOR_FIRST_SPROUT + 4,
                             SPROUT_ANIMATION_HALF_GROWN);
    // UB: the scene places only actors up to ACTOR_DEW, so there is no
    // actor 15 and Actor_GetById returns NULL (watched in tbs-en
    // as the scene loads from each of its entrances); the store goes to
    // the BIOS, which ignores it.
#ifndef UBFIX
    Actor_GetById(15)->scaleY = 8 * Q16_ONE / 5;
#endif
    MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  }
  return 0;
}

/// In Suhalla Gate (SceneId_SuhallaGateCheckpoint), walking north into the door
/// of the house beside it: the door opens in two frames and the leader steps
/// into Gate to Suhalla.
static void SuhallaGate_EnterHouse(void) {
  s32 exitTrigger;
  s32 cellSize;

  exitTrigger = gFieldWork->stepTrigger;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  cellSize = 2;
  Event_PlaySong(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, cellSize, cellSize);
  WaitFrames(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, cellSize, cellSize);
  WaitFrames(4);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Import_EventSetExitRequest(exitTrigger);
}

/// The party leaves the checkpoint for the World Map. In SuhallaGateCheckpoint
/// it leaves by the south way out (SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH)
/// or the east one (SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST), forgetting the
/// side it came in by.
static void SuhallaGate_LeaveForWorldMap(void) {
  s16 exitTrigger = gFieldWork->stepTrigger;

  Sound_PlaySong(SE_MAP_EXIT);
  GameFlag_Clear(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
  GameFlag_Clear(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  Import_EventSetExitRequest(exitTrigger);
}

/// In Suhalla Gate (SceneId_SuhallaGateCheckpoint) before Babi's audience, each
/// time the party nears the passage north (metatile behavior 10): the two
/// guards step together in front of it, and the map cells around them change.
static void SuhallaGate_GuardsBlockPassage(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_WEST_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_EAST_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_WEST_GUARD, 136, 384);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 152, 384);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH, 0);
  Import_ActorSetAnimation(ACTOR_WEST_GUARD, ANIM_STAND);
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  MapGrid_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  Event_End();
}

/// In Suhalla Gate (SceneId_SuhallaGateCheckpoint) after Babi's audience, on
/// reaching the passage (metatile behavior 10), once: Iodem steps out of the
/// party and demands to know what happened. "What is the meaning of this!?" The
/// wounded guards tell of a strange group without papers whose strange powers
/// beat them; Iodem asks whether it was Felix, the party's foe, and the guards
/// send the party on after the attackers, toward Babi Lighthouse. Iodem rejoins
/// the party. Sets FLAG_SUHALLA_GATE_ATTACK_SEEN.
static void SuhallaGate_IodemQuestionsGuards(void) {
  struct Actor *leader;
#if !TBS_PATCH_3
  s32 destinationColumn;
#endif

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 438);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
  }
  WaitFrames(1);
  Actor_SetSpeed(ACTOR_IODEM, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_IODEM, 108, 431);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 20);
  Actor_StartRepeatedMotion(ACTOR_IODEM, 2);
  Event_SetMessageId(MsgSuhallaMeaning);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  // The guards describe the attack before Iodem asks who was responsible.
  Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_WalkToAndWait(ACTOR_IODEM, 132, 420);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 138, 416);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_StartRepeatedMotion(ACTOR_IODEM, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 40);
  Actor_ShowEmote(ACTOR_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_IODEM, 144, 420);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_SetAttachedEffect(ACTOR_EAST_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 20);
  // Iodem: "It must have been Felix!", and the party answers yes or no. Once he
  // has heard the wounded soldiers in Suhalla he adds that they must find
  // Sheba, then track down the monsters that did this.
  Event_AskYesNo(ACTOR_IODEM, 0);
  if (GameFlag_Get(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH + FACING_STEP, 40);
    Actor_SetAttachedEffect(ACTOR_IODEM,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(40);
    FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  } else {
    gFieldWork->message += 1;
  }
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 40);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  Actor_WalkToAndWait(ACTOR_IODEM, 138, 416);
  Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 10);
  Actor_SetAttachedEffect(ACTOR_IODEM,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_EAST_GUARD, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_EAST_GUARD, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_WEST_GUARD, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_WEST_GUARD, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_IODEM, 0, 10);
  Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IODEM);
  Actor_SetPosition(ACTOR_IODEM, 0, 0);
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  destinationColumn = 7;
  MapGrid_CopyCellAttributes(6, 27, 1, 1, destinationColumn, 27);
  MapGrid_CopyCellAttributes(9, 26, 2, 1, destinationColumn, 26);
#endif
  GameFlag_Set(FLAG_SUHALLA_GATE_ATTACK_SEEN);
  Event_End();
}

/// In Suhalla Gate (SceneId_SuhallaGateCheckpoint), talking to the east guard.
/// Until Iodem has questioned the guards he asks whether the party is bound for
/// Babi Lighthouse, `Do you want to go to Babi Lighthouse?`: yes, and the party
/// must show travel papers it does not have; no, and he calls them strange.
/// Afterwards, wounded, he vows to settle the score.
static void SuhallaGate_TalkToEastGuard(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_SUHALLA_GATE_ATTACK_SEEN) != 0) {
    Event_SetMessageId(MsgSuhallaPityColossoVictor);
    Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgSuhallaWantGoBabi);
    {
      s32 mode = 0;
      s32 no = ACTOR_EAST_GUARD;

      FieldMessage_ShowAtActor(no, mode);
    }

    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
      Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, ANIM_SHAKE_HEAD);
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
    } else {
      gFieldWork->message += 2;
      Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
    }
  }

  Event_End();
}

/// The soldier east of the gate gasps out a warning and vanishes. In Suhalla
/// Gate (SceneId_SuhallaGateCheckpoint) after Babi's audience, this plays on
/// talking to him or reading his mind, until he has vanished: he gasps that
/// Venus Lighthouse was attacked by strange wanderers, then flickers, slowly
/// and then fast, and vanishes. Sets FLAG_SUHALLA_GATE_SOLDIER_VANISHED.
static void SuhallaGate_SoldierVanishes(void) {
  unsigned int beat;

  Event_Start();

  Event_SetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
  FieldEvent_ShowMessageThenWait(ACTOR_VANISHING_SOLDIER, 0, 10);

  beat = 0;
  // Six slow blinks are followed by twelve fast ones.
  do {
    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 1);
    WaitFrames(4);

    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 0);
    beat++;
    WaitFrames(4);
  } while (beat <= 5);

  beat = 0;
  do {
    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 1);
    WaitFrames(2);

    Actor_SetSpriteColorById(ACTOR_VANISHING_SOLDIER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_VANISHING_SOLDIER), 0);
    beat++;
    WaitFrames(2);
  } while (beat <= 11);

  Actor_SetPosition(ACTOR_VANISHING_SOLDIER, 0, 0);

  GameFlag_Set(FLAG_SUHALLA_GATE_SOLDIER_VANISHED);

#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
  Event_End();
}

// How the wounded soldiers lie: animation argument 5, the west guard
// mirrored. In the Spanish, French and Italian editions each script first
// turns collision off and runs on into the one after it.
#if TBS_PATCH_3
s32 gSuhallaGateWoundedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWounded[] = {
    SC_ANIMATION(5),
    SC_END,
};

#if TBS_PATCH_3
s32 gSuhallaGateWoundedFlippedNoCollision[] = {
    SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuhallaGateWoundedFlipped[] = {
    SC_SET(SW_SCALE_X, -Q16_ONE),
    SC_ANIMATION(5),
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

struct ActorTemplate gSuhallaGateActorTemplatesOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gSuhallaGateDewActorTemplates[] = {
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(264), 0, PIXELS(400), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(280), 0, PIXELS(368), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(536), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(552), 0, PIXELS(880), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_DEW), MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gSuhallaGateCheckpointActorTemplates[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gSuhallaGateCheckpointActorTemplatesAfterAudience[] = {
#if !TBS_PATCH_3
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlipped,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        PIXELS(168), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWounded,
        PIXELS(472), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_PARTY, 0 },
#else
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedFlippedNoCollision,
        PIXELS(140), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        PIXELS(164), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, (s32)gSuhallaGateWoundedNoCollision,
        PIXELS(476), 0, PIXELS(300), FACING_SOUTH, TALK_FACING_PARTY, 0 },
#endif
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gSuhallaGateTriggersOther[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForWorldMap) },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(SuhallaGate_LeaveForWorldMap) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(SuhallaGate_EnterHouse) },
    { TRIGGER_STEP, 10, FLAG_BABI_SPOKE_AFTER_COLOSSO, EVENT(SuhallaGate_GuardsBlockPassage) },
    { TRIGGER_STEP, 10, FLAG_SUHALLA_GATE_ATTACK_SEEN, EVENT(SuhallaGate_IodemQuestionsGuards) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaTravelPapersRequired) },
    { TRIGGER_TALK, ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
    { TRIGGER_TALK, ACTOR_EAST_GUARD, CONDITION_ALWAYS, EVENT(SuhallaGate_TalkToEastGuard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaObeyBabiOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaSeeColossoNextYear) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
    { TRIGGER_TALK, ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT(SuhallaGate_SoldierVanishes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT(SuhallaGate_SoldierVanishes) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gSuhallaGateDewTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, SUHALLA_GATE_DEW_WEST_TO_EAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, SUHALLA_GATE_DEW_EAST_TO_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_GATEWAY_CAVE},
    {TRIGGER_TALK, ACTOR_DEW, CONDITION_ALWAYS, EVENT(SuhallaGate_FightDew)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_NORTH, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_EAST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {TRIGGERS_END},
};

struct Trigger gGatewayCaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, GATEWAY_CAVE_TO_SUHALLA_GATE_DEW},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
     GATEWAY_CAVE_NORTH_TO_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
     GATEWAY_CAVE_SOUTH_TO_NORTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, GATEWAY_CAVE_TO_WORLD_MAP},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100, FLAG_GATEWAY_CAVE_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {TRIGGERS_END},
};
