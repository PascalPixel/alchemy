/// @file
/// Imil Falls Cave, the little cave behind Imil's frozen falls, and Fuchin
/// Falls Cave, the grotto behind the waterfall at Fuchin Temple where Master
/// Nyunpa sends the party for its test.
///
/// Imil Falls Cave (`SceneId_ImilFallsCave`) is reached by sliding across
/// Imil's frozen river to the cave mouth at the foot of the falls. Inside,
/// Isaac walks up a strip of ice to Fever, a Mars Djinni, which joins the party
/// without a fight. Retreat does not work in the cave, and the doorway at the
/// bottom of the ice leads back out to Imil.
///
/// Fuchin Falls Cave is Nyunpa's test: he tells the party how to pass through
/// Mogall Forest only once they have come back from it with the Orb of Force,
/// and back at the temple he names its power Ki (fuchin_temple.c). The cave is
/// six rooms, crossed on golden logs that float on the cave's water. Isaac
/// steps onto a log and keeps walking the way it can go, and the log floats
/// with him on it to the far end of its run, glowing orange as it goes. From
/// some of a log's stops he jumps across a gap of water to a stump or to
/// another log. The logs are back where they started whenever the party comes
/// into the cave again.
///
/// In the order the player meets them:
///
/// - The first room (`SceneId_FuchinFallsCaveEntrance`), behind the falls. On
///   the south shore a tablet reads `The dragon's flame illuminates the path of
///   truth in darkness.` The crosswise log on the right floats north to the
///   ledge with a chest, which is a Mimic. The one on the left floats north to
///   the ledge below the room's north half, a pool of stumps that Isaac hops
///   across to the doorway in its north-east corner. An upright log at the
///   pool's west bank ferries Isaac to the east bank and back.
/// - The room of the Dragon's Eye (`SceneId_FuchinFallsCaveDragonsEye`), with
///   an upright log, two crosswise logs in a long channel and a small pool in
///   its south-west corner. Stairs down from its west ledge lead to a round
///   chamber where the Dragon's Eye lies on a little table.
/// - The dragon statue's room (`SceneId_FuchinFallsCaveDragonStatue`), up the
///   stairs at that room's north-west. Its northern half is dark and blue, its
///   floor a field of spikes with an eyeless stone dragon in the north-east
///   corner, and a tablet in its southern half reads `Rays of light give birth
///   to shadows, revealing the way.` Using the Dragon's Eye on the statue makes
///   the dragon breathe fire, lights the room and shows the shadow of a hidden
///   path across the spikes. The path leads to two doorways in the north wall.
/// - The room of the Arctic Blade (`SceneId_FuchinFallsCaveArcticBlade`), down
///   the stairs at the west of the dragon statue's room. In its north half a
///   crosswise log and an upright log cross a pool to the chest of the Arctic
///   Blade. In its south half an upright log carries Isaac east across a pool
///   to the way back to the first room's west bank.
/// - Zephyr's room (`SceneId_FuchinFallsCaveZephyr`), through the doorway at
///   the first room's east bank. Zephyr, a Jupiter Djinni, waits on a ledge on
///   its east side and fights when spoken to. Its doorway in the north leads to
///   the south end of the Dragon's Eye room.
/// - The room of Ki (`SceneId_FuchinFallsCaveOrbOfForce`), through the
///   doorways the hidden path leads to. A tablet reads `The secret of Ki shall
///   be revealed to the disciples of truth.`, and the chest beside it holds the
///   Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), which ends the test.
///
/// For the programmer: each log's place is an area temp flag named beside the
/// includes, and each room's rebuild routine puts its logs where their flags
/// say. It also rewrites the cells of every log's run, copied from spare cells
/// that each room's map keeps beside it: the cells under a log get the step
/// trigger that moves it, and the empty ends of its run become walls of water.
/// The logs' moves are Actor_PlacePairAtOffset, which carries the log and the
/// leader together. The darkness of the dragon statue's room is a palette
/// tint, and the hidden path is a run of cells copied over the spikes when the
/// eye is set (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET).

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_side.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "field/facing.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/ground_mask.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/main_routine.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s16 gIce_KeyHeadings[];
extern s16 gImilFuchinFalls_SlideHeadings[];
extern struct Actor *gImilFuchinFalls_DragonsEye;
extern struct SceneEntrance gImilFuchinFalls_NoEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[];
extern struct SceneEntrance gImilFuchinFalls_EntranceEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ZephyrRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonsEyeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_ArcticBladeRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[];
extern struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[];
extern u32 gImilFuchinFalls_Exits[];
extern struct ActorPlacement gImilFuchinFalls_NoActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ImilFallsCaveActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_EntranceActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ZephyrRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_DragonsEyeRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_ArcticBladeRoomActorPlacements[];
extern struct ActorPlacement gImilFuchinFalls_DragonRoomActorPlacements[];
extern struct Trigger gImilFuchinFalls_NoTriggers[];
extern struct Trigger gImilFuchinFalls_ImilFallsCaveTriggers[];
extern struct Trigger gImilFuchinFalls_EntranceTriggers[];
extern struct Trigger gImilFuchinFalls_ZephyrRoomTriggers[];
extern struct Trigger gImilFuchinFalls_DragonsEyeRoomTriggers[];
extern struct Trigger gImilFuchinFalls_ArcticBladeRoomTriggers[];
extern struct Trigger gImilFuchinFalls_KiRoomTriggers[];
extern struct Trigger gImilFuchinFalls_DragonRoomTriggers[];
extern s32 gImilFuchinFalls_ShortDustScript[];
extern s32 gImilFuchinFalls_MediumDustScript[];
extern s32 gImilFuchinFalls_LongDustScript[];

extern void *gHeapSlots[];

/// The placed actors, numbering each scene's placement rows from
/// ACTOR_FIRST_PLACED: Fever in Imil Falls Cave, the floating logs of Fuchin
/// Falls Cave's first room, Zephyr's room, the Dragon's Eye room and the Arctic
/// Blade's room, followed in Zephyr's room by Zephyr, and the one sprite the
/// dragon statue's room places.
enum ImilFuchinFallsActorId {
  /// A Mars Djinni, at the top of the ice in Imil Falls Cave.
  ACTOR_FEVER = ACTOR_FIRST_PLACED,
  /// The floating logs: three in the first room, Zephyr's room and the Arctic
  /// Blade's room, four in the Dragon's Eye room. The cell behaviours below say
  /// which log is which in each room.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_LOG,
  ACTOR_THIRD_LOG,
  ACTOR_FOURTH_LOG,
  /// Zephyr, a Jupiter Djinni, after the three logs of its room.
  ACTOR_ZEPHYR = ACTOR_FIRST_PLACED + 3,
  /// A crate in the dragon statue's room, beside the statue at cell (19, 8),
  /// which the player never sees: on entry ImilFuchinFalls_PrepareRoom stills
  /// it at floor height and draws it hidden, as
  /// FuchinFallsCave_CreateHiddenActor draws the two in the Dragon's Eye room.
  ACTOR_INVISIBLE_DRAGON_ROOM_CRATE = ACTOR_FIRST_PLACED
};

enum {
  /// Three times normal animation speed.
  IMIL_SLIDE_ANIMATION_STEP = ANIM_STEP_NORMAL * 3
};

