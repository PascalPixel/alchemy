/// @file
/// Inside Air's Rock, reached through the doorway the lightning opens at its
/// top, with a golden door low on its cliff also leading into the great hall
/// (airs_rock.c): the hall and the rooms around it, their doorways marked by
/// coloured dots.
///
/// Felix moves pillars with Move or by pushing them, so that they fill gaps
/// or block the vents whose puffs of steam blow him off his path; Pound
/// sinks one of a pair of stakes and raises the other; Whirlwind on a Wind
/// Stone sends a gust that carries pillars, blows golden barriers away and
/// turns bridges, and on a twister rock wakes the statues, whose little
/// whirlwinds carry the party. The party crosses tightropes and stepping
/// stones that vanish behind them. At the heart of the rock, in the room of
/// the Reveal tablet, Sheba reads the tablet and learns Reveal
/// (AirsRockInside_ReadRevealTablet), and a Psynergy Stone in another room
/// restores the party's PP (AirsRockInside_CheckPsynergyStone).

#include "field/field_sequence.h"
#include "field/emote.h"
#include "field/field_psynergy_state.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "sounds.h"
#include "types.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/iwram_call.h"
#include "system/io_write.h"
#include "system/main_routine_state.h"
#include "system/frame.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/scene_call.h"
#include "scene/airs_rock.h"
#include "scene/dehkan_plateau.h"
#include "scene/gabomba_statue.h"
#include "graphics/palette_buffers.h"
#include "window/window.h"

/// Set once Move has left the hall's movable statue on half cell (103, 41).
#define FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED 0xa8e
/// Set once Move has left the Vial chest room's western pillar on (13, 49).
#define FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED 0x898
/// Set once Move has left its eastern pillar on (41, 49).
#define FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED 0x897
/// Set once Move has left the Clarity Circlet chest room's first pillar on (33,
/// 79).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED 0x89b
/// Its second, on (77, 71).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED 0x89c
/// Its third, on (59, 65).
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED 0x89d
/// Set once Move has left the tightropes room's pillar on (81, 37).
#define FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED 0x89e
/// Set once a gust has dropped the Elixir chest room's pillar below its bridge
/// (AirsRockInside_BlowGust). The set-up sets it on every entry, so the room
/// always shows the pillar dropped and its Move trigger never runs.
#define FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED 0x89f
/// Set once Move has left the paired stakes room's pillar on (43, 87).
#define FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED 0x8d0
/// Set once Move has left the face vents room's first pillar on (35, 59).
#define FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED 0x8d1
/// Its second, on (45, 105).
#define FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED 0x8d2
/// Its third, on (23, 77).
#define FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED 0xa8f
/// Set once Move has left the coins chest room's pillar on (45, 33).
#define FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED 0x8d3

/// Story flags: the statues of the hall and the Vial chest room woken by
/// Whirlwind, and the Reveal tablet read.
#define FLAG_AIRS_ROCK_HALL_STATUE_WOKEN 0x89a
#define FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN 0x899
#define FLAG_REVEAL_TABLET_READ 0x8d4

/// The paired stakes room's stakes, each set while it is down; B and C go
/// down as the party comes in.
#define FLAG_STAKE_A_DOWN FLAG_SCENE_TEMP_00
#define FLAG_STAKE_B_DOWN FLAG_SCENE_TEMP_01
#define FLAG_STAKE_C_DOWN FLAG_SCENE_TEMP_02
#define FLAG_STAKE_D_DOWN FLAG_SCENE_TEMP_03

/// The face vents room's vents kept still by where Move leaves a pillar
/// (AirsRockInside_FinishPushingPillar): the vent beside each puffs in its
/// turn (AirsRockInside_PuffVents).
#define FLAG_SOUTHEAST_VENT_STILL FLAG_SCENE_TEMP_00
#define FLAG_MIDDLE_WEST_VENT_STILL FLAG_SCENE_TEMP_01
#define FLAG_NORTHEAST_VENT_STILL FLAG_SCENE_TEMP_02

/// The Reveal tablet room: set once a stepping stone has vanished, and one
/// flag for each stone from the southernmost, set once it has.
#define FLAG_STONE_VANISHED FLAG_SCENE_TEMP_00
#define FLAG_FIRST_STONE_VANISHED FLAG_SCENE_TEMP_02

/// What the gusts leave for the rest of the visit: the cells a gust clears on
/// its path in the tightropes room (the set-up sets it on every entry, so the
/// room always shows them cleared), a turning bridge turned from its first lie,
/// the Elixir chest room's pillar left on its bridge, and a golden barrier
/// blown away in the paired stakes and coins chest rooms.
#define FLAG_TIGHTROPES_GUST_PATH_CLEARED FLAG_AREA_TEMP_00
#define FLAG_TIGHTROPES_BRIDGE_TURNED FLAG_AREA_TEMP_01
#define FLAG_ELIXIR_CHEST_BRIDGE_TURNED FLAG_AREA_TEMP_02
#define FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE FLAG_AREA_TEMP_03
#define FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_04
#define FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_05
#define FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_06

/// The Elixir chest room's pillar column, in half cells, that stands it on
/// the turning bridge.
#define ELIXIR_CHEST_PILLAR_ON_BRIDGE_X 65

/// The hall's entrance from the Reveal tablet's room, and where the party
/// lands after dropping from the Vial chest room (AirsRockInside_LandInHall);
/// the Vial chest room's entrance on the lift from the hall
/// (AirsRockInside_RiseOnLift).
enum {
  ENTRANCE_HALL_FROM_REVEAL_TABLET = 16,
  ENTRANCE_HALL_LANDING = 11,
  ENTRANCE_VIAL_CHEST_ON_THE_LIFT = 3
};

/// The hall's entrances by the part of it they open on: up to the first of
/// these the great hall itself, then its northeast room, and past the second
/// its east room. ENTRANCE_HALL_FROM_REVEAL_TABLET, past both, takes the great
/// hall's set-up and the east room's.
enum {
  HALL_LAST_GREAT_HALL_ENTRANCE = 11,
  HALL_LAST_NORTHEAST_ROOM_ENTRANCE = 13
};

/// The collision codes this scene reads by name. On the overhead layer a
/// whirlwind ends where it reaches WHIRLWIND_END; on the ground a raised
/// stake's top is STAKE_TOP; in the hall the tower's door is TOWER_DOOR, and
/// the Vial chest room is left by jumping off VIAL_CHEST_LEDGE. The
/// Reveal tablet room's stepping stones run north from STONES_SOUTH_END to
/// STONES_NORTH_END on the overhead layer, the five between them vanishing,
/// and its tablet, as the Psynergy Stone room's stone, is examined on its
/// cell.
enum AirsRockInsideCellBehavior {
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END = 1,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE = 2,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END = 7,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE = 20,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR = 30,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET = 30,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE = 30,
  /// The stepping stones Felix is drawn standing on: two kinds, a stack of
  /// two and the same one row nearer, and the Reveal tablet room's stones that
  /// light gold under him (AirsRockInside_StampCellUnderLeader and the rest).
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE = 80,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE = 90,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END = 212,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP = 212
};

/// The face vents room's six vents, by the cell in front of each on the ground
/// layer: south of the room the west and east one, in its middle the west and
/// east one, in its north the west and east one. A vent's puff reaching the
/// leader there blows him down to the place below it
/// (AirsRockInside_RideFaceVent).
enum AirsRockInsideFaceVentsVent {
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT = 50,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHEAST_VENT,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_WEST_VENT,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_EAST_VENT,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHWEST_VENT,
  CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHEAST_VENT
};

/// Where a gust stops on the overhead layer (AirsRockInside_BlowGust), its
/// code less CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP: up to LAST_BARRIER it
/// blows on, clearing what blocks its path, as the golden barriers of the
/// paired stakes and coins chest rooms; a turning bridge reads
/// BRIDGE_NORTH_SOUTH while it runs north and south, and BRIDGE_EAST_WEST while
/// it lies east and west, and the gust turns it the other way.
enum AirsRockInsideGustStop {
  GUST_STOP_PAIRED_STAKES_NORTH_BARRIER = 0,
  GUST_STOP_LAST_BARRIER = 4,
  GUST_STOP_BRIDGE_NORTH_SOUTH = 5,
  GUST_STOP_BRIDGE_EAST_WEST = 6
};
#define CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP 30 ///< the first stop's code

/// The triggers this scene raises (FieldState.raisedTrigger) besides a statue's
/// whirlwind carrying the leader (RAISED_RIDE_WHIRLWIND), each room reading
/// them its own way: a vent's puff reaching him, and in the Reveal tablet room,
/// a cell he has left, as TRIGGER_LEFT_CELL plus its collision code.
enum AirsRockInsideRaisedTrigger {
  TRIGGER_PUFF_REACHES_LEADER = 201,
  TRIGGER_LEFT_CELL = 200,
  TRIGGER_FIRST_VANISHING_STONE = TRIGGER_LEFT_CELL +
      CELL_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE
};

void Actor_StartScript(struct Actor *actor, const u8 *data);

/// The actors of each inner room, numbering its placement table. The statues
/// (SPRITE_WIND_STATUE) breathe whirlwinds once Whirlwind on their room's
/// twister rock wakes them; Whirlwind on a Wind Stone (SPRITE_WIND_STONE)
/// sends a gust; stone pillars (SPRITE_STONE_PILLAR) are Moved.
enum AirsRockInsideHallActorId {
  ACTOR_HALL_WEST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_HALL_EAST_STATUE,
  /// The statue the hall's own twister rock wakes.
  ACTOR_HALL_TWISTER_STATUE,
  ACTOR_HALL_TWISTER_ROCK,
  /// The statue in the hall's east room, which Move pushes onto its mark.
  ACTOR_HALL_MOVABLE_STATUE,
  /// The three whirlwinds the movable statue breathes in turn.
  ACTOR_HALL_FIRST_WHIRLWIND
};

/// The Vial chest's room (gAirsRockInside_VialChestPlacements).
enum AirsRockInsideVialChestActorId {
  ACTOR_VIAL_CHEST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_VIAL_CHEST_EAST_PILLAR,
  ACTOR_VIAL_CHEST_TWISTER_ROCK,
  ACTOR_VIAL_CHEST_WEST_PILLAR
};

/// The Clarity Circlet chest's room
/// (gAirsRockInside_ClarityCircletChestPlacements).
enum AirsRockInsideClarityCircletChestActorId {
  ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
  ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
  ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
  ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C
};

/// The tightropes' room (gAirsRockInside_TightropesPlacements).
enum AirsRockInsideTightropesActorId {
  ACTOR_TIGHTROPES_NORTH_WIND_STONE = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPES_PILLAR,
  ACTOR_TIGHTROPES_SOUTH_WIND_STONE,
  /// The posts of its five tightropes, two to a rope.
  ACTOR_TIGHTROPES_POST_A,
  ACTOR_TIGHTROPES_POST_B,
  ACTOR_TIGHTROPES_POST_C,
  ACTOR_TIGHTROPES_POST_D,
  ACTOR_TIGHTROPES_POST_E,
  ACTOR_TIGHTROPES_POST_F,
  ACTOR_TIGHTROPES_POST_G,
  ACTOR_TIGHTROPES_POST_H,
  ACTOR_TIGHTROPES_POST_I,
  ACTOR_TIGHTROPES_POST_J
};

/// The Reveal tablet (SPRITE_SLATE), which rises as Sheba reads it.
#define ACTOR_REVEAL_TABLET ACTOR_FIRST_PLACED

/// The Elixir chest's room (gAirsRockInside_ElixirChestPlacements).
enum AirsRockInsideElixirChestActorId {
  /// The stone pillar AirsRockInside_MoveElixirChestPillar moves.
  ACTOR_ELIXIR_CHEST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE,
  ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE,
  ACTOR_ELIXIR_CHEST_POST_A,
  ACTOR_ELIXIR_CHEST_POST_B,
  ACTOR_ELIXIR_CHEST_POST_C,
  ACTOR_ELIXIR_CHEST_POST_D,
  ACTOR_ELIXIR_CHEST_POST_E,
  ACTOR_ELIXIR_CHEST_POST_F
};

/// The paired stakes' room (gAirsRockInside_PairedStakesPlacements).
enum AirsRockInsidePairedStakesActorId {
  ACTOR_PAIRED_STAKES_PILLAR = ACTOR_FIRST_PLACED,
  /// The three pillars pushed among the stakes.
  ACTOR_PAIRED_STAKES_PILLAR_A,
  ACTOR_PAIRED_STAKES_PILLAR_B,
  ACTOR_PAIRED_STAKES_PILLAR_C,
  /// The four stakes (SPRITE_STAKE), in two pairs: as one sinks, the other
  /// rises.
  ACTOR_PAIRED_STAKES_STAKE_A,
  ACTOR_PAIRED_STAKES_STAKE_B,
  ACTOR_PAIRED_STAKES_STAKE_C,
  ACTOR_PAIRED_STAKES_STAKE_D,
  ACTOR_PAIRED_STAKES_NORTH_WIND_STONE,
  ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE
};

/// The room of the face vents (gAirsRockInside_FaceVentsPlacements).
enum AirsRockInsideFaceVentsActorId {
  ACTOR_FACE_VENTS_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FACE_VENTS_SECOND_PILLAR,
  ACTOR_FACE_VENTS_WIND_STONE,
  ACTOR_FACE_VENTS_STATUE,
  ACTOR_FACE_VENTS_THIRD_PILLAR,
  ACTOR_FACE_VENTS_PILLAR_A,
  ACTOR_FACE_VENTS_PILLAR_B
};

/// The coins chest's room (gAirsRockInside_CoinsChestPlacements).
enum AirsRockInsideCoinsChestActorId {
  ACTOR_COINS_CHEST_WIND_STONE = ACTOR_FIRST_PLACED,
  ACTOR_COINS_CHEST_PILLAR,
  ACTOR_COINS_CHEST_STATUE
};

/// The cell stampers keep their last cell and the block they copy.
extern s32 gAirsRockInside_StampedCell;
s32 gAirsRockInside_StampedWidth BSS_DATA;
s32 gAirsRockInside_StampedHeight BSS_DATA;
s32 gAirsRockInside_StampedX BSS_DATA;
s32 gAirsRockInside_StampedZ BSS_DATA;

#include "system/system.h"
#include "field/actor.h"
#include "types.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "field/map_grid.h"
#include "flags.h"
#include "field/field_sprite.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"
#include "field/call.h"
#include "system/main_routine_state.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "field/field_sequence_event.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/task.h"
#include "field/event_commands.h"
#include "field/object.h"
#include "graphics/resource.h"
#include "system/bg_scroll.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "version.h"
#include "shared/tables.h"
#include "field/field_effect.h"

struct Actor;

extern struct ActorPlacement gAirsRockInside_Placements[];
extern struct ActorPlacement gAirsRockInside_HallPlacements[];
extern struct ActorPlacement gAirsRockInside_VialChestPlacements[];
extern struct ActorPlacement gAirsRockInside_ClarityCircletChestPlacements[];
extern struct ActorPlacement gAirsRockInside_RevealTabletPlacements[];

void AirsRockInside_RestoreCellBehindLeader(void);

void AirsRockInside_BurstDust(s32 x, s32 z);
void AirsRockInside_ScatterSparks(s32 x, s32 z);

extern s16 gAirsRockInside_HallPillars[];
extern s16 gAirsRockInside_VialChestPillars[];
extern s16 gAirsRockInside_ClarityCircletChestPillars[];
extern s16 gAirsRockInside_TightropesPillars[];
extern s16 gAirsRockInside_PairedStakesPillars[];
extern s16 gAirsRockInside_FaceVentsPillars[];
extern s16 gAirsRockInside_CoinsChestPillars[];

void Actor_LiftTargetBlock(const s16 *list);
void Actor_PlaceMovedBlock(void);

void AirsRockInside_StampCellUnderLeader(void);
void AirsRockInside_ReadRevealTablet(void);

extern struct Trigger gAirsRockInside_HallTriggers[];
extern struct Trigger gAirsRockInside_VialChestTriggers[];
extern struct Trigger gAirsRockInside_ClarityCircletChestTriggers[];
extern struct Trigger gAirsRockInside_TightropesTriggers[];
extern struct Trigger gAirsRockInside_RevealTabletTriggers[];
extern struct Trigger gAirsRockInside_ElixirChestTriggers[];
extern struct Trigger gAirsRockInside_PairedStakesTriggers[];
extern struct Trigger gAirsRockInside_FaceVentsTriggers[];
extern struct Trigger gAirsRockInside_CoinsChestTriggers[];
extern struct Trigger gAirsRockInside_PsynergyStoneTriggers[];

