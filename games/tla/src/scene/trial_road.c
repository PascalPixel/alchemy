/// @file
/// Trial Road, the mountain path above the Shaman Village where the party races
/// Moapa to the summit: four stages dealt from eight puzzle rooms, a left and a
/// right path through each, the blue chests that open each stage's door for
/// the weight of the party's equipment, and the battle at the peak; after the
/// race, the summit's cave and pools and Gasp the Djinni.
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
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/output.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/serial.h"
#include "system/system.h"
#include "system/task.h"
#include "system/vector3.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "text/msg_ids.h"

/// Set once the Mad Plant hiding in the bushes Cyclone cuts has been beaten.
#define FLAG_TRIAL_ROAD_MAD_PLANT_BEATEN 0x9b9
/// Set once Move has pushed the peak's pillar into column 8, where it stays.
#define FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED 0x9fc

/// Set while the leader stands on the pillars' tops
/// (TrialRoad_TrackLeaderOnPillars).
#define FLAG_LEADER_ON_PILLARS FLAG_SCENE_TEMP_08
/// Set once a step at a stage's foot has offered to give up; stepping away
/// clears it.
#define FLAG_GIVE_UP_ASKED FLAG_SCENE_TEMP_81
/// Set once both blue chests hold their items and the leader's door opens.
#define FLAG_STAGE_DOOR_OPEN FLAG_SCENE_TEMP_82
/// Set once the leader reaches the door at a stage's end and the blue chests
/// open (TrialRoad_OpenChests).
#define FLAG_STAGE_CHESTS_OPENED FLAG_SCENE_TEMP_84
/// Set while the inner blue chest stands open for an item.
#define FLAG_INNER_CHEST_OPEN FLAG_SCENE_TEMP_85
/// Set while the outer blue chest stands open for an item: it opens only when
/// Moapa reached the door first.
#define FLAG_OUTER_CHEST_OPEN FLAG_SCENE_TEMP_86
/// Set once Lift has raised the rock at the summit cave's mouth.
#define FLAG_CAVE_ROCK_LIFTED FLAG_SCENE_TEMP_89

#include "shared/trial_road.h"

/// The actors every stage room places first, numbering the rows of each
/// stage's table: Moapa, the four blue chests (two on the left path, two on
/// the right, inner and outer) and Moapa's two guards. A stage's own puzzle
/// pieces follow from ACTOR_FIRST_STAGE_PIECE.
enum TrialRoadStageActorId {
  ACTOR_MOAPA = ACTOR_FIRST_PLACED,
  ACTOR_LEFT_INNER_CHEST,
  ACTOR_LEFT_OUTER_CHEST,
  ACTOR_RIGHT_INNER_CHEST,
  ACTOR_RIGHT_OUTER_CHEST,
  ACTOR_WEST_GUARD,
  ACTOR_EAST_GUARD,
  ACTOR_FIRST_STAGE_PIECE
};

/// The log stage's three logs (gTrialRoad_LogStagePlacements): two rolling
/// east and west, upper and lower, and one rolling north and south east of
/// them. Kraden, at the summit (gTrialRoad_SummitPlacements), after Moapa.
#define ACTOR_KRADEN (ACTOR_MOAPA + 1)

enum TrialRoadLogStageActorId {
  ACTOR_UPPER_LOG = ACTOR_FIRST_STAGE_PIECE + 1,
  ACTOR_LOWER_LOG,
  ACTOR_EAST_LOG
};

/// The Pound stage's four pillars (gTrialRoad_PoundStagePlacements), west to
/// east, in two seesaw pairs: pounding one raises its partner.
enum TrialRoadPoundStageActorId {
  ACTOR_WEST_PILLAR = ACTOR_FIRST_STAGE_PIECE + 3,
  ACTOR_INNER_WEST_PILLAR,
  ACTOR_INNER_EAST_PILLAR,
  ACTOR_EAST_PILLAR
};

/// The crumbling stage's two pillars (gTrialRoad_CrumblingStagePlacements).
enum TrialRoadCrumblingStageActorId {
  ACTOR_WEST_CRUMBLING_PILLAR = ACTOR_FIRST_STAGE_PIECE,
  ACTOR_EAST_CRUMBLING_PILLAR
};

/// The pillars most stages stand from ACTOR_FIRST_STAGE_PIECE: five, or six
/// where the torch stage's carry torches; the log stage has one, the block.
enum TrialRoadPillarActorId {
  ACTOR_PILLAR_A = ACTOR_FIRST_STAGE_PIECE,
  ACTOR_PILLAR_B,
  ACTOR_PILLAR_C,
  ACTOR_PILLAR_D,
  ACTOR_PILLAR_E,
  ACTOR_PILLAR_F
};

/// The bushes after the pillars: on pillar C in the pillar stage, on pillars A
/// and B in the bush stage.
enum { ACTOR_BUSH_A = ACTOR_FIRST_STAGE_PIECE + 5, ACTOR_BUSH_B };

/// The bush on pillar B in the Pound stage, after its four pillars.
enum { ACTOR_POUND_STAGE_BUSH = ACTOR_EAST_PILLAR + 1 };

/// The bush in the sand stage, after its two sand pieces.
enum { ACTOR_SAND_STAGE_BUSH = ACTOR_FIRST_STAGE_PIECE + 2 };

/// The pillar at the summit (gTrialRoad_SummitPlacements), after Kraden.
#define ACTOR_PEAK_PILLAR (ACTOR_KRADEN + 1)

/// Gasp, alone in his room (gTrialRoad_GaspPlacements).
enum { ACTOR_GASP = ACTOR_FIRST_PLACED };

/// The fountain stage's five spouts (gTrialRoad_FountainStagePlacements): three
/// in a row west to east, and two south of them.
enum TrialRoadFountainStageActorId {
  ACTOR_WEST_SPOUT = ACTOR_FIRST_STAGE_PIECE + 1,
  ACTOR_MIDDLE_SPOUT,
  ACTOR_EAST_SPOUT,
  ACTOR_SOUTHWEST_SPOUT,
  ACTOR_SOUTHEAST_SPOUT
};

/// The entrances the code tests. The Shaman Village enters the first stage by
/// its left or right path; the summit returns to the last stage the same way;
/// while the elder explains the trial's rules
/// (ShamanVillage_ExplainTrialRules), TRIAL_ROAD_RULES deals a course and
/// TRIAL_ROAD_RULES_COURSE shows its traps. The summit is entered again after
/// the battle with Moapa, won or lost.
enum TrialRoadEntrance {
  TRIAL_ROAD_SUMMIT_AFTER_WIN = 4,
  TRIAL_ROAD_SUMMIT_AFTER_DEFEAT = 5,
  TRIAL_ROAD_FROM_VILLAGE_LEFT = 31,
  TRIAL_ROAD_FROM_VILLAGE_RIGHT = 32,
  TRIAL_ROAD_FROM_SUMMIT_LEFT = 33,
  TRIAL_ROAD_FROM_SUMMIT_RIGHT = 34,
  TRIAL_ROAD_RULES = 50,
  TRIAL_ROAD_RULES_COURSE = 55
};

/// The sand stage's two tightrope posts (gTrialRoad_SandStagePlacements).
enum { ACTOR_WEST_POST = ACTOR_FIRST_STAGE_PIECE, ACTOR_EAST_POST };

/// The torch stage's pillars C to F are frozen puddles; each one's flag,
/// FLAG_SCENE_TEMP_14 to FLAG_SCENE_TEMP_17, is set once it has melted.
#define FLAG_PUDDLE_MELTED_BASE (FLAG_SCENE_TEMP_14 - ACTOR_PILLAR_C)
/// The bush stage's bushes; each one's flag, FLAG_AREA_TEMP_27 and
/// FLAG_AREA_TEMP_28, is set once Whirlwind has blown it away.
#define FLAG_BUSH_BLOWN_BASE (FLAG_AREA_TEMP_27 - ACTOR_BUSH_A)

/// The log stage's logs' flags, FLAG_SCENE_TEMP_53 to FLAG_SCENE_TEMP_55, set
/// once each has been pushed.
#define FLAG_LOG_PUSHED_BASE (FLAG_SCENE_TEMP_53 - ACTOR_UPPER_LOG)
/// The fountain stage's spouts' flags, FLAG_AREA_TEMP_63 to FLAG_AREA_TEMP_65,
/// set once each has been uncovered.
#define FLAG_SPOUT_UNCOVERED_BASE (FLAG_AREA_TEMP_63 - ACTOR_WEST_SPOUT)

/// A piece the party cannot push: a pillar a bush holds down, or a log.
/// Leader_CanPushObjectAhead refuses a target with this byte set.
#define PUSH_BLOCKED(object) *(u8 *)&object->riseCounter
/// The height in pixels of a pillar's top (TrialRoad_AddPillarTop) above the
/// pillar: 16 standing, -16 lowered.
#define PILLAR_TOP_HEIGHT(top) *(u8 *)&top->riseEnabled

/// A map cell's collision code for a wall: nothing steps onto it.
#define CELL_WALL 0xff

/// A fountain rises and falls once each FOUNTAIN_PERIOD frames, rising at
/// FOUNTAIN_RISE_AT and falling at FOUNTAIN_FALL_AT.
#define FOUNTAIN_PERIOD 240
#define FOUNTAIN_RISE_AT 120
#define FOUNTAIN_FALL_AT 150

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

/// Sets an actor on the ground two pixels up, still, and half again as tall.
s32 TrialRoad_SettleOnGround(struct Actor *object) {
  s32 height =
      Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed) + toFIXED(2);

  object->groundHeight = height;
  object->y.fixed = height;
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->scaleY = FIXED_ONE * 3 / 2;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_STAGES)
    return gTrialRoad_StagesPlacements[pattern & 7];
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
    return gTrialRoad_CaveRockPlacements;
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_GASP)
    return gTrialRoad_GaspPlacements;
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_SUMMIT)
    return gTrialRoad_SummitPlacements;
  return gTrialRoad_Placements;
}

/// Whether Moapa is ahead in the race: by the stages each has cleared, then by
/// who reached the last door first; on a tie, fallback.
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
void Actor_SetSpriteColorMode(struct Actor *object, u32 mode);

/// Taking the stairs at a stage's foot or top, with the race's rules: while the
/// party's equipment is held, the first stairs offer to give up.
void TrialRoad_TakeStairs(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;

  if ((u32)(direction - 1) <= 1) {
    if (state->stage[0] <= 0) {
      if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
#ifdef UBFIX
        if (TrialRoad_OfferToGiveUp() == 0)
#else
        // UB: TrialRoad_OfferToGiveUp takes no argument, but this call passes
        // 0.
        if (((s32 (*)(s32))TrialRoad_OfferToGiveUp)(0) == 0)
#endif
          Actor_WalkByAndWait(gMainRoutineWork.leaderActorId, 0, -16);
        return;
      }
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->stage[0]--;
      Import_EventSetExitRequest(direction);
    }
    Event_PlaySound(SE_MAP_EXIT);
  }
  if ((u32)(direction - 3) <= 1) {
    state->stage[0]++;
    if (state->stage[0] > 3) {
      if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT) &&
          TrialRoad_RivalIsAhead(0) == 0)
        Event_SetFlag(FLAG_AREA_TEMP_85);
      Event_PlaySound(SE_STAIRS_UP);
      Exit_StartStairsUp();
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

/// Trial Road's stage puzzles: the stairs, chests and doors, and the pillars,
/// torches, logs, spouts and sand the eight rooms are built from.

struct Actor;
struct FieldSprite;
union FieldObject;

/// The map work's ground effects byte (MapWork.groundEffects), read as its
/// bits.
struct MapGroundEffectsView {
  u8 padding[0x17];
  u8 lowBits : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 middleBits : 2;
  u8 highBits : 4;
};

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
  RIVAL_REPLAY = 1,  ///< walks the recorded path
  RIVAL_RECORD = 2,  ///< records the actor's path
  RIVAL_LINKED = 3,  ///< stands where the linked player's record puts it
  RIVAL_MIRROR = 4,  ///< follows the leader mirrored across the course
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

/// The heap slot TrialRoad_ShowProgress takes for the two parties' progress
/// display and the rival's recorded path.
struct TrialRoadLinkWork {
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
  s16 path[0x1c20]; ///< x, z pairs in pixels, 0, 0 ending them
};

s32 TrialRoad_IsMirrored(void);

void TrialRoad_BlockCellUnder(s32 id);
void TrialRoad_UnblockCellUnder(s32 id);
void TrialRoad_AddPillarTop(s32 id);
void TrialRoad_SetPillarTopLift(s32 id, s32 value);
void TrialRoad_OpenSpoutCell(s32 id);
void TrialRoad_TrackLeaderOnPillars(void);
void TrialRoad_CheckPoundedPillarsCovered(s32 a, s32 b);
void TrialRoad_PoundPillar(s32 id, s32 phase, s32 frame);
void TrialRoad_StepOnSeesawPillar(s32 id, s32 other, s32 flag, s32 side);
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis);
void TrialRoad_UncoverSpout(s32 id, s32 phase, s32 frame);
void MagmaRockInside_CheckCrumblingStep(const u16 *table);
void TrialRoad_CrumblePillar(const void *data, s32 a);

void TrialRoad_ClearGiveUpAsked(void);

void TrialRoad_BlowBushOffLonePillar(void);

void TrialRoad_JumpAcrossGap(void);

void TrialRoad_JumpMeltedPuddle(void);

void TrialRoad_StepOnWestPillar(void);

void TrialRoad_StepOnInnerWestPillar(void);

void TrialRoad_StepOnInnerEastPillar(void);

void TrialRoad_StepOnEastPillar(void);

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

void TrialRoad_BlowBushOffVine(void);

void TrialRoad_DriftWithStream(void);

void TrialRoad_LiftCaveRock(void);

void TrialRoad_CutBushes(void);

void TrialRoad_CutMadPlantBushes(void);

/// Shows the pools' hidden tiles.
void TrialRoad_RevealPoolTiles(void);

/// Hides the pools' tiles again.
void TrialRoad_HidePoolTiles(void);

void TrialRoad_ChallengeMoapa(void);

