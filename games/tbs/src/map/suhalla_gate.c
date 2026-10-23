/// @file
/// Suhalla Gate and Gateway Cave, the pass Babi's soldiers guard on the way
/// from the Suhalla Desert to Babi Lighthouse. Three maps share this overlay,
/// in the order the pass is crossed:
///
/// - SuhallaGateCheckpoint (MapId_SuhallaGateCheckpoint, area name Suhalla
///   Gate): the gate itself, entered from the world map from the south and
///   from the east, each into its own part of the map, with the house beside
///   it (Gate to Suhalla, lunpa_interiors.c). Two soldiers keep the passage
///   north.
/// - SuhallaGateDew (MapId_SuhallaGateDew, area name Suhalla Gate): beyond
///   the passage, in a west part and an east part, with Dew, a Mercury
///   Djinni, and a chest.
/// - Gateway Cave (MapId_GatewayCave, area name Gateway Cave): two caves
///   out onto the world map, with a Psynergy Stone.
///
/// One warp table serves the three maps; the other getters choose by
/// gGameState.mapId, and SuhallaGateCheckpoint's object event templates also
/// by FLAG_BABI_SPOKE_AFTER_COLOSSO. Its event table answers any map not
/// otherwise named.
///
/// Before Babi's audience after Colosso the guards turn the party away for want
/// of travel papers, and metatile behavior 10 before the passage brings them
/// together in front of it (EventScript_GuardsBlockPassage). After it they lie
/// wounded, a third soldier lies east of the gate, and Iodem, who travels with
/// the party, has an object event out of sight: metatile behavior 10 then plays
/// EventScript_IodemQuestionsGuards once (FLAG_SUHALLA_GATE_ATTACK_SEEN), after
/// which a defeat returns the party to the house beside the gate. The third
/// soldier, talked to or read, warns of the attack on Venus Lighthouse and
/// vanishes for good (EventScript_SoldierVanishes,
/// FLAG_SUHALLA_GATE_SOLDIER_VANISHED).
///
/// Map_Initialize remembers which side of the gate the party came in by
/// (FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH, FLAG_SUHALLA_GATE_ENTERED_FROM_EAST),
/// so that Retreat from the house leads back out that way, and leaving for the
/// world map forgets it. From TBS_PATCH_3 on, the editions lay the wounded
/// soldiers out without collision and patch the gate's cells around them, and
/// Retreat always leads back to SuhallaGateCheckpoint. Gateway Cave's
/// Psynergy Stone is kept in a map section flag (FLAG_MAPSEC_TEMP_50), so it is
/// back whenever the party returns from another map section.
///
/// Open questions: none.

#include "game_state.h"
#include "version.h"
#include "unit/map/suhalla_gate.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "map_tables.h"
#include "djinn_ids.h"
#include "types.h"
#include "script_layer.h"
#include "map_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"
#include "fixed_math.h"

#if TBS_PATCH_3
#define SUHALLA_GATE_RETREAT_MAP(currentMap) ((s32)&MapId_SuhallaGateCheckpoint)
#else
#define SUHALLA_GATE_RETREAT_MAP(currentMap) (currentMap)
#endif

/// The object events of the gate itself (SuhallaGateCheckpoint), numbered in
/// the order of gSuhallaGateCheckpointObjectEventTemplates and, after Babi's
/// audience, of gSuhallaGateCheckpointObjectEventTemplatesAfterAudience.
enum SuhallaGateCheckpointLocalId {
  /// The two of Babi's soldiers who keep the passage north, standing before
  /// Babi's audience and lying wounded after it.
  /// ★ Named from the game: "Only those with travel papers may pass through."
  /// (MsgSuhallaTravelPapersRequired)
  LOCALID_WEST_GUARD = LOCALID_FIRST_PLACED,
  /// Asks whether the party wants to go to Babi Lighthouse.
  /// ★ Named from the game: "Only those with travel papers may pass through."
  /// (MsgSuhallaTravelPapersRequired)
  LOCALID_EAST_GUARD,
  /// After Babi's audience, the soldier east of the gate who tells of the
  /// attack on Venus Lighthouse and vanishes.
  /// ★ Named from what the player sees: a wounded soldier lying east of the
  /// gate, who flickers and vanishes (its render)
  LOCALID_VANISHING_SOLDIER,
  /// After Babi's audience, out of sight until EventScript_IodemQuestionsGuards
  /// brings him out of the party.
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  LOCALID_IODEM
};

