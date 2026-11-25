/// @file
/// Trial Road, the mountain path above the Shaman Village where Moapa tests
/// the party before he will hand over the Hover Jade. Felix races Moapa and his
/// two knights up four stages, by the left path or the right. Each stage is
/// one of eight puzzle rooms (logs to roll, stakes to Pound, weeds to blow
/// away, frozen puddles, crumbling pillars, fountains, a sand stream), dealt in
/// an order that depends on the letters of the leader's name. At the top of
/// each stage the switch in front of the door opens the blue chests beside it,
/// and the door opens once each open chest holds a piece of the party's
/// equipment: one chest, or two when Moapa got there first. A button at the
/// top offers to give up. Moapa's party waits at the summit, or arrives after
/// the party, and fights them for the Jade. Long after, Hover on the summit's
/// purple tile reaches a cave whose rock Lift raises, and beyond it a pool
/// with weeds hiding a Mad Plant, a hidden tile Reveal shows, an Elixir chest
/// and Gasp the Djinni, who moves opposite to Felix.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "sprite_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/field_state.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/window_sprite_list.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/ground_effects.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/serial.h"
#include "system/system.h"
#include "system/task.h"
#include "system/vector3.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "text/msg_ids.h"

/// The map cells the trigger tables answer, by the behaviour the map gives
/// them. The race's frame, the same in every stage: the stairs at the foot of
/// each path and the door at its top, the give-up buttons and the switches
/// before the doors (west and east of the map's middle), each with the ring of
/// cells around it that steps off it.
enum TrialRoadCell {
  CELL_LEFT_PATH_STAIRS_DOWN = 1,
  CELL_RIGHT_PATH_STAIRS_DOWN = 2,
  CELL_LEFT_PATH_DOOR = 3,
  CELL_RIGHT_PATH_DOOR = 4,
  CELL_AROUND_WEST_GIVE_UP_BUTTON = 10,
  CELL_WEST_GIVE_UP_BUTTON = 11,
  CELL_AROUND_EAST_GIVE_UP_BUTTON = 20,
  CELL_EAST_GIVE_UP_BUTTON = 21,
  CELL_AROUND_WEST_SWITCH = 30,
  CELL_WEST_SWITCH = 31,
  CELL_AROUND_EAST_SWITCH = 40,
  CELL_EAST_SWITCH = 41,
  // The pillar stage: the south edge of the upper ledge, over the gap a
  // pillar can be pushed into, and the edge beside it.
  CELL_PILLAR_STAGE_GAP = 51,
  CELL_PILLAR_STAGE_LEDGE = 52,
  // The torch stage: north and south of each pillar of ice.
  CELL_ICE_PILLAR_SIDE = 33,
  // The weeds stage's ledge, jumped from southward.
  CELL_WEEDS_STAGE_LEDGE = 33,
  // The Pound stage's four stakes, and the ground the pillars stand on.
  CELL_WEST_STAKE = 50,
  CELL_INNER_WEST_STAKE = 51,
  CELL_INNER_EAST_STAKE = 52,
  CELL_EAST_STAKE = 53,
  CELL_STAKE_GROUND = 60,
  // The crumbling stage: its ledge, the cells beside the cracked pillars, and
  // the pillars themselves.
  CELL_CRUMBLING_STAGE_LEDGE = 33,
  CELL_BESIDE_CRUMBLING_PILLAR = 34,
  CELL_WEST_CRUMBLING_PILLAR = 50,
  CELL_EAST_CRUMBLING_PILLAR = 51,
  // The log stage: its ledge over the water, the rows the upper and lower logs
  // roll along, and the two ends of the east log's track.
  CELL_LOG_STAGE_LEDGE = 33,
  CELL_UPPER_LOG_ROWS = 50,
  CELL_LOWER_LOG_ROWS = 51,
  CELL_EAST_LOG_NORTH_END = 52,
  CELL_EAST_LOG_SOUTH_END = 53,
  // The fountain stage: the column the block can stand in, and the three
  // buried vents.
  CELL_BLOCK_TRACK = 33,
  CELL_WEST_VENT = 50,
  CELL_MIDDLE_VENT = 51,
  CELL_EAST_VENT = 52,
  // The sand stage's stream.
  CELL_STREAM = 50,
  // Gasp's pool: the weeds, and the clump hiding the Mad Plant.
  CELL_WEEDS = 33,
  CELL_WEEDS_HIDING_MAD_PLANT = 34,
  // The summit: in front of each doorway from the stages, and the cells
  // that bring on Moapa.
  CELL_SUMMIT_DOORWAY = 33,
  CELL_LEFT_PATH_SUMMIT = 40,
  CELL_RIGHT_PATH_SUMMIT = 41,
};

/// Trial Road's Mad Plant, the last of the five hidden in weeds: its
/// FLAG_MAD_PLANT and its column of ENCOUNTER_ZONE_MAD_PLANTS.
#define MAD_PLANT_TRIAL_ROAD 4
/// Set once the summit's stone pillar has been moved one cell east, into
/// column 8 of the map, where it stays.
#define FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED 0x9fc

/// Set while Felix stands up on the pillar stage's upper ledge or a pillar's
/// top (TrialRoad_CheckLeaderOnPillars).
#define FLAG_LEADER_ON_PILLARS FLAG_SCENE_TEMP_08
/// Set once the give-up button has asked; stepping off it clears it.
#define FLAG_GIVE_UP_ASKED FLAG_SCENE_TEMP_81
/// Set once both blue chests hold their items and the leader's door opens.
#define FLAG_STAGE_DOOR_OPEN FLAG_SCENE_TEMP_82
/// Set once Felix has stood on the switch at the top of the stage and the
/// blue chests have opened (TrialRoad_OpenChests).
#define FLAG_STAGE_CHESTS_OPENED FLAG_SCENE_TEMP_84
/// Set while the inner blue chest stands open for an item.
#define FLAG_INNER_CHEST_OPEN FLAG_SCENE_TEMP_85
/// Set while the outer blue chest stands open for an item: it opens only when
/// Moapa reached the door first.
#define FLAG_OUTER_CHEST_OPEN FLAG_SCENE_TEMP_86
/// Set once the doorway Felix came out of at the summit has shut behind him
/// (TrialRoad_ShutSummitDoorway).
#define FLAG_SUMMIT_DOORWAY_SHUT FLAG_SCENE_TEMP_83
/// Set once Lift has raised the rock at the summit cave's mouth.
#define FLAG_CAVE_ROCK_LIFTED FLAG_SCENE_TEMP_89
/// Set once Gasp has stopped mirroring Felix and hopped away to where he can
/// be talked to.
#define FLAG_GASP_FLED FLAG_SCENE_TEMP_88
/// The link race's flags, which no code in the game sets, as nothing chooses
/// RIVAL_LINKED: the
/// byte of flags from FLAG_LINK_PLAYER holds this player's number in the link,
/// so the rival follows the other player's record; while
/// FLAG_LINKED_RIVAL_HALTS is set, a rival whose record has its top byte set
/// stops 12 pixels beside the course's corner, 16 pixels north of it with the
/// far side's timer if FLAG_LINKED_RIVAL_ON_FAR_SIDE is set and 16 south with
/// the near side's if not.
#define FLAG_LINK_PLAYER FLAG_SCENE_TEMP_F8
#define FLAG_LINKED_RIVAL_HALTS FLAG_SCENE_TEMP_11
#define FLAG_LINKED_RIVAL_ON_FAR_SIDE FLAG_SCENE_TEMP_12
/// Set once Whirlwind has blown away the weeds on the pillar stage's pillar C,
/// on the Pound stage's pillar B, and on the sand stage's cliff.
#define FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY FLAG_AREA_TEMP_07
#define FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY FLAG_AREA_TEMP_39
#define FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY FLAG_AREA_TEMP_74
/// Set while the Pound stage's inner west stake stands raised and the west one
/// sunk; clear the other way round.
#define FLAG_INNER_WEST_STAKE_RAISED FLAG_AREA_TEMP_35
/// Set while the Pound stage's east stake stands raised and the inner east one
/// sunk; clear the other way round.
#define FLAG_EAST_STAKE_RAISED FLAG_AREA_TEMP_37
/// The four stages' flags, from FLAG_AREA_TEMP_81: each set once Moapa reached
/// that stage's door before Felix.
#define FLAG_RIVAL_FIRST_AT_DOOR_BASE FLAG_AREA_TEMP_81
/// Each cracked pillar of the crumbling stage: set once Felix has first
/// stepped on it and it has flashed, and once it has crumbled away.
#define FLAG_WEST_CRUMBLING_PILLAR_STEPPED_ON FLAG_AREA_TEMP_42
#define FLAG_WEST_CRUMBLING_PILLAR_CRUMBLED FLAG_AREA_TEMP_43
#define FLAG_EAST_CRUMBLING_PILLAR_STEPPED_ON FLAG_AREA_TEMP_44
#define FLAG_EAST_CRUMBLING_PILLAR_CRUMBLED FLAG_AREA_TEMP_45
/// Set once Felix climbs to the summit ahead of Moapa.
#define FLAG_PARTY_FIRST_AT_PEAK FLAG_AREA_TEMP_85
/// Set once the stages' order has been dealt from the leader's name.
#define FLAG_STAGE_ORDER_DEALT FLAG_AREA_TEMP_80

#include "shared/trial_road.h"

/// The actors every stage room places first, numbering the rows of each
/// stage's table: Moapa, the four blue chests (two on the left path, two on
/// the right, inner and outer) and Moapa's two knights. A stage's own puzzle
/// pieces follow from ACTOR_FIRST_STAGE_PIECE.
enum TrialRoadStageActorId {
  ACTOR_MOAPA = ACTOR_FIRST_PLACED,
  ACTOR_LEFT_INNER_CHEST,
  ACTOR_LEFT_OUTER_CHEST,
  ACTOR_RIGHT_INNER_CHEST,
  ACTOR_RIGHT_OUTER_CHEST,
  ACTOR_LEFT_KNIGHT,
  ACTOR_RIGHT_KNIGHT,
  ACTOR_FIRST_STAGE_PIECE
};

/// Kraden, at the summit (gTrialRoad_SummitPlacements), after Moapa.
#define ACTOR_KRADEN (ACTOR_MOAPA + 1)

/// The log stage's three logs (gTrialRoad_LogStagePlacements): the upper and
/// lower logs lie north to south and roll east or west, and the east log lies
/// across them and rolls north or south.
enum TrialRoadLogStageActorId {
  ACTOR_UPPER_LOG = ACTOR_FIRST_STAGE_PIECE + 1,
  ACTOR_LOWER_LOG,
  ACTOR_EAST_LOG
};

/// The Pound stage's four cone-shaped stakes (gTrialRoad_PoundStagePlacements),
/// west to east at cells (13, 26), (15, 26), (17, 26) and (19, 26), in two
/// pairs: driving one down raises its partner.
enum TrialRoadPoundStageActorId {
  ACTOR_WEST_STAKE = ACTOR_FIRST_STAGE_PIECE + 3,
  ACTOR_INNER_WEST_STAKE,
  ACTOR_INNER_EAST_STAKE,
  ACTOR_EAST_STAKE
};

/// The crumbling stage's two pillars (gTrialRoad_CrumblingStagePlacements).
enum TrialRoadCrumblingStageActorId {
  ACTOR_WEST_CRUMBLING_PILLAR = ACTOR_FIRST_STAGE_PIECE,
  ACTOR_EAST_CRUMBLING_PILLAR
};

/// The stone pillars most stages stand from ACTOR_FIRST_STAGE_PIECE: five, or
/// six in the torch and Pound stages; the log and fountain stages have one,
/// the block.
enum TrialRoadPillarActorId {
  ACTOR_PILLAR_A = ACTOR_FIRST_STAGE_PIECE,
  ACTOR_PILLAR_B,
  ACTOR_PILLAR_C,
  ACTOR_PILLAR_D,
  ACTOR_PILLAR_E,
  ACTOR_PILLAR_F
};

/// The torch stage's two torches burning on stands, after its six pillars
/// (gTrialRoad_TorchStagePlacements): pushed next to a frozen puddle, a torch
/// melts it.
enum TrialRoadTorchStageActorId {
  ACTOR_WEST_TORCH = ACTOR_FIRST_STAGE_PIECE + 6,
  ACTOR_EAST_TORCH
};

/// The weeds after the pillars: on pillar C in the pillar stage, on pillars A
/// and B in the weeds pillar stage.
enum { ACTOR_WEEDS_A = ACTOR_FIRST_STAGE_PIECE + 5, ACTOR_WEEDS_B };

/// The weeds on pillar B in the Pound stage, after its four stakes.
enum { ACTOR_POUND_STAGE_WEEDS = ACTOR_EAST_STAKE + 1 };

/// The weeds on the sand stage's cliff, after its two tightrope posts.
enum { ACTOR_SAND_STAGE_WEEDS = ACTOR_FIRST_STAGE_PIECE + 2 };

/// The pillar at the summit (gTrialRoad_SummitPlacements), after Kraden.
#define ACTOR_PEAK_PILLAR (ACTOR_KRADEN + 1)

/// Gasp, alone in his room (gTrialRoad_GaspPlacements).
enum { ACTOR_GASP = ACTOR_FIRST_PLACED };

/// The round boulder on the ledge of the cave beyond the summit, which Lift
/// raises (gTrialRoad_CaveRockPlacements).
enum { ACTOR_CAVE_ROCK = ACTOR_FIRST_PLACED };

/// The two parts of the cave beyond the summit, each way between them leading
/// in by the entrance on its far side: the west cave, where the summit's
/// passage comes in and the boulder sits on its ledge, and the east passage
/// north to Gasp's pool.
enum {
  TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE = 1,
  TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE = 2
};
/// The entrances, one for each way above.
enum {
  TRIAL_ROAD_CAVE_ROCK_WEST_CAVE_FROM_EAST_PASSAGE = 1,
  TRIAL_ROAD_CAVE_ROCK_EAST_PASSAGE_FROM_WEST_CAVE = 2
};

/// The fountain stage's five vents (gTrialRoad_FountainStagePlacements): three
/// in a row west to east, and two south of them.
enum TrialRoadFountainStageActorId {
  ACTOR_WEST_VENT = ACTOR_FIRST_STAGE_PIECE + 1,
  ACTOR_MIDDLE_VENT,
  ACTOR_EAST_VENT,
  ACTOR_SOUTHWEST_VENT,
  ACTOR_SOUTHEAST_VENT
};

/// The stage room's entrances. Every stage is the same room redrawn: the
/// stairs down at a stage's foot lead in at the top of the stage below, and
/// the door at its top in at the foot of the next. The Shaman Village enters
/// the first stage by its left or right path, at
/// TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST or _EAST, and the
/// summit returns to the last stage the same way, at
/// TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST or _EAST; each goes on to the
/// stage entrance of its last digit. While Moapa explains the trial's rules
/// (ShamanVillage_ExplainTrialRules), the village sends the party in at
/// TRIAL_ROAD_STAGES_FOR_TRIAL_RULES, which deals a course for
/// TRIAL_ROAD_RULES_COURSE to show. The summit is entered again
/// after the battle with Moapa, won or lost.
enum TrialRoadEntrance {
  TRIAL_ROAD_STAGE_FOOT_LEFT = 1,
  TRIAL_ROAD_STAGE_FOOT_RIGHT = 2,
  TRIAL_ROAD_STAGE_TOP_LEFT = 3,
  TRIAL_ROAD_STAGE_TOP_RIGHT = 4,
  TRIAL_ROAD_SUMMIT_AFTER_WIN = 4,
  TRIAL_ROAD_SUMMIT_AFTER_DEFEAT = 5,
  TRIAL_ROAD_RULES_COURSE = 55
};

/// The sand stage's two tightrope posts (gTrialRoad_SandStagePlacements).
enum { ACTOR_WEST_POST = ACTOR_FIRST_STAGE_PIECE, ACTOR_EAST_POST };

/// The torch stage's pillars C to F are frozen puddles, pillars of ice; each
/// one's flag, FLAG_SCENE_TEMP_14 to FLAG_SCENE_TEMP_17, is set once a torch
/// has melted it to a puddle.
#define FLAG_PUDDLE_MELTED_BASE (FLAG_SCENE_TEMP_14 - ACTOR_PILLAR_C)
/// The weeds pillar stage's two clumps of weeds; each one's flag,
/// FLAG_AREA_TEMP_27 and FLAG_AREA_TEMP_28, is set once Whirlwind has blown it
/// away.
#define FLAG_WEEDS_BLOWN_AWAY_BASE (FLAG_AREA_TEMP_27 - ACTOR_WEEDS_A)

/// The log stage's logs' flags, FLAG_SCENE_TEMP_53 to FLAG_SCENE_TEMP_55, set
/// once each has rolled off the edge into the water.
#define FLAG_LOG_IN_WATER_BASE (FLAG_SCENE_TEMP_53 - ACTOR_UPPER_LOG)
/// The fountain stage's vents' flags, FLAG_AREA_TEMP_63 to FLAG_AREA_TEMP_65,
/// set once each has been uncovered.
#define FLAG_VENT_UNCOVERED_BASE (FLAG_AREA_TEMP_63 - ACTOR_WEST_VENT)

/// A piece Felix cannot push: a pillar weeds hold down, a frozen puddle, a
/// stake, or a log (logs roll by their own events).
/// Leader_CanPushObjectAhead refuses a target with this byte set.
#define PUSH_BLOCKED(object) *(u8 *)&object->sceneCounter
/// The height in pixels of a pillar's top (TrialRoad_AddPillarTop) above the
/// pillar: PILLAR_HEIGHT standing, -PILLAR_HEIGHT lowered.
#define PILLAR_TOP_HEIGHT(top) *(u8 *)&top->sceneByte

/// A fountain rises and falls once each FOUNTAIN_PERIOD frames, rising at
/// FOUNTAIN_RISE_AT and falling at FOUNTAIN_FALL_AT.
#define FOUNTAIN_PERIOD 240
#define FOUNTAIN_RISE_AT 120
#define FOUNTAIN_FALL_AT 150

/// How far a pillar's top stands above it; once the pillar is lowered, its
/// top is as far below it.
#define PILLAR_HEIGHT 16

/// Talking to Gasp starts his battle.
void TrialRoad_FightGasp(void) {
  Field_RequestDjinnBattle(ACTOR_GASP, DJINNI_GASP);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTrialRoad_Placements[];
extern struct ActorPlacement gTrialRoad_CaveRockPlacements[];
extern struct ActorPlacement gTrialRoad_GaspPlacements[];
extern struct ActorPlacement gTrialRoad_SummitPlacements[];
extern s32 gTrialRoad_StagesPlacements[];

/// The script of each clump of weeds: it sits on the ground at the foot of its
/// pillar or cliff, drawn half again as tall as its picture.
s32 TrialRoad_SettleOnGround(struct Actor *actor) {
  s32 height =
      Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) + toFIXED(2);

  actor->groundHeight = height;
  actor->y.fixed = height;
  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  actor->scaleY = FIXED_ONE * 3 / 2;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_STAGES)
    return gTrialRoad_StagesPlacements[pattern & 7];
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
    return gTrialRoad_CaveRockPlacements;
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_GASP)
    return gTrialRoad_GaspPlacements;
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_SUMMIT)
    return gTrialRoad_SummitPlacements;
  return gTrialRoad_Placements;
}

/// Whether Moapa is ahead in the race: by the stages each has cleared, then by
/// who reached the switch at the top of this stage first; on a tie, fallback.
s32 TrialRoad_RivalIsAhead(s32 fallback) {
  struct TrialRoadState *state = gTrialRoadState;

  if (state->leaderBest < state->rivalBest)
    return 1;
  if (state->leaderBest > state->rivalBest)
    return 0;
  if (Event_GetFlag(FLAG_STAGE_CHESTS_OPENED))
    return 0;
  if (state->rivalTimer != 0)
    return 1;
  return fallback;
}

s32 TrialRoad_OfferToGiveUp(void);
void Actor_SetSpriteColorMode(struct Actor *actor, u32 mode);