void AirsRockInside_StartRestoringCells(void);
void AirsRockInside_SetLedgeCellsByLevel(void);
void AirsRockInside_SpawnMotes(void);

void Actor_PlaceBlocks(u16 *actors);

void AirsRockInside_RiseOnLift(void);

void Tightrope_Initialize(void);
s32 Tightrope_Place(s32 slot, s32 a, s32 b);

void AirsRockInside_PlaceStakes(u16 *list);
void AirsRockInside_TakeOverWhirlwind(void);
void AirsRockInside_RideWhirlwind(void);
void AirsRockInside_BlowGust(void);

void AirsRockInside_StampOtherCellUnderLeader(void);
void AirsRockInside_StampTallCellUnderLeader(void);
void AirsRockInside_StampTallCellUnderLeaderNearer(void);
void AirsRockInside_WakeHallStatue(void);
void AirsRockInside_OpenTabletRoomDoor(void);
void AirsRockInside_StartPushingPillar(void);
void AirsRockInside_FinishPushingPillar(void);
void AirsRockInside_WakeVialChestStatue(void);
void AirsRockInside_JumpDownLedge(void);
void AirsRockInside_RideClarityCircletVent(void);
void AirsRockInside_StepOffVanishingStone(void);
void AirsRockInside_StampWideCellUnderLeader(void);
void AirsRockInside_HideRevealedDoors(void);
void AirsRockInside_RevealDoors(void);
void AirsRockInside_WalkToRevealTablet(void);
void AirsRockInside_ReadTabletFacingWest(void);
void AirsRockInside_ReadTabletFacingEast(void);
void AirsRockInside_MoveElixirChestPillar(void);
void AirsRockInside_PoundPairedStakesStake(s32 mode, s32 step);
void AirsRockInside_StepOntoStake(void);
void AirsRockInside_RideFaceVent(void);
void AirsRockInside_CheckPsynergyStone(void);
void AirsRockInside_IgnoreStep(void);

extern u32 gFieldEffect_MediumScript[];
extern u32 gAirsRockInside_WindStoneScript[];

void Actor_PlaySoundInView(struct Actor *actor, s32 cue);
s32 Actor_HasArrived(struct Actor *actor);

void AirsRockInside_SetStakeCells(struct Actor *actor);

/// Unused: places the pillars of a list as shared/pillar_place.h does, reading
/// its halfwords as signed.
UNUSED void Pillar_Place(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == ACTOR_LIST_END)
    return;
  for (entry = list; (u16)entry[0] != ACTOR_LIST_END; entry += 3) {
    s32 id = (u16)entry[0];
    struct Actor *actor = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
  }
}

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern struct ActorPlacement gAirsRockInside_TightropesPlacements[];
extern struct ActorPlacement gAirsRockInside_ElixirChestPlacements[];
extern struct ActorPlacement gAirsRockInside_PairedStakesPlacements[];
extern struct ActorPlacement gAirsRockInside_FaceVentsPlacements[];
extern struct ActorPlacement gAirsRockInside_CoinsChestPlacements[];

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

/// A mote of light moves on to a random spot, x 176 to 431 pixels and y and z
/// up to 31 and 127 pixels past the leader's, to drift there for its timer's
/// frames, swelling slowly or quickly by its lifetime.
void AirsRockInside_ScatterMote(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  if (Random16() & 0xff) {
    actor->x.fixed = INTtoFIXED(Random16() & 0xff) + toFIXED(176);
    actor->y.fixed = leader->y.fixed + INTtoFIXED(Random16() & 31);
    actor->z.fixed = leader->z.fixed + INTtoFIXED(Random16() & 127);
  }
  actor->userValue = actor->timer;
  if (actor->timer == PARTICLE_SLOW_LIFETIME)
    Actor_SelectAnimation(actor, ANIM_PARTICLE_SLOW_SWELL);
  else
    Actor_SelectAnimation(actor, ANIM_PARTICLE_QUICK_SWELL);
}

/// A mote's update: after its start delay it sways as it drifts upward, a
/// sway every 64 pixels of depth and every 128 of height, scattering again
/// when its time is up; it and the map's animations stop while a menu,
/// Psynergy or an item is in use.
void AirsRockInside_DriftMote(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0) {
    Actor_SetAnimationSpeed(actor, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if (actor->sceneByte != 0) {
    if ((gFrameCount & 7) == 0)
      actor->sceneByte--;
    return;
  }
  if (actor->userValue == 0)
    AirsRockInside_ScatterMote(actor);
  actor->userValue--;
  actor->x.fixed += Trig_Cos((FIXEDtoINT_FOUR_TIMES(actor->z.fixed) +
                              FIXEDtoINT_TWICE(actor->y.fixed))
                             << 8) /
                    2;
  actor->y.fixed += actor->speedLimit;
}

/// Nothing visible happens of its own as Felix starts casting Whirlwind on a
/// twister rock or a Wind Stone inside the rock; the game clears what is
/// pending so that the cast can go on to the rock's or stone's own whirlwind
/// (AirsRockInside_TakeOverWhirlwind).
void AirsRockInside_StartWhirlwind(void) { Event_EndLastingPsynergy(0); }

/// Nothing visible happens of its own as Felix casts Whirlwind on a twister
/// rock or a Wind Stone inside the rock; the game tells Whirlwind to leave out
/// its usual little whirlwind, so that what follows is the rock's or the
/// stone's own (AirsRockInside_BlowGust, AirsRockInside_WakeHallStatue and
/// AirsRockInside_WakeVialChestStatue).
void AirsRockInside_TakeOverWhirlwind(void) {
  ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->sceneVariant =
      1;
}

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_landed.c"

#include "../field/map_cell_get.c"

/// Sets a stake's cells by its flag (its timer), set once it has sunk: a sunk
/// stake's cell is open ground and its height drops 32 pixels unless it stands
/// on its ground, drawn under the leader; a raised one is a wall on the
/// overhead layer with its top (CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP) on
/// the ground layer, 32 pixels higher.
void AirsRockInside_SetStakeCells(struct Actor *actor) {
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 ground = MapCell_GetAttributes(MAPGRID_LAYER_GROUND, x, z);
  s32 cell = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, x, z);
  s32 height = CELL_HEIGHT_STEPS(
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));
  s32 set = GameFlag_Get(actor->timer);
  u8 *flags = &actor->priorityFlags;
  s32 px = FIELD_CELL_ORIGIN(x);
  s32 pz = FIELD_CELL_ORIGIN(z);

  if (set) {
    cell &= ~CELL_CODE_BITS;
    ground = 0;
    if (CELL_HEIGHT_STEPS(actor->groundHeight) != height)
      height -= 4;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Scene_SetActorSpritePriority(actor, 3);
  } else {
    cell |= CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
    ground |= CELL_ATTRIBUTES(CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP,
                              MAP_CELL_JUMP);
    *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
    height += 4;
    Scene_SetActorSpritePriority(actor, 1);
  }
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, x, z, ground);
  MapCell_SetAttributes(MAPGRID_LAYER_OVERHEAD, x, z, cell);
  MapGrid_SetCellHeight(px, pz, actor->layer, height);
}

/// A stake's picture in OBJ VRAM (stake.png): 32 by 64 pixels of 256-colour
/// tiles, four to each 8-pixel row, the stake itself within the first three
/// of them. A partner rising is drawn from its target's picture: the top
/// STAKE_TOP_ROWS rows at once, then a row a step.
enum { STAKE_ROW_TILES = 4, STAKE_DRAWN_TILES = 3, STAKE_TOP_ROWS = 3 };
/// The bytes of one 8-pixel row of a stake's picture.
#define STAKE_ROW_BYTES (STAKE_ROW_TILES * TILE_SIZE_8BPP)

/// One step of Pound on a stake: the target sinks and its partner, the other
/// of its pair, rises. On step 0 the partner's picture is cleared and the top
/// of the target's drawn over it, 28 pixels up; on steps 1 to 5 a row more of
/// it is drawn as it comes down; through step 7 the target flattens. In mode 1
/// both settle, the target's flag is set and the partner's cleared, they take
/// their next poses and their cells are set again
/// (AirsRockInside_SetStakeCells).
void AirsRockInside_PoundStake(s32 mode, s32 step) {
  s16 *pair =
      &((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->targetId;
  s32 even = *pair - ((*(u16 *)pair & 1) << 1);
  struct Actor *first = Actor_GetById(*pair);
  struct Actor *second = Actor_GetById(even + 1);
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramCacheEntry *entry;

  if (step == 0) {
    firstSprite = first->sprite;
    secondSprite = second->sprite;
    second->motionFlags = 0;
    second->y.fixed -= toFIXED(28);
    wait(1);
    entry = &gVramCacheEntries[secondSprite->slot];
    second->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
    entry = &gVramCacheEntries[firstSprite->slot];
    first->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
    Dma_Fill32(step, second->watchedActor, entry->size);
    Dma_Set(first->watchedActor, second->watchedActor,
            DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4), REG_DMA3);
    first->watchedActor =
        (void *)((u8 *)first->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
    second->watchedActor =
        (void *)((u8 *)second->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
  } else if (step <= 5) {
    second->y.fixed += toFIXED(26) / 5;
    Dma_Set(first->watchedActor, second->watchedActor,
            DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
    first->watchedActor = (void *)((u8 *)first->watchedActor + STAKE_ROW_BYTES);
    second->watchedActor =
        (void *)((u8 *)second->watchedActor + STAKE_ROW_BYTES);
  }
  if (step <= 7)
    first->scaleY += -(FIXED_ONE * 31 / 256);
  if (mode == 1) {
    first->scaleY = FIXED_ONE;
    second->y.fixed = second->groundHeight;
    GameFlag_Set(first->timer);
    GameFlag_Clear(second->timer);
    Actor_SelectAnimation(first,
                          ((struct FieldSprite *)first->sprite)->lastNo + 1);
    Actor_SelectAnimation(second,
                          ((struct FieldSprite *)second->sprite)->lastNo - 1);
    AirsRockInside_SetStakeCells(first);
    AirsRockInside_SetStakeCells(second);
  }
}

/// Places the stakes of a list ended by ACTOR_LIST_END, each entry three
/// halfwords: the stake, the first of its poses and the flag that says whether
/// it has sunk, which picks between them. The stake stands still, no longer
/// solid, and keeps its flag in its timer (AirsRockInside_SetStakeCells).
void AirsRockInside_PlaceStakes(u16 *list) {
  struct Actor *actor;
  s32 flag, base;
  s16 pose;
  u16 *entry;

  if (*list == ACTOR_LIST_END)
    return;
  for (entry = list; *entry != ACTOR_LIST_END; entry += 3) {
    actor = Actor_GetById(*entry);
    pose = ((s16 *)entry)[1];
    flag = ((s16 *)entry)[2];
    Actor_SetSpriteFlags(actor, 0);
    {
      s32 choice = GameFlag_Get((u16)flag);

      base = (u16)pose;
      base += base << 1;
      base += choice;
    }
    Actor_SelectAnimation(actor, base);
    actor->motionFlags = 0;
    actor->collisionFlags &= ~ACTOR_SOLID;
    actor->timer = flag;
    AirsRockInside_SetStakeCells(actor);
  }
}

/// The leader rides a statue's whirlwind along its path until it fades.
void AirsRockInside_RideWhirlwind(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *ride = leader->watchedActor;
  struct Actor *statue, *dust;
  s32 rise;
  FIXED dx, dz;
  s32 moved, frames, height;
  u16 facing;
  s32 angle;

  Event_Start();
  Event_EndLastingPsynergy(0);
  leader->motionFlags = ACTOR_FLOATS;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_RIDING_WHIRLWIND);
  Sound_Play(SE_KIRIN);
  rise = 0;
  while (ride->sceneByte != 0) {
    leader->x.fixed = ride->x.fixed;
    leader->y.fixed = ride->y.fixed + rise;
    leader->z.fixed = ride->z.fixed;
    if (rise < toFIXED(12))
      rise += toFIXED(1) / 10;
    leader->facing = (u8)gFrameCount * FACING_STEP;
    leader->priorityFlags ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  // The hall's whirlwind at x 312 lifts him into the Vial chest room.
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL &&
      FIELD_HALF_CELL(leader->x.fixed) == 39) {
    Event_SetExitRequest(
        AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_VIAL_CHEST_SOUTH_B);
    Event_End();
    return;
  }
  facing = ride->facing;
  switch (facing) {
  case FACING_NORTH:
    statue = ride->watchedActor;
    dx = 0;
    dz = 0;
    leader->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    leader->x.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->x.fixed) & ~1) + 1);
    leader->y.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->y.fixed) & ~1) + 3);
    leader->z.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->z.fixed) & ~1) + 1);
    for (frames = 0; frames != 60; frames++) {
      moved = 0;
      if (gInput.held & KEY_UP) {
        dx = 0;
        dz = -toFIXED(16);
        leader->facing = FACING_NORTH;
        moved = 1;
      }
      if (gInput.held & KEY_RIGHT) {
        dx = toFIXED(16);
        dz = 0;
        leader->facing = FACING_EAST;
        moved = 1;
      }
      if (gInput.held & KEY_LEFT) {
        dx = -toFIXED(16);
        leader->facing = FACING_WEST;
        dz = 0;
        moved = 1;
      }
      if (moved && CELL_CODE(MapCell_GetAttributes(
                       MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed + dx),
                       FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) !=
                       CELL_BEHAVIOR_WALL)
        goto found;
      wait(1);
    }
    dx = 0;
    dz = -toFIXED(16);
    leader->facing = FACING_NORTH;
    if ((u16)CELL_CODE(MapCell_GetAttributes(
            MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed),
            FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) ==
        CELL_BEHAVIOR_WALL) {
      dx = toFIXED(16);
      dz = 0;
      leader->facing = FACING_EAST;
      if ((u16)CELL_CODE(MapCell_GetAttributes(
              MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed + dx),
              FIELD_CELL(ride->z.fixed - ride->y.fixed))) ==
          CELL_BEHAVIOR_WALL) {
        dx = -toFIXED(16);
        leader->facing = FACING_WEST;
      }
    }
  found:
    // The statue's sceneCounter holds the layer of the ledge he lands on.
    height = Map_GetTerrainHeight(statue->sceneCounter, ride->x.fixed + dx,
                                  ride->z.fixed + dz);
    leader->y.fixed = height;
    leader->groundHeight = height;
    Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
    leader->speedLimit = toFIXED(2);
    leader->acceleration = toFIXED(1) / 2;
    leader->velocityY = toFIXED(4);
    leader->motionFlags = ACTOR_FALLS;
    leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    leader->layer = statue->sceneCounter;
    Actor_SetMoveTarget(leader, leader->x.fixed + dx, leader->y.fixed,
                        leader->z.fixed + dz);
    Actor_WaitUntilLanded(leader);
    Actor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    break;
  default:
    angle = ride->facing;
    leader->x.fixed =
        FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->x.fixed) & ~1) +
                               (angle = FACING_EIGHTH_INDEX(angle)) - 1);
    leader->z.fixed = FIELD_HALF_CELL_ORIGIN(
        (FIELD_HALF_CELL(ride->z.fixed) & ~1) - (angle & 2) + 1);
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = toFIXED(6);
    leader->priorityFlags =
        ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_AUTOMATIC;
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    FieldCamera_MoveToFX(-1, -1, -1, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = toFIXED(8);
    dust->acceleration = toFIXED(1);
    dust->motionFlags = 0;
    Actor_SetMoveTarget(
        dust, leader->x.fixed,
        Map_GetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
        leader->z.fixed);
    Actor_WaitUntilLanded(leader);
    Actor_WaitForMove(dust);
    break;
  }
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  GameFlag_Clear(FLAG_RIDING_WHIRLWIND);
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern u32 gAirsRockInside_WhirlwindFadesScript[];
extern u32 gAirsRockInside_WhirlwindRisesScript[];

/// How near a whirlwind comes to catch the leader above him while others move
/// him, in his cell's row, in pixels; on his level it is WHIRLWIND_REACH.
enum { WHIRLWIND_REACH_ABOVE = 16 };