/// The object events of the second map (SuhallaGateDew), numbered in the order
/// of gSuhallaGateDewObjectEventTemplates.
enum SuhallaGateDewLocalId {
  /// Five sprouts (SPRITE_SPROUT, the sprout Growth grows in Suhalla), each
  /// drawn in the frame Map_Initialize gives it.
  /// ★ Named from what the player sees: small green sprouts on the grassy
  /// ledges (its render)
  LOCALID_FIRST_SPROUT = LOCALID_FIRST_PLACED,
  /// A Mercury Djinni.
  /// ★ Named from the game: "Dew" (MsgDjinnName)
  LOCALID_DEW = LOCALID_FIRST_SPROUT + 5
};

/// SPRITE_SPROUT's animations as the sprouts here stand: half grown or fully
/// grown (spr_238's frames 3 and 6).
enum { SPROUT_ANIMATION_HALF_GROWN = 3, SPROUT_ANIMATION_GROWN = 4 };

/// Suhalla Gate's second map (MapId_SuhallaGateDew): talking to Dew, a
/// Mercury Djinni, starts its battle.
static void EventScript_FightDjinni(void) {
  Field_RequestDjinnBattle(LOCALID_DEW, DJINNI_DEW);
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_SuhallaGateDew) {
    return gSuhallaGateDewWarpDests;
  }
  if (mapId == (s32)&MapId_GatewayCave) {
    return gGatewayCaveWarpDests;
  }
  return gSuhallaGateWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gSuhallaGateWarps; }

/// Suhalla Gate's guards, standing or, after Babi's audience, lying wounded
/// beside the vanishing soldier and Iodem; Dew and the second map's sprouts.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_SuhallaGateDew) {
    return gSuhallaGateDewObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_SuhallaGateCheckpoint) {
    if (GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
      return gSuhallaGateCheckpointObjectEventTemplatesAfterAudience;
    }
    return gSuhallaGateCheckpointObjectEventTemplates;
  }
  return gSuhallaGateObjectEventTemplatesOther;
}

/// The events of Suhalla Gate's second map (MapId_SuhallaGateDew), of
/// Gateway Cave (MapId_GatewayCave), or of the gate itself
/// (MapId_SuhallaGateCheckpoint) for any other map.
const struct MapEvent *Map_GetEvents(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_SuhallaGateDew) {
    return gSuhallaGateDewEvents;
  }
  if (mapId == (s32)&MapId_GatewayCave) {
    return gGatewayCaveEvents;
  }
  return gSuhallaGateEventsOther;
}

/// Sets up whichever of the pass's maps is entered. Once Iodem has
/// questioned the guards, a defeat returns the party to the house beside the
/// gate. In Suhalla Gate (MapId_SuhallaGateCheckpoint) the vanished soldier
/// stays hidden, the side the party came in by is remembered for Retreat, and
/// arriving from the south before Babi's audience changes cells by the passage;
/// the Spanish, French and Italian editions also change cells around the
/// wounded guards. In the second map (MapId_SuhallaGateDew) the five sprouts
/// take their frames and one cell changes; its stretch of object event 15 does
/// nothing, as the comment at it says.
s32 Map_Initialize(void) {
  s32 mapId;

  if (GameFlag_Test(FLAG_SUHALLA_GATE_ATTACK_SEEN)) {
    gGameState.savedMap = (s32)&MapId_LunpaInteriors;
    gGameState.savedWarpDest = LUNPA_INTERIORS_FROM_SUHALLA_GATE_CHECKPOINT;
  }
  mapId = gGameState.mapId;
  if (mapId == (s32)&MapId_SuhallaGateCheckpoint) {
    if (GameFlag_Test(FLAG_SUHALLA_GATE_SOLDIER_VANISHED))
      Engine_ObjectEventSetPosition(LOCALID_VANISHING_SOLDIER, 0, 0);
#if TBS_PATCH_3
    else if (GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO))
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 29, 18);
    if (GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 8, 25);
      MapGrid_CopyCellAttributes(0, 0, 2, 1, 9, 26);
    }
