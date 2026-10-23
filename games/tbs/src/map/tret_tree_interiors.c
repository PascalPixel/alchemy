/// @file
/// Tret Tree (MapId_TretTreeInteriors), the inside of Tret, the holy tree
/// of Kolima Forest, and the first of Tret Tree's two maps. The party comes
/// in from Kolima Forest (kolima_forest_tree.c) to face Tret's wrathful side.
/// FLAG_TRET_DEFEATED, whether that battle is won and Tret has spoken with the
/// party since, decides most of the map.
///
/// The map is six rooms, the six camera boxes of its warp destination table.
/// The party comes in from Kolima Forest (MapId_KolimaForestTree) at
/// TRET_TREE_INTERIORS_FROM_KOLIMA_FOREST_TREE and leaves by
/// TRET_TREE_INTERIORS_TO_KOLIMA_FOREST_TREE; eight warps, from
/// TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST to _NORTH_NORTHEAST,
/// lead out to Tret Tree's other map (MapId_TretTreeBranch), and six more,
/// TRET_TREE_INTERIORS_ENTRANCE_TO_SECOND_FLOOR to
/// TRET_TREE_INTERIORS_ENTRANCE_TO_TRET, lead from room to room, three of them
/// through regions 28 to 36 pixels up (Map_GetWarpRegions).
///
/// Webs hold cocoons, each a coord event (metatile behaviors 21 to 29, 31, 32
/// and 50 to 59) with a temporary flag of its own (FLAG_MAP_TEMP_00 to _14).
/// The first step on a cocoon tears it: its cell is redrawn and its flag set.
/// The next step on a torn cocoon breaks through, and the leader spins and
/// falls to the room below, arriving at the warp destination numbered as the
/// cocoon (FLAG_ARRIVE_FALLING plays the landing): the cocoons 21 to 29 and 31
/// and 32 drop the leader into one room and 50 to 59 into another
/// (https://alchemy.pascalpixel.com/places/tret-tree-interiors/cocoons.png:
/// the whole map, its six rooms where the grid lays them out, each cocoon's
/// cells labelled with its metatile behavior and the warp destination of the
/// same number ringed in the same colour). The layout draws four cocoons
/// already torn (22, 52, 54 and 58) and the setup sets their flags, so they
/// give way at the first step. A latch in the map state (sLastCocoon) keeps a
/// step from counting twice until the leader walks on. Cocoon 50 is the way
/// down to Tret: warp destinations 50, 40, 30 and 20 each let the leader fall
/// on to the warp destination numbered ten lower, and at warp destination 10
/// the leader lands in a ring of dust as the map shakes, in front of Tret. Torn
/// cocoons stay torn across a battle: after one (FLAG_KEEP_PARTY_POSITION) the
/// setup redraws them from gTretTreeInteriors_MapPatches.
///
/// Tret's face (LOCALID_TRET) and a second Tret object event at the same spot
/// speak his lines. Walking up to him (metatile behavior 40, while
/// FLAG_TRET_DEFEATED is clear) runs EventScript_FightTret: he roars, the party
/// gathers in front of him, and the evil within him scorns them before the
/// battle with Tret. The battle returns to
/// TRET_TREE_INTERIORS_AFTER_FIGHT_TRET, where the setup plays the conversation
/// after it: a passage opens in his room, he comes to himself, thanks the party
/// and asks whether they need something, realises that he turned the people of
/// Kolima into trees ("I turned the people of Kolima into trees?"), tries to
/// free them but cannot reach that far, and his face sinks away as the party
/// resolve to heal him. Only then is FLAG_TRET_DEFEATED set; in Kolima Forest
/// it lets Hermes' Water heal Tret and lift the curse. Once it is set, the
/// setup moves both Tret object events away, opens that passage and redraws
/// cells near warp destination 20; before, it keeps the passage closed.
///
/// The setup also saves the background palettes and starts a slow tint that
/// shifts them every 32 frames (not after the battle until the conversation
/// ends; casting Reveal stops it), and scales the first layer of the map grid
/// to the room. Two chests hold a Healing Ring and a Nut, and a Psynergy Stone
/// lies here (bg events 100 to 102).
///
/// Open questions: none.

#include "speaker.h"
#include "version.h"
#include "save_block.h"
#include "map_ids.h"
#include "unit/map/tret_tree_interiors.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"

#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "create_configured_overlay_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "heap_slots.h"
#include "iwram_call.h"
#include "dma.h"
#include "field_service.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "heap.h"
#include "field_effect.h"
#include "flags.h"
#include "character_ids.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "script.h"

/// Tret's face (SPRITE_TRET), the first object event placed after the party,
/// and a second Tret object event (SPRITE_TRET_RAISED) placed at the same spot.
/// Both speak Tret's lines: the face as he attacks, the second after the
/// battle.
enum {
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_TRET = LOCALID_FIRST_PLACED,
  /// Draws nothing the render shows beyond Tret's face.
  /// ★ Named from the game: "We can't let Tret die like this!" (MsgTretMmmmm)
  LOCALID_SECOND_TRET
};

/// The speaker of a message; every speaker on this map has
/// SPEAKER_WINDOW_BELOW.
#define SPEAKER(localId) (SPEAKER_WINDOW_BELOW | (localId))

// The map's temporary flags: FLAG_MAP_TEMP_00 to _14 are set the first
// time each cocoon is stepped on, tearing it; the next step breaks through
// (gTretTreeInteriors_MapPatches lists them).

#define COCOON_FALL_SPIN_FRAMES 30
#define SWIRL_SPARK_LAST_FRAME 79
#define SWIRL_SPARK_SHRINK_LAST_FRAME 39
#define TRET_TINT_COLOR_COUNT 63
#define TRET_TINT_CHANNEL_MAX 31
#define TRET_TINT_END 99

static void TretPalette_SaveMapColors(void);
static void TretPalette_UpdateMapTint(void);
/// The face gestures TretTreeInteriors_PlayGesture plays on Tret, each from
/// the face with its eyes open; the wrathful ones on his wrathful face.
enum TretTreeInteriorsGesture {
  TRET_GESTURE_CLOSE_EYES = 0,
  TRET_GESTURE_OPEN_EYES = 1,
  TRET_GESTURE_LOOK_LEFT = 2,
  TRET_GESTURE_LOOK_RIGHT = 3,
  TRET_GESTURE_BLINK = 4, ///< twice
  TRET_GESTURE_HALF_CLOSE_EYES = 5,
  TRET_GESTURE_WRATHFUL_CLOSE_EYES = 7,
  TRET_GESTURE_WRATHFUL_OPEN_EYES = 8,
  TRET_GESTURE_WRATHFUL_LOOK_RIGHT = 9,
  TRET_GESTURE_WRATHFUL_LOOK_LEFT = 10,
  TRET_GESTURE_WRATHFUL_BLINK = 11, ///< twice
  TRET_GESTURE_WRATHFUL_STARE = 12  ///< as TRET_GESTURE_WRATHFUL_OPEN_EYES
};

