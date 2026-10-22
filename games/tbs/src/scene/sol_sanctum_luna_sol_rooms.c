/// @file
/// Sol Sanctum (SceneId_SolSanctumLunaSolRooms), the rooms of Sol and Luna at
/// the heart of the sanctum, reached from SceneId_SolSanctumReliefs
/// (sol_sanctum_entrance.c) by
/// SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS. Kraden has led Isaac,
/// Garet and Jenna this far looking for the hidden passage Saturos and Menardi
/// spoke of; here he finds what he came for, and from here the floor opens on
/// the way up to the Elemental Star Room. Its scenes follow the story:
///
/// - Arriving (cell code 11, Event_KradenFindsHeartOfSanctum), Kraden runs to
///   the symbol on the floor: `This is it! We've found it!`, the mark of the
///   sun, the heart of Sol Sanctum. The strangers told the truth, so they must
///   be thieves, and Jenna wants to hurry back and tell the village; but
///   Kraden, so close now, asks to go a little farther, and the others give in
///   (FLAG_SOL_SANCTUM_FOUND). Until the way up opens, Retreat is blocked, and
///   heading back out (cell code 9) brings Kraden over: `That way leads out of
///   the sanctum.`
/// - Crossing into the Luna room the first time, Kraden finds `a picture of
///   Luna`, wonders how a room for Luna and one for Sol are connected, and asks
///   Isaac to look farther up the passage
///   (FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN; agreeing sets
///   FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN, which changes what he says
///   when spoken to). He waits by the picture from then on.
/// - The statue puzzle of SceneId_SolSanctumStatue (sol_sanctum_statue.c),
///   which opens off these rooms by
///   SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE, lights one quarter of
///   each of the two pictures here
///   for each statue pushed into its place
///   (FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN to _SOUTHEAST_SEAL_OPEN), and cuts
///   back here to Kraden twice. Before its trap is disarmed, the first statue
///   in place springs it
///   (SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG,
///   SolSanctumLunaSolRooms_SpringTrap): the
///   quarter's light flashes, lights fall through the Luna room, Kraden cries
///   `Stop!!! You're about to make a big mistake!`, and every quarter goes dark
///   again (FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN). Once the centre statue has
///   dropped into the floor, the next statue in place only flashes its light,
///   and Kraden runs off to see what Isaac has found
///   (SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED,
///   FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING). Each scene ends by taking
///   the party back to SceneId_SolSanctumStatue.
/// - With all four quarters lit, walking up to Kraden (cell code 8) has him see
///   that the picture of Luna has changed to Sol
///   (FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED), and crossing cell code 13 has him
///   see that the floor's symbol has changed from Sol to Luna
///   (FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN). Searching the symbol (cell code 20)
///   then opens the floor (FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN), and Isaac,
///   then Kraden, rise through it to the Elemental Star Room
///   (SceneId_SolSanctumElementalStarRoom,
///   SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM).
/// - After the eruption (FLAG_SOL_SANCTUM_ERUPTED), the Wise One, the floating
///   rock that held back the volcano (sol_sanctum_eruption.c), sends Isaac and
///   Garet out of the collapsing Elemental Star Room to these rooms in a white
///   flash (SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION). Garet asks
///   `Did that floating eye thing save us?`, the rooms shake, and the two set
///   off for the way out (FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN). From then on
///   the rooms rumble.
///
/// The rooms' colours follow the party. Crossing cell code 13 or 14 fades them
/// to a tint or back to plain colours: until all four quarters are lit, code 14
/// tints and 13 restores, and afterwards the other way round. Code 12 fades to
/// a second tint, 15 back to plain, and 19 blacks out the backdrop. One of
/// three scene flags remembers which colours show, so a return after a battle
/// keeps them. On entry SolSanctumLunaSolRooms_Initialize copies each lit
/// quarter into its FLAG_SOL_SANCTUM_*_LIGHT_SHOWN and redraws the pictures,
/// the floor and the open way up from the story flags, unless an entrance's own
/// scene draws them. The falling lights are four callbacks, each with a random
/// timer of its own, that the trap adds and removes. The scene of
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN
/// (SolSanctumLunaSolRooms_AnimateSavedStatueLights) flashes the quarters lit
/// since and has Kraden urge Isaac on, but SceneId_SolSanctumStatue leads to it
/// only from a branch that cannot run.
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS leads back to
/// SceneId_SolSanctumReliefs and
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM up to
/// SceneId_SolSanctumElementalStarRoom;
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE leads into
/// SceneId_SolSanctumStatue, and the scenes take the exits named for them back
/// to it.

#include "game_state.h"
#include "unit/scene/sol_sanctum_luna_sol_rooms.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "sol_sanctum_luna_sol_rooms.h"
#include "call.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "types.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "iowrite.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "heap_state.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN 0x309

// Scene flags, cleared on every ordinary entrance; one of the three is set at
// a time, for the colours the rooms show:
// - FLAG_SCENE_TEMP_00: the plain colours (PALETTE_TRANSFORM_PLAIN);
// - FLAG_SCENE_TEMP_01: the first tint (colour target 0x2051cc), from cell
//   codes 13 and 14;
// - FLAG_SCENE_TEMP_02: the second tint (0x202db1), from cell code 12.

static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void);
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void);
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void);
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames);
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void);

void InitializeSceneRecordBuffer(void);

const struct SceneEntrance *SolSanctumLunaSolRooms_GetEntrances(void) {
  return gSolSanctumLunaSolRoomsEntrances;
}

const struct SceneRegion *SolSanctumLunaSolRooms_GetRegions(void) { return 0; }