/// The cells the rooms' step and search triggers answer, by the cell behaviour
/// their map gives them. A log's cells are the three under it at one of its
/// stops; the rebuild routines copy them in where the log lies.
enum {
  /// Imil Falls Cave: the strip of ice from the doorway north to Fever, cells
  /// (16, 14) to (16, 18).
  CELL_BEHAVIOR_IMIL_FALLS_CAVE_ICE = 10,

  /// First room: the tablet on the south shore, cell (15, 35).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_TABLET = 1,
  /// First room: the upright log at the pool's west bank, cells (8, 30) to
  /// (8, 32).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST = 11,
  /// First room: the upright log at the pool's east bank, cells (22, 30) to
  /// (22, 32).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST = 12,
  /// First room: the left crosswise log below the ledge on the north side,
  /// cells (11, 29) to (13, 29).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH = 13,
  /// First room: the left crosswise log at the south shore, cells (11, 33) to
  /// (13, 33).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH = 14,
  /// First room: the right crosswise log below the ledge with the chest, cells
  /// (17, 29) to (19, 29).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH = 15,
  /// First room: the right crosswise log at the south shore, cells (17, 33) to
  /// (19, 33).
  CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH = 16,

  /// Zephyr's room: the upright log where it starts, at the west end of the
  /// pool, cells (13, 19) to (13, 21).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START = 11,
  /// Zephyr's room: the upright log at the east end, below Zephyr's ledge,
  /// cells (21, 19) to (21, 21).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END = 12,
  /// Zephyr's room: the crosswise log at the foot of the sandy path down from
  /// the spiked floor, cells (14, 17) to (16, 17).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH = 13,
  /// Zephyr's room: the crosswise log at the pool's south shore, cells (14, 21)
  /// to (16, 21).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH = 14,
  /// Zephyr's room: the crosswise log at the north end of the long channel,
  /// beside the spiked floor, cells (18, 8) to (20, 8).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH = 15,
  /// Zephyr's room: the crosswise log at the south end of the long channel,
  /// cells (18, 25) to (20, 25).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH = 16,
  /// Zephyr's room: the upright log pushed against the west wall, below the
  /// stump, cells (12, 19) to (12, 21).
  CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END = 17,

  /// The Dragon's Eye room: the upright log at the pool's east edge, cells
  /// (20, 24) to (20, 26).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END = 10,
  /// The Dragon's Eye room: the upright log in the middle of the pool, below
  /// the stump, cells (17, 24) to (17, 26).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE = 11,
  /// The Dragon's Eye room: the upright log at the head of the long channel,
  /// cells (14, 24) to (14, 26).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END = 12,
  /// The Dragon's Eye room: the upright log in the small south-west pool, at
  /// its west side, cells (13, 34) to (13, 36).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST = 13,
  /// The Dragon's Eye room: the same log at the pool's east side, cells
  /// (15, 34) to (15, 36).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST = 14,
  /// The Dragon's Eye room: the upper crosswise log at the foot of the
  /// channel, row 30.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END = 15,
  /// The upper crosswise log at the head of the channel, below the north
  /// shore, row 22.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END = 16,
  /// The upper crosswise log one cell down the channel, row 23.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP = 17,
  /// The upper crosswise log halfway down the channel, row 26.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE = 18,
  /// The upper crosswise log at row 27, a stop no move gives it.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP = 19,
  /// The lower crosswise log at the foot of the channel, row 31.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END = 20,
  /// The lower crosswise log at the head of the channel, row 23.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END = 21,
  /// The lower crosswise log at row 24.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP = 22,
  /// The lower crosswise log at row 27, beside the stump below the west ledge.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE = 23,
  /// The lower crosswise log at row 28, a stop no move gives it.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP = 24,
  /// The Dragon's Eye room: the stump in the pool below the north shore, cell
  /// (17, 22). The north shore above the channel, cells (14, 21) to (16, 21),
  /// takes it too while the upper crosswise log lies at its upper stop.
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP = 25,
  /// The Dragon's Eye room's chamber: the little table, cell (15, 9).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_TABLE = 30,

  /// The Arctic Blade's room: the crosswise log at the pool's north shore,
  /// cells (19, 10) to (21, 10).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH = 11,
  /// The same log at the south shore, cells (19, 17) to (21, 17).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH = 12,
  /// The upright log at the pool's east shore, cells (22, 15) to (22, 17).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST = 13,
  /// The same log at the west shore, below the way up to the chest, cells
  /// (14, 15) to (14, 17).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST = 14,
  /// The south half's upright log at the west end of its pool, cells (7, 45)
  /// to (7, 47).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST = 15,
  /// The same log at the pool's east end, cells (17, 45) to (17, 47).
  CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST = 16,

  /// The room of Ki: the tablet beside the chest, cell (16, 5).
  CELL_BEHAVIOR_FUCHIN_FALLS_KI_ROOM_TABLET = 7,

  /// The dragon statue's room: the row where the middle passage from the
  /// south opens into the dark half, cells (14, 17) to (16, 17).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_NORTH_OF_MIDDLE_PASSAGE = 15,
  /// The dragon statue's room: the top of the middle passage, cells (14, 18)
  /// to (16, 18).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_MIDDLE_PASSAGE = 16,
  /// The dragon statue's room: the tablet in the south half, cell (15, 29).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_TABLET = 19,
  /// The dragon statue's room: the steps below the statue, cells (20, 14) to
  /// (22, 14).
  CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE = 20
};

static void FuchinFallsCave_RebuildEntranceLogs(void);
static void FuchinFallsCave_RebuildZephyrRoomLogs(void);
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void);
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void);
static void Imil_WalkTowardFever(void);
static void ImilFuchinFalls_PrepareRoom(void);
static void ImilFuchinFalls_RebuildLogRoom(void);
static void Scene_HopLeaderBy(s32 dx, s32 dz);
static void ImilFuchinFalls_PlaceDragonsEye(void);
static void Imil_SlideActorAcrossIce(void);

s32 gImilFuchinFalls_TrackLeader;

/// EffectOptions cut after its scales: the caller's frame holds only these 24
/// bytes.
struct CaveDustOptions {
  s32 priority;
  s32 palette;
  s32 startScaleX;
  s32 startScaleY;
  s32 targetScaleX;
  s32 targetScaleY;
};

STATIC_ASSERT(offsetof(struct CaveDustOptions, priority) ==
              offsetof(struct EffectOptions, priority));
STATIC_ASSERT(offsetof(struct CaveDustOptions, palette) ==
              offsetof(struct EffectOptions, palette));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleX) ==
              offsetof(struct EffectOptions, startScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleY) ==
              offsetof(struct EffectOptions, startScaleY));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleX) ==
              offsetof(struct EffectOptions, targetScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleY) ==
              offsetof(struct EffectOptions, targetScaleY));