static void TretTreeInteriors_PlayGesture(s32 gesture);
static void TretTreeInteriors_SpawnSwirlSparks(void);
static void TretTreeInteriors_ApplyFlaggedMapPatches(void);
static void TretTreeInteriors_RunLandingDustScene(void);

static struct FieldPosition sSparkOrigin;
static s32 sSparkFrame;
static s16 *sLastCocoon;

#include "common/effect_create.h"

#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// Returns the map's warp destinations, the cocoons' landing spots among them.
const struct MapWarpDest *Map_GetWarpDests(void) {
  return gTretTreeInteriorsWarpDests;
}

/// Returns the three warp regions 28 to 36 pixels up: up to the second floor
/// (TRET_TREE_INTERIORS_ENTRANCE_TO_SECOND_FLOOR), up to the fourth
/// (TRET_TREE_INTERIORS_THIRD_FLOOR_TO_FOURTH_FLOOR) and back from Tret
/// (TRET_TREE_INTERIORS_TRET_TO_ENTRANCE).
const struct MapWarpRegion *Map_GetWarpRegions(void) {
  return gTretTreeInteriorsRegions;
}

const u32 *Map_GetWarps(void) { return gTretTreeInteriorsWarps; }

/// Returns the party's other slots (Garet, Ivan and, once she has joined,
/// Mia), placed for the scripted scenes, and the two Tret object events.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gTretTreeInteriorsObjectEventTemplates;
}

/// Stepping on a cocoon in Tret Tree: the first step tears it, copying its
/// torn picture from the staging cell (row 35) and setting its temporary flag;
/// the next step breaks through (row 36's cell), and the leader spins and
/// falls, to land at the given warp destination, numbered as the cocoon. The
/// latch ignores the same cocoon again until the leader walks on.
static void TretTreeInteriors_StepOnCocoon(s32 cocoonFlag, s32 sourceColumn,
                                           s32 sourceRow, s32 warpDest) {
  s32 destinationColumn;
  s32 destinationRow;
  s32 leaderLocalId;
  struct ObjectEvent *objectEvent;
  s32 spinFrame;

  destinationColumn = (gSaveBlock.x >> 20) + 64;
  destinationRow = gSaveBlock.z >> 20;
  leaderLocalId = gSaveBlock.leaderLocalId;
  objectEvent = ObjectEvent_GetById(leaderLocalId);
  if (warpDest == *sLastCocoon)
    return;
  *sLastCocoon = warpDest;
  if (GameFlag_Get(cocoonFlag) == 0) {
    MapGrid_CopyCells(sourceColumn, sourceRow, 1, 1, destinationColumn,
                      destinationRow);
    GameFlag_Set(cocoonFlag);
    return;
  }
  // On the next touch, replace the cell and spin the object event into the
  // fall.
  *sLastCocoon = -1;
  MapGrid_CopyCells(sourceColumn, sourceRow + 1, 1, 1, destinationColumn,
                    destinationRow);
  SE(SE_206);
  SCRIPT_START();
  FieldScript_SetDestination((s32)&MapId_TretTreeInteriors, warpDest);
  OBJECT_ANIMATION(leaderLocalId, ANIM_LEADER_SPIN_AWAY);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(leaderLocalId), 0);
  ObjectEvent_SetAttachedEffect(leaderLocalId, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ATTACH);
  WAIT(30);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  objectEvent->motionFlags = OBJ_EVENT_FALLS;
  objectEvent->groundHeight = -PIXELS(160);
  objectEvent->gravity = PIXELS(1) / 2;
  SE(SE_204);
  WAIT(3);
  objectEvent->layer = 2;
  ObjectMotion_SetActionVariant(leaderLocalId, 3);
  for (spinFrame = COCOON_FALL_SPIN_FRAMES - 1; spinFrame >= 0; spinFrame--) {

    objectEvent->facing += 2 * FACING_STEP;
    WaitFrames(1);
  }
  if (warpDest != 50)
    GameFlag_Set(FLAG_ARRIVE_FALLING);
}

// The cocoons' coord events in Tret Tree, one per cocoon: each passes its
// flag, the staging cell of its torn picture and its own number, the landing
// warp destination, to TretTreeInteriors_StepOnCocoon.

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon21(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_00, 64, 35, 21);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon22(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_01, 65, 35, 22);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon23(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_02, 66, 35, 23);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon24(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_03, 67, 35, 24);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon25(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_04, 68, 35, 25);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon26(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_05, 69, 35, 26);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon27(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_06, 70, 35, 27);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon28(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_07, 71, 35, 28);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon29(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_08, 72, 35, 29);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon31(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_09, 73, 35, 31);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon32(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0A, 74, 35, 32);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon50(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0B, 79, 35, 50);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon51(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0C, 75, 35, 51);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon52(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0D, 76, 35, 52);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon53(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0E, 77, 35, 53);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon54(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_0F, 78, 35, 54);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon55(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_10, 80, 35, 55);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below. Cocoon 56 lands at the same spot as cocoon 54: their warp
/// destinations agree.
static void EventScript_StepOnCocoon56(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_11, 81, 35, 56);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon57(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_12, 82, 35, 57);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon58(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_13, 83, 35, 58);
}

/// Stepping on a cocoon tears it, and the next step drops the leader to the
/// room below.
static void EventScript_StepOnCocoon59(void) {
  TretTreeInteriors_StepOnCocoon(FLAG_MAP_TEMP_14, 84, 35, 59);
}

/// Walking on over a cocoon in Tret Tree clears the latch, so the next step on
/// a cocoon counts again.
static void EventScript_ResetCocoonLatch(void) {
  if (Field_TryJumpForward() == 0)
    *sLastCocoon = -1;
}

/// Returns the warps, the cocoons, Tret, Reveal and the chests' events.
const struct MapEvent *Map_GetEvents(void) { return gTretTreeInteriorsEvents; }