const u32 *SolSanctumLunaSolRooms_GetExits(void) {
  return gSolSanctumLunaSolRoomsExits;
}

/// Returns the eight statues, four in each room, Kraden, Jenna and Garet, all
/// always placed.
const struct ScenePlacement *SolSanctumLunaSolRooms_GetPlacements(void) {
  return gSolSanctumLunaSolRoomsPlacements;
}

/// Returns the floor's cell codes that tint the rooms, Kraden's talk and
/// cutscenes, the floor symbol and the Herb's chest.
const struct SceneEvent *SolSanctumLunaSolRooms_GetEvents(void) {
  return gSolSanctumLunaSolRoomsEvents;
}

/// Sol Sanctum: crossing cell code 13. Until all four quarters are lit the
/// rooms fade back to plain colours; afterwards to the first tint, and Kraden
/// sees the floor symbol changed the first time (and the Luna room, had he not
/// yet). Also called by Event_KradenFindsHeartOfSanctum after the eruption.
/// Sets one of FLAG_SCENE_TEMP_00 and _01 and clears the other two tint flags;
/// Kraden's scenes set FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN and
/// FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN.
static void Event_UpdateRoomTint(void) {
  s32 paletteActive;

  // Test all four quarters' flags without skipping any flag reads.
  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = GameFlag_IsSet(FLAG_SCENE_TEMP_01);
    if (paletteActive == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
        if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
          SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna();
        }
      }
      Engine_EventEnd();
    }
  } else {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      Engine_EventEnd();
    }
  }
}

/// Sol Sanctum: crossing cell code 15 fades the rooms back to plain colours.
/// Also called by Event_KradenStopsPartyLeaving after the eruption. Sets
/// FLAG_SCENE_TEMP_00 and clears _01 and _02.
static void Event_RestorePlainTint(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    Engine_EventBegin();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
    Engine_ColorBufferInterpolate(20);
    GameFlag_Set(FLAG_SCENE_TEMP_00);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    GameFlag_Clear(FLAG_SCENE_TEMP_02);
    Engine_EventEnd();
  }
}

/// Sol Sanctum: crossing cell code 14, as code 13 the other way round. Until
/// all four quarters are lit the rooms fade to the first tint, and the first
/// time Kraden sees the Luna room; afterwards back to plain colours. Sets one
/// of FLAG_SCENE_TEMP_00 and _01 and clears the other two.
static void Event_UpdateRoomTintInverse(void) {
  s32 paletteActive;

  if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
    paletteActive = GameFlag_IsSet(FLAG_SCENE_TEMP_00);
    if (paletteActive == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      Engine_EventEnd();
    }
  } else {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
      Engine_EventBegin();
      ColorBuffer_ApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(20);
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_00);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
      if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
        SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms();
      }
      Engine_EventEnd();
    }
  }
}

/// Sol Sanctum: crossing cell code 12 fades the rooms to the second tint. Sets
/// FLAG_SCENE_TEMP_02 and clears _00 and _01.
static void Event_ApplySecondRoomTint(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) == 0) {
    ColorBuffer_ApplyTarget(0x202db1, 1);
    Engine_ColorBufferInterpolate(20);
    GameFlag_Set(FLAG_SCENE_TEMP_02);
    GameFlag_Clear(FLAG_SCENE_TEMP_00);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
  }
}

/// Sol Sanctum: crossing cell code 19 turns the backdrop black.
static void Event_ClearBackdropColor(void) {
  s32 black = 0;
  u16 *backdropColor = (u16 *)&PLTT_BACKDROP;
  *backdropColor = black;
}

/// Draws the light of each lit quarter of the Luna room's picture, from four
/// consecutive flags, with the tiles of the given source row.
static inline void SolSanctumLunaSolRooms_DrawStatueLightTiles(s32 firstFlag,
                                                               s32 sourceRow) {
  if (Engine_GameFlagIsSet(firstFlag) != 0)
    Map_CopyCellsTo(45, sourceRow, 34, 10, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 1) != 0)
    Map_CopyCellsTo(47, sourceRow, 36, 10, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 2) != 0)
    Map_CopyCellsTo(45, sourceRow + 1, 34, 11, 2, 1);
  if (Engine_GameFlagIsSet(firstFlag + 3) != 0)
    Map_CopyCellsTo(47, sourceRow + 1, 36, 11, 2, 1);
}

/// Draws the lights of the quarters lit now (the _SEAL_OPEN flags).
static void SolSanctumLunaSolRooms_DrawCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 28);
}

/// Draws the lights the room last showed (the _LIGHT_SHOWN flags).
static void SolSanctumLunaSolRooms_DrawSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 28);
}

/// Draws the current lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, 30);
}

/// Draws the saved lights using their alternate tile rows.
static void SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights(void) {
  SolSanctumLunaSolRooms_DrawStatueLightTiles(
      FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, 30);
}

/// Flashes both tile sets of the lit quarters' lights at this pace.
static inline void SolSanctumLunaSolRooms_FlashCurrentStatueLights(s32 frames) {
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Engine_EventWait(frames);
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawAlternateCurrentStatueLights();
  Engine_EventWait(frames);
}

/// Flashes both tile sets of the lights last shown at this pace.
static inline void SolSanctumLunaSolRooms_FlashSavedStatueLights(s32 frames) {
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Engine_EventWait(frames);
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawAlternateSavedStatueLights();
  Engine_EventWait(frames);
}