/// A statue's whirlwind: it moves 1.5 pixels a frame the way it faces, upward
/// when it faces north, and where the overhead layer reads
/// CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END it fades (facing north) or
/// rises. The first time it comes within reach of the leader it carries him
/// along (RAISED_RIDE_WHIRLWIND). It stops while the leader moves as sand, a
/// menu or Psynergy is up, or an item is in use.
void AirsRockInside_MoveWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  FIXED speed = toFIXED(3) / 2;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0 ||
      gMainRoutineState.movementMode == LEADER_MOVEMENT_SAND) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, ANIM_STEP_NORMAL);
  if (CELL_CODE(MapCell_GetAttributes(
          MAPGRID_LAYER_OVERHEAD, FIELD_CELL(shot->x.fixed),
          FIELD_CELL(shot->z.fixed - shot->y.fixed))) ==
      CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
    shot->sceneByte = 0;
    shot->update = 0;
    Call2((void (*)())Actor_PlaySoundInView, (s32)shot, SE_SAND_EMERGE);
    if (shot->facing == FACING_NORTH)
      Actor_StartScript(shot, gAirsRockInside_WhirlwindFadesScript);
    else
      Actor_StartScript(shot, gAirsRockInside_WhirlwindRisesScript);
    return;
  }
  if (shot->sceneCounter == 0) {
    near = 0;
    leaderPosition = &leader->x.fixed;
    shotPosition = &shot->x.fixed;
    if (gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <=
              WHIRLWIND_REACH_ABOVE &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        near = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <=
              WHIRLWIND_REACH &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        near = 1;
    }
    if (near && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
      shot->sceneCounter = 1;
      shot->sceneByte = 1;
    }
  }
  if (shot->facing == FACING_NORTH) {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulFX(Trig_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulFX(Trig_Sin(shot->facing), speed);
  }
}

/// A woken statue's update: while no menu, Psynergy or item is in use it counts
/// frames in its timer, and every WHIRLWIND_BREATH_FRAMES breathes a new
/// whirlwind 8 pixels ahead of it, with a puff of dust below and a flare unless
/// the leader is riding one.
void AirsRockInside_StatueBreathesWhirlwind(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0)
    return;
  if (actor->timer == WHIRLWIND_BREATH_FRAMES) {
    if (!GameFlag_Get(FLAG_RIDING_WHIRLWIND))
      Actor_PlaySoundInView(actor, SE_FLAME_FLARE);
    if (actor->facing == FACING_NORTH) {
      position.x = actor->x.fixed;
      position.z = actor->z.fixed;
    } else {
      position.x =
          actor->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(actor->facing));
      position.z =
          actor->z.fixed + Math_MulFX(toFIXED(8), Trig_Sin(actor->facing));
    }
    shot = (struct Actor *)Import_ActorCreate(SPRITE_WHIRLWIND, position.x,
                                              actor->y.fixed, position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    shot->sceneCounter = 0;
    shot->sceneByte = 0;
    shot->facing = actor->facing;
    shot->update = AirsRockInside_MoveWhirlwind;
    shot->watchedActor = actor;
    actor->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, actor->y.fixed, position.z - toFIXED(6), 0, 0,
                      0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  actor->timer++;
}

/// Wakes a statue unless the party keeps its place: it breathes its first
/// whirlwind once its timer reaches WHIRLWIND_BREATH_FRAMES, and its whirlwinds
/// set the leader down on the ledge of layer `landingLayer`.
void AirsRockInside_WakeStatue(struct Actor *actor, s16 timer,
                               u8 landingLayer) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    actor->steeringFlags = 0;
    actor->timer = timer;
    actor->sceneCounter = landingLayer;
    actor->update = (s32)AirsRockInside_StatueBreathesWhirlwind;
  }
}

extern u32 gAirsRockInside_PooledWhirlwindFadesScript[];

/// A whirlwind of the hall's movable statue, one of three it uses in turn: as
/// AirsRockInside_MoveWhirlwind, but it also holds still until the room's
/// opening transition has begun (FieldState.transitionActive).
void AirsRockInside_MovePooledWhirlwind(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 near;
  FIXED speed = toFIXED(3) / 2;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0 || state->transitionActive == 0) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, ANIM_STEP_NORMAL);
  if (CELL_CODE(MapCell_GetAttributes(
          MAPGRID_LAYER_OVERHEAD, FIELD_CELL(shot->x.fixed),
          FIELD_CELL(shot->z.fixed - shot->y.fixed))) ==
      CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
    Actor_PlaySoundInView(shot, SE_SAND_EMERGE);
    shot->sceneByte = 0;
    shot->update = 0;
    Actor_StartScript(shot, gAirsRockInside_PooledWhirlwindFadesScript);
    /* FAKEMATCH(dead): an empty once-only block. */
    do {
    } while (0);
    return;
  }
  if (shot->sceneCounter == 0) {
    near = 0;
    if (gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS) {
      if (Position_DistanceAbove(&leader->x.fixed, &shot->x.fixed) <=
              WHIRLWIND_REACH_ABOVE &&
          FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
        near = 1;
    } else {
      if (Position_FlatDistance(&leader->x.fixed, &shot->x.fixed) <=
              WHIRLWIND_REACH &&
          FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
        near = 1;
    }
    if (near && !GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      leader->facing = shot->facing;
      leader->watchedActor = shot;
      leader->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
      shot->sceneCounter = 1;
      shot->sceneByte = 1;
    }
  }
  if (shot->facing == FACING_NORTH) {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->y.fixed -= Iwram_MulFX(Trig_Sin(shot->facing), speed);
  } else {
    shot->x.fixed += Iwram_MulFX(Trig_Cos(shot->facing), speed);
    shot->z.fixed += Iwram_MulFX(Trig_Sin(shot->facing), speed);
  }
}

/// The movable statue's update: as AirsRockInside_StatueBreathesWhirlwind, but
/// breathing the first of its three whirlwinds that is not on its way.
void AirsRockInside_StatueBreathesPooledWhirlwind(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;
  s32 i;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0)
    return;
  // DEAD: the three whirlwinds looked up, results unused; the ROM keeps the
  // three calls.
  for (i = 0; i != 3;)
    Actor_GetById(ACTOR_HALL_FIRST_WHIRLWIND + i++);
  if (actor->timer == WHIRLWIND_BREATH_FRAMES) {
    shot = 0;
    if (!GameFlag_Get(FLAG_RIDING_WHIRLWIND))
      Actor_PlaySoundInView(actor, SE_FLAME_FLARE);
    if (actor->facing == FACING_NORTH) {
      position.x = actor->x.fixed;
      position.z = actor->z.fixed;
    } else {
      position.x =
          actor->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(actor->facing));
      position.z =
          actor->z.fixed + Math_MulFX(toFIXED(8), Trig_Sin(actor->facing));
    }
    for (i = 0; i != 3; i++) {
      shot = Actor_GetById(ACTOR_HALL_FIRST_WHIRLWIND + i);
      if (shot->update == 0)
        break;
    }
    shot->x.fixed = position.x;
    shot->y.fixed = actor->y.fixed;
    shot->z.fixed = position.z;
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    shot->motionFlags = 0;
    shot->sceneCounter = 0;
    shot->sceneByte = 0;
    shot->facing = actor->facing;
    shot->watchedActor = actor;
    shot->update = AirsRockInside_MovePooledWhirlwind;
    actor->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, actor->y.fixed, position.z - toFIXED(6), 0, 0,
                      0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  actor->timer++;
}

/// Wakes the hall's movable statue as AirsRockInside_WakeStatue does.
void AirsRockInside_WakeMovableStatue(struct Actor *actor, s16 timer,
                                      u8 landingLayer) {
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    actor->steeringFlags = 0;
    actor->timer = timer;
    actor->sceneCounter = landingLayer;
    actor->update = (s32)AirsRockInside_StatueBreathesPooledWhirlwind;
    Actor_SetSpriteFlags(actor, 0);
  }
}

/// A vent's puff of steam: it drifts and grows until its time runs out;
/// reaching the leader it bounces back and carries him
/// (TRIGGER_PUFF_REACHES_LEADER), and it bounces off the room's pillars.
void AirsRockInside_UpdateVentPuff(struct Actor *actor) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *other;
  s32 *pos;
  u8 i;

  if (((struct PaletteFade *)Ram_HeapSlots->paletteFade)->duration != 0)
    return;
  if (state->transitionActive == 0)
    return;
  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0) {
    Actor_SetAnimationSpeed(actor, 0);
    return;
  }
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL);
  if (actor->timer == 0) {
    if (actor->x.fixed == 0) {
      Actor_Release(actor);
    } else {
      actor->z.fixed = 0;
      actor->x.fixed = 0;
    }
    return;
  }
  if (Position_FlatDistance(&leader->x.fixed, pos = &actor->x.fixed) <= 8 &&
      leader->y.part[0].integer == actor->y.part[0].integer) {
    state->raisedTrigger = TRIGGER_PUFF_REACHES_LEADER;
    actor->sceneWord = 0;
    actor->speedLimit = -actor->speedLimit >> 1;
    actor->z.fixed = leader->z.fixed - toFIXED(8);
  }
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    other = Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR);
    if (Position_FlatDistance(&other->x.fixed, pos) <= 8) {
      actor->speedLimit = -actor->speedLimit >> 1;
      actor->sceneWord = 0;
      actor->z.fixed = other->z.fixed - toFIXED(8);
    }
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
    for (i = 0; i != 3; i++) {
      struct Actor *pillar = Actor_GetById(i + ACTOR_FACE_VENTS_THIRD_PILLAR);

      if (Position_FlatDistance(&pillar->x.fixed, pos) <= 8) {
        actor->speedLimit = -actor->speedLimit >> 1;
        actor->sceneWord = 0;
        actor->z.fixed = pillar->z.fixed - toFIXED(8);
      }
    }
  }
  actor->timer--;
  actor->z.fixed += actor->speedLimit;
  actor->x.fixed += actor->acceleration;
  actor->speedLimit -= actor->sceneWord;
  actor->scaleX += FIXED_ONE / 20;
  actor->scaleY += FIXED_ONE / 20;
}

/// A puff of steam blown from a vent.
void AirsRockInside_PuffVent(s32 x, s32 y, s32 z) {
  struct Actor *actor = Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);

  Actor_SelectAnimation(actor, 1);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetSpriteColor(actor, SPRITE_COLOR_GREY);
  Actor_SetBlendMode(actor, 1);
  actor->timer = 27;
  actor->motionFlags = 0;
  actor->steeringFlags = 0;
  actor->scaleX = FIXED_ONE / 2;
  actor->speedLimit = toFIXED(4);
  actor->acceleration = ((Random16() & 0x7fff) << 1) - (toFIXED(1) / 2 - 1);
  actor->sceneWord = toFIXED(7) / 32;
  actor->update = (s32)AirsRockInside_UpdateVentPuff;
}

/// A task: on two frames in eight, while no menu, Psynergy or item is in use, a
/// vent puffs steam (AirsRockInside_PuffVent). In the Clarity Circlet chest
/// room the west vent puffs, or on odd frames the east one until its pillar is
/// placed; in the face vents room a vent near the leader puffs, the scene's
/// flags passing over a vent a pillar has blocked.
void AirsRockInside_PuffVents(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u32 frame;
  s32 scene;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
      state->item != 0)
    return;
  frame = gFrameCount;
  if ((frame & 6) != 0)
    return;
  scene = gMainRoutineState.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    FIXED x;

    if ((frame & 1) &&
        !GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
      x = toFIXED(376);
    else
      x = toFIXED(280);
    AirsRockInside_PuffVent(x, 0, toFIXED(616));
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
    if (frame & 1) {
      if (FIELD_HALF_CELL(
              Actor_GetById(gMainRoutineState.leaderActorId)->z.fixed) > 41) {
        if (FIELD_HALF_CELL(
                Actor_GetById(gMainRoutineState.leaderActorId)->x.fixed) <= 44)
        puffSouthwestVent:
          AirsRockInside_PuffVent(toFIXED(168), 0, toFIXED(603));
        else if (!GameFlag_Get(FLAG_MIDDLE_WEST_VENT_STILL))
          AirsRockInside_PuffVent(toFIXED(504), 0, toFIXED(456));
        else
          goto puffMiddleEastVent;
      } else {
      puffNorthwestVent:
        AirsRockInside_PuffVent(toFIXED(616), -toFIXED(32), toFIXED(136));
      }
    } else {
      if (FIELD_HALF_CELL(
              Actor_GetById(gMainRoutineState.leaderActorId)->z.fixed) > 41) {
        if (FIELD_HALF_CELL(
                Actor_GetById(gMainRoutineState.leaderActorId)->x.fixed) > 49)
        puffMiddleEastVent:
          AirsRockInside_PuffVent(toFIXED(552), 0, toFIXED(456));
        else if (GameFlag_Get(FLAG_SOUTHEAST_VENT_STILL))
          goto puffSouthwestVent;
        else
          AirsRockInside_PuffVent(toFIXED(264), 0, toFIXED(600));
      } else if (GameFlag_Get(FLAG_NORTHEAST_VENT_STILL)) {
        goto puffNorthwestVent;
      } else {
        AirsRockInside_PuffVent(toFIXED(648), -toFIXED(32), toFIXED(136));
      }
    }
  }
}

/// An actor a gust carries: it rides 8 pixels above the gust, its draw order
/// flickering.
void AirsRockInside_FollowGust(struct Actor *actor) {
  struct Actor *target = actor->watchedActor;

  actor->priorityFlags ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
  SET_ACTOR_POSITION(actor, target->x.fixed, target->y.fixed + toFIXED(8),
                     target->z.fixed);
}

void AirsRockInside_SpiralGust(struct Actor *actor);

/// The actor the gust is cast at: the field effect's target.
#define TARGET(effect) ((effect)->target)

/// The half cell where a gust drops what it carries: x in the high half and z
/// in the low.
#define GUST_DROP(x, z) (((x) / 8 << 16) | (z) / 8)
#define GUST_DROP_X(drop) ((drop) >> 16)

/// The gust picks up an actor it passes over, to drop it at `at`
/// (GUST_DROP).
#define PICK_UP(actor, at)                                                     \
  do {                                                                         \
    (actor)->watchedActor = gust;                                              \
    (actor)->update = AirsRockInside_FollowGust;                               \
    (actor)->motionFlags = 0;                                                  \
    (actor)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;                       \
    Actor_SetSpriteFlags(actor, 0);                                            \
    ((struct FieldSprite *)(actor)->sprite)->priority =                        \
        ((struct FieldSprite *)gust->sprite)->priority;                        \
    gust->watchedActor = (actor);                                              \
    carried = (at);                                                            \
  } while (0)

/// One shake of the walls, with the grinding of a gate.
#define SHAKE()                                                                \
  (Sound_Play(SE_GATE_MOVE), MapGrid_SetShake(5, 5, 1),                        \
   MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9))