/// Walking down the stairs at the foot of a stage, or through the open door at
/// its top. Down from the first stage during the race, Felix is asked whether
/// he gives up, and stays if he does not; up through the fourth door, he
/// climbs to the summit.
void TrialRoad_TakeStairs(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;

  if (direction == CELL_LEFT_PATH_STAIRS_DOWN ||
      direction == CELL_RIGHT_PATH_STAIRS_DOWN) {
    if (state->stage[0] <= 0) {
      if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
#ifdef UBFIX
        if (TrialRoad_OfferToGiveUp() == 0)
#else
        // UB: TrialRoad_OfferToGiveUp takes no argument, but this call passes
        // 0.
        if (((s32 (*)(s32))TrialRoad_OfferToGiveUp)(0) == 0)
#endif
          Actor_WalkByAndWait(gMainRoutineState.leaderActorId, 0, -16);
        return;
      }
      // Out to the trial grounds: the stairs' exits to them are numbered
      // four on from the stairs' cells.
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->stage[0]--;
      Import_EventSetExitRequest(direction);
    }
    Event_PlaySound(SE_MAP_EXIT);
  }
  if (direction == CELL_LEFT_PATH_DOOR || direction == CELL_RIGHT_PATH_DOOR) {
    state->stage[0]++;
    if (state->stage[0] > 3) {
      if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT) &&
          TrialRoad_RivalIsAhead(0) == 0)
        Event_SetFlag(FLAG_PARTY_FIRST_AT_PEAK);
      Event_PlaySound(SE_STAIRS_UP);
      Exit_StartStairsUp();
      // Up to the summit, by the exits numbered four on from the doors'.
      Import_EventSetExitRequest(direction + 4);
    } else {
      Import_EventSetExitRequest(direction);
      Event_PlaySound(SE_MAP_EXIT);
    }
    if (state->stage[0] > state->leaderBest)
      state->leaderBest = state->stage[0];
  }
  state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
}

struct Actor;
struct FieldSprite;
union FieldObject;

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 tileHigh : 2; ///< the tile number's top two bits
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 padding[9];
  struct EffectSpriteBits bits;
};

/// What the rival does each frame (TrialRoad_MoveRival).
enum TrialRoadRivalMode {
  RIVAL_REPLAY = 1, ///< walks the recorded path
  RIVAL_RECORD = 2, ///< records the actor's path
  RIVAL_LINKED = 3, ///< Unused: stands where the linked player's record puts it
  RIVAL_MIRROR = 4, ///< follows the leader mirrored across the course
  RIVAL_WAITING = 9, ///< the path is done
};

/// The rival's state in gSceneState.
struct TrialRoadReplay {
  s16 mode; ///< enum TrialRoadRivalMode
  s16 mirrored;
  s16 id;       ///< the rival's actor
  s16 point;    ///< the next x, z pair of the path
  s16 still;    ///< frames the rival has not moved
  s16 resource; ///< the recording's resource
};

/// One queued sprite: the three words Oam_AddSpriteAtDepth adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

/// The halfwords of the rival's recorded path: x, z pairs, the last pair 0, 0.
#define RIVAL_PATH_LENGTH 7200

/// The tiles of the race map's picture (gTrialRoad_RaceMapPicture): a
/// stage of the road in blue, 16 by 32 pixels with a 16 by 8 foot below it,
/// and the two players' 16 by 16 markers, "1P" and "2P".
enum TrialRoadRaceMapTile {
  RACE_MAP_TILE_STAGE = 0,
  RACE_MAP_TILE_STAGE_FOOT = 8,
  RACE_MAP_TILE_1P = 10,
  RACE_MAP_TILE_2P = 14,
  RACE_MAP_TILE_COUNT = 18
};
/// The bytes of the race map's tiles.
#define RACE_MAP_PICTURE_BYTES (RACE_MAP_TILE_COUNT * TILE_SIZE_4BPP)
/// The sprite palette the race map loads and draws with.
#define RACE_MAP_PALETTE 14
/// The pixels between one stage's picture and the next, down the display.
#define RACE_MAP_STAGE_SPACING 36

/// The heap slot TrialRoad_ShowRaceMap takes for the race map of both parties
/// and the rival's recorded path.
struct TrialRoadLinkState {
  struct TrialRoadSprite sprites[18]; ///< the display's queued sprites
  s16 vramEntry;
  s16 fade; ///< 0 hidden to 2 shown, a step a frame
  s16 scrollX;
  s16 scrollY;
  s16 holdFade; ///< nonzero holds the fade at 2
  s16 leaderId; ///< the actor the first marker follows
  s16 rivalId;  ///< the second marker's, which the recording replays
  /// The rival's timer in the link race, beside the far and the near side;
  /// nothing in the overlay writes them.
  u16 rivalTimers[2];
  s16 resource; ///< the recorded path's resource, once decoded
  s32 originX;  ///< the course's corner the markers measure from
  s32 originZ;
  s16 path[RIVAL_PATH_LENGTH]; ///< x, z pairs in pixels, 0, 0 ending them
};

s32 TrialRoad_IsMirrored(void);

void TrialRoad_SetJumpCellUnder(s32 id);
void TrialRoad_ClearJumpCellUnder(s32 id);
void TrialRoad_AddPillarTop(s32 id);
void TrialRoad_SetPillarTopLift(s32 id, s32 value);
void TrialRoad_OpenVentCell(s32 id);
void TrialRoad_CheckLeaderOnPillars(void);
void TrialRoad_CheckStakesCovered(s32 unused, s32 id);
void TrialRoad_PoundStake(s32 id, s32 phase, s32 frame);
void TrialRoad_StepOntoStake(s32 id, s32 other, s32 flag, s32 side);
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis);
void TrialRoad_UncoverVent(s32 id, s32 phase, s32 frame);
void Leader_CheckCrumblingStep(const u16 *table);
void TrialRoad_CrumblePillar(const void *data, s32 id);

void TrialRoad_ClearGiveUpAsked(void);

void TrialRoad_BlowAwayPillarStageWeeds(void);

void TrialRoad_JumpAcrossGap(void);

void TrialRoad_JumpMeltedPuddle(void);

void TrialRoad_StepOntoWestStake(void);

void TrialRoad_StepOntoInnerWestStake(void);

void TrialRoad_StepOntoInnerEastStake(void);

void TrialRoad_StepOntoEastStake(void);

void TrialRoad_LandOnPillar(void);

void TrialRoad_CheckCrumblingPillars(void);

void TrialRoad_PushUpperLogWest(void);

void TrialRoad_PushUpperLogEast(void);

void TrialRoad_PushLowerLogWest(void);

void TrialRoad_PushLowerLogEast(void);

void TrialRoad_PushEastLogNorth(void);

void TrialRoad_PushEastLogSouth(void);

void TrialRoad_PushEastLogSouthOrClimbDown(void);

void TrialRoad_StartSand(void);

void TrialRoad_EndSand(void);

void TrialRoad_BlowAwaySandStageWeeds(void);

void TrialRoad_DriftWithStream(void);

void TrialRoad_LiftCaveRock(void);

void TrialRoad_ScatterWeeds(void);

void TrialRoad_ScatterWeedsHidingMadPlant(void);

void TrialRoad_RevealPoolTiles(void);
void TrialRoad_HidePoolTiles(void);

void TrialRoad_ChallengeMoapa(void);

extern struct ActorPlacement gTrialRoad_PillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_TorchStagePlacements[];
extern struct ActorPlacement gTrialRoad_WeedsPillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_PoundStagePlacements[];
extern struct ActorPlacement gTrialRoad_CrumblingStagePlacements[];
extern struct ActorPlacement gTrialRoad_LogStagePlacements[];
extern struct ActorPlacement gTrialRoad_FountainStagePlacements[];
extern struct ActorPlacement gTrialRoad_SandStagePlacements[];
extern struct Trigger gTrialRoad_PillarStageTriggers[];
extern struct Trigger gTrialRoad_TorchStageTriggers[];
extern struct Trigger gTrialRoad_WeedsPillarStageTriggers[];
extern struct Trigger gTrialRoad_PoundStageTriggers[];
extern struct Trigger gTrialRoad_CrumblingStageTriggers[];
extern struct Trigger gTrialRoad_LogStageTriggers[];
extern struct Trigger gTrialRoad_FountainStageTriggers[];
extern struct Trigger gTrialRoad_SandStageTriggers[];

void InventorySnapshot_Restore();

void TrialRoad_Idle(void);

s32 TrialRoad_RivalIsAhead(s32 fallback);

/// Inventory_ClearSlot through its far call: empties an inventory slot and
/// returns 2.
s32 Inventory_ClearSlot(s32 owner, s32 slot);

extern s16 gTrialRoad_CrumblingPillars[];

void TrialRoad_ReachPeakFirst(void);
void TrialRoad_ReachPeakSecond(void);

void TrialRoad_LowerStake(s32 id);

void TrialRoad_RaiseStake(s32 id);

void TrialRoad_BlockLogs(void);

void TrialRoad_StartFountains(s32 unused, s32 shape);

void Tightrope_Initialize(void);
s32 Tightrope_Place(s32 slotNo, s32 firstId, s32 secondId);

extern u32 gTrialRoad_GaspHopScript[];

/// The summit's colours as the cycle loads them (TrialRoad_StartPaletteCycle).
u16 gTrialRoad_PaletteCycleColors[16] BSS_DATA;

void TrialRoad_StepPaletteCycle(void);
void TrialRoad_ShutSummitDoorway();
void TrialRoad_PlaceMoapaAtPeak(void);
void TrialRoad_BeatMoapa(void);
void TrialRoad_LoseToMoapa(void);
void TrialRoad_ShowTraps(void);

void TrialRoad_SetUpCave(void);

void TrialRoad_SetUpPoolRoom(void);

extern u32 gTrialRoad_RivalRecordings[];
s32 TrialRoad_RivalIsWaiting(void);
void TrialRoad_PlayRivalRecording(s32 resource);
void TrialRoad_DealStageOrder(u32 seed);

void Map_CopyRawCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                          s32 dstY);

void TrialRoad_ShowRaceMap(s32 leaderId, s32 rivalId);

/// Opens the door at the top of Felix's stage.
void TrialRoad_OpenLeaderDoor(s32 side, s32 phase);

extern u32 gTrialRoad_LeftKnightEntersScript[];

extern u32 gTrialRoad_RightKnightEntersScript[];

extern u32 gTrialRoad_LeftKnightToChestScript[];
extern u32 gTrialRoad_RightKnightToChestsScript[];

extern u32 gTrialRoad_RightKnightLeavesScript[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Actor_DisableById(s32 id);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void Dust_Scatter(s32 value);
void Leader_SetCellShapeAt(s32 offset, s32 layer);
void MagmaRockInside_SpawnDustRing(void);
void Actor_SetCellShape(s32 level, s32 layer, s32 id);

void TrialRoad_DriftDust(union FieldObject *object);

void Actor_StartIdle(struct Actor *actor);

/// The race map's colours.
extern const u16 gTrialRoad_RaceMapPalette[];

void TrialRoad_MoveRival(void);
void TrialRoad_SetUpPillarStage(void);
void TrialRoad_SetUpTorchStage(void);
void TrialRoad_SetUpWeedsPillarStage(void);
void TrialRoad_SetUpPoundStage(void);
void TrialRoad_SetUpCrumblingStage(void);
void TrialRoad_SetUpLogStage(void);
void TrialRoad_SetUpFountainStage(void);

void TrialRoad_SetUpSandStage(void);

void TrialRoad_UpdateFountains(void);

/// Defines a routine that opens a stage's door at the top: on the left path
/// (side 0) the door at column leftX, on the right the one at rightX, redrawn
/// open from the spare cells, with the upper layer's at upperLeftX or
/// upperRightX and the ground of the cell at column groundLeftX or
/// groundRightX of row 127.
#define DEFINE_TRIAL_ROAD_OPEN_DOOR(name, leftX, upperLeftX, groundLeftX,      \
                                    rightX, upperRightX, groundRightX)         \
  void name(s32 side, s32 phase) {                                             \
    if (side == 0) {                                                           \
      if (phase == 3)                                                          \
        MapGrid_CopyCellTilesCellsRect(0, 120, 1, 2, leftX, phase);            \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, upperLeftX, 3);                   \
      MapGrid_CopyCellAttributes(groundLeftX, 127, 1, 1, leftX, 8);            \
    } else {                                                                   \
      if (phase == 3)                                                          \
        MapGrid_CopyCellTilesCellsRect(0, 120, 1, 2, rightX, phase);           \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, upperRightX, 3);                  \
      MapGrid_CopyCellAttributes(groundRightX, 127, 1, 1, rightX, 8);          \
    }                                                                          \
  }

/// Opens the door at the top of Felix's stage: the door at cell (23, 3) on the
/// left path, (9, 3) on the right, is redrawn open and its doorway leads on.
DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenLeaderDoor, 23, 57, 0, 9, 43, 1)

/// Opens the door at the top of Moapa's stage, beside Felix's: cell (28, 3)
/// when Felix took the left path, (4, 3) when he took the right.
DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenRivalDoor, 28, 62, 1, 4, 38, 0)

/// Stepping on the give-up button at the top of a stage during the race
/// (cell (12, 8) on the left path, (20, 8) on the right) asks "Do you want to
/// give up?". Felix walks onto the button's middle and faces south; answering
/// Yes, he leans back with a drop of sweat, gets the party's equipment back
/// and is sent down to the Shaman Village's trial grounds.
s32 TrialRoad_OfferToGiveUp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 message;

  if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId,
                        ((FIELD_CELL(leader->x.fixed)) << 4) + 8,
                        ((FIELD_CELL(leader->z.fixed)) << 4) + 8);
    Event_PlaySound(SE_SPLASH);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    message = (s32)&MsgTrialRoadGiveUpPrompt;
    // "Do you want to give up?"
    FieldMessage_Show(message, QUESTION_MESSAGE);
    state->choiceMessage = message + 1;
    state->choiceWidth = 4;
    state->choiceSelection = 1;
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 1) ==
        0) {
      Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
      Event_Wait(10);
      Actor_SetAnimation(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_LEAN_BACK);
      Event_Wait(10);
      FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                           EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
      Event_Wait(30);
      InventorySnapshot_Restore();
      Import_EventSetExitRequest(
          TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_C);
      return 1;
    }
  }
  Event_SetFlag(FLAG_GIVE_UP_ASKED);
  return 0;
}

/// Stepping off the give-up button lets it ask again next time Felix steps on
/// it.
void TrialRoad_ClearGiveUpAsked(void) { GameFlag_Clear(FLAG_GIVE_UP_ASKED); }

/// Standing on the switch in front of the door at the top of a stage (cell
/// (22, 9) on the left path, (10, 9) on the right) opens the blue chest
/// beside the door, and the second chest too when Moapa got there
/// first ("it will take twice the weight to open the doors").
void TrialRoad_OpenChests(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId,
                      ((FIELD_CELL(leader->x.fixed)) << 4) + 8,
                      ((FIELD_CELL(leader->z.fixed)) << 4) + 8);
  TrialRoad_Idle();
  found = TrialRoad_RivalIsAhead(0);
  if (found != 0)
    Event_SetFlag(state->stage[0] + FLAG_RIVAL_FIRST_AT_DOOR_BASE);
  Event_PlaySound(SE_CHEST_OPEN);
  if (state->mirrored == 0) {
    Actor_SetAnimation(ACTOR_LEFT_INNER_CHEST, 6);
    Event_SetFlag(FLAG_INNER_CHEST_OPEN);
    if (found != 0) {
      Actor_SetAnimation(ACTOR_LEFT_OUTER_CHEST, 6);
      Event_SetFlag(FLAG_OUTER_CHEST_OPEN);
    }
  } else {
    Actor_SetAnimation(ACTOR_RIGHT_INNER_CHEST, 6);
    Event_SetFlag(FLAG_INNER_CHEST_OPEN);
    if (found != 0) {
      Actor_SetAnimation(ACTOR_RIGHT_OUTER_CHEST, 6);
      Event_SetFlag(FLAG_OUTER_CHEST_OPEN);
    }
  }
  Event_SetFlag(FLAG_STAGE_CHESTS_OPENED);
  Event_End();
}

/// Choosing a piece of equipment for an open blue chest puts it inside, and
/// the message names its owner and the item ("Felix put ...'s ... in the
/// chest."). The chest shuts, and once every open chest is shut the door
/// beside them opens. The item is taken from its owner's inventory.
void TrialRoad_PutItemInChest(s32 item, s32 owner, s32 slot) {
  s32 chest = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  struct TrialRoadState *state = gTrialRoadState;

  Event_Start();
  Event_EndLastingPsynergy(0);
  MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
  // "Felix put ...'s ... in the chest."
  FieldMessage_Show((s32)&MsgTrialRoadStoredItem, FIELD_MESSAGE_ZOOM);
  Event_PlaySound(SE_CHEST_CLOSE);
  Actor_SetAnimationAndWait(chest, 5);
  if (chest == ACTOR_LEFT_INNER_CHEST || chest == ACTOR_RIGHT_INNER_CHEST)
    Event_ClearFlag(FLAG_INNER_CHEST_OPEN);
  else
    Event_ClearFlag(FLAG_OUTER_CHEST_OPEN);
  if (!Event_GetFlag(FLAG_INNER_CHEST_OPEN) &&
      !Event_GetFlag(FLAG_OUTER_CHEST_OPEN)) {
    TrialRoad_OpenLeaderDoor(state->mirrored, state->stage[0]);
    Event_PlaySound(SE_ITEM_BREAK);
    Event_SetFlag(FLAG_STAGE_DOOR_OPEN);
  }
  Inventory_ClearSlot(owner, slot);
  Event_End();
}

/// Searching a blue chest before the switch has opened it says "The chest
/// won't open. There's something keeping it shut tight." Once it stands open,
/// Felix is asked "Place this item in the chest?", and Yes opens the item menu.
void TrialRoad_ExamineChest(s32 id) {
  s32 flag;

  if (id == ACTOR_LEFT_INNER_CHEST || id == ACTOR_RIGHT_INNER_CHEST)
    flag = FLAG_INNER_CHEST_OPEN;
  else
    flag = FLAG_OUTER_CHEST_OPEN;
  if (!Event_GetFlag(flag)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    // "The chest won't open. There's something keeping it shut tight."
    FieldMessage_Show((s32)MsgTrialRoadChestBlocked, FIELD_MESSAGE_ZOOM);
    Event_End();
  } else {
    // "Place this item in the chest?"
    FieldMessage_Show((s32)MsgTrialRoadPlaceItemPrompt, QUESTION_MESSAGE);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 1) ==
        0)
      gFieldState->fieldMenu = 1;
  }
}

/// Walking around the switch once the chests have opened does nothing.
void TrialRoad_StepOffSwitch(void) {}

/// Lets Felix jump onto the top of the pillar standing at the actor's cell
/// (MAP_CELL_JUMP).
void TrialRoad_SetJumpCellUnder(s32 id) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    u8 *cells = (u8 *)map->layers[0].cells;
    u8 *cell =
        cells +
        (((FIELD_CELL(actor->x.fixed)) + ((FIELD_CELL(actor->z.fixed)) << 7))
         << 2);
    s32 bit = MAP_CELL_JUMP;

    cell[3] = bit | cell[3];
  }
}

/// Stops Felix jumping onto the cell the actor is leaving.
void TrialRoad_ClearJumpCellUnder(s32 id) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    u8 *cells = (u8 *)map->layers[0].cells;
    u8 *cell =
        cells +
        (((FIELD_CELL(actor->x.fixed)) + ((FIELD_CELL(actor->z.fixed)) << 7))
         << 2);

    cell[3] &= (u8)~MAP_CELL_JUMP;
  }
}

/// Keeps an invisible pillar top on its pillar, so many pixels above it.
s32 TrialRoad_FollowPillar(struct Actor *actor) {
  struct Actor *owner = &actor->watchedActor->actor;

  SET_ACTOR_POSITION(actor, owner->x.fixed,
                     owner->y.fixed + toFIXED(PILLAR_TOP_HEIGHT(actor)),
                     owner->z.fixed);
  actor->groundHeight = owner->groundHeight;
  return 1;
}

extern s32 gTrialRoad_PillarTopScript[];

/// Puts an invisible top on the pillar for Felix to stand on.
void TrialRoad_AddPillarTop(s32 id) {
  struct Actor *owner = Actor_GetById(id);
  struct Actor *actor;

  if (owner != NULL) {
    actor = FieldActor_Create(SPRITE_UNSEEN_MARKER, owner->x.fixed,
                              owner->y.fixed, owner->z.fixed);
    if (actor != NULL) {
      Actor_SetSpriteFlags(actor, 0);
      Actor_StartScript(actor, gTrialRoad_PillarTopScript);
      actor->watchedActor = (union FieldObject *)owner;
      actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      PILLAR_TOP_HEIGHT(actor) = PILLAR_HEIGHT;
      owner->watchedActor = (union FieldObject *)actor;
    }
  }
}

/// Sets how many pixels above the pillar its top stands.
void TrialRoad_SetPillarTopLift(s32 id, s32 value) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    actor = &actor->watchedActor->actor;
    if (actor != NULL)
      PILLAR_TOP_HEIGHT(actor) = value;
  }
}

/// Defines a push or Move trigger's event that, while the leader is on foot,
/// calls onFoot with the pushed actor.
#define DEFINE_TRIAL_ROAD_ON_FOOT(name, onFoot)                                \
  void name(s32 unused, s32 id) {                                              \
    struct MainRoutineState *party = &gMainRoutineState;                       \
    u8 mode;                                                                   \
                                                                               \
    mode = party->movementMode;                                                \
                                                                               \
    if (mode == LEADER_MOVEMENT_WALK)                                          \
      onFoot(id);                                                              \
  }