/// Talking to Fever, the Mars Djinni at the top of the ice in Imil Falls Cave,
/// makes it join the party without a fight.
static void Imil_FeverJoins(void) {
  Djinn_JoinParty(ACTOR_FEVER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
}

/// Talking to Zephyr, the Jupiter Djinni on the ledge on the east side of its
/// room in Fuchin Falls Cave, starts a battle with it.
static void FuchinFallsCave_FightZephyr(void) {
  Field_RequestDjinnBattle(ACTOR_ZEPHYR, DJINNI_ZEPHYR);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_IMIL_FALLS_CAVE) {
    return gImilFuchinFalls_ImilFallsCaveEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
    return gImilFuchinFalls_EntranceEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
    return gImilFuchinFalls_ZephyrRoomEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
    return gImilFuchinFalls_DragonsEyeRoomEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
    return gImilFuchinFalls_ArcticBladeRoomEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ORB_OF_FORCE) {
    return gImilFuchinFalls_KiRoomEntrances;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
    return gImilFuchinFalls_DragonRoomEntrances;
  }
  return gImilFuchinFalls_NoEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Returns the exit table every scene of the overlay shares.
const u32 *Scene_GetExits(void) { return gImilFuchinFalls_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_IMIL_FALLS_CAVE) {
    return gImilFuchinFalls_ImilFallsCaveActorPlacements;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
    return gImilFuchinFalls_EntranceActorPlacements;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
    return gImilFuchinFalls_ZephyrRoomActorPlacements;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
    return gImilFuchinFalls_DragonsEyeRoomActorPlacements;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
    return gImilFuchinFalls_ArcticBladeRoomActorPlacements;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
    return gImilFuchinFalls_DragonRoomActorPlacements;
  }
  return gImilFuchinFalls_NoActorPlacements;
}

#include "shared/actor_pair_move.c"

/// The first room's upright log lies at the pool's east bank.
#define FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST FLAG_AREA_TEMP_01
/// The first room's left crosswise log lies below the ledge on the north side.
#define FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH FLAG_AREA_TEMP_02
/// The first room's right crosswise log lies below the ledge with the chest.
#define FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH FLAG_AREA_TEMP_03
/// Zephyr's room's upright log lies at the east end of the pool, below Zephyr's
/// ledge.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END FLAG_AREA_TEMP_04
/// Zephyr's room's upright log lies against the west wall, a cell west of where
/// it starts, unless it has since been taken to the east end.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_05
/// Zephyr's room's crosswise log lies at the pool's south shore.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH FLAG_AREA_TEMP_06
/// The crosswise log in Zephyr's room's long channel lies at its north end.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH FLAG_AREA_TEMP_07
/// The Dragon's Eye room's upright log lies at the head of the long channel.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_08
/// The Dragon's Eye room's upright log lies in the middle of the pool, below
/// the stump, where a crosswise log stopped it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE FLAG_AREA_TEMP_09
/// The upright log in the Dragon's Eye room's small south-west pool lies at the
/// pool's west side.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_0A
/// The Dragon's Eye room's upper crosswise log lies at the head of the channel.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END FLAG_AREA_TEMP_0B
/// The upper crosswise log lies a cell down the channel from its head.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP                \
  FLAG_AREA_TEMP_0C
/// The upper crosswise log lies halfway down the channel.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE FLAG_AREA_TEMP_0D
/// The upper crosswise log lies a cell below halfway. No move sets it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP                \
  FLAG_AREA_TEMP_0E
/// The Dragon's Eye room's lower crosswise log lies a cell below the channel's
/// head, as far up as it goes.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END                \
  FLAG_AREA_TEMP_0F
/// The lower crosswise log lies two cells below the channel's head.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP               \
  FLAG_AREA_TEMP_10
/// The lower crosswise log lies halfway down the channel, beside the stump.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE FLAG_AREA_TEMP_11
/// The lower crosswise log lies a cell below halfway. No move sets it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP               \
  FLAG_AREA_TEMP_12
/// The Arctic Blade's room's crosswise log lies at the pool's south shore.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH FLAG_AREA_TEMP_13
/// The Arctic Blade's room's upright log lies at the pool's west shore.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_14
/// The upright log in the south half of the Arctic Blade's room lies at the
/// pool's east side.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST FLAG_AREA_TEMP_15

/// Ends a log's move by fading out the sound of the moving water, then puts
/// its room's logs and their cells to rights.
static inline void FuchinFallsCave_FinishLogMove(void (*rebuildLogs)(void)) {
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  wait(2);
  rebuildLogs();
}

/// In the first room, Isaac steps onto the upright log at the pool's west bank
/// (cell (8, 31)) and walks on east, and the log carries him across the pool to
/// the east bank, by the doorway to Zephyr's room.
static void FuchinFallsCave_Entrance_MoveFirstLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In the first room, Isaac walks west on the upright log at the pool's east
/// bank (cell (22, 31)), and it carries him back across the pool to the west
/// bank, by the doorway from the Arctic Blade's room.
static void FuchinFallsCave_Entrance_MoveFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -112, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -112, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In the first room, Isaac walks south on the left crosswise log below the
/// ledge on the north side (cell (12, 29)), and it floats him back to the south
/// shore, west of the tablet.
static void FuchinFallsCave_Entrance_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In the first room, Isaac steps onto the left crosswise log at the south
/// shore (cell (12, 33)) and walks on north, and it floats him to the ledge on
/// the north side, where a passage leads up to the room's north half.
static void FuchinFallsCave_Entrance_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In the first room, Isaac walks south on the right crosswise log below the
/// ledge with the chest (cell (18, 29)), and it floats him back to the south
/// shore, east of the tablet.
static void FuchinFallsCave_Entrance_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In the first room, Isaac steps onto the right crosswise log at the south
/// shore (cell (18, 33)) and walks on north, and it floats him to the ledge
/// with the chest, which holds a Mimic.
static void FuchinFallsCave_Entrance_MoveThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Zephyr's room, Isaac walks east on the upright log where it starts, at
/// the west end of the pool (cell (13, 20)), and it carries him the length of
/// the pool to below Zephyr's ledge.
///
/// It goes only while the crosswise log is not at the south shore, where it
/// would lie across the way.
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac walks west on the upright log below Zephyr's ledge
/// (cell (21, 20)), and it carries him back along the pool to the west wall,
/// below the stump.
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac walks west on the upright log where it starts
/// (cell (13, 20)), and it bumps the last little way west against the wall,
/// right below the stump.
///
/// From there Isaac can hop north onto the stump, and a hop west from the
/// crosswise log at the south shore lands on the upright log.
static void FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -14, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac walks east on the upright log against the west
/// wall (cell (12, 20)), and it carries him the length of the pool to below
/// Zephyr's ledge.
///
/// While the crosswise log lies at the south shore, across the way, the upright
/// log only goes back the one cell to where it started.
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogEast(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 16, 0);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);

    GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildZephyrRoomLogs);
}

/// In Zephyr's room, Isaac steps onto the crosswise log at the foot of the
/// sandy path down from the spiked floor (cell (15, 17)) and walks on south,
/// and it floats him across the pool to the south shore.
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac walks north on the crosswise log at the pool's
/// south shore (cell (15, 21)), and it floats him back to the foot of the sandy
/// path down from the spiked floor.
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac steps onto the crosswise log at the north end of
/// the long channel, beside the spiked floor (cell (19, 8)), and walks on
/// south, and it floats him all the way down the channel, past Zephyr's ledge,
/// to its south end.
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 144);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Zephyr's room, Isaac walks north on the crosswise log at the south end
/// of the long channel (cell (19, 25)), and it floats him all the way up the
/// channel, past Zephyr's ledge, to the spiked floor below the north doorway.
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -96);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -96);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In the Dragon's Eye room, Isaac steps onto the upright log at the pool's
/// east edge (cell (20, 25)) and walks on west, and it carries him across the
/// pool to the head of the long channel.
///
/// When a crosswise log lies in the channel across its way (the lower one a
/// cell from the channel's head, or the upper one halfway down), it stops in
/// the middle of the pool, below the stump, instead.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
          0 ||
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -96, 0);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks east on the upright log in the middle
/// of the pool, below the stump (cell (17, 25)), and it carries him back to the
/// pool's east edge.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks west on the upright log in the middle
/// of the pool (cell (17, 25)), and it carries him on to the head of the long
/// channel.
///
/// While a crosswise log still lies across the channel there (the lower one a
/// cell from the head, or the upper one halfway down), nothing happens.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear(void) {
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
      0) {
    return;
  }
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    return;
  }

  Sound_Play(SE_WATER_MOVE);

  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);

  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks east on the upright log at the head of