/// Walking up to Tret in Tret Tree (metatile behavior 40, while
/// FLAG_TRET_DEFEATED is clear): he roars, the party gathers in front of him,
/// and the evil within him scorns them before the battle with Tret starts.
/// "Hm... HROOOM!!!" and "You hoped to find my kindly self?" The battle returns
/// to TRET_TREE_INTERIORS_AFTER_FIGHT_TRET.
static void EventScript_FightTret(void) {
  s32 hasMia;
  struct ObjectEvent *leader;

  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  SCRIPT_START();
  SE(SONG_FADE_OUT_BGM);
  MESSAGE_SET(MsgTretHmHrooom);
  FieldScript_ShowMessageThenWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  SE(MUS_029);
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  OBJECT_SPEED(GARET, PIXELS(1), PIXELS(1) / 2);
  OBJECT_SPEED(IVAN, PIXELS(1), PIXELS(1) / 2);
  OBJECT_SPEED(MIA, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_GetById(MIA)->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_SetActionVariant(MIA, 2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ObjectEventSetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ObjectEventSetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  if (hasMia != 0) {
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      Engine_ObjectEventSetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetActionScript(MIA, gTretTreeInteriors_MiaGathersActions);
  }
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gTretTreeInteriors_LeaderGathersActions);
  ObjectEvent_SetActionScript(GARET, gTretTreeInteriors_GaretGathersActions);
  ObjectEvent_RunActionScriptAndWait(IVAN,
                                     gTretTreeInteriors_IvanGathersActions);
  WAIT(10);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  OBJECT_DIR(MIA, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 40);
  OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_TURNING_WRATHFUL);
  WAIT(10);
  OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
  WAIT(20);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_WRATHFUL_OPEN_EYES);
  Script_ShowMessage(SPEAKER(LOCALID_TRET), 0);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_WRATHFUL_BLINK);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_TRET), 0, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(GARET, 1);
  ObjectMotion_SetVariantCallback(MIA, 1);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  Script_ShowMessage(SPEAKER(LOCALID_TRET), 0);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  FieldEffect_ConfigureLinkedObject(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                              ATTACHED_EFFECT_ONCE);
  WAIT(40);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_WRATHFUL_BLINK);
  MESSAGE_PUT(SPEAKER(LOCALID_TRET), 0);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  gFieldWork->transitionFrames = 64;
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_TretTreeInteriors,
                                 TRET_TREE_INTERIORS_AFTER_FIGHT_TRET);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        0);
  SCRIPT_END();
}

/// After the battle with Tret, in Tret Tree: a passage opens in his room, and
/// he comes to, thanks the party and says good-bye. "Oh... Mmmmm..." Garet and
/// Ivan stop him and he asks whether they need something; answering no earns
/// the leader a scolding. Tret realises that he turned the people of Kolima
/// into trees, tries to free them in a swirl of sparks but cannot, and the
/// party resolve to heal him. Sets FLAG_TRET_DEFEATED, which lets Hermes'
/// Water heal him in Kolima Forest.
static void TretTreeInteriors_PlayPostBattleConversation(void) {
  s32 hasMia;
  struct ObjectEvent *partyObjectEvent;
  s32 openingColumn;
  TaskFunc sparkTask;
  const s32 *actions;

  // Restore the party positions before opening the post-battle conversation.
  hasMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  ObjectEvent_GetById(MIA)->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_SetActionVariant(MIA, 2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  PartyInventory_FindMember(ITEM_HERMES_WATER);
  SE(SONG_FADE_OUT_BGM);
  SCRIPT_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, 0xcccc, 0x6666);
  OBJECT_SPEED(GARET, 0xcccc, 0x6666);
  OBJECT_SPEED(IVAN, 0xcccc, 0x6666);
  OBJECT_SPEED(MIA, 0xcccc, 0x6666);
  OBJECT_POSITION_SET(LOCALID_PARTY_LEADER, 166, 80);
  partyObjectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  partyObjectEvent->facing = FACING_NORTH;
  OBJECT_POSITION_SET(GARET, 148, 90);
  partyObjectEvent = ObjectEvent_Get(GARET);
  partyObjectEvent->facing = FACING_NORTH;
  OBJECT_POSITION_SET(IVAN, 182, 90);
  partyObjectEvent = ObjectEvent_GetById(IVAN);
  partyObjectEvent->facing = FACING_NORTH;
  if (hasMia != 0) {
    OBJECT_POSITION_SET(MIA, 166, 104);
    partyObjectEvent = ObjectEvent_GetById(MIA);
    partyObjectEvent->facing = FACING_NORTH;
  }
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  WaitFrames(10);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 48;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  WAIT(20);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(152), 1);
  FieldCamera_WaitForArrival();
  WAIT(10);
  // Open the passage in Tret's room before continuing the conversation.
  openingColumn = 10;
  SE(SE_MAP_EXIT);
  MapGrid_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
  MapGrid_CopyCells(26, 38, 1, 1, openingColumn, 43);
  WaitFrames(4);
  MapGrid_CopyCells(26, 37, 1, 2, openingColumn, 42);
  WaitFrames(4);
  MapGrid_CopyCells(26, 36, 1, 3, openingColumn, 41);
  WaitFrames(4);
  MapGrid_CopyCells(26, 35, 1, 4, openingColumn, 40);
  WaitFrames(80);
  // Tret comes to: "Oh... Mmmmm... What has happened to me?"
  MESSAGE_SET(MsgTretMmmmm);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  WAIT(20);
  FieldCamera_MoveTo(PIXELS(168), -1, PIXELS(90), 1);
  FieldCamera_WaitForArrival();
  WAIT(40);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_OPEN_EYES);
  WAIT(60);
  SE(MUS_021);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_BLINK);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Script_ShowMessage(SPEAKER(LOCALID_SECOND_TRET), 0);
  WAIT(40);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIM_NOD);
  OBJECT_ANIMATION(GARET, ANIM_NOD);
  OBJECT_ANIMATION(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  WAIT(40);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  WAIT(60);
  // Garet and Ivan will not let Tret die while they still need his help.
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  OBJECT_DIR(GARET, FACING_NORTHEAST, 10);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Script_ShowMessageAndWait(SPEAKER(GARET), 0, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJECT_DIR(IVAN, FACING_NORTHWEST, 0);
  Script_ShowMessageAndWait(SPEAKER(IVAN), 0, 20);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  WAIT(40);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 40);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_BLINK);
  // "Do you need something?" Answering no: "What are you doing, Isaac! Now's
  // not the time to fool around!"
  Script_OpenMessage(SPEAKER(LOCALID_SECOND_TRET), 0);
  OBJECT_DIR(GARET, FACING_NORTHEAST, 0);
  OBJECT_DIR(IVAN, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    OBJECT_ANIMATION(GARET, ANIM_SHAKE_HEAD);
    MESSAGE_SET(MsgTretDoingNowsNot);
    Script_ShowMessage(SPEAKER(GARET), 0);
    OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
    OBJECT_ANIMATION(IVAN, ANIM_NOD);
    Script_ShowMessage(SPEAKER(IVAN), 0);
  }
  WAIT(20);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_BLINK);
  MESSAGE_SET(MsgTretTurnedPeopleKolima);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 20);
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 10);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  WAIT(20);
  // Tret tries to release the people of Kolima: the colours shift and sparks
  // swirl out from his face, then fade.
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x406218, 1);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  OBJECT_DIR(GARET, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 20);
  WAIT(20);
  sSparkFrame = 0;
  {
    struct FieldPosition *origin = &sSparkOrigin;
    origin->x = PIXELS(168);
    origin->y = PIXELS(32);
    sparkTask = TretTreeInteriors_SpawnSwirlSparks;
    origin->z = PIXELS(52);
  }
  Task_AddCallback(sparkTask, TASK_PRIORITY_MAP);
  WAIT(220);
  Task_Remove(sparkTask);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_BLINK);
  WAIT(20);
  // "I can't do it... My power no longer reaches Kolima..." His face sinks
  // away on "People of Kolima... Forgive me...", and the party resolve to
  // heal him.
  Script_ShowMessageAndWait(SPEAKER(LOCALID_SECOND_TRET), 0, 10);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  Script_ShowMessage(SPEAKER(LOCALID_SECOND_TRET), 0);
  ObjectEvent_RunActionScriptAndWait(LOCALID_TRET,
                                     gTretTreeInteriors_FaceSinksActions);
  WAIT(40);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessage(SPEAKER(GARET), 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 10);
  Script_ShowMessage(SPEAKER(IVAN), 0);
  OBJECT_DIR(GARET, FACING_NORTHEAST, 0);
  OBJECT_DIR(IVAN, FACING_NORTHWEST, 10);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  Script_ShowMessageAndWait(SPEAKER(GARET), 0, 10);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  Script_ShowMessageAndWait(SPEAKER(IVAN), 0, 10);
  if (hasMia != 0) {
    ObjectMotion_SetVarCbAndRefresh(MIA, 1);
    FieldScript_ShowMessageThenWait(SPEAKER(MIA), 0, 10);
  }
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIM_NOD);
  OBJECT_ANIMATION(GARET, ANIM_NOD);
  OBJECT_ANIMATION(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  actions = gTretTreeInteriors_RejoinLeaderActions;
  ObjectEvent_SetActionScript(GARET, actions);
  if (hasMia != 0) {
    ObjectEvent_SetActionScript(MIA, actions);
  }
  ObjectEvent_RunActionScriptAndWait(IVAN, actions);
  WAIT(20);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  GameFlag_Set(FLAG_TRET_DEFEATED);
  FieldScheduler_AddCallback(TretPalette_UpdateMapTint, TASK_PRIORITY_MAP);
  SCRIPT_END();
}