/// As a pillar starts to slide, Felix can no longer jump onto the place it is
/// leaving.
DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_ClearJumpUnderPushedPillar,
                          TrialRoad_ClearJumpCellUnder)

/// Once a pushed or Moved pillar stops, Felix can jump onto its top where it
/// stands.
DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_SetJumpUnderPushedPillar,
                          TrialRoad_SetJumpCellUnder)

/// Whirlwind on the weeds at the foot of the pillar stage's overgrown pillar,
/// at cell (15, 24), blows it away, and the pillar can then be pushed.
void TrialRoad_BlowAwayPillarStageWeeds(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 0;
  Event_SetFlag(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY);
}

/// Walking off the edge of a ledge or pillar top toward another one jumps
/// Felix across, when there is somewhere to land.
void TrialRoad_JumpForward(void) { Field_TryJumpForward(); }

/// Lets Felix walk into the vent where the actor stands, now that it is dug
/// out.
void TrialRoad_OpenVentCell(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL)
    MapGrid_CopyCellsLayered(14, 27, 1, 1, FIELD_CELL(actor->x.fixed),
                             FIELD_CELL(actor->z.fixed));
}

/// Keeps note, each frame, of whether Felix is up on the pillar stage's upper
/// ledge or a pillar's top, 32 pixels up (FLAG_LEADER_ON_PILLARS).
void TrialRoad_CheckLeaderOnPillars(void) {
  FIXED y = Actor_GetById(gMainRoutineState.leaderActorId)->y.fixed;

  if (y == toFIXED(32))
    Event_SetFlag(FLAG_LEADER_ON_PILLARS);
  else
    Event_ClearFlag(FLAG_LEADER_ON_PILLARS);
}

/// Walking south off the pillar stage's upper ledge, beside the vine, jumps
/// Felix across the gap when the pillar beneath has been pushed into it (row
/// 23 of the map); otherwise he hops down to the ground in a puff of dust.
void TrialRoad_JumpAcrossGap(void) {
  if (FIELD_CELL(Actor_GetById(ACTOR_PILLAR_B)->z.fixed) == 23)
    Field_TryJumpForward();
  else
    Actor_JumpDownLedge(gMainRoutineState.leaderActorId, ANIM_LEADER_SPRING, 0);
}

/// As one of the torch stage's two stone pillars starts to slide, Felix can no
/// longer jump onto the place it is leaving.
void TrialRoad_ClearJumpUnderTorchStagePillar(s32 unused, s32 id) {
  if ((u32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A)
    TrialRoad_ClearJumpCellUnder(id);
}

/// Pushing a torch against one of the torch stage's two pillars of ice, at
/// cells (13, 27) and (19, 27), melts it to a puddle Felix can jump across.
/// A stone pillar pushed in the torch stage can be jumped onto where it
/// stops.
void TrialRoad_MeltIceBesideTorch(s32 unused, s32 id) {
  s32 x;

  if ((u32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A) {
    TrialRoad_SetJumpCellUnder(id);
    return;
  }
  x = (s32)Actor_GetById(id);
  x = FIELD_CELL(((struct Actor *)x)->x.fixed);
  id = 0;
  if (x == 13)
    id = ACTOR_PILLAR_C;
  if (x == 15)
    id = ACTOR_PILLAR_D;
  if (x == 17)
    id = ACTOR_PILLAR_E;
  if (x == 19)
    id = ACTOR_PILLAR_F;
  if (id != 0) {
    s32 flag = id + FLAG_PUDDLE_MELTED_BASE;

    if (Event_GetFlag(flag) == 0) {
      struct Actor *actor = Actor_GetById(id);

      actor->update = NULL;
      Actor_SetSpriteColorMode(actor, 0);
      Actor_SetAnimation(id, 6);
      TrialRoad_SetPillarTopLift(id, -PILLAR_HEIGHT);
      Event_SetFlag(flag);
    }
  }
}

/// Walking north or south into a melted puddle in the torch stage jumps Felix
/// across it; a frozen one stops him.
void TrialRoad_JumpMeltedPuddle(void) {
  if (Event_GetFlag(
          Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId) +
          FLAG_PUDDLE_MELTED_BASE))
    Field_TryJumpForward();
}

/// Frost on a melted puddle in the torch stage freezes it into a pillar of ice
/// again, which Felix can stand on.
void TrialRoad_FreezePuddle(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  TrialRoad_SetPillarTopLift(id, PILLAR_HEIGHT);
  Event_ClearFlag(id + FLAG_PUDDLE_MELTED_BASE);
  if (actor != NULL)
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
}

/// Whirlwind on the weeds at the foot of one of the two overgrown pillars in
/// the weeds stage blows it away, and the pillar can then be pushed or Moved.
void TrialRoad_BlowAwayWeeds(s32 unused, s32 id) {
  PUSH_BLOCKED(Actor_GetById(id - (ACTOR_WEEDS_A - ACTOR_PILLAR_A))) = 0;
  Event_SetFlag(id + FLAG_WEEDS_BLOWN_AWAY_BASE);
}

/// Sinks a stake into the ground, so that Felix walks over it.
void TrialRoad_LowerStake(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    TrialRoad_SetPillarTopLift(id, -PILLAR_HEIGHT);
    actor->collisionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SelectAnimation(actor, 6);
  }
}

/// Raises a stake out of the ground, solid, its top 16 pixels up.
void TrialRoad_RaiseStake(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    TrialRoad_SetPillarTopLift(id, PILLAR_HEIGHT);
    actor->collisionFlags = ACTOR_SOLID;
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    Actor_SelectAnimation(actor, 5);
  }
}

/// Whirlwind on the weeds at the foot of the Pound stage's middle pillar, at
/// cell (16, 28), blows it away, and the pillar can then be pushed.
void TrialRoad_BlowAwayPoundStageWeeds(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 0;
  Event_SetFlag(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY);
}

/// The stake each of the pound stage's four stakes raises when it is pounded
/// down: they go in pairs.
extern s8 gTrialRoad_PoundPartners[4];

/// After Move or a push, notes each stake whose partner has a pillar standing
/// on it: stepping on that stake can no longer raise the partner
/// (TrialRoad_StepOntoStake).
void TrialRoad_CheckStakesCovered(s32 unused, s32 pushedId) {
  s32 id;

  for (id = ACTOR_WEST_STAKE; id <= ACTOR_EAST_STAKE; id++) {
    struct Actor *actor = Actor_GetById(id);
    s32 x = FIELD_CELL(actor->x.fixed);
    s32 z = FIELD_CELL(actor->z.fixed);
    u8 *covered = &PILLAR_TOP_HEIGHT(
        Actor_GetById(gTrialRoad_PoundPartners[(id - ACTOR_WEST_STAKE) & 3]));
    s32 other;

    *covered = 0;
    for (other = ACTOR_PILLAR_A; other <= ACTOR_PILLAR_C; other++) {
      actor = Actor_GetById(other);
      if (x == FIELD_CELL(actor->x.fixed) && z == FIELD_CELL(actor->z.fixed)) {
        *covered = 1;
        break;
      }
    }
  }
}

/// Pound on a raised stake in the Pound stage squashes it into the ground and
/// pops its partner up (gTrialRoad_PoundPartners); Felix, standing where the
/// partner rises, is thrown up onto it.
void TrialRoad_PoundStake(s32 id, s32 phase, s32 frame) {
  s32 other = gTrialRoad_PoundPartners[(id - ACTOR_WEST_STAKE) & 3];
  struct Actor *actor = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  if (frame <= 10)
    actor->scaleY = FIXED_ONE - frame * (FIXED_ONE / 16);
  else
    actor->scaleY = FIXED_ONE;
  if (phase == POUND_STAGE_LANDED) {
    TrialRoad_LowerStake(id);
    TrialRoad_RaiseStake(other);
    switch (id) {
    case ACTOR_WEST_STAKE:
      Event_SetFlag(FLAG_INNER_WEST_STAKE_RAISED);
      break;
    case ACTOR_INNER_WEST_STAKE:
      Event_ClearFlag(FLAG_INNER_WEST_STAKE_RAISED);
      break;
    case ACTOR_INNER_EAST_STAKE:
      Event_SetFlag(FLAG_EAST_STAKE_RAISED);
      break;
    case ACTOR_EAST_STAKE:
      Event_ClearFlag(FLAG_EAST_STAKE_RAISED);
      break;
    }
    if (FIELD_CELL(partner->x.fixed) == FIELD_CELL(leader->x.fixed) &&
        FIELD_CELL(partner->z.fixed) == FIELD_CELL(leader->z.fixed)) {
      leader->velocityY = toFIXED(8);
      leader->x.fixed = (leader->x.fixed & -toFIXED(16)) + toFIXED(8);
      leader->z.fixed = (leader->z.fixed & -toFIXED(16)) + toFIXED(8);
    }
  }
  if (frame == 9)
    partner->scaleY = FIXED_ONE * 12 / 10;
  else if (frame == 11)
    partner->scaleY = FIXED_ONE * 11 / 10;
  else if (frame == 13)
    partner->scaleY = FIXED_ONE * 105 / 100;
  else
    partner->scaleY = FIXED_ONE;
}

/// Pound on the west stake, cell (13, 26), drives it down and raises the inner
/// west stake.
void TrialRoad_PoundWestStake(s32 phase, s32 frame) {
  TrialRoad_PoundStake(ACTOR_WEST_STAKE, phase, frame);
}

/// Pound on the inner west stake, cell (15, 26), drives it down and raises
/// the west stake.
void TrialRoad_PoundInnerWestStake(s32 phase, s32 frame) {
  TrialRoad_PoundStake(ACTOR_INNER_WEST_STAKE, phase, frame);
}

/// Pound on the inner east stake, cell (17, 26), drives it down and raises
/// the east stake.
void TrialRoad_PoundInnerEastStake(s32 phase, s32 frame) {
  TrialRoad_PoundStake(ACTOR_INNER_EAST_STAKE, phase, frame);
}

/// Pound on the east stake, cell (19, 26), drives it down and raises the
/// inner east stake.
void TrialRoad_PoundEastStake(s32 phase, s32 frame) {
  TrialRoad_PoundStake(ACTOR_EAST_STAKE, phase, frame);
}

/// Stepping onto a raised stake in the Pound stage sinks it slowly under
/// Felix's weight while its partner grows out of the ground, unless a pillar
/// stands on the partner.
void TrialRoad_StepOntoStake(s32 id, s32 other, s32 flag, s32 side) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  FIXED t;

  if (PILLAR_TOP_HEIGHT(actor) != 0)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  partner->scaleY = FIXED_ONE / 8;
  wait(1);
  TrialRoad_RaiseStake(other);
  t = FIXED_ONE / 8;
  do {
    TrialRoad_SetPillarTopLift(id, (FIXED_ONE / 2 - t) / (FIXED_ONE / 32));
    actor->scaleY = FIXED_ONE - t;
    partner->scaleY = t;
    wait(1);
    t += FIXED_ONE / 8;
  } while (t <= FIXED_ONE);
  actor->scaleY = FIXED_ONE;
  TrialRoad_LowerStake(id);
  Event_PlaySound(SE_ITEM_BREAK);
  if (side != 0)
    Event_SetFlag(flag);
  else
    Event_ClearFlag(flag);
  Event_End();
}

/// Stepping onto the west stake sinks it and raises the inner west one.
void TrialRoad_StepOntoWestStake(void) {
  TrialRoad_StepOntoStake(ACTOR_WEST_STAKE, ACTOR_INNER_WEST_STAKE,
                          FLAG_INNER_WEST_STAKE_RAISED, 1);
}

/// Stepping onto the inner west stake sinks it and raises the west one.
void TrialRoad_StepOntoInnerWestStake(void) {
  TrialRoad_StepOntoStake(ACTOR_INNER_WEST_STAKE, ACTOR_WEST_STAKE,
                          FLAG_INNER_WEST_STAKE_RAISED, 0);
}

/// Stepping onto the inner east stake sinks it and raises the east one.
void TrialRoad_StepOntoInnerEastStake(void) {
  TrialRoad_StepOntoStake(ACTOR_INNER_EAST_STAKE, ACTOR_EAST_STAKE,
                          FLAG_EAST_STAKE_RAISED, 1);
}

/// Stepping onto the east stake sinks it and raises the inner east one.
void TrialRoad_StepOntoEastStake(void) {
  TrialRoad_StepOntoStake(ACTOR_EAST_STAKE, ACTOR_INNER_EAST_STAKE,
                          FLAG_EAST_STAKE_RAISED, 0);
}

/// Walking into a pillar or stake in the Pound stage: from a raised top Felix
/// jumps onto it, and from the ground he pushes it, if it can be pushed.
void TrialRoad_LandOnPillar(void) {
  FIXED y = Actor_GetById(gMainRoutineState.leaderActorId)->y.fixed;

  if (y >= toFIXED(16)) {
    Field_TryJumpForward();
  } else if (Leader_CanPushObjectAhead()) {
    Leader_PushObjectAhead();
    TrialRoad_CheckStakesCovered(0, 0);
  }
}

/// Hopping onto one of the crumbling stage's two cracked pillars, from the
/// east or the west: the first time it flashes, and the second time it
/// crumbles under Felix and drops him to the ground.
void TrialRoad_CheckCrumblingPillars(void) {
  Leader_CheckCrumblingStep(gTrialRoad_CrumblingPillars);
}

/// Burst on the crumbling stage's west cracked pillar crumbles it away.
void TrialRoad_CrumbleWestPillar(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars,
                            ACTOR_WEST_CRUMBLING_PILLAR);
}

/// Burst on the crumbling stage's east cracked pillar crumbles it away.
void TrialRoad_CrumbleEastPillar(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars,
                            ACTOR_EAST_CRUMBLING_PILLAR);
}

/// Makes a log solid where it lies: its own cell and the two beside it along
/// its length, north and south of it in mode 0, east and west otherwise
/// (CELL_BEHAVIOR_WALL on the ground and the upper level).
void TrialRoad_BlockLogCells(s32 id, s32 mode) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s32 index;
  struct MapCell *low;
  struct MapCell *high;

  if (actor == 0)
    return;
  index = (FIELD_CELL(actor->x.fixed)) + ((FIELD_CELL(actor->z.fixed)) << 7);
  low = map->layers[0].cells + index;
  high = map->layers[2].cells + index;
  if (mode == 0) {
    low[-MAPGRID_SIZE].collisionCode = CELL_BEHAVIOR_WALL;
    low->collisionCode = CELL_BEHAVIOR_WALL;
    low[MAPGRID_SIZE].collisionCode = CELL_BEHAVIOR_WALL;
    high[-MAPGRID_SIZE].collisionCode = CELL_BEHAVIOR_WALL;
    high->collisionCode = CELL_BEHAVIOR_WALL;
    high[MAPGRID_SIZE].collisionCode = CELL_BEHAVIOR_WALL;
  } else {
    low[-1].collisionCode = CELL_BEHAVIOR_WALL;
    low->collisionCode = CELL_BEHAVIOR_WALL;
    low[1].collisionCode = CELL_BEHAVIOR_WALL;
    high[-1].collisionCode = CELL_BEHAVIOR_WALL;
    high->collisionCode = CELL_BEHAVIOR_WALL;
    high[1].collisionCode = CELL_BEHAVIOR_WALL;
  }
}

/// Makes the three logs solid where they lie, and lets Felix jump east across
/// a log that has fallen into the water.
void TrialRoad_BlockLogs(void) {
  MapGrid_CopyCellAttributes(11, 64, 11, 8, 11, 22);
  MapGrid_CopyCellAttributes(79, 64, 11, 8, 79, 22);
  TrialRoad_BlockLogCells(ACTOR_UPPER_LOG, 0);
  TrialRoad_BlockLogCells(ACTOR_LOWER_LOG, 0);
  TrialRoad_BlockLogCells(ACTOR_EAST_LOG, 1);
  if (Event_GetFlag(FLAG_LOG_IN_WATER_BASE + ACTOR_UPPER_LOG))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 23);
  if (Event_GetFlag(FLAG_LOG_IN_WATER_BASE + ACTOR_LOWER_LOG))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 26);
}

/// Whether a log rolling into the cell is stopped there by the block or a
/// wall.
s32 TrialRoad_IsLogBlocked(s32 x, s32 z) {
  struct Actor *actor = Actor_GetById(ACTOR_PILLAR_A);
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *cells;
  u8 *cell;
  s32 value;

  if (FIELD_CELL(actor->x.fixed) == x && FIELD_CELL(actor->z.fixed) == z)
    return 1;
  cells = (u8 *)map->layers[0].cells;
  cell = cells + ((x + (z << 7)) << 2);
  value = cell[2];
  return IsZero(value ^ CELL_BEHAVIOR_WALL);
}

/// Felix leans on a log in the log stage and it rolls, up to six cells, until
/// the block or a wall stops it. Rolled east past column 17 of the map, it
/// rolls on to the edge and falls into the water, where it stays and Felix
/// can jump across it.
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 fromX = FIELD_CELL(actor->x.fixed);
  s32 fromZ = FIELD_CELL(actor->z.fixed);
  s32 toX = fromX;
  s32 toZ = fromZ;
  s32 n;
  s32 mode = 2;
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  struct Vector3 unused;

  if ((dx != 0 && toX != x + dx) || (dz != 0 && toZ != z + dz)) {
    gFieldState->blockedSteps = FIELD_PUSH_FRAMES + 1;
    return;
  }
  for (n = 0; n <= 5; n++) {
    x = toX + dx;
    z = toZ + dz;
    if (axis == 0) {
      if (TrialRoad_IsLogBlocked(x, z - 1))
        break;
      if (TrialRoad_IsLogBlocked(x, z))
        break;
      if (TrialRoad_IsLogBlocked(x, z + 1))
        break;
    } else {
      if (TrialRoad_IsLogBlocked(x - 1, z))
        break;
      if (TrialRoad_IsLogBlocked(x, z))
        break;
      if (TrialRoad_IsLogBlocked(x + 1, z))
        break;
    }
    toX = x;
    toZ = z;
  }
  if (fromX == toX && fromZ == toZ)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, 8);
  Event_Wait(6);
  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  if (dx < 0 || dz < 0)
    mode = 3;
  Actor_SelectAnimation(actor, mode);
  Actor_SetMoveTarget(actor, FIELD_CELL_CENTER(toX), 0, FIELD_CELL_CENTER(toZ));
  Event_Wait(6);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineState.leaderActorId, 0.3, 0.2);
  Actor_SetDestinationOffsetById(gMainRoutineState.leaderActorId, dx << 3,
                                 dz << 3);
  Event_Wait(24);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_WaitForMove(actor);
  if (toX <= 17) {
    Actor_SelectAnimation(actor, 1);
    Sound_Play(SE_STOP_EXIT_SE);
    Sound_Play(SE_HEAVY_STOP);
  } else {
    Actor_SelectAnimation(actor, 3);
    Actor_SetMoveTarget(actor, toFIXED(312), 0, FIELD_CELL_CENTER(toZ));
    Event_Wait(30);
    Actor_SelectAnimation(actor, 8);
    Actor_WaitForMove(actor);
    /* FAKEMATCH(dead): the flags stored through their taken address; as
     * actor->priorityFlags. */
    /* FAKEMATCH(temporary): mode reused to hold ACTOR_PRIORITY_UNDERFOOT for
     * the store; as the constant stored. */
    *&actor->priorityFlags = mode = ACTOR_PRIORITY_UNDERFOOT;
    Sound_Play(SE_STOP_EXIT_SE);
    Sound_Play(SE_LOG_FALL);
    GameFlag_Set(id + FLAG_LOG_IN_WATER_BASE);
  }
  Event_Wait(15);
  Event_End();
  TrialRoad_BlockLogs();
}

/// Walking west into the upper log rolls it west.
void TrialRoad_PushUpperLogWest(void) {
  TrialRoad_PushLog(ACTOR_UPPER_LOG, -1, 0, 0);
}

/// Walking east into the upper log rolls it east.
void TrialRoad_PushUpperLogEast(void) {
  TrialRoad_PushLog(ACTOR_UPPER_LOG, 1, 0, 0);
}

/// Walking west into the lower log rolls it west.
void TrialRoad_PushLowerLogWest(void) {
  TrialRoad_PushLog(ACTOR_LOWER_LOG, -1, 0, 0);
}

/// Walking east into the lower log rolls it east.
void TrialRoad_PushLowerLogEast(void) {
  TrialRoad_PushLog(ACTOR_LOWER_LOG, 1, 0, 0);
}

/// Walking north into the east log rolls it north.
void TrialRoad_PushEastLogNorth(void) {
  TrialRoad_PushLog(ACTOR_EAST_LOG, 0, -1, 1);
}