/// Whirlwind on a Wind Stone: a gust rises from it and blows away from the
/// leader until a wall or something on the overhead layer stops it, carrying
/// the room's pillar or statue it passes (GUST_DROP). A golden barrier in its
/// way is blown away, and the turning bridge it reaches turns with a shake,
/// unless the Elixir chest room's pillar stands on it.
void AirsRockInside_BlowGust(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct Actor *gust, *spark, *actor;
  struct Vector3 velocity;
  struct EffectOptions glints, dust;
  struct Cell dir, cell;
  s32 i, steps = 0, carried;
  s32 x, z;
  s32 hit = 0;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_RUSH);
  gust = Import_ActorCreate(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                            leader->z.fixed);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = FIXED_ONE * 17 / 10;
  gust->scaleY = FIXED_ONE / 5;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read through a long. */
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->target)->sprite)
          ->priority;
  Actor_SetMoveTarget(gust, TARGET(effect)->x.fixed, TARGET(effect)->y.fixed,
                      TARGET(effect)->z.fixed - toFIXED(1));
  gust->groundHeight = Map_GetTerrainHeight(0, gust->x.fixed, gust->z.fixed);
  while (gust->scaleY < FIXED_ONE * 13 / 10) {
    gust->scaleY += FIXED_ONE / 10;
    FieldEvent_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z.fixed = TARGET(effect)->z.fixed - toFIXED(1);
  while (gust->scaleY < FIXED_ONE * 18 / 10) {
    if ((gFrameCount & 3) == 0) {
      spark =
          Import_ActorCreate(SPRITE_PARTICLE, TARGET(effect)->x.fixed,
                             TARGET(effect)->y.fixed, TARGET(effect)->z.fixed);
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->groundHeight =
          Map_GetTerrainHeight(0, spark->x.fixed, spark->z.fixed);
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(4) / 5;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRockInside_SpiralGust;
    }
    if (gFrameCount & 1) {
      gust->scaleX += FIXED_ONE / 100;
      gust->scaleY += FIXED_ONE / 100;
      Actor_SetSpriteColor(TARGET(effect), SPRITE_COLOR_HIDDEN);
    } else {
      Actor_SetSpriteColor(TARGET(effect), SPRITE_COLOR_GBR);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetSpriteColor(TARGET(effect), SPRITE_COLOR_NORMAL);
  Sound_Play(SE_WHOOSH);
  cell.x = FIELD_CELL(TARGET(effect)->x.fixed);
  cell.z = FIELD_CELL(TARGET(effect)->z.fixed);
  if (FIELD_CELL(leader->x.fixed) < cell.x) {
    dir.x = 1;
    dir.z = 0;
  } else if (FIELD_CELL(leader->x.fixed) > cell.x) {
    dir.x = -1;
    dir.z = 0;
  } else if (FIELD_CELL(leader->z.fixed) < cell.z) {
    dir.x = 0;
    dir.z = 1;
  } else {
    dir.x = 0;
    dir.z = -1;
  }
  do {
    hit = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, cell.x, cell.z);
    hit >>= 8;
    if (Map_GetTerrainHeight(2, FIELD_CELL_ORIGIN(cell.x),
                             FIELD_CELL_ORIGIN(cell.z)) != gust->groundHeight)
      hit = 0;
    cell.x += dir.x;
    cell.z += dir.z;
    steps += 2;
  } while (hit == 0);
  MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, cell.x, cell.z);
  steps -= 2;
  x = FIXEDtoINT(TARGET(effect)->x.fixed) + ((steps * dir.x) << 3);
  z = FIXEDtoINT(TARGET(effect)->z.fixed) + ((steps * dir.z) << 3);
  ((struct FieldSprite *)gust->sprite)->priority = 1;
  /* FAKEMATCH(temporary): cell, a map cell's pair, takes the gust's pixel
   * target; as x and z at the move target. */
  cell.x = x;
  cell.z = z;
  gust->speedLimit = toFIXED(4);
  gust->acceleration = toFIXED(4);
  Actor_SetMoveTarget(gust, INTtoFIXED(cell.x), gust->y.fixed,
                      INTtoFIXED(cell.z));
  FieldCamera_SetSpeedAndAcceleration(5, 5);
  FieldCamera_MoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
  carried = 0;
  while (Actor_HasArrived(gust) == 0) {
    if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
      actor = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR);
      if (FIELD_HALF_CELL(gust->x.fixed) == FIELD_HALF_CELL(actor->x.fixed) &&
          carried == 0)
        PICK_UP(actor, GUST_DROP(312, 296));
    } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
      actor = Actor_GetById(ACTOR_FACE_VENTS_SECOND_PILLAR);
      if (FIELD_CELL(gust->x.fixed) == FIELD_CELL(actor->x.fixed) &&
          FIELD_CELL(gust->z.fixed) == FIELD_CELL(actor->z.fixed) &&
          carried == 0)
        PICK_UP(actor, GUST_DROP(328, 840));
    } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
      actor = Actor_GetById(ACTOR_COINS_CHEST_STATUE);
      if (FIELD_CELL(gust->x.fixed) == FIELD_CELL(actor->x.fixed) &&
          FIELD_CELL(gust->z.fixed) == FIELD_CELL(actor->z.fixed) &&
          carried == 0)
        PICK_UP(actor, GUST_DROP(120, 408));
    }
    if ((gFrameCount & 1) == 0) {
      glints.priority = 1;
      glints.type = SPRITE_GLINT;
      glints.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) * (TRIG_TURN / 16)) << 1;
      velocity.y = gust->y.fixed + INTtoFIXED(Random16() & 31);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) * (TRIG_TURN / 16));
      FieldEffect_Spawn(
          gust->x.fixed, velocity.y, gust->z.fixed, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &glints);
    }
    FieldEvent_Wait(1);
  }
  if (carried != 0) {
    actor = gust->watchedActor;
    actor->x.fixed = FIELD_HALF_CELL_ORIGIN(GUST_DROP_X(carried));
    // The x half shifts out, leaving the z one.
    actor->z.fixed = FIELD_HALF_CELL_ORIGIN(carried);
    actor->velocityY = toFIXED(8);
    actor->update = 0;
    actor->motionFlags = ACTOR_FALLS;
    actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    actor->priorityFlags &=
        ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
    Actor_SetSpriteFlags(actor, 1);
    Scene_SetActorSpritePriority(actor, 2);
    if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
      actor->groundHeight = -toFIXED(32);
      MapGrid_CopyCellAttributes(19, 16, 1, 1, 19, 18);
      GameFlag_Set(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
    } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
      MapGrid_CopyCellAttributes(19, 38, 1, 1, 20, 38);
      Actor_WaitUntilLanded(actor);
      wait(1);
      Actor_WaitUntilLanded(actor);
      actor->motionFlags = 0;
      actor->groundHeight = 0;
      actor->y.fixed = 0;
      MapGrid_SetCellHeight(toFIXED(320), toFIXED(832), 0, 4);
    } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
      Actor_WaitUntilLanded(actor);
      wait(1);
      Actor_WaitUntilLanded(actor);
      Actor_SetSpriteFlags(actor, 0);
      actor->motionFlags = 0;
      actor->groundHeight = 0;
      actor->y.fixed = 0;
      MapGrid_SetCellHeight(toFIXED(120), toFIXED(408), 0, 4);
      if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
        AirsRockInside_WakeStatue(actor, 60, 0);
    }
  }
  if (hit == CELL_BEHAVIOR_WALL) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = FIXEDtoINT(gust->x.fixed) + dir.x * 48;
    cell.z = FIXEDtoINT(gust->z.fixed) + dir.z * 48;
    gust->speedLimit = toFIXED(1) / 2;
    gust->acceleration = toFIXED(1) / 2;
    FieldActor_SetMoveTargetFX(gust, INTtoFIXED(cell.x), gust->y.fixed,
                               INTtoFIXED(cell.z));
    hit -= CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP;
    i = 0;
    while (Actor_HasArrived(gust) == 0) {
      if (hit > GUST_STOP_LAST_BARRIER)
        goto shrink;
      if (i == 0) {
        Sound_Play(SE_GUST);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY = (Random16() & 0xffff) + FIXED_ONE;
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) * (TRIG_TURN / 16)) << 1;
        velocity.y = toFIXED(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) * (TRIG_TURN / 16));
        FieldEffect_Spawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
                          velocity.x, velocity.y, velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
          GameFlag_Set(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
          cell.x = FIELD_CELL(gust->x.fixed) + 64;
          cell.z = FIELD_CELL(gust->z.fixed);
          MapGrid_CopyCellTilesIndicesRect(90, 25, cell.x + 1, cell.z, 1, 3);
          MapGrid_CopyCellTilesIndicesRect(21, 88, 24, 89, 1, 2);
          Map_CopyCellAttributeRect(23, 92, 2, 1, 23, 89);
          Map_CopyCellAttributeRect(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineState.next ==
                   SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
          cell.x = FIELD_CELL(gust->x.fixed);
          cell.z = FIELD_CELL(gust->z.fixed - gust->y.fixed) + 64;
          if (hit == GUST_STOP_PAIRED_STAKES_NORTH_BARRIER) {
            GameFlag_Set(FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY);
            MapGrid_CopyCellTilesIndicesRect(39, 82, cell.x, cell.z, 1, 1);
            Map_CopyCellAttributeRect(39, 82, 1, 1, 38, 82);
            MapGrid_CopyCellAttributes(38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_Set(FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY);
            MapGrid_CopyCellsTo(39, 82, cell.x, cell.z, 1, 1);
            MapGrid_CopyCellAttributes(38, 98, 1, 1, 38, 99);
            MapGrid_CopyCellAttributes(38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineState.next ==
                   SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
          GameFlag_Set(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY);
          cell.x = FIELD_CELL(gust->x.fixed);
          MapGrid_CopyCellsTo(15, 80, cell.x, 77, 1, 3);
          MapGrid_CopyCellAttributes(15, 80, 1, 1, 15, 79);
          MapGrid_CopyCellAttributes(17, 15, 2, 1, 15, 15);
        }
        MapGrid_CenterOnFollowedPoint();
      }
      FieldEvent_Wait(1);
    }
    Sound_Play(SE_STOP_EXIT_SE);
  }
shrink:
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= FIXED_ONE * 3 / 20;
    gust->scaleY -= FIXED_ONE / 4;
    gust->y.fixed += toFIXED(7) / 2;
    FieldEvent_Wait(1);
  }
  Actor_Release(gust);
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    if (hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
      SHAKE();
      MapGrid_CopyCellsTo(79, 64, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      MapGrid_CopyCellsTo(69, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(46, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(32, 89, 1, 1, 31, 89);
      MapGrid_CopyCellAttributes(50, 86, 1, 1, 33, 87);
      GameFlag_Set(FLAG_TIGHTROPES_BRIDGE_TURNED);
    } else if (hit == GUST_STOP_BRIDGE_EAST_WEST) {
      SHAKE();
      MapGrid_CopyCellsTo(74, 64, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      MapGrid_CopyCellsTo(64, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(52, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(33, 88, 1, 1, 33, 87);
      MapGrid_CopyCellAttributes(48, 88, 1, 1, 31, 89);
      GameFlag_Clear(FLAG_TIGHTROPES_BRIDGE_TURNED);
    }
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    if (hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
      SHAKE();
      MapGrid_CopyCellsTo(79, 64, 32, 79, 5, 7);
      if (FIELD_HALF_CELL(Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
          ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
        velocity.x =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed - toFIXED(8);
        velocity.y = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
        velocity.z =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed + toFIXED(8);
        Actor_SetSpritePriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + INTtoFIXED(Random16() & 15),
                            velocity.y + INTtoFIXED(Random16() & 31),
                            velocity.z, 0, 0, -(toFIXED(1) / 2),
                            EFFECT_USE_PRIORITY, &dust);
        }
        wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(64, 64, 32, 79, 5, 7);
        Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_AUTOMATIC;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(69, 64, 32, 79, 5, 7);
        MapGrid_CopyCellAttributes(49, 15, 5, 5, 32, 15);
        MapGrid_CopyCellAttributes(33, 82, 1, 1, 34, 82);
        MapGrid_CopyCellAttributes(52, 81, 1, 1, 36, 81);
        GameFlag_Clear(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
      }
    } else if (hit == GUST_STOP_BRIDGE_EAST_WEST) {
      SHAKE();
      if (FIELD_HALF_CELL(Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
          ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
        velocity.x =
            Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed - toFIXED(8);
        velocity.y = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
        velocity.z = Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          FieldEffect_Spawn(velocity.x + INTtoFIXED(Random16() & 15),
                            velocity.y,
                            velocity.z - INTtoFIXED(Random16() & 31), 0, 0,
                            -(toFIXED(1) / 2), EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        MapGrid_CopyCellsTo(79, 64, 32, 79, 5, 7);
        Sound_Play(SE_GATE_MOVE);
        FieldEvent_Wait(30);
        SHAKE();
        MapGrid_CopyCellsTo(64, 64, 32, 79, 5, 7);
        MapGrid_CopyCellAttributes(49, 21, 5, 5, 32, 15);
        MapGrid_CopyCellAttributes(35, 81, 1, 1, 36, 81);
        MapGrid_CopyCellAttributes(50, 82, 1, 1, 34, 82);
        GameFlag_Set(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
      }
    }
  }
  FieldEvent_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

extern struct SceneEntrance gAirsRockInside_Entrances[];
extern struct SceneEntrance gAirsRockInside_HallEntrances[];

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallEntrances;
  return gAirsRockInside_Entrances;
}

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
    return gAirsRockInside_VialChestPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
    return gAirsRockInside_ClarityCircletChestPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
    return gAirsRockInside_TightropesPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return gAirsRockInside_RevealTabletPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
    return gAirsRockInside_ElixirChestPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
    return gAirsRockInside_PairedStakesPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
    return gAirsRockInside_FaceVentsPlacements;
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
    return gAirsRockInside_CoinsChestPlacements;
  return gAirsRockInside_Placements;
}

/// Nothing visible of its own: once Felix stands still on a new cell, the
/// stepping stone he last stood on is drawn as before; in the Reveal tablet
/// room, leaving a cell with a collision code raises TRIGGER_LEFT_CELL plus the
/// code, which for a vanishing stone is its trigger
/// (TRIGGER_FIRST_VANISHING_STONE on).
void AirsRockInside_RestoreCellBehindLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 key = CELL_KEY(x, z);
  s32 last;
  s32 value;
  struct FieldState *state;

  if (actor->velocityX != 0 || actor->velocityZ != 0)
    return;
  last = gAirsRockInside_StampedCell;
  if (last == -1 || key == last)
    return;
  x = CELL_KEY_X(last);
  z = CELL_KEY_Z(last);
  gAirsRockInside_StampedCell = -1;
  MapGrid_CopyCellsLayered(73, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
  if (gMainRoutineState.next != SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return;
  state = Ram_HeapSlots->fieldState;
  value = CELL_CODE(MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, x, z));
  if (value != 0) {
    state->raisedTrigger = TRIGGER_LEFT_CELL + value;
  }
}

/// Starts AirsRockInside_RestoreCellBehindLeader with no cell stamped yet.
void AirsRockInside_StartRestoringCells(void) {
  s32 value = -1;

  gAirsRockInside_StampedCell = value;
  Task_Add(AirsRockInside_RestoreCellBehindLeader, TASK_PASS_FRAME);
}

/// As Felix steps onto one of the yellow stepping stones marked with a
/// diamond, in the great hall and the rooms around it, the stone is drawn
/// again under his feet, in front of what lies behind it, so that he is seen
/// standing on it; nothing else changes. The stone goes back to its own
/// picture once he has left it (AirsRockInside_RestoreCellBehindLeader).
///
/// The cells under him are saved at (73, 72) and the stone's picture kept at
/// (70, 72) is copied in their place.
void AirsRockInside_StampCellUnderLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 cell = CELL_KEY(x, z);
  s32 scene;

  if (cell == gAirsRockInside_StampedCell)
    return;
  gAirsRockInside_StampedCell = cell;
  gAirsRockInside_StampedWidth = 1;
  gAirsRockInside_StampedHeight = 2;
  scene = gMainRoutineState.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
      scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST ||
      scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    gAirsRockInside_StampedX = x;
    gAirsRockInside_StampedZ = z - y + 64;
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
             scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST ||
             scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES ||
             scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
  }
  MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                           gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight, 73, 72);
  MapGrid_CopyCellsLayered(70, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
}

/// The same for the stepping stones of a second kind, in the hall, the Vial
/// chest room, the Reveal tablet's room and the Elixir chest room: the stone
/// is drawn again under Felix's feet from the picture kept at (71, 72).
void AirsRockInside_StampOtherCellUnderLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 cell = CELL_KEY(x, z);
  s32 scene;

  if (cell == gAirsRockInside_StampedCell)
    return;
  gAirsRockInside_StampedCell = cell;
  gAirsRockInside_StampedWidth = 1;
  gAirsRockInside_StampedHeight = 2;
  scene = gMainRoutineState.next;
  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
      scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    gAirsRockInside_StampedX = x;
    gAirsRockInside_StampedZ = z - y + 64;
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
             scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
  }
  MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                           gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight, 73, 72);
  MapGrid_CopyCellsLayered(71, 72, gAirsRockInside_StampedWidth,
                           gAirsRockInside_StampedHeight,
                           gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
}

/// As Felix steps onto a stepping stone stacked on another, three cells of
/// the picture under him are saved at (73, 72) and the stack's picture kept
/// at (72, 72) is drawn in their place.
#define DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(name, n1)                    \
  void name(void) {                                                            \
    struct Actor *actor = Actor_GetById(Party_GetLeaderActor());               \
    s32 x = FIELD_CELL(actor->x.fixed);                                        \
    s32 y = FIELD_CELL(actor->y.fixed);                                        \
    s32 z = FIELD_CELL(actor->z.fixed);                                        \
    s32 key = CELL_KEY(x, z);                                                  \
                                                                               \
    if (key != gAirsRockInside_StampedCell) {                                  \
      gAirsRockInside_StampedCell = key;                                       \
      gAirsRockInside_StampedWidth = 1;                                        \
      gAirsRockInside_StampedHeight = 3;                                       \
      gAirsRockInside_StampedX = x;                                            \
      gAirsRockInside_StampedZ = z - y + n1;                                   \
      MapGrid_CopyCellsLayered(x, gAirsRockInside_StampedZ, 1, 3, 73, 72);     \
      MapGrid_CopyCellsLayered(                                                \
          72, 72, gAirsRockInside_StampedWidth, gAirsRockInside_StampedHeight, \
          gAirsRockInside_StampedX, gAirsRockInside_StampedZ);                 \
    }                                                                          \
  }

/// As Felix steps onto one of the stacked stepping stones in the great hall,
/// the stack is drawn again under his feet, in front of the wall behind it,
/// so that he is seen standing on top of it; nothing else changes.
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
    AirsRockInside_StampTallCellUnderLeader, 64)

/// The same for the stacked stepping stones one row nearer the screen.
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
    AirsRockInside_StampTallCellUnderLeaderNearer, 63)