/// Sets the map grid by FLAG_TRET_DEFEATED: the passage in Tret's room closed
/// before, open after, with cells near warp destination 20 redrawn. With
/// playRecoveryScene it first plays the conversation after the battle, unless
/// FLAG_KEEP_PARTY_POSITION is set.
static void TretTreeInteriors_ApplyPostBattleState(s32 playRecoveryScene) {
  if (playRecoveryScene != 0 && GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    TretTreeInteriors_PlayPostBattleConversation();

  WaitFrames(1);
  if (GameFlag_Get(FLAG_TRET_DEFEATED) != 0) {
    s32 openingColumn = 10;
    MapGrid_CopyCells(121, 34, 3, 1, 93, openingColumn);
    {
      s32 passageColumn = 30;
      MapGrid_CopyCells(46, 38, 1, 1, passageColumn, 43);
      MapGrid_CopyCellAttributes(0, 0, 1, 2, passageColumn, 9);
    }
    MapGrid_CopyCellAttributes(26, 3, 1, 2, openingColumn, 8);
    MapGrid_CopyCells(26, 35, 1, 4, openingColumn, 40);
  } else {
    s32 openingColumn = 10;
    s32 passageColumn = 8;
    MapGrid_CopyCellAttributes(11, 8, 1, 2, openingColumn, passageColumn);
  }
}

/// Tret Tree's setup: places or removes the Tret object events, starts the
/// tint, redraws the torn cocoons after a battle, sets the map grid by
/// FLAG_TRET_DEFEATED, and plays what the warp destination calls for: the falls
/// on from warp destinations 50, 40, 30 and 20, the landing at warp destination
/// 10, or the conversation after the battle at
/// TRET_TREE_INTERIORS_AFTER_FIGHT_TRET.
s32 TretTreeInteriors_EnterRoom(void) {
  struct ObjectEvent *tret;
  struct ObjectEvent *objectEvent;
  struct MapGridScrollWork *grid;
  struct MapGridLayerScroll *layer;
  struct FieldWork *work;
  s32 warpDest;
  s32 leaderLocalId;

  tret = ObjectEvent_GetById(LOCALID_TRET);
  // Point the cocoon latch at the map state and save the palette first.
  sLastCocoon = (s16 *)gMapLocalState;
  TretPalette_SaveMapColors();
  tret->motionFlags = 0;
  tret->y.fixed = -PIXELS(10);
  {
    struct ObjectEvent *tretSpeaker = ObjectEvent_GetById(LOCALID_SECOND_TRET);

    tretSpeaker->motionFlags = 0;
    tretSpeaker->y.fixed = -PIXELS(10);
  }
  ObjectGroup_ConfigureChildValue(LOCALID_SECOND_TRET, 15);
  TretTreeInteriors_PlayGesture(TRET_GESTURE_CLOSE_EYES);
  if (gSaveBlock.warpDest != TRET_TREE_INTERIORS_AFTER_FIGHT_TRET)
    FieldScheduler_AddCallback(TretPalette_UpdateMapTint, TASK_PRIORITY_MAP);
  if (GameFlag_Get(FLAG_TRET_DEFEATED)) {
    Engine_ObjectEventSetPosition(LOCALID_SECOND_TRET, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_TRET, 0, 0);
  }
  if (Script_GetFlag(FLAG_KEEP_PARTY_POSITION))
    TretTreeInteriors_ApplyFlaggedMapPatches();
  grid = HEAP_SLOT_BLOCK(8);
  // Match the first layer's parallax offset and scale to the room bounds.
  layer = &grid->layers[0];
  layer->offsetX += Iwram_MulQ16(grid->minX + PIXELS(160), 0x1999);
  layer->offsetY += Iwram_MulQ16(grid->minY + PIXELS(136), 0x1999);
  layer->scaleX = 0xe666;
  layer->scaleY = 0xe666;
  // The layout draws four cocoons already torn: the first step on them breaks
  // through.
  GameFlag_Set(FLAG_MAP_TEMP_01);
  GameFlag_Set(FLAG_MAP_TEMP_0D);
  GameFlag_Set(FLAG_MAP_TEMP_0F);
  GameFlag_Set(FLAG_MAP_TEMP_13);
  WaitFrames(1);
  TretTreeInteriors_ApplyPostBattleState(0);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  work->transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  warpDest = gSaveBlock.warpDest;
  leaderLocalId = gSaveBlock.leaderLocalId;
  objectEvent = ObjectEvent_GetById(leaderLocalId);
  // Warp destinations 20, 30, 40 and 50: the leader falls on through the room,
  // to the warp destination numbered ten lower. Warp destination 10: the leader
  // lands in a cloud of dust in front of Tret.
  // TRET_TREE_INTERIORS_AFTER_FIGHT_TRET: back from the battle with Tret.
  if (warpDest == 50 || warpDest == 40 || warpDest == 30 || warpDest == 20) {
    FieldScript_StartScreenTransition();
    OBJECT_ANIMATION(leaderLocalId, ANIM_LEADER_SPIN_AWAY);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(leaderLocalId), 0);
    ObjectEvent_SetAttachedEffect(leaderLocalId, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ATTACH);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    objectEvent->motionFlags = OBJ_EVENT_FALLS;
    objectEvent->y.fixed = PIXELS(100);
    objectEvent->groundHeight = -PIXELS(160);
    objectEvent->gravity = PIXELS(1) / 2;
    SE(SE_204);
    FieldScript_SetDestination((s32)&MapId_TretTreeInteriors, warpDest - 10);
    WAIT(20);
    objectEvent->layer = 2;
    ObjectMotion_SetActionVariant(leaderLocalId, 3);
    WAIT(2);
    FieldEffect_ConfigureLinkedObject(
        leaderLocalId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    WAIT(8);
  } else if (warpDest == 10) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      TretTreeInteriors_RunLandingDustScene();
  } else if (warpDest == TRET_TREE_INTERIORS_AFTER_FIGHT_TRET) {
    TretTreeInteriors_ApplyPostBattleState(1);
  }
  return 0;
}