/// the long channel (cell (14, 25)), and it carries him back across the pool to
/// its east edge.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 96, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac steps onto the upright log in the small pool
/// in the south-west corner (cell (15, 35)) and walks on west, and it floats
/// him across to the pool's west side, by the doorway to the first room.
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac walks east on the upright log at the west
/// side of the small south-west pool (cell (13, 35)), and it floats him back
/// across to the pool's east side.
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

// XXX (no evidence): whether the log was meant to stop below the upright log
// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, which no move
// sets); the move here ends between the two stops.
/// In the Dragon's Eye room, Isaac steps onto the upper of the two crosswise
/// logs at the foot of the long channel (cell (15, 30)) and walks on north, and
/// it floats him up the channel to its head, below the north shore.
///
/// While the upright log lies at the head of the channel, the crosswise log
/// stops against it, then is put back at its stop a cell from the head, beyond
/// the upright log, and Isaac is left standing on the water where it stopped.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -128);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks south on the upper crosswise log at
/// the head of the long channel (cell (15, 22)), and it floats him back down
/// the channel as far as it can go.
///
/// That is one cell when the upright log or the lower crosswise log lies just
/// below, halfway when the lower crosswise log lies halfway down, and otherwise
/// all the way to the foot of the channel.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks north on the upper crosswise log a
/// cell down the long channel (cell (15, 23)), and it floats him the last cell
/// up to the head of the channel.
static void FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac walks south on the upper crosswise log a
/// cell down the long channel (cell (15, 23)), and it floats him back down: to
/// halfway when the lower crosswise log lies halfway down, else all the way to
/// the foot of the channel.
///
/// It does not go while the upright log lies across the channel below it. With
/// the lower crosswise log right below it, the log floats two cells and is put
/// back where it was, and Isaac is left on the lower log.
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop(void) {
  Sound_Play(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 48);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);

    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 32);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 112);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks north on the upper crosswise log
/// halfway down the long channel (cell (15, 26)), and it floats him up to the
/// head of the channel.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac walks south on the upper crosswise log
/// halfway down the long channel (cell (15, 26)), and it floats him back to the
/// foot of the channel. It does not go while the lower crosswise log lies a
/// cell from the channel's head.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: walking north on the upper crosswise log at row 27 of the Dragon's
/// Eye room's long channel (cell (15, 27)) would float it to the head of the
/// channel.
///
/// The log never stops there: only
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP puts it there, and
/// no move sets that flag.
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: walking south on the upper crosswise log at row 27 of the Dragon's
/// Eye room's long channel (cell (15, 27)) would float it back to the foot of
/// the channel. The log never stops there, as
/// FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop says.
///
/// The same routine as
/// FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle under another
/// name: the room's step table holds both.
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac steps onto the lower of the two crosswise
/// logs at the foot of the long channel (cell (15, 31)) and walks on north, and
/// it floats him up the channel until something stops it.
///
/// It stops a cell below the channel's head, below the upper crosswise log
/// there; a cell lower when the upper log lies a cell down; and halfway, beside
/// the stump below the west ledge, when the upright log lies across the channel
/// or the upper crosswise log lies halfway down. From there Isaac can hop
/// west onto the stump and reach the stairs down to the Dragon's Eye.
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
          0 ||
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -112);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -128);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Forgets the stops of the Dragon's Eye room's lower crosswise log, which puts
/// it back at the foot of the long channel.
static void FuchinFallsCave_ClearFourthLogStops(void) {
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
}

/// In the Dragon's Eye room, Isaac walks south on the lower crosswise log a
/// cell below the head of the long channel (cell (15, 23)), and it floats him
/// all the way back to the foot of the channel. It does not go while the
/// upright log lies across the channel below it.
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 128);
  FuchinFallsCave_ClearFourthLogStops();
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In the Dragon's Eye room, Isaac walks north on the lower crosswise log two
/// cells below the head of the long channel (cell (15, 24)), and it floats him
/// one cell further up.
static void FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac walks south on the lower crosswise log two
/// cells below the head of the long channel (cell (15, 24)), and it floats him
/// back to the foot of the channel.
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Dragon's Eye room, Isaac walks south on the lower crosswise log
/// halfway down the long channel, beside the stump (cell (15, 27)), and it
/// floats him back to the foot of the channel.
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