#endif
    // Out of the house, Retreat leads back to the side the party came in by,
    // and the map section's name is not shown again.
    if (gGameState.warpDest == SUHALLA_GATE_CHECKPOINT_FROM_LUNPA_INTERIORS) {
      if (GameFlag_Test(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH)) {
        gGameState.retreatMap = SUHALLA_GATE_RETREAT_MAP(mapId);
        gGameState.retreatWarpDest =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH;
      }
      if (GameFlag_Test(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST)) {
        gGameState.retreatMap = SUHALLA_GATE_RETREAT_MAP(mapId);
        gGameState.retreatWarpDest =
            SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST;
      }
      GameFlag_ClearBit(FLAG_SHOW_LOCATION_NAME);
    }
    if (gGameState.warpDest == SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH) {
      Engine_GameFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
      if (!GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO))
        MapGrid_CopyCellAttributes(6, 0, 2, 1, 8, 27);
    }
    if (gGameState.warpDest == SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST)
      Engine_GameFlagSet(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  } else if (mapId == (s32)&MapId_SuhallaGateDew) {
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_SPROUT,
                                   SPROUT_ANIMATION_GROWN);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_SPROUT + 1,
                                   SPROUT_ANIMATION_GROWN);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_SPROUT + 2,
                                   SPROUT_ANIMATION_HALF_GROWN);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_SPROUT + 3,
                                   SPROUT_ANIMATION_GROWN);
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_SPROUT + 4,
                                   SPROUT_ANIMATION_HALF_GROWN);
    // UB: the map places only object events up to LOCALID_DEW, so there is no
    // object event 15 and ObjectEvent_GetById returns NULL (watched in tbs-en
    // as the map loads from each of its warp destinations); the store goes to
    // the BIOS, which ignores it.
#ifndef UBFIX
    ObjectEvent_GetById(15)->scaleY = 8 * Q16_ONE / 5;
#endif
    MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  }
  return 0;
}

/// In Suhalla Gate (MapId_SuhallaGateCheckpoint), walking north into the door
/// of the house beside it: the door opens in two frames and the leader steps
/// into Gate to Suhalla.
static void EventScript_EnterHouse(void) {
  s32 warpTrigger;
  s32 cellSize;

  warpTrigger = gFieldWork->coordEvent;
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 0;
  cellSize = 2;
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(66, 36, 71, 8, cellSize, cellSize);
  WaitFrames(4);
  MapGrid_CopyCellsTo(68, 36, 71, 8, cellSize, cellSize);
  WaitFrames(4);
  ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 3, -16);
  Engine_ScriptRequestWarp(warpTrigger);
}

/// The party leaves the checkpoint for the world map. In SuhallaGateCheckpoint
/// it leaves by the south way out (SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH)
/// or the east one (SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST), forgetting the
/// side it came in by.
static void EventScript_LeaveForWorldMap(void) {
  s16 warpTrigger = gFieldWork->coordEvent;

  Sound_PlaySong(SE_MAP_EXIT);
  GameFlag_ClearBit(FLAG_SUHALLA_GATE_ENTERED_FROM_SOUTH);
  GameFlag_ClearBit(FLAG_SUHALLA_GATE_ENTERED_FROM_EAST);
  Engine_ScriptRequestWarp(warpTrigger);
}

/// In Suhalla Gate (MapId_SuhallaGateCheckpoint) before Babi's audience, each
/// time the party nears the passage north (metatile behavior 10): the two
/// guards step together in front of it, and the map cells around them change.
static void EventScript_GuardsBlockPassage(void) {
  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_WEST_GUARD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_EAST_GUARD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_WEST_GUARD, 136, 384);
  ObjectEvent_WalkToAndWait(LOCALID_EAST_GUARD, 152, 384);
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH, 0);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_GUARD, ANIMATION_STAND);
  MapGrid_CopyCellAttributes(6, 27, 1, 1, 7, 27);