/// A burst of eight puffs of dust around x, z, and the walls shake.
void AirsRockInside_BurstDust(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 spawn;
  s32 i;

  options.priority = 1;
  options.startScaleX = FIXED_ONE * 2;
  options.startScaleY = FIXED_ONE * 2;
  options.targetScaleX = FIXED_ONE / 2;
  options.targetScaleY = FIXED_ONE / 2;
  for (i = 0; i != 8; i++) {
    spawn.x = x + INTtoFIXED(Random16() & 63) - toFIXED(32);
    spawn.y = (Random16() & 15) << 12;
    spawn.z = z + INTtoFIXED(Random16() & 7) - toFIXED(16);
    FieldEffect_Spawn(spawn.x, 0, spawn.z, 0, spawn.y, 0,
                      EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PRIORITY | 1,
                      &options);
  }
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// The sparks AirsRockInside_ScatterSparks raises, 5 frames apart.
enum { SCATTERED_SPARKS = 12 };

/// Sparks rise one after another around x, z.
void AirsRockInside_ScatterSparks(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 position;
  s32 i;

  options.type = SPRITE_PARTICLE;
  options.priority = 1;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 3;
  for (i = 0; i != SCATTERED_SPARKS; i++) {
    position.x = x + INTtoFIXED(Random16() & 31) - toFIXED(16);
    position.y = toFIXED(4);
    position.z = z;
    FieldEffect_Spawn(position.x, 0, position.z, 0, position.y, 0,
                      EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PRIORITY,
                      &options);
    FieldEvent_Wait(5);
  }
}

/// A gust's spark: circles the centre kept in bounce and sceneWord, a lap every
/// 32 frames, its radius growing as it rises above the ground.
void AirsRockInside_SpiralGust(struct Actor *actor) {
  FIXED radius = ((actor->y.fixed - actor->groundHeight) >> 1) + toFIXED(16);

  actor->x.fixed =
      actor->bounce +
      Iwram_MulFX(Trig_Cos((actor->speedLimit & 0xff) * (TRIG_TURN / 32)),
                  radius);
  actor->z.fixed =
      actor->sceneWord +
      Iwram_MulFX(Trig_Sin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)),
                  radius) /
          3;
  actor->y.fixed += actor->gravity;
  actor->speedLimit++;
}

/// A spark: circles the centre kept in bounce and sceneWord on an ellipse twice
/// as wide as deep, the radius growing with its height; it rises by its gravity
/// each frame and turns blue (SPRITE_COLOR_BLUE_TINT) and back every two
/// frames.
void AirsRockInside_SpiralSpark(struct Actor *actor) {
  FIXED y = actor->y.fixed;
  s32 phase;
  FIXED radius;

  phase = actor->speedLimit;
  radius = toFIXED(12) + (y >> 2);
  actor->x.fixed =
      actor->bounce +
      Iwram_MulFX(Trig_Cos((phase & 0xff) * (TRIG_TURN / 32)), radius) * 2;
  actor->z.fixed =
      actor->sceneWord +
      Iwram_MulFX(Trig_Sin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)),
                  radius);
  actor->y.fixed += actor->gravity;
  actor->speedLimit++;
  Actor_SetSpriteColor(actor,
                       ((gFrameCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/// A Whirlwind target's update while the gust gathers: it flickers to the
/// colour mode in sceneCounter (a SpriteColorMode), at the rate sceneByte
/// sets.
void AirsRockInside_FlickerTarget(struct Actor *actor) {
  s32 bit = ((gFrameCount & 3) >> actor->sceneByte) & 1;

  Actor_SetSpriteColor(actor, (u8)(bit * actor->sceneCounter));
}

/// Felix casts Whirlwind on the twister rock in the great hall: it glows, a
/// ball of purple light flies north up the hall to the great stone face,
/// whose eyes light red as its mouth opens with a rumble and rocks fall, and
/// from then on its statues breathe whirlwinds the party can ride. The rock
/// turns grey, spent.
void AirsRockInside_WakeHallStatue(void) {
  u16 *buffer = Heap_Alloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK),
             BG_SCREEN_SIZE);
  buffer = Heap_Alloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
  cell = buffer;
  // The map's tile numbers less 512.
  for (i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
    *cell &= 0xfcff;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != LIGHTNING_MAP_ROWS; i++) {
    Dma_Set(
        cell,
        (void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) + i * MAP_ROW_BYTES),
        DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4), REG_DMA3);
    cell += LIGHTNING_MAP_COLUMNS;
  }
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->sceneCounter = SPRITE_COLOR_BLUE_TINT;
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->sceneByte = 1;
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->update = AirsRockInside_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(
          SPRITE_PARTICLE, Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->x.fixed,
          Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->y.fixed,
          Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRockInside_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeedFX(toFIXED(1), toFIXED(1) / 8);
  FieldCamera_MoveToFX(toFIXED(312), -1, toFIXED(680), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(ACTOR_HALL_TWISTER_ROCK)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_ROCK),
                       SPRITE_COLOR_GREY);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = -104;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_SetSource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellTilesIndicesRect(64, 78, 18, 105, 3, 3);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  for (i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
    wait(1);
  }
  PaletteFade_SetSource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  AirsRockInside_BurstDust(toFIXED(312), toFIXED(720));
  MapGrid_CopyCellsTo(64, 78, 18, 105, 3, 5);
  Camera_SetSpeedParameters(toFIXED(3) / 2, toFIXED(3) / 16);
  FieldCamera_MoveToActorById(ACTOR_HALL_TWISTER_STATUE, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetSpriteColorById(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_BLINK);
  AirsRockInside_ScatterSparks(
      Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->x.fixed,
      Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->z.fixed);
  Actor_ConfigureSpriteColor(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE),
                       SPRITE_COLOR_WHITE);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_WEST_STATUE),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_EAST_STATUE),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE),
                       SPRITE_COLOR_NORMAL);
  wait(40);
  AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineState.windowHue,
                    gMainRoutineState.windowBrightness);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN);
  Event_End();
}

/// Walking north into the door at the top of the hall's tower: it opens with a
/// thump, and the leader walks through into the Reveal tablet's room.
void AirsRockInside_OpenTabletRoomDoor(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 width;
  s32 height;

  if (CELL_CODE(MapCell_GetAttributes(MAPGRID_LAYER_MIDDLE,
                                      FIELD_CELL(leader->x.fixed),
                                      FIELD_CELL(leader->z.fixed) - 1)) !=
      CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_THUMP);
  width = 1;
  height = 2;
  MapGrid_CopyCellTilesIndicesRect(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellTilesIndicesRect(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellTilesIndicesRect(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellTilesIndicesRect(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = 0;
  Actor_GetById(gMainRoutineState.leaderActorId)->speedLimit =
      Actor_GetById(gMainRoutineState.leaderActorId)->acceleration =
          toFIXED(1) / 2;
  Sound_Play(SE_MAP_EXIT);
  Actor_WalkToById(gMainRoutineState.leaderActorId, 312, 792);
  FieldEvent_Wait(5);
  Event_SetExitRequest(AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_REVEAL_TABLET);
  Event_End();
}

/// Nothing visible happens of its own as Felix starts to push a pillar or a
/// statue inside the rock, or casts Move on one; the game takes it off the
/// cells it was blocking so that it can slide. In the paired statues room it
/// also smooths two cells' ground, and in the coins chest room the statue
/// being moved stops breathing whirlwinds.
void AirsRockInside_StartPushingPillar(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL) {
    Actor_LiftTargetBlock(gAirsRockInside_HallPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    Actor_LiftTargetBlock(gAirsRockInside_VialChestPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    Actor_LiftTargetBlock(gAirsRockInside_ClarityCircletChestPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    Actor_LiftTargetBlock(gAirsRockInside_TightropesPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
    FieldMapGrid_SetCellHeight(toFIXED(608), toFIXED(336), 2, 0);
    MapGrid_SetCellHeight(toFIXED(624), toFIXED(336), 2, 0);
    Actor_LiftTargetBlock(gAirsRockInside_PairedStakesPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
    Actor_LiftTargetBlock(gAirsRockInside_FaceVentsPillars);
  } else if (scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    Actor_GetById(ACTOR_COINS_CHEST_STATUE)->timer = 0;
    Actor_LiftTargetBlock(gAirsRockInside_CoinsChestPillars);
  }
}

/// Once Felix has pushed or moved a pillar or a statue inside the rock: one
/// left over lower ground drops onto it with a thump, and one left in the
/// place it belongs stays there on later visits, as a shortcut or a step.
///
/// In the paired statues' room and the face vents room the cells along the
/// pillar's rows are marked or cleared again, which in the face vents room
/// decides which vents it keeps still.
void AirsRockInside_FinishPushingPillar(void) {
  struct Actor *pushed =
      ((struct FieldState *)Ram_HeapSlots->fieldState)->movedBlock;
  s32 x, z;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_PlaceMovedBlock();
  if (Map_GetTerrainHeight(2, pushed->x.fixed, pushed->z.fixed) !=
      pushed->y.fixed) {
    pushed->layer = 2;
    pushed->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_WaitUntilLanded(pushed);
    Sound_Play(SE_THUMP);
    Actor_WaitUntilLanded(pushed);
    pushed->motionFlags = 0;
    pushed->layer = 0;
  }
  x = FIELD_HALF_CELL(pushed->x.fixed);
  z = FIELD_HALF_CELL(pushed->z.fixed);
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
    if (x == 103 && z == 41)
      GameFlag_Set(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    if (x == 13 && z == 49)
      GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED);
    else if (x == 41 && z == 49)
      GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED);
  } else if (gMainRoutineState.next ==
             SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    if (x == 33 && z == 79)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED);
    else if (x == 77 && z == 71)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED);
    else if (x == 59 && z == 65)
      GameFlag_Set(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    if (x == 81 && z == 37)
      GameFlag_Set(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
    FieldMapGrid_SetCellHeight(toFIXED(608), toFIXED(336), 2, -4);
    FieldMapGrid_SetCellHeight(toFIXED(624), toFIXED(336), 2, -4);
    if (x == 43 && z == 87)
      GameFlag_Set(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED);
    if ((FIELD_HALF_CELL(
             Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_A)->x.fixed) == 73 &&
         FIELD_HALF_CELL(
             Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_A)->z.fixed) == 43) ||
        (FIELD_HALF_CELL(
             Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_B)->x.fixed) == 73 &&
         FIELD_HALF_CELL(
             Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_B)->z.fixed) == 43))
      Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneByte = 1;
    else
      Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneByte = 0;
    if (pushed->x.fixed ==
            Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_C)->x.fixed &&
        pushed->z.fixed ==
            Actor_GetById(ACTOR_PAIRED_STAKES_PILLAR_C)->z.fixed) {
      if (x == 83 && z == 43)
        Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneByte = 1;
      else
        Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneByte = 0;
    }
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
    if (x == 35 && z == 59)
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED);
    if (x == 45 && z == 105)
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED);
    if (x == 23 && z == 77) {
      GameFlag_Set(FLAG_SOUTHEAST_VENT_STILL);
      GameFlag_Set(FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED);
    }
    if (z == 59 && (x == 67 || x == 71 || x == 73))
      GameFlag_Set(FLAG_MIDDLE_WEST_VENT_STILL);
    if (z == 19 && (x == 75 || x == 79))
      GameFlag_Set(FLAG_NORTHEAST_VENT_STILL);
    if (x == 69 && z == 59)
      GameFlag_Clear(FLAG_MIDDLE_WEST_VENT_STILL);
    if (x == 77 && z == 19)
      GameFlag_Clear(FLAG_NORTHEAST_VENT_STILL);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed), 0);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed), 0);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    if (x == 45 && z == 33)
      GameFlag_Set(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
  }
  Event_End();
}

/// Felix casts Whirlwind on the twister rock in the Vial chest room: it
/// glows, a ball of purple light flies north to the room's great stone face,
/// whose eyes light red and whose mouth opens with a roar (the cells from
/// (12, 77) down), and the statue below it wakes and starts breathing
/// whirlwinds. The rock turns grey, spent.
void AirsRockInside_WakeVialChestStatue(void) {
  u16 *buffer = Heap_Alloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Io_Write16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK),
             BG_SCREEN_SIZE);
  buffer = Heap_Alloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
  cell = buffer;
  // The map's tile numbers less 512.
  for (i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
    *cell &= 0xfcff;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != LIGHTNING_MAP_ROWS; i++) {
    Dma_Set(
        cell,
        (void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) + i * MAP_ROW_BYTES),
        DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4), REG_DMA3);
    cell += LIGHTNING_MAP_COLUMNS;
  }
  Heap_Release(buffer);
  Sound_Play(SE_GLITTER);
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneCounter =
      SPRITE_COLOR_BLUE_TINT;
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneByte = 1;
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update =
      AirsRockInside_FlickerTarget;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = Import_ActorCreate(
          SPRITE_PARTICLE,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->x.fixed,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->y.fixed,
          Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->z.fixed);
      ((struct FieldSprite *)spark->sprite)->priority =
          ((struct FieldSprite *)Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)
               ->sprite)
              ->priority;
      spark->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      spark->motionFlags = 0;
      spark->bounce = spark->x.fixed;
      spark->gravity = toFIXED(3) / 2;
      spark->sceneWord = spark->z.fixed;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gFieldEffect_MediumScript);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
      spark->update = AirsRockInside_SpiralSpark;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeedFX(toFIXED(1), toFIXED(1) / 8);
  FieldCamera_MoveToFX(toFIXED(216), -1, toFIXED(224), 1);
  FieldCamera_WaitForArrival();
  Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update = 0;
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK),
                       SPRITE_COLOR_GREY);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = -104;
  gBgScroll[0].y = 42;
  wait(30);
  Sound_Play(SE_ITEM_BREAK);
  PaletteFade_ApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_SetSource(RGB_WHITE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellTilesIndicesRect(68, 84, 12, 77, 3, 3);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  for (i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
    wait(1);
  }
  PaletteFade_SetSource(PALETTE_TINT_WARM, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  wait(30);
  Sound_Play(SE_BOSS_ROAR);
  AirsRockInside_BurstDust(toFIXED(216), toFIXED(301));
  MapGrid_CopyCellsTo(68, 84, 12, 77, 3, 6);
  Camera_SetSpeedParameters(toFIXED(3) / 2, toFIXED(3) / 16);
  FieldCamera_MoveToActorById(ACTOR_VIAL_CHEST_STATUE, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetSpriteColorById(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_BLINK);
  AirsRockInside_ScatterSparks(Actor_GetById(ACTOR_VIAL_CHEST_STATUE)->x.fixed,
                               Actor_GetById(ACTOR_VIAL_CHEST_STATUE)->z.fixed +
                                   toFIXED(32));
  Actor_ConfigureSpriteColor(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE),
                       SPRITE_COLOR_WHITE);
  wait(3);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE),
                       SPRITE_COLOR_NORMAL);
  wait(40);
  AirsRockInside_WakeStatue(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 8, 0);
  Window_LoadGraphics();
  DebugText_Clear();
  WindowColor_Apply(gMainRoutineState.windowHue,
                    gMainRoutineState.windowBrightness);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  GameFlag_Set(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN);
  Event_End();
}