extern struct ActorPlacement gTrialRoad_PillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_TorchStagePlacements[];
extern struct ActorPlacement gTrialRoad_BushPillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_PoundStagePlacements[];
extern struct ActorPlacement gTrialRoad_CrumblingStagePlacements[];
extern struct ActorPlacement gTrialRoad_LogStagePlacements[];
extern struct ActorPlacement gTrialRoad_FountainStagePlacements[];
extern struct ActorPlacement gTrialRoad_SandStagePlacements[];
extern struct Trigger gTrialRoad_PillarStageTriggers[];
extern struct Trigger gTrialRoad_TorchStageTriggers[];
extern struct Trigger gTrialRoad_BushPillarStageTriggers[];
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

void TrialRoad_LowerPillar(s32 id);

void TrialRoad_RaisePillar(s32 id);

void TrialRoad_BlockLogs(void);

void TrialRoad_StartFountains(s32 a, s32 b);

void Tightrope_Initialize(void);
void Tightrope_Place(s32 a, s32 b, s32 c);

extern u32 gTrialRoad_GaspHopScript[];

u16 gTrialRoad_PaletteCycleColors[16] BSS_DATA;

void PaletteCycle_StepAt(void);
void TrialRoad_OpenSummitGate();
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

void Map_CopyRawCellsRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void TrialRoad_ShowProgress(s32 leaderId, s32 rivalId);

/// Opens the door at the end of the leader's stage, on his side of the road.
void TrialRoad_OpenLeaderDoor(s32 side, s32 phase);

extern u32 gTrialRoad_LeftKnightEntersScript[];

extern u32 gTrialRoad_RightKnightEntersScript[];

extern u32 gTrialRoad_LeftKnightToChestScript[];
extern u32 gTrialRoad_RightKnightToChestsScript[];

extern u32 gTrialRoad_RightKnightLeavesScript[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Actor_DisableById(s32 id);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void MagmaRockInside_SpawnRubble(s32 value);
void MagmaRockInside_SetLeaderCellShape(s32 offset, s32 layer);
void MagmaRockInside_SpawnDustRing(void);
void TrialRoad_SetActorCellShape(s32 a, s32 b, s32 c);

void TrialRoad_DriftRubble(union FieldObject *object);

void Actor_StartIdle(struct Actor *object);

extern const u16 gTrialRoad_ProgressPalette[];

void TrialRoad_MoveRival(void);
void TrialRoad_SetUpPillarStage(void);
void TrialRoad_SetUpTorchStage(void);
void TrialRoad_SetUpBushPillarStage(void);
void TrialRoad_SetUpPoundStage(void);
void TrialRoad_SetUpCrumblingStage(void);
void TrialRoad_SetUpLogStage(void);
void TrialRoad_SetUpFountainStage(void);

void TrialRoad_SetUpSandStage(void);

void TrialRoad_UpdateFountains(void);

#define DEFINE_TRIAL_ROAD_OPEN_DOOR(name, n1, n2, n3, n4, n5, n6)              \
  void name(s32 side, s32 phase) {                                             \
    if (side == 0) {                                                           \
      if (phase == 3)                                                          \
        MapGrid_CopyCellPictureCellsRect(0, 120, 1, 2, n1, phase);             \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n2, 3);                           \
      MapGrid_CopyCellAttributes(n3, 127, 1, 1, n1, 8);                        \
    } else {                                                                   \
      if (phase == 3)                                                          \
        MapGrid_CopyCellPictureCellsRect(0, 120, 1, 2, n4, phase);             \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n5, 3);                           \
      MapGrid_CopyCellAttributes(n6, 127, 1, 1, n4, 8);                        \
    }                                                                          \
  }

/// Opens the door at the end of the leader's stage, on his side of the road.
DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenLeaderDoor, 23, 57, 0, 9, 43, 1)

/// Opens the door at the end of Moapa's stage, on his side of the road.
DEFINE_TRIAL_ROAD_OPEN_DOOR(TrialRoad_OpenRivalDoor, 28, 62, 1, 4, 38, 0)

/// A step at a stage's foot asks; giving up takes the party back to the start.
s32 TrialRoad_OfferToGiveUp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 message;

  if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId,
                        ((FIELD_CELL(leader->x.fixed)) << 4) + 8,
                        ((FIELD_CELL(leader->z.fixed)) << 4) + 8);
    Event_PlaySound(SE_SPLASH);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    message = (s32)&MsgTrialRoadGiveUpPrompt;
    FieldMessage_Show(message, 13);
    work->choiceMessage = message + 1;
    work->choiceWidth = 4;
    work->choiceSelection = 1;
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 1) ==
        0) {
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
      Event_Wait(10);
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, 22);
      Event_Wait(10);
      FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                           EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
      Event_Wait(30);
      InventorySnapshot_Restore();
      Import_EventSetExitRequest(9);
      return 1;
    }
  }
  Event_SetFlag(FLAG_GIVE_UP_ASKED);
  return 0;
}

/// Stepping away from the stairs lets the offer to give up be asked again.
void TrialRoad_ClearGiveUpAsked(void) { GameFlag_Clear(FLAG_GIVE_UP_ASKED); }

/// The leader reaches the door at a stage's end: the blue chests open
/// (FLAG_INNER_CHEST_OPEN and FLAG_OUTER_CHEST_OPEN), the outer one only when
/// the rival arrived first and twice the weight is due.
void TrialRoad_OpenChests(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId,
                      ((FIELD_CELL(leader->x.fixed)) << 4) + 8,
                      ((FIELD_CELL(leader->z.fixed)) << 4) + 8);
  TrialRoad_Idle();
  found = TrialRoad_RivalIsAhead(0);
  if (found != 0)
    Event_SetFlag(state->stage[0] + FLAG_AREA_TEMP_81);
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

/// Puts an item in the chest the leader faces: "{protagonist} put {actor}'s
/// {item} in the chest." (MsgTrialRoadStoredItem), and the chest closes. Once
/// both chests are closed the leader's door opens (FLAG_STAGE_DOOR_OPEN).
/// Then the item leaves the owner's slot.
void TrialRoad_PutItemInChest(s32 item, s32 owner, s32 slot) {
  s32 chest = Field_FindNearbyTalkTarget(Party_GetLeaderActor());
  struct TrialRoadState *state = gTrialRoadState;

  Event_Start();
  Event_ResolvePendingActions(0);
  MessageArgument_Push(owner, MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
  // "Felix put ...'s ... in the chest."
  FieldMessage_Show((s32)&MsgTrialRoadStoredItem, 1);
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

/// Searching a blue chest before its door is open tells what it wants.
void TrialRoad_ExamineChest(s32 id) {
  s32 flag;

  if (id == ACTOR_LEFT_INNER_CHEST || id == ACTOR_RIGHT_INNER_CHEST)
    flag = FLAG_INNER_CHEST_OPEN;
  else
    flag = FLAG_OUTER_CHEST_OPEN;
  if (!Event_GetFlag(flag)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    // "The chest won't open. There's something keeping it shut tight."
    FieldMessage_Show((s32)MsgTrialRoadChestBlocked, 1);
    Event_End();
  } else {
    // "Place this item in the chest?"
    FieldMessage_Show((s32)MsgTrialRoadPlaceItemPrompt, 13);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 1) ==
        0)
      gFieldWork->fieldMenu = 1;
  }
}

/// Does nothing: the step past the chests needs no event.
void TrialRoad_StepPastChests(void) {}

/// Makes the cell under the actor a wall.
void TrialRoad_BlockCellUnder(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = (u8 *)map->layers[0].cells;
    u8 *cell =
        cells +
        (((FIELD_CELL(object->x.fixed)) + ((FIELD_CELL(object->z.fixed)) << 7))
         << 2);
    s32 bit = 128;

    cell[3] = bit | cell[3];
  }
}

/// Opens the cell under the actor again.
void TrialRoad_UnblockCellUnder(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    u8 *cells = (u8 *)map->layers[0].cells;
    u8 *cell =
        cells +
        (((FIELD_CELL(object->x.fixed)) + ((FIELD_CELL(object->z.fixed)) << 7))
         << 2);

    cell[3] &= 0x7f;
  }
}

/// A pillar top's update: it stays on its pillar at the pillar's lift.
s32 TrialRoad_FollowPillar(struct Actor *object) {
  struct Actor *owner = object->watchedActor;

  SET_ACTOR_POSITION(object, owner->x.fixed,
                     owner->y.fixed + toFIXED(PILLAR_TOP_HEIGHT(object)),
                     owner->z.fixed);
  object->groundHeight = owner->groundHeight;
  return 1;
}

extern s32 gTrialRoad_PillarTopScript[];

/// Puts an invisible top on the pillar for the leader to stand on.
void TrialRoad_AddPillarTop(s32 id) {
  struct Actor *owner = Actor_GetById(id);
  struct Actor *object;

  if (owner != NULL) {
    object = FieldActor_Create(SPRITE_UNSEEN_MARKER, owner->x.fixed,
                               owner->y.fixed, owner->z.fixed);
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      Actor_StartScript(object, gTrialRoad_PillarTopScript);
      object->watchedActor = owner;
      object->priorityFlags |= 0x80;
      object->motionFlags = 0;
      object->collisionFlags = 0;
      PILLAR_TOP_HEIGHT(object) = 16;
      owner->watchedActor = object;
    }
  }
}

/// Sets how many pixels above the pillar its top stands.
void TrialRoad_SetPillarTopLift(s32 id, s32 value) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    object = object->watchedActor;
    if (object != NULL)
      PILLAR_TOP_HEIGHT(object) = value;
  }
}

/// While the party's render mode is 0, clears the map cell flag under the
/// object.
#define DEFINE_TRIAL_ROAD_ON_FOOT(name, onFoot)                                \
  void name(s32 a, s32 id) {                                                   \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
    u8 mode;                                                                   \
                                                                               \
    mode = party->movementMode;                                                \
                                                                               \
    if (mode == 0)                                                             \
      onFoot(id);                                                              \
  }

/// While the party's render mode is 0, clears the map cell flag under the
/// object.
DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_UnblockMovedPillar,
                          TrialRoad_UnblockCellUnder)

/// While the party's render mode is 0, sets the map cell flag under the object.
DEFINE_TRIAL_ROAD_ON_FOOT(TrialRoad_BlockMovedPillar, TrialRoad_BlockCellUnder)

/// Whirlwind blows the bush off the lone pillar, which can then be pushed.
void TrialRoad_BlowBushOffLonePillar(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 0;
  Event_SetFlag(FLAG_AREA_TEMP_07);
}

/// The leader jumps across, if he can.
void TrialRoad_JumpForward(void) { Field_TryJumpForward(); }

/// Opens the cell where the actor stands.
void TrialRoad_OpenSpoutCell(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL)
    MapGrid_CopyCellsLayered(14, 27, 1, 1, FIELD_CELL(object->x.fixed),
                             FIELD_CELL(object->z.fixed));
}

/// Notes whether the leader stands on the pillars' tops.
void TrialRoad_TrackLeaderOnPillars(void) {
  FIXED y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;

  if (y == toFIXED(32))
    Event_SetFlag(FLAG_LEADER_ON_PILLARS);
  else
    Event_ClearFlag(FLAG_LEADER_ON_PILLARS);
}

/// Jumps across the gap when the pillar fills it, else drops down the ledge.
void TrialRoad_JumpAcrossGap(void) {
  if (FIELD_CELL(Actor_GetById(ACTOR_PILLAR_B)->z.fixed) == 23)
    Field_TryJumpForward();
  else
    Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
}

/// Once a pillar is moved off a torch's cell, the cell opens.
void TrialRoad_UnblockTorch(s32 a, s32 id) {
  if ((u32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A)
    TrialRoad_UnblockCellUnder(id);
}

/// A pillar pushed beside a torch blocks its cell; the frozen puddle beside a
/// lit torch melts.
void TrialRoad_MeltIceBesideTorch(s32 a, s32 id) {
  s32 x;

  if ((u32)(id - ACTOR_PILLAR_A) <= ACTOR_PILLAR_B - ACTOR_PILLAR_A) {
    TrialRoad_BlockCellUnder(id);
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
      struct Actor *object = Actor_GetById(id);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(id, 6);
      TrialRoad_SetPillarTopLift(id, -16);
      Event_SetFlag(flag);
    }
  }
}

/// The leader jumps across a melted puddle.
void TrialRoad_JumpMeltedPuddle(void) {
  if (Event_GetFlag(Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId) +
                    FLAG_PUDDLE_MELTED_BASE))
    Field_TryJumpForward();
}

/// Frost freezes a melted puddle into a pillar again.
void TrialRoad_FreezePuddle(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  TrialRoad_SetPillarTopLift(id, 16);
  Event_ClearFlag(id + FLAG_PUDDLE_MELTED_BASE);
  if (object != NULL)
    object->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
}

/// Whirlwind blows the bush off a pillar, which can then be pushed.
void TrialRoad_BlowBushOffPillar(s32 a, s32 id) {
  PUSH_BLOCKED(Actor_GetById(id - (ACTOR_BUSH_A - ACTOR_PILLAR_A))) = 0;
  Event_SetFlag(id + FLAG_BUSH_BLOWN_BASE);
}

/// Lowers a pillar into the ground, its top with it.
void TrialRoad_LowerPillar(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    TrialRoad_SetPillarTopLift(id, -16);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SelectAnimation(object, 6);
  }
}

/// Raises a pillar out of the ground, its top with it.
void TrialRoad_RaisePillar(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    TrialRoad_SetPillarTopLift(id, 16);
    object->collisionFlags = 1;
    object->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    Actor_SelectAnimation(object, 5);
  }
}

/// Whirlwind blows the bush off the pound stage's pillar.
void TrialRoad_BlowBushOffPoundPillar(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 0;
  Event_SetFlag(FLAG_AREA_TEMP_39);
}

/// The pillar each of the pound stage's four pillars, 18 to 21, raises when
/// it is pounded down: they go in pairs.
extern s8 gTrialRoad_PoundPartners[4];

/// Checks the pound stage's pillars against the cells they cover.
void TrialRoad_CheckPoundedPillarsCovered(s32 a, s32 b) {
  s32 id;

  id = ACTOR_WEST_PILLAR;
  do {
    struct Actor *object = Actor_GetById(id);
    s32 x = FIELD_CELL(object->x.fixed);
    s32 z = FIELD_CELL(object->z.fixed);
    u8 *covered = &PILLAR_TOP_HEIGHT(
        Actor_GetById(gTrialRoad_PoundPartners[(id - ACTOR_WEST_PILLAR) & 3]));
    s32 other;

    *covered = 0;
    for (other = ACTOR_PILLAR_A; other <= ACTOR_PILLAR_C; other++) {
      object = Actor_GetById(other);
      if (x == FIELD_CELL(object->x.fixed) &&
          z == FIELD_CELL(object->z.fixed)) {
        *covered = 1;
        break;
      }
    }
    id++;
  } while (id <= ACTOR_EAST_PILLAR);
}

