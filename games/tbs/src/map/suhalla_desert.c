/// @file
/// Suhalla Desert, the desert of sandstorms between Suhalla and Suhalla Gate,
/// which the party crosses with Iodem after Babi's audience while Sheba is
/// missing in it. Three maps share this overlay and the area name Suhalla
/// Desert:
///
/// - SuhallaDesertNorth (MapId_SuhallaDesertNorth): entered from the world
///   map near Suhalla, with five storms.
/// - SuhallaDesertSouth (MapId_SuhallaDesertSouth): four storms and a fifth
///   that gives chase, Flash, a Mars Djinni, a stone pillar that Reveal shows,
///   and near its way out Iodem's question whether to search on for Sheba
///   (EventScript_IodemAsksWhetherToPressOn,
///   FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA).
/// - SuhallaDesertCaves (MapId_SuhallaDesertCaves): two caves under the
///   desert, reached from SuhallaDesertSouth; they place nothing.
///
/// One warp table and, for the first two maps, one event table serve the
/// overlay; the warps in that event table keep their numbers, which each
/// map's warp rows lead its own way.
///
/// The storms are the desert's mechanism, LOCALID_FIRST_STORM to
/// LOCALID_LAST_STORM in the first two maps. As the party nears one (metatile
/// behaviors 61 to 65 and 71 to 73) it rises, growing to full size, and the
/// sand haze, BG1 blended over the rest (SUHALLA_DESERT_HAZE_BLEND), fades in
/// the first time (SuhallaDesert_RaiseStorm); the first storm to rise while
/// Iodem travels with the party brings him out to warn of them. Walking into
/// one (metatile behaviors 41 to 45 and 51 to 54) draws the leader to its
/// centre and lifts the party, spinning, for STORM_CAPTURE_FRAMES
/// (SuhallaDesert_CatchInStorm). Douse in that time stops the storm and the
/// monster in it attacks (EventScript_DouseStorm); otherwise the storm carries
/// the party up and drops it on the world map outside Suhalla
/// (EventScript_StormCarriesPartyAway), which is why everyone the storms catch
/// winds up there. In SuhallaDesertSouth, metatile behavior 81 sets the fifth
/// storm chasing the leader (SuhallaDesert_ChaseLeader), and it catches a
/// leader on the move.
///
/// A doused storm's flag is a map section flag, five from
/// FLAG_FIRST_MAP_STORM_DOUSED and five from FLAG_SECOND_MAP_STORM_DOUSED,
/// so the storm stays gone until the party leaves the desert's map section; the
/// chasing storm, once doused, is gone for good
/// (FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED). Its map flags:
///
/// - FLAG_MAP_TEMP_06 to FLAG_MAP_TEMP_0A: storms LOCALID_FIRST_STORM to
///   LOCALID_LAST_STORM have risen on this visit.
/// - FLAG_MAP_TEMP_0E: the chasing storm has set off.
/// - FLAG_MAP_TEMP_0F: a storm has caught the leader.
/// - FLAG_MAP_TEMP_10: a byte, the frames left before the storm carries the
///   party away.
/// - FLAG_MAP_TEMP_18: a byte, the storm that caught the leader.
///
/// Open questions:
/// - XXX (no evidence): what flag 0x1b7 holds, which
///   EventScript_IodemAsksWhetherToPressOn requires clear; no code in the game
///   sets or reads it elsewhere, so the test always passes.

#include "action_script.h"
#include "flags.h"
#include "version.h"
#include "game_state.h"
#include "engine_call.h"
#include "unit/map/suhalla_desert.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_work.h"
#include "heap_slots.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "map_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "songs.h"
#include "heap.h"
#include "map_grid.h"
#include "fixed_math.h"

/// Storms doused in the first and second maps, one flag per storm from the
/// first; each storm stays away once doused.
enum {
  /// The first map's storms, five from this one.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_FIRST_MAP_STORM_DOUSED = FLAG_MAPSEC_TEMP_01,
  /// The second map's storms, five from this one.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_SECOND_MAP_STORM_DOUSED = FLAG_MAPSEC_TEMP_11,
  /// A storm has risen: the sand haze stays faded in.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_STORMS_RISEN = FLAG_MAPSEC_TEMP_40,
  /// The Psynergy Stone has been found, a map section flag.
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_SUHALLA_DESERT_PSYNERGY_STONE = FLAG_MAPSEC_TEMP_51
};

/// The object events, numbering the object event template rows of the first two
/// maps from LOCALID_FIRST_PLACED; the third map places none.
enum {
  /// The storms, five in each map.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  LOCALID_FIRST_STORM = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  LOCALID_FOURTH_STORM = LOCALID_FIRST_STORM + 3,
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  LOCALID_LAST_STORM = LOCALID_FIRST_STORM + 4,
  /// In the second map the last storm gives chase.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  LOCALID_CHASING_STORM = LOCALID_LAST_STORM,
  /// Iodem, who steps out of the party to talk and then rejoins it.
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  LOCALID_IODEM,
  /// In the second map, seen only while Reveal lasts, drawn as Goma Cave's
  /// stone pillars.
  /// ★ Named from what the player sees: its sprite, SPRITE_STONE_PILLAR, a
  /// stone pillar (its render)
  LOCALID_REVEALED_PILLAR,
  /// In the second map, the Mars Djinni.
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  LOCALID_FLASH
};