/// Sol Sanctum: stepping on cell code 11 the first time. Kraden runs to the
/// symbol on the floor, the mark of the sun: "This is it! We've found it!" The
/// strangers told the truth and must be thieves, but Kraden talks the others
/// into going a little farther. Plays until FLAG_SOL_SANCTUM_FOUND is set,
/// which it sets with FLAG_RETREAT_BLOCKED; after the eruption the crossing
/// tints the rooms as cell code 13 does.
static void Event_KradenFindsHeartOfSanctum(void) {
  struct FieldActor *leader;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Event_UpdateRoomTint();
  }
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FOUND) == 0) {
    Engine_EventBegin();
    // The leader walks in as the music changes.
    Engine_EventSetMessage(MsgSolSanctumFound);
    Engine_AudioPlayCue(MUS_017);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_AudioPlayCue(MUS_021);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    // Kraden hurries out of the leader to the symbol on the floor.
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(7) / 5, PIXELS(7) / 10);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 206);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_ActorJump(ACTOR_KRADEN, 4, 60);
    // Kraden: "This is it! We've found it!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    // Garet and Jenna step out behind the leader.
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetPosition(JENNA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkTo(GARET, 280, 248);
    Actor_WalkToAndWait(JENNA, 296, 248);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(288), -1, PIXELS(213), 1);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2) / 5, PIXELS(1) / 5);
    Engine_ActorWalkToAndWait(ACTOR_KRADEN, 288, 176);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    // Kraden: the mark of the sun, the heart of Sol Sanctum; so those two told
    // the truth.
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 60);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(JENNA, 2);
    Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 10);
    // Jenna: then they must have been... Garet: "They were thieves!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(JENNA, 10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 10);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 6);
    Actor_SetAttachedEffect(JENNA, 0x102);
    Engine_EventWait(40);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
    // Jenna: "Let's hurry back to the village and tell everybody!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA,
                                              10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 20);
    Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 40);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // Garet: "Kraden? What's the matter?" Kraden: "We're so close..." The
    // three wonder at turning back from the heart of the sanctum.
    Engine_EventShowMessage(GARET, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
    Engine_EventWait(40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 20);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    SolSanctumLunaSolRooms_ShowMessageAndWait(JENNA, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 60);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 3);
    Engine_EventWait(6);
    // Kraden: "Shall we go a little farther?" Yes: "Thank you!" No: then go
    // back without him.
    Engine_EventOpenMessage(ACTOR_KRADEN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventSetMessage(MsgSolSanctumThank);
    } else {
      Engine_EventSetMessage(MsgSolSanctumGoBackVillage);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    }
    Engine_ActorJump(ACTOR_KRADEN, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    // Either way Jenna gives in and Garet sides with Kraden, who is
    // delighted: "Good! It's decided. Let's move ahead!"
    Engine_EventSetMessage(MsgSolSanctumPutWayDont);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | JENNA,
                                              6);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 20);
    Engine_ActorJump(ACTOR_KRADEN, 6, 20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 30);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    // End the discussion and return the followers to Isaac.
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
    Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_KRADEN);
    Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetAnimation(JENNA, ANIM_WALK);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(JENNA);
    Engine_ActorSetPosition(JENNA, 0, 0);
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_FOUND);
    Engine_EventEnd();
  }
}

/// Sol Sanctum, the first time the party crosses into the Luna room: Kraden
/// steps out beside Isaac, finds the picture of Luna, wonders how the rooms of
/// Luna and Sol are connected, and asks Isaac to look farther up the passage.
/// Called by Event_UpdateRoomTint and Event_UpdateRoomTintInverse until
/// FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN is set, which it sets; Isaac agreeing
/// sets FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN. Leaves Kraden standing by
/// the picture on the northern wall.
static void SolSanctumLunaSolRooms_IntroduceSolAndLunaRooms(void) {
  struct FieldActor *leader;
  s32 request;

  // Kraden steps out beside Isaac: "What is this room?"
  Engine_EventSetMessage(MsgSolSanctumWhRoom);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 488, 176);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 1);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 472, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(575), -1, PIXELS(181), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(120);
  SolSanctumLunaSolRooms_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 80);
  Camera_MoveTo(PIXELS(492), -1, PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  // Kraden: "These rooms must be connected somehow, don't you think?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumLunaSolRooms);
  } else {
    Engine_EventSetMessage(MsgSolSanctumRoomLunaOne);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            10);
  // Kraden: would Isaac look farther up the passage? Yes: "Let me know what
  // you find." No: "Then just wait there."
  request = MsgSolSanctumMeanLookFarther;
  Engine_EventSetMessage(request);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 4);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage((request + 1));
    GameFlag_Set(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN);
  } else {
    Engine_EventSetMessage((request + 2));
  }
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN,
                                            4);
  Engine_CameraFollowActor(ACTOR_KRADEN, 1);
  Actor_WalkToAndWait(ACTOR_KRADEN, 486, 131);
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 120);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 2);
  // Leave Kraden by the northern wall for the search interactions.
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
}

/// Sol Sanctum: heading back out (cell code 9) once Kraden has found the heart
/// of the sanctum and until the way up is open: he hurries over. "That way
/// leads out of the sanctum." After the eruption the rooms only fade back to
/// plain colours (Event_RestorePlainTint).
static void Event_KradenStopsPartyLeaving(void) {
  struct FieldActor *kraden;
  struct FieldActor *leader;
  struct FieldActor *destinationActor;

  kraden = Object_GetById(ACTOR_KRADEN);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FOUND) == 0) {
    return;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Event_RestorePlainTint();
    return;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    return;
  }
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgSolSanctumWayLeadsOutSanctum);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
    }
    Engine_EventWait(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
             kraden->x.fixed > PIXELS(340)) {
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(392), PIXELS(168));
    Engine_EventWait(4);
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      kraden->x.fixed > PIXELS(340)) {
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  } else {
    // The result is unused.
    GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
  }
  Actor_WalkToAndWait(ACTOR_KRADEN, 288, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
      GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
    Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
    destinationActor = Object_GetById(ACTOR_PARTY_LEADER);
    if (destinationActor != 0) {
      Actor_SetDestination(ACTOR_KRADEN, destinationActor->x.part.pixel,
                           destinationActor->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
  } else {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 248);
  }
  Engine_EventEnd();
}