/// Pound drives a pillar down and raises its partner
/// (gTrialRoad_PoundPartners).
void TrialRoad_PoundPillar(s32 id, s32 phase, s32 frame) {
  s32 other = gTrialRoad_PoundPartners[(id - ACTOR_WEST_PILLAR) & 3];
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  if (frame <= 10)
    object->scaleY = FIXED_ONE - (frame << 12);
  else
    object->scaleY = FIXED_ONE;
  if (phase == 1) {
    TrialRoad_LowerPillar(id);
    TrialRoad_RaisePillar(other);
    switch (id) {
    case ACTOR_WEST_PILLAR:
      Event_SetFlag(FLAG_AREA_TEMP_35);
      break;
    case ACTOR_INNER_WEST_PILLAR:
      Event_ClearFlag(FLAG_AREA_TEMP_35);
      break;
    case ACTOR_INNER_EAST_PILLAR:
      Event_SetFlag(FLAG_AREA_TEMP_37);
      break;
    case ACTOR_EAST_PILLAR:
      Event_ClearFlag(FLAG_AREA_TEMP_37);
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

/// Pound on the west pillar.
void TrialRoad_PoundWestPillar(s32 phase, s32 frame) {
  TrialRoad_PoundPillar(ACTOR_WEST_PILLAR, phase, frame);
}

/// Pound on the inner west pillar.
void TrialRoad_PoundInnerWestPillar(s32 phase, s32 frame) {
  TrialRoad_PoundPillar(ACTOR_INNER_WEST_PILLAR, phase, frame);
}

/// Pound on the inner east pillar.
void TrialRoad_PoundInnerEastPillar(s32 phase, s32 frame) {
  TrialRoad_PoundPillar(ACTOR_INNER_EAST_PILLAR, phase, frame);
}

/// Pound on the east pillar.
void TrialRoad_PoundEastPillar(s32 phase, s32 frame) {
  TrialRoad_PoundPillar(ACTOR_EAST_PILLAR, phase, frame);
}

/// Stepping onto a raised pillar of a pair sinks it and raises its partner.
void TrialRoad_StepOnSeesawPillar(s32 id, s32 other, s32 flag, s32 side) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *partner = Actor_GetById(other);
  FIXED t;

  if (PILLAR_TOP_HEIGHT(object) != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  partner->scaleY = FIXED_ONE / 8;
  wait(1);
  TrialRoad_RaisePillar(other);
  t = FIXED_ONE / 8;
  do {
    TrialRoad_SetPillarTopLift(id, (FIXED_ONE / 2 - t) / (FIXED_ONE / 32));
    object->scaleY = FIXED_ONE - t;
    partner->scaleY = t;
    wait(1);
    t += FIXED_ONE / 8;
  } while (t <= FIXED_ONE);
  object->scaleY = FIXED_ONE;
  TrialRoad_LowerPillar(id);
  Event_PlaySound(SE_ITEM_BREAK);
  if (side != 0)
    Event_SetFlag(flag);
  else
    Event_ClearFlag(flag);
  Event_End();
}

/// Stepping onto the west pillar sinks it and raises the inner west one.
void TrialRoad_StepOnWestPillar(void) {
  TrialRoad_StepOnSeesawPillar(ACTOR_WEST_PILLAR, ACTOR_INNER_WEST_PILLAR,
                               FLAG_AREA_TEMP_35, 1);
}

/// Stepping onto the inner west pillar sinks it and raises the west one.
void TrialRoad_StepOnInnerWestPillar(void) {
  TrialRoad_StepOnSeesawPillar(ACTOR_INNER_WEST_PILLAR, ACTOR_WEST_PILLAR,
                               FLAG_AREA_TEMP_35, 0);
}

/// Stepping onto the inner east pillar sinks it and raises the east one.
void TrialRoad_StepOnInnerEastPillar(void) {
  TrialRoad_StepOnSeesawPillar(ACTOR_INNER_EAST_PILLAR, ACTOR_EAST_PILLAR,
                               FLAG_AREA_TEMP_37, 1);
}

/// Stepping onto the east pillar sinks it and raises the inner east one.
void TrialRoad_StepOnEastPillar(void) {
  TrialRoad_StepOnSeesawPillar(ACTOR_EAST_PILLAR, ACTOR_INNER_EAST_PILLAR,
                               FLAG_AREA_TEMP_37, 0);
}

/// Jumps onto a pillar from a raised one, else pushes the pillar ahead.
void TrialRoad_LandOnPillar(void) {
  FIXED y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;

  if (y >= toFIXED(16)) {
    Field_TryJumpForward();
  } else if (Leader_CanPushObjectAhead()) {
    Leader_PushObjectAhead();
    TrialRoad_CheckPoundedPillarsCovered(0, 0);
  }
}

/// Each step checks the crumbling pillars under the leader.
void TrialRoad_CheckCrumblingPillars(void) {
  MagmaRockInside_CheckCrumblingStep(gTrialRoad_CrumblingPillars);
}

/// Burst crumbles the west crumbling pillar.
void TrialRoad_CrumbleWestPillar(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars,
                            ACTOR_WEST_CRUMBLING_PILLAR);
}

/// Burst crumbles the east crumbling pillar.
void TrialRoad_CrumbleEastPillar(s32 phase) {
  if (phase == 3)
    TrialRoad_CrumblePillar(gTrialRoad_CrumblingPillars,
                            ACTOR_EAST_CRUMBLING_PILLAR);
}

// Blocks the three cells through an object's cell on layers 0 and 2 (collision
// code 255): the column above and below it in mode 0, the row beside it
// otherwise.

/// Blocks the three cells through an object's cell on layers 0 and 2 (collision
/// code 255): the column above and below it in mode 0, the row beside it
/// otherwise.
void TrialRoad_BlockLogCells(s32 id, s32 mode) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_GetById(id);
  s32 index;
  struct MapCell *low;
  struct MapCell *high;

  if (object == 0)
    return;
  index = (FIELD_CELL(object->x.fixed)) + ((FIELD_CELL(object->z.fixed)) << 7);
  low = map->layers[0].cells + index;
  high = map->layers[2].cells + index;
  if (mode == 0) {
    low[-MAPGRID_SIZE].collisionCode = CELL_WALL;
    low->collisionCode = CELL_WALL;
    low[MAPGRID_SIZE].collisionCode = CELL_WALL;
    high[-MAPGRID_SIZE].collisionCode = CELL_WALL;
    high->collisionCode = CELL_WALL;
    high[MAPGRID_SIZE].collisionCode = CELL_WALL;
  } else {
    low[-1].collisionCode = CELL_WALL;
    low->collisionCode = CELL_WALL;
    low[1].collisionCode = CELL_WALL;
    high[-1].collisionCode = CELL_WALL;
    high->collisionCode = CELL_WALL;
    high[1].collisionCode = CELL_WALL;
  }
}

/// Makes the logs' cells walls where they lie.
void TrialRoad_BlockLogs(void) {
  MapGrid_CopyCellAttributes(11, 64, 11, 8, 11, 22);
  MapGrid_CopyCellAttributes(79, 64, 11, 8, 79, 22);
  TrialRoad_BlockLogCells(ACTOR_UPPER_LOG, 0);
  TrialRoad_BlockLogCells(ACTOR_LOWER_LOG, 0);
  TrialRoad_BlockLogCells(ACTOR_EAST_LOG, 1);
  if (Event_GetFlag(FLAG_SCENE_TEMP_53))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 23);
  if (Event_GetFlag(FLAG_SCENE_TEMP_54))
    MapGrid_CopyCellAttributes(0, 48, 1, 3, 19, 26);
}

/// Whether a log rolling to the cell is stopped by the block or a wall.
s32 TrialRoad_IsLogBlocked(s32 x, s32 z) {
  struct Actor *object = Actor_GetById(ACTOR_PILLAR_A);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *cells;
  u8 *cell;
  s32 value;

  if (FIELD_CELL(object->x.fixed) == x && FIELD_CELL(object->z.fixed) == z)
    return 1;
  cells = (u8 *)map->layers[0].cells;
  cell = cells + ((x + (z << 7)) << 2);
  value = cell[2];
  return IsZero(value ^ 0xff);
}

/// Pushes a log, which rolls on until something stops it.
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis) {
  struct Actor *object = Actor_GetById(id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 fromX = FIELD_CELL(object->x.fixed);
  s32 fromZ = FIELD_CELL(object->z.fixed);
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
    gFieldWork->blockedSteps = 13;
    return;
  }
  for (n = 0; n <= 5;) {
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
    n++;
    toZ = z;
  }
  if (fromX == toX && fromZ == toZ)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 8);
  Event_Wait(6);
  object->speedLimit = toFIXED(1) / 2;
  object->acceleration = toFIXED(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  if (dx < 0 || dz < 0)
    mode = 3;
  Actor_SelectAnimation(object, mode);
  Actor_SetMoveTarget(object, FIELD_CELL_CENTER(toX), 0,
                      z = FIELD_CELL_CENTER(toZ));
  Event_Wait(6);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.2);
  Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, dx << 3,
                                 dz << 3);
  Event_Wait(24);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_WaitForMove(object);
  if (toX <= 17) {
    Actor_SelectAnimation(object, 1);
    Sound_Play(SE_STOP_EXIT_SE);
    Sound_Play(SE_HEAVY_STOP);
  } else {
    Actor_SelectAnimation(object, 3);
    Actor_SetMoveTarget(object, toFIXED(312), 0, z);
    Event_Wait(30);
    Actor_SelectAnimation(object, 8);
    Actor_WaitForMove(object);
    {
      /* FAKEMATCH(pin): the 2 held in r2 while the address takes r3. */
      u8 *flags = &object->priorityFlags;
      register s32 value asm("r2") = 2;

      *flags = value;
    }
    Sound_Play(SE_STOP_EXIT_SE);
    Sound_Play(SE_LOG_FALL);
    GameFlag_Set(id + FLAG_LOG_PUSHED_BASE);
  }
  Event_Wait(15);
  Event_End();
  TrialRoad_BlockLogs();
}

/// Pushes the upper log west.
void TrialRoad_PushUpperLogWest(void) {
  TrialRoad_PushLog(ACTOR_UPPER_LOG, -1, 0, 0);
}

/// Pushes the upper log east.
void TrialRoad_PushUpperLogEast(void) {
  TrialRoad_PushLog(ACTOR_UPPER_LOG, 1, 0, 0);
}

/// Pushes the lower log west.
void TrialRoad_PushLowerLogWest(void) {
  TrialRoad_PushLog(ACTOR_LOWER_LOG, -1, 0, 0);
}

/// Pushes the lower log east.
void TrialRoad_PushLowerLogEast(void) {
  TrialRoad_PushLog(ACTOR_LOWER_LOG, 1, 0, 0);
}

/// Pushes the east log north.
void TrialRoad_PushEastLogNorth(void) {
  TrialRoad_PushLog(ACTOR_EAST_LOG, 0, -1, 1);
}

/// Pushes the east log south.
void TrialRoad_PushEastLogSouth(void) {
  TrialRoad_PushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/// Climbing, the leader climbs down; else he pushes the east log south.
void TrialRoad_PushEastLogSouthOrClimbDown(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode == 2)
    Field_ClimbDown();
  else
    TrialRoad_PushLog(ACTOR_EAST_LOG, 0, 1, 1);
}

/// Scoop uncovers a spout at its thirtieth frame: the spout is set on the
/// ground at its own place (its timer and user value) and its cell opens.
void TrialRoad_UncoverSpout(s32 id, s32 phase, s32 frame) {
  if (frame == 30) {
    struct Actor *object = Actor_GetById(id);
    s32 height;

    object->x.fixed = toFIXED(object->timer);
    object->z.fixed = toFIXED((s16)object->userValue);
    height = Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed);
    object->groundHeight = height;
    object->y.fixed = height;
    TrialRoad_OpenSpoutCell(id);
    Event_SetFlag(id + FLAG_SPOUT_UNCOVERED_BASE);
  }
}

/// Scoop uncovers the west spout.
void TrialRoad_UncoverWestSpout(s32 phase, s32 frame) {
  TrialRoad_UncoverSpout(ACTOR_WEST_SPOUT, phase, frame);
}

/// Scoop uncovers the middle spout.
void TrialRoad_UncoverMiddleSpout(s32 phase, s32 frame) {
  TrialRoad_UncoverSpout(ACTOR_MIDDLE_SPOUT, phase, frame);
}

/// Scoop uncovers the east spout.
void TrialRoad_UncoverEastSpout(s32 phase, s32 frame) {
  TrialRoad_UncoverSpout(ACTOR_EAST_SPOUT, phase, frame);
}