// XXX (no evidence): whether walking north was meant to float the log north,
// as the upper log's matching stop does; the log never stops here, so the game
// never shows which.
/// Unused: walking north on the lower crosswise log at row 28 of the Dragon's
/// Eye room's long channel (cell (15, 28)) would float it south instead, five
/// cells, past the foot of the channel.
///
/// The log never stops there: only
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP puts it there, and
/// no move sets that flag. With the flag set by hand, walking north on the log
/// carries Isaac south past the channel's foot, and the rebuild puts the log
/// back at the foot.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: walking south on the lower crosswise log at row 28 of the Dragon's
/// Eye room's long channel (cell (15, 28)) would float it back to the foot of
/// the channel. The log never stops there, as
/// FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop says.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 48);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In the Arctic Blade's room, Isaac steps onto the crosswise log at the pool's
/// north shore, below the stairs (cell (20, 10)), and walks on south, and it
/// floats him across the pool to the south shore, where the way leads on to
/// the room's south half.
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In the Arctic Blade's room, Isaac walks north on the crosswise log at the
/// pool's south shore (cell (20, 17)), and it floats him back across to the
/// north shore, below the stairs.
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, -112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In the Arctic Blade's room, Isaac steps onto the upright log at the pool's
/// east shore (cell (22, 16)) and walks on west, and it carries him across the
/// pool to the west shore, below the way up to the chest of the Arctic Blade.
///
/// It goes only while the crosswise log is at the north shore; at the south
/// shore it lies across the way.
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In the Arctic Blade's room, Isaac walks east on the upright log at the
/// pool's west shore (cell (14, 16)), and it carries him back across to the
/// east shore.
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In the south half of the Arctic Blade's room, Isaac steps onto the upright
/// log at the west end of the pool (cell (7, 46)) and walks on east, and it
/// carries him across to the east side, where the way leads on to the first
/// room's west bank.
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In the south half of the Arctic Blade's room, Isaac walks west on the
/// upright log at the pool's east side (cell (17, 46)), and it carries him back
/// across to the west end.
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, -160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Stepping onto a room's first log makes it dip under Isaac's step and then
/// bob on the water: the upright log of the first room, of Zephyr's room and of
/// the Dragon's Eye room, and the crosswise log in the north of the Arctic
/// Blade's room.
static void FuchinFallsCave_SettleFirstLog(void) {
  Actor_SetAnimation(ACTOR_FIRST_LOG, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimation(ACTOR_FIRST_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/// Stepping onto a room's second log makes it dip under Isaac's step and then
/// bob on the water: the left crosswise log of the first room, the crosswise
/// log at the middle of Zephyr's room, the upright log in the Dragon's Eye
/// room's small south-west pool, and the upright log in the north of the
/// Arctic Blade's room.
static void FuchinFallsCave_SettleSecondLog(void) {
  Actor_SetAnimation(ACTOR_SECOND_LOG, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimation(ACTOR_SECOND_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/// Stepping onto a room's third log makes it dip under Isaac's step and then
/// bob on the water: the right crosswise log of the first room, the crosswise
/// log in Zephyr's room's long channel, the upper crosswise log in the Dragon's
/// Eye room's channel, and the upright log in the south of the Arctic Blade's
/// room.
static void FuchinFallsCave_SettleThirdLog(void) {
  Actor_SetAnimation(ACTOR_THIRD_LOG, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimation(ACTOR_THIRD_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/// Stepping onto the lower crosswise log in the Dragon's Eye room's long
/// channel makes it dip under Isaac's step and then bob on the water.
static void FuchinFallsCave_SettleFourthLog(void) {
  Actor_SetAnimation(ACTOR_FOURTH_LOG, FLOATING_LOG_ANIMATION_FLOAT);
  Actor_SetAnimation(ACTOR_FOURTH_LOG, FLOATING_LOG_ANIMATION_SETTLE);
}

/// Creates an actor that stands still and is drawn hidden, so that the player
/// never sees it.
static struct Actor *FuchinFallsCave_CreateHiddenActor(s32 x, s32 y, s32 z,
                                                       s32 resourceId) {
  struct Actor *actor = FieldActor_Create(resourceId, x, y, z);

  if (actor != NULL) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_IMIL_FALLS_CAVE) {
    return gImilFuchinFalls_ImilFallsCaveTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
    return gImilFuchinFalls_EntranceTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
    return gImilFuchinFalls_ZephyrRoomTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
    return gImilFuchinFalls_DragonsEyeRoomTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
    return gImilFuchinFalls_ArcticBladeRoomTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_ORB_OF_FORCE) {
    return gImilFuchinFalls_KiRoomTriggers;
  }
  if (sceneId == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
    return gImilFuchinFalls_DragonRoomTriggers;
  }
  return gImilFuchinFalls_NoTriggers;
}

/// Isaac walks out of the doorway at the bottom of Imil Falls Cave (cell
/// (16, 19)) and comes out at the foot of Imil's frozen falls.
static void Imil_LeaveFallsCave(void) {
  Event_Start();
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_SOUTH;
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(IMIL_FALLS_CAVE_TO_IMIL);
}

/// Isaac steps onto the strip of ice between the doorway and Fever in Imil
/// Falls Cave (cells (16, 14) to (16, 18)) and slides along it the way the pad
/// points, until a wall stops him or he slides off the ice. Sliding south takes
/// him right out of the doorway.
static void Imil_MoveAcrossIce(void) { Imil_SlideActorAcrossIce(); }

/// Puts a log at a map cell, drawn under the leader's feet.
static void FuchinFallsCave_PlaceLogAtCell(s32 id, s32 column, s32 row) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    Actor_SetSpritePriority(id, 3);
    actor->layer = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = toFIXED(column * 16) + toFIXED(8);
    actor->z.fixed = toFIXED(row * 16) + toFIXED(8);
  }
}

/// Puts the logs of Fuchin Falls Cave's first room where their flags say, and
/// rewrites the cells of their runs to match from the spare copy of the pool
/// kept below the room.
static void FuchinFallsCave_RebuildEntranceLogs(void) {
  MapGrid_CopyCellAttributes(8, 42, 15, 5, 8, 29);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 22, 31);
    MapGrid_CopyCellAttributes(9, 30, 1, 3, 8, 30);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 8, 31);
    MapGrid_CopyCellAttributes(9, 30, 1, 3, 22, 30);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 29);
    MapGrid_CopyCellAttributes(14, 33, 3, 1, 11, 33);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 33);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 11, 29);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 29);
    MapGrid_CopyCellAttributes(14, 33, 3, 1, 17, 33);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 33);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 17, 29);
  }
}

/// Puts the logs of Zephyr's room where their flags say, and rewrites the
/// cells of their runs to match.
static void FuchinFallsCave_RebuildZephyrRoomLogs(void) {
  MapGrid_CopyCellAttributes(0, 28, 10, 18, 12, 8);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 13, 20);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 21, 19);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 12, 20);
    MapGrid_CopyCellAttributes(5, 19, 1, 3, 12, 19);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
    if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
      FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
      MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
      MapGrid_CopyCellAttributes(20, 19, 1, 3, 12, 19);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 21);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 14, 17);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 17);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 14, 21);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 8);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 18, 25);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 25);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 18, 8);
  }
}

/// Puts the four logs of the Dragon's Eye room where their flags say, and
/// rewrites the cells of their runs to match.
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void) {
  MapGrid_CopyCellAttributes(12, 3, 9, 16, 12, 21);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 14, 25);
    MapGrid_CopyCellAttributes(16, 24, 1, 3, 20, 24);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 17, 25);
    MapGrid_CopyCellAttributes(18, 24, 1, 3, 20, 24);
    MapGrid_CopyCellAttributes(18, 24, 1, 3, 14, 24);
    MapGrid_CopyCellAttributes(8, 41, 1, 3, 17, 24);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 25);
    MapGrid_CopyCellAttributes(16, 24, 1, 3, 14, 24);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 13, 35);
    MapGrid_CopyCellAttributes(14, 34, 1, 3, 15, 34);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 35);
    MapGrid_CopyCellAttributes(14, 34, 1, 3, 13, 34);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 22);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(5, 41, 3, 1, 14, 22);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 23);
    MapGrid_CopyCellAttributes(5, 42, 3, 1, 14, 23);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(10, 44, 3, 1, 14, 21);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 26);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 22);
    MapGrid_CopyCellAttributes(5, 43, 3, 1, 14, 26);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP) !=
             0) {
    // Unused: no move sets this flag, so the log never lies here.
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 27);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 22);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(5, 44, 3, 1, 14, 27);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 30);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 23);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 40, 3, 1, 14, 23);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 24);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 41, 3, 1, 14, 24);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 27);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 42, 3, 1, 14, 27);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP) !=
             0) {
    // Unused: no move sets
    // FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, so the log
    // never lies here and the room never gets the cells of
    // CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP.
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 28);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 43, 3, 1, 14, 28);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 31);
  }
}

/// Puts the logs of the Arctic Blade's room, in both of its halves, where their
/// flags say, and rewrites the cells of their runs to match.
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void) {
  if (gMainRoutineWork.entrance ==
          FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
      gMainRoutineWork.entrance ==
          FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
    MapGrid_CopyCellAttributes(22, 20, 9, 8, 14, 10);
  } else {
    MapGrid_CopyCellAttributes(20, 45, 11, 4, 7, 45);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 17);
    MapGrid_CopyCellAttributes(19, 11, 3, 1, 19, 10);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 10);
    MapGrid_CopyCellAttributes(19, 11, 3, 1, 19, 17);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 14, 16);
    MapGrid_CopyCellAttributes(16, 15, 1, 3, 22, 15);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 22, 16);
    MapGrid_CopyCellAttributes(16, 15, 1, 3, 14, 15);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 17, 46);
    MapGrid_CopyCellAttributes(15, 15, 1, 3, 7, 45);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 7, 46);
    MapGrid_CopyCellAttributes(15, 15, 1, 3, 17, 45);
  }
}