/// Walking south into the east log rolls it south.
void TrialRoad_PushEastLogSouth(void) {
  TrialRoad_PushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/// Pressing south at the north end of the east log's track: if Felix is
/// climbing there he climbs down, and on foot he rolls the east log south.
void TrialRoad_PushEastLogSouthOrClimbDown(void) {
  struct MainRoutineState *party = &gMainRoutineState;

  if (party->movementMode == LEADER_MOVEMENT_KEYS)
    Field_ClimbDown();
  else
    TrialRoad_PushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/// Scoop over one of the fountain stage's three buried vents, at cells
/// (13, 23), (16, 23) and (19, 23), digs it out: the vent shows in the
/// ground, Felix can step onto it, and its fountain rises and falls with the
/// others.
void TrialRoad_UncoverVent(s32 id, s32 phase, s32 frame) {
  if (frame == 30) {
    struct Actor *actor = Actor_GetById(id);
    s32 height;

    actor->x.fixed = toFIXED(actor->timer);
    actor->z.fixed = toFIXED((s16)actor->userValue);
    height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    actor->groundHeight = height;
    actor->y.fixed = height;
    TrialRoad_OpenVentCell(id);
    Event_SetFlag(id + FLAG_VENT_UNCOVERED_BASE);
  }
}

/// Scoop over the west vent digs it out.
void TrialRoad_UncoverWestVent(s32 phase, s32 frame) {
  TrialRoad_UncoverVent(ACTOR_WEST_VENT, phase, frame);
}

/// Scoop over the middle vent digs it out.
void TrialRoad_UncoverMiddleVent(s32 phase, s32 frame) {
  TrialRoad_UncoverVent(ACTOR_MIDDLE_VENT, phase, frame);
}

/// Scoop over the east vent digs it out.
void TrialRoad_UncoverEastVent(s32 phase, s32 frame) {
  TrialRoad_UncoverVent(ACTOR_EAST_VENT, phase, frame);
}

/// Stepping onto the fountain stage's block from above leaves Felix standing
/// on its top, 32 pixels up, rather than inside it.
void TrialRoad_StandOnBlock(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *block = Actor_GetById(ACTOR_PILLAR_A);

  if (FIELD_CELL(block->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(block->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    FIXED y = leader->y.fixed;

    if (y < toFIXED(32))
      leader->y.fixed = toFIXED(32);
  }
}

/// Sand at the sand stage lets Felix, sunk into the ground, go up the stream
/// that would otherwise carry him back. The game moves him onto the map's
/// upper level for as long as he stays sunk.
void TrialRoad_StartSand(void) {
  Actor_GetById(gMainRoutineState.leaderActorId)->layer =
      MAPGRID_LAYER_OVERHEAD;
}

/// When Felix rises out of the sand at the sand stage, he walks on the ground
/// level again.
void TrialRoad_EndSand(void) {
  Actor_GetById(gMainRoutineState.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
}

/// Whirlwind on the weeds on the sand stage's cliff blows it away and uncovers
/// the vine behind it, cell (19, 21), which Felix can then climb.
void TrialRoad_BlowAwaySandStageWeeds(void) {
  MapGrid_CopyCellAttributes(12, 21, 1, 1, 19, 21);
  Event_SetFlag(FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY);
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_TRIAL_ROAD_OPEN_DOOR
#undef DEFINE_TRIAL_ROAD_ON_FOOT

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#include "../field/field_effect_advance_motion_priority.c"

/// Walking in the sand stage's stream carries Felix south a pixel a frame,
/// throwing up spray, unless he is crossing the tightrope above it.
void TrialRoad_DriftWithStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct EffectOptions options;
  s32 velocity[3];

  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_TIGHTROPE)
    return;
  leader->z.fixed += toFIXED(1);
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = FieldEffect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = FIXED_ONE * 8 / 10;
  velocity[1] = 0;
  velocity[2] = FIXED_ONE;
  velocity[0] = 0;
  velocity[0] += ((Random16() << 17) >> 16) - toFIXED(1);
  velocity[2] += ((Random16() << 16) >> 16) - toFIXED(0.5);
  FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                    velocity[0], velocity[1], velocity[2],
                    EFFECT_USE_UPDATE | 1, &options);
}

/// Lift on the rock in the cave beyond the summit raises it out of the way to
/// Gasp's pool.
void TrialRoad_LiftCaveRock(void) { GameFlag_Set(FLAG_CAVE_ROCK_LIFTED); }

/// Cyclone on the clumps of weeds around Gasp's pool scatters the ones it
/// touches.
void TrialRoad_ScatterWeeds(void) {
  FieldCyclone_CopyTouchedCells(68, 6, MAPGRID_LAYER_MIDDLE, 21, 13, 64, 64);
}

/// Cyclone on the left-hand weeds by the entrance of Gasp's pool, cell (4, 9),
/// scatters them, and the Mad Plant hiding there leaps out and attacks.
void TrialRoad_ScatterWeedsHidingMadPlant(void) {
  FieldCyclone_CopyTouchedCells(68, 6, MAPGRID_LAYER_MIDDLE, 21, 13, 64, 64);
  if (!Event_GetFlag(FLAG_MAD_PLANT(MAD_PLANT_TRIAL_ROAD))) {
    Import_MadPlantLeapAtLeader(SPRITE_MAD_PLANT, 72, 152);
    gMainRoutineState.pendingDjinni =
        IF_FLAG_SET(FLAG_MAD_PLANT(MAD_PLANT_TRIAL_ROAD));
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_MAD_PLANTS,
                                          MAD_PLANT_TRIAL_ROAD);
  }
}

/// Unused: always 1.
UNUSED s32 TrialRoad_ReturnTrue(void) { return 1; }

/// Defines a routine that sets the three tiles in Gasp's pools, at cells
/// (10, 11), (18, 7) and (18, 15), to the cell at column groundX of row 30, and
/// the upper layer's to the one at upperX.
#define DEFINE_TRIAL_ROAD_SET_POOL_TILES(name, groundX, upperX)                \
  void name(void) {                                                            \
    MapGrid_CopyCells(groundX, 30, 1, 1, 10, 11);                              \
    Map_CopyCellsRect(groundX, 30, 1, 1, 18, 7);                               \
    Map_CopyCellsRect(groundX, 30, 1, 1, 18, 15);                              \
    Map_CopyCellsRect(upperX, 30, 1, 1, 74, 11);                               \
    Map_CopyCellsRect(upperX, 30, 1, 1, 82, 7);                                \
    Map_CopyCellsRect(upperX, 30, 1, 1, 82, 15);                               \
  }

/// Reveal anywhere in Gasp's pool room shows the hidden tiles in the pools,
/// at cells (10, 11), (18, 7) and (18, 15).
DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_RevealPoolTiles, 33, 35)

/// When Reveal wears off in Gasp's pool room, the tiles in the pools are
/// hidden again.
DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_HidePoolTiles, 34, 36)

/// Felix comes out of the last stage's doorway at the summit during the race
/// and steps on the cell in front of it, cell (13, 19) on the left path or
/// (18, 19) on the right: with the sound of a door, the dark doorway behind
/// him shuts, a closed green door like the other path's. The game also gives
/// cell (77, 18) (or (82, 18) on the right) the walking rules of the cell
/// beside it. Coming back to the summit while the race is on, the door is
/// shut again, without the sound.
void TrialRoad_ShutSummitDoorway(s32 sound) {
  if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
    if (gTrialRoadState->mirrored == 0) {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
      MapGrid_CopyCellAttributes(78, 18, 1, 1, 77, 18);
    } else {
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
      MapGrid_CopyCellAttributes(81, 18, 1, 1, 82, 18);
    }
    if (sound)
      Event_PlaySound(SE_ITEM_BREAK);
  }
  Event_SetFlag(FLAG_SUMMIT_DOORWAY_SHUT);
}

/// At the summit during the race, stepping onto the top of the peak, cell
/// (13, 16) on the left path or (18, 16) on the right, either side of the
/// round tile in the middle of the top, brings on Moapa: he speaks according
/// to who arrived first, and the battle with him and his knights begins,
/// chosen by how many doors he reached first. (The doorways below, cells
/// (13, 19) and (18, 19), are the two dark openings in the cliff under the
/// top.)
void TrialRoad_ChallengeMoapa(void) {
  s32 count = Event_GetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE);

  count += Event_GetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 1);
  count += Event_GetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 2);
  count += Event_GetFlag(FLAG_RIVAL_FIRST_AT_DOOR_BASE + 3);
  if (Event_GetFlag(FLAG_PARTY_FIRST_AT_PEAK))
    TrialRoad_ReachPeakFirst();
  else
    TrialRoad_ReachPeakSecond();
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Event_SetAfterBattleDestination(SCENE_TRIAL_ROAD_SUMMIT,
                                  TRIAL_ROAD_SUMMIT_AFTER_WIN);
  Event_SetDefeatDestination(SCENE_TRIAL_ROAD_SUMMIT,
                             TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_MOAPA, 4 - count);
}

/// Moving or pushing the summit's stone pillar, at cell (7, 20), one cell east
/// into column 8 settles it there for good.
void TrialRoad_SetSummitPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (FIELD_CELL(actor->x.fixed) == 8) {
    Event_SetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED);
    actor->motionFlags = 0;
    actor->y.fixed -= toFIXED(1);
  }
}

extern struct Trigger gTrialRoad_Triggers[];
extern struct Trigger gTrialRoad_CaveRockTriggers[];
extern struct Trigger gTrialRoad_GaspTriggers[];
extern struct Trigger gTrialRoad_SummitTriggers[];
extern s32 gTrialRoad_StagesTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_STAGES)
    return gTrialRoad_StagesTriggers[pattern & 7];
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
    return gTrialRoad_CaveRockTriggers;
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_GASP)
    return gTrialRoad_GaspTriggers;
  if (gMainRoutineState.next == SCENE_TRIAL_ROAD_SUMMIT)
    return gTrialRoad_SummitTriggers;
  return gTrialRoad_Triggers;
}

/// Readies the pillar stage: the middle pillar cannot be pushed until
/// Whirlwind blows its weeds off, and Felix can jump between the pillars' tops.
void TrialRoad_SetUpPillarStage(void) {
  if (!Event_GetFlag(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 1;
  else
    Actor_SetPosition(ACTOR_WEEDS_A, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_B);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_C);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_D);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_E);
  Task_Add(TrialRoad_CheckLeaderOnPillars, TASK_PASS_FRAME);
}

/// Readies the torch stage: its pillars of ice cannot be pushed, and those
/// already melted stay puddles.
void TrialRoad_SetUpTorchStage(void) {
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_F);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_A);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_B);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_C);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_D);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_E);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_F);
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_D)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_E)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_F)) = 1;
  if (!Event_GetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_C))
    Actor_SetAnimation(ACTOR_PILLAR_C, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_C, -PILLAR_HEIGHT);
  if (!Event_GetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_D))
    Actor_SetAnimation(ACTOR_PILLAR_D, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_D, -PILLAR_HEIGHT);
  if (!Event_GetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_E))
    Actor_SetAnimation(ACTOR_PILLAR_E, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_E, -PILLAR_HEIGHT);
  if (!Event_GetFlag(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_F))
    Actor_SetAnimation(ACTOR_PILLAR_F, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_F, -PILLAR_HEIGHT);
}

/// Readies the weeds stage: its two overgrown pillars cannot be pushed until
/// Whirlwind blows their weeds off.
void TrialRoad_SetUpWeedsPillarStage(void) {
  if (!Event_GetFlag(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_A)) = 1;
  else
    Actor_SetPosition(ACTOR_WEEDS_A, 0, 0);
  if (!Event_GetFlag(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 1;
  else
    Actor_SetPosition(ACTOR_WEEDS_B, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_A);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_B);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_C);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_D);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_E);
}

/// Readies the Pound stage: its four stakes cannot be pushed, one of each pair
/// stands raised, and the overgrown pillar cannot be pushed until Whirlwind
/// blows its weeds off.
void TrialRoad_SetUpPoundStage(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_WEST_STAKE)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_INNER_WEST_STAKE)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_INNER_EAST_STAKE)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_EAST_STAKE)) = 1;
  if (!Event_GetFlag(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 1;
  else
    Actor_SetPosition(ACTOR_POUND_STAGE_WEEDS, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_F);
  TrialRoad_AddPillarTop(ACTOR_EAST_STAKE);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_A);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_B);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_C);
  if (!Event_GetFlag(FLAG_INNER_WEST_STAKE_RAISED)) {
    TrialRoad_RaiseStake(ACTOR_WEST_STAKE);
    TrialRoad_LowerStake(ACTOR_INNER_WEST_STAKE);
  } else {
    TrialRoad_RaiseStake(ACTOR_INNER_WEST_STAKE);
    TrialRoad_LowerStake(ACTOR_WEST_STAKE);
  }
  if (!Event_GetFlag(FLAG_EAST_STAKE_RAISED)) {
    TrialRoad_RaiseStake(ACTOR_INNER_EAST_STAKE);
    TrialRoad_LowerStake(ACTOR_EAST_STAKE);
  } else {
    TrialRoad_RaiseStake(ACTOR_EAST_STAKE);
    TrialRoad_LowerStake(ACTOR_INNER_EAST_STAKE);
  }
}

void MagmaRockInside_RestoreCrumblingPillars(const u16 *table);

/// Readies the crumbling stage: the cracked pillars already crumbled stay
/// gone.
void TrialRoad_SetUpCrumblingStage(void) {
  MagmaRockInside_RestoreCrumblingPillars(gTrialRoad_CrumblingPillars);
}

/// Readies the log stage: the logs roll by their own events rather than
/// sliding like blocks, their cells are walls (TrialRoad_BlockLogs), and
/// Felix can jump onto the block.
void TrialRoad_SetUpLogStage(void) {
  MapGrid_CopyCellAttributes(11, 22, 11, 8, 11, 64);
  MapGrid_CopyCellAttributes(79, 22, 11, 8, 79, 64);
  Actor_GetById(ACTOR_UPPER_LOG)->sceneCounter = 1;
  Actor_GetById(ACTOR_LOWER_LOG)->sceneCounter = 1;
  Actor_GetById(ACTOR_EAST_LOG)->sceneCounter = 1;
  Actor_GetById(ACTOR_UPPER_LOG)->collisionFlags = 0;
  Actor_GetById(ACTOR_LOWER_LOG)->collisionFlags = 0;
  Actor_GetById(ACTOR_EAST_LOG)->collisionFlags = 0;
  TrialRoad_BlockLogs();
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_A);
}

/// One of the fountain stage's fountains: the vent its water rises from, and
/// the platform the leader rides on top of it.
struct TrialRoadFountain {
  s16 id;           ///< its vent; 0 for an empty slot
  s16 phase;        ///< frames into its period
  s16 height;       ///< how far it has risen, in pixels
  s16 periodFrames; ///< frames in one rise and fall
  s16 riseFrame;    ///< the phase it starts to rise at
  s16 fallFrame;    ///< the phase it starts to fall at
  s16 high;
  s16 low;
  s16 unusedAfterLow;   ///< set to 1 as the fountain is added; nothing reads it
  s16 speed;            ///< pixels a frame, 4 rising and -8 falling
  s16 unusedAfterSpeed; ///< cleared as the fountain is added; nothing reads it
  s16 bob;              ///< the top's bob, a sine phase
  struct Actor *top;    ///< an unseen platform on the water the leader rides
};

/// The fountains the fountain stage's state has room for.
#define FOUNTAIN_COUNT 16
/// The sprites the fountains' columns may queue in a frame.
#define FOUNTAIN_SPRITE_COUNT 80
/// A fountain's sprites sort at their screen row plus this many depth slots,
/// four behind an actor's sprite at the same place (field_sprite_draw.c adds
/// 62), so the leader riding a fountain is drawn in front of its water.
#define FOUNTAIN_DEPTH_OFFSET 58

/// The fountain stage's state, kept in the inventory snapshot's spare room.
struct TrialRoadFountains {
  s16 vramEntry;
  s16 shape; ///< the first of the shape grid's values for the fountains
  struct TrialRoadFountain fountains[FOUNTAIN_COUNT];
  u32 *sprites;
};