#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 7, 26);
  MapGrid_CopyCellAttributes(11, 26, 1, 1, 8, 26);
#else
  MapGrid_CopyCellAttributes(9, 26, 2, 1, 7, 26);
#endif
  FieldScript_End();
}

/// In Suhalla Gate (MapId_SuhallaGateCheckpoint) after Babi's audience, on
/// reaching the passage (metatile behavior 10), once: Iodem steps out of the
/// party and demands to know what happened. "What is the meaning of this!?" The
/// wounded guards tell of a strange group without papers whose strange powers
/// beat them; Iodem asks whether it was Felix, the party's foe, and the guards
/// send the party on after the attackers, toward Babi Lighthouse. Iodem rejoins
/// the party. Sets FLAG_SUHALLA_GATE_ATTACK_SEEN.
static void EventScript_IodemQuestionsGuards(void) {
  struct ObjectEvent *leader;
#if !TBS_PATCH_3
  s32 destinationColumn;
#endif

  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 120, 438);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(LOCALID_IODEM, leader->x.fixed, leader->z.fixed);
  }
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 108, 431);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_IODEM, 2);
  FieldScript_SetMessageId(MsgSuhallaMeaning);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 40);
  // The guards describe the attack before Iodem asks who was responsible.
  ObjectEvent_ShowEmote(LOCALID_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 132, 420);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 138, 416);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectMotion_SetVariantCallback(LOCALID_IODEM, 2);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_WEST_GUARD, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_EAST_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_EAST_GUARD, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 144, 420);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_EAST_GUARD, 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_EAST_GUARD, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVariantCallback(LOCALID_EAST_GUARD, 3);
  Script_ShowMessageAndWait(LOCALID_EAST_GUARD, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + FACING_STEP, 20);
  // Iodem: "It must have been Felix!", and the party answers yes or no. Once he
  // has heard the wounded soldiers in Suhalla he adds that they must find
  // Sheba, then track down the monsters that did this.
  Script_AskYesNo(LOCALID_IODEM, 0);
  if (GameFlag_IsSet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH + FACING_STEP, 40);
    ObjectEvent_SetAttachedEffect(LOCALID_IODEM, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Script_ShowMessageAndWait(LOCALID_IODEM, 0, 10);
  } else {
    gFieldWork->message += 1;
  }
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 40);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_IODEM, 138, 416);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetVariantCallback(LOCALID_WEST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_WEST_GUARD, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_IODEM, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_GUARD, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_GUARD, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_EAST_GUARD, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_GUARD, 2);
  Script_ShowMessageAndWait(LOCALID_WEST_GUARD, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_IODEM, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_IODEM, ANIMATION_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetDestination(LOCALID_IODEM, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
  ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
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
  FieldScript_End();
}

/// In Suhalla Gate (MapId_SuhallaGateCheckpoint), talking to the east guard.
/// Until Iodem has questioned the guards he asks whether the party is bound for
/// Babi Lighthouse, `Do you want to go to Babi Lighthouse?`: yes, and the party
/// must show travel papers it does not have; no, and he calls them strange.
/// Afterwards, wounded, he vows to settle the score.
static void EventScript_TalkToEastGuard(void) {
  FieldScript_Start();

  if (GameFlag_Test(FLAG_SUHALLA_GATE_ATTACK_SEEN) != 0) {
    FieldScript_SetMessageId(MsgSuhallaPityColossoVictor);
    FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
  } else {
    FieldScript_SetMessageId(MsgSuhallaWantGoBabi);
    {
      s32 mode = 0;
      s32 no = LOCALID_EAST_GUARD;

      FieldMessage_ShowAtObject(no, mode);
    }

    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        0) {
      FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
      ObjectMotion_SetAnimationAndWait(LOCALID_EAST_GUARD,
                                       ANIMATION_SHAKE_HEAD);
      FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
    } else {
      gFieldWork->message += 2;
      FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
    }
  }

  FieldScript_End();
}