/// Nothing visible happens when Isaac walks north out of the middle passage
/// into the dragon statue's dark room (cells (14, 17) to (16, 17)); the fade
/// overlay the room started takes him up again.
///
/// The overlay (field/field_sequence_ground_mask.c) covers him with squares
/// the colour of the floor wherever the ground just south of him stands
/// higher than his feet: walking along the south side of the room's sunken
/// middle floor, he shows only from the knees down. It runs only when he came
/// in from the room of Ki or up the middle passage, and coming up the passage
/// starts it with no one to follow; come in any other way, he walks there
/// whole.
static void FuchinFallsCave_FadeOverlayFollowsLeader(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + HEAP_SLOT_FADE_OVERLAY);

    work->actor = Actor_Get(ACTOR_PARTY_LEADER);
  }
}

/// Nothing visible happens when Isaac walks south into the top of the middle
/// passage out of the dragon statue's dark room (cells (14, 18) to (16, 18));
/// the room's fade overlay stops following him.
static void FuchinFallsCave_FadeOverlayStopsFollowing(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + HEAP_SLOT_FADE_OVERLAY);

    work->actor = NULL;
  }
}

/// Sets up the scene as the party enters: Isaac walks up to Fever in Imil Falls
/// Cave, and each room of Fuchin Falls Cave is put in order.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.next == SCENE_IMIL_FALLS_CAVE) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardFever();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Isaac comes into Imil Falls Cave and walks up the ice to Fever.
static void Imil_WalkTowardFever(void) {
  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FEVER), 0);
  Event_StartScreenTransition();
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 0.1);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
  Event_End();
}

/// Puts a room of Fuchin Falls Cave in order for the way the party comes in:
/// its logs, the Dragon's Eye on its table, and the dragon statue's room dark
/// or lit.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct Actor *actor;
  struct FadeOverlayWork *work;

  ImilFuchinFalls_RebuildLogRoom();
  if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE) {
    if (!GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) &&
        gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if (gMainRoutineWork.entrance >=
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE &&
        gMainRoutineWork.entrance <=
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR) {
      // Two hidden crates on the west ledge by the stairs down, at cells
      // (9, 28) and (11, 28). Isaac walks over both cells and nothing shows.
      // XXX (no evidence): what they are for; nothing in the scene reads them
      // again, and no recording can take them away to compare (a probe hides
      // an actor only in its photo, where these show nothing already).
      FuchinFallsCave_CreateHiddenActor(toFIXED(156), 0, toFIXED(452),
                                        SPRITE_CRATE);
      FuchinFallsCave_CreateHiddenActor(toFIXED(188), 0, toFIXED(452),
                                        SPRITE_CRATE);
    }
  } else if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGON_STATUE) {
    // Default: NULL unless the arrival from the south half's middle takes the
    // fade overlay.
    work = NULL;
    actor = Actor_Get(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE);
    gImilFuchinFalls_TrackLeader = 0;
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    Actor_SetSpritePriority(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE, 1);
    Actor_SetSpriteColorById(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE,
                             SPRITE_COLOR_HIDDEN);
    switch (gMainRoutineWork.entrance) {
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      work = gFadeOverlayWork;
      work->actor = NULL;
      break;
    }
    if (gMainRoutineWork.entrance <=
        FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
      // Once the eye is set, the shadow of the hidden path is drawn again;
      // before, the room's northern half is dimmed to a dark blue.
      if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
        MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
        MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      } else {
        gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        PaletteFade_ApplySource(PALETTE_TINT_CUSTOM | RGB(8, 8, 12),
                                PALETTES_BG);
        PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12),
                                PALETTES_BG);
        PaletteFade_StartInterpolation(1);
        wait(1);
      }
    }
  }
}

/// Puts the logs of the room being entered where their flags say, first saving
/// the cells of their runs as the map draws them, which the rebuild routines
/// copy back from.
static void ImilFuchinFalls_RebuildLogRoom(void) {
  if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_ENTRANCE) {
    MapGrid_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    FuchinFallsCave_RebuildEntranceLogs();
  } else if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
    MapGrid_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    FuchinFallsCave_RebuildZephyrRoomLogs();
  } else if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE &&
             gMainRoutineWork.entrance !=
                 FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
    MapGrid_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    FuchinFallsCave_RebuildDragonsEyeRoomLogs();
  } else if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_ARCTIC_BLADE) {
    if (gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
        gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
      MapGrid_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      MapGrid_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    FuchinFallsCave_RebuildArcticBladeRoomLogs();
  }
}

/// Isaac jumps two cells across the water: in Zephyr's room from a log onto
/// the stump at the pool's west end or onto the upright log below it, and in
/// the Dragon's Eye room from the stump below the north shore onto the upright
/// log.
///
/// In Zephyr's room he jumps north from the upright log against the west wall
/// (cell (12, 19)) onto the stump (cell (12, 17)), west from the crosswise log
/// at the foot of the sandy path onto the same stump, and west from the
/// crosswise log at the south shore onto the upright log against the wall. In
/// the Dragon's Eye room he jumps south from the stump (cell (17, 22)) onto the
/// upright log, but only while it lies in the middle of the pool.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gFieldWork->stepTrigger;

  if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_ZEPHYR) {
    if (trigger == CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) {
      Scene_HopLeaderBy(0, -32);
    } else {
      Scene_HopLeaderBy(-32, 0);
    }
  }
  if (gMainRoutineWork.next == SCENE_FUCHIN_FALLS_CAVE_DRAGONS_EYE &&
      trigger == CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP &&
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE)) {
    Scene_HopLeaderBy(0, 32);
  }
}

/// In the Dragon's Eye room, Isaac jumps north from the upright log across the
/// water: from the middle of the pool onto the stump (cell (17, 22)), or from
/// the head of the long channel onto the upper crosswise log, when it lies at
/// the head too.
static void FuchinFallsCave_HopNorth(void) { Scene_HopLeaderBy(0, -32); }

/// In the Dragon's Eye room, Isaac jumps west from the lower crosswise log
/// halfway down the long channel onto the stump below the west ledge (cell
/// (12, 27)), from which he walks on to the stairs down to the Dragon's Eye.
static void FuchinFallsCave_HopWest(void) { Scene_HopLeaderBy(-32, 0); }

#include "shared/leader_hop.c"

/// Puts the Dragon's Eye on the little table in the middle of the round
/// chamber below the Dragon's Eye room, drawn as its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  actor =
      FieldActor_Create(SPRITE_BLANK, toFIXED(248), toFIXED(8), toFIXED(152));
  if (actor != NULL) {
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*glyph));
    Icon_DrawItem(ITEM_DRAGONS_EYE);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    gImilFuchinFalls_DragonsEye = actor;
  }
}

/// Searching the little table in the round chamber below the Dragon's Eye room
/// (cell (15, 9)) gives the party the Dragon's Eye that lies on it.
static void FuchinFallsCave_FindDragonsEye(void) {
  Event_Start();

  if (gImilFuchinFalls_DragonsEye != NULL) {
    ItemDisplay_ShowPicture(gImilFuchinFalls_DragonsEye, 3);
  }

  Party_GiveItem(ITEM_DRAGONS_EYE, 0);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND);

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Actor_Release(gImilFuchinFalls_DragonsEye);
  }

  Event_End();
}