/// Frames a caught party spins before the storm carries it away.
enum { STORM_CAPTURE_FRAMES = 180 };

/// The treasure spot of SuhallaDesertSouth's hidden Psynergy Stone, whose
/// search row finds the stone (FIND_PSYNERGY_STONE). Its object event stands in
/// its cell only while Reveal lasts (EventScript_RevealStarts moves it to the
/// cell's centre) and is moved off the map as Reveal ends.
enum { TREASURE_SPOT_PSYNERGY_STONE = 100 };

/// The events the storms raise: the spinning has run out, or the chasing storm
/// has reached the moving leader.
enum {
  RAISED_CHASING_STORM_CATCHES = 55,
  RAISED_STORM_CARRIES_PARTY_AWAY = 99
};

/// The sand haze: BG1 blended over every layer, its weight rising from 0 to
/// 16 as the first storm rises.
#define SUHALLA_DESERT_HAZE_BLEND                                              \
  (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

static void SuhallaDesert_SetUpFirstMapStorms(void);
static void SuhallaDesert_SetUpSecondMapStorms(void);

static u16 sSuhallaDesertSavedColor;

#define SUHALLA_REVEAL_PALETTE_COLOR (*(volatile u16 *)0x0500019e)

#include "common/effect_create.h"
#include "common/effect_spawn.h"

void Map_QueueMusic(s32 cue);

/// In SuhallaDesertSouth, talking to Flash, a Mars Djinni, starts its battle.
static void EventScript_FightDjinni(void) {
  Field_RequestDjinnBattle(LOCALID_FLASH, DJINNI_FLASH);
}

/// A storm's object event template script: clears its sprite and collision
/// flags.
static s32 SuhallaDesert_ClearObjectEventSpriteAndCollisionFlags(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent->collisionFlags = 0;
  return 0;
}

/// A storm's object event template script: toggles its single-sprite storage.
static s32
SuhallaDesert_ToggleObjectEventActive(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  objectEvent->spriteStorageKind ^= 1;
  return 1;
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 selector = gGameState.mapId;

  if (selector == (s32)&MapId_SuhallaDesertNorth) {
    return gSuhallaDesertNorthWarpDests;
  }
  if (selector == (s32)&MapId_SuhallaDesertSouth) {
    return gSuhallaDesertSouthWarpDests;
  }
  if (selector == (s32)&MapId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesWarpDests;
  }
  return gSuhallaDesertWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

/// Where each warp of the three maps leads.
const u32 *Map_GetWarps(void) { return gSuhallaDesertWarps; }

/// The storms, Iodem, and in the second map Flash and what Reveal shows.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 selector = gGameState.mapId;

  if (selector == (s32)&MapId_SuhallaDesertNorth) {
    return gSuhallaDesertNorthObjectEventTemplates;
  }
  if (selector == (s32)&MapId_SuhallaDesertSouth) {
    return gSuhallaDesertSouthObjectEventTemplates;
  }
  if (selector == (s32)&MapId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesObjectEventTemplates;
  }
  return gSuhallaDesertObjectEventTemplatesOther;
}

/// Each frame while caught: spins the leader, and counts down to raising event
/// 99, the storm carrying the party away.
static void SuhallaDesert_UpdateCaptureTimer(void) {
  struct ObjectEvent *selectedObjectEvent;
  struct FieldWork *requests;
  s32 remainingFrames;

  selectedObjectEvent = ObjectEvent_GetById(gGameState.leaderLocalId);
  requests = gFieldWork;
  selectedObjectEvent->facing = (gFrameCount << 12);

  remainingFrames = FieldGameFlag_GetByte(FLAG_MAP_TEMP_10);
  if (remainingFrames != 0) {
    if (remainingFrames == 1) {
      requests->raisedEvent = RAISED_STORM_CARRIES_PARTY_AWAY;
    } else if (GameFlag_Test(FLAG_MENU_OPEN) == 0) {
      remainingFrames -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_MAP_TEMP_10, remainingFrames);
}

/// The leader walks into a storm: drawn to its centre and lifted, the party
/// spins until Douse stops the storm or it carries them away.
static void SuhallaDesert_CatchInStorm(s32 stormId) {
  s32 stormIdCopy;
  s32 captureActive;
  struct ObjectEvent *selectedObjectEvent;
  struct ObjectEvent *storm;
  s32 leaderLocalId;

  // The event-selected object event is pulled toward the storm before being
  // lifted.
  leaderLocalId = gGameState.leaderLocalId;
  stormIdCopy = stormId;
  selectedObjectEvent = ObjectEvent_GetById(leaderLocalId);
  ObjectEvent_Get(stormIdCopy);
  captureActive = GameFlag_IsSet(FLAG_MAP_TEMP_0F);
  if (captureActive == 0) {
    FieldScript_Start();
    ObjectEvent_SetAttachedEffect(leaderLocalId, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ATTACH);
    Engine_ObjectEventSetAnimation(leaderLocalId, ANIM_LEADER_LOOK_AROUND);
    storm = ObjectEvent_GetById(stormIdCopy);
    if (storm != 0) {
      ObjectEvent_SetDestination(leaderLocalId, storm->x.part.pixel,
                                 storm->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(leaderLocalId);
    // Start the capture countdown once the approach finishes.
    Script_PlaySong(SE_SANDSTORM);
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_MAP);
    selectedObjectEvent->motionFlags = captureActive;
    Object_SetPosition(selectedObjectEvent, selectedObjectEvent->x.fixed,
                       selectedObjectEvent->y.fixed + PIXELS(32),
                       selectedObjectEvent->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(leaderLocalId);
    selectedObjectEvent->velocityY = captureActive;
    selectedObjectEvent->motionFlags = OBJ_EVENT_FLOATS;
    gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_MAP_TEMP_0F);
    FieldGameFlag_SetByte(FLAG_MAP_TEMP_18, stormIdCopy);
    FieldGameFlag_SetByte(FLAG_MAP_TEMP_10, STORM_CAPTURE_FRAMES);
    FieldScript_End();
    gFieldWork->battle = 0; // drop any battle this step rolled
  }
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the leader walks into the map's first storm
/// (metatile behavior 41 or 51).
static void EventScript_CaughtByFirstStorm(void) {
  SuhallaDesert_CatchInStorm(LOCALID_FIRST_STORM);
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the leader walks into the second storm (metatile
/// behavior 42 or 52).
static void EventScript_CaughtBySecondStorm(void) {
  SuhallaDesert_CatchInStorm(LOCALID_FIRST_STORM + 1);
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the leader walks into the third storm (metatile
/// behavior 43 or 53).
static void EventScript_CaughtByThirdStorm(void) {
  SuhallaDesert_CatchInStorm(LOCALID_FIRST_STORM + 2);
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the leader walks into the fourth storm (metatile
/// behavior 44 or 54).
static void EventScript_CaughtByFourthStorm(void) {
  SuhallaDesert_CatchInStorm(LOCALID_FIRST_STORM + 3);
}

/// The fifth storm catches the leader. In SuhallaDesertNorth the leader walks
/// into it (metatile behavior 45); in SuhallaDesertSouth it's the chasing
/// storm, which reaches the moving leader (event 55).
static void EventScript_CaughtByFifthStorm(void) {
  SuhallaDesert_CatchInStorm(LOCALID_FIRST_STORM + 4);
}

/// As the party nears a storm it rises, growing to full size with the sand
/// haze fading in the first time. The first time a storm rises with Iodem in
/// the party, he steps out and warns of the sandstorms: "If we don't get past
/// here, we'll never make it to Babi Lighthouse."
static void SuhallaDesert_RaiseStorm(s32 stormId) {
  s32 frame;
  s32 nextFrame;
  s32 stormIdCopy;
  s32 leaderX;
  s32 leaderZ;
  struct ObjectEvent *storm;
  s32 appearanceRestored;
  struct ObjectEvent *selectedObjectEvent;
  struct ObjectEvent *leader;

  stormIdCopy = stormId;
  selectedObjectEvent = ObjectEvent_GetById(gGameState.leaderLocalId);
  storm = ObjectEvent_GetById(stormIdCopy);
  appearanceRestored = GameFlag_Test(FLAG_STORMS_RISEN);
  leaderX = selectedObjectEvent->x.part.pixel;
  leaderZ = selectedObjectEvent->z.part.pixel;
  FieldScript_Start();
  Sound_PlaySong(SE_SANDSTORM);
  // Grow over 16 frames and, the first time, fade the haze in with it.
  frame = 0;
  do {
    storm->scaleX = ((frame << 11) + 0x800);
    storm->scaleY = ((frame << 12) + 0x1000);
    if (appearanceRestored == 0) {
      nextFrame = frame + 1;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                          BLEND_ALPHA(nextFrame, 16 - nextFrame), 0x20000);
    } else {
      nextFrame = frame + 1;
    }
    WaitFrames(1);
    frame = nextFrame;
  } while (frame <= 15);
  Engine_GameFlagSet((FLAG_MAP_TEMP_06 - LOCALID_FIRST_STORM + stormIdCopy));
  Engine_GameFlagSet(FLAG_STORMS_RISEN);
  if (GameFlag_Test(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_IsSet(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0) {
    Engine_GameFlagSet(FLAG_SUHALLA_DESERT_IODEM_WARNED);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if ((s32)leader != 0) {
      Engine_ObjectEventSetPosition(LOCALID_IODEM, leader->x.fixed,
                                    leader->z.fixed);
    }
#ifdef BUGFIX
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
#else
    // BUG: a heading where ObjectEvent_FaceObjectEvent takes a local id.
    ObjectEvent_FaceObjectEvent(LOCALID_IODEM, FACING_SOUTH, 0);
#endif
    ObjectEvent_GetById(LOCALID_IODEM)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    ObjectMotion_ResetAndSetPositionInMode2(LOCALID_IODEM, leaderX,
                                            (leaderZ - 16));
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
        ~OBJ_EVENT_FACES_MOTION;
    ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, (leaderX + 8),
                                     (leaderZ - 40));
    FieldScript_WaitFrames(1);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
        OBJ_EVENT_FACES_MOTION;
    Engine_ObjectEventSetAnimation(LOCALID_IODEM, ANIM_STAND);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    FieldScript_SetMessageId(MsgSuhallaSandstorm);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(1), PIXELS(1) / 2);
    Engine_ObjectEventSetAnimation(LOCALID_IODEM, ANIM_WALK);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if ((s32)leader != 0) {
      ObjectMotion_ResetAndSetPosition(LOCALID_IODEM, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
    Engine_ObjectEventSetPosition(LOCALID_IODEM, 0, 0);
  }
  FieldScript_End();
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the party nears the map's first storm (metatile
/// behavior 61 or 71).
static void EventScript_FirstStormRises(void) {
  SuhallaDesert_RaiseStorm(LOCALID_FIRST_STORM);
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and MapId_SuhallaDesertSouth),
/// the party nears the second storm (metatile behavior 62 or 72).
static void EventScript_SecondStormRises(void) {
  SuhallaDesert_RaiseStorm(LOCALID_FIRST_STORM + 1);
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and MapId_SuhallaDesertSouth),
/// the party nears the third storm (metatile behavior 63 or 73).
static void EventScript_ThirdStormRises(void) {
  SuhallaDesert_RaiseStorm(LOCALID_FIRST_STORM + 2);
}

/// In SuhallaDesertNorth, the party nears the fourth storm (metatile behavior
/// 64).
static void EventScript_FourthStormRises(void) {
  SuhallaDesert_RaiseStorm(LOCALID_FIRST_STORM + 3);
}

/// In SuhallaDesertNorth, the party nears the fifth storm (metatile behavior
/// 65).
static void EventScript_FifthStormRises(void) {
  SuhallaDesert_RaiseStorm(LOCALID_FIRST_STORM + 4);
}

/// Iodem asks whether to search the desert for Sheba again or press on. It
/// plays in SuhallaDesertSouth, near its way out to the world map (metatile
/// behavior 9), once Iodem has heard the wounded soldiers in Suhalla: he steps
/// out of the party. "Sheba is nowhere to be found. Should we search the desert
/// once more or press on to Lalivero?" Whatever the answer, Iodem decides to
/// head first for the guard gate ahead, Suhalla Gate, and leave Sheba to the
/// soldiers there, and rejoins the party. Sets
/// FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA.
static void EventScript_IodemAsksWhetherToPressOn(void) {
  struct ObjectEvent *leader;

  if (GameFlag_IsSet(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_IsSet(0x1b7) == 0 &&
      GameFlag_IsSet(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA);
    FieldScript_Start();
    FieldScript_SetMessageId(MsgSuhallaNowhereFound);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetPosition(LOCALID_IODEM, leader->x.fixed, leader->z.fixed);
    }
#ifdef BUGFIX
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
#else
    // BUG: a heading where ObjectEvent_FaceObjectEvent takes a local id.
    ObjectEvent_FaceObjectEvent(LOCALID_IODEM, FACING_NORTH, 0);
#endif
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 440, 1256);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 444, 1240);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 30);
    ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          60);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Script_ShowMessage(LOCALID_IODEM, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 30);
    Script_AskYesNo(LOCALID_IODEM, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_NOD);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(7) / 10, PIXELS(7) / 20);
    ObjectEvent_WalkToAndWait(LOCALID_IODEM, 440, 1256);
    Script_ShowMessage(LOCALID_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_IODEM, ANIM_WALK);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetDestination(LOCALID_IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IODEM);
    ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
    FieldScript_End();
  }
}

/// The chasing storm's script: it follows the leader, throwing up sand every
/// eight frames, and catches a moving leader it comes within 8 pixels of
/// (event 55).
static s32 SuhallaDesert_ChaseLeader(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  struct EffectOptions options;
  struct ObjectEvent *selectedObjectEvent;
  s32 dx;
  s32 dz;
  s32 sparkFrame;

  selectedObjectEvent = ObjectEvent_GetById(gGameState.leaderLocalId);
  objectEvent->acceleration = PIXELS(1) / 4;
  objectEvent->speed = PIXELS(3) / 2;
  objectEvent->motionFlags = 0;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent->spriteStorageKind ^= 1;
  if (GameFlag_Test(FLAG_FIELD_HANDLING_REQUESTS)) {
    objectEvent->targetX = OBJ_EVENT_NO_TARGET;
    objectEvent->targetY = OBJ_EVENT_NO_TARGET;
    objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  } else {
    objectEvent->targetX = selectedObjectEvent->x.fixed;
    objectEvent->targetY = selectedObjectEvent->groundHeight;
    objectEvent->targetZ = selectedObjectEvent->z.fixed;
    dx = objectEvent->x.fixed - selectedObjectEvent->x.fixed;
    if (dx < 0) {
      dx = selectedObjectEvent->x.fixed - objectEvent->x.fixed;
    }
    dz = objectEvent->z.fixed - selectedObjectEvent->z.fixed;
    if (dx + (dz >= 0 ? objectEvent->z.fixed - selectedObjectEvent->z.fixed
                      : selectedObjectEvent->z.fixed - objectEvent->z.fixed) <
        PIXELS(8)) {
      struct FieldWork *work = gFieldWork;

      if (selectedObjectEvent->motionFlags != 0) {
        work->raisedEvent = RAISED_CHASING_STORM_CATCHES;
      }
      objectEvent->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
      objectEvent->targetX = selectedObjectEvent->x.fixed;
      objectEvent->targetY = selectedObjectEvent->y.fixed;
      objectEvent->targetZ = selectedObjectEvent->z.fixed;
    }
  }
  // Sand every eight frames, with a random spin.
  sparkFrame = gFrameCount & 7;
  if (sparkFrame == 0) {
    options.startScaleX = 4 * Q16_ONE / 5;
    options.startScaleY = 4 * Q16_ONE / 5;
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                      objectEvent->z.fixed, 0, sparkFrame, sparkFrame,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  return 1;
}

/// In SuhallaDesertSouth, until the chasing storm has been doused: it
/// appears behind the party and gives chase.
static void EventScript_StormGivesChase(void) {
  if (GameFlag_IsSet(FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED) == 0) {
    GameFlag_Set(FLAG_MAP_TEMP_0E);
    ObjectEvent_SetPosition(LOCALID_CHASING_STORM, PIXELS(120), PIXELS(824));
    ObjectEvent_SetActionScript(LOCALID_CHASING_STORM,
                                gSuhallaDesertChaseScript);
  }
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), Douse cast at the storm that caught the leader:
/// sets the Psynergy effect's child option, so that Douse's orb rises to 80
/// pixels above the storm rather than 32 and rains on it from above
/// (FieldEffect_RunDouse).
static void EventScript_DouseCastAtStorm(void) {
  u8 *state = gFieldEffectWork;
  ((struct FieldEffectWork *)state)->childOption = 1;
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), Douse stops the storm that caught the leader:
/// it is doused for good, and the monster in it attacks.
/// MapId_SuhallaDesertSouth's fourth and chasing storms hold other monsters.
static void EventScript_DouseStorm(s32 unused, s32 stormId) {
  struct GameState *state;

  // The storm no longer carries the party away.
  if (gFieldWork->raisedEvent == RAISED_STORM_CARRIES_PARTY_AWAY) {
    gFieldWork->raisedEvent = 0;
  }
  GameFlag_ClearBit(FLAG_MAP_TEMP_0F);
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertNorth) {
    Engine_GameFlagSet(
        (stormId + FLAG_FIRST_MAP_STORM_DOUSED - LOCALID_FIRST_STORM));
  } else {
    if (gGameState.mapId == (s32)&MapId_SuhallaDesertSouth) {
      Engine_GameFlagSet(
          (stormId + FLAG_SECOND_MAP_STORM_DOUSED - LOCALID_FIRST_STORM));
    }
  }
  FieldGameFlag_SetByte(FLAG_MAP_TEMP_10, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  state = &gGameState;
  if (state->mapId == (s32)&MapId_SuhallaDesertSouth) {
    if (stormId == LOCALID_FOURTH_STORM) {
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
    } else {
      if (stormId == LOCALID_CHASING_STORM) {
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 6);
        ObjectMotion_EnableActionAndResetMotion(LOCALID_CHASING_STORM);
        Engine_ObjectEventSetPosition(LOCALID_CHASING_STORM, 0, 0);
      }
    }
  }
  ObjectEvent_GetById(gGameState.leaderLocalId)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
}

/// In Suhalla Desert (MapId_SuhallaDesertNorth and
/// MapId_SuhallaDesertSouth), the spinning runs out (event 99): the storm
/// rises into the sky with the party, the screen closes, and the party lands
/// falling (FLAG_ARRIVE_FALLING) on the world map outside Suhalla, or where the
/// second map's fourth storm drops it.
static void EventScript_StormCarriesPartyAway(void) {
  s32 stormId;
  struct ObjectEvent *selectedObjectEvent;
  struct ObjectEvent *storm;
  s32 frame;

  stormId = FieldGameFlag_GetByte(FLAG_MAP_TEMP_18);
  selectedObjectEvent = ObjectEvent_GetById(gGameState.leaderLocalId);
  storm = ObjectEvent_GetById(stormId);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Sound_PlaySong(SE_219);
  // BUG: the native passes a local id where the sprite helper needs a pointer.
#ifdef BUGFIX
  ObjectDispatch_SetSingleChildFlags(selectedObjectEvent, 0);
#else
  ObjectEvent_SetSpriteFlags((struct ObjectEvent *)gGameState.leaderLocalId, 0);
#endif
  storm->motionFlags = 0;
  selectedObjectEvent->motionFlags = 0;
  selectedObjectEvent->velocityY = 0;
  selectedObjectEvent->frozen = 1;
  storm->frozen = 1;
  // Both rise faster and faster for a second.
  for (frame = 0; frame < 60; frame++) {
    selectedObjectEvent->velocityY += PIXELS(1) / 5;
    storm->velocityY += PIXELS(1) / 5;
    WaitFrames(1);
  }
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
  Engine_GameFlagSet(FLAG_ARRIVE_FALLING);
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertSouth &&
      FieldGameFlag_GetByte(FLAG_MAP_TEMP_18) == LOCALID_FOURTH_STORM) {
    FieldScript_SetDestination((s32)&MapId_WorldMap,
                               WORLD_MAP_FROM_SUHALLA_DESERT_STORM);
  } else {
    FieldScript_SetDestination((s32)&MapId_WorldMap, WORLD_MAP_FROM_SUHALLA);
  }
}

/// While Reveal lasts: one palette color pulses, brightening and dimming over
/// 64 frames.
static void SuhallaDesert_PulseRevealPalette(void) {
  u16 brightnessFrame = gFrameCount & 63;
  s32 color;

  if (brightnessFrame > 31)
    brightnessFrame = 64 - brightnessFrame;
  color = (brightnessFrame >> 1) + 7;
  color |= (color << 10) | (color << 5);
  SUHALLA_REVEAL_PALETTE_COLOR = ((u32)color << 16) >> 16;
}

// Native event return forms: see MATCHING.md.
#ifdef UBFIX
/// Reveal wears off, and what it showed is hidden again. It runs as Reveal
/// ends and whenever a map is entered, and only in SuhallaDesertSouth does it
/// hide what Reveal showed and put back the pulsing color.
static void EventScript_RevealEnds(void) {
#else
/// Reveal wears off, and what it showed is hidden again. It runs as Reveal
/// ends and whenever a map is entered, and only in SuhallaDesertSouth does it
/// hide what Reveal showed and put back the pulsing color.
static s32 EventScript_RevealEnds(void) {
  s32 removalResult;
#endif
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertSouth) {
    ObjectEvent_GetById(LOCALID_REVEALED_PILLAR)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_REVEALED_PILLAR)->motionFlags =
        OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    ObjectEvent_SetPosition(LOCALID_REVEALED_PILLAR, 0, 0);
    MapGrid_CopyCellAttributes(16, 44, 1, 1, 15, 44);
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
    MapGrid_CopyCellAttributes(12, 71, 1, 1, 127, 127);
    MapGrid_CopyCellAttributes(11, 71, 1, 1, 12, 71);
#ifdef UBFIX
    Task_Remove(SuhallaDesert_PulseRevealPalette);
#else
    removalResult = Task_Remove(SuhallaDesert_PulseRevealPalette);
#endif
    Io_Write16(sSuhallaDesertSavedColor, &SUHALLA_REVEAL_PALETTE_COLOR);
#ifndef UBFIX
    return removalResult;
#endif
  }
}

#ifdef UBFIX
/// Reveal shows a hidden stone pillar in the desert's south. In
/// SuhallaDesertSouth the stone pillar appears at 248, 712, map cells and the
/// Psynergy Stone's map object change, and a palette color pulses.
static void EventScript_RevealStarts(void) {
#else
/// Reveal shows a hidden stone pillar in the desert's south. In
/// SuhallaDesertSouth the stone pillar appears at 248, 712, map cells and the
/// Psynergy Stone's map object change, and a palette color pulses.
static s32 EventScript_RevealStarts(void) {
#endif
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertSouth) {
    ObjectEvent_GetById(LOCALID_REVEALED_PILLAR)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_REVEALED_PILLAR)->motionFlags = 0;
    ObjectEvent_SetPosition(LOCALID_REVEALED_PILLAR, PIXELS(248), PIXELS(712));
    MapGrid_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    TreasureSpot_MoveLeaderAway();
    MapGrid_CopyCellAttributes(127, 127, 1, 1, 12, 71);
#ifdef UBFIX
    FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                               TASK_PRIORITY_MAP);
#else
    return FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                                      TASK_PRIORITY_MAP);
#endif
  }
}

/// The third map's warps, or the storms, Iodem, treasure and Reveal of the
/// first two.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesEvents;
  }
  return gSuhallaDesertEventsOther;
}