/// Saves background palettes 0 to 13 as the base for Tret's map tint.
static void TretPalette_SaveMapColors(void) {
  Dma_Set((const void *)BG_PLTT, gPaletteFadeWork->palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 112, REG_DMA3);
}

/// Tints background colours 16 to 78 every thirty-two frames, leaving
/// palette 4's last colour alone. Each later palette receives a weaker tint.
static void TretPalette_UpdateMapTint(void) {
  u16 *sourceColors;
  u16 *destinationColors;
  u32 colorIndex;
  s32 redStep;
  s32 greenStep;
  s32 blueStep;
  s32 red;
  s32 green;
  s32 blue;
  u32 color;
  struct FieldWork *work;
  struct PaletteFade *paletteWork;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  paletteWork = HEAP_SLOT_BLOCK(HEAP_SLOT_PALETTE_FADE);
  sourceColors = paletteWork->palette;
  if (work->psynergy != 0)
    return;
  if ((gFrameCount & 31) != 0)
    return;
  sourceColors += 16;
  destinationColors = (u16 *)&BG_PLTT_COLOR(1, 0);
  colorIndex = 0;
  for (; colorIndex < TRET_TINT_COLOR_COUNT; colorIndex++, sourceColors++) {
    redStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex];
    greenStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex + 1];
    blueStep = gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex + 2];
    if (colorIndex > 47) {
      redStep -= redStep / 2 + redStep / 3;
      greenStep -= greenStep / 2 + greenStep / 3;
      blueStep -= blueStep / 2 + blueStep / 3;
    } else if (colorIndex > 31) {
      redStep -= redStep / 3 + redStep / 4;
      greenStep -= greenStep / 3 + greenStep / 4;
      blueStep -= blueStep / 3 + blueStep / 4;
    } else if (colorIndex > 15) {
      redStep -= redStep / 4 + redStep / 5;
      greenStep -= greenStep / 4 + greenStep / 5;
      blueStep -= blueStep / 4 + blueStep / 5;
    }
    color = *sourceColors;
    red = color & 31;
    green = (color >> 5) & 31;
    blue = (color >> 10) & 31;
    red += redStep;
    green += greenStep;
    blue += blueStep;
    if (red > TRET_TINT_CHANNEL_MAX)
      red = TRET_TINT_CHANNEL_MAX;
    if (green > TRET_TINT_CHANNEL_MAX)
      green = TRET_TINT_CHANNEL_MAX;
    if (blue > TRET_TINT_CHANNEL_MAX)
      blue = TRET_TINT_CHANNEL_MAX;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *destinationColors++ = (blue << 10) | (green << 5) | red;
  }
  gTretTreeInteriors_TintIndex += (Engine_RandomNext() & 7) * 3;
  if (gTretTreeInteriors_TintSteps[gTretTreeInteriors_TintIndex] ==
      TRET_TINT_END)
    gTretTreeInteriors_TintIndex = 0;
}