/// The fountain stage's fountains, kept in the inventory snapshot's spare
/// bytes.
#define TRIAL_ROAD_FOUNTAINS                                                   \
  ((struct TrialRoadFountains *)(gInventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high);

/// How far the fountain stage's northern three fountains rise, and its southern
/// two.
#define NORTH_FOUNTAIN_HEIGHT 64
#define SOUTH_FOUNTAIN_HEIGHT 32

/// Readies the fountain stage: its five vents' fountains rise and fall
/// together, the northern three buried until Scoop digs them out; Felix rides
/// a risen fountain as a step up, and can jump onto the block.
void TrialRoad_SetUpFountainStage(void) {
  struct TrialRoadFountain *fountain;

  TrialRoad_StartFountains(0, FOUNTAIN_PERIOD);
  fountain =
      TrialRoad_AddFountain(0, ACTOR_WEST_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
  fountain->periodFrames = FOUNTAIN_PERIOD;
  fountain->riseFrame = FOUNTAIN_RISE_AT;
  fountain->fallFrame = FOUNTAIN_FALL_AT;
  fountain =
      TrialRoad_AddFountain(1, ACTOR_MIDDLE_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
  fountain->periodFrames = FOUNTAIN_PERIOD;
  fountain->riseFrame = FOUNTAIN_RISE_AT;
  fountain->fallFrame = FOUNTAIN_FALL_AT;
  fountain =
      TrialRoad_AddFountain(2, ACTOR_EAST_VENT, 0, 0, NORTH_FOUNTAIN_HEIGHT);
  fountain->periodFrames = FOUNTAIN_PERIOD;
  fountain->riseFrame = FOUNTAIN_RISE_AT;
  fountain->fallFrame = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(3, ACTOR_SOUTHWEST_VENT, 0, 0,
                                   SOUTH_FOUNTAIN_HEIGHT);
  fountain->periodFrames = FOUNTAIN_PERIOD;
  fountain->riseFrame = FOUNTAIN_RISE_AT;
  fountain->fallFrame = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(4, ACTOR_SOUTHEAST_VENT, 0, 0,
                                   SOUTH_FOUNTAIN_HEIGHT);
  fountain->periodFrames = FOUNTAIN_PERIOD;
  fountain->riseFrame = FOUNTAIN_RISE_AT;
  fountain->fallFrame = FOUNTAIN_FALL_AT;
  if (!Event_GetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_WEST_VENT))
    Actor_SetPosition(ACTOR_WEST_VENT, 0, 0);
  else
    TrialRoad_OpenVentCell(ACTOR_WEST_VENT);
  if (!Event_GetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_MIDDLE_VENT))
    Actor_SetPosition(ACTOR_MIDDLE_VENT, 0, 0);
  else
    TrialRoad_OpenVentCell(ACTOR_MIDDLE_VENT);
  if (!Event_GetFlag(FLAG_VENT_UNCOVERED_BASE + ACTOR_EAST_VENT))
    Actor_SetPosition(ACTOR_EAST_VENT, 0, 0);
  else
    TrialRoad_OpenVentCell(ACTOR_EAST_VENT);
  Map_SetUpperLevelWading(Ram_HeapSlots->mapGrid);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_SetJumpCellUnder(ACTOR_PILLAR_A);
}

/// Readies the sand stage: its tightrope between two posts, and the vine on
/// the cliff, climbable once Whirlwind has blown its weeds away.
void TrialRoad_SetUpSandStage(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_WEST_POST, ACTOR_EAST_POST);
  if (!Event_GetFlag(FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetPosition(ACTOR_SAND_STAGE_WEEDS, 0, 0);
}

/// The cave beyond the summit needs no setting up.
void TrialRoad_SetUpCave(void) {}

/// Gasp moves opposite to Felix about the hidden tile at cell (18, 15):
/// whatever way Felix steps, Gasp steps the other. Once Felix stands on that
/// tile, Gasp cries out and hops away to cell (21, 16), there to be talked
/// to.
void TrialRoad_GaspFlees(void) {
  struct Actor *gasp = Actor_GetById(ACTOR_GASP);
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 dx;
  s32 dz;
  s32 gaspDistanceSquared;
  s32 spotDistanceSquared;
  FIXED y;

  if (Event_GetFlag(FLAG_GASP_FLED))
    return;
  dx = FIXEDtoINT(leader->x.fixed - gasp->x.fixed);
  dz = FIXEDtoINT(leader->z.fixed - gasp->z.fixed);
  gaspDistanceSquared = dx * dx + dz * dz;
  dx = (FIXEDtoINT(leader->x.fixed)) - 296;
  dz = (FIXEDtoINT(leader->z.fixed)) - 248;
  spotDistanceSquared = dx * dx + dz * dz;
  if (spotDistanceSquared <= 80 * 80 || gaspDistanceSquared <= 48 * 48)
    Actor_SetMoveTarget(gasp, toFIXED(296 - dx), gasp->y.fixed,
                        toFIXED(248 - dz));
  if (spotDistanceSquared >= 4 * 4)
    return;
  Event_PlaySound(SE_ENEMY_CRY);
  Event_PlaySound(SE_DJINNI_FLEES);
  gasp->speedLimit = toFIXED(2);
  gasp->acceleration = toFIXED(2);
  gasp->motionFlags = ACTOR_FALLS;
  gasp->groundHeight = 0;
  gasp->collisionFlags = ACTOR_SOLID;
  gasp->velocityY = toFIXED(10);
  Actor_SetScript(ACTOR_GASP, gTrialRoad_GaspHopScript);
  y = gasp->y.fixed;
  Actor_SetMoveTarget(gasp, toFIXED(344), y, toFIXED(264));
  Event_SetFlag(FLAG_GASP_FLED);
}

/// Whether the palette cycle runs; the frame it has reached.
extern s32 gTrialRoad_PaletteCycleRunning;

/// Readies Gasp's pool room, unless Gasp has joined the party: he floats over
/// the water until he first hops away, and stands on the ground after; each
/// frame he mirrors Felix (TrialRoad_GaspFlees).
void TrialRoad_SetUpPoolRoom(void) {
  if (!Event_GetFlag(FLAG_DJINNI(DJINNI_GASP))) {
    struct Actor *actor = Actor_GetById(ACTOR_GASP);
    s32 started = Event_GetFlag(FLAG_GASP_FLED);
    u8 *flags = &actor->motionFlags;

    if (!started) {
      *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
      actor->y.fixed += toFIXED(17) / 2;
    } else {
      *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      actor->y.fixed = 0;
    }
    Task_Add(TrialRoad_GaspFlees, TASK_PASS_FRAME);
  }
}

/// Starts or stops the summit's colour cycle.
void TrialRoad_SetPaletteCycleRunning(s32 value) {
  gTrialRoad_PaletteCycleRunning = value;
}

extern s32 gTrialRoad_PaletteCycleFrame;

/// Sets the palette cycle's frame counter: the PaletteCycle_SetFrame of the
/// Jupiter and Mars Lighthouse rooms, whose palette cycles this one copies (the
/// name is ours).
static inline void PaletteCycle_SetFrame(s32 value) {
  gTrialRoad_PaletteCycleFrame = value;
}

/// Each frame: turns the summit's five purples over by one every four frames
/// and writes the first four of them to BG palette 5's colours 2 to 5; it
/// pauses while a Psynergy other than Hover, Reveal or the menu is up.
void TrialRoad_StepPaletteCycle(void) {
  u16 colors[16];
  s32 phase = (gTrialRoad_PaletteCycleFrame / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gTrialRoad_PaletteCycleRunning != 0) {
    struct FieldState *fieldState = Ram_HeapSlots->fieldState;
    s16 *state = &fieldState->psynergy;

    if (*state != 0 && ABILITY_ID(*state) != ABILITY_HOVER)
      return;
    if (fieldState->revealActive != 0)
      return;
    if (fieldState->pauseMenu != 0)
      return;
  }
  i = 0;
  offset = 4;
  do {
    p = colors;
    {
      s32 slot = (phase + i) % 5;
      u8 *table = (u8 *)gTrialRoad_PaletteCycleColors;

      colors[slot] = *(u16 *)(table + offset);
    }
    i++;
    offset += 2;
  } while (i <= 4);
  Dma_Set(p, (void *)&BG_PLTT_COLOR(5, 2), DMA_COPY_WORDS(2), REG_DMA3);
  gTrialRoad_PaletteCycleFrame++;
}

/// Starts the summit's colour cycle, loading its colours unless mode is 2;
/// mode 1 also starts its task. On the summit the purple stripes along the pale
/// stone channels that run down the cliff face, and the round purple Hover pad
/// at their foot, shimmer: their five shades of purple turn over every four
/// frames, so the light seems to pulse along the channels and ripple in the
/// pad's rings.
void TrialRoad_StartPaletteCycle(s32 mode) {
  gTrialRoad_PaletteCycleRunning = 1;
  if (mode != 2)
    Iwram_CopyWords(gTrialRoad_PaletteCycleColors, (void *)&BG_PLTT_COLOR(5, 0),
                    32);
  if (mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(TrialRoad_StepPaletteCycle, TASK_PASS_FRAME);
  }
}

void TrialRoad_LeaveMoapaWaiting(void);

/// Readies the summit: after the battle, Moapa's surrender or his scorn;
/// during the race, Moapa and his knights waiting at the top if they got there
/// first, or nobody yet.
void TrialRoad_SetUpPeak(void) {
  s32 entrance = gMainRoutineState.entrance;

  if (entrance == TRIAL_ROAD_SUMMIT_AFTER_WIN) {
    InventorySnapshot_Restore();
    TrialRoad_BeatMoapa();
    Import_EventSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_WIN);
  } else if (entrance == TRIAL_ROAD_SUMMIT_AFTER_DEFEAT) {
    InventorySnapshot_Restore();
    TrialRoad_LoseToMoapa();
    Import_EventSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
  } else if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
    if (Event_GetFlag(FLAG_SUMMIT_DOORWAY_SHUT))
      TrialRoad_ShutSummitDoorway(0);
    if (Event_GetFlag(FLAG_PARTY_FIRST_AT_PEAK))
      TrialRoad_LeaveMoapaWaiting();
    else
      TrialRoad_PlaceMoapaAtPeak();
  }
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  TrialRoad_StartPaletteCycle(1);
  TrialRoad_AddPillarTop(ACTOR_PEAK_PILLAR);
  if (Event_GetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED))
    Actor_SetPosition(ACTOR_PEAK_PILLAR, 136, 328);
}

/// Each stage pattern's set-up, by the pattern's low three bits.
extern void (*gTrialRoad_StageSetUps[])(void);

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineState.next;
  s32 entrance = gMainRoutineState.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (gFieldState->exit != 0)
    return 0;
  if (scene == SCENE_TRIAL_ROAD_STAGES) {
    if (entrance <= 7) {
      Event_SetFlag(FLAG_RETREAT_BLOCKED);
      Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
      Value0((s32 (*)())gTrialRoad_StageSetUps[pattern & 7]);
    } else if (entrance == TRIAL_ROAD_RULES_COURSE) {
      TrialRoad_ShowTraps();
    }
  }
  if (scene == SCENE_TRIAL_ROAD_CAVE_ROCK)
    TrialRoad_SetUpCave();
  if (scene == SCENE_TRIAL_ROAD_GASP)
    TrialRoad_SetUpPoolRoom();
  if (scene == SCENE_TRIAL_ROAD_SUMMIT) {
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
    TrialRoad_SetUpPeak();
  }
  return 0;
}

/// Copies a rectangle of map cells and their shapes, last row first, so that
/// overlapping rectangles copy whole.
void TrialRoad_CopyCellsBackward(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 dstX, s32 dstY) {
  u32 *cells = (u32 *)Ram_MapCellBuffer;
  s32 dst = (dstY << 7) + dstX;
  u8 *dstShapes = gMapShapeGrid + dst;
  s32 src = (srcY << 7) + srcX;
  u8 *srcShapes = gMapShapeGrid + src;
  u32 *dstCells = cells + dst;
  s16 row;
  s16 col;

  cells += src;
  for (row = height - 1; row >= 0; row--) {
    u32 *dstCell = dstCells + ((row << 7) + width) - 1;
    u32 *srcCell = cells + ((row << 7) + width) - 1;
    u8 *dstShape = dstShapes + (row << 7) + width - 1;
    u8 *srcShape = srcShapes + (row << 7) + width - 1;

    for (col = width - 1; col >= 0; col--) {
      *dstCell-- = *srcCell--;
      *dstShape = *srcShape;
      dstShape--;
      srcShape--;
    }
  }
}

void TrialRoad_OpenRivalDoor(s32 mirrored, s32 stage);

/// Each frame of the race, moves Moapa's party on by its recording, and opens
/// his door when he reaches it.
void TrialRoad_UpdateRival(void) {
  struct TrialRoadState *state = gTrialRoadState;
  struct TrialRoadState *road = gTrialRoadState;
  s32 side;

  if (state->stage[1] > 3)
    return;
  if (TrialRoad_RivalIsWaiting() != 0) {
    if (state->rivalTimer == 0) {
      side = TrialRoad_RivalIsAhead(1) ^ 1;
      TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[side + 8]);
      // Moapa's time at the chests counts from 1 when he reached the switch
      // first, with one chest to fill, and from 601 when Felix did, with two.
      state->rivalTimer = side * 5 * 120 + 1;
    } else {
      if (state->stage[1] >= state->rivalBest)
        state->rivalBest = state->stage[1] + 1;
      if (++road->stage[1] <= 3) {
        s32 pattern =
            state->order[state->stage[1] * 2 + (s16)(state->mirrored ^ 1)];

        TrialRoad_PlayRivalRecording(gTrialRoad_RivalRecordings[pattern & 7]);
        Actor_GetById(ACTOR_MOAPA)->groundHeight = 0;
        Actor_GetById(ACTOR_MOAPA)->y.fixed = 0;
      } else {
        Actor_SetPosition(ACTOR_MOAPA, 0, 0);
      }
      state->rivalTimer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(ACTOR_MOAPA)->spriteStorageKind;

    if (state->stage[0] == state->stage[1])
      *kind = 1;
    else
      *kind = 0;
  }
  if (state->rivalTimer == 0)
    return;
  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
    return;
  if (state->stage[0] == state->stage[1]) {
    switch (state->rivalTimer) {
    case 1:
    case 601:
      Sound_Play(SE_CHEST_OPEN);
      if (state->mirrored != 0) {
        Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 6);
        if (state->rivalTimer > 600)
          Actor_SetAnimationById(ACTOR_LEFT_OUTER_CHEST, 6);
      } else {
        Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 6);
        if (state->rivalTimer > 600)
          Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 6);
      }
      break;
    case 655:
      Sound_Play(SE_CHEST_CLOSE);
      if (state->mirrored != 0)
        Actor_SetAnimationById(ACTOR_LEFT_OUTER_CHEST, 5);
      else
        Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(ACTOR_MOAPA)->facing = FACING_NORTH;
      break;
    case 50:
    case 710:
      Sound_Play(SE_CHEST_CLOSE);
      if (state->mirrored != 0)
        Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 5);
      else
        Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 5);
      break;
    case 70:
    case 730:
      TrialRoad_OpenRivalDoor(state->mirrored, state->stage[1]);
      Sound_Play(SE_ITEM_BREAK);
      break;
    }
  }
  road->rivalTimer++;
}

extern s16 gTrialRoad_StageMapCells[][2];
extern s8 gTrialRoad_RulesTreasureStages[];
extern s8 gTrialRoad_TreasureStages[];

void TrialRoad_StartRival(s32 mirrored);