/// The soldier east of the gate gasps out a warning and vanishes. In Suhalla
/// Gate (MapId_SuhallaGateCheckpoint) after Babi's audience, this plays on
/// talking to him or reading his mind, until he has vanished: he gasps that
/// Venus Lighthouse was attacked by strange wanderers, then flickers, slowly
/// and then fast, and vanishes. Sets FLAG_SUHALLA_GATE_SOLDIER_VANISHED.
static void EventScript_SoldierVanishes(void) {
  unsigned int beat;

  FieldScript_Start();

  FieldScript_SetMessageId(MsgFieldVenusLighthouseWasAttackedBy);
  Script_ShowMessageAndWait(LOCALID_VANISHING_SOLDIER, 0, 10);

  beat = 0;
  // Six slow blinks are followed by twelve fast ones.
  do {
    ObjectEvent_SetChildValue(LOCALID_VANISHING_SOLDIER, 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_VANISHING_SOLDIER), 1);
    WaitFrames(4);

    ObjectEvent_SetChildValue(LOCALID_VANISHING_SOLDIER, 15);
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_VANISHING_SOLDIER), 0);
    beat++;
    WaitFrames(4);
  } while (beat <= 5);

  beat = 0;
  do {
    ObjectEvent_SetChildValue(LOCALID_VANISHING_SOLDIER, 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_VANISHING_SOLDIER), 1);
    WaitFrames(2);

    ObjectEvent_SetChildValue(LOCALID_VANISHING_SOLDIER, 15);
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_VANISHING_SOLDIER), 0);
    beat++;
    WaitFrames(2);
  } while (beat <= 11);

  ObjectEvent_SetPosition(LOCALID_VANISHING_SOLDIER, 0, 0);

  GameFlag_Set(FLAG_SUHALLA_GATE_SOLDIER_VANISHED);

#if TBS_PATCH_3
  MapGrid_CopyCellAttributes(29, 19, 2, 1, 29, 18);
#endif
  FieldScript_End();
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