/// Lifts the leader onto the block when he stands on its cell.
void TrialRoad_StandOnBlock(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *block = Actor_GetById(ACTOR_PILLAR_A);

  if (FIELD_CELL(block->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(block->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    FIXED y = leader->y.fixed;

    if (y < toFIXED(32))
      leader->y.fixed = toFIXED(32);
  }
}

/// While Sand is cast, the leader moves on layer 2.
void TrialRoad_StartSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// As Sand fades, the leader moves on layer 0 again.
void TrialRoad_EndSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Whirlwind blows the bush off the vine and opens the way up it.
void TrialRoad_BlowBushOffVine(void) {
  MapGrid_CopyCellAttributes(12, 21, 1, 1, 19, 21);
  Event_SetFlag(FLAG_AREA_TEMP_74);
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_TRIAL_ROAD_OPEN_DOOR
#undef DEFINE_TRIAL_ROAD_ON_FOOT
// Trial Road's effect spawner and the sand stage's stream.

extern const s32 *gFieldEffect_DurationScripts[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect =
        ((struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - FIXED_ONE) / frames;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "../field/field_effect_advance_motion_priority.c"

/// In the stream the leader drifts south, throwing spray.
void TrialRoad_DriftWithStream(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  s32 velocity[3];

  if (gMainRoutineWork.movementMode == 4)
    return;
  leader->z.fixed += toFIXED(1);
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = FIXED_ONE * 8 / 10;
  velocity[1] = 0;
  velocity[2] = FIXED_ONE;
  velocity[0] = 0;
  velocity[0] += ((Random16() << 17) >> 16) - toFIXED(1);
  velocity[2] += ((Random16() << 16) >> 16) - 0x8000;
  FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                    velocity[0], velocity[1], velocity[2],
                    EFFECT_USE_UPDATE | 1, &options);
}

// Trial Road's rooms set up, the rival Moapa's race, the peak, the
// fountains and crumbling pillars, the race's progress display and the tables.

/// Lift raises the rock at the cave's mouth.
void TrialRoad_LiftCaveRock(void) { GameFlag_Set(FLAG_CAVE_ROCK_LIFTED); }

/// Cyclone cuts the bushes it touches.
void TrialRoad_CutBushes(void) {
  Cyclone_CopyTouchedCells_(68, 6, 1, 21, 13, 64, 64);
}

/// Cyclone cuts the bushes, and the Mad Plant hiding in them attacks.
void TrialRoad_CutMadPlantBushes(void) {
  Cyclone_CopyTouchedCells_(68, 6, 1, 21, 13, 64, 64);
  if (!Event_GetFlag(FLAG_TRIAL_ROAD_MAD_PLANT_BEATEN)) {
    Import_MadPlantLeapAtLeader(249, 72, 152);
    gMainRoutineWork.pendingDjinni =
        IF_FLAG_SET(FLAG_TRIAL_ROAD_MAD_PLANT_BEATEN);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(106, 4);
  }
}

/// Always 1.
s32 TrialRoad_ReturnTrue(void) { return 1; }

#define DEFINE_TRIAL_ROAD_SET_POOL_TILES(name, n1, n2)                         \
  void name(void) {                                                            \
    MapGrid_CopyCells(n1, 30, 1, 1, 10, 11);                                   \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 7);                                    \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 15);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 74, 11);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 7);                                    \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 15);                                   \
  }

/// Shows the pools' hidden tiles.
DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_RevealPoolTiles, 33, 35)

/// Hides the pools' tiles again.
DEFINE_TRIAL_ROAD_SET_POOL_TILES(TrialRoad_HidePoolTiles, 34, 36)

/// The gate at the top opens while the party's equipment is held.
void TrialRoad_OpenSummitGate(s32 sound) {
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
  Event_SetFlag(FLAG_SCENE_TEMP_83);
}

/// Moapa at the summit: the race's result decides his challenge.
void TrialRoad_ChallengeMoapa(void) {
  struct MainRoutineWork *party;
  s32 count = Event_GetFlag(FLAG_AREA_TEMP_81);

  count += Event_GetFlag(FLAG_AREA_TEMP_82);
  count += Event_GetFlag(FLAG_AREA_TEMP_83);
  count += Event_GetFlag(FLAG_AREA_TEMP_84);
  if (Event_GetFlag(FLAG_AREA_TEMP_85))
    TrialRoad_ReachPeakFirst();
  else
    TrialRoad_ReachPeakSecond();
  party = &gMainRoutineWork;
  party->battleType = BATTLE_TYPE_NO_ESCAPE;
  do {
  } while (0);
  Event_SetAfterBattleDestination(SCENE_TRIAL_ROAD_SUMMIT,
                                  TRIAL_ROAD_SUMMIT_AFTER_WIN);
  Event_SetDefeatDestination(SCENE_TRIAL_ROAD_SUMMIT,
                             TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
  Field_RequestBattleFromEncounterTable(ACTOR_WEST_GUARD, 4 - count);
}

/// When the object stands in column 8, sets FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
/// stops its motion and lifts it one pixel.
void TrialRoad_SetSummitPillar(s32 a, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (FIELD_CELL(object->x.fixed) == 8) {
    Event_SetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED);
    object->motionFlags = 0;
    object->y.fixed -= toFIXED(1);
  }
}

extern struct Trigger gTrialRoad_Triggers[];
extern struct Trigger gTrialRoad_CaveRockTriggers[];
extern struct Trigger gTrialRoad_GaspTriggers[];
extern struct Trigger gTrialRoad_SummitTriggers[];
extern s32 gTrialRoad_StagesTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 pattern = gTrialRoadState->pattern;

  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_STAGES)
    return gTrialRoad_StagesTriggers[pattern & 7];
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_CAVE_ROCK)
    return gTrialRoad_CaveRockTriggers;
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_GASP)
    return gTrialRoad_GaspTriggers;
  if (gMainRoutineWork.next == SCENE_TRIAL_ROAD_SUMMIT)
    return gTrialRoad_SummitTriggers;
  return gTrialRoad_Triggers;
}

/// Readies the pillar stage: the lone pillar stays blocked by its bush until
/// Whirlwind blows it off, and each pillar gets its top.
void TrialRoad_SetUpPillarStage(void) {
  if (!Event_GetFlag(FLAG_AREA_TEMP_07))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 1;
  else
    Actor_SetPosition(ACTOR_BUSH_A, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_B);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_C);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_D);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_E);
  Task_Add(TrialRoad_TrackLeaderOnPillars, TASK_PASS_VBLANK);
}

/// Readies the torch stage: each pillar gets its top.
void TrialRoad_SetUpTorchStage(void) {
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_F);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_A);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_B);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_C);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_D);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_E);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_F);
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_C)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_D)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_E)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_F)) = 1;
  if (!Event_GetFlag(FLAG_SCENE_TEMP_14))
    Actor_SetAnimation(ACTOR_PILLAR_C, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_C, -16);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_15))
    Actor_SetAnimation(ACTOR_PILLAR_D, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_D, -16);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_16))
    Actor_SetAnimation(ACTOR_PILLAR_E, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_E, -16);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_17))
    Actor_SetAnimation(ACTOR_PILLAR_F, 5);
  else
    TrialRoad_SetPillarTopLift(ACTOR_PILLAR_F, -16);
}

/// Readies the stage of pillars in bushes: each stays blocked until Whirlwind
/// blows its bush off.
void TrialRoad_SetUpBushPillarStage(void) {
  if (!Event_GetFlag(FLAG_AREA_TEMP_27))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_A)) = 1;
  else
    Actor_SetPosition(ACTOR_BUSH_A, 0, 0);
  if (!Event_GetFlag(FLAG_AREA_TEMP_28))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 1;
  else
    Actor_SetPosition(ACTOR_BUSH_B, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_A);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_B);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_C);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_D);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_E);
}

/// Readies the pound stage: its four pillars cannot be pushed.
void TrialRoad_SetUpPoundStage(void) {
  PUSH_BLOCKED(Actor_GetById(ACTOR_WEST_PILLAR)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_INNER_WEST_PILLAR)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_INNER_EAST_PILLAR)) = 1;
  PUSH_BLOCKED(Actor_GetById(ACTOR_EAST_PILLAR)) = 1;
  if (!Event_GetFlag(FLAG_AREA_TEMP_39))
    PUSH_BLOCKED(Actor_GetById(ACTOR_PILLAR_B)) = 1;
  else
    Actor_SetPosition(ACTOR_POUND_STAGE_BUSH, 0, 0);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_B);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_C);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_D);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_E);
  TrialRoad_AddPillarTop(ACTOR_PILLAR_F);
  TrialRoad_AddPillarTop(ACTOR_EAST_PILLAR);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_A);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_B);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_C);
  if (!Event_GetFlag(FLAG_AREA_TEMP_35)) {
    TrialRoad_RaisePillar(ACTOR_WEST_PILLAR);
    TrialRoad_LowerPillar(ACTOR_INNER_WEST_PILLAR);
  } else {
    TrialRoad_RaisePillar(ACTOR_INNER_WEST_PILLAR);
    TrialRoad_LowerPillar(ACTOR_WEST_PILLAR);
  }
  if (!Event_GetFlag(FLAG_AREA_TEMP_37)) {
    TrialRoad_RaisePillar(ACTOR_INNER_EAST_PILLAR);
    TrialRoad_LowerPillar(ACTOR_EAST_PILLAR);
  } else {
    TrialRoad_RaisePillar(ACTOR_EAST_PILLAR);
    TrialRoad_LowerPillar(ACTOR_INNER_EAST_PILLAR);
  }
}

void MagmaRockInside_RestoreCrumblingPillars(const u16 *table);

/// Readies the crumbling stage: the pillars already crumbled stay gone.
void TrialRoad_SetUpCrumblingStage(void) {
  MagmaRockInside_RestoreCrumblingPillars(gTrialRoad_CrumblingPillars);
}

/// Readies the log stage: the logs cannot be pushed as blocks are and are not
/// solid, their cells are walls (TrialRoad_BlockLogs), and the block on pillar
/// A gets its top.
void TrialRoad_SetUpLogStage(void) {
  MapGrid_CopyCellAttributes(11, 22, 11, 8, 11, 64);
  MapGrid_CopyCellAttributes(79, 22, 11, 8, 79, 64);
  Actor_GetById(ACTOR_UPPER_LOG)->riseCounter = 1;
  Actor_GetById(ACTOR_LOWER_LOG)->riseCounter = 1;
  Actor_GetById(ACTOR_EAST_LOG)->riseCounter = 1;
  Actor_GetById(ACTOR_UPPER_LOG)->collisionFlags = 0;
  Actor_GetById(ACTOR_LOWER_LOG)->collisionFlags = 0;
  Actor_GetById(ACTOR_EAST_LOG)->collisionFlags = 0;
  TrialRoad_BlockLogs();
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_A);
}

/// One of the fountain stage's fountains and the spout drawn over it.
struct TrialRoadFountain {
  s16 id;     ///< its actor; 0 for an empty slot
  s16 phase;  ///< frames into its period
  s16 height; ///< how far it has risen, in pixels
  s16 period; ///< frames in one rise and fall
  s16 riseAt; ///< the phase it starts to rise at
  s16 fallAt; ///< the phase it starts to fall at
  s16 high;
  s16 low;
  s16 unusedAfterLow;   ///< set to 1 as the fountain is added; nothing reads it
  s16 speed;            ///< pixels a frame, 4 rising and -8 falling
  s16 unusedAfterSpeed; ///< cleared as the fountain is added; nothing reads it
  s16 bob;              ///< the spout's sine phase
  struct Actor *spout;  ///< drawn over it
};

/// The fountain stage's work, kept in the inventory snapshot's spare room.
struct TrialRoadFountains {
  s16 vramEntry;
  s16 shape; ///< the first of the shape grid's values for the fountains
  struct TrialRoadFountain fountains[16];
  u32 *sprites;
};

#define TRIAL_ROAD_FOUNTAINS                                                   \
  ((struct TrialRoadFountains *)(gInventorySnapshot + 0x188))

struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high);

/// Readies the fountain stage: its five spouts rise and fall together, the
/// northern three 64 pixels and the southern two 32; each of the northern three
/// stays hidden until it has been uncovered, and then opens its cell; the block
/// on pillar A gets its top.
void TrialRoad_SetUpFountainStage(void) {
  struct TrialRoadFountain *fountain;

  TrialRoad_StartFountains(0, FOUNTAIN_PERIOD);
  fountain = TrialRoad_AddFountain(0, ACTOR_WEST_SPOUT, 0, 0, 64);
  fountain->period = FOUNTAIN_PERIOD;
  fountain->riseAt = FOUNTAIN_RISE_AT;
  fountain->fallAt = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(1, ACTOR_MIDDLE_SPOUT, 0, 0, 64);
  fountain->period = FOUNTAIN_PERIOD;
  fountain->riseAt = FOUNTAIN_RISE_AT;
  fountain->fallAt = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(2, ACTOR_EAST_SPOUT, 0, 0, 64);
  fountain->period = FOUNTAIN_PERIOD;
  fountain->riseAt = FOUNTAIN_RISE_AT;
  fountain->fallAt = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(3, ACTOR_SOUTHWEST_SPOUT, 0, 0, 32);
  fountain->period = FOUNTAIN_PERIOD;
  fountain->riseAt = FOUNTAIN_RISE_AT;
  fountain->fallAt = FOUNTAIN_FALL_AT;
  fountain = TrialRoad_AddFountain(4, ACTOR_SOUTHEAST_SPOUT, 0, 0, 32);
  fountain->period = FOUNTAIN_PERIOD;
  fountain->riseAt = FOUNTAIN_RISE_AT;
  fountain->fallAt = FOUNTAIN_FALL_AT;
  if (!Event_GetFlag(FLAG_AREA_TEMP_63))
    Actor_SetPosition(ACTOR_WEST_SPOUT, 0, 0);
  else
    TrialRoad_OpenSpoutCell(ACTOR_WEST_SPOUT);
  if (!Event_GetFlag(FLAG_AREA_TEMP_64))
    Actor_SetPosition(ACTOR_MIDDLE_SPOUT, 0, 0);
  else
    TrialRoad_OpenSpoutCell(ACTOR_MIDDLE_SPOUT);
  if (!Event_GetFlag(FLAG_AREA_TEMP_65))
    Actor_SetPosition(ACTOR_EAST_SPOUT, 0, 0);
  else
    TrialRoad_OpenSpoutCell(ACTOR_EAST_SPOUT);
  ((struct MapGroundEffectsView *)Ram_HeapSlots->mapGrid)->middleBits = 1;
  TrialRoad_AddPillarTop(ACTOR_PILLAR_A);
  TrialRoad_BlockCellUnder(ACTOR_PILLAR_A);
}

/// Readies the sand stage: its tightrope, and the vine once Whirlwind has
/// blown its bush away.
void TrialRoad_SetUpSandStage(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_WEST_POST, ACTOR_EAST_POST);
  if (!Event_GetFlag(FLAG_AREA_TEMP_74))
    MapGrid_CopyCellAttributes(18, 21, 1, 1, 19, 21);
  else
    Actor_SetPosition(ACTOR_SAND_STAGE_BUSH, 0, 0);
}

/// Does nothing: the summit's cave needs no setting up.
void TrialRoad_SetUpCave(void) {}