/// Sets a map up as the party enters: a capture goes on spinning, Reveal's
/// color is saved, and the first two maps set up their storms.
s32 Map_Initialize(void) {
  struct MapGridState *gridState;

  gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (FieldGameFlag_GetByte(FLAG_MAP_TEMP_10) != 0) {
    gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PRIORITY_MAP);
  }
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertNorth ||
      gGameState.mapId == (s32)&MapId_SuhallaDesertSouth) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    EventScript_RevealEnds();
  }
  if (gGameState.mapId == (s32)&MapId_SuhallaDesertNorth) {
    SuhallaDesert_SetUpFirstMapStorms();
  } else if (gGameState.mapId == (s32)&MapId_SuhallaDesertSouth) {
    SuhallaDesert_SetUpSecondMapStorms();
  } else {
    Sound_PlaySong(SE_STOP_SLOT3);
  }
  if (gGameState.warpDest == WARP_DEST_DEFAULT) {
    gridState->flags &= ~DISPCNT_BG1_ON;
  }
  return 0;
}

/// The first map's storms on entry: a doused storm counts as risen, the
/// others start small (unless the party is coming back to where it stood), and
/// the sand haze is faded in if a storm has risen before.
static void SuhallaDesert_SetUpFirstMapStorms(void) {
  // Trial: sharing the blend tail through an ordinary inline shrinks this
  // function from 0x144 to 0x138 bytes in tbs-en; retain the original body.
  struct ObjectEvent *objectEvent;
  s32 stormId;
  s32 stormAlpha = 0;

  if (GameFlag_Test(FLAG_FIRST_MAP_STORM_DOUSED)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_06);
  }
  if (GameFlag_Test(FLAG_FIRST_MAP_STORM_DOUSED + 1)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_07);
  }
  if (GameFlag_Test(FLAG_FIRST_MAP_STORM_DOUSED + 2)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_08);
  }
  if (GameFlag_Test(FLAG_FIRST_MAP_STORM_DOUSED + 3)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_09);
  }
  if (GameFlag_Test(FLAG_FIRST_MAP_STORM_DOUSED + 4)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_0A);
  }
  for (stormId = LOCALID_FIRST_STORM; stormId <= LOCALID_LAST_STORM;
       stormId++) {
    objectEvent = ObjectEvent_GetById(stormId);
    if (objectEvent != 0) {
      if (!GameFlag_Test(FLAG_KEEP_PARTY_POSITION)) {
        objectEvent->scaleX = Q16_ONE / 32;
        objectEvent->scaleY = Q16_ONE / 32;
      }
      objectEvent->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, 0x20000);
  if (GameFlag_Test(FLAG_STORMS_RISEN)) {
    stormAlpha = 16;
    Map_QueueMusic(SE_SANDSTORM);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - stormAlpha) << 8) | stormAlpha,
                      0x20000);
}