/// Animates the first light falling through the Luna room and manages the
/// shared shake pulse. Run each frame as a callback that
/// SolSanctumLunaSolRooms_SpringTrap adds and removes.
static void SolSanctumLunaSolRooms_UpdateFirstFallingLight(void) {
  if ((Random_Next() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light1Timer) {
    case 0:
      Event_PlayCue(SE_187);
      Map_CopyCellsTo(46, 59, 30, 33, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(46, 59, 30, 33, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 34, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(46, 59, 30, 34, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 35, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(46, 59, 30, 35, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(46, 59, 30, 36, 1, 1);
      Map_CopyCellsTo(46, 59, 30, 37, 1, 5);
      break;
    case 0x50:
      Map_CopyCellsTo(46, 49, 30, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light1Timer++;
    if (gSolSanctumLunaSolRooms_Light1Timer >
        ((u32)(Random_Next() * 40) >> 16) + 90) {
      gSolSanctumLunaSolRooms_Light1Timer = 0;
    }
  }
  if (gSolSanctumLunaSolRooms_FlashState != 0) {
    if (gSolSanctumLunaSolRooms_FlashState == 2) {
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    } else if (gSolSanctumLunaSolRooms_FlashState == 1) {
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
    gSolSanctumLunaSolRooms_FlashState--;
  }
}

/// Animates the second falling light with its own random restart delay, as
/// the first.
static void SolSanctumLunaSolRooms_UpdateSecondFallingLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light2Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(47, 59, 42, 33, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(47, 59, 42, 33, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 34, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(47, 59, 42, 34, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 35, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(47, 59, 42, 35, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 36, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(47, 59, 42, 36, 1, 1);
      Map_CopyCellsTo(47, 59, 42, 37, 1, 5);
      break;
    case 0x5a:
      Map_CopyCellsTo(47, 49, 42, 33, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light2Timer++;
    if (gSolSanctumLunaSolRooms_Light2Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 100) {
      gSolSanctumLunaSolRooms_Light2Timer = 0;
    }
  }
}

/// Animates the third falling light with its own random restart delay, as the
/// first.
static void SolSanctumLunaSolRooms_UpdateThirdFallingLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light3Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(48, 59, 31, 36, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(48, 59, 31, 36, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 37, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(48, 59, 31, 37, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 38, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(48, 59, 31, 38, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(48, 59, 31, 39, 1, 1);
      Map_CopyCellsTo(48, 59, 31, 40, 1, 5);
      break;
    case 0x5f:
      Map_CopyCellsTo(48, 49, 31, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light3Timer++;
    if (gSolSanctumLunaSolRooms_Light3Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 105) {
      gSolSanctumLunaSolRooms_Light3Timer = 0;
    }
  }
}

/// Animates the fourth falling light with its own random restart delay, as
/// the first.
static void SolSanctumLunaSolRooms_UpdateFourthFallingLight(void) {
  if ((Engine_RandomNext() & 3) != 0) {
    switch (gSolSanctumLunaSolRooms_Light4Timer) {
    case 0:
      Engine_AudioPlayCue(SE_187);
      Map_CopyCellsTo(46, 59, 41, 36, 1, 5);
      break;
    case 1:
      Map_CopyCellsTo(46, 59, 41, 36, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 37, 1, 5);
      break;
    case 2:
      Map_CopyCellsTo(46, 59, 41, 37, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 38, 1, 5);
      break;
    case 3:
      Map_CopyCellsTo(46, 59, 41, 38, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 39, 1, 5);
      break;
    case 4:
      gSolSanctumLunaSolRooms_FlashState = 2;
      Map_CopyCellsTo(46, 59, 41, 39, 1, 1);
      Map_CopyCellsTo(46, 59, 41, 40, 1, 5);
      break;
    case 0x55:
      Map_CopyCellsTo(46, 49, 41, 36, 1, 10);
      break;
    }
    gSolSanctumLunaSolRooms_Light4Timer++;
    if (gSolSanctumLunaSolRooms_Light4Timer >
        ((u32)(Engine_RandomNext() * 40) >> 16) + 95) {
      gSolSanctumLunaSolRooms_Light4Timer = 0;
    }
  }
}

/// Opens on Kraden in the Luna room with its first tint, as a flash crosses it.
/// Shared by the three scenes that play on arriving from
/// SceneId_SolSanctumStatue's statue puzzle. Sets FLAG_SCENE_TEMP_01 and clears
/// _00 and _02.
static void SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom(void) {
  Camera_MoveTo(-1, -1, -1, 0);
  Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
  Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
  Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
  Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(158), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(574), PIXELS(120));
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(20);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
  GameFlag_Clear(FLAG_SCENE_TEMP_02);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Event_PlayCue(SE_171);
  ColorBuffer_ApplyTarget(0x10005, 1);
  Engine_ColorBufferInterpolate(8);
  Engine_EventWait(32);
  Engine_ColorBufferApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(24);
}

/// Sol Sanctum, the first statue placed in SceneId_SolSanctumStatue after the
/// centre statue has dropped into the floor, disarming the trap: the quarter's
/// light flashes ever faster around Kraden in the Luna room. "What's
/// happening?" "Isaac must have found something!" He runs off to see. Called by
/// SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED until
/// FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING is set, which it sets; the flag
/// marks this scene, not the trap. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED back to
/// SceneId_SolSanctumStatue.
static void SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed(void) {
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Engine_EventBegin();
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  // Kraden turns, startled: "What's happening?"
  Engine_EventSetMessage(MsgSolSanctumWhatsHappeningAtTheTrap);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  // Shorten each flash interval as the flashing accelerates.
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  SolSanctumLunaSolRooms_DrawCurrentStatueLights();
  Engine_EventWait(6);
  // Kraden: "Isaac must have found something!" He runs off south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
  Engine_EventRequestExit(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED);
}

/// Sol Sanctum, the first statue placed in SceneId_SolSanctumStatue before the
/// trap is disarmed: the quarter's light flashes and lights fall through the
/// Luna room while Kraden cries "Stop!!! You're about to make a big mistake!"
/// Called by SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG until
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set; the caller then sets it and
/// darkens all four quarters. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_SPRUNG back to
/// SceneId_SolSanctumStatue, where Kraden explains the trap.
static void SolSanctumLunaSolRooms_SpringTrap(void) {
  s32 flash;
  Engine_EventBegin();
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  gSolSanctumLunaSolRooms_Light1Timer = 0;
  gSolSanctumLunaSolRooms_Light2Timer = 0;
  gSolSanctumLunaSolRooms_Light3Timer = 0;
  gSolSanctumLunaSolRooms_Light4Timer = 0;
  // Kraden turns, startled: "What's happening?" The lights flash four times.
  Engine_EventSetMessage(MsgSolSanctumWhatsHappening);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            20);
  for (flash = 0; flash != 4; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(12);
  }
  // Kraden: "Isaac must have found something!"
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  // Each light starts falling after 20 to 79 frames.
  gSolSanctumLunaSolRooms_Light1Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light2Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light3Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_Light4Timer =
      (((u32)Engine_RandomNext() * 60) >> 16) + 20;
  gSolSanctumLunaSolRooms_FlashState = 0;
  // Run the falling lights independently while the quarters keep flashing.
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateFirstFallingLight,
                   TASK_PRIORITY_SCENE);
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateSecondFallingLight,
                   TASK_PRIORITY_SCENE);
  Task_AddCallback(SolSanctumLunaSolRooms_UpdateThirdFallingLight,
                   TASK_PRIORITY_SCENE);
  FieldScheduler_AddCallback(SolSanctumLunaSolRooms_UpdateFourthFallingLight,
                             TASK_PRIORITY_SCENE);
  for (flash = 0; flash != 6; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(5);
  }
  for (flash = 0; flash != 8; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(4);
  }
  for (flash = 0; flash != 10; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(3);
  }
  for (flash = 0; flash != 12; flash++) {
    SolSanctumLunaSolRooms_FlashCurrentStatueLights(2);
  }
  Map_CopyCellsTo(45, 30, 34, 10, 4, 2);
  Engine_ActorJump(ACTOR_KRADEN, 6, 40);
  // Kraden: "Stop!!! You're about to make a big mistake!" He runs south.
  SolSanctumLunaSolRooms_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                                            6);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_KRADEN, 576, 280);
  // Stop all light callbacks before leaving this room.
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateFirstFallingLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateSecondFallingLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateThirdFallingLight);
  Engine_TaskRemoveCallback(SolSanctumLunaSolRooms_UpdateFourthFallingLight);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_SPRUNG);
}

/// Sol Sanctum: the quarters lit since the room last showed them flash ever
/// faster in the Luna room, and Kraden, once: "You found it, Isaac." Called by
/// SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN once
/// FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN is set; SceneId_SolSanctumStatue leads
/// there only from a branch that cannot run
/// (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN). Toggles
/// each lit quarter's FLAG_SOL_SANCTUM_*_LIGHT_SHOWN; Kraden speaks until the
/// caller sets FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT. Takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN back to
/// SceneId_SolSanctumStatue.
static void SolSanctumLunaSolRooms_AnimateSavedStatueLights(void) {
  s32 speaker;
  s32 twelveFrameFlashes;
  s32 eightFrameFlashes;
  s32 sixFrameFlashes;
  s32 fourFrameFlashes;
  s32 twoFrameFlashes;

  Engine_EventBegin();
  // Toggle the shown lights only where the quarters are lit now.
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
    Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
    Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
    Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) != 0) {
    GameFlag_Clear(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
    Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
  } else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
             GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) == 0) {
    GameFlag_Set(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  SolSanctumLunaSolRooms_OpenOnKradenInLunaRoom();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_KRADEN, 6, 30);
  Camera_MoveTo(PIXELS(574), -1, PIXELS(174), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  for (twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(12);
  }
  for (eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(8);
  }
  for (sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(6);
  }
  for (fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(4);
  }
  for (twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
    SolSanctumLunaSolRooms_FlashSavedStatueLights(2);
  }
  Event_PlayCue(SE_246);
  SolSanctumLunaSolRooms_DrawSavedStatueLights();
  Engine_EventWait(6);
  // Kraden, once: "You found it, Isaac." and "Keep doing that until the light
  // bathes all the remaining quarters."
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT) == 0) {
    speaker = SPEAKER_WINDOW_BELOW | ACTOR_KRADEN;
    Engine_EventSetMessage(MsgSolSanctumYouFoundIt);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    SolSanctumLunaSolRooms_ShowMessageAndWait(speaker, 6);
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(
      SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN);
}

/// Sol Sanctum: walking up to Kraden in the Luna room (cell code 8) once all
/// four quarters are lit. The picture of Luna has changed to Sol: "Look,
/// Isaac! The picture of Luna changed to Sol!" But nothing else has, and he
/// sends Isaac to look for what must have changed. Until
/// FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED is set, which it sets.
static void Event_KradenSeesLunaPictureChange(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0) {
  } else {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() == 0) {
    } else {
      Engine_EventBegin();
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(577), PIXELS(147));
      Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 1);
      Camera_MoveTo(PIXELS(574), -1, PIXELS(184), 1);
      // Isaac walks up to Kraden, who hops: "The picture of Luna changed to
      // Sol!"
      Engine_EventSetMessage(MsgSolSanctumThePictureOfLunaChanged);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 576, 232);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
      Engine_CameraWaitForMove();
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 152);
      Engine_EventWait(6);
      Engine_ActorJump(ACTOR_KRADEN, 6, 30);
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_EventWait(2);
      Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
      // Kraden: "But other than that, nothing looks different..."
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
      Engine_EventWait(30);
      // Kraden: a trap here, and this is all that happens?
      SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 184);
      Engine_EventWait(6);
      Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
      Engine_EventWait(40);
      // Kraden: "Somewhere, something else must have changed." He walks back
      // into the leader.
      SolSanctumLunaSolRooms_ShowMessageAndWait(
          SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
      Actor_WalkToAndWait(ACTOR_KRADEN, 576, 208);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_EventWait(6);
      Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1) / 2, PIXELS(1) / 4);
      Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      if (leader != 0) {
        Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
      Engine_ActorWaitForMove(ACTOR_KRADEN);
      Actor_SetPosition(ACTOR_KRADEN, 0, 0);
      GameFlag_Set(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
      Engine_EventEnd();
    }
  }
}