/// Places `layer` `column` cells into the map: its offset (column << 20), its
/// cells and terrain from that column on, and its position from the view.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = FIELD_CELL_ORIGIN(column);                                        \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulFX(*viewX, words[4]) + words[2];                         \
  words[1] = Iwram_MulFX(*viewZ, words[5]) + words[3]

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineState.next;
  s32 entrance = gMainRoutineState.entrance;

  if (scene == SCENE_TRIAL_ROAD_STAGES) {
    struct MapState *map = Ram_HeapSlots->mapGrid;
    struct TrialRoadState *state = gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    u32 i;

    if ((u32)(entrance -
              TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST) <= 1) {
      if (!GameFlag_Get(FLAG_STAGE_ORDER_DEALT)) {
        unit = Unit_Get(gMainRoutineState.leaderActorId);
        Iwram_ClearWordsEntry(state, sizeof(*state));
        value = 0;
        for (n = 0; n < UNIT_NAME_SIZE - 1; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(FLAG_STAGE_ORDER_DEALT);
      }
      state->mirrored = (entrance - 1) & 1;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      value = state->order[state->mirrored ^ 1];
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      // From the village, in at the foot of the left or right path.
      n = entrance % 10;
      Event_SetDestination(SCENE_TRIAL_ROAD_STAGES, n);
    } else if ((u32)(entrance -
                     TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST) <= 1) {
      if (!GameFlag_Get(FLAG_STAGE_ORDER_DEALT)) {
        unit = Unit_Get(gMainRoutineState.leaderActorId);
        Iwram_ClearWordsEntry(state, sizeof(*state));
        value = 0;
        for (n = 0; n < UNIT_NAME_SIZE - 1; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(FLAG_STAGE_ORDER_DEALT);
      }
      {
        s16 side = (entrance - 1) & 1;

        state->mirrored = side;
        state->stage[0] = 3;
        state->stage[1] = 3;
        state->leaderBest = 4;
        state->rivalBest = 4;
        {
          // The rival runs stage 3 on the other side of the course.
          s32 rivalSlot = 3 * 2 + (side ^ 1);

          value = state->order[rivalSlot];
        }
      }
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      // From the summit, in at the top of the left or right path.
      n = entrance % 10;
      Event_SetDestination(SCENE_TRIAL_ROAD_STAGES, n);
    } else if (entrance == TRIAL_ROAD_STAGES_FOR_TRIAL_RULES) {
      unit = Unit_Get(gMainRoutineState.leaderActorId);
      Iwram_ClearWordsEntry(state, sizeof(*state));
      value = 0;
      for (n = 0; n < UNIT_NAME_SIZE - 1; n++)
        value += unit[n];
      TrialRoad_DealStageOrder(value);
      state->mirrored = 0;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      state->pattern = state->order[0];
      Event_SetDestination(SCENE_TRIAL_ROAD_STAGES, TRIAL_ROAD_RULES_COURSE);
    } else {
      s32 *viewX = &map->viewX;
      s32 *viewZ = &map->viewZ;
      struct MapLayer *layer;
      s32 *words;
      s32 row;

      // The map holds the room three times, for layers 0 to 2, 34 cells
      // apart, with the course patterns south of them: each redraw below
      // is made on all three. Rows 0 to 12 are the stage's top (the doors,
      // the chests and the switch), rows 13 to 33 the arena that a pattern
      // fills, and rows 34 to 43 its foot.
      if (entrance == TRIAL_ROAD_RULES_COURSE) {
        s32 x1;
        s32 x2;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_LEFT_INNER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_LEFT_OUTER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_RIGHT_INNER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_RIGHT_OUTER_CHEST), 3);
        // The course Moapa shows: the arena is cleared, each copy of the
        // blank cell at its west edge carried across it, and the dealt
        // pattern drawn from column 1.
        Map_CopyRawCellsRect(0, 12, 1, 21, 0, 13);
        Map_CopyRawCellsRect(x1, 12, 1, 21, x1, 13);
        Map_CopyRawCellsRect(x2, 12, 1, 21, x2, 13);
        Map_CopyRawCellsRect(0, 13, 33, 21, 1, 13);
        Map_CopyRawCellsRect(x1, 13, 33, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(x2, 13, 33, 21, x2 + 1, 13);
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 1, 13);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, x1 + 1, 13);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, x2 + 1, 13);
        for (i = 0; i <= 9; i++) {
          if (gTrialRoad_RulesTreasureStages[i] != value)
            Actor_SetPosition(i + FIELD_TREASURE_ACTOR, 0, 0);
        }
        MapGrid_CenterOnFollowedPoint();
      } else {
        s32 x1;
        s32 x2;
        s32 destinationY;

        row = 0;
        layer = &map->layers[1];
        SET_LAYER(34, x1);
        layer = &map->layers[2];
        SET_LAYER(68, x2);
        value = 1;
        // The room is drawn for the leader's path: from the left path the
        // top and the foot slide
        // 9 cells east, and from the right path 10 cells west with the
        // four blue chests following; the strip either leaves is blanked
        // by carrying a blank cell across it.
        if (entrance & value) {
          TrialRoad_CopyCellsBackward(0, 0, 102, 13, 9, 0);
          Map_CopyRawCellsRect(0, 0, 10, 1, 1, 0);
          Map_CopyRawCellsRect(0, 0, 10, 12, 0, 1);
          Map_CopyRawCellsRect(x1, 0, 10, 12, x1, 1);
          Map_CopyRawCellsRect(x2, 0, 10, 12, x2, 1);
          TrialRoad_CopyCellsBackward(0, 34, 102, 10, 9, 34);
          Map_CopyRawCellsRect(0, 33, 10, 10, 0, 34);
          Map_CopyRawCellsRect(x1, 33, 10, 10, x1, 34);
          Map_CopyRawCellsRect(x2, 33, 10, 10, x2, 34);
        } else {
          Map_CopyRawCellsRect(10, 0, 92, 13, 0, 0);
          Map_CopyRawCellsRect(23, 0, 10, 1, 24, 0);
          Map_CopyRawCellsRect(23, 0, 10, 12, 23, 1);
          Map_CopyRawCellsRect(57, 0, 10, 12, 57, 1);
          Map_CopyRawCellsRect(91, 0, 10, 12, 91, 1);
          Map_CopyRawCellsRect(10, 34, 92, 10, 0, 34);
          Map_CopyRawCellsRect(24, 33, 10, 10, 24, 34);
          Map_CopyRawCellsRect(58, 33, 10, 10, 58, 34);
          Map_CopyRawCellsRect(92, 33, 10, 10, 92, 34);
          Actor_SetPosition(ACTOR_LEFT_INNER_CHEST, 40, 152);
          Actor_SetPosition(ACTOR_LEFT_OUTER_CHEST, 24, 152);
          Actor_SetPosition(ACTOR_RIGHT_INNER_CHEST, 184, 152);
          Actor_SetPosition(ACTOR_RIGHT_OUTER_CHEST, 200, 152);
        }
        Actor_SetSpriteColor(Actor_GetById(ACTOR_LEFT_INNER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_LEFT_OUTER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_RIGHT_INNER_CHEST), 3);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_RIGHT_OUTER_CHEST), 3);
        // The stage's own pattern fills the arena from column 10, and the
        // other stages' treasures are put away.
        destinationY = 13;
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 10, destinationY);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, 44, destinationY);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, 78, destinationY);
        for (i = 0; i <= 9; i++) {
          if (gTrialRoad_TreasureStages[i] != value)
            Actor_SetPosition(i + FIELD_TREASURE_ACTOR, 0, 0);
        }
        MapGrid_CenterOnFollowedPoint();
        leader = Actor_GetById(gMainRoutineState.leaderActorId);
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
          leader->y.fixed = leader->groundHeight =
              Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed);
        if (GameFlag_Get(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
          struct TrialRoadState *road;

          GameFlag_Set(FLAG_RETREAT_BLOCKED);
          road = gTrialRoadState;
          if (road->stage[0] < road->leaderBest ||
              GameFlag_Get(FLAG_STAGE_DOOR_OPEN)) {
            TrialRoad_OpenLeaderDoor(road->mirrored, road->stage[0]);
            GameFlag_Set(FLAG_STAGE_DOOR_OPEN);
            GameFlag_Set(FLAG_STAGE_CHESTS_OPENED);
          }
          if (road->stage[0] < road->rivalBest)
            TrialRoad_OpenRivalDoor(road->mirrored, road->stage[0]);
          TrialRoad_ShowRaceMap(gMainRoutineState.leaderActorId, ACTOR_MOAPA);
          TrialRoad_StartRival((s16)(state->mirrored ^ 1));
          Task_Add(TrialRoad_UpdateRival, TASK_PASS_FRAME);
        } else {
          TrialRoad_OpenLeaderDoor(state->mirrored, state->stage[0]);
          GameFlag_Set(FLAG_STAGE_DOOR_OPEN);
          GameFlag_Set(FLAG_STAGE_CHESTS_OPENED);
          TrialRoad_OpenRivalDoor(state->mirrored, state->stage[0]);
        }
      }
    }
  } else if (scene == SCENE_TRIAL_ROAD_CAVE_ROCK) {
  } else if (scene == SCENE_TRIAL_ROAD_GASP) {
  } else if (scene == SCENE_TRIAL_ROAD_SUMMIT &&
             GameFlag_Get(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT) &&
             !GameFlag_Get(FLAG_PARTY_FIRST_AT_PEAK)) {
    // During the race, the doorway of the path the party did not take is
    // drawn shut, as TrialRoad_ShutSummitDoorway shuts the party's own.
    if (gTrialRoadState->mirrored != 0)
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

extern s8 gTrialRoad_KeyStages[];

/// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
/// eight slots at the start of the stage state, again until both the even and
/// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
/// the random state is restored after.
void TrialRoad_DealStageOrder(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomSeed;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomSeed = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gTrialRoad_KeyStages[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomSeed = saved;
}

/// The stage a side of the race has reached: 0 the leader's, 1 Moapa's.
s32 TrialRoad_GetStage(s32 side) {
  struct TrialRoadState *state;

  state = gTrialRoadState;
  return state->stage[side & 1];
}

/// Whether the leader took the right path, on which the road is mirrored.
s32 TrialRoad_IsMirrored(void) { return gTrialRoadState->mirrored; }

/// A pixel x on the mirrored road, the right path's, when the leader took it.
s32 TrialRoad_MirrorX(s32 x) {
  if (TrialRoad_IsMirrored())
    return 512 - x;
  return x;
}

/// A cell x, in pixels, on the mirrored road when the leader took it.
s32 TrialRoad_MirrorCellX(s32 x) {
  if (TrialRoad_IsMirrored())
    return (64 - x) << 3;
  return x << 3;
}

/// An x offset of so many 8-pixel steps, in pixels, turned round on the
/// mirrored road.
s32 TrialRoad_MirrorOffset(s32 x) {
  if (TrialRoad_IsMirrored())
    return -x << 3;
  return x << 3;
}

/// An angle on the mirrored road, turned round when it is.
s32 TrialRoad_MirrorAngle(s32 angle) {
  if (TrialRoad_IsMirrored())
    return (DEGtoANG(180) - angle) & TRIG_ANGLE_MASK;
  return angle & TRIG_ANGLE_MASK;
}

/// Places an actor at a pixel position and facing, mirrored when the leader
/// took the right path.
void TrialRoad_PlaceActor(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetPositionAndFacing(id, toFIXED(TrialRoad_MirrorX(x)), toFIXED(z),
                             (u16)TrialRoad_MirrorAngle(angle));
}

/// Felix got to the summit first, so Moapa is not placed yet; he arrives
/// when Felix walks up (TrialRoad_ReachPeakFirst).
void TrialRoad_LeaveMoapaWaiting(void) {}

/// Places Moapa and his two knights at the summit, waiting for the party.
void TrialRoad_PlaceMoapaAtPeak(void) {
  TrialRoad_PlaceActor(ACTOR_MOAPA, 288, 264, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_LEFT_KNIGHT, 296, 248, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_RIGHT_KNIGHT, 296, 280, FACING_WEST);
}

void Actor_PlaceOnActor(s32 actor, s32 value);

/// The camera pans to the summit, and the party walks up to its near side and
/// turns to face the far one, where Moapa stands or will land.
static inline void TrialRoad_GatherAtPeak(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 264, 1);
  Actor_PlaceOnActor(ACTOR_JENNA, ACTOR_FELIX);
  Actor_PlaceOnActor(ACTOR_SHEBA, ACTOR_FELIX);
  Actor_PlaceOnActor(ACTOR_PIERS, ACTOR_FELIX);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_WalkTo(ACTOR_FELIX, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(ACTOR_PIERS, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(ACTOR_JENNA, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(ACTOR_SHEBA, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, TrialRoad_MirrorAngle(FACING_EAST), 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_FaceDirection(ACTOR_PIERS, TrialRoad_MirrorAngle(FACING_EAST), 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, TrialRoad_MirrorAngle(FACING_EAST), 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_SHEBA, TrialRoad_MirrorAngle(FACING_EAST), 0);
  FieldCamera_WaitForArrival();
}

/// The party gathers at the summit first, and Moapa and his knights arrive
/// after them: "You were quite swift! You may prove a challenge yet!"
void TrialRoad_ReachPeakFirst(void) {
  struct Actor *actor;

  TrialRoad_GatherAtPeak();
  wait(20);
  Event_PlaySound(SE_ITEM_BREAK);
  wait(20);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 344, 296, FACING_NORTH);
  actor = Actor_GetById(ACTOR_MOAPA);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  actor->motionFlags = 0;
  Actor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed + toFIXED(88),
                      actor->z.fixed);
  Actor_SelectAnimation(actor, 2);
  FieldActor_WaitForMove(actor);
  Actor_SelectAnimation(actor, 1);
  actor->motionFlags = ACTOR_FALLS;
  actor->groundHeight = toFIXED(128);
  actor->velocityY = toFIXED(4);
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  Actor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed,
                      actor->z.fixed - toFIXED(16));
  FieldActor_WaitForMove(actor);
  wait(12);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(actor, toFIXED(TrialRoad_MirrorCellX(41)),
                             actor->y.fixed, toFIXED(264));
  FieldActor_WaitForMove(actor);
  wait(6);
  Event_PlaySound(SE_JUMP);
  actor->velocityY = toFIXED(6);
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  Actor_SetMoveTarget(actor, toFIXED(TrialRoad_MirrorCellX(37)), actor->y.fixed,
                      toFIXED(264));
  FieldActor_WaitForMove(actor);
  wait(6);
  Actor_PlaceOnActor(ACTOR_LEFT_KNIGHT, ACTOR_MOAPA);
  Actor_PlaceOnActor(ACTOR_RIGHT_KNIGHT, ACTOR_MOAPA);
  Actor_SetSpeed(ACTOR_MOAPA, 1);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 1);
  Actor_SetSpeed(ACTOR_RIGHT_KNIGHT, 1);
  Actor_WalkTo(ACTOR_MOAPA, TrialRoad_MirrorCellX(36), 264);
  Actor_WalkTo(ACTOR_LEFT_KNIGHT, TrialRoad_MirrorCellX(37), 248);
  Actor_WalkTo(ACTOR_RIGHT_KNIGHT, TrialRoad_MirrorCellX(37), 280);
  Actor_WaitForMoveById(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_WaitForMoveById(ACTOR_LEFT_KNIGHT);
  Actor_FaceDirection(ACTOR_LEFT_KNIGHT, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_WaitForMoveById(ACTOR_RIGHT_KNIGHT);
  Actor_FaceDirection(ACTOR_RIGHT_KNIGHT, TrialRoad_MirrorAngle(FACING_WEST),
                      0);
  Event_SetMessageId((s32)MsgTrialRoadSwiftChallenge);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  // "You were quite swift! You may prove a challenge yet!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "However, just because you're fast doesn't mean you're strong...."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 2);
  // "Let the battle begin!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(15);
  Actor_SetSpeed(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_JENNA, 1.9);
  Actor_SetSpeed(ACTOR_SHEBA, 1.7);
  Actor_SetSpeed(ACTOR_PIERS, 1.8);
  Actor_SetSpeed(ACTOR_MOAPA, 2);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 1.9);
  Actor_SetSpeed(ACTOR_RIGHT_KNIGHT, 1.7);
  Actor_WalkBy(ACTOR_FELIX, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_MOAPA, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_JENNA, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_LEFT_KNIGHT, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_PIERS, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_RIGHT_KNIGHT, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_SHEBA, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

/// Moapa and his knights are already waiting when the party reaches the
/// summit: "You may have been slow, but you made it through Trial Road".
void TrialRoad_ReachPeakSecond(void) {
  TrialRoad_GatherAtPeak();
  Event_SetMessageId((s32)MsgTrialRoadSlowArrival);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 10);
  // "You may have been slow, but you made it through Trial Road to the..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "Well, then... The time is nigh for us to decide who is stronger!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 2);
  // "Let the battle begin!"
  Event_ShowMessage(ACTOR_MOAPA, 0);
  // The same charge as TrialRoad_ReachPeakFirst's: written once and shared,
  // it does not compile to the ROM's code.
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(15);
  Actor_SetSpeed(ACTOR_FELIX, 2);
  Actor_SetSpeed(ACTOR_JENNA, 1.9);
  Actor_SetSpeed(ACTOR_SHEBA, 1.7);
  Actor_SetSpeed(ACTOR_PIERS, 1.8);
  Actor_SetSpeed(ACTOR_MOAPA, 2);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 1.9);
  Actor_SetSpeed(ACTOR_RIGHT_KNIGHT, 1.7);
  Actor_WalkBy(ACTOR_FELIX, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_MOAPA, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_JENNA, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_LEFT_KNIGHT, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_PIERS, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_RIGHT_KNIGHT, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_SHEBA, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

void PartyInventory_Discard(s32 cue);

/// After the party beats Moapa at the summit, he admits their strength and
/// gives them the Hover Jade for the Shaman's Rod, and the party goes back to
/// the Shaman Village to rest.
void TrialRoad_BeatMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  TrialRoad_PlaceActor(ACTOR_FELIX, 248, 272, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_PIERS, 224, 248, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_JENNA, 232, 264, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_SHEBA, 224, 280, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_KRADEN, 248, 256, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 272, 264, FACING_SOUTH);
  TrialRoad_PlaceActor(ACTOR_LEFT_KNIGHT, 288, 248, FACING_SOUTH);
  TrialRoad_PlaceActor(ACTOR_RIGHT_KNIGHT, 288, 280, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_MOAPA, 6);
  Actor_SetAnimation(ACTOR_LEFT_KNIGHT, 6);
  Actor_SetAnimation(ACTOR_RIGHT_KNIGHT, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(ACTOR_LEFT_KNIGHT, 2);
  wait(4);
  Actor_StartRepeatedMotion(ACTOR_RIGHT_KNIGHT, 2);
  wait(10);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 2);
  // "All right, all right. You've proven your strength, and you are..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 7);
  Event_Wait(15);
  Actor_SetAnimation(ACTOR_LEFT_KNIGHT, 7);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_RIGHT_KNIGHT, 7);
  Event_Wait(5);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  Event_Wait(15);
  Actor_FaceDirection(ACTOR_MOAPA, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_SetAnimation(ACTOR_LEFT_KNIGHT, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_LEFT_KNIGHT, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_SetAnimation(ACTOR_RIGHT_KNIGHT, 1);
  Event_Wait(5);
  Actor_FaceDirection(ACTOR_RIGHT_KNIGHT, TrialRoad_MirrorAngle(FACING_WEST),
                      0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "As we promised, you will receive the Hover Jade, and we will take..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetSpeed(ACTOR_MOAPA, 1);
  Actor_WalkTo(ACTOR_MOAPA, TrialRoad_MirrorCellX(32), 264);
  Actor_FaceEachOther(ACTOR_KRADEN, ACTOR_PIERS, 0);
  Actor_FaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
  Actor_WaitForMoveById(ACTOR_MOAPA);
  Actor_FaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
  Event_Wait(20);
  PartyInventory_Discard(ITEM_SHAMANS_ROD);
  Party_GiveItem(ITEM_HOVER_JADE, 0);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_MOAPA, 0);
  Actor_GetById(ACTOR_MOAPA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MOAPA, TrialRoad_MirrorCellX(35), 264);
  Event_Wait(1);
  Actor_GetById(ACTOR_MOAPA)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  FieldActor_ShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 10);
  // "After many years, the vow that Hoabna made to Yegelos has at last..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, TrialRoad_MirrorAngle(FACING_EAST));
  Actor_FaceActor(ACTOR_LEFT_KNIGHT, ACTOR_MOAPA, 0);
  Actor_FaceActor(ACTOR_RIGHT_KNIGHT, ACTOR_MOAPA, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "I'm tired now. I'll be back in the Shaman Village resting."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_LEFT_KNIGHT, 3);
  wait(3);
  Actor_SetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
  Actor_FaceActor(ACTOR_FELIX, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  // "I'm sure you must be exhausted as well. Let's go back to the village..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  wait(2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  wait(1);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  wait(30);
}

/// After the party loses to Moapa at the summit: "You've lost. Now leave
/// Shaman village."
void TrialRoad_LoseToMoapa(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 264, 264, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_LEFT_KNIGHT, 280, 248, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_RIGHT_KNIGHT, 280, 280, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_FELIX, 240, 276, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_PIERS, 216, 254, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_JENNA, 232, 244, FACING_EAST);
  TrialRoad_PlaceActor(ACTOR_SHEBA, 224, 296, FACING_EAST);
  // Felix alone is left on his feet, the others lying collapsed.
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_STILL);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_COLLAPSE);
  FieldActor_SetSpriteRotationById(ACTOR_PIERS, DEGtoANG(90));
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_JENNA), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PIERS), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadLeaveVillage);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "You've lost. Now leave Shaman village."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 2);
  wait(1);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 2);
  wait(2);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(15);
}

extern u32 gTrialRoad_LeftKnightLeavesScript[];

/// While Moapa explains the trial, the camera tours a stage: its traps, the
/// four doors, a knight showing how the switch opens the blue chests and how
/// filling them opens the door, and the give-up button.
void TrialRoad_ShowTraps(void) {
  s32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 1016);
  Actor_SetPosition(ACTOR_MOAPA, 1016, 1016);
  for (i = ACTOR_MOAPA + 1; i < FIELD_ACTOR_COUNT; i++) {
    struct Actor *actor = Actor_GetById(i);

    if (actor->x.fixed != 0)
      actor->x.fixed -= toFIXED(144);
  }
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetMovementTypeById(ACTOR_LEFT_KNIGHT,
                            gTrialRoad_LeftKnightEntersScript);
  Actor_SetMovementTypeById(ACTOR_RIGHT_KNIGHT,
                            gTrialRoad_RightKnightEntersScript);
  Event_StartScreenTransition();
  Event_Wait(20);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 600, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120, CAMERA_KEEP, 552, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(120, CAMERA_KEEP, 120, 1);
  Event_SetMessageId((s32)MsgTrialRoadTrapInstructions);
  // "The room is filled with traps and snares. Use the power of Contigo..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(216, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  // "This road has four doors. To open them, you will need to place items..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  // "Why?"
  Event_ShowMessageAndWait(ACTOR_FELIX, 0);
  Actor_SetMovementTypeById(ACTOR_LEFT_KNIGHT,
                            gTrialRoad_LeftKnightToChestScript);
  Actor_WaitForScriptEndById(ACTOR_LEFT_KNIGHT);
  Actor_SetAnimationById(ACTOR_LEFT_KNIGHT, 1);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 0.25);
  Actor_GetById(ACTOR_LEFT_KNIGHT)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_LEFT_KNIGHT, 0, 8);
  Event_Wait(1);
  Actor_GetById(ACTOR_LEFT_KNIGHT)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 2);
  Actor_SetAnimationById(ACTOR_LEFT_KNIGHT, 5);
  Actor_SetDestinationOffset(ACTOR_LEFT_KNIGHT, 0, -40);
  Actor_WaitForMoveById(ACTOR_LEFT_KNIGHT);
  Sound_Play(SE_CURSE);
  MapGrid_SetShake(2, 4, 1);
  Actor_GetById(ACTOR_LEFT_KNIGHT)->velocityY = toFIXED(4);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_GetById(ACTOR_LEFT_KNIGHT)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_LEFT_KNIGHT, 0, 16);
  Event_Wait(1);
  Actor_GetById(ACTOR_LEFT_KNIGHT)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(60);
  Actor_FaceDirectionById(ACTOR_LEFT_KNIGHT, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LEFT_KNIGHT, 4);
  // "The door is triggered only when the treasure chests are filled to..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_LEFT_KNIGHT, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_LEFT_KNIGHT, 3);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_LEFT_KNIGHT, 1);
  Actor_WalkByAndWait(ACTOR_LEFT_KNIGHT, -16, -16);
  Actor_FaceDirection(ACTOR_LEFT_KNIGHT, FACING_WEST, 0);
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 6);
  Event_Wait(30);
  // "Try putting heavier items, like weapons and tools, into the chests..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_WalkToAndWaitById(ACTOR_LEFT_KNIGHT, 200, 168);
  Actor_FaceDirection(ACTOR_LEFT_KNIGHT, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 5);
  Event_Wait(20);
  Sound_Play(SE_ITEM_BREAK);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(ACTOR_LEFT_KNIGHT,
                            gTrialRoad_LeftKnightLeavesScript);
  Actor_WaitForScriptEndById(ACTOR_LEFT_KNIGHT);
  Sound_Play(SE_MAP_EXIT);
  // "If you reach the door after your opponents do, you will be at a..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_SetMovementTypeById(ACTOR_RIGHT_KNIGHT,
                            gTrialRoad_RightKnightToChestsScript);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  // "You see, it will take twice the weight to open the doors."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  // "Stand on the switch, and the number of chests you need to fill will..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_RIGHT_KNIGHT, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_RIGHT_KNIGHT, 1);
  Actor_WalkByAndWait(ACTOR_RIGHT_KNIGHT, 16, -16);
  Actor_FaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_EAST, 0);
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 6);
  Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_RIGHT_KNIGHT, 350, 168);
  Actor_FaceDirection(ACTOR_RIGHT_KNIGHT, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_RIGHT_KNIGHT, 364, 168);
  Actor_FaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 5);
  Event_Wait(20);
  Sound_Play(SE_ITEM_BREAK);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_RIGHT_KNIGHT, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_RIGHT_KNIGHT, 3);
  // "If you are last to each of the four doors, you will lose eight items..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  // "If Trial Road proves too much for you, press this button."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_SetMovementTypeById(ACTOR_RIGHT_KNIGHT,
                            gTrialRoad_RightKnightLeavesScript);
  Actor_WaitForScriptEndById(ACTOR_RIGHT_KNIGHT);
  // "This ends the battle, though. A true hero would not surrender so easily."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Import_EventSetExitRequest(
      TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_D);
}