/// Gasp keeps away from the leader until he is cornered.
void TrialRoad_GaspFlees(void) {
  struct Actor *partner = Actor_GetById(ACTOR_GASP);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 dx;
  s32 dz;
  s32 near;
  s32 far;
  FIXED y;

  if (Event_GetFlag(FLAG_SCENE_TEMP_88))
    return;
  dx = FIXEDtoINT(leader->x.fixed - partner->x.fixed);
  dz = FIXEDtoINT(leader->z.fixed - partner->z.fixed);
  near = dx * dx + dz * dz;
  dx = (FIXEDtoINT(leader->x.fixed)) - 296;
  dz = (FIXEDtoINT(leader->z.fixed)) - 248;
  far = dx * dx + dz * dz;
  if (far <= 6400 || near <= 2304)
    Actor_SetMoveTarget(partner, toFIXED(296 - dx), partner->y.fixed,
                        toFIXED(248 - dz));
  if (far > 15)
    return;
  Event_PlaySound(SE_ENEMY_CRY);
  Event_PlaySound(SE_DJINNI_FLEES);
  partner->speedLimit = toFIXED(2);
  partner->acceleration = toFIXED(2);
  partner->motionFlags = ACTOR_FALLS;
  partner->groundHeight = 0;
  partner->collisionFlags = 1;
  partner->velocityY = toFIXED(10);
  Actor_SetScript(ACTOR_GASP, gTrialRoad_GaspHopScript);
  y = partner->y.fixed;
  Actor_SetMoveTarget(partner, toFIXED(344), y, toFIXED(264));
  Event_SetFlag(FLAG_SCENE_TEMP_88);
}

/// Whether the palette cycle runs; the frame it has reached.
extern s32 gTrialRoad_PaletteCycleRunning;

/// Unless Gasp is caught, readies him: before FLAG_SCENE_TEMP_88 he hovers 8.5
/// pixels up with motion 5, after it he stands with motion 3; then schedules
/// TrialRoad_GaspFlees.
void TrialRoad_SetUpPoolRoom(void) {
  if (!Event_GetFlag(FLAG_DJINNI(DJINNI_GASP))) {
    struct Actor *object = Actor_GetById(ACTOR_GASP);
    s32 started = Event_GetFlag(FLAG_SCENE_TEMP_88);
    u8 *flags = &object->motionFlags;

    if (!started) {
      *flags = 5;
      object->y.fixed += toFIXED(17) / 2;
    } else {
      *flags = 3;
      object->y.fixed = 0;
    }
    Task_Add(TrialRoad_GaspFlees, TASK_PASS_VBLANK);
  }
}

/// Starts or stops the pools' palette cycle.
void TrialRoad_SetPaletteCycleRunning(s32 value) {
  gTrialRoad_PaletteCycleRunning = value;
}

extern s32 gTrialRoad_PaletteCycleFrame;

void PaletteCycle_StepAt(void) {
  u16 colors[16];
  s32 phase = (gTrialRoad_PaletteCycleFrame / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gTrialRoad_PaletteCycleRunning != 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    s16 *state = &work->psynergy;

    if (*state != 0 && (*state & 0x3ff) != ABILITY_HOVER)
      return;
    if (work->revealActive != 0)
      return;
    if (work->pauseMenu != 0)
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

/// Starts the pools' palette cycle, loading its colours unless mode is 2; mode
/// 1 also starts its task.
void TrialRoad_StartPaletteCycle(s32 mode) {
  gTrialRoad_PaletteCycleRunning = 1;
  if (mode != 2)
    Iwram_CopyWords(gTrialRoad_PaletteCycleColors, (void *)&BG_PLTT_COLOR(5, 0),
                    32);
  if (mode == 1) {
    s32 zero = 0;

    gTrialRoad_PaletteCycleFrame = zero;
    Task_Add(PaletteCycle_StepAt, TASK_PASS_VBLANK);
  }
}

void TrialRoad_LeaveMoapaWaiting(void);

/// The mountain's peak: after the battle, the victory or the defeat; on
/// arrival, Moapa and his knights wait.
void TrialRoad_SetUpPeak(void) {
  s32 entrance = gMainRoutineWork.entrance;

  if (entrance == TRIAL_ROAD_SUMMIT_AFTER_WIN) {
    InventorySnapshot_Restore();
    TrialRoad_BeatMoapa();
    Import_EventSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_WIN);
  } else if (entrance == TRIAL_ROAD_SUMMIT_AFTER_DEFEAT) {
    InventorySnapshot_Restore();
    TrialRoad_LoseToMoapa();
    Import_EventSetExitRequest(TRIAL_ROAD_SUMMIT_AFTER_DEFEAT);
  } else if (Event_GetFlag(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT)) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_83))
      TrialRoad_OpenSummitGate(0);
    if (Event_GetFlag(FLAG_AREA_TEMP_85))
      TrialRoad_LeaveMoapaWaiting();
    else
      TrialRoad_PlaceMoapaAtPeak();
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
  TrialRoad_StartPaletteCycle(1);
  TrialRoad_AddPillarTop(ACTOR_PEAK_PILLAR);
  if (Event_GetFlag(FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED))
    Actor_SetPosition(ACTOR_PEAK_PILLAR, 136, 328);
}

/// Each stage pattern's set-up, by the pattern's low three bits.

extern void (*gTrialRoad_StageSetUps[])(void);

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;
  s32 pattern = gTrialRoadState->pattern;

  if (gFieldWork->exit != 0)
    return 0;
  if (scene == SCENE_TRIAL_ROAD_STAGES) {
    if (entrance <= 7) {
      Event_SetFlag(FLAG_RETREAT_BLOCKED);
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
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

/// Opens the door at the end of Moapa's stage, on his side of the road.
void TrialRoad_OpenRivalDoor(s32 mirrored, s32 stage);

/// Each frame of the race, moves Moapa's party on by its recording.
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
extern s8 gTrialRoad_DemoSceneryStages[];
extern s8 gTrialRoad_SceneryStages[];

void TrialRoad_StartRival(s32 mirrored);

/// Places `layer` `column` cells into the map: its offset (column << 20), its
/// cells and terrain from that column on, and its position from the view.
#define SET_LAYER(column, x)                                                   \
  words = (s32 *)layer;                                                        \
  words[2] = (column) << 20;                                                   \
  words[3] = row;                                                              \
  x = (column);                                                                \
  layer->cells = (struct MapCell *)Ram_MapCellBuffer + x;                      \
  layer->terrainIds = gMapShapeGrid + x;                                       \
  words[0] = Iwram_MulFX(*viewX, words[4]) + words[2];                         \
  words[1] = Iwram_MulFX(*viewZ, words[5]) + words[3]

s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.next;
  s32 entrance = gMainRoutineWork.entrance;

  if (scene == SCENE_TRIAL_ROAD_STAGES) {
    struct MapWork *map = Ram_HeapSlots->mapGrid;
    struct TrialRoadState *state = gTrialRoadState;
    struct Actor *leader;
    u8 *unit;
    s32 value;
    s32 n;
    s32 px;
    u32 i;

    if ((u32)(entrance - TRIAL_ROAD_FROM_VILLAGE_LEFT) <= 1) {
      if (!GameFlag_Get(FLAG_AREA_TEMP_80)) {
        unit = Unit_Get(gMainRoutineWork.leaderActorId);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n < UNIT_NAME_SIZE - 1; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(FLAG_AREA_TEMP_80);
      }
      state->mirrored = (entrance - 1) & 1;
      state->stage[0] = 0;
      state->stage[1] = 0;
      state->leaderBest = 0;
      state->rivalBest = 0;
      value = state->order[state->mirrored ^ 1];
      TrialRoad_PlayRivalRecording((s32)gTrialRoad_RivalRecordings[value & 7]);
      state->pattern = state->order[state->stage[0] * 2 + state->mirrored];
      n = entrance % 10;
      Event_SetDestination(SCENE_TRIAL_ROAD_STAGES, n);
    } else if ((u32)(entrance - TRIAL_ROAD_FROM_SUMMIT_LEFT) <= 1) {
      if (!GameFlag_Get(FLAG_AREA_TEMP_80)) {
        unit = Unit_Get(gMainRoutineWork.leaderActorId);
        Iwram_ClearWordsEntry(state, 32);
        value = 0;
        for (n = 0; n < UNIT_NAME_SIZE - 1; n++)
          value += unit[n];
        TrialRoad_DealStageOrder(value);
        GameFlag_Set(FLAG_AREA_TEMP_80);
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
      n = entrance % 10;
      Event_SetDestination(SCENE_TRIAL_ROAD_STAGES, n);
    } else if (entrance == TRIAL_ROAD_RULES) {
      unit = Unit_Get(gMainRoutineWork.leaderActorId);
      Iwram_ClearWordsEntry(state, 32);
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
          if (gTrialRoad_DemoSceneryStages[i] != value)
            Actor_SetPosition(i + 64, 0, 0);
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
        destinationY = 13;
        value = state->pattern;
        px = gTrialRoad_StageMapCells[value][0];
        row = gTrialRoad_StageMapCells[value][1];
        Map_CopyRawCellsRect(px, row, 13, 21, 10, destinationY);
        Map_CopyRawCellsRect(px + 42, row, 13, 21, 44, destinationY);
        Map_CopyRawCellsRect(px + 84, row, 13, 21, 78, destinationY);
        for (i = 0; i <= 9; i++) {
          if (gTrialRoad_SceneryStages[i] != value)
            Actor_SetPosition(i + 64, 0, 0);
        }
        MapGrid_CenterOnFollowedPoint();
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
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
          TrialRoad_ShowProgress(gMainRoutineWork.leaderActorId, 8);
          TrialRoad_StartRival((s16)(state->mirrored ^ 1));
          Task_Add(TrialRoad_UpdateRival, TASK_PASS_VBLANK);
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
             !GameFlag_Get(FLAG_AREA_TEMP_85)) {
    if (gTrialRoadState->mirrored != 0)
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 13, 79);
    else
      MapGrid_CopyCellsLayered(26, 79, 1, 2, 18, 79);
  }
  return 0;
}

// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the
// eight slots at the start of the stage state, again until both the even and
// the odd slots hold at least one of the four values in gTrialRoad_KeyStages;
// the random state is restored after.

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
    return (DEGtoANG(180) - angle) & 0xffff;
  return angle & 0xffff;
}

/// Places an actor at a pixel position and facing, mirrored when the leader
/// took the right path.
void TrialRoad_PlaceActor(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetPositionAndFacing(id, toFIXED(TrialRoad_MirrorX(x)), toFIXED(z),
                             (u16)TrialRoad_MirrorAngle(angle));
}

/// Does nothing: Moapa stays where he waits.
void TrialRoad_LeaveMoapaWaiting(void) {}

/// Places Moapa and his two guards at the peak.
void TrialRoad_PlaceMoapaAtPeak(void) {
  TrialRoad_PlaceActor(ACTOR_MOAPA, 288, 264, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_WEST_GUARD, 296, 248, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_EAST_GUARD, 296, 280, FACING_WEST);
}

void Actor_MoveToObject(s32 actor, s32 value);

/// The party reaches the peak first, before Moapa's men.
void TrialRoad_ReachPeakFirst(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 264, 1);
  Actor_MoveToObject(ACTOR_JENNA, ACTOR_FELIX);
  Actor_MoveToObject(ACTOR_SHEBA, ACTOR_FELIX);
  Actor_MoveToObject(ACTOR_PIERS, ACTOR_FELIX);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_WalkTo(ACTOR_FELIX, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(ACTOR_PIERS, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(ACTOR_JENNA, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(ACTOR_SHEBA, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_FaceDirection(ACTOR_PIERS, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_SHEBA, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
  wait(20);
  Event_PlaySound(SE_ITEM_BREAK);
  wait(20);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 344, 296, FACING_NORTH);
  object = Actor_GetById(ACTOR_MOAPA);
  object->speedLimit = toFIXED(1);
  object->acceleration = toFIXED(1) / 2;
  object->motionFlags = 0;
  Actor_SetMoveTarget(object, object->x.fixed, object->y.fixed + toFIXED(88),
                      object->z.fixed);
  Actor_SelectAnimation(object, 2);
  FieldActor_WaitForMove(object);
  Actor_SelectAnimation(object, 1);
  object->motionFlags = ACTOR_FALLS;
  object->groundHeight = toFIXED(128);
  object->velocityY = toFIXED(4);
  object->speedLimit = toFIXED(3);
  object->acceleration = toFIXED(2);
  Actor_SetMoveTarget(object, object->x.fixed, object->y.fixed,
                      object->z.fixed - toFIXED(16));
  FieldActor_WaitForMove(object);
  wait(12);
  object->speedLimit = toFIXED(1);
  object->acceleration = toFIXED(1) / 2;
  FieldActor_SetMoveTargetFX(object, toFIXED(TrialRoad_MirrorCellX(41)),
                             object->y.fixed, toFIXED(264));
  FieldActor_WaitForMove(object);
  wait(6);
  Event_PlaySound(SE_JUMP);
  object->velocityY = toFIXED(6);
  object->speedLimit = toFIXED(3);
  object->acceleration = toFIXED(2);
  Actor_SetMoveTarget(object, toFIXED(TrialRoad_MirrorCellX(37)),
                      object->y.fixed, toFIXED(264));
  FieldActor_WaitForMove(object);
  wait(6);
  Actor_MoveToObject(ACTOR_WEST_GUARD, 8);
  Actor_MoveToObject(ACTOR_EAST_GUARD, 8);
  Actor_SetSpeed(ACTOR_MOAPA, 1);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
  Actor_WalkTo(ACTOR_MOAPA, TrialRoad_MirrorCellX(36), 264);
  Actor_WalkTo(ACTOR_WEST_GUARD, TrialRoad_MirrorCellX(37), 248);
  Actor_WalkTo(ACTOR_EAST_GUARD, TrialRoad_MirrorCellX(37), 280);
  Actor_WaitForMoveById(ACTOR_MOAPA);
  Actor_FaceDirection(ACTOR_MOAPA, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_WaitForMoveById(ACTOR_WEST_GUARD);
  Actor_FaceDirection(ACTOR_WEST_GUARD, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_WaitForMoveById(ACTOR_EAST_GUARD);
  Actor_FaceDirection(ACTOR_EAST_GUARD, TrialRoad_MirrorAngle(FACING_WEST), 0);
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
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.9);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.7);
  Actor_WalkBy(ACTOR_FELIX, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_MOAPA, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_JENNA, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_WEST_GUARD, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_PIERS, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_EAST_GUARD, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_SHEBA, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

/// The party reaches the peak after Moapa's men.
void TrialRoad_ReachPeakSecond(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(256, CAMERA_KEEP, 264, 1);
  Actor_MoveToObject(ACTOR_JENNA, ACTOR_FELIX);
  Actor_MoveToObject(ACTOR_SHEBA, ACTOR_FELIX);
  Actor_MoveToObject(ACTOR_PIERS, ACTOR_FELIX);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_PIERS, 1);
  Actor_WalkTo(ACTOR_FELIX, TrialRoad_MirrorCellX(28), 272);
  Actor_WalkTo(ACTOR_PIERS, TrialRoad_MirrorCellX(28), 256);
  Actor_WalkTo(ACTOR_JENNA, TrialRoad_MirrorCellX(27), 248);
  Actor_WalkTo(ACTOR_SHEBA, TrialRoad_MirrorCellX(27), 280);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_FaceDirection(ACTOR_FELIX, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_FaceDirection(ACTOR_PIERS, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, TrialRoad_MirrorAngle(0), 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_SHEBA, TrialRoad_MirrorAngle(0), 0);
  FieldCamera_WaitForArrival();
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
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1.9);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1.7);
  Actor_WalkBy(ACTOR_FELIX, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_MOAPA, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_JENNA, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_WEST_GUARD, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_PIERS, TrialRoad_MirrorOffset(3), 0);
  Actor_WalkBy(ACTOR_EAST_GUARD, TrialRoad_MirrorOffset(-3), 0);
  wait(1);
  Actor_WalkBy(ACTOR_SHEBA, TrialRoad_MirrorOffset(3), 0);
  wait(1);
  Event_Wait(6);
}

void PartyInventory_Discard(s32 cue);

/// After beating Moapa at the peak: he gives up the Hover Jade.
void TrialRoad_BeatMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  TrialRoad_PlaceActor(4, 248, 272, 0);
  TrialRoad_PlaceActor(7, 224, 248, 0);
  TrialRoad_PlaceActor(5, 232, 264, 0);
  TrialRoad_PlaceActor(6, 224, 280, 0);
  TrialRoad_PlaceActor(9, 248, 256, 0);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 272, 264, FACING_SOUTH);
  TrialRoad_PlaceActor(ACTOR_WEST_GUARD, 288, 248, FACING_SOUTH);
  TrialRoad_PlaceActor(ACTOR_EAST_GUARD, 288, 280, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_MOAPA, 6);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 6);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Event_SetMessageId((s32)MsgTrialRoadHeroesProven);
  Actor_StartRepeatedMotion(ACTOR_WEST_GUARD, 2);
  wait(4);
  Actor_StartRepeatedMotion(ACTOR_EAST_GUARD, 2);
  wait(10);
  Actor_RunRepeatedMotion(ACTOR_MOAPA, 2);
  // "All right, all right. You've proven your strength, and you are..."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_MOAPA, 7);
  Event_Wait(15);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 7);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 7);
  Event_Wait(5);
  Actor_SetAnimation(ACTOR_MOAPA, 1);
  Event_Wait(15);
  Actor_FaceDirection(ACTOR_MOAPA, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WEST_GUARD, TrialRoad_MirrorAngle(FACING_WEST), 0);
  Actor_SetAnimation(ACTOR_EAST_GUARD, 1);
  Event_Wait(5);
  Actor_FaceDirection(ACTOR_EAST_GUARD, TrialRoad_MirrorAngle(FACING_WEST), 0);
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
  Actor_FaceDirectionAndWait(ACTOR_MOAPA, TrialRoad_MirrorAngle(0));
  Actor_FaceActor(ACTOR_WEST_GUARD, 8, 0);
  Actor_FaceActor(ACTOR_EAST_GUARD, 8, 0);
  Actor_SetAnimationAndWait(ACTOR_MOAPA, 3);
  // "I'm tired now. I'll be back in the Shaman Village resting."
  Event_ShowMessage(ACTOR_MOAPA, 0);
  Actor_SetAnimation(ACTOR_WEST_GUARD, 3);
  wait(3);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Actor_FaceActor(ACTOR_FELIX, 9, 0);
  Actor_FaceActor(ACTOR_PIERS, 9, 0);
  Actor_FaceActor(ACTOR_JENNA, 9, 0);
  Actor_FaceActor(ACTOR_SHEBA, 9, 0);
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