/// Sol Sanctum: Kraden notices that the symbol on the floor has changed from
/// Sol to Luna. Called by Event_UpdateRoomTint once all four quarters are lit,
/// until FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN is set, which it sets. Leaves
/// Kraden off the map.
static void SolSanctumLunaSolRooms_DiscussFloorSymbolChangingToLuna(void) {
  Engine_AudioPlayCue(MUS_021);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 184);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(376), PIXELS(184));
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 392, 168);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventSetMessage(MsgSolSanctumLookSymbolFloor);
  Engine_ActorJump(ACTOR_KRADEN, 4, 30);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(6);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 376, 184);
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(1608), PIXELS(1608));
  Engine_EventWait(4);
  Engine_GameFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
}

/// Sol Sanctum: searching the floor symbol (cell code 20) once Kraden has seen
/// it change. The floor opens, and Isaac, then Kraden ("Wait for me, Isaac...
/// I'm on my way!"), rise through it. Does nothing until
/// FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN is set. Sets
/// FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN the first time and takes
/// SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, to the
/// Elemental Star Room (SceneId_SolSanctumElementalStarRoom).
static void Event_RiseToElementalStarRoom(void) {
  struct FieldActor *actor;
  s32 frame;
  struct AnimationObject *sprite;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
  } else {
    Engine_EventBegin();
    // Isaac steps onto the symbol and Kraden comes out beside him.
    Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
    Camera_MoveTo(PIXELS(287), -1, PIXELS(148), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 30);
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(288), PIXELS(120));
    Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_KRADEN, 276, 136);
    Actor_WalkTo(ACTOR_KRADEN, 264, 136);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 136);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
      Engine_AudioPlayCue(SE_220);
    }
    Engine_EventWait(40);
    // Reveal the passage only once; later visits reuse its open state.
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
    } else {
      Map_CopyCellsTo(36, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(44, 59, 17, 38, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(38, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(44, 59, 17, 39, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(40, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(0, 32, 17, 39, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 40, 2, 1);
      Engine_EventWait(10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      0);
      Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      Map_CopyCellsTo(42, 62, 17, 36, 2, 3);
      Map_CopyCellsTo(0, 32, 17, 40, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 41, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(0, 32, 17, 41, 2, 1);
      Map_CopyCellsTo(44, 59, 17, 42, 2, 1);
      Engine_EventWait(10);
      Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
      Engine_EventWait(80);
      Map_ClearLayerEntryFlag(9);
      Map_ClearLayerEntryFlag(10);
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
    }
    Object_LinkPair(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    // Kraden: "Ah ha! This has to be it." and "Let's go, Isaac!"
    Engine_EventSetMessage(MsgSolSanctumHa);
    SolSanctumLunaSolRooms_ShowMessageAndWait(
        SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
    Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
    Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
    Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(60);
    // Raise Isaac first, then shrink his sprite into the passage.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(287), -1, PIXELS(114), 1);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    /* FAKEMATCH(temporary): the zero goes through sprite, whose register is
     * r7; as motionFlags = 0, 2 of 442 instructions differ in tbs-en. */
    sprite = 0;
    actor->motionFlags = (u32)sprite;
    Engine_AudioPlayCue(SE_201);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0x100);
    sprite = actor->sprite;
    frame = 0;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 120);
    Engine_AudioPlayCue(SE_190);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 60);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorJump(ACTOR_KRADEN, 4, 20);
    // Kraden: "Wait for me, Isaac... I'm on my way!"
    SolSanctumLunaSolRooms_ShowMessageAndWait(ACTOR_KRADEN, 6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 288, 120);
    Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
    // Repeat the ascent for Kraden before closing the room transition.
    Engine_ActorSetSpeed(ACTOR_KRADEN, PIXELS(3) / 10, PIXELS(3) / 20);
    actor = Object_GetById(ACTOR_KRADEN);
    /* FAKEMATCH(temporary): the mask in a variable, so both byte stores wait
     * past the zero; as &= ~ACTOR_FACES_MOTION, 5 of 442 instructions differ
     * in tbs-en. */
    {
      s32 steeringFlags = 254;

      steeringFlags &= actor->steeringFlags;
      frame = 0;
      actor->steeringFlags = steeringFlags;
      actor->motionFlags = frame;
    }
    Engine_AudioPlayCue(SE_201);
    Actor_SetChildValue(ACTOR_KRADEN, 0x100);
    sprite = actor->sprite;
    sprite->flags = 0;
    do {
      actor->y.fixed += PIXELS(1) / 5;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 120);
    Engine_AudioPlayCue(SE_190);
    frame = 0;
    do {
      actor->y.fixed += PIXELS(1) / 10;
      sprite->scale += -Q16_ONE / 64;
      Engine_TaskWait(1);
      frame++;
    } while (frame != 60);
    Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    Engine_EventWait(80);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    gEventWork->transitionFrames = 24;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ColorBufferApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    Engine_EventRequestExit(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM);
    Engine_EventEnd();
  }
}

/// Sets up Sol Sanctum's rooms of Sol and Luna and plays the scene this
/// entrance calls for. Called by Field_RunScene once the actors exist. Blocks
/// Retreat (FLAG_RETREAT_BLOCKED) from Kraden's find until the way up opens,
/// before the eruption; sets FLAG_SCENE_TEMP_00 on an ordinary entrance; copies
/// each lit quarter into its FLAG_SOL_SANCTUM_*_LIGHT_SHOWN; plays the disarmed
/// trap's flash at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED, the trap
/// at SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG (then
/// darkening the four quarters), the newly lit quarters at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN, and the
/// arrival after the Wise One's rescue at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION until
/// FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN is set. Otherwise it redraws the
/// quarters, pictures, floor and open way up from the story flags, and after
/// the eruption the rooms rumble.
s32 SolSanctumLunaSolRooms_Initialize(void) {
  s32 playedEntrySequence;
  s32 anyLightLit;

  Engine_TaskWait(1);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  playedEntrySequence = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FOUND) != 0 &&
      Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) == 0 &&
      Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  }
  Engine_ColorBufferApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
      Engine_ColorBufferApplyTarget(0x2051cc, 1);
      Engine_ColorBufferInterpolate(1);
      Engine_TaskWait(1);
    } else if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
      Engine_ColorBufferApplyTarget(0x202db1, 1);
      Engine_ColorBufferInterpolate(1);
      Engine_TaskWait(1);
    }
  } else {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) != 0) {
      Actor_SetPosition(ACTOR_KRADEN, PIXELS(576), PIXELS(120));
    }
  }
  // Play the one-time scene this entrance comes back from
  // SceneId_SolSanctumStatue for.
  if (gGameState.entrance ==
      SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
      SolSanctumLunaSolRooms_FlashLightsWithTrapDisarmed();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
      playedEntrySequence = 1;
    }
  } else if (gGameState.entrance ==
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG) {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
      SolSanctumLunaSolRooms_SpringTrap();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
      Engine_GameFlagClear(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
      playedEntrySequence = 1;
    }
  } else if (gGameState.entrance ==
             SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN) {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0) {
      SolSanctumLunaSolRooms_AnimateSavedStatueLights();
      Engine_GameFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT);
    }
    playedEntrySequence = 1;
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
  }
  Engine_TaskWait(4);
  // Restore completed puzzle tiles without replaying the entrance scene.
  if (playedEntrySequence == 0) {
    if (SolSanctumLunaSolRooms_AreAllStatueLightsLit() != 0) {
      Map_CopyCellsTo(30, 44, 30, 38, 12, 5);
      Map_CopyCellsTo(30, 44, 34, 37, 4, 1);
      Map_CopyCellsTo(14, 41, 32, 41, 8, 4);
      Map_CopyCellsTo(45, 28, 34, 10, 4, 2);
      Map_CopyCellsTo(45, 30, 16, 10, 4, 2);
      Map_CopyCellsTo(14, 45, 14, 41, 8, 4);
      if (gGameState.entrance !=
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
        if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) == 0) {
          if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) !=
              0) {
            Map_CopyCellsTo(0, 32, 17, 39, 2, 1);
            Map_CopyCellsTo(42, 62, 17, 36, 2, 3);
            Map_CopyCellsTo(0, 32, 17, 40, 2, 1);
            Map_CopyCellsTo(0, 32, 17, 41, 2, 1);
            Map_CopyCellsTo(0, 32, 17, 42, 2, 3);
          } else {
            Map_CopyCellsTo(44, 59, 17, 37, 2, 6);
          }
          Map_ClearLayerEntryFlag(9);
          Map_ClearLayerEntryFlag(10);
        }
      }
      Engine_ActorSetPosition(ACTOR_KRADEN, 0, 0);
    } else {
      anyLightLit = 0;
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(45, 28, 34, 10, 2, 1);
        Map_CopyCellsTo(45, 30, 16, 10, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(47, 28, 36, 10, 2, 1);
        Map_CopyCellsTo(47, 30, 18, 10, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(45, 29, 34, 11, 2, 1);
        Map_CopyCellsTo(45, 31, 16, 11, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
        Map_CopyCellsTo(47, 29, 36, 11, 2, 1);
        Map_CopyCellsTo(47, 31, 18, 11, 2, 1);
        anyLightLit = 1;
      }
      if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0 ||
          anyLightLit != 0) {
        Map_CopyCellsTo(30, 43, 32, 40, 8, 3);
        Map_CopyCellsTo(30, 43, 33, 39, 8, 1);
        Map_CopyCellsTo(30, 43, 36, 38, 3, 3);
        Map_CopyCellsTo(36, 58, 32, 41, 8, 4);
      }
      Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  // The arrival after the Wise One's rescue plays only once.
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN) == 0 &&
      gGameState.entrance ==
          SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
    SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue();
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN);
    Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
  } else {
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
      FieldFx_SetQueuedSoundAndPlay(SE_141);
      Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
      InitializeSceneRecordBuffer();
      Map_CopyCellAttributes(15, 6, 2, 1, 17, 6);
    }
  }
  return 0;
}