/// Moves the fountains each frame: at its rise phase a fountain starts up 4
/// pixels a frame, at its fall phase down 8, until it reaches its high or low
/// height; its phase runs round its period while the field handles no request.
/// Its cell's collision height follows it; while it is up the cell is water to
/// wade in, and while it also stands still a gap to jump onto; its top bobs on
/// the water.
void TrialRoad_UpdateFountains(void) {
  struct TrialRoadFountains *state = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *fountain = state->fountains;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 shape = state->shape;
  u8 *shapes = map->layers[0].terrainIds;
  u8 *cells = (u8 *)map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  for (collision = (s32 *)Ram_MapCollision + shape; i < FOUNTAIN_COUNT;
       collision++, i++, fountain++) {
    if (fountain->id == 0)
      continue;
    actor = Actor_GetById(fountain->id);
    if (actor->x.fixed == 0 && actor->z.fixed == 0)
      continue;
    if (fountain->phase == fountain->riseFrame) {
      Sound_Play(SE_CURSE_FALL);
      fountain->speed = 4;
    }
    if (fountain->phase == fountain->fallFrame) {
      Sound_Play(SOUND_FADE_OUT_EXIT_SE);
      fountain->speed = -8;
    }
    if (fountain->speed != 0) {
      fountain->height += fountain->speed;
      if (fountain->height >= fountain->high) {
        fountain->height = fountain->high;
        fountain->speed = 0;
        fountain->bob = 0;
      }
      if (fountain->height <= fountain->low) {
        fountain->height = fountain->low;
        fountain->speed = 0;
        fountain->bob = 0;
      }
    }
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      if (++fountain->phase >= fountain->periodFrames)
        fountain->phase = 0;
    }
    cell = (FIELD_CELL(actor->x.fixed)) + ((FIELD_CELL(actor->z.fixed)) << 7);
    shapes[cell] = (u8)state->shape + i;
    *collision = ((actor->y.part[0].integer + fountain->height) / 8) << 8;
    attributes = cells + cell * 4;
    if (fountain->speed != 0)
      attributes[3] = 0;
    else {
      s32 bits = MAP_CELL_JUMP;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (fountain->height != 0) {
      s32 bits = MAP_CELL_WATER;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = fountain->top;
    if (marker != 0) {
      if (fountain->height == 0) {
        marker->y.fixed = actor->y.fixed - toFIXED(16);
      } else {
        FIXED wave;
        fountain->bob += TRIG_TURN / 16;
        wave = Iwram_MulFX(toFIXED(2), Trig_Sin(fountain->bob)) - toFIXED(14);
        marker->y.fixed = actor->y.fixed + toFIXED(fountain->height) + wave;
      }
      marker->x.fixed = actor->x.fixed;
      marker->z.fixed = actor->z.fixed;
    }
  }
}
#undef SET_LAYER

/// Queues the fountains' sprites each frame: each risen fountain's column of
/// water, in 16-pixel segments up from its vent, and its spout bobbing on top.
void TrialRoad_DrawFountains(void) {
  struct TrialRoadFountains *base = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *fountain = base->fountains;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  FIXED rise;
  struct MapState *map;
  s32 *view;
  struct Actor *actor;
  u32 *entry;
  u32 *cursor;
  s32 segmentHeight;
  s32 x;
  s32 height;
  FIXED z;
  s32 screenY;
  s32 depth;
  FIXED bob;

  tile = gVramCacheEntries[base->vramEntry].offset >> 5;
  entry = base->sprites;
  count = 0;
  rise = -toFIXED(2);
  map = Ram_HeapSlots->mapGrid;
  view = &map->viewX;
  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  viewY = map->origin[1];

  for (i = 0; i < FOUNTAIN_COUNT; i++, fountain++) {
    if (fountain->id == 0 || fountain->height == 0)
      continue;
    actor = Actor_GetById(fountain->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    segmentHeight = 0;
    if (segmentHeight < fountain->height) {
      do {
        if (count >= FOUNTAIN_SPRITE_COUNT)
          continue;
        x = actor->x.fixed - viewX;
        height = actor->y.fixed + toFIXED(segmentHeight) - viewY;
        z = actor->z.fixed - viewZ - viewY;
        x = FIXEDtoINT(x) - 8;
        screenY = z - height;
        screenY = FIXEDtoINT(screenY) - 16;
        depth = FIXEDtoINT(height + z) + FOUNTAIN_DEPTH_OFFSET;
        if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
          continue;
        x &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        cursor = entry;
        *cursor++ = 0;
        *cursor++ = screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *cursor = (tile + frame) | OBJ_PRIORITY(2);
        Oam_AddSpriteAtDepth(entry, depth);
        entry += 3;
        count++;
      } while ((segmentHeight += 16) < fountain->height);
    }
    if (count >= FOUNTAIN_SPRITE_COUNT)
      continue;
    x = actor->x.fixed - viewX;
    height = actor->y.fixed + toFIXED(fountain->height) - viewY;
    z = actor->z.fixed - viewZ - viewY;
    x = FIXEDtoINT(x) - 8;
    screenY = z - height;
    bob = Iwram_MulFX(rise, Trig_Sin(fountain->bob));
    screenY = FIXEDtoINT(screenY + bob + rise) - 4;
    depth = FIXEDtoINT(height + z) + FOUNTAIN_DEPTH_OFFSET;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      continue;
    x &= OBJ_X_MASK;
    screenY &= OBJ_Y_MASK;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = screenY | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    *cursor = (tile + frame - 32) | OBJ_PRIORITY(2);
    Oam_AddSpriteAtDepth(entry, depth);
    entry += 3;
    count++;
  }
}

/// Allocates the fountains' working buffer.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

/// Starts the fountains' state and their task.
void TrialRoad_StartFountains(s32 unused, s32 shape) {
  struct TrialRoadFountains *state = TRIAL_ROAD_FOUNTAINS;
  void *buffer;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords(state, sizeof(*state));
  buffer = Pool_Alloc(2048);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  state->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(state->vramEntry, 2048, buffer);
  Heap_Release(buffer);
  state->sprites =
      Heap_AllocEwram(FOUNTAIN_SPRITE_COUNT * sizeof(struct TrialRoadSprite));
  state->shape = shape;
  Task_Add(TrialRoad_UpdateFountains, TASK_PASS_FRAME + 13);
  Task_Add(TrialRoad_DrawFountains, TASK_PASS_FRAME);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
}

/// Sets up one fountain on its vent, with its timer and height.
struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high) {
  struct TrialRoadFountains *state = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *fountain = &state->fountains[slot];
  struct Actor *actor = Actor_GetById(id);
  struct Actor *top;

  Actor_GetById(id);
  Actor_SetSpriteFlags(actor, 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    fountain->id = id;
    fountain->height = 0;
    fountain->phase = timer;
    fountain->periodFrames = 360;
    fountain->high = high;
    fountain->riseFrame = 60;
    fountain->fallFrame = 240;
    fountain->low = low;
    fountain->unusedAfterLow = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    fountain->speed = 0;
    fountain->bob = 0;
    fountain->unusedAfterSpeed = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  top = ((struct Actor *)Import_ActorCreate(
      ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE), actor->x.fixed, actor->y.fixed,
      actor->z.fixed));
  top->collisionFlags = 0;
  top->motionFlags = 0;
  top->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  fountain->top = top;
  return fountain;
}

#include "shared/pillar_restore_raised.h"

#include "shared/leader_check_crumbling_step.h"

/// A cracked pillar in the crumbling stage shakes, crumbles and falls away, as
/// Magma Rock's do.
void TrialRoad_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *actor = Actor_GetById(id);
  s32 flag;

  while (entry->actor != ACTOR_LIST_END) {
    if (entry->actor != id) {
      entry++;
      continue;
    }
    {
      flag = entry->flag;
      Event_Start();
      if (!Event_GetFlag(flag + 1)) {
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Dust_Scatter(0);
        wait(8);
        Actor_SelectAnimation(actor, 2);
        Actor_SetCellShape(-4, actor->layer, id);
        Dust_Scatter(1);
        wait(16);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        Actor_SetPositionAndResetMotion(actor, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRockInside_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag);
        Event_SetFlag(flag + 1);
      }
      Event_End();
      break;
    }
  }
}

#include "shared/pillar_take_out_of_play.c"

void Actor_PlaceOnActor(s32 id, s32 owner);
#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/// Unused: Magma Rock's trap floor, kept in this overlay: a cracked pillar
/// stepped on flashes the first time and gives way under Felix the second,
/// with Moapa's actor standing in for the trap's picture. The crumbling stage
/// uses Leader_CheckCrumblingStep instead.
UNUSED void TrialRoad_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  partner = Actor_GetById(ACTOR_MOAPA);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    while (*table != ACTOR_LIST_END) {
      id = table[0];
      if (Actor_GetById(id) != trap) {
        table += 2;
        continue;
      }
      flag = table[1];
      Event_Start();
      if (!GameFlag_Get(flag)) {
        Actor_SelectAnimation(partner, 1);
        Actor_TakePlaceOf(partner, trap);
        wait(1);
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_MOAPA), 7);
        wait(2);
        Actor_SelectAnimation(partner, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_MOAPA), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_MOAPA), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_MOAPA), 0);
        Dust_Scatter(0);
        Actor_TakePlaceOf(trap, partner);
        wait(1);
        GameFlag_Set(flag);
      } else {
        second = flag + 1;
        if (!GameFlag_Get(second)) {
          Actor_SelectAnimation(partner, 0);
          Actor_TakePlaceOf(partner, trap);
          motion = &leader->motionFlags;
          wait(1);
          *motion = 0;
          Sound_Play(SE_PUSH);
          MapGrid_SetShake(1, 4, 1);
          Dust_Scatter(0);
          wait(8);
          Actor_SelectAnimation(partner, 2);
          Leader_SetCellShapeAt(-2, trap->layer);
          Dust_Scatter(1);
          wait(16);
          Leader_SetCellShapeAt(-2, trap->layer);
          wait(4);
          MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
          wait(8);
          *motion = 3;
          wait(5);
          Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
          wait(2);
          Sound_Play(SE_THUMP);
          MagmaRockInside_SpawnDustRing();
          wait(20);
          GameFlag_Set(second);
        }
      }
      Event_End();
      return;
    }
  }
}

/// The dust's motion each frame, as
/// FieldEffect_AdvanceMotionAtLeaderPriority's: it drifts, slows, grows and
/// spins, drawn at the leader's sprite priority.
void TrialRoad_DriftDust(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineState *party;
  struct FieldSprite *sprite;
  union FieldObject *leader;
  struct EffectSpriteAttributes *to;
  struct EffectSpriteAttributes *from;
  struct EffectSpriteBits source;
  struct EffectSpriteBits bits;

  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;
  effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;
  {
    effect->scaleX += effect->scaleRateX;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineState;

  leader = Actor_GetById(party->leaderActorId);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

extern u32 gDust_ScatterScript[];

/// Eight clouds of dust puff up in a row across the leader's feet; with spread
/// set they fly out all round.
void Dust_Scatter(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i;

  for (i = 0; i <= 7; i++) {
    {
      // Each piece starts 0 to 13 pixels lower than the leader.
      FIXED y = leader->y.fixed - ((Random_Below(14)) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, i * toFIXED(2) + leader->x.fixed - toFIXED(8),
          y - toFIXED(2), leader->z.fixed);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gDust_ScatterScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i * (TRIG_TURN / 8)), FIXED_ONE * 12 / 10);
        effect->velocity.z = Trig_Sin(i * (TRIG_TURN / 8));
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + (toFIXED(3) / 8));
      effect->spin = ((Random16() << 9) >> 16) - (TRIG_TURN / 256);
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = TrialRoad_DriftDust;
    }
  }
}

extern u32 gDust_BurstScript[];
/// The clouds of dust MagmaRockInside_SpawnDustRing throws out in a ring.
#define DUST_RING_PIECES 17

/// Clouds of dust burst out in a ring round the leader as something lands with
/// a thump.
void MagmaRockInside_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  for (i = 0; i < DUST_RING_PIECES; i++) {
    effect = (struct FieldEffect *)Import_ActorCreate(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gDust_BurstScript);
      effect->velocity.x = Trig_Cos(i * (TRIG_TURN / 16));
      effect->velocity.y = 0;
      effect->velocity.z = Trig_Sin(i * (TRIG_TURN / 16));
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - (toFIXED(3) / 8);
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - (toFIXED(3) / 16);
      effect->spin = ((Random16() << 12) >> 16) - (TRIG_TURN / 32);
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = TrialRoad_DriftDust;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

extern u32 gTrialRoad_HexDigits[];

/// Unused: writes value as eight hexadecimal digits (gTrialRoad_HexDigits) and
/// a terminating null.
UNUSED void String_WriteU32AsHex(u8 *text, u32 value) {
  s32 i;
  const u8 *digits = gTrialRoad_HexDigits;

  text += 8;
  *text = 0;
  text--;
  for (i = 0; i < 8; i++) {
    *text = digits[value & 0xf];
    value >>= 4;
    text--;
  }
}

extern u8 gSceneState[];

/// Does nothing.
void TrialRoad_Idle(void) {}

/// Stops Moapa's party where it stands, as at the end of its recording.
void TrialRoad_StopRival(void) {
  ((struct TrialRoadReplay *)gSceneState)->mode = RIVAL_WAITING;
}

/// Waits until Moapa's party has finished its recording.
void TrialRoad_WaitForRival(void) {
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;

  while (state->mode != RIVAL_WAITING) {
    wait(1);
  }
}

/// The most the rival turns in a frame: a sixteenth of a turn.
#define RIVAL_MAX_TURN (TRIG_TURN / 16)

/// Updates the Trial Road rival by its mode: RIVAL_MIRROR follows the leader's
/// position mirrored across the course; RIVAL_LINKED places the rival and its
/// partner from the linked player's record; RIVAL_REPLAY replays the recorded
/// path; RIVAL_RECORD records the actor's position.
void TrialRoad_MoveRival(void) {
  struct TrialRoadLinkState *trialRoadLinkState =
      (struct TrialRoadLinkState *)Ram_HeapSlots->trialRoadLink;
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 x, z, turn;
  FIXED dx, dz;
  u16 heading;

  if (rival == 0)
    return;
  if (state->mode == RIVAL_MIRROR) {
    x = leader->x.fixed;
    z = leader->z.fixed;
    if ((u32)(z - toFIXED(256)) > toFIXED(304))
      x = toFIXED(528) - (x & FIELD_PIXEL_MASK);
    if (gMainRoutineState.entrance & 1)
      x += toFIXED(304);
    else
      x -= toFIXED(304);
    if (rival->x.fixed != x || rival->z.fixed != z) {
      dx = x - rival->x.fixed;
      dz = z - rival->z.fixed;
      heading = Trig_Atan(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > RIVAL_MAX_TURN)
        turn = RIVAL_MAX_TURN;
      if (turn < -RIVAL_MAX_TURN)
        turn = -RIVAL_MAX_TURN;
      rival->facing += turn;
      rival->x.fixed = x;
      rival->z.fixed = z;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == RIVAL_LINKED) {
    s32 player;
    s16 *signature;
    s32 angle, link, animation, partner;
    s16 packed;

    player = ((s32)GameFlag_GetByte(FLAG_LINK_PLAYER)) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    packed = *signature++;
    link = packed >> 8;
    animation = packed & 0xff;
    if (GameFlag_Get(dz = FLAG_LINKED_RIVAL_HALTS) && link != 0) {
      state->mode = RIVAL_WAITING;
      Actor_SelectAnimation(rival, 1);
      if (trialRoadLinkState->originX < rival->x.fixed)
        x = trialRoadLinkState->originX + toFIXED(12);
      else
        x = trialRoadLinkState->originX - toFIXED(12);
      if (!GameFlag_Get(FLAG_LINKED_RIVAL_ON_FAR_SIDE)) {
        z = trialRoadLinkState->originZ + toFIXED(16);
        rival->timer = trialRoadLinkState->rivalTimers[1];
      } else {
        z = trialRoadLinkState->originZ - toFIXED(16);
        rival->timer = trialRoadLinkState->rivalTimers[0];
      }
      rival->acceleration = toFIXED(1) / 4;
      rival->speedLimit = toFIXED(1);
      Actor_SetMoveTarget(rival, x, 0, z);
      Actor_StartIdle(rival);
    } else {
      x <<= 16;
      z <<= 16;
      rival->facing = angle;
      rival->x.fixed = x;
      rival->z.fixed = z;
      Actor_SelectAnimation(rival, animation);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    animation = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x.fixed = toFIXED(x);
    rival->facing = angle;
    rival->z.fixed = toFIXED(z);
    Actor_SelectAnimation(rival, animation);
  } else if (state->mode == RIVAL_REPLAY) {
    if (state->resource != trialRoadLinkState->resource) {
      Decompress_TaggedLz(Resource_GetTableEntry(state->resource),
                          trialRoadLinkState->path);
      trialRoadLinkState->resource = state->resource;
    }
    x = trialRoadLinkState->path[state->point];
    z = trialRoadLinkState->path[state->point + 1];
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
      state->point += 2;
    if (x == 0 && z == 0) {
      state->mode = RIVAL_WAITING;
      return;
    }
    x <<= 16;
    z <<= 16;
    if (state->mirrored != 0 && (u32)(z - toFIXED(256)) > toFIXED(304))
      x = toFIXED(528) - x;
    if (gMainRoutineState.entrance & 1)
      x += toFIXED(304);
    else
      x -= toFIXED(304);
    if (rival->x.fixed != x || rival->z.fixed != z) {
      dx = x - rival->x.fixed;
      dz = z - rival->z.fixed;
      heading = Trig_Atan(dz, dx);
      turn = (s16)(heading - rival->facing);
      if (turn > RIVAL_MAX_TURN)
        turn = RIVAL_MAX_TURN;
      if (turn < -RIVAL_MAX_TURN)
        turn = -RIVAL_MAX_TURN;
      rival->facing += turn;
      rival->x.fixed = x;
      rival->z.fixed = z;
      state->still = 0;
    } else {
      state->still++;
    }
    if (state->still > 2)
      Actor_SelectAnimation(rival, 1);
    else
      Actor_SelectAnimation(rival, 5);
  } else if (state->mode == RIVAL_RECORD) {
    x = FIXEDtoINT(rival->x.fixed);
    z = FIXEDtoINT(rival->z.fixed);
    trialRoadLinkState->path[state->point] = x;
    trialRoadLinkState->path[state->point + 1] = z;
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
      state->point += 2;
    if (state->point == RIVAL_PATH_LENGTH - 2) {
      u16 next = state->point + 1;

      trialRoadLinkState->path[state->point] = 0;
      trialRoadLinkState->path[(s16)next] = 0;
      state->id = trialRoadLinkState->rivalId;
      state->point = 0;
      state->mode = RIVAL_REPLAY;
    }
  }
}

/// The race map's picture, tagged LZ.
extern const u8 gTrialRoad_RaceMapPicture[];

/// The picture's buffer in EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(RACE_MAP_PICTURE_BYTES);
}

/// Where an actor's marker stands on the race map: its place in cells, moved by
/// `left` or `right` on the entrance's side, and raised by the stage it has
/// reached.
#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    s32 side = gMainRoutineState.entrance & 1;                                 \
    if (side) {                                                                \
      x = (actor->x.fixed - state->originX) / toFIXED(16) left;                \
      lift = TrialRoad_GetStage(which);                                        \
      y = (actor->z.fixed - state->originZ) / toFIXED(16) + state->scrollY -   \
          lift * RACE_MAP_STAGE_SPACING + 4;                                   \
    } else {                                                                   \
      x = (actor->x.fixed - state->originX) / toFIXED(16) right;               \
      lift = TrialRoad_GetStage(which);                                        \
      y = (actor->z.fixed - state->originZ) / toFIXED(16) + state->scrollY -   \
          lift * RACE_MAP_STAGE_SPACING + 4;                                   \
    }                                                                          \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= OBJ_X(x);                                                             \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) |                                           \
                (OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1));             \
    Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);                            \
  } while (0)