/// Plays a Tret animation sequence with its original pauses.
static void TretTreeInteriors_PlayGesture(s32 gesture) {
  switch (gesture) {
  case TRET_GESTURE_CLOSE_EYES:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_CLOSED);
    break;
  case TRET_GESTURE_OPEN_EYES:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    break;
  case TRET_GESTURE_LOOK_LEFT:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_LOOK_LEFT);
    break;
  case TRET_GESTURE_LOOK_RIGHT:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_LOOK_RIGHT);
    break;
  case TRET_GESTURE_BLINK:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_CLOSED);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_CLOSED);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    break;
  case TRET_GESTURE_HALF_CLOSE_EYES:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_HALF_CLOSED);
    break;
  case TRET_GESTURE_WRATHFUL_CLOSE_EYES:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
    break;
  case TRET_GESTURE_WRATHFUL_OPEN_EYES:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_GESTURE_WRATHFUL_LOOK_RIGHT:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
    break;
  case TRET_GESTURE_WRATHFUL_LOOK_LEFT:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
    break;
  case TRET_GESTURE_WRATHFUL_BLINK:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
    WaitFrames(6);
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  case TRET_GESTURE_WRATHFUL_STARE:
    OBJECT_ANIMATION(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
    break;
  }
  WaitFrames(12);
}

/// Moves a fading spark outward from its saved origin, then releases it.
static void TretTreeInteriors_UpdateSwirlSpark(union FieldObject *fieldObject) {
  struct ObjectEvent *spark = (struct ObjectEvent *)fieldObject;
  struct FieldPosition position;
  s32 frame;

  frame = spark->timer;
  if (frame <= SWIRL_SPARK_LAST_FRAME) {
    position.x = spark->targetX;
    position.y = spark->targetY;
    position.z = spark->targetZ;
    {
      s32 initialAngle = (s16)spark->work;

      Vector_AddPolarOffset(frame << 16, ((frame * 3) << 8) + initialAngle,
                            &position);
    }
    spark->x.fixed = position.x;
    spark->y.fixed = position.y;
    spark->z.fixed = position.z;
    if (spark->timer <= SWIRL_SPARK_SHRINK_LAST_FRAME) {
      spark->scaleX += -0x51e;
      spark->scaleY += -0x51e;
    }
    spark->timer++;
  } else {
    VramCache_FreeEntry(spark->sprite->slot);
    ObjectEvent_Release(spark);
  }
}

/// Emits five shrinking waves of fading sparks from the saved origin.
static void TretTreeInteriors_SpawnSwirlSparks(void) {
  struct ObjectEvent *spark;
  u32 frame;
  void *previousResourceEntry;
  s32 wave;
  u32 sparkIndex;

  frame = sSparkFrame;
  previousResourceEntry = 0;
  wave = (s32)frame / 10;
  switch (frame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    SE(SE_220);
    // Waves contain 6, 5, 4, 3 and 2 sparks. Divide degrees before
    // converting them to turns, preserving the original rounding.
    for (sparkIndex = 0; sparkIndex < 6 - wave; sparkIndex++) {
      spark = ObjectEvent_Create(SPRITE_SPARKLE, sSparkOrigin.x, sSparkOrigin.y,
                                 sSparkOrigin.z);
      if (spark != 0) {
        previousResourceEntry =
            Sprite_ShareVramCacheEntry(spark->sprite, previousResourceEntry);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 0;
        ObjectEvent_SetSpriteFlags(spark, 0);
        ObjectDispatch_SelectAnimation(spark, 1);
        spark->timer = 0;
        spark->work = ((360 / (u32)(6 - wave) * sparkIndex) << 16) / 360;
        spark->targetX = sSparkOrigin.x;
        spark->targetY = sSparkOrigin.y;
        spark->targetZ = sSparkOrigin.z;
        spark->speed = 0x19999;
        spark->update = TretTreeInteriors_UpdateSwirlSpark;
      }
    }
    // Each wave also plays the closing sound.
    // fall through
  case 44:
    SE(SONG_FADE_OUT_ACTION_SE);
    break;
  }
  if (++sSparkFrame > 120) {
    sSparkFrame = 0;
  }
}

/// Redraws each torn cocoon whose flag is set, as the party left it before a
/// battle.
static void TretTreeInteriors_ApplyFlaggedMapPatches(void) {
  s32 patchOffset;

  for (patchOffset = 0; gTretTreeInteriors_MapPatches[patchOffset] != -1;
       patchOffset += 6) {
    if (GameFlag_Get(gTretTreeInteriors_MapPatches[patchOffset]) &&
        gTretTreeInteriors_MapPatches[patchOffset + 1] != 0)
      MapGrid_CopyCellsTo(gTretTreeInteriors_MapPatches[patchOffset + 2],
                          gTretTreeInteriors_MapPatches[patchOffset + 3],
                          gTretTreeInteriors_MapPatches[patchOffset + 4],
                          gTretTreeInteriors_MapPatches[patchOffset + 5], 1, 1);
  }
}

/// Casting Reveal in Tret Tree (PSYNERGY_PHASE_EFFECT_ACTS) stops the map
/// tint until the map is entered again.
static void EventScript_CastReveal(void) {
  Task_Remove(TretPalette_UpdateMapTint);
}

#include "common/effect_advance.h"