/// After losing to Moapa at the peak.
void TrialRoad_LoseToMoapa(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  TrialRoad_PlaceActor(ACTOR_MOAPA, 264, 264, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_WEST_GUARD, 280, 248, FACING_WEST);
  TrialRoad_PlaceActor(ACTOR_EAST_GUARD, 280, 280, FACING_WEST);
  TrialRoad_PlaceActor(4, 240, 276, 0);
  TrialRoad_PlaceActor(7, 216, 254, 0);
  TrialRoad_PlaceActor(5, 232, 244, 0);
  TrialRoad_PlaceActor(6, 224, 296, 0);
  Actor_SetAnimation(ACTOR_FELIX, 38);
  Actor_SetAnimation(ACTOR_JENNA, 19);
  Actor_SetAnimation(ACTOR_SHEBA, 19);
  Actor_SetAnimation(ACTOR_PIERS, 19);
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

// The byte at 0x5a of an actor, as the field its bit 0 is set through.

extern u32 gTrialRoad_LeftKnightLeavesScript[];

/// The camera tours the road while Moapa explains its traps and its four doors.
void TrialRoad_ShowTraps(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 1016);
  Actor_SetPosition(ACTOR_MOAPA, 1016, 1016);
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x.fixed != 0)
      object->x.fixed -= toFIXED(144);
  }
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetMovementTypeById(ACTOR_WEST_GUARD,
                            gTrialRoad_LeftKnightEntersScript);
  Actor_SetMovementTypeById(ACTOR_EAST_GUARD,
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
  Actor_SetMovementTypeById(ACTOR_WEST_GUARD,
                            gTrialRoad_LeftKnightToChestScript);
  Actor_WaitForScriptEndById(ACTOR_WEST_GUARD);
  Actor_SetAnimationById(ACTOR_WEST_GUARD, 1);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 0.25);
  Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_WEST_GUARD, 0, 8);
  Event_Wait(1);
  Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 2);
  Actor_SetAnimationById(ACTOR_WEST_GUARD, 5);
  Actor_SetDestinationOffset(ACTOR_WEST_GUARD, 0, -40);
  Actor_WaitForMoveById(ACTOR_WEST_GUARD);
  Sound_Play(SE_CURSE);
  MapGrid_SetShake(2, 4, 1);
  Actor_GetById(ACTOR_WEST_GUARD)->velocityY = toFIXED(4);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWaitById(ACTOR_WEST_GUARD, 0, 16);
  Event_Wait(1);
  Actor_GetById(ACTOR_WEST_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(60);
  Actor_FaceDirectionById(ACTOR_WEST_GUARD, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 4);
  // "The door is triggered only when the treasure chests are filled to..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_WEST_GUARD, 3);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_WEST_GUARD, 1);
  Actor_WalkByAndWait(ACTOR_WEST_GUARD, -16, -16);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_WEST, 0);
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 6);
  Event_Wait(30);
  // "Try putting heavier items, like weapons and tools, into the chests..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_WalkToAndWaitById(ACTOR_WEST_GUARD, 200, 168);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_LEFT_INNER_CHEST, 5);
  Event_Wait(20);
  Sound_Play(SE_ITEM_BREAK);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 48, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 14, 8);
  Event_Wait(20);
  Actor_SetMovementTypeById(ACTOR_WEST_GUARD,
                            gTrialRoad_LeftKnightLeavesScript);
  Actor_WaitForScriptEndById(ACTOR_WEST_GUARD);
  Sound_Play(SE_MAP_EXIT);
  // "If you reach the door after your opponents do, you will be at a..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_SetMovementTypeById(ACTOR_EAST_GUARD,
                            gTrialRoad_RightKnightToChestsScript);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  // "You see, it will take twice the weight to open the doors."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  // "Stand on the switch, and the number of chests you need to fill will..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_EAST_GUARD, 1);
  Actor_WalkByAndWait(ACTOR_EAST_GUARD, 16, -16);
  Actor_FaceDirectionById(ACTOR_EAST_GUARD, FACING_EAST, 0);
  Sound_Play(SE_CHEST_OPEN);
  Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 6);
  Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 6);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 350, 168);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_RIGHT_INNER_CHEST, 5);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_EAST_GUARD, 364, 168);
  Actor_FaceDirectionById(ACTOR_EAST_GUARD, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetAnimationById(ACTOR_RIGHT_OUTER_CHEST, 5);
  Event_Wait(20);
  Sound_Play(SE_ITEM_BREAK);
  MapGrid_CopyCellsLayered(127, 3, 1, 2, 53, 3);
  Map_CopyCellAttributeRect(0, 127, 1, 1, 19, 8);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_EAST_GUARD, FACING_NORTHWEST, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_EAST_GUARD, 3);
  // "If you are last to each of the four doors, you will lose eight items..."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  // "If Trial Road proves too much for you, press this button."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Actor_SetMovementTypeById(ACTOR_EAST_GUARD,
                            gTrialRoad_RightKnightLeavesScript);
  Actor_WaitForScriptEndById(ACTOR_EAST_GUARD);
  // "This ends the battle, though. A true hero would not surrender so easily."
  Event_ShowMessageAndWait(ACTOR_MOAPA, 0);
  Import_EventSetExitRequest(10);
}

/// Updates the sixteen movers at TRIAL_ROAD_FOUNTAINS each frame: at its
/// upAt and downAt phases a mover calls Sound_Play (206 or 395) and moves by 4
/// or -8 a frame until it reaches its high or low offset; its phase counts
/// round its period unless flag 260 is set. Each mover's shape, collision
/// height and cell attribute byte follow its actor and offset, and its marker
/// actor is placed over it, bobbing on a sine while the offset is not 0.
void TrialRoad_UpdateFountains(void) {
  struct TrialRoadFountains *work = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *mover = work->fountains;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 shape = work->shape;
  u8 *shapes = map->layers[0].terrainIds;
  u8 *cells = (u8 *)map->layers[0].cells;
  s32 *collision;
  struct Actor *actor;
  struct Actor *marker;
  s32 cell;
  u8 *attributes;
  s32 i;

  i = 0;
  collision = (s32 *)Ram_MapCollision + shape;
  for (; i <= 15; collision++, i++, mover++) {
    if (mover->id == 0)
      continue;
    actor = Actor_GetById(mover->id);
    if (actor->x.fixed == 0 && actor->z.fixed == 0)
      continue;
    if (mover->phase == mover->riseAt) {
      Sound_Play(SE_CURSE_FALL);
      mover->speed = 4;
    }
    if (mover->phase == mover->fallAt) {
      Sound_Play(SOUND_FADE_OUT_EXIT_SE);
      mover->speed = -8;
    }
    if (mover->speed != 0) {
      mover->height += mover->speed;
      if (mover->height >= mover->high) {
        mover->height = mover->high;
        mover->speed = 0;
        mover->bob = 0;
      }
      if (mover->height <= mover->low) {
        mover->height = mover->low;
        mover->speed = 0;
        mover->bob = 0;
      }
    }
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      if (++mover->phase >= mover->period)
        mover->phase = 0;
    }
    cell = (FIELD_CELL(actor->x.fixed)) + ((FIELD_CELL(actor->z.fixed)) << 7);
    shapes[cell] = (u8)work->shape + i;
    *collision = ((actor->y.part[0].integer + mover->height) / 8) << 8;
    attributes = cells + cell * 4;
    if (mover->speed != 0)
      attributes[3] = 0;
    else {
      s32 bits = 128;
      bits |= attributes[3];
      attributes[3] = bits;
    }
    if (mover->height != 0) {
      s32 bits = 16;
      bits |= attributes[3];
      attributes[3] = bits;
    } else
      attributes[3] = 0;
    marker = mover->spout;
    if (marker != 0) {
      if (mover->height == 0) {
        marker->y.fixed = actor->y.fixed - toFIXED(16);
      } else {
        FIXED wave;
        mover->bob += TRIG_TURN / 16;
        wave = Iwram_MulFX(toFIXED(2), Trig_Sin(mover->bob)) - toFIXED(14);
        marker->y.fixed = actor->y.fixed + toFIXED(mover->height) + wave;
      }
      marker->x.fixed = actor->x.fixed;
      marker->z.fixed = actor->z.fixed;
    }
  }
}
#undef SET_LAYER

// Trial Road's movers' sprites, the setup of its gauge, a rising platform's
// entry, the trap table's setup, MagmaRockInside_CheckCrumblingStep, the
// placing of actors 8, 9 and 10, TrialRoad_StepOnTrap and a second copy of
// Effect_AdvanceMotionAtLeaderPriority, in ROM order.

/// Queues the Trial Road movers' sprites.
void TrialRoad_DrawFountains(void) {
  struct TrialRoadFountains *base = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *mover = base->fountains;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  FIXED rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  FIXED z;
  s32 sy;
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

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->height == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->height) {
      do {
        if (count > 79)
          continue;
        x = object->x.fixed - viewX;
        height = object->y.fixed + toFIXED(lift) - viewY;
        z = object->z.fixed - viewZ - viewY;
        x = FIXEDtoINT(x) - 8;
        sy = z - height;
        sy = (sy >> 16) - 16;
        depth = ((height + z) >> 16) + 58;
        if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
          continue;
        x &= 0x1ff;
        sy &= 0xff;
        cursor = entry;
        *cursor++ = 0;
        *cursor++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *cursor = (tile + frame) | OBJ_PRIORITY(2);
        Oam_AddSpriteAtDepth(entry, depth);
        entry += 3;
        count++;
      } while ((lift += 16) < mover->height);
    }
    if (count > 79)
      continue;
    x = object->x.fixed - viewX;
    height = object->y.fixed + toFIXED(mover->height) - viewY;
    z = object->z.fixed - viewZ - viewY;
    x = FIXEDtoINT(x) - 8;
    sy = z - height;
    bob = Iwram_MulFX(rise, Trig_Sin(mover->bob));
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    *cursor = (tile + frame - 32) | OBJ_PRIORITY(2);
    Oam_AddSpriteAtDepth(entry, depth);
    entry += 3;
    count++;
  }
}

/// Allocates the fountains' working buffer.
static __inline__ void *Pool_Alloc(s32 size) { return Heap_AllocEwram(size); }

/// Starts the fountains' work and their task.
void TrialRoad_StartFountains(s32 unused, s32 shape) {
  struct TrialRoadFountains *work = TRIAL_ROAD_FOUNTAINS;
  void *buffer;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords(work, 0x1c8);
  buffer = Pool_Alloc(0x800);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 0x800, buffer);
  Heap_Release(buffer);
  work->sprites = Heap_AllocEwram(0x3c0);
  work->shape = shape;
  Task_Add(TrialRoad_UpdateFountains, TASK_PASS_VBLANK + 13);
  Task_Add(TrialRoad_DrawFountains, TASK_PASS_VBLANK);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