/// Draws the race map each frame, once TrialRoad_ShowRaceMap starts
/// it: the display slides in from both sides of the screen, and out while a
/// menu or a Psynergy is open. Each side shows the road's stages one above the
/// other, and on eleven frames of sixteen a marker shows where each party is:
/// "1P" for the leader's, "2P" for Moapa's.
void TrialRoad_DrawRaceMap(void) {
  struct TrialRoadLinkState *state = Ram_HeapSlots->trialRoadLink;
  u32 *cursor = (u32 *)state;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)state;
  s16 *vramEntry = &state->vramEntry;
  u32 row, i;
  s32 tile = gVramCacheEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 x;
  s32 y;
  u32 markerSize = OBJ_SIZE_16X16;
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct Actor *actor;

  if (state->holdFade != 0) {
    state->fade = 2;
  } else if (GameFlag_Get(FLAG_MENU_OPEN) || event->psynergy != 0) {
    if (state->fade > 0)
      state->fade--;
  } else if (state->fade <= 1) {
    if (++state->fade == 1) {
      void *buffer;
      Dma_Set(gTrialRoad_RaceMapPalette,
              (void *)&OBJ_PLTT_COLOR(RACE_MAP_PALETTE, 0),
              DMA_COPY_HALFWORDS(16), REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Decompress_TaggedLz(gTrialRoad_RaceMapPicture, buffer);
      VramCache_Load(*vramEntry, RACE_MAP_PICTURE_BYTES, buffer);
      Heap_Release(buffer);
    }
  }
  if (state->fade == 0) {
    VramCache_ReleaseVram(state->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    y = (state->scrollY + 8) & OBJ_Y_MASK;
    if (row == 0) {
      x = state->fade * 6 + state->scrollX - 12;
      x &= OBJ_X_MASK;
      flip = 0;
    } else {
      x = 236 - state->fade * 6 - state->scrollX;
      x &= OBJ_X_MASK;
      flip = OBJ_HFLIP;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | OBJ_X(x) | flip | OBJ_SIZE_16X32;
      *cursor++ = (tile + RACE_MAP_TILE_STAGE) |
                  (OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1));
      Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | OBJ_X(x) | flip | OBJ_SIZE_16X8;
      *cursor++ = (tile + RACE_MAP_TILE_STAGE_FOOT) |
                  (OBJ_PALETTE(RACE_MAP_PALETTE) | OBJ_PRIORITY(1));
      Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
      y += RACE_MAP_STAGE_SPACING;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = Actor_Lookup(state->leaderId);
  if (actor != NULL) {
    MARKER(+state->fade * 6 + state->scrollX - 19,
           -state->fade * 6 - state->scrollX + 232, 0, RACE_MAP_TILE_1P);
  }
  actor = Actor_Lookup(state->rivalId);
  if (actor == NULL || actor->x.fixed == 0)
    return;
  {
    // This side's offset adds scrollX where the first marker's subtracts it.
    // Nothing writes scrollX, and in a render of the race (the stage room by
    // entrance 31 with FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT set) both columns stand
    // at the screen's edges with "1P" and "2P" at their feet: it is 0, so the
    // two agree.
    MARKER(+state->fade * 6 + state->scrollX + 189,
           -state->fade * 6 + state->scrollX + 23, 1, RACE_MAP_TILE_2P);
  }
}

/// Shows the race map for both sides.
void TrialRoad_ShowRaceMap(s32 leaderId, s32 rivalId) {
  struct TrialRoadLinkState *state = HeapSlot_AllocEwram(
      HEAP_SLOT_TRIAL_ROAD_LINK, sizeof(struct TrialRoadLinkState));
  void *buffer = Heap_AllocEwram(RACE_MAP_PICTURE_BYTES);

  state->leaderId = leaderId;
  state->rivalId = rivalId;
  if (leaderId == gMainRoutineState.leaderActorId) {
    state->originX = toFIXED(160);
    state->originZ = toFIXED(-1632);
  } else {
    state->originX = toFIXED(112);
    state->originZ = toFIXED(-1632);
  }
  Actor_GetById(leaderId);
  Actor_GetById(rivalId);
  state->fade = 0;
  state->holdFade = 0;
  Decompress_TaggedLz(gTrialRoad_RaceMapPicture, buffer);
  state->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(state->vramEntry, RACE_MAP_PICTURE_BYTES, buffer);
  Task_Add(TrialRoad_DrawRaceMap, TASK_PASS_FRAME - 10);
  Heap_Release(buffer);
}

/// Starts Moapa's party on its recording, mirrored when the leader went right.
void TrialRoad_StartRival(s32 mirrored) {
  struct TrialRoadLinkState *trialRoadLinkState = Ram_HeapSlots->trialRoadLink;
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    state->mirrored = mirrored;
    state->id = trialRoadLinkState->rivalId;
    state->still = 0;
  }
  Task_Add(TrialRoad_MoveRival, TASK_PASS_FRAME + 5);
}

/// Plays Moapa's recording from the given resource.
void TrialRoad_PlayRivalRecording(s32 resource) {
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;

  state->resource = resource;
  ((struct TrialRoadLinkState *)Ram_HeapSlots->trialRoadLink)->resource = 0;
  state->point = 0;
  state->mode = RIVAL_REPLAY;
  TrialRoad_MoveRival();
}

/// Whether Moapa's party is waiting.
s32 TrialRoad_RivalIsWaiting(void) {
  s32 mode = ((struct TrialRoadReplay *)gSceneState)->mode;

  return IsZero(mode ^ RIVAL_WAITING);
}

/// Unused: does nothing.
UNUSED void TrialRoad_DoNothing(void) {}

s32 gTrialRoad_SettleScript[] = {
    SC_CALL((s32)TrialRoad_SettleOnGround),
    SC_END,
};

s32 gTrialRoad_PillarTopScript[] = {
    SC_CALL((s32)TrialRoad_FollowPillar),
    SC_END,
};

s8 gTrialRoad_PoundPartners[4] ALIGNED(4) = {ACTOR_INNER_WEST_STAKE,
                                             ACTOR_WEST_STAKE, ACTOR_EAST_STAKE,
                                             ACTOR_INNER_EAST_STAKE};

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// Unused: an empty script nothing starts.
u32 gTrialRoad_EmptyScript[] = {
    SC_END,
};

u32 gTrialRoad_GaspHopScript[] = {
    SC_ADD(SW_FACING, FACING_EAST_SOUTHEAST),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

void (*gTrialRoad_StageSetUps[])(void) = {
    TrialRoad_SetUpPillarStage,
    TrialRoad_SetUpTorchStage,
    TrialRoad_SetUpWeedsPillarStage,
    TrialRoad_SetUpPoundStage,
    TrialRoad_SetUpCrumblingStage,
    TrialRoad_SetUpLogStage,
    TrialRoad_SetUpFountainStage,
    TrialRoad_SetUpSandStage,
};

/// The recordings Moapa's party runs by, as resource ids: one for each stage
/// pattern, in gTrialRoad_StageSetUps' order, then his run at the blue chests
/// when he reached the stage's switch first and when Felix did.
u32 gTrialRoad_RivalRecordings[] = {
    RESOURCE_TRIAL_ROAD_PILLAR_RECORDING,
    RESOURCE_TRIAL_ROAD_TORCH_RECORDING,
    RESOURCE_TRIAL_ROAD_WEEDS_RECORDING,
    RESOURCE_TRIAL_ROAD_POUND_RECORDING,
    RESOURCE_TRIAL_ROAD_CRUMBLING_RECORDING,
    RESOURCE_TRIAL_ROAD_LOG_RECORDING,
    RESOURCE_TRIAL_ROAD_FOUNTAIN_RECORDING,
    RESOURCE_TRIAL_ROAD_SAND_RECORDING,
    RESOURCE_TRIAL_ROAD_CHESTS_MOAPA_FIRST_RECORDING,
    RESOURCE_TRIAL_ROAD_CHESTS_FELIX_FIRST_RECORDING,
};

/// Each stage's place in the map, in cells.
s16 gTrialRoad_StageMapCells[][2] = {
    {1, 48}, {15, 48}, {29, 48}, {1, 70},
    {15, 70}, {29, 70}, {1, 92}, {15, 92},
};

/// The stage pattern each of the ten treasures belongs to: as a stage is laid,
/// the treasures of the others are put aside at (0, 0).
s8 gTrialRoad_RulesTreasureStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

/// The same table as gTrialRoad_RulesTreasureStages, under another name.
s8 gTrialRoad_TreasureStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

/// The stages each path's order must hold at least one of: the pillar, weeds
/// pillar, Pound and sand stages. TrialRoad_DealStageOrder reads only these
/// first four.
s8 gTrialRoad_KeyStages[] = {
    0, 2, 3, 7, 1, 4, 5, 6,
};

u32 gTrialRoad_RightKnightEntersScript[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_SET(SW_X, toFIXED(312)),
    SC_SET(SW_Z, toFIXED(680)),
    SC_ANIMATION(5),
    SC_MOVE(312, 0, 632),
    SC_SYNC,
    SC_MOVE(344, 0, 616),
    SC_SYNC,
    SC_MOVE(408, 0, 616),
    SC_SYNC,
    SC_MOVE(424, 0, 600),
    SC_SYNC,
    SC_MOVE(424, 0, 456),
    SC_SYNC,
    SC_END,
};

u32 gTrialRoad_LeftKnightEntersScript[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_SET(SW_X, toFIXED(232)),
    SC_SET(SW_Z, toFIXED(680)),
    SC_ANIMATION(5),
    SC_MOVE(232, 0, 632),
    SC_SYNC,
    SC_MOVE(200, 0, 616),
    SC_SYNC,
    SC_MOVE(136, 0, 616),
    SC_SYNC,
    SC_MOVE(120, 0, 600),
    SC_SYNC,
    SC_MOVE(120, 0, 456),
    SC_SYNC,
    SC_END,
};

u32 gTrialRoad_LeftKnightToChestScript[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_SET(SW_X, toFIXED(120)),
    SC_SET(SW_Z, toFIXED(296)),
    SC_ANIMATION(5),
    SC_MOVE(120, 0, 216),
    SC_SYNC,
    SC_MOVE(136, 0, 200),
    SC_SYNC,
    SC_MOVE(216, 0, 200),
    SC_SYNC,
    SC_MOVE(232, 0, 184),
    SC_SYNC,
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

u32 gTrialRoad_LeftKnightLeavesScript[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_ANIMATION(5),
    SC_MOVE(216, 0, 168),
    SC_SYNC,
    SC_MOVE(232, 0, 152),
    SC_SYNC,
    SC_MOVE(232, 0, 136),
    SC_SYNC,
    SC_SET(SW_X, 0),
    SC_SET(SW_Z, 0),
    SC_END,
};

u32 gTrialRoad_RightKnightToChestsScript[] = {
    SC_SPEED(1),
    SC_ACCELERATION(0.5),
    SC_SET(SW_X, toFIXED(344)),
    SC_SET(SW_Z, toFIXED(184)),
    SC_ANIMATION(2),
    SC_MOVE(312, 0, 168),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

u32 gTrialRoad_RightKnightLeavesScript[] = {
    SC_SPEED(1.5),
    SC_ACCELERATION(1),
    SC_ANIMATION(5),
    SC_MOVE(376, 0, 200),
    SC_SYNC,
    SC_MOVE(472, 0, 200),
    SC_SYNC,
    SC_MOVE(488, 0, 184),
    SC_SYNC,
    SC_MOVE(488, 0, 136),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};
// clang-format on
#undef MARKER

#undef DEFINE_TRIAL_ROAD_SET_POOL_TILES
#undef SET_LAYER
#undef MARKER

// clang-format off
#define DUST_SCRIPT_SECTION LATE_DATA
#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"
// clang-format on

void TrialRoad_FightGasp(void);
void TrialRoad_OpenChests(void);
void TrialRoad_JumpForward(void);
void TrialRoad_BlowAwayPoundStageWeeds(void);
void TrialRoad_StandOnBlock(void);
void TrialRoad_ShutSummitDoorway(s32 sound);
void TrialRoad_StepOffSwitch(void);

/// Unused: the digits 0 to 9 and A to F, four to a word.
// clang-format off
LAST_DATA u32 gTrialRoad_HexDigits[] = {
    0x33323130, 0x37363534, 0x42413938, 0x46454443, 0,
};

LAST_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 344, 0, 264, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LAST_DATA u32 gSceneExits[] = {
    SCENE_EXITS(TrialRoadStages),
    SCENE_EXIT(CELL_LEFT_PATH_STAIRS_DOWN, TrialRoadStages, TRIAL_ROAD_STAGE_TOP_LEFT),
    SCENE_EXIT(CELL_RIGHT_PATH_STAIRS_DOWN, TrialRoadStages, TRIAL_ROAD_STAGE_TOP_RIGHT),
    SCENE_EXIT(CELL_LEFT_PATH_DOOR, TrialRoadStages, TRIAL_ROAD_STAGE_FOOT_LEFT),
    SCENE_EXIT(CELL_RIGHT_PATH_DOOR, TrialRoadStages, TRIAL_ROAD_STAGE_FOOT_RIGHT),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_A, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_A),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_B, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_B),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_WEST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_WEST),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_EAST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_EAST),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_C, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_C),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_D, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_E),
    SCENE_EXITS(TrialRoadCaveRock),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_EAST_PASSAGE_FROM_WEST_CAVE),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_WEST_CAVE_FROM_EAST_PASSAGE),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_CAVE_ROCK),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP, TrialRoadGasp, TRIAL_ROAD_GASP_FROM_TRIAL_ROAD_CAVE_ROCK),
    SCENE_EXITS(TrialRoadGasp),
    SCENE_EXIT(TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_GASP),
    SCENE_EXITS(TrialRoadSummit),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_EAST),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_SUMMIT),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_HOUSES_EAST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_HOUSES_EAST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_B),
    SCENE_EXITS_END,
};

LAST_DATA struct ActorPlacement gTrialRoad_Placements[] = {
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_PillarStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 248, 0, 400, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_TorchStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_WeedsPillarStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 232, 0, 432, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 296, 0, 432, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_PoundStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 264, 0, 464, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_CrumblingStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_LogStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE_TRUNK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE_TRUNK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_LOGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 456, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_FountainStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 472, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 440, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_SandStagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 408, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 312, 0, 336, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_CaveRockPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_GaspPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GASP), MOVEMENT_TYPE_STAND, 344, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LAST_DATA struct ActorPlacement gTrialRoad_SummitPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 328, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Each stage pattern's actor placements.
LAST_DATA s32 gTrialRoad_StagesPlacements[] = {
    (s32)gTrialRoad_PillarStagePlacements,
    (s32)gTrialRoad_TorchStagePlacements,
    (s32)gTrialRoad_WeedsPillarStagePlacements,
    (s32)gTrialRoad_PoundStagePlacements,
    (s32)gTrialRoad_CrumblingStagePlacements,
    (s32)gTrialRoad_LogStagePlacements,
    (s32)gTrialRoad_FountainStagePlacements,
    (s32)gTrialRoad_SandStagePlacements,
};

LAST_DATA struct Trigger gTrialRoad_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_PillarStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_PILLAR_STAGE_GAP, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), EVENT(TrialRoad_JumpAcrossGap)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_PILLAR_STAGE_LEDGE, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), EVENT(TrialRoad_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY), EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_D, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_E, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY), EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_D, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_E, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_A, FLAG_PILLAR_STAGE_WEEDS_BLOWN_AWAY, EVENT(TrialRoad_BlowAwayPillarStageWeeds)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_TorchStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_ICE_PILLAR_SIDE, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_ICE_PILLAR_SIDE, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderTorchStagePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderTorchStagePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderTorchStagePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WEST_TORCH, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderTorchStagePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_TORCH, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderTorchStagePillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_TORCH, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_TORCH, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_C, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_C), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_D, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_D), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_E, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_E), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PILLAR_F, IF_FLAG_SET(FLAG_PUDDLE_MELTED_BASE + ACTOR_PILLAR_F), EVENT(TrialRoad_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_WeedsPillarStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_WEEDS_STAGE_LEDGE, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_A, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A), EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B), EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_C, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_D, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR_E, CONDITION_ALWAYS, EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A), EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B), EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_D, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_E, CONDITION_ALWAYS, EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_A, FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_A, EVENT(TrialRoad_BlowAwayWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_B, FLAG_WEEDS_BLOWN_AWAY_BASE + ACTOR_WEEDS_B, EVENT(TrialRoad_BlowAwayWeeds)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_PoundStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING, CELL_WEST_STAKE, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, CELL_INNER_WEST_STAKE, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, CELL_INNER_EAST_STAKE, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, CELL_EAST_STAKE, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {TRIGGER_STEP, CELL_WEST_STAKE, FLAG_INNER_WEST_STAKE_RAISED, EVENT(TrialRoad_StepOntoWestStake)},
    {TRIGGER_STEP, CELL_INNER_WEST_STAKE, IF_FLAG_SET(FLAG_INNER_WEST_STAKE_RAISED), EVENT(TrialRoad_StepOntoInnerWestStake)},
    {TRIGGER_STEP, CELL_INNER_EAST_STAKE, FLAG_EAST_STAKE_RAISED, EVENT(TrialRoad_StepOntoInnerEastStake)},
    {TRIGGER_STEP, CELL_EAST_STAKE, IF_FLAG_SET(FLAG_EAST_STAKE_RAISED), EVENT(TrialRoad_StepOntoEastStake)},
    {STEP_TRIGGER_WALKING, CELL_STAKE_GROUND, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS, EVENT(TrialRoad_CheckStakesCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_B, IF_FLAG_SET(FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY), EVENT(TrialRoad_CheckStakesCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_C, CONDITION_ALWAYS, EVENT(TrialRoad_CheckStakesCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_POUND_STAGE_WEEDS, FLAG_POUND_STAGE_WEEDS_BLOWN_AWAY, EVENT(TrialRoad_BlowAwayPoundStageWeeds)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_WEST_STAKE, FLAG_INNER_WEST_STAKE_RAISED, EVENT(TrialRoad_PoundWestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_INNER_WEST_STAKE, IF_FLAG_SET(FLAG_INNER_WEST_STAKE_RAISED), EVENT(TrialRoad_PoundInnerWestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_INNER_EAST_STAKE, FLAG_EAST_STAKE_RAISED, EVENT(TrialRoad_PoundInnerEastStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_EAST_STAKE, IF_FLAG_SET(FLAG_EAST_STAKE_RAISED), EVENT(TrialRoad_PoundEastStake)},
    {TRIGGER_TABLE_END},
};

/// The crumbling stage's two cracked pillars, west and east, each with the flag
/// that remembers its first step (Leader_CheckCrumblingStep), ended by -1.
LAST_DATA s16 gTrialRoad_CrumblingPillars[] = {
    ACTOR_WEST_CRUMBLING_PILLAR, FLAG_WEST_CRUMBLING_PILLAR_STEPPED_ON,
    ACTOR_EAST_CRUMBLING_PILLAR, FLAG_EAST_CRUMBLING_PILLAR_STEPPED_ON,
    -1, 0,
};
// clang-format on

LAST_DATA struct Trigger gTrialRoad_CrumblingStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_CRUMBLING_STAGE_LEDGE,
     CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_CRUMBLING_STAGE_LEDGE,
     CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_WEST_CRUMBLING_PILLAR,
     CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_EAST_CRUMBLING_PILLAR,
     CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BESIDE_CRUMBLING_PILLAR,
     CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BESIDE_CRUMBLING_PILLAR,
     CONDITION_ALWAYS, EVENT(TrialRoad_CheckCrumblingPillars)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_WEST_CRUMBLING_PILLAR, FLAG_WEST_CRUMBLING_PILLAR_CRUMBLED,
     EVENT(TrialRoad_CrumbleWestPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_EAST_CRUMBLING_PILLAR, FLAG_EAST_CRUMBLING_PILLAR_CRUMBLED,
     EVENT(TrialRoad_CrumbleEastPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_LogStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_LOG_STAGE_LEDGE,
     CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_UPPER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushUpperLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_UPPER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushUpperLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_LOWER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushLowerLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_LOWER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushLowerLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_UPPER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_UPPER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_LOWER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_LOWER_LOG_ROWS,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_EAST_LOG_NORTH_END,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_EAST_LOG_NORTH_END,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogSouthOrClimbDown)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_EAST_LOG_SOUTH_END,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_EAST_LOG_SOUTH_END,
     CONDITION_ALWAYS, EVENT(TrialRoad_PushEastLogSouth)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_A, CONDITION_ALWAYS,
     EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS,
     EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_FountainStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, CELL_BLOCK_TRACK, CONDITION_ALWAYS,
     EVENT(TrialRoad_StandOnBlock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR_A, CONDITION_ALWAYS,
     EVENT(TrialRoad_ClearJumpUnderPushedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR_A, CONDITION_ALWAYS,
     EVENT(TrialRoad_SetJumpUnderPushedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_WEST_VENT,
     FLAG_VENT_UNCOVERED_BASE + ACTOR_WEST_VENT,
     EVENT(TrialRoad_UncoverWestVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_MIDDLE_VENT, FLAG_VENT_UNCOVERED_BASE + ACTOR_MIDDLE_VENT,
     EVENT(TrialRoad_UncoverMiddleVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_EAST_VENT,
     FLAG_VENT_UNCOVERED_BASE + ACTOR_EAST_VENT,
     EVENT(TrialRoad_UncoverEastVent)},
    {TRIGGER_TABLE_END},
};

LAST_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LAST_DATA struct Trigger gTrialRoad_SandStageTriggers[] = {
    {TRIGGER_STEP, CELL_LEFT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_STAIRS_DOWN, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_LEFT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_DOOR, CONDITION_ALWAYS,
     EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, CELL_AROUND_WEST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_WEST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_EAST_GIVE_UP_BUTTON,
     IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, CELL_EAST_GIVE_UP_BUTTON, FLAG_GIVE_UP_ASKED,
     EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, CELL_AROUND_WEST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_WEST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, CELL_AROUND_EAST_SWITCH,
     IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepOffSwitch)},
    {TRIGGER_STEP, CELL_EAST_SWITCH, FLAG_STAGE_CHESTS_OPENED,
     EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_LEFT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_INNER_CHEST,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_HEAVY),
     TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_RIGHT_OUTER_CHEST,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, ACTOR_LEFT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_LEFT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_INNER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, ACTOR_RIGHT_OUTER_CHEST, CONDITION_ALWAYS,
     EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, CELL_STREAM, CONDITION_ALWAYS,
     EVENT(TrialRoad_DriftWithStream)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_StartSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_EndSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_SAND_STAGE_WEEDS,
     FLAG_SAND_STAGE_WEEDS_BLOWN_AWAY, EVENT(TrialRoad_BlowAwaySandStageWeeds)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_CaveRockTriggers[] = {
    {TRIGGER_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE, CONDITION_ALWAYS,
     TRIAL_ROAD_CAVE_ROCK_TO_EAST_PASSAGE},
    {TRIGGER_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE, CONDITION_ALWAYS,
     TRIAL_ROAD_CAVE_ROCK_TO_WEST_CAVE},
    {TRIGGER_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT, CONDITION_ALWAYS,
     TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT},
    {TRIGGER_EXIT, TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP, CONDITION_ALWAYS,
     TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_CAVE_ROCK, FLAG_CAVE_ROCK_LIFTED,
     EVENT(TrialRoad_LiftCaveRock)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_GaspTriggers[] = {
    {TRIGGER_EXIT, TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK, CONDITION_ALWAYS,
     TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS,
     CONDITION_ALWAYS, EVENT(TrialRoad_ScatterWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_WEEDS_HIDING_MAD_PLANT, CONDITION_ALWAYS,
     EVENT(TrialRoad_ScatterWeedsHidingMadPlant)},
    {TRIGGER_TALK, ACTOR_GASP, CONDITION_ALWAYS, EVENT(TrialRoad_FightGasp)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_RevealPoolTiles)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_HidePoolTiles)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_SummitTriggers[] = {
    {TRIGGER_STEP, CELL_SUMMIT_DOORWAY, FLAG_SUMMIT_DOORWAY_SHUT,
     EVENT(TrialRoad_ShutSummitDoorway)},
    {TRIGGER_STEP, CELL_LEFT_PATH_SUMMIT,
     IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
     EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_STEP, CELL_RIGHT_PATH_SUMMIT,
     IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
     EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST, CONDITION_ALWAYS,
     TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST, CONDITION_ALWAYS,
     TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST},
    {TRIGGER_EXIT, TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK, CONDITION_ALWAYS,
     TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PEAK_PILLAR,
     FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED, EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_PUSHED, 0, FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
     EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA s32 gTrialRoad_StagesTriggers[] = {
    (s32)gTrialRoad_PillarStageTriggers,
    (s32)gTrialRoad_TorchStageTriggers,
    (s32)gTrialRoad_WeedsPillarStageTriggers,
    (s32)gTrialRoad_PoundStageTriggers,
    (s32)gTrialRoad_CrumblingStageTriggers,
    (s32)gTrialRoad_LogStageTriggers,
    (s32)gTrialRoad_FountainStageTriggers,
    (s32)gTrialRoad_SandStageTriggers,
};

/// The summit's colour cycle, in frames; -1 before it starts.
LAST_DATA s32 gTrialRoad_PaletteCycleFrame = -1;

LAST_DATA s32 gTrialRoad_PaletteCycleRunning = 1;