/// Walking south off the Vial chest room's ledge: the leader jumps down, to
/// land in the hall below (AirsRockInside_LandInHall).
void AirsRockInside_JumpDownLedge(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_CreateSilhouette(toFIXED(216), -toFIXED(32), toFIXED(544),
                         SPRITE_CRATE);
  Actor_JumpDownLedge(gMainRoutineState.leaderActorId, ANIM_LEADER_SPRING, 0);
  Event_SetExitRequest(
      AIRS_ROCK_INSIDE_VIAL_CHEST_TO_AIRS_ROCK_INSIDE_HALL_SOUTH);
  Event_End();
}

/// A puff of steam from one of the vents in the Clarity Circlet chest room
/// catches Felix north of the vents and, with a chime, blows him back south
/// to cell (23, 41) in front of them, without turning him round. South of
/// them the puff does nothing.
void AirsRockInside_RideClarityCircletVent(void) {

  if (Actor_GetById(gMainRoutineState.leaderActorId)->z.part[0].integer > 653)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeedParameters(gMainRoutineState.leaderActorId, toFIXED(4),
                           toFIXED(2));
  Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId, 376, 668);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Event_End();
}

/// A stepping stone the leader has crossed fades with a drip and is gone.
void AirsRockInside_VanishStone(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(SPRITE_STACKED_CRATES, INTtoFIXED(x), 0,
                              INTtoFIXED(y));
  REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSpriteFlags(marker, 0);
  sprite = marker->sprite;
  sprite->objectMode = OBJ_MODE_WINDOW;
  Event_Start();
  Event_EndLastingPsynergy(0);
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
               WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Sound_Play(SE_DRIP);
  for (i = 0; i != BLEND_WEIGHT_MAX / 2; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, i * 2);
    wait(1);
  }
  for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
    wait(1);
  }
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  MapGrid_CopyCellsTo(65, 1, cellX + 64, cellY, 1, 2);
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, cellX, cellY,
                        CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
  Actor_Release(marker);
  Event_End();
  REG_BLDCNT = 0;
  REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

/// Felix steps off one of the stepping stones that cross the Reveal tablet's
/// room, and it fades away behind him with a drip, leaving a gap, for good.
/// Stepping from a stone back onto the floor at either end of the row, the
/// stone goes only once another already has.
///
/// The floor at the ends is read as
/// CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END and _NORTH_END on the
/// overhead layer; FLAG_STONE_VANISHED says a stone has gone, and each stone
/// has its own flag.
void AirsRockInside_StepOffVanishingStone(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  cell =
      MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(leader->x.fixed),
                            FIELD_CELL(leader->z.fixed));
  // The stone's row in half cells: the first's is 50, each next four north.
  row -= (state->raisedTrigger - TRIGGER_FIRST_VANISHING_STONE + 1) << 2;
  cell = CELL_CODE(cell);
  if (cell != CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END &&
      cell != CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END) {
    AirsRockInside_StampCellUnderLeader();
    AirsRockInside_VanishStone(264, row * 8 + 10);
    if (!Event_GetFlag(FLAG_STONE_VANISHED))
      GameFlag_Set(FLAG_STONE_VANISHED);
    GameFlag_Set(state->raisedTrigger - TRIGGER_FIRST_VANISHING_STONE +
                 FLAG_FIRST_STONE_VANISHED);
  } else if (GameFlag_Get(FLAG_STONE_VANISHED)) {
    AirsRockInside_VanishStone(264, row * 8 + 10);
  }
}

/// As Felix steps onto one of the stepping stones on row 19 of the Reveal
/// tablet's room, beside its two hidden doorways, it lights up gold under his
/// feet, and goes back to yellow once he has left it.
///
/// Two cells of the overhead layer under him are saved at (73, 72) and the
/// lit stone's picture kept at (70, 74) is drawn in their place.
void AirsRockInside_StampWideCellUnderLeader(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 y = FIELD_CELL(actor->y.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 key = CELL_KEY(x, z);

  if (key != gAirsRockInside_StampedCell) {
    gAirsRockInside_StampedCell = key;
    gAirsRockInside_StampedWidth = 1;
    gAirsRockInside_StampedHeight = 2;
    gAirsRockInside_StampedX = x + 64;
    gAirsRockInside_StampedZ = z - y;
    MapGrid_CopyCellsLayered(gAirsRockInside_StampedX, gAirsRockInside_StampedZ,
                             1, 2, 73, 72);
    MapGrid_CopyCellsLayered(
        70, 74, gAirsRockInside_StampedWidth, gAirsRockInside_StampedHeight,
        gAirsRockInside_StampedX, gAirsRockInside_StampedZ);
  }
}

extern u32 gOrbitSpark_Script[];

/// Defines a routine that sets the Reveal tablet room's two hidden doorways,
/// at cells (8, 19) and (24, 19), from the spare cells of row 17: their ground
/// from column groundColumn, their pictures from column pictureColumn.
#define DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(name, groundColumn, pictureColumn)   \
  void name(void) {                                                            \
    Map_CopyCellAttributeRect(groundColumn, 17, 1, 1, 8, 19);                  \
    Map_CopyCellAttributeRect(groundColumn, 17, 1, 1, 24, 19);                 \
    MapGrid_CopyCellsTo(pictureColumn, 17, 72, 19, 1, 2);                      \
    MapGrid_CopyCellTilesIndicesRect(pictureColumn, 17, 88, 19, 1, 2);         \
  }

/// Nothing visible of its own: as Reveal fades, the Reveal tablet room's two
/// hidden doorways, at cells (8, 19) and (24, 19), are walls again.
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(AirsRockInside_HideRevealedDoors, 26, 90)

/// Felix casts Reveal in the Reveal tablet's room, and two hidden doorways
/// show in its walls, at cells (8, 19) and (24, 19), for as long as it
/// lasts.
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(AirsRockInside_RevealDoors, 25, 89)

#include "shared/orbit_spark_spawn.h"

/// An orbiter's update: circles 8 pixels above its anchor, closing in 4 pixels
/// a step and leaving sparks, until its countdown ends; 1 once it is 2 pixels
/// out.
s32 AirsRockInside_OrbitAnchor(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s16)orbiter->speed >> 1;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  FieldVector_AddPolarOffsetFX(orbiter->orbitRadius, orbiter->angle,
                               (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->orbitRadius == toFIXED(2))
    return 1;
  OrbitSpark_Spawn(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  orbiter->angle += (s16)orbiter->speed >> 1;
  orbiter->x = anchor->x.fixed;
  orbiter->y = anchor->y.fixed + toFIXED(8);
  orbiter->z = anchor->z.fixed;
  Vector_AddPolarOffset(orbiter->orbitRadius, orbiter->angle, &orbiter->x);
  OrbitSpark_Spawn(orbiter);
  orbiter->orbitRadius -= toFIXED(4);
  return 1;
}

#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_sparkles.h"

extern u32 gLesson_TabletRiseScript[];
extern u32 gAirsRockInside_RisingSparkScript[];
extern s32 gAirsRockInside_OrbitingSparkScript[];
extern u32 gLesson_AdeptRiseScript[];
extern s32 gLesson_SeekingSparkScript[];
extern s32 gLesson_TabletSettleScript[];
extern s32 gLesson_AdeptSettleScript[];

/// The sparks that rise from the tablet as it lifts.
enum { TABLET_SPARKS = 16 };
/// The sparks that spiral in on Sheba, one set out every ORBITING_SPARK_FRAMES.
enum { ORBITING_SPARKS = 16, ORBITING_SPARK_FRAMES = 8 };
/// The sparks that then close in on her start every 2 pixels up her, each
/// circling out as far as the sine of its height in 64ths of a turn
/// (Orbit_Update's phase), short of SEEKING_SPARK_PHASES, half a turn, where
/// the sine is 0 again.
enum { SEEKING_SPARK_PHASES = 32 };

/// The tablet rises in a rumble, sparks circle Sheba and she learns Reveal.
void AirsRockInside_ShebaLearnsReveal(s32 tabletId, s32 shebaId) {
  struct Actor *tablet = Actor_GetById(tabletId);
  struct Actor *sheba = Actor_GetById(shebaId);
  s32 i;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  s32 unused[3];

  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(30);
  Camera_SetSpeedParameters(toFIXED(1) / 2, toFIXED(1) / 16);
  FieldCamera_MoveWithinMapBounds(INTtoFIXED(tablet->x.part[0].integer),
                                  INTtoFIXED(tablet->y.part[0].integer),
                                  INTtoFIXED(tablet->z.part[0].integer), 1);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 2, 1);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  tablet->motionFlags = 0;
  tablet->acceleration = toFIXED(1) / 5;
  tablet->speedLimit = toFIXED(1) / 5;
  Actor_SetMovementTypeById(tabletId, gLesson_TabletRiseScript);
  Sound_Play(SE_TABLET_RISE);
  FieldEvent_Wait(45);
  Actor_ConfigureSpriteColor(tabletId, SPRITE_COLOR_BLINK);
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  for (i = 0; i != TABLET_SPARKS; i++) {
    struct Actor *spark = Import_ActorCreate(SPRITE_PARTICLE, tablet->x.fixed,
                                             tablet->y.fixed, tablet->z.fixed);
    Sound_Play(SE_GLITTER);
    spark->x.fixed += INTtoFIXED((Random16() & 31) - 16);
    spark->y.fixed += INTtoFIXED((Random16() & 15) - 8);
    spark->speedLimit = toFIXED(8);
    spark->acceleration = toFIXED(1) / 5;
    spark->motionFlags = 0;
    Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gAirsRockInside_RisingSparkScript);
    REG_BLDALPHA = BLEND_ALPHA(4, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);
    wait(2);
    REG_BLDALPHA = BLEND_ALPHA(10, 16);
    wait(2);
  }
  REG_BLDALPHA = BLEND_ALPHA(8, 16);
  wait(30);
  for (i = 0; i != ORBITING_SPARKS; i++) {
    struct Actor *spark = FieldActor_Create(SPRITE_PARTICLE, sheba->x.fixed,
                                            sheba->y.fixed, sheba->z.fixed);
    Sound_Play(SE_SPARKS);
    spark->timer = (Random16() & 0x7fff) + DEGtoANG(180);
    spark->userValue = ((i & 1) * 2 - 1) << ((Random16() & 3) + 9);
    spark->motionFlags = 0;
    spark->sceneWord = toFIXED(122);
    spark->watchedActor = sheba;
    // All spirals end together, 15 frames after the loop's last wait.
    spark->sceneCounter = (ORBITING_SPARKS - i) * ORBITING_SPARK_FRAMES + 15;
    Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
    Scene_SetActorSpritePriority(spark, 1);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    FieldVector_AddPolarOffsetFX(spark->sceneWord, spark->timer,
                                 (s32)&spark->x.fixed);
    Actor_StartScript(spark, gAirsRockInside_OrbitingSparkScript);
    if (i == 3) {
      Actor_SetSpriteColorById(shebaId, SPRITE_COLOR_BLINK);
      sheba->motionFlags = 0;
      sheba->acceleration = toFIXED(1) / 5;
      sheba->speedLimit = toFIXED(1) / 5;
      Actor_SetMovementTypeById(shebaId, gLesson_AdeptRiseScript);
    }
    wait(ORBITING_SPARK_FRAMES);
  }
  Sound_Play(SE_SHIMMER);
  wait(16);
  for (i = 2; i != SEEKING_SPARK_PHASES; i += 2) {
    struct Actor *spark = FieldActor_Create(
        SPRITE_PARTICLE, sheba->x.fixed,
        sheba->y.fixed + toFIXED(i) - toFIXED(8), sheba->z.fixed);
    spark->timer = Random16();
    spark->userValue = TRIG_TURN / 32;
    spark->sceneCounter = i;
    spark->sceneByte = 1;
    spark->bounce = spark->x.fixed;
    spark->sceneWord = 0;
    spark->gravity = spark->z.fixed;
    spark->watchedActor = sheba;
    spark->motionFlags = 0;
    spark->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteColor(spark, SPRITE_COLOR_RBG);
    Actor_SelectAnimation(spark, 7);
    Actor_SetSpriteFlags(spark, 0);
    Actor_SetBlendMode(spark, 1);
    Actor_StartScript(spark, gLesson_SeekingSparkScript);
  }
  Sound_Play(SE_SHIMMER);
  wait(50);
  PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(tabletId, SPRITE_COLOR_NORMAL);
  Actor_ConfigureSpriteColor(shebaId, SPRITE_COLOR_NORMAL);
  Sound_Play(SE_FLASH);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(80);
  Actor_SetMovementTypeById(tabletId, gLesson_TabletSettleScript);
  Actor_SetMovementTypeById(shebaId, gLesson_AdeptSettleScript);
  sheba->update = Actor_EmitSparkles;
  wait(120);
  sheba->update = 0;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Scene_PlayMusic();
  wait(1);
}

#include "text/msg_ids.h"

s32 Actor_PlaceOnActor();

/// Checking the tablet shows its message; the first time
/// (FLAG_REVEAL_TABLET_READ) Sheba learns Reveal, as Magma Rock's tablet does
/// for Blaze.
void AirsRockInside_ReadRevealTablet(void) {
  struct Actor *actor;

  Event_Start();
  Event_EndLastingPsynergy(0);
  // "Felix checked the tablet. It has strange characters written on it."
  // "Wielder of Wind's might... Lay your hands upon this stone."
  // "We bestow upon thee the power to see the truth unclouded..."
  FieldMessage_Show(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
  if (Event_GetFlag(FLAG_REVEAL_TABLET_READ) != 0) {
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgCheckedTheTablet);
    gFieldState->message++;
    Actor_SetSpeedFX(gMainRoutineState.leaderActorId, toFIXED(7) / 10,
                     toFIXED(7) / 20);
    Actor_SetSpeedFX(ACTOR_SHEBA, toFIXED(7) / 10, toFIXED(7) / 20);
    Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineState.leaderActorId, 256, 136);
    Actor_WalkToById(ACTOR_SHEBA, 272, 136);
    Actor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Actor_WaitForMoveById(ACTOR_SHEBA);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_EAST, 0);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_WEST, 0);
    FieldEvent_Wait(10);
    // "Do you think I ought to touch it?"
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 256, 152);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    FieldEvent_Wait(30);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(ACTOR_SHEBA, 264, 137);
    Actor_FaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    // "Sheba touched the tablet."
    FieldMessage_Show(MsgCheckedTheTablet + 2, FIELD_MESSAGE_ZOOM);
    MapGrid_SetShake(5, 7, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_WaitForShakeToSettle();
    AirsRockInside_ShebaLearnsReveal(ACTOR_REVEAL_TABLET, ACTOR_SHEBA);
    Party_LearnAbility(SHEBA, ABILITY_REVEAL);
    GameFlag_Set(FLAG_REVEAL_TABLET_READ);
    Actor_TurnToFaceEachOther(gMainRoutineState.leaderActorId, ACTOR_SHEBA, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_SetSpeedFX(ACTOR_SHEBA, toFIXED(6) / 5, toFIXED(3) / 5);
    Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    if (actor != 0)
      Actor_SetDestinationById(ACTOR_SHEBA, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_SHEBA);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_SHEBA, 0, 0);
    Actor_SetMovementType(Actor_GetById(ACTOR_REVEAL_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
    Event_End();
  }
}

/// Felix examines the Reveal tablet from the south: read already,
/// it says so; otherwise he walks round to its front and Sheba reads it.
void AirsRockInside_WalkToRevealTablet(void) {
  if (GameFlag_Get(FLAG_REVEAL_TABLET_READ)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    FieldMessage_Show(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
    Event_End();
  } else {
    Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 248, 107);
    Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 248, 136);
    Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 264, 136);
    Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    AirsRockInside_ReadRevealTablet();
  }
}

/// Defines a routine for examining the Reveal tablet from one side: once it
/// has been read the game only says so; before, Felix walks from sideX round
/// to its front, faces it, and Sheba reads it.
#define DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(name, sideX)                       \
  void name(void) {                                                            \
    if (GameFlag_Get(FLAG_REVEAL_TABLET_READ)) {                               \
      Event_Start();                                                           \
      Event_EndLastingPsynergy(0);                                             \
      FieldMessage_Show(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);              \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, sideX, 136);    \
      Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 264, 136);      \
      Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH,   \
                              0);                                              \
      FieldEvent_Wait(10);                                                     \
      AirsRockInside_ReadRevealTablet();                                       \
    }                                                                          \
  }