/// Moves and grows a puff of the dragon's fire for one frame.
static void FuchinFallsCave_UpdateDustMotion(struct Actor *raw) {
  struct FieldEffect *object = ((struct FieldEffect *)raw);

  object->x += object->velocity.x;
  object->y += object->velocity.y;
  object->z += object->velocity.z;
  object->scaleX += object->scaleRateX;
  object->scaleY += object->scaleRateY;
}

/// Hides a puff of the dragon's fire when its script has run out.
static s32 FuchinFallsCave_HideDust(struct Actor *actor) {
  Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// Creates one puff of the dust sprite, flying at the given speed, in the
/// given colour and growing to the given size: the puffs of the dragon's fire.
void ImilFuchinFalls_SpawnDust(FIXED x, FIXED y, FIXED z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra) {
  struct FieldEffect *effect;
  struct Sprite *sprite;
  const s32 *script;
  const s32 *scripts[3] = {
      gImilFuchinFalls_ShortDustScript,
      gImilFuchinFalls_MediumDustScript,
      gImilFuchinFalls_LongDustScript,
  };

  effect = (struct FieldEffect *)Actor_Create(SPRITE_DUST_PUFF, x, y, z);
  if (effect == NULL)
    return;
  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript((struct Actor *)effect,
                    scripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = FuchinFallsCave_UpdateDustMotion;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  sprite->image.priority = 1;
  if ((flags & EFFECT_OPTION_MASK) == 0 || extra == NULL)
    return;
  if (flags & EFFECT_USE_PALETTE)
    Actor_SetSpriteColor((struct Actor *)effect, extra->palette);
  if (flags & EFFECT_USE_PRIORITY) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = extra->priority;
  }
  if (flags & EFFECT_USE_START_SCALE) {
    effect->scaleX = extra->startScaleX;
    effect->scaleY = extra->startScaleY;
  }
  if (flags & EFFECT_SCALE_TO_TARGET) {
    script = scripts[flags & EFFECT_SCRIPT_MASK];
    if (flags & EFFECT_USE_START_SCALE) {
      effect->scaleRateX = (extra->targetScaleX - effect->scaleX) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
      effect->scaleRateY = (extra->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (extra->targetScaleX - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
      effect->scaleRateY = (extra->targetScaleY - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
}

/// Every fourth frame while the dragon statue wakes, a red puff of fire blows
/// out of its mouth toward the west, with a whoosh every eighth.
static void FuchinFallsCave_BreatheDragonFire(void) {
  struct CaveDustOptions options;
  s32 framePhase;
  FIXED velocityX;
  FIXED velocityY;

  framePhase = gFrameCount & 3;
  if (framePhase != 0)
    return;
  options.palette = SPRITE_COLOR_RED_TINT;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  options.targetScaleX = 9 * FIXED_ONE / 5;
  options.targetScaleY = 9 * FIXED_ONE / 5;
  if ((gFrameCount & 7) == 0)
    Sound_Play(SE_WHOOSH);
  velocityX = -toFIXED(1) - toFIXED(Random_Below(2));
  velocityY = -((Random_Below(3)) * (toFIXED(1) / 5));
  ImilFuchinFalls_SpawnDust(toFIXED(308), toFIXED(64), toFIXED(222), velocityX,
                            velocityY, framePhase,
                            EFFECT_SCRIPT_MEDIUM | EFFECT_USE_PALETTE |
                                EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
                            (const struct EffectOptions *)&options);
}

/// Isaac uses the Dragon's Eye at the steps below the dragon statue (cells
/// (20, 14) to (22, 14)): the music stops, the dark room flashes, and the
/// dragon breathes fire west across the room, which glows red and then comes
/// back in its own colours, with the shadow of a hidden path across the spikes.
///
/// The eye is used up, and the room stays lit from then on
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET).
static void FuchinFallsCave_LightDragonWithEye(void) {
  u32 pass;

  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_GEM_SET);
  Event_Start();
  Field_EndBusyAndReveal();

  for (pass = 0; pass < 4; pass++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(24, 24, 16), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    wait(2);
    if (pass == 0) {
      MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    wait(2);
  }

  wait(30);

  // The dragon breathes fire while the room turns red.
  Task_AddFrame(FuchinFallsCave_BreatheDragonFire, TASK_PASS_FRAME);
  wait(40);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 4, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  wait(80);
  Task_Remove(FuchinFallsCave_BreatheDragonFire);
  wait(20);

  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(80);

  wait(80);

  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
  PartyInventory_Discard(ITEM_DRAGONS_EYE);
  Scene_PlayMusic();

  Event_End();
}

/// Isaac reads the tablet on the first room's south shore (cell (15, 35)): `The
/// dragon's flame illuminates the path of truth in darkness.`
static void FuchinFallsCave_ReadDragonFlameInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinDragonFlameShowsPath, 1);
  Event_End();
}

/// Isaac reads the tablet beside the chest in the room of Ki (cell (16, 5)):
/// `The secret of Ki shall be revealed to the disciples of truth.`
static void FuchinFallsCave_ReadKiInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinSecretKiRevealed, 1);
  Event_End();
}

/// Isaac reads the tablet in the south half of the dragon statue's room (cell
/// (15, 29)): `Rays of light give birth to shadows, revealing the way.`
static void FuchinFallsCave_ReadShadowInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinLightRevealsShadows, 1);
  Event_End();
}

/// Isaac looks at the dragon statue from the steps below it (cells (20, 14) to
/// (22, 14)): `It's a statue of an eyeless dragon.`, and once the eye is set,
/// `It's a statue of a dragon with burning red eyes.`
///
/// While the party carries the Dragon's Eye, the field menu opens after the
/// first, so that the player can use it.
static void FuchinFallsCave_SearchDragonStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
    FieldMessage_Show(MsgFuchinDragonRedEyes, 1);
  } else {
    FieldMessage_Show(MsgFuchinEyelessDragon, 1);
    if (PartyInventory_FindMember(ITEM_DRAGONS_EYE) != -1) {
      gFieldWork->fieldMenu = 1;
    }
  }
  Event_End();
}

/// When Isaac's Reveal fades in the dark half of the dragon statue's room,
/// before the Dragon's Eye is set, the room fades up from black to its dim blue
/// again.
static void FuchinFallsCave_EndRevealLighting(void) {
  struct FieldEffectWork *effectWork;
  struct ScreenTransitionState *state;

  if (gMainRoutineWork.entrance <=
      FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
    effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
    state = *(gHeapSlots + HEAP_SLOT_SCREEN_TRANSITION);
    effectWork->childOption = 1;
    state->nextMode = 0;
    state->duration = 1;
    state->step = 1;
    PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_BG);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(16);
    wait(16);
  }
}