/// Warp destination 10, at the end of the fall from cocoon 50: the leader drops
/// in from 130 pixels up in front of Tret, lands in a ring of dust and the map
/// shakes.
static void TretTreeInteriors_RunLandingDustScene(void) {
  struct EffectOptions options;
  struct FieldPosition velocity;
  struct EffectOptions *particleOptions;
  struct FieldPosition *particleVelocity;
  struct ObjectEvent *leader;
  u32 particleIndex;
  s32 velocityX;
  s32 velocityZ;

  SCRIPT_START();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed = PIXELS(130);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->gravity = 0x8000;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->bounce = 0;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  SE(SE_204);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags =
      OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  WAIT(24);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  // Spread particles around the leader while the landing effect finishes.
  particleOptions = &options;
  particleOptions->palette = 7;
  particleOptions->update = FieldEffect_AdvanceMotion;
  particleOptions->startScaleX = 0xcccc;
  particleOptions->startScaleY = 0xcccc;
  for (particleIndex = 0; particleIndex <= 16; particleIndex++) {
    particleVelocity = &velocity;
    particleVelocity->x = Trig_Cos(particleIndex << 12);
    particleVelocity->y = 0;
    velocityZ = Trig_Sin(particleIndex << 12);
    velocityX = particleVelocity->x;
    particleVelocity->z = velocityZ;
    velocityX += velocityX / 2;
    particleVelocity->x = velocityX;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocityX, particleVelocity->y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      particleOptions);
  }
  SE(SE_188);
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_ConfigureLinkedObject(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->gravity = 0x10000;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->bounce = 0x4000;
  SCRIPT_END();
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

/// The torn cocoons redrawn after a battle: the cocoon's flag, whether it is
/// redrawn (0 for the four the layout draws torn), the staging cell x/y and the
/// cocoon's cell x/y.
// clang-format off
s16 gTretTreeInteriors_MapPatches[] = {
    FLAG_MAP_TEMP_00, 1, 64, 35, 113, 6,
    FLAG_MAP_TEMP_01, 0, 65, 35, 115, 6,
    FLAG_MAP_TEMP_02, 1, 66, 35, 110, 8,
    FLAG_MAP_TEMP_03, 1, 67, 35, 110, 10,
    FLAG_MAP_TEMP_04, 1, 68, 35, 111, 12,
    FLAG_MAP_TEMP_05, 1, 69, 35, 113, 12,
    FLAG_MAP_TEMP_06, 1, 70, 35, 112, 14,
    FLAG_MAP_TEMP_07, 1, 71, 35, 114, 14,
    FLAG_MAP_TEMP_08, 1, 72, 35, 116, 14,
    FLAG_MAP_TEMP_09, 1, 73, 35, 71, 23,
    FLAG_MAP_TEMP_0A, 1, 74, 35, 77, 24,
    FLAG_MAP_TEMP_0C, 1, 75, 35, 112, 24,
    FLAG_MAP_TEMP_0D, 0, 76, 35, 114, 24,
    FLAG_MAP_TEMP_0E, 1, 77, 35, 116, 24,
    FLAG_MAP_TEMP_0F, 0, 78, 35, 112, 26,
    FLAG_MAP_TEMP_0B, 1, 79, 35, 114, 26,
    FLAG_MAP_TEMP_10, 1, 80, 35, 116, 26,
    FLAG_MAP_TEMP_11, 1, 81, 35, 112, 28,
    FLAG_MAP_TEMP_12, 1, 82, 35, 114, 28,
    FLAG_MAP_TEMP_13, 0, 83, 35, 116, 28,
    FLAG_MAP_TEMP_14, 1, 84, 35, 118, 28,
    -1, 0,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

/// The party gathers in front of Tret before the battle (EventScript_FightTret),
/// each walking to its own place.
s32 gTretTreeInteriors_LeaderGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTretTreeInteriors_GaretGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(148), 0, PIXELS(90)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTretTreeInteriors_IvanGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(182), 0, PIXELS(90)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

s32 gTretTreeInteriors_MiaGathersActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(104)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// Tret's face sinks away after the conversation: eight times it shows 80
/// pixels up for 4 frames and drops back 78, shrinking a sixteenth each time,
/// and then it is put away.
s32 gTretTreeInteriors_FaceSinksActions[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(80)),
    SC_WAIT(4),
    SC_ADD(SW_Y, -PIXELS(78)),
    SC_ADD(SW_SCALE_X, -0x1000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Garet, Ivan and Mia walk back to the leader and are put away.
s32 gTretTreeInteriors_RejoinLeaderActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(166), 0, PIXELS(80)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gTretTreeInteriors_TintSteps[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 2, 1,
    1, 2, 1, 1, 2, 1, 1, 2,
    1, 1, 2, 2, 1, 2, 2, 1,
    2, 2, 2, 3, 2, 2, 3, 2,
    2, 3, 3, 2, 3, 3, 2, 3,
    3, 2, 3, 3, 2, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    4, 3, 3, 4, 3, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 3, 4, 4, 3, 4, 4,
    3, 4, 4, 3, 4, 4, 3, 4,
    4, 3, 4, 4, 3, 4, 4, 3,
    4, 4, 3, 4, 4, 3, 4, 3,
    3, 4, 3, 3, 4, 3, 3, 4,
    3, 3, 4, 3, 3, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 3, 3, 2, 3, 3, 2,
    3, 3, 2, 3, 2, 2, 3, 2,
    2, 2, 2, 2, 2, 2, 1, 2,
    2, 1, 2, 1, 1, 2, 1, 1,
    2, 1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 0, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99,
};

struct MapWarpDest gTretTreeInteriorsWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { TRET_TREE_INTERIORS_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 488, 0, 248, FACING_NORTH, 0, 352, 0, 640, 272, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, CONDITION_ALWAYS, 712, 0, 152, FACING_EAST, 0, 672, 0, 944, 272, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, CONDITION_ALWAYS, 920, 0, 152, FACING_WEST, 0, 672, 0, 944, 272, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTH, CONDITION_ALWAYS, 168, 0, 520, FACING_NORTH, 0, 32, 272, 304, 544, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_SOUTHEAST, CONDITION_ALWAYS, 296, 0, 424, FACING_WEST, 0, 32, 272, 304, 544, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTHWEST, CONDITION_ALWAYS, 376, 0, 424, FACING_EAST, 0, 352, 272, 640, 544, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH, CONDITION_ALWAYS, 488, 0, 520, FACING_NORTH, 0, 352, 272, 640, 544, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHWEST, CONDITION_ALWAYS, 712, 0, 424, FACING_EAST, 0, 672, 272, 960, 544, 0 },
    { TRET_TREE_INTERIORS_FROM_TRET_TREE_BRANCH_NORTH_NORTHEAST, CONDITION_ALWAYS, 920, 0, 424, FACING_WEST, 0, 672, 272, 960, 544, 0 },
    { 10, CONDITION_ALWAYS, 168, 0, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TRET_TREE_INTERIORS_ENTRANCE_FROM_SECOND_FLOOR, CONDITION_ALWAYS, 424, 24, 72, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TRET_TREE_INTERIORS_SECOND_FLOOR_FROM_ENTRANCE, CONDITION_ALWAYS, 744, 0, 88, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { TRET_TREE_INTERIORS_THIRD_FLOOR_FROM_FOURTH_FLOOR, CONDITION_ALWAYS, 232, 24, 344, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { TRET_TREE_INTERIORS_FOURTH_FLOOR_FROM_THIRD_FLOOR, CONDITION_ALWAYS, 552, 0, 360, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { TRET_TREE_INTERIORS_TRET_FROM_ENTRANCE, CONDITION_ALWAYS, 168, 24, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { TRET_TREE_INTERIORS_ENTRANCE_FROM_TRET, CONDITION_ALWAYS, 488, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 21, CONDITION_ALWAYS, 472, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 22, CONDITION_ALWAYS, 504, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 23, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 24, CONDITION_ALWAYS, 424, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 25, CONDITION_ALWAYS, 440, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 26, CONDITION_ALWAYS, 488, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 27, CONDITION_ALWAYS, 456, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 28, CONDITION_ALWAYS, 488, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 29, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { 31, CONDITION_ALWAYS, 760, 0, 104, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 32, CONDITION_ALWAYS, 856, 0, 120, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 51, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 52, CONDITION_ALWAYS, 488, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 53, CONDITION_ALWAYS, 520, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 54, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 55, CONDITION_ALWAYS, 520, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 56, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 57, CONDITION_ALWAYS, 488, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 58, CONDITION_ALWAYS, 520, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 59, CONDITION_ALWAYS, 552, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 50, CONDITION_ALWAYS, 488, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
    { 40, CONDITION_ALWAYS, 168, 0, 424, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
    { 30, CONDITION_ALWAYS, 808, 0, 152, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
    { 20, CONDITION_ALWAYS, 488, 0, 152, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
    { TRET_TREE_INTERIORS_AFTER_FIGHT_TRET, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
    { MAP_TABLE_END },
};

struct MapWarpRegion gTretTreeInteriorsRegions[] = {
    { 420, 28, 68, 428, 36, 76, -1, TRET_TREE_INTERIORS_ENTRANCE_TO_SECOND_FLOOR },
    { 228, 28, 338, 236, 36, 346, -1, TRET_TREE_INTERIORS_THIRD_FLOOR_TO_FOURTH_FLOOR },
#if !TBS_PATCH_3
    { 163, 28, 148, 171, 36, 156, -1, TRET_TREE_INTERIORS_TRET_TO_ENTRANCE },
#else
    { 159, 24, 144, 175, 40, 160, -1, TRET_TREE_INTERIORS_TRET_TO_ENTRANCE },
#endif
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gTretTreeInteriorsWarps[] = {
    MAP_WARPS(TretTreeInteriors),
    MAP_WARP(TRET_TREE_INTERIORS_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_TRET_TREE_INTERIORS),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHWEST),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH_SOUTHEAST),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTH),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_SOUTHEAST),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTHWEST),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHWEST),
    MAP_WARP(TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST, TretTreeBranch, TRET_TREE_BRANCH_FROM_TRET_TREE_INTERIORS_NORTH_NORTHEAST),
    MAP_WARP(TRET_TREE_INTERIORS_ENTRANCE_TO_SECOND_FLOOR, TretTreeInteriors, TRET_TREE_INTERIORS_SECOND_FLOOR_FROM_ENTRANCE),
    MAP_WARP(TRET_TREE_INTERIORS_SECOND_FLOOR_TO_ENTRANCE, TretTreeInteriors, TRET_TREE_INTERIORS_ENTRANCE_FROM_SECOND_FLOOR),
    MAP_WARP(TRET_TREE_INTERIORS_THIRD_FLOOR_TO_FOURTH_FLOOR, TretTreeInteriors, TRET_TREE_INTERIORS_FOURTH_FLOOR_FROM_THIRD_FLOOR),
    MAP_WARP(TRET_TREE_INTERIORS_FOURTH_FLOOR_TO_THIRD_FLOOR, TretTreeInteriors, TRET_TREE_INTERIORS_THIRD_FLOOR_FROM_FOURTH_FLOOR),
    MAP_WARP(TRET_TREE_INTERIORS_TRET_TO_ENTRANCE, TretTreeInteriors, TRET_TREE_INTERIORS_ENTRANCE_FROM_TRET),
    MAP_WARP(TRET_TREE_INTERIORS_ENTRANCE_TO_TRET, TretTreeInteriors, TRET_TREE_INTERIORS_TRET_FROM_ENTRANCE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTretTreeInteriorsObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET_RAISED, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gTretTreeInteriorsEvents[] = {
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_SOUTH, 1, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_KOLIMA_FOREST_TREE},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_WEST, 2, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHWEST},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_EAST, 3, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH_SOUTHEAST},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_SOUTH, 4, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTH},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_EAST, 5, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_SOUTHEAST},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_WEST, 6, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTHWEST},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_SOUTH, 7, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_WEST, 8, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHWEST},
    {MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_EAST, 9, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_TO_TRET_TREE_BRANCH_NORTH_NORTHEAST},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_SECOND_FLOOR_TO_ENTRANCE},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_FOURTH_FLOOR_TO_THIRD_FLOOR},
    {MAP_EVENT_WARP, 15, CONDITION_ALWAYS,
     TRET_TREE_INTERIORS_ENTRANCE_TO_TRET},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 23, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 26, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 27, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 28, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 29, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 55, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 56, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 57, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 58, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 59, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_ResetCocoonLatch)},
    {MAP_EVENT_COORD, 21, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon21)},
    {MAP_EVENT_COORD, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon22)},
    {MAP_EVENT_COORD, 23, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon23)},
    {MAP_EVENT_COORD, 24, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon24)},
    {MAP_EVENT_COORD, 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon25)},
    {MAP_EVENT_COORD, 26, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon26)},
    {MAP_EVENT_COORD, 27, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon27)},
    {MAP_EVENT_COORD, 28, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon28)},
    {MAP_EVENT_COORD, 29, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon29)},
    {MAP_EVENT_COORD, 31, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon31)},
    {MAP_EVENT_COORD, 32, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon32)},
    {MAP_EVENT_COORD, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon50)},
    {MAP_EVENT_COORD, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon51)},
    {MAP_EVENT_COORD, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon52)},
    {MAP_EVENT_COORD, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon53)},
    {MAP_EVENT_COORD, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon54)},
    {MAP_EVENT_COORD, 55, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon55)},
    {MAP_EVENT_COORD, 56, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon56)},
    {MAP_EVENT_COORD, 57, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon57)},
    {MAP_EVENT_COORD, 58, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon58)},
    {MAP_EVENT_COORD, 59, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_StepOnCocoon59)},
    {MAP_EVENT_COORD, 40, FLAG_TRET_DEFEATED,
     EVENT_SCRIPT(EventScript_FightTret)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CastReveal)},
    {BG_EVENT(BG_EVENT_CHEST), 100, 0xf5b, FIND_ITEM(ITEM_HEALING_RING)},
    {BG_EVENT(BG_EVENT_CHEST), 101, 0xf5c, FIND_ITEM(ITEM_NUT)},
    {BG_EVENT(BG_EVENT_SILENT), 102, 0x350, FIND_PSYNERGY_STONE},
    {MAP_EVENTS_END},
};

s32 gTretTreeInteriors_TintIndex = 0;