/// Felix examines the Reveal tablet from its east side, facing west: he walks
/// round to its front and Sheba reads it, as from the south
/// (AirsRockInside_WalkToRevealTablet).
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(AirsRockInside_ReadTabletFacingWest, 280)

/// Felix examines the Reveal tablet from its west side, facing east: the same.
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(AirsRockInside_ReadTabletFacingEast, 248)

/// Nothing happens as Felix walks west into the ledge of the Reveal tablet's
/// room.
void AirsRockInside_IgnoreStep(void) {}

/// Felix casts Move on the pillar in the Elixir chest room; nothing more than
/// the move shows, but the game remembers whether it now stands on the
/// turning bridge, which then cannot turn under a gust.
void AirsRockInside_MoveElixirChestPillar(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (FIELD_HALF_CELL(Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
      ELIXIR_CHEST_PILLAR_ON_BRIDGE_X)
    GameFlag_Set(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
  else
    GameFlag_Clear(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
  Event_End();
}

/// Felix casts Pound on one of the four stakes in the paired stakes room: it
/// sinks into the floor and the other stake of its pair rises
/// (AirsRockInside_PoundStake).
void AirsRockInside_PoundPairedStakesStake(s32 mode, s32 step) {
  AirsRockInside_PoundStake(mode, step);
}

extern u16 gAirsRockInside_PairedStakesStakes[];
void AirsRockInside_SetStakeCells(struct Actor *actor);

/// Stepping onto a raised stake's top: unless a pillar stands on its pair, the
/// stake sinks under the leader and its partner rises (as
/// AirsRockInside_PoundStake draws it).
void AirsRockInside_StepOntoStake(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  pair = gAirsRockInside_PairedStakesStakes;
  if (*pair != ACTOR_LIST_END) {
    do {
      id = *pair;
      first = Actor_GetById(id);
      if (FIELD_CELL(first->x.fixed) == FIELD_CELL(leader->x.fixed) &&
          FIELD_CELL(first->z.fixed) == FIELD_CELL(leader->z.fixed)) {
        second = Actor_GetById(id - ((id & 1) << 1) + 1);
        break;
      }
      pair += 3;
    } while (*pair != ACTOR_LIST_END);
  }
  busy = first->sceneByte | second->sceneByte;
  if (busy != 0)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  firstSprite = first->sprite;
  secondSprite = second->sprite;
  second->motionFlags = busy;
  second->y.fixed -= toFIXED(28);
  wait(1);
  entry = &gVramCacheEntries[secondSprite->slot];
  second->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
  entry = &gVramCacheEntries[firstSprite->slot];
  first->watchedActor = (void *)(entry->offset + OBJ_VRAM0);
  Dma_Fill32(busy, second->watchedActor, entry->size);
  Dma_Set(first->watchedActor, second->watchedActor,
          DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4), REG_DMA3);
  first->watchedActor =
      (void *)((u8 *)first->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
  second->watchedActor =
      (void *)((u8 *)second->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
  leader->motionFlags = busy;
  for (i = 0; i != 8; i++) {
    if (i <= 4) {
      second->y.fixed += toFIXED(26) / 5;
      Dma_Set(first->watchedActor, second->watchedActor,
              DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
      first->watchedActor =
          (void *)((u8 *)first->watchedActor + STAKE_ROW_BYTES);
      second->watchedActor =
          (void *)((u8 *)second->watchedActor + STAKE_ROW_BYTES);
    }
    leader->y.fixed += -(toFIXED(7) / 2);
    leader->groundHeight = leader->y.fixed;
    first->scaleY += -(FIXED_ONE * 31 / 256);
    wait(1);
  }
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  first->scaleY = FIXED_ONE;
  second->y.fixed = second->groundHeight;
  GameFlag_Set(first->timer);
  GameFlag_Clear(second->timer);
  Actor_SelectAnimation(first,
                        ((struct FieldSprite *)first->sprite)->lastNo + 1);
  Actor_SelectAnimation(second,
                        ((struct FieldSprite *)second->sprite)->lastNo - 1);
  AirsRockInside_SetStakeCells(first);
  AirsRockInside_SetStakeCells(second);
  FieldEvent_Wait(10);
  Event_End();
}

/// A vent's puff has reached the leader in the face vents room: standing in
/// front of a vent, he is blown down to the place below it with a chime.
void AirsRockInside_RideFaceVent(void) {
  s16 *vent;
  s32 x, z;

  x = 0;
  z = 0;
  vent = &gFieldState->stepTrigger;
  if (*vent == 0)
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_SetSpeedParameters(gMainRoutineState.leaderActorId, toFIXED(4),
                           toFIXED(2));
  switch (*vent) {
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT:
    // DEAD: the leader looked up twice, both results unused; the ROM keeps
    // both calls.
    Actor_GetById(gMainRoutineState.leaderActorId);
    Actor_GetById(gMainRoutineState.leaderActorId);
    x = 148;
    z = 632;
    break;
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHEAST_VENT:
    x = 264;
    z = 652;
    break;
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_WEST_VENT:
    x = 504;
    z = 508;
    break;
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_EAST_VENT:
    x = 552;
    z = 508;
    break;
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHWEST_VENT:
    x = 616;
    z = 188;
    break;
  case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHEAST_VENT:
    x = 648;
    z = 188;
    break;
  }
  Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId, x, z);
  Actor_GetById(gMainRoutineState.leaderActorId)->steeringFlags |=
      ACTOR_FACES_MOTION;
  Event_End();
}

/// Checking the Psynergy Stone restores the party's PP.
void AirsRockInside_CheckPsynergyStone(void) {
  const u8 *message;

  Event_Start();
  Event_EndLastingPsynergy(0);
  message = MsgCheckedThePsynergy;
  // "Felix checked the Psynergy Stone."
  FieldMessage_Show(message, FIELD_MESSAGE_ZOOM);
  Sound_Play(SE_RECOVERY);
  Party_AddPp(PARTY_PP_REPLENISH, 0);
  FieldEvent_Wait(10);
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  // "Everyone's PP was replenished!"
  FieldMessage_Show(message + 1, FIELD_MESSAGE_ZOOM);
  Event_End();
}

/// The trigger table for the current room.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_AIRS_ROCK_INSIDE_HALL)
    return gAirsRockInside_HallTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
    return gAirsRockInside_VialChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
    return gAirsRockInside_ClarityCircletChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
    return gAirsRockInside_TightropesTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
    return gAirsRockInside_RevealTabletTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
    return gAirsRockInside_ElixirChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
    return gAirsRockInside_PairedStakesTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
    return gAirsRockInside_FaceVentsTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
    return gAirsRockInside_CoinsChestTriggers;
  if (scene == SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE)
    return gAirsRockInside_PsynergyStoneTriggers;
  return gAirsRockInside_VialChestTriggers;
}

/// The leader rises 80 pixels on a lift into the room.
void AirsRockInside_RiseOnLift(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *prepared, *lift;
#if TLA_PATCH_1
  u8 priorityFlags;
#endif
  s32 height;

  Event_Start();
  Event_EndLastingPsynergy(0);
#if TLA_PATCH_1
  priorityFlags = leader->priorityFlags;
#endif
  prepared = Actor_CreateSilhouette(toFIXED(216), -toFIXED(32), toFIXED(544),
                                    SPRITE_CRATE);
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(leader, 0);
  leader->speedLimit = toFIXED(3) / 2;
  leader->acceleration = toFIXED(3) / 2;
  lift = Import_ActorCreate(SPRITE_WHIRLWIND, leader->x.fixed,
                            leader->y.fixed - toFIXED(8), leader->z.fixed);
  Actor_SelectAnimation(lift, 2);
  Actor_SetSpriteFlags(lift, 0);
  lift->motionFlags = 0;
  lift->speedLimit = toFIXED(7) / 5;
  lift->acceleration = toFIXED(7) / 5;
  Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed + toFIXED(80),
                      leader->z.fixed);
  Actor_SetMoveTarget(lift, lift->x.fixed, lift->y.fixed + toFIXED(72),
                      lift->z.fixed);
  Sound_Play(SE_KIRIN);
  Event_StartScreenTransition();
  while (!Actor_HasArrived(leader)) {
    leader->facing = (u8)gFrameCount * FACING_STEP;
    leader->priorityFlags ^= ACTOR_PRIORITY_UNDERFOOT << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  FieldEvent_Wait(10);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  height = Map_GetTerrainHeight(0, lift->x.fixed, lift->z.fixed - toFIXED(16));
  leader->y.fixed = height;
  leader->groundHeight = height;
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  wait(6);
  Sound_Play(SE_JUMP);
  Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
  leader->speedLimit = toFIXED(2);
  leader->acceleration = toFIXED(1) / 2;
  leader->velocityY = toFIXED(4);
  leader->motionFlags = ACTOR_FALLS;
  leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
                      leader->z.fixed - toFIXED(16));
  Actor_WaitUntilLanded(leader);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_SelectAnimation(lift, 5);
  wait(12);
  Actor_Release(lift);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->groundHeight = leader->y.fixed;
  Actor_SetSpriteFlags(leader, 1);
#if TLA_PATCH_1
  leader->priorityFlags = priorityFlags;
#endif
  Actor_Release(prepared);
  wait(1);
  Event_WaitTransitionFrames();
  Event_End();
}

/// Eight motes of light drift about the hall, each starting a little after the
/// last; the odd ones swell slowly, the even ones quickly.
void AirsRockInside_SpawnMotes(void) {
  struct Actor *actor;
  s32 i;
  struct FieldSprite *sprite;

  for (i = 0; i != 8; i++) {
    actor = Import_ActorCreate(SPRITE_PARTICLE, 0, 0, 0);
    sprite = actor->sprite;
    sprite->priority = 1;
    actor->sceneByte = i << 4;
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->acceleration = actor->speedLimit =
        i * (toFIXED(1) / 16) + (toFIXED(1) / 2);
    actor->update = AirsRockInside_DriftMote;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    if (i & 1) {
      actor->timer = PARTICLE_SLOW_LIFETIME;
      actor->userValue = 0;
    } else {
      actor->timer = PARTICLE_QUICK_LIFETIME;
      actor->userValue = 0;
    }
    Actor_SetBlendMode(actor, 1);
  }
}

extern s32 gAirsRockInside_LeaderLevel;

/// A task in the great hall: as the leader climbs, two blocks of cells at the
/// hall's west and east sides take the collision codes kept for the level he
/// is at, the three levels 96 pixels apart.
void AirsRockInside_SetLedgeCellsByLevel(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (actor->y.fixed + toFIXED(32) >= toFIXED(312))
    level = 2;
  else if (actor->y.fixed + toFIXED(32) >= toFIXED(216))
    level = 1;
  else
    level = 0;
  if (gAirsRockInside_LeaderLevel == level)
    return;
  switch (level) {
  case 2: {
    s32 x = 7, z = 39;
    Map_CopyCellAttributeRect(64, 64, 6, 4, x, z);
  }
    Map_CopyCellAttributeRect(70, 64, 6, 4, 26, 39);
    break;
  case 1: {
    s32 x = 7, z = 39;
    Map_CopyCellAttributeRect(64, 68, 6, 4, x, z);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  case 0: {
    s32 x = 7, z = 39;
    Map_CopyCellAttributeRect(64, 72, 6, 4, x, z);
  }
    Map_CopyCellAttributeRect(70, 68, 6, 4, 26, 39);
    break;
  }
  gAirsRockInside_LeaderLevel = level;
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_AIRS_ROCK_INSIDE_SET_DOORS
#undef DEFINE_AIRS_ROCK_INSIDE_READ_TABLET
#undef DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef TARGET
#undef PICK_UP
#undef SHAKE

/// Dropping from the Vial chest room: the leader falls 130 pixels into the hall
/// and lands in a ring of dust.
void AirsRockInside_LandInHall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveToFX(-1, -1, -1, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  // A puff each sixteenth of a turn, the last over the first.
  for (i = 0; i <= TRIG_TURN / FACING_STEP; i++) {
    s32 angle;
    FIXED velocity[3];

    angle = i * FACING_STEP;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -(toFIXED(3) / 8);
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -(toFIXED(3) / 16);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

void AirsRockInside_PuffVents(void);

s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 i, z;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  GameFlag_Set(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
  GameFlag_Set(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
    AirsRockInside_StartRestoringCells();
    // The Japanese edition settles the leader here whatever
    // FLAG_KEEP_PARTY_POSITION says.
    if (gMainRoutineState.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET &&
        (!TLA_PATCH_1 || !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))) {
      Actor_GetById(gMainRoutineState.leaderActorId)->layer = 1;
      wait(8);
      MapGrid_CenterOnFollowedPoint();
      wait(1);
    }
    if (gMainRoutineState.entrance <= HALL_LAST_GREAT_HALL_ENTRANCE ||
        gMainRoutineState.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET) {
      Task_Add(AirsRockInside_SetLedgeCellsByLevel, TASK_PASS_FRAME);
      REG_BLDALPHA = BLEND_ALPHA(8, 16);
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      AirsRockInside_SpawnMotes();
    }
    if (gMainRoutineState.entrance > HALL_LAST_NORTHEAST_ROOM_ENTRANCE) {
      if (GameFlag_Get(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED))
        Actor_SetPositionFX(ACTOR_HALL_MOVABLE_STATUE, toFIXED(824),
                            toFIXED(328));
      Actor_PlaceBlocks(gAirsRockInside_HallPillars);
    }
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->motionFlags = 0;
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->y.fixed = 0;
    Actor_GetById(ACTOR_HALL_TWISTER_STATUE)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_HALL_TWISTER_STATUE, 3);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_STATUE),
                           SPRITE_COLOR_GREY);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_TWISTER_ROCK),
                           SPRITE_COLOR_GREY);
      MapGrid_CopyCellsTo(64, 78, 18, 105, 3, 5);
    }
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_WEST_STATUE),
                           SPRITE_COLOR_GREY);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_EAST_STATUE),
                           SPRITE_COLOR_GREY);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_HALL_MOVABLE_STATUE),
                           SPRITE_COLOR_GREY);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_WEST_STATUE), 180, 0);
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_HALL_EAST_STATUE), 60, 0);
      AirsRockInside_WakeMovableStatue(Actor_GetById(ACTOR_HALL_MOVABLE_STATUE),
                                       60, 0);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_WEST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_EAST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_HALL_MOVABLE_STATUE), 0);
    if (gMainRoutineState.entrance == ENTRANCE_HALL_LANDING &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      AirsRockInside_LandInHall();
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
    REG_BLDALPHA = BLEND_ALPHA(8, 16);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    AirsRockInside_StartRestoringCells();
    if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_VIAL_CHEST_EAST_PILLAR, toFIXED(328),
                          toFIXED(392));
    if (GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_VIAL_CHEST_WEST_PILLAR, toFIXED(104),
                          toFIXED(392));
    Actor_PlaceBlocks(gAirsRockInside_VialChestPillars);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_STATUE),
                           SPRITE_COLOR_GREY);
    } else {
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 60, 0);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_VIAL_CHEST_TWISTER_ROCK),
                           SPRITE_COLOR_GREY);
      MapGrid_CopyCellsTo(68, 84, 12, 77, 3, 6);
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_VIAL_CHEST_STATUE), 0);
    if (gMainRoutineState.entrance == ENTRANCE_VIAL_CHEST_ON_THE_LIFT &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
      AirsRockInside_RiseOnLift();
  } else if (gMainRoutineState.next ==
             SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
    AirsRockInside_StartRestoringCells();
    Scene_QueueMusic(SE_AMBIENT_STREAM);
    REG_BLDALPHA = BLEND_ALPHA(8, 12);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    Task_Add(AirsRockInside_PuffVents, TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
                          toFIXED(264), toFIXED(632));
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
                          toFIXED(616), toFIXED(568));
    if (GameFlag_Get(FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
                          toFIXED(472), toFIXED(520));
    Actor_PlaceBlocks(gAirsRockInside_ClarityCircletChestPillars);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
                           SPRITE_COLOR_GREY);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
                           SPRITE_COLOR_GREY);
      Actor_SetSpriteColor(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
                           SPRITE_COLOR_GREY);
    } else {
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A), 60, 0);
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B), 180, 0);
      AirsRockInside_WakeStatue(
          Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C), 200, 0);
    }
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A)->steeringFlags = 0;
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B)->steeringFlags = 0;
    Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C)->steeringFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
                         0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
                         0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
                         0);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPES_POST_A, ACTOR_TIGHTROPES_POST_B);
    Tightrope_Place(1, ACTOR_TIGHTROPES_POST_C, ACTOR_TIGHTROPES_POST_D);
    Tightrope_Place(2, ACTOR_TIGHTROPES_POST_E, ACTOR_TIGHTROPES_POST_F);
    Tightrope_Place(3, ACTOR_TIGHTROPES_POST_G, ACTOR_TIGHTROPES_POST_H);
    Tightrope_Place(4, ACTOR_TIGHTROPES_POST_I, ACTOR_TIGHTROPES_POST_J);
    Actor_GetById(ACTOR_TIGHTROPES_POST_H)->sceneCounter = 1;
    if (GameFlag_Get(FLAG_TIGHTROPES_BRIDGE_TURNED)) {
      MapGrid_CopyCellsTo(69, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(46, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(32, 89, 1, 1, 31, 89);
      MapGrid_CopyCellAttributes(50, 86, 1, 1, 33, 87);
    } else {
      MapGrid_CopyCellsTo(64, 64, 29, 85, 5, 7);
      MapGrid_CopyCellAttributes(52, 20, 5, 5, 29, 21);
      MapGrid_CopyCellAttributes(33, 88, 1, 1, 33, 87);
      MapGrid_CopyCellAttributes(48, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Get(FLAG_TIGHTROPES_GUST_PATH_CLEARED)) {
      MapGrid_CopyCellsTo(90, 25, 86, 25, 4, 3);
      MapGrid_CopyCellsTo(21, 88, 24, 89, 1, 2);
      MapGrid_CopyCellAttributes(23, 92, 2, 1, 23, 89);
      MapGrid_CopyCellAttributes(18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Get(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_TIGHTROPES_PILLAR, toFIXED(648), toFIXED(296));
    Actor_PlaceBlocks(gAirsRockInside_TightropesPillars);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    AirsRockInside_StartRestoringCells();
    for (i = 0; i != 6; i++) {
      if (GameFlag_Get(FLAG_FIRST_STONE_VANISHED + i)) {
        z = 24 - (i << 1);
        MapGrid_CopyCellsTo(65, 1, 80, z, 1, 2);
        FieldMapCell_SetAttributes(MAPGRID_LAYER_GROUND, 16, z,
                                   CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
      }
    }
    Actor_GetById(ACTOR_REVEAL_TABLET)->motionFlags = 0;
    actor = Actor_GetById(ACTOR_REVEAL_TABLET);
    Actor_GetById(ACTOR_REVEAL_TABLET)->groundHeight = toFIXED(16);
    actor->y.fixed = toFIXED(16);
    Actor_SetMovementType(Actor_GetById(ACTOR_REVEAL_TABLET),
                          MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
    AirsRockInside_StartRestoringCells();
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_ELIXIR_CHEST_POST_A, ACTOR_ELIXIR_CHEST_POST_B);
    Tightrope_Place(1, ACTOR_ELIXIR_CHEST_POST_C, ACTOR_ELIXIR_CHEST_POST_D);
    Tightrope_Place(2, ACTOR_ELIXIR_CHEST_POST_E, ACTOR_ELIXIR_CHEST_POST_F);
    if (!GameFlag_Get(FLAG_ELIXIR_CHEST_BRIDGE_TURNED)) {
      MapGrid_CopyCellsTo(69, 64, 32, 79, 5, 7);
      MapGrid_CopyCellAttributes(49, 15, 5, 5, 32, 15);
      MapGrid_CopyCellAttributes(33, 82, 1, 1, 34, 82);
      MapGrid_CopyCellAttributes(52, 81, 1, 1, 36, 81);
    } else {
      MapGrid_CopyCellsTo(64, 64, 32, 79, 5, 7);
      MapGrid_CopyCellAttributes(49, 21, 5, 5, 32, 15);
      MapGrid_CopyCellAttributes(35, 81, 1, 1, 36, 81);
      MapGrid_CopyCellAttributes(50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Get(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE))
      Actor_SetPositionFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(520),
                          toFIXED(296));
    if (GameFlag_Get(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED)) {
      Actor_SetPositionFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(312),
                          toFIXED(296));
      Actor_SetSpritePriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
      Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->motionFlags = ACTOR_FALLS;
      Actor_GetById(ACTOR_ELIXIR_CHEST_PILLAR)->groundHeight = -toFIXED(32);
      MapGrid_CopyCellAttributes(19, 16, 1, 1, 19, 18);
    }
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
    AirsRockInside_StartRestoringCells();
    Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_A)->sceneCounter = 1;
    Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneCounter = 1;
    Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneCounter = 1;
    Actor_GetById(ACTOR_PAIRED_STAKES_STAKE_D)->sceneCounter = 1;
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      GameFlag_Set(FLAG_STAKE_B_DOWN);
      GameFlag_Set(FLAG_STAKE_C_DOWN);
    }
    AirsRockInside_PlaceStakes(gAirsRockInside_PairedStakesStakes);
    if (GameFlag_Get(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_PAIRED_STAKES_PILLAR, toFIXED(344),
                          toFIXED(696));
    Actor_PlaceBlocks(gAirsRockInside_PairedStakesPillars);
    if (GameFlag_Get(FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY)) {
      MapGrid_CopyCellsTo(39, 82, 38, 82, 1, 3);
      MapGrid_CopyCellAttributes(39, 82, 1, 1, 38, 82);
      MapGrid_CopyCellAttributes(38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Get(FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY)) {
      MapGrid_CopyCellsTo(39, 82, 38, 99, 1, 3);
      MapGrid_CopyCellAttributes(38, 98, 1, 1, 38, 99);
      MapGrid_CopyCellAttributes(38, 35, 1, 1, 38, 37);
    }
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
    Scene_QueueMusic(SE_AMBIENT_STREAM);
    REG_BLDALPHA = BLEND_ALPHA(8, 12);
    REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
    Task_Add(AirsRockInside_PuffVents, TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_FACE_VENTS_FIRST_PILLAR, toFIXED(280),
                          toFIXED(472));
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_FACE_VENTS_SECOND_PILLAR, toFIXED(360),
                          toFIXED(840));
    if (GameFlag_Get(FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED)) {
      Actor_SetPositionFX(ACTOR_FACE_VENTS_THIRD_PILLAR, toFIXED(184),
                          toFIXED(616));
      GameFlag_Set(FLAG_SOUTHEAST_VENT_STILL);
    }
    Actor_SetSpritePriority(ACTOR_FACE_VENTS_SECOND_PILLAR, 2);
    Actor_PlaceBlocks(gAirsRockInside_FaceVentsPillars);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed), 0);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed), 0);
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_FACE_VENTS_STATUE),
                           SPRITE_COLOR_GREY);
    else
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_FACE_VENTS_STATUE), 8, 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FACE_VENTS_STATUE), 0);
  } else if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
    AirsRockInside_StartRestoringCells();
    if (GameFlag_Get(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED))
      Actor_SetPositionFX(ACTOR_COINS_CHEST_PILLAR, toFIXED(360), toFIXED(264));
    Actor_PlaceBlocks(gAirsRockInside_CoinsChestPillars);
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD,
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
        FIELD_CELL(Actor_GetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
    Actor_GetById(ACTOR_COINS_CHEST_STATUE)->steeringFlags = 0;
    if (!GameFlag_Get(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_COINS_CHEST_STATUE),
                           SPRITE_COLOR_GREY);
    else
      AirsRockInside_WakeStatue(Actor_GetById(ACTOR_COINS_CHEST_STATUE), 60, 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_COINS_CHEST_STATUE), 0);
    if (GameFlag_Get(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY)) {
      MapGrid_CopyCellsTo(15, 80, 15, 77, 2, 3);
      MapGrid_CopyCellAttributes(15, 80, 1, 1, 15, 79);
      MapGrid_CopyCellAttributes(17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_AIRS_ROCK_INSIDE_HALL)
    AirsRockInside_SetLedgeCellsByLevel();
  return 0;
}