struct MapWarpDest gSuhallaGateWarpDestsOther[] = {
    {0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0,
     520, FACING_NORTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST, CONDITION_ALWAYS, 248,
     0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_LUNPA_INTERIORS, CONDITION_ALWAYS, 136, 0,
     304, FACING_SOUTH, 0, 32, 32, 272, 544, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST, CONDITION_ALWAYS, 360,
     0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0},
    {SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 664, 0, 424,
     FACING_WEST, 0, 336, 32, 736, 496, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSuhallaGateDewWarpDests[] = {
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
    {MAP_TABLE_END},
};

struct MapWarpDest gGatewayCaveWarpDests[] = {
    {GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW, CONDITION_ALWAYS, 72, 0, 264,
     FACING_NORTH, 0, 24, 24, 280, 256, 0},
    {GATEWAY_CAVE_NORTH_FROM_SOUTH, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH,
     0, 24, 24, 280, 256, 0},
    {GATEWAY_CAVE_SOUTH_FROM_NORTH, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH,
     0, 208, 288, 704, 640, 0},
    {GATEWAY_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH,
     0, 208, 288, 704, 640, 0},
    {MAP_TABLE_END},
};

u32 gSuhallaGateWarps[] = {
    MAP_WARPS(SuhallaGateCheckpoint),
    MAP_WARP(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_SOUTH, WorldMap,
             WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH),
    MAP_WARP(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST, SuhallaGateDew,
             SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_WEST),
    MAP_WARP(SUHALLA_GATE_CHECKPOINT_TO_LUNPA_INTERIORS, LunpaInteriors,
             LUNPA_INTERIORS_FROM_SUHALLA_GATE_CHECKPOINT),
    MAP_WARP(SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST, SuhallaGateDew,
             SUHALLA_GATE_DEW_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
    MAP_WARP(SUHALLA_GATE_CHECKPOINT_TO_WORLD_MAP_EAST, WorldMap,
             WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST),
    MAP_WARPS(SuhallaGateDew),
    MAP_WARP(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST,
             SuhallaGateCheckpoint,
             SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_WEST),
    MAP_WARP(SUHALLA_GATE_DEW_WEST_TO_EAST, SuhallaGateDew,
             SUHALLA_GATE_DEW_EAST_FROM_WEST),
    MAP_WARP(SUHALLA_GATE_DEW_EAST_TO_WEST, SuhallaGateDew,
             SUHALLA_GATE_DEW_WEST_FROM_EAST),
    MAP_WARP(SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST,
             SuhallaGateCheckpoint,
             SUHALLA_GATE_CHECKPOINT_FROM_SUHALLA_GATE_DEW_EAST),
    MAP_WARP(SUHALLA_GATE_DEW_TO_GATEWAY_CAVE, GatewayCave,
             GATEWAY_CAVE_FROM_SUHALLA_GATE_DEW),
    MAP_WARPS(GatewayCave),
    MAP_WARP(GATEWAY_CAVE_TO_SUHALLA_GATE_DEW, SuhallaGateDew,
             SUHALLA_GATE_DEW_FROM_GATEWAY_CAVE),
    MAP_WARP(GATEWAY_CAVE_NORTH_TO_SOUTH, GatewayCave,
             GATEWAY_CAVE_SOUTH_FROM_NORTH),
    MAP_WARP(GATEWAY_CAVE_SOUTH_TO_NORTH, GatewayCave,
             GATEWAY_CAVE_NORTH_FROM_SOUTH),
    MAP_WARP(GATEWAY_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GATEWAY_CAVE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSuhallaGateObjectEventTemplatesOther[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gSuhallaGateDewObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaGateCheckpointObjectEventTemplates[] = {
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaGateCheckpointObjectEventTemplatesAfterAudience[] = {
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
    { MAP_TABLE_END },
};

struct MapEvent gSuhallaGateEventsOther[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveForWorldMap) },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_WEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SUHALLA_GATE_CHECKPOINT_TO_SUHALLA_GATE_DEW_EAST },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveForWorldMap) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterHouse) },
    { MAP_EVENT_COORD, 10, FLAG_BABI_SPOKE_AFTER_COLOSSO, EVENT_SCRIPT(EventScript_GuardsBlockPassage) },
    { MAP_EVENT_COORD, 10, FLAG_SUHALLA_GATE_ATTACK_SEEN, EVENT_SCRIPT(EventScript_IodemQuestionsGuards) },
    { MAP_EVENT_TALK, LOCALID_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaTravelPapersRequired) },
    { MAP_EVENT_TALK, LOCALID_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSendHelpFromTower) },
    { MAP_EVENT_TALK, LOCALID_EAST_GUARD, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToEastGuard) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaObeyBabiOrders) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaAshamedBeforeIodem) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, FLAG_SUHALLA_GATE_ATTACK_SEEN, MSGNO(MsgSuhallaSeeColossoNextYear) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_GUARD, CONDITION_ALWAYS, MSGNO(MsgSuhallaSurvivedAttack) },
    { MAP_EVENT_TALK, LOCALID_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT_SCRIPT(EventScript_SoldierVanishes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), MAP_EVENT_TRIGGER_OWN_EFFECT | LOCALID_VANISHING_SOLDIER, FLAG_SUHALLA_GATE_SOLDIER_VANISHED, EVENT_SCRIPT(EventScript_SoldierVanishes) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gSuhallaGateDewEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_WEST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SUHALLA_GATE_DEW_WEST_TO_EAST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, SUHALLA_GATE_DEW_EAST_TO_WEST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     SUHALLA_GATE_DEW_TO_SUHALLA_GATE_CHECKPOINT_EAST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SUHALLA_GATE_DEW_TO_GATEWAY_CAVE},
    {MAP_EVENT_TALK, LOCALID_DEW, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_FightDjinni)},
    {BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_NORTH, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_EAST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_WEST, 100,
     FLAG_SUHALLA_GATE_MINT, FIND_ITEM(ITEM_MINT)},
    {MAP_EVENTS_END},
};

struct MapEvent gGatewayCaveEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, GATEWAY_CAVE_TO_SUHALLA_GATE_DEW},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
     GATEWAY_CAVE_NORTH_TO_SOUTH},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
     GATEWAY_CAVE_SOUTH_TO_NORTH},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, GATEWAY_CAVE_TO_WORLD_MAP},
    {BG_EVENT(BG_EVENT_SILENT), 100, FLAG_MAPSEC_TEMP_50, FIND_PSYNERGY_STONE},
    {MAP_EVENTS_END},
};