/// While Isaac slides on Imil Falls Cave's ice, turns him a step at a time
/// toward the way the pad points, and has him look around when it points
/// nowhere.
static void Imil_TurnOnIceTowardHeldDirection(struct Actor *actor) {
  s32 timer;
  s32 direction;
  u16 facing;

  timer = actor->timer;
  if (timer != 0) {
    actor->timer--;
    return;
  }
  actor->steeringFlags = 0;
  direction = gIce_KeyHeadings[DPAD_INDEX(gKeysHeld)];
  if (direction == KEY_HEADING_NONE) {
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
    return;
  }
  facing = actor->facing;
  direction = (s16)(direction - facing);
  if (direction > FACING_STEP)
    direction = FACING_STEP;
  if (direction < -FACING_STEP)
    direction = -FACING_STEP;
  actor->facing = facing + direction;
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, IMIL_SLIDE_ANIMATION_STEP);
}

/// Slides Isaac along Imil Falls Cave's ice, a cell at a time the way the pad
/// points, until a wall or higher ground stops him or he slides off the ice.
static void Imil_SlideActorAcrossIce(void) {
  struct Actor *actor;
  struct FieldPosition probe;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  FIXED z;
  FIXED x;

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[DPAD_INDEX(gKeysHeld)];
    if ((u16)heading == (u16)KEY_HEADING_NONE) {
      return;
    }

    Event_Start();

    probe.x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    z = probe.z;
    x = probe.x;
    startBehavior = MapGrid_GetCellBehaviorAt(actor->layer, x, z);
    FieldVector_AddPolarOffset(16, heading, &probe);

    nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
                actor->y.fixed >
            toFIXED(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    actor->timer = 0;
    FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);

    Actor_SelectAnimation(actor, ANIM_WALK);
    Actor_SetAnimationSpeed(actor, IMIL_SLIDE_ANIMATION_STEP);
    Actor_WaitForMove(actor);
    actor->update = Imil_TurnOnIceTowardHeldDirection;

    for (;;) {
      FieldVector_AddPolarOffset(16, heading, &probe);
      nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
      if (nextBehavior == CELL_BEHAVIOR_WALL) {
        break;
      }
      if (MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
              actor->y.fixed >
          toFIXED(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speedLimit = toFIXED(2);
      actor->acceleration = toFIXED(1) / 10;
      FieldActor_SetMoveTargetFX(actor, probe.x, probe.y, probe.z);
      Actor_WaitForMove(actor);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1);
    FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);
    Actor_WaitForMove(actor);
    wait(2);
  }

blocked:
  // Sliding off the ice ends the slide and gives back normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = toFIXED(1) / 4;

tail:
  wait(10);
  Event_End();
}

// clang-format off
#include "shared/ice_key_headings.c"

s16 gImilFuchinFalls_SlideHeadings[] = {
    KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE,
    FACING_EAST, FACING_EAST,
};

struct Actor *gImilFuchinFalls_DragonsEye = NULL;

struct SceneEntrance gImilFuchinFalls_NoEntrances[] = {
    { 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FALLS_CAVE_FROM_IMIL, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_EntranceEntrances[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ZephyrRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonsEyeRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ArcticBladeRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
    { SCENE_TABLE_END },
};

u32 gImilFuchinFalls_Exits[] = {
    SCENE_EXITS(ImilFallsCave),
    SCENE_EXIT(IMIL_FALLS_CAVE_TO_IMIL, Imil, IMIL_FROM_IMIL_FALLS_CAVE),
    SCENE_EXITS(FuchinFallsCaveEntrance),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds, FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST),
    SCENE_EXITS(FuchinFallsCaveZephyr),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    SCENE_EXITS(FuchinFallsCaveDragonsEye),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXITS(FuchinFallsCaveArcticBlade),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    SCENE_EXITS(FuchinFallsCaveOrbOfForce),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST),
    SCENE_EXITS(FuchinFallsCaveDragonStatue),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gImilFuchinFalls_NoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ImilFallsCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FEVER), MOVEMENT_TYPE_STAND,
        263, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_EntranceActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        200, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        296, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ZephyrRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ZEPHYR), MOVEMENT_TYPE_STAND,
        344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonsEyeRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        328, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ArcticBladeRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        120, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gImilFuchinFalls_NoTriggers[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gImilFuchinFalls_ImilFallsCaveTriggers[] = {
    { TRIGGER_STEP, IMIL_FALLS_CAVE_TO_IMIL, CONDITION_ALWAYS, EVENT(Imil_LeaveFallsCave) },
    { TRIGGER_STEP, CELL_BEHAVIOR_IMIL_FALLS_CAVE_ICE, CONDITION_ALWAYS, EVENT(Imil_MoveAcrossIce) },
    { TRIGGER_TALK, ACTOR_FEVER, CONDITION_ALWAYS, EVENT(Imil_FeverJoins) },
    { TRIGGER_TABLE_END },
};

struct Trigger gImilFuchinFalls_EntranceTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveFirstLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveFirstLogWest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_WEST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveSecondLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveSecondLogNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveThirdLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveThirdLogNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 27, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FUCHIN_FALLS_ENTRANCE_TABLET, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ReadDragonFlameInscription) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_MIMIC_BEATEN, FIND_MIMIC | 1 },
    { TRIGGER_TABLE_END },
};

struct Trigger gImilFuchinFalls_ZephyrRoomTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_START, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_SOUTH, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { TRIGGER_TALK, ACTOR_ZEPHYR, CONDITION_ALWAYS, EVENT(FuchinFallsCave_FightZephyr) },
    { TRIGGER_TABLE_END },
};

struct Trigger gImilFuchinFalls_DragonsEyeRoomTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, IF_FLAG_SET(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END), EVENT(FuchinFallsCave_HopNorth) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_EAST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_EAST, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_SOUTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_SOUTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFourthLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFourthLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFourthLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFourthLog) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFourthLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_ROOM_STUMP, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGONS_EYE_TABLE, FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND, EVENT(FuchinFallsCave_FindDragonsEye) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gImilFuchinFalls_ArcticBladeRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH,
     CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
     CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_NORTH,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleFirstLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST,
     FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST,
     CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_EAST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleSecondLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_WEST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SettleThirdLog)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ARCTIC_BLADE,
     FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {TRIGGER_TABLE_END},
};

struct Trigger gImilFuchinFalls_KiRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FUCHIN_FALLS_KI_ROOM_TABLET,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_ReadKiInscription)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
     FIND_ITEM(ITEM_ORB_OF_FORCE)},
    {TRIGGER_TABLE_END},
};

struct Trigger gImilFuchinFalls_DragonRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 12, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST},
    {TRIGGER_STEP,
     CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_NORTH_OF_MIDDLE_PASSAGE,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_FadeOverlayFollowsLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_MIDDLE_PASSAGE,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_FadeOverlayStopsFollowing)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT),
     CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_ROOM_TABLET, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ReadShadowInscription)},
    {USE_ITEM(ITEM_DRAGONS_EYE), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE,
     FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT(FuchinFallsCave_LightDragonWithEye)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FUCHIN_FALLS_DRAGON_STATUE,
     CONDITION_ALWAYS, EVENT(FuchinFallsCave_SearchDragonStatue)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT(FuchinFallsCave_EndRevealLighting)},
    {TRIGGER_TABLE_END},
};

s32 gImilFuchinFalls_ShortDustScript[] = {
    SC_WAIT(1), SC_LOOP(22, SC_TOP),  SC_CALL(FuchinFallsCave_HideDust),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

s32 gImilFuchinFalls_MediumDustScript[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP),  SC_CALL(FuchinFallsCave_HideDust),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

s32 gImilFuchinFalls_LongDustScript[] = {
    SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_CALL(FuchinFallsCave_HideDust),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};