/// Returns 1 when all four quarters are lit.
static s32 SolSanctumLunaSolRooms_AreAllStatueLightsLit(void) {
  s32 allLit = 1;

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) == 0)
    allLit = 0;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) == 0)
    allLit = 0;

  return allLit;
}

/// Advances the active dialogue and pauses for the requested frames.
static void SolSanctumLunaSolRooms_ShowMessageAndWait(s32 speaker,
                                                      s32 waitFrames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(waitFrames);
}

/// Sol Sanctum after the eruption: the Wise One has sent Isaac and Garet out
/// of the collapsing Elemental Star Room, and they appear here in a white
/// flash, bewildered, as the rooms shake. Garet: "Did that floating eye thing
/// save us?" A stronger tremor throws them aside, and they set off: "Let's get
/// out of here..." Called by SolSanctumLunaSolRooms_Initialize at
/// SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION until
/// FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN is set; the caller sets it. Sets
/// FLAG_SCENE_TEMP_01 and clears _00 and _02.
static void SolSanctumLunaSolRooms_ArriveAfterWiseOnesRescue(void) {
  struct FieldActor *leader;
  struct ColorBuffer *palette;

  Engine_EventBegin();
  // The screen opens all white on Isaac and Garet, then fades to the tint.
  Camera_MoveTo(PIXELS(286), -1, PIXELS(134), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  ColorBuffer_ApplySource(0x7fff, 0);
  Engine_ColorBufferApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  Actor_SetPosition(GARET, PIXELS(280), PIXELS(134));
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  // This event reads its work slots from one address: HEAP_SLOT_PALETTE_BUFFER
  // is reached from HEAP_SLOT_EVENT's slot. As gEventWork and
  // gPaletteBufferWork they would load two.
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ColorBufferApplyTarget(0x2051cc, 1);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(100);
  // Both look around, bewildered, as the rooms give a jolt.
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_ActorSetAttachedEffect(GARET, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 60);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 20);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  // Garet: "Did that floating eye thing save us?" A stronger tremor throws
  // them both aside.
  Engine_EventSetMessage(MsgSolSanctumFloatingEyeThing);
  Engine_EventOpenMessage(1, 0);
  Engine_EventChooseYesNo(0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_107);
  Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Engine_EventWait(40);
  InitializeSceneRecordBuffer();
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 300, 130);
  Engine_ActorSetDestination(GARET, 266, 144);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(40);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  // Recover from the tremor and bring Garet back into the party.
  FieldFx_SetQueuedSoundAndPlay(SE_141);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  // Garet: "Yeah, right! Let's get out of here..."
  SolSanctumLunaSolRooms_ShowMessageAndWait(GARET, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetPosition(GARET, 0, 0);
  palette = WORK_SLOT(HEAP_SLOT_PALETTE_BUFFER);
  palette->target[0] = 0;
  palette->target[1] = 0;
  palette->target[2] = 0;
  palette->page = 0;
  palette->duration = 1;
  palette->step = 1;
  palette->padding2a03 = 1;
  Io_Write16(0, &PLTT_BACKDROP);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transitionFrames = 16;
  Engine_EventEnd();
}