/// Sets up one fountain on its spout actor, with its timer and height.
struct TrialRoadFountain *TrialRoad_AddFountain(s32 slot, s32 id, s32 timer,
                                                s32 low, s32 high) {
  struct TrialRoadFountains *work = TRIAL_ROAD_FOUNTAINS;
  struct TrialRoadFountain *fountain = &work->fountains[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *spout;

  Actor_GetById(id);
  Actor_SetSpriteFlags(object, 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    fountain->id = id;
    fountain->height = 0;
    fountain->phase = timer;
    fountain->period = 360;
    fountain->high = high;
    fountain->riseAt = 60;
    fountain->fallAt = 240;
    fountain->low = low;
    fountain->unusedAfterLow = 1;
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    fountain->speed = 0;
    fountain->bob = 0;
    fountain->unusedAfterSpeed = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  spout = ((struct Actor *)Import_ActorCreate(
      0x8000, object->x.fixed, object->y.fixed, object->z.fixed));
  spout->collisionFlags = 0;
  spout->motionFlags = 0;
  spout->priorityFlags |= 128;
  fountain->spout = spout;
  return fountain;
}

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.

// The trap table's setup: a trap whose second flag is set is put away; any
// other marks its cell, writes a shape for its height into the shape grid, and
// takes mode 0 when its first flag is set. Then the leader is lifted onto the
// ground if it stands below it.
#include "shared/pillars_restore_raised.h"

#include "shared/traps_check.h"

/// Magma Rock's crumbling pillar: the listed pillar shakes, crumbles and falls
/// away.
void TrialRoad_CrumblePillar(const void *data, s32 id) {
  const struct TrapEntry *entry = data;
  struct Actor *object = Actor_GetById(id);
  s32 flag;

  while (entry->actor != 0xffff) {
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
        MagmaRockInside_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(object, 2);
        TrialRoad_SetActorCellShape(-4, object->layer, id);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        Actor_SetPositionAndResetMotion(object, 0, 0, 0);
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

void MagmaRockInside_SetPillarMode3(s32 id) {
  Actor_GetById(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

void Actor_MoveToObject(s32 id, s32 owner);
// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
// it copy actor 9's visual attributes.
#include "shared/pillars_restore.h"

/// Moves the actor to the target's place and the target out of sight.
void TrialRoad_TakePlaceOf(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x.fixed, target->y.fixed,
                                  target->z.fixed);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

/// Magma Rock's trap floor: a listed pillar stepped on flashes the first time
/// and gives way under the leader the second.
void TrialRoad_StepOnTrap(const u16 *table) {
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
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  partner = Actor_GetById(ACTOR_MOAPA);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Actor_SelectAnimation(partner, 1);
      TrialRoad_TakePlaceOf(partner, trap);
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
      MagmaRockInside_SpawnRubble(0);
      TrialRoad_TakePlaceOf(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Actor_SelectAnimation(partner, 0);
        TrialRoad_TakePlaceOf(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        MagmaRockInside_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        MagmaRockInside_SetLeaderCellShape(-2, trap->layer);
        MagmaRockInside_SpawnRubble(1);
        wait(16);
        MagmaRockInside_SetLeaderCellShape(-2, trap->layer);
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
  test:
    if (*table != 0xffff)
      goto next;
  }
}

/// A second copy of Effect_AdvanceMotionAtLeaderPriority in this overlay:
/// FieldEffect_AdvanceMotion, then the leader's sprite priority for the effect.
void TrialRoad_DriftRubble(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX = effect->velocity.x;
  s32 velocityZ;
  struct MainRoutineWork *party;
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
  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;
  {
    effect->scaleX += effect->scaleRateX;
  }
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;

  party = &gMainRoutineWork;

  leader = Actor_GetById(party->leaderActorId);
  to = (struct EffectSpriteAttributes *)effect->sprite;
  from = (struct EffectSpriteAttributes *)leader->effect.sprite;
  source = from->bits;
  bits = to->bits;
  bits.priority = source.priority;
  to->bits = bits;
}

extern s32 gTrialRoad_RubbleScript[];

void MagmaRockInside_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      // Each piece starts 0 to 13 pixels lower than the leader.
      FIXED y = leader->y.fixed - ((Random_Below(14)) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, (i << 17) + leader->x.fixed - toFIXED(8),
          y - toFIXED(2), leader->z.fixed);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_RubbleScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i << 13), FIXED_ONE * 12 / 10);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = TrialRoad_DriftRubble;
    }
    i++;
  } while (i <= 7);
}

extern s32 gTrialRoad_DustScript[];

void MagmaRockInside_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)Import_ActorCreate(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript((struct Actor *)effect, gTrialRoad_DustScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Trig_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = TrialRoad_DriftRubble;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

/// Sets the shape of the cell under an actor (Actor_SetCellShape).
void TrialRoad_SetActorCellShape(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapGrid->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value =
      MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
  s32 height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed) >>
      19;

  height += level;

  shape += object->x.fixed / toFIXED(16) + object->z.fixed / toFIXED(16) * 128;
  *shape = MapCollision_FindOrAddTag(object->layer, value, height);
}

void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b) {
  TrialRoad_SetActorCellShape(a, b, gMainRoutineWork.leaderActorId);
}

extern u32 gTrialRoad_HexDigits[];

/// Writes value as eight hexadecimal digits and a terminating null.
UNUSED void String_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;
  const u8 *digits = gTrialRoad_HexDigits;

  buf += 8;
  *buf = 0;
  buf--;
  i = 7;
  do {
    *buf = digits[value & 15];
    value >>= 4;
    buf--;
    i--;
  } while (i >= 0);
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

// The Trial Road rival's per-frame update.

/// The most the rival turns in a frame: a sixteenth of a turn.
#define RIVAL_MAX_TURN (TRIG_TURN / 16)

/// Updates the Trial Road rival by its mode: RIVAL_MIRROR follows the leader's
/// position mirrored across the course; RIVAL_LINKED places the rival and its
/// partner from the linked player's record; RIVAL_REPLAY replays the recorded
/// path; RIVAL_RECORD records the actor's position.
void TrialRoad_MoveRival(void) {
  struct TrialRoadLinkWork *work =
      (struct TrialRoadLinkWork *)Ram_HeapSlots->trialRoadLink;
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;
  struct Actor *rival = Actor_GetById(state->id);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
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
    if (gMainRoutineWork.entrance & 1)
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
    s32 angle, link, ok, partner;
    s16 st;

    player = ((s32)GameFlag_GetByte(760)) ^ 1;
    signature = (s16 *)gLinkPeerSignatures[player];
    if (((gLinkStatus >> player) & 1) == 0)
      return;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    st = *signature++;
    link = st >> 8;
    ok = st & 0xff;
    if (GameFlag_Get(dz = 529) && link != 0) {
      state->mode = RIVAL_WAITING;
      Actor_SelectAnimation(rival, 1);
      if (work->originX < rival->x.fixed)
        x = work->originX + toFIXED(12);
      else
        x = work->originX - toFIXED(12);
      if (!GameFlag_Get(FLAG_SCENE_TEMP_12)) {
        z = work->originZ + toFIXED(16);
        rival->timer = work->rivalTimers[1];
      } else {
        z = work->originZ - toFIXED(16);
        rival->timer = work->rivalTimers[0];
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
      Actor_SelectAnimation(rival, ok);
    }
    partner = *signature++;
    x = *signature++;
    z = *signature++;
    angle = *signature++;
    ok = *signature++;
    if (partner == 0)
      return;
    rival = Actor_GetById(partner);
    if (rival == 0)
      return;
    rival->x.fixed = toFIXED(x);
    rival->facing = angle;
    rival->z.fixed = toFIXED(z);
    Actor_SelectAnimation(rival, ok);
  } else if (state->mode == RIVAL_REPLAY) {
    if (state->resource != work->resource) {
      Decompress_TaggedLz(Resource_GetTableEntry(state->resource), work->path);
      work->resource = state->resource;
    }
    x = work->path[state->point];
    z = work->path[state->point + 1];
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
    if (gMainRoutineWork.entrance & 1)
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
    work->path[state->point] = x;
    work->path[state->point + 1] = z;
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
      state->point += 2;
    if (state->point == 7198) {
      u16 next = state->point + 1;

      work->path[state->point] = 0;
      work->path[(s16)next] = 0;
      state->id = work->rivalId;
      state->point = 0;
      state->mode = RIVAL_REPLAY;
    }
  }
}

// next object the overlay links, and the overlay's data.

extern const u8 gTrialRoad_ProgressPicture[];

/// The picture's buffer: 576 bytes of EWRAM, the size VramCache_Load copies.
static inline void *TrialRoadLink_AllocPicture(void) {
  return Heap_AllocEwram(576);
}

#define MARKER(left, right, which, tileAdd)                                    \
  do {                                                                         \
    s32 lift;                                                                  \
    /* FAKEMATCH(pin): originZ in r6. */                                       \
    register s32 *originZ asm("r6");                                           \
    /* FAKEMATCH(pin): x in r9. */                                             \
    register s32 x asm("r9");                                                  \
    /* FAKEMATCH(pin): arg in r0. */                                           \
    register s32 arg asm("r0");                                                \
    s32 side = gMainRoutineWork.entrance & 1;                                  \
    if (side) {                                                                \
      x = (actor->x.fixed - work->originX) / toFIXED(16) left;                 \
      arg = which;                                                             \
    } else {                                                                   \
      x = (actor->x.fixed - work->originX) / toFIXED(16) right;                \
      arg = which;                                                             \
    }                                                                          \
    originZ = &work->originZ;                                                  \
    lift = TrialRoad_GetStage(arg);                                            \
    y = (actor->z.fixed - *originZ) / toFIXED(16) + work->scrollY -            \
        lift * 36 + 4;                                                         \
    /* Once-only: ends the block, so y + 4 is scheduled before x & 511. */     \
    do {                                                                       \
    } while (0);                                                               \
    x &= 511;                                                                  \
    y &= 255;                                                                  \
    *cursor++ = 0;                                                             \
    y |= x << 16;                                                              \
    y |= markerSize;                                                           \
    *cursor++ = y;                                                             \
    *cursor++ = (tile + (tileAdd)) | (OBJ_PALETTE(14) | OBJ_PRIORITY(1));      \
    Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);                            \
  } while (0)

/// The Trial Road link display's scheduled callback, which
/// TrialRoad_ShowProgress starts. Its fade state falls during events or while
/// flag 262 is set and otherwise rises to 2 (unke0 holds it at 2); rising from
/// 0 to 1 it loads the palette and picture again, and at 0 it frees its VRAM.
/// Otherwise it queues two rows of sprites, each up to four pairs 36 pixels
/// apart down from the scrolled offset, sliding in 6 pixels per fade step (the
/// right row mirrored), and on frames 5 to 15 of every 16 a marker over each of
/// the two watched actors, projected at 1/16 scale from their positions and the
/// course side.
void TrialRoad_DrawProgress(void) {
  struct TrialRoadLinkWork *work = Ram_HeapSlots->trialRoadLink;
  u32 *cursor = (u32 *)work;
  struct TrialRoadSprite *entry = (struct TrialRoadSprite *)work;
  s16 *vramEntry = &work->vramEntry;
  u32 row, i;
  s32 tile = gVramCacheEntries[*vramEntry].offset >> 5;
  u32 flip, word;
  s32 y;
  // One constant for its whole life: the markers' OAM size bits (attribute 1
  // size 1, 16 by 16), held in a register across both markers.
  u32 markerSize = OBJ_SIZE_16X16;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor;

  if (work->holdFade != 0) {
    work->fade = 2;
  } else if (GameFlag_Get(FLAG_MENU_OPEN) || event->psynergy != 0) {
    if (work->fade > 0)
      work->fade--;
  } else if (work->fade <= 1) {
    if (++work->fade == 1) {
      void *buffer;
      Dma_Set(gTrialRoad_ProgressPalette, (void *)&OBJ_PLTT_COLOR(14, 0),
              DMA_COPY_HALFWORDS(16), REG_DMA3);
      buffer = TrialRoadLink_AllocPicture();
      Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
      VramCache_Load(*vramEntry, 576, buffer);
      Heap_Release(buffer);
    }
  }
  if (work->fade == 0) {
    VramCache_ReleaseVram(work->vramEntry);
    return;
  }
  for (row = 0; row < 2; row++) {
    s32 x;
    y = (work->scrollY + 8) & 255;
    if (row == 0) {
      x = work->fade * 6 + work->scrollX - 12;
      x &= 511;
      flip = 0;
    } else {
      x = 236 - work->fade * 6 - work->scrollX;
      x &= 511;
      flip = OBJ_HFLIP;
    }
    for (i = 0; i < 4 && y < 160; i++) {
      *cursor++ = 0;
      *cursor++ = y | OBJ_X(x) | flip | OBJ_SIZE_16X32;
      *cursor++ = tile | (OBJ_PALETTE(14) | OBJ_PRIORITY(1));
      Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
      word = y + 32;
      *cursor++ = 0;
      *cursor++ = word | OBJ_X(x) | flip | OBJ_SIZE_16X8;
      *cursor++ = (tile + 8) | (OBJ_PALETTE(14) | OBJ_PRIORITY(1));
      Oam_AddSpriteAtDepth(entry++, OAM_DEPTH_FRONT);
      y += 36;
    }
  }
  if ((gFrameCount & 15) < 5)
    return;
  actor = Actor_Lookup(work->leaderId);
  if (actor != NULL) {
    MARKER(+work->fade * 6 + work->scrollX - 19,
           -work->fade * 6 - work->scrollX + 232, 0, 10);
  }
  actor = Actor_Lookup(work->rivalId);
  if (actor == NULL || actor->x.fixed == 0)
    return;
  {
    // XXX: this side's offset adds scrollX where the first marker's subtracts
    // it; the native does so, and a render of the course's far side would show
    // whether it is a slip.
    MARKER(+work->fade * 6 + work->scrollX + 189,
           -work->fade * 6 + work->scrollX + 23, 1, 14);
  }
}