/// The second map's storms on entry, as the first map's; the fourth storm's
/// first sprite part takes parameter 10, and once the chasing storm has been
/// doused it is gone for good.
static void SuhallaDesert_SetUpSecondMapStorms(void) {
  // Trial: the same blend-tail inline shrinks this function from 0x158 to 0x14c
  // bytes in tbs-en; retain the original body.
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  s32 stormId;
  s32 stormAlpha = 0;

  if (GameFlag_Test(FLAG_SECOND_MAP_STORM_DOUSED)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_06);
  }
  if (GameFlag_Test(FLAG_SECOND_MAP_STORM_DOUSED + 1)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_07);
  }
  if (GameFlag_Test(FLAG_SECOND_MAP_STORM_DOUSED + 2)) {
    Engine_GameFlagSet(FLAG_MAP_TEMP_08);
  }
  for (stormId = LOCALID_FIRST_STORM; stormId < LOCALID_FOURTH_STORM;
       stormId++) {
    objectEvent = ObjectEvent_GetById(stormId);
    if (objectEvent != 0) {
      if (!GameFlag_Test(FLAG_KEEP_PARTY_POSITION)) {
        objectEvent->scaleX = Q16_ONE / 32;
        objectEvent->scaleY = Q16_ONE / 32;
      }
      sprite = objectEvent->sprite;
      sprite->flags = 0;
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_FOURTH_STORM);
  if (objectEvent != 0) {
    struct SpriteAnim *part;

    sprite = objectEvent->sprite;
    part = sprite->entries[0];
    if (part != 0) {
      part->colorMode = 10;
    }
    sprite->dirty = 1;
    sprite->flags = 0;
  }
  if (GameFlag_Test(FLAG_SECOND_MAP_STORM_DOUSED + 4)) {
    Engine_GameFlagSet(FLAG_SUHALLA_DESERT_CHASING_STORM_DOUSED);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, 0x20000);
  if (GameFlag_Test(FLAG_STORMS_RISEN)) {
    stormAlpha = 16;
    Map_QueueMusic(SE_SANDSTORM);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - stormAlpha) << 8) | stormAlpha,
                      0x20000);
}