void AirsRockInside_StartWhirlwind(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The blocks Move pushes in each room (Actor_PlaceBlocks), ended by -1: the
/// hall's statue.
s16 gAirsRockInside_HallPillars[] = {
    ACTOR_HALL_MOVABLE_STATUE, -1,
};

/// The Vial chest room's two pillars.
s16 gAirsRockInside_VialChestPillars[] = {
    ACTOR_VIAL_CHEST_EAST_PILLAR, ACTOR_VIAL_CHEST_WEST_PILLAR, -1,
};

/// The Clarity Circlet chest room's three pillars and its statue.
s16 gAirsRockInside_ClarityCircletChestPillars[] = {
    ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
    -1,
};

/// The tightropes room's pillar.
s16 gAirsRockInside_TightropesPillars[] = {
    ACTOR_TIGHTROPES_PILLAR, -1,
};

/// The paired stakes room's four pillars.
s16 gAirsRockInside_PairedStakesPillars[] = {
    ACTOR_PAIRED_STAKES_PILLAR,   ACTOR_PAIRED_STAKES_PILLAR_A,
    ACTOR_PAIRED_STAKES_PILLAR_B, ACTOR_PAIRED_STAKES_PILLAR_C,
    -1,
};

/// The face vents room's five pillars.
s16 gAirsRockInside_FaceVentsPillars[] = {
    ACTOR_FACE_VENTS_THIRD_PILLAR, ACTOR_FACE_VENTS_FIRST_PILLAR,
    ACTOR_FACE_VENTS_SECOND_PILLAR, ACTOR_FACE_VENTS_PILLAR_A,
    ACTOR_FACE_VENTS_PILLAR_B, -1,
};

/// The coins chest room's pillar and statue.
s16 gAirsRockInside_CoinsChestPillars[] = {
    ACTOR_COINS_CHEST_PILLAR, ACTOR_COINS_CHEST_STATUE, -1,
};

u32 gOrbitSpark_Script[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_RisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 gAirsRockInside_OrbitingSparkScript[] = {
    SC_CALL(AirsRockInside_OrbitAnchor),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gAirsRockInside_WhirlwindRisesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_WhirlwindFadesScript[] = {
    SC_WAIT(60),
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 gAirsRockInside_PooledWhirlwindFadesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 gAirsRockInside_WindStoneScript[] = {
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_END,
};

/// The paired stakes room's four stakes, as AirsRockInside_PlaceStakes reads
/// them: each stake, its first pose and the flag set while it is sunk.
u16 gAirsRockInside_PairedStakesStakes[] = {
    ACTOR_PAIRED_STAKES_STAKE_A, 1, FLAG_STAKE_A_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_B, 1, FLAG_STAKE_B_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_C, 1, FLAG_STAKE_C_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_D, 1, FLAG_STAKE_D_DOWN,
    ACTOR_LIST_END, 0,
};

struct SceneEntrance gAirsRockInside_Entrances[] = {
    {0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAirsRockInside_HallEntrances[] = {
    {1, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {2, CONDITION_ALWAYS, 152, 0, 136, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {3, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {4, CONDITION_ALWAYS, 152, 320, 648, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {5, CONDITION_ALWAYS, 152, 128, 648, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {6, CONDITION_ALWAYS, 232, 0, 632, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {7, CONDITION_ALWAYS, 312, 0, 616, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {8, CONDITION_ALWAYS, 472, 0, 136, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {9, CONDITION_ALWAYS, 472, 0, 232, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {10, CONDITION_ALWAYS, 408, 0, 952, FACING_NORTH, 0, 64, 16, 560, 1008, 0},
    {11, CONDITION_ALWAYS, 312, 0, 840, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {12, CONDITION_ALWAYS, 808, 0, 136, FACING_NORTH, 0, 608, 16, 864, 224, 0},
    {13, CONDITION_ALWAYS, 680, 0, 168, FACING_SOUTH, 0, 608, 16, 864, 224, 0},
    {14, CONDITION_ALWAYS, 872, 0, 344, FACING_SOUTH, 0, 592, 256, 976, 1008, 0},
    {15, CONDITION_ALWAYS, 904, 0, 904, FACING_SOUTH, 0, 592, 256, 976, 1008, 0},
    {16, CONDITION_ALWAYS, 312, 0, 808, FACING_SOUTH, 0, 64, 16, 560, 1008, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRockInsideHall),
    SCENE_EXIT_UNLESS(1, AirsRockInsideVialChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideClarityCircletChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideTightropes, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInsideElixirChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRockInsideHall, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRockInsidePsynergyStone, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRockInsidePsynergyStone, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRockInsidePairedStakes, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRockInsideFaceVents, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRockEntrance, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRockInsideHall, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRockInsideHall, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRockInsideHall, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AirsRockInsideCoinsChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AirsRockInsideRevealTablet, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AirsRockInsideVialChest, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideVialChest),
    SCENE_EXIT_UNLESS(1, AirsRockFujinShieldChest, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideHall, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideClarityCircletChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideTightropes, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideTightropes),
    SCENE_EXIT_UNLESS(1, AirsRockInsideClarityCircletChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideElixirChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRockInsideCoinsChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInsideHall, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideRevealTablet),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 16, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideElixirChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideTightropes, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsidePairedStakes),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideFaceVents, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideFaceVents),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsidePairedStakes, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsideCoinsChest),
    SCENE_EXIT_UNLESS(1, AirsRockInsideTightropes, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 15, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRockInsidePsynergyStone),
    SCENE_EXIT_UNLESS(1, AirsRockInsideHall, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRockInsideHall, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_HallPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 312, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_VialChestPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_ClarityCircletChestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_TightropesPlacements[] = {
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_RevealTabletPlacements[] = {
    {SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_ElixirChestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_PairedStakesPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_FaceVentsPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRockInside_CoinsChestPlacements[] = {
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)gAirsRockInside_WindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

s32 gAirsRockInside_StampedCell = -1;

struct Trigger gAirsRockInside_HallTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, EVENT(AirsRockInside_OpenTabletRoomDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES, CONDITION_ALWAYS, EVENT(AirsRockInside_StampTallCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER, CONDITION_ALWAYS, EVENT(AirsRockInside_StampTallCellUnderLeaderNearer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(AirsRockInside_WakeHallStatue)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE, CONDITION_ALWAYS, EVENT(AirsRockInside_JumpDownLedge)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(AirsRockInside_WakeVialChestStatue)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_ClarityCircletChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_RAISED, TRIGGER_PUFF_REACHES_LEADER, CONDITION_ALWAYS, EVENT(AirsRockInside_RideClarityCircletVent)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_TightropesTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_RevealTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampWideCellUnderLeader)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(AirsRockInside_IgnoreStep)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(AirsRockInside_WalkToRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(AirsRockInside_ReadTabletFacingEast)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AirsRockInside_RevealDoors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AirsRockInside_HideRevealedDoors)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 1, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 2, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 3, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 4, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOffVanishingStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_ElixirChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampOtherCellUnderLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELIXIR_CHEST_PILLAR, FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED, EVENT(AirsRockInside_MoveElixirChestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_PairedStakesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP, CONDITION_ALWAYS, EVENT(AirsRockInside_StepOntoStake)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_A, FLAG_STAKE_A_DOWN, EVENT(AirsRockInside_PoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_B, FLAG_STAKE_B_DOWN, EVENT(AirsRockInside_PoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_C, FLAG_STAKE_C_DOWN, EVENT(AirsRockInside_PoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_D, FLAG_STAKE_D_DOWN, EVENT(AirsRockInside_PoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_FaceVentsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_RAISED, TRIGGER_PUFF_REACHES_LEADER, CONDITION_ALWAYS, EVENT(AirsRockInside_RideFaceVent)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_CoinsChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StampCellUnderLeader)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(AirsRockInside_RideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(AirsRockInside_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(AirsRockInside_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_StartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_TakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_BlowGust)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(AirsRockInside_CheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

/// The great hall's level the leader stood at when its ledge cells were last
/// set; -1 before.
s32 gAirsRockInside_LeaderLevel = -1;
#undef Actor_SetHorizontalPositionWithTerrain