/// Shows the race's progress for both sides.
void TrialRoad_ShowProgress(s32 leaderId, s32 rivalId) {
  struct TrialRoadLinkWork *link = HeapSlot_AllocEwram(
      HEAP_SLOT_TRIAL_ROAD_LINK, sizeof(struct TrialRoadLinkWork));
  void *buffer = Heap_AllocEwram(576);
  s32 zero;
  s32 owner;
  s16 *fade;
  s16 *holdFade;
  s32 *originX;
  s32 *originZ;
  s16 *vramEntry;

  link->leaderId = leaderId;
  link->rivalId = rivalId;
  owner = gMainRoutineWork.leaderActorId;
  originX = &link->originX;
  originZ = &link->originZ;
  fade = &link->fade;
  holdFade = &link->holdFade;
  vramEntry = &link->vramEntry;
  if (leaderId == owner) {
    *originX = toFIXED(160);
    *originZ = toFIXED(-1632);
  } else {
    *originX = toFIXED(112);
    *originZ = toFIXED(-1632);
  }
  Actor_GetById(leaderId);
  Actor_GetById(rivalId);
  zero = 0;
  *fade = zero;
  *holdFade = zero;
  Decompress_TaggedLz(gTrialRoad_ProgressPicture, buffer);
  *vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(*vramEntry, 576, buffer);
  Task_Add(TrialRoad_DrawProgress, TASK_PASS_VBLANK - 10);
  Heap_Release(buffer);
}

/// Starts Moapa's party on its recording, mirrored when the leader went right.
void TrialRoad_StartRival(s32 mirrored) {
  struct TrialRoadLinkWork *work = Ram_HeapSlots->trialRoadLink;
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    state->mirrored = mirrored;
    state->id = work->rivalId;
    state->still = 0;
  }
  Task_Add(TrialRoad_MoveRival, TASK_PASS_VBLANK + 5);
}

/// Plays Moapa's recording from the given resource.
void TrialRoad_PlayRivalRecording(s32 resource) {
  struct TrialRoadReplay *state = (struct TrialRoadReplay *)gSceneState;
  s32 zero = 0;

  ((struct TrialRoadLinkWork *)Ram_HeapSlots->trialRoadLink)->resource = zero;
  state->resource = resource;
  state->point = zero;
  state->mode = RIVAL_REPLAY;
  TrialRoad_MoveRival();
}

/// Whether Moapa's party is waiting.
s32 TrialRoad_RivalIsWaiting(void) {
  s32 mode = ((struct TrialRoadReplay *)gSceneState)->mode;

  return IsZero(mode ^ RIVAL_WAITING);
}

/// Does nothing.
void TrialRoad_DoNothing(void) {}

s32 gTrialRoad_SettleScript[] = {
    SC_CALL((s32)TrialRoad_SettleOnGround),
    SC_END,
};

s32 gTrialRoad_PillarTopScript[] = {
    SC_CALL((s32)TrialRoad_FollowPillar),
    SC_END,
};

s8 gTrialRoad_PoundPartners[4] ALIGNED(4) = {
    ACTOR_INNER_WEST_PILLAR, ACTOR_WEST_PILLAR, ACTOR_EAST_PILLAR,
    ACTOR_INNER_EAST_PILLAR};

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
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
    TrialRoad_SetUpBushPillarStage,
    TrialRoad_SetUpPoundStage,
    TrialRoad_SetUpCrumblingStage,
    TrialRoad_SetUpLogStage,
    TrialRoad_SetUpFountainStage,
    TrialRoad_SetUpSandStage,
};

u32 gTrialRoad_RivalRecordings[] = {
    450, 451, 452, 453, 454, 455,
    456, 457, 458, 459,
};

s16 gTrialRoad_StageMapCells[][2] = {
    {1, 48}, {15, 48}, {29, 48}, {1, 70},
    {15, 70}, {29, 70}, {1, 92}, {15, 92},
};

s8 gTrialRoad_DemoSceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

s8 gTrialRoad_SceneryStages[] = {
    0, 1, 2, 3, 4, 4, 4, 5,
    6, 7,
};

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
LATE_DATA s32 gTrialRoad_RubbleScript[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

LATE_DATA s32 gTrialRoad_DustScript[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};
// clang-format on

void TrialRoad_FightGasp(void);
void TrialRoad_OpenChests(void);
void TrialRoad_JumpForward(void);
void TrialRoad_BlowBushOffPoundPillar(void);
void TrialRoad_StandOnBlock(void);
void TrialRoad_OpenSummitGate(s32 sound);
void TrialRoad_StepPastChests(void);

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
    SCENE_EXIT(1, TrialRoadStages, 3),
    SCENE_EXIT(2, TrialRoadStages, 4),
    SCENE_EXIT(3, TrialRoadStages, 1),
    SCENE_EXIT(4, TrialRoadStages, 2),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_A, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_A),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_B, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_B),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_WEST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_WEST),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_TRIAL_ROAD_SUMMIT_EAST_A, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_STAGES_EAST),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_C, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_C),
    SCENE_EXIT(TRIAL_ROAD_STAGES_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS_D, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_E),
    SCENE_EXITS(TrialRoadCaveRock),
    SCENE_EXIT(1, TrialRoadCaveRock, 2),
    SCENE_EXIT(2, TrialRoadCaveRock, 1),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_SUMMIT, TrialRoadSummit, TRIAL_ROAD_SUMMIT_FROM_TRIAL_ROAD_CAVE_ROCK),
    SCENE_EXIT(TRIAL_ROAD_CAVE_ROCK_TO_TRIAL_ROAD_GASP, TrialRoadGasp, TRIAL_ROAD_GASP_FROM_TRIAL_ROAD_CAVE_ROCK),
    SCENE_EXITS(TrialRoadGasp),
    SCENE_EXIT(TRIAL_ROAD_GASP_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_GASP),
    SCENE_EXITS(TrialRoadSummit),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_WEST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_WEST_B),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_STAGES_EAST, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_TRIAL_ROAD_SUMMIT_EAST_B),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_TRIAL_ROAD_CAVE_ROCK, TrialRoadCaveRock, TRIAL_ROAD_CAVE_ROCK_FROM_TRIAL_ROAD_SUMMIT),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_INN_EAST_A, ShamanVillageInn, SHAMAN_VILLAGE_INN_FROM_TRIAL_ROAD_SUMMIT_EAST_A),
    SCENE_EXIT(TRIAL_ROAD_SUMMIT_TO_SHAMAN_VILLAGE_INN_EAST_B, ShamanVillageInn, SHAMAN_VILLAGE_INN_FROM_TRIAL_ROAD_SUMMIT_EAST_B),
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
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 248, 0, 400, FACING_EAST, TALK_FACING_KEEP, 0),
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

LAST_DATA struct ActorPlacement gTrialRoad_BushPillarStagePlacements[] = {
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
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 232, 0, 432, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 296, 0, 432, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 264, 0, 464, FACING_EAST, TALK_FACING_KEEP, 0),
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
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gTrialRoad_SettleScript, 312, 0, 336, FACING_EAST, TALK_FACING_KEEP, 0),
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

LAST_DATA s32 gTrialRoad_StagesPlacements[] = {
    (s32)gTrialRoad_PillarStagePlacements,
    (s32)gTrialRoad_TorchStagePlacements,
    (s32)gTrialRoad_BushPillarStagePlacements,
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
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 51, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), EVENT(TrialRoad_JumpAcrossGap)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 52, IF_FLAG_SET(FLAG_LEADER_ON_PILLARS), EVENT(TrialRoad_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, IF_FLAG_SET(FLAG_AREA_TEMP_07), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, IF_FLAG_SET(FLAG_AREA_TEMP_07), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, FLAG_AREA_TEMP_07, EVENT(TrialRoad_BlowBushOffLonePillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_TorchStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpMeltedPuddle)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockTorch)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT(TrialRoad_MeltIceBesideTorch)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, IF_FLAG_SET(FLAG_SCENE_TEMP_14), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, IF_FLAG_SET(FLAG_SCENE_TEMP_15), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, IF_FLAG_SET(FLAG_SCENE_TEMP_16), EVENT(TrialRoad_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, IF_FLAG_SET(FLAG_SCENE_TEMP_17), EVENT(TrialRoad_FreezePuddle)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_BushPillarStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT(TrialRoad_JumpForward)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, IF_FLAG_SET(FLAG_AREA_TEMP_27), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, IF_FLAG_SET(FLAG_AREA_TEMP_28), EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, IF_FLAG_SET(FLAG_AREA_TEMP_27), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(FLAG_AREA_TEMP_28), EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, FLAG_AREA_TEMP_27, EVENT(TrialRoad_BlowBushOffPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, FLAG_AREA_TEMP_28, EVENT(TrialRoad_BlowBushOffPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_PoundStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED), EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED), EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b, IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c, IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING, 50, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, 51, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, 52, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {STEP_TRIGGER_WALKING, 53, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {TRIGGER_STEP, 50, FLAG_AREA_TEMP_35, EVENT(TrialRoad_StepOnWestPillar)},
    {TRIGGER_STEP, 51, IF_FLAG_SET(FLAG_AREA_TEMP_35), EVENT(TrialRoad_StepOnInnerWestPillar)},
    {TRIGGER_STEP, 52, FLAG_AREA_TEMP_37, EVENT(TrialRoad_StepOnInnerEastPillar)},
    {TRIGGER_STEP, 53, IF_FLAG_SET(FLAG_AREA_TEMP_37), EVENT(TrialRoad_StepOnEastPillar)},
    {STEP_TRIGGER_WALKING, 60, CONDITION_ALWAYS, EVENT(TrialRoad_LandOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, IF_FLAG_SET(FLAG_AREA_TEMP_39), EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(TrialRoad_CheckPoundedPillarsCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, FLAG_AREA_TEMP_39, EVENT(TrialRoad_BlowBushOffPoundPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, FLAG_AREA_TEMP_35, EVENT(TrialRoad_PoundWestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 19, IF_FLAG_SET(FLAG_AREA_TEMP_35), EVENT(TrialRoad_PoundInnerWestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 20, FLAG_AREA_TEMP_37, EVENT(TrialRoad_PoundInnerEastPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 21, IF_FLAG_SET(FLAG_AREA_TEMP_37), EVENT(TrialRoad_PoundEastPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA s16 gTrialRoad_CrumblingPillars[] = {
    15, 834, 16, 836, -1, 0,
};
// clang-format on

LAST_DATA struct Trigger gTrialRoad_CrumblingStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS,
     EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS,
     EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
     EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS,
     EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS,
     EVENT(TrialRoad_CheckCrumblingPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS,
     EVENT(TrialRoad_CheckCrumblingPillars)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     FLAG_AREA_TEMP_43, EVENT(TrialRoad_CrumbleWestPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     FLAG_AREA_TEMP_45, EVENT(TrialRoad_CrumbleEastPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_LogStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS,
     EVENT(TrialRoad_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushUpperLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushUpperLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 51, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushLowerLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 51, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushLowerLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 51, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 52, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 52, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogSouthOrClimbDown)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS,
     EVENT(TrialRoad_PushEastLogSouth)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_FountainStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(TrialRoad_StandOnBlock)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     CONDITION_ALWAYS, EVENT(TrialRoad_UnblockMovedPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS,
     EVENT(TrialRoad_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50,
     FLAG_AREA_TEMP_63, EVENT(TrialRoad_UncoverWestSpout)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51,
     FLAG_AREA_TEMP_64, EVENT(TrialRoad_UncoverMiddleSpout)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 52,
     FLAG_AREA_TEMP_65, EVENT(TrialRoad_UncoverEastSpout)},
    {TRIGGER_TABLE_END},
};

LAST_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

LAST_DATA struct Trigger gTrialRoad_SandStageTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TrialRoad_TakeStairs)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 11, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 20, IF_FLAG_SET(FLAG_GIVE_UP_ASKED),
     EVENT(TrialRoad_ClearGiveUpAsked)},
    {TRIGGER_STEP, 21, FLAG_GIVE_UP_ASKED, EVENT(TrialRoad_OfferToGiveUp)},
    {TRIGGER_STEP, 30, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 31, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_STAGE_CHESTS_OPENED),
     EVENT(TrialRoad_StepPastChests)},
    {TRIGGER_STEP, 41, FLAG_STAGE_CHESTS_OPENED, EVENT(TrialRoad_OpenChests)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x409,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40a,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40b,
     IF_FLAG_SET(FLAG_INNER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {USE_ITEM_ON_ACTOR(ITEM_FILTER_EQUIPMENT), 0x40c,
     IF_FLAG_SET(FLAG_OUTER_CHEST_OPEN), EVENT(TrialRoad_PutItemInChest)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(TrialRoad_ExamineChest)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TrialRoad_DriftWithStream)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_StartSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(TrialRoad_EndSand)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, FLAG_AREA_TEMP_74,
     EVENT(TrialRoad_BlowBushOffVine)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_CaveRockTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, FLAG_CAVE_ROCK_LIFTED,
     EVENT(TrialRoad_LiftCaveRock)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_GaspTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(TrialRoad_CutMadPlantBushes)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TrialRoad_FightGasp)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_RevealPoolTiles)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TrialRoad_HidePoolTiles)},
    {TRIGGER_TABLE_END},
};

LAST_DATA struct Trigger gTrialRoad_SummitTriggers[] = {
    {TRIGGER_STEP, 33, FLAG_SCENE_TEMP_83, EVENT(TrialRoad_OpenSummitGate)},
    {TRIGGER_STEP, 40, IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
     EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_STEP, 41, IF_FLAG_SET(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT),
     EVENT(TrialRoad_ChallengeMoapa)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
     EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_PUSHED, 0, FLAG_TRIAL_ROAD_PEAK_PILLAR_PLACED,
     EVENT(TrialRoad_SetSummitPillar)},
    {TRIGGER_TABLE_END},
};

LAST_DATA s32 gTrialRoad_StagesTriggers[] = {
    (s32)gTrialRoad_PillarStageTriggers,     (s32)gTrialRoad_TorchStageTriggers,
    (s32)gTrialRoad_BushPillarStageTriggers, (s32)gTrialRoad_PoundStageTriggers,
    (s32)gTrialRoad_CrumblingStageTriggers,  (s32)gTrialRoad_LogStageTriggers,
    (s32)gTrialRoad_FountainStageTriggers,   (s32)gTrialRoad_SandStageTriggers,
};

LAST_DATA s32 gTrialRoad_PaletteCycleFrame = -1;

LAST_DATA s32 gTrialRoad_PaletteCycleRunning = 1;