s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

s32 gSuhallaDesertStormScript[] = {
    SC_CALL(SuhallaDesert_ClearObjectEventSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleObjectEventActive),
    SC_END,
};

s32 gSuhallaDesertFourthStormScript[] = {
    SC_CALL(SuhallaDesert_ClearObjectEventSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleObjectEventActive),
    SC_END,
};

struct MapWarpDest gSuhallaDesertWarpDestsOther[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSuhallaDesertNorthWarpDests[] = {
    {SUHALLA_DESERT_NORTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 264, 0, 56,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, 24, 0,
     936, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSuhallaDesertSouthWarpDests[] = {
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, 488, 0,
     456, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 1208,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST, CONDITION_ALWAYS, 440,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST, CONDITION_ALWAYS, 56,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSuhallaDesertCavesWarpDests[] = {
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST, CONDITION_ALWAYS, 72,
     0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_WEST_FROM_EAST, CONDITION_ALWAYS, 440, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_EAST_FROM_WEST, CONDITION_ALWAYS, 632, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST, CONDITION_ALWAYS, 648,
     0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gSuhallaDesertWarps[] = {
    MAP_WARPS(SuhallaDesertNorth),
    MAP_WARP(SUHALLA_DESERT_NORTH_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_SUHALLA_DESERT_NORTH),
    MAP_WARP(SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
             SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH),
    MAP_WARPS(SuhallaDesertSouth),
    MAP_WARP(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
             SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH),
    MAP_WARP(SUHALLA_DESERT_SOUTH_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH),
    MAP_WARP(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST,
             SuhallaDesertCaves,
             SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST),
    MAP_WARP(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST,
             SuhallaDesertCaves,
             SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST),
    MAP_WARPS(SuhallaDesertCaves),
    MAP_WARP(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST,
             SuhallaDesertSouth,
             SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST),
    MAP_WARP(SUHALLA_DESERT_CAVES_WEST_TO_EAST, SuhallaDesertCaves,
             SUHALLA_DESERT_CAVES_EAST_FROM_WEST),
    MAP_WARP(SUHALLA_DESERT_CAVES_EAST_TO_WEST, SuhallaDesertCaves,
             SUHALLA_DESERT_CAVES_WEST_FROM_EAST),
    MAP_WARP(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST,
             SuhallaDesertSouth,
             SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSuhallaDesertObjectEventTemplatesOther[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gSuhallaDesertNorthObjectEventTemplates[] = {
    { SPRITE_SANDSTORM, FLAG_FIRST_MAP_STORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_FIRST_MAP_STORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_FIRST_MAP_STORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_FIRST_MAP_STORM_DOUSED + 3, (s32)gSuhallaDesertStormScript,
        PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_FIRST_MAP_STORM_DOUSED + 4, (s32)gSuhallaDesertStormScript,
        PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaDesertSouthObjectEventTemplates[] = {
    { SPRITE_SANDSTORM, FLAG_SECOND_MAP_STORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_SECOND_MAP_STORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SECOND_MAP_STORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SECOND_MAP_STORM_DOUSED + 3, (s32)gSuhallaDesertFourthStormScript,
        PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_SECOND_MAP_STORM_DOUSED + 4, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_FLASH), MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaDesertCavesObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

s32 gSuhallaDesertChaseScript[] = {
    SC_CALL(SuhallaDesert_ChaseLeader),
    SC_END,
};

/// The first two maps' events, in one table. Its warps keep their numbers,
/// since each map's warp table gives them its own ways out: warp 1 is
/// SUHALLA_DESERT_NORTH_TO_WORLD_MAP in the first and
/// SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH in the second.
struct MapEvent gSuhallaDesertEventsOther[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, 1 },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, 2 },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, 3 },
#if !TBS_PATCH_1
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, 4 },
#else
    { MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4 },
#endif
    { MAP_EVENT_COORD, 9, FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA, EVENT_SCRIPT(EventScript_IodemAsksWhetherToPressOn) },
    { MAP_EVENT_COORD, 41, FLAG_FIRST_MAP_STORM_DOUSED, EVENT_SCRIPT(EventScript_CaughtByFirstStorm) },
    { MAP_EVENT_COORD, 42, FLAG_FIRST_MAP_STORM_DOUSED + 1, EVENT_SCRIPT(EventScript_CaughtBySecondStorm) },
    { MAP_EVENT_COORD, 43, FLAG_FIRST_MAP_STORM_DOUSED + 2, EVENT_SCRIPT(EventScript_CaughtByThirdStorm) },
    { MAP_EVENT_COORD, 44, FLAG_FIRST_MAP_STORM_DOUSED + 3, EVENT_SCRIPT(EventScript_CaughtByFourthStorm) },
    { MAP_EVENT_COORD, 45, FLAG_FIRST_MAP_STORM_DOUSED + 4, EVENT_SCRIPT(EventScript_CaughtByFifthStorm) },
    { MAP_EVENT_COORD, 51, FLAG_SECOND_MAP_STORM_DOUSED, EVENT_SCRIPT(EventScript_CaughtByFirstStorm) },
    { MAP_EVENT_COORD, 52, FLAG_SECOND_MAP_STORM_DOUSED + 1, EVENT_SCRIPT(EventScript_CaughtBySecondStorm) },
    { MAP_EVENT_COORD, 53, FLAG_SECOND_MAP_STORM_DOUSED + 2, EVENT_SCRIPT(EventScript_CaughtByThirdStorm) },
    { MAP_EVENT_COORD, 61, FLAG_MAP_TEMP_06, EVENT_SCRIPT(EventScript_FirstStormRises) },
    { MAP_EVENT_COORD, 62, FLAG_MAP_TEMP_07, EVENT_SCRIPT(EventScript_SecondStormRises) },
    { MAP_EVENT_COORD, 63, FLAG_MAP_TEMP_08, EVENT_SCRIPT(EventScript_ThirdStormRises) },
    { MAP_EVENT_COORD, 64, FLAG_MAP_TEMP_09, EVENT_SCRIPT(EventScript_FourthStormRises) },
    { MAP_EVENT_COORD, 65, FLAG_MAP_TEMP_0A, EVENT_SCRIPT(EventScript_FifthStormRises) },
    { MAP_EVENT_COORD, 71, FLAG_MAP_TEMP_06, EVENT_SCRIPT(EventScript_FirstStormRises) },
    { MAP_EVENT_COORD, 72, FLAG_MAP_TEMP_07, EVENT_SCRIPT(EventScript_SecondStormRises) },
    { MAP_EVENT_COORD, 73, FLAG_MAP_TEMP_08, EVENT_SCRIPT(EventScript_ThirdStormRises) },
    { MAP_EVENT_COORD, 54, FLAG_SECOND_MAP_STORM_DOUSED + 3, EVENT_SCRIPT(EventScript_CaughtByFourthStorm) },
    { MAP_EVENT_COORD, 81, FLAG_MAP_TEMP_0E, EVENT_SCRIPT(EventScript_StormGivesChase) },
    { MAP_EVENT_TALK, LOCALID_FLASH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FightDjinni) },
    { BG_EVENT(BG_EVENT_SILENT), TREASURE_SPOT_PSYNERGY_STONE, FLAG_SUHALLA_DESERT_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
    { BG_EVENT(BG_EVENT_CHEST), 104, FLAG_SUHALLA_DESERT_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_SUHALLA_DESERT_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_SUHALLA_DESERT_MIMIC_BEATEN, FIND_MIMIC | 6 },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FIRST_STORM, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FIRST_STORM + 1, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FIRST_STORM + 2, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FIRST_STORM + 3, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FIRST_STORM + 4, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_FIRST_STORM, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_FIRST_STORM + 1, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_FIRST_STORM + 2, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_FIRST_STORM + 3, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_FIRST_STORM + 4, IF_FLAG_SET(FLAG_MAP_TEMP_0F), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealStarts) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealEnds) },
    { MAP_EVENT_RAISED, RAISED_STORM_CARRIES_PARTY_AWAY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StormCarriesPartyAway) },
    { MAP_EVENT_RAISED, RAISED_CHASING_STORM_CATCHES, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CaughtByFifthStorm) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gSuhallaDesertCavesEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST},
    {MAP_EVENTS_END},
};