/// Sol Sanctum: talking to Kraden in the Luna room. "Let me know what you find"
/// once Isaac has agreed to look farther up the passage, otherwise "Just wait
/// over there."
static void Event_TalkToKraden(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN) != 0) {
    Engine_EventSetMessage(MsgSolSanctumKradenLetMeKnowWhat);
  } else {
    Engine_EventSetMessage(MsgSolSanctumKradenJustWaitOverThere);
  }
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
  Engine_EventEnd();
}

struct SceneEntrance gSolSanctumLunaSolRoomsEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 288, 0, 157, FACING_SOUTH, 0, 0, 0,
     752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 55,
     0, 477, FACING_NORTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE, CONDITION_ALWAYS, 647,
     0, 327, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED, CONDITION_ALWAYS, 287, 0, 218,
     FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_SPRUNG,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED,
     CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
    {SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION, CONDITION_ALWAYS,
     297, 0, 119, FACING_NORTHWEST, 0, 0, 0, 752, 496, 0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumLunaSolRoomsExits[] = {
    SCENE_EXITS(SolSanctumLunaSolRooms),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS,
               SolSanctumReliefs,
               SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    SCENE_EXIT(
        SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED,
        SolSanctumStatue,
        SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_SPRUNG,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_SPRUNG),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_UNUSED,
               SolSanctumStatue,
               SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED),
    SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM,
               SolSanctumElementalStarRoom,
               SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
    SCENE_EXIT_TO_BATTLE(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_BATTLE, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSolSanctumLunaSolRoomsPlacements[] = {
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gSolSanctumLunaSolRoomsEvents[] = {
    {EVENT_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToKraden)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_UpdateRoomTintInverse)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UpdateRoomTint)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RestorePlainTint)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_ApplySecondRoomTint)},
    {EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearBackdropColor)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_RiseToElementalStarRoom)},
    {EVENT_TOUCH, 8, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_KradenSeesLunaPictureChange)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 2, CONDITION_ALWAYS,
     SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_KradenFindsHeartOfSanctum)},
    {EVENT_TOUCH, 9, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_KradenStopsPartyLeaving)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_SOL_SANCTUM_HERB_FOUND,
     FIND_ITEM(ITEM_HERB)},
    {SCENE_EVENTS_END},
};
