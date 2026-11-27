/* @file */
/* Inside Air's Rock, reached through the doorway the lightning opens at its */
/* top, with a golden door low on its cliff also leading into the great hall */
/* (airs_rock.c): the hall and the rooms around it, their doorways marked by */
/* coloured dots. */
/* */
/* Felix moves pillars with Move or by pushing them, so that they fill gaps */
/* or block the vents whose puffs of steam blow him off his path; Pound */
/* sinks one of a pair of stakes and raises the other; Whirlwind on a Wind */
/* Stone sends a gust that carries pillars, blows golden barriers away and */
/* turns bridges, and on a twister rock wakes the statues, whose little */
/* whirlwinds carry the party. The party crosses tightropes and stepping */
/* stones that vanish behind them. At the heart of the rock, in the room of */
/* the Reveal tablet, Sheba reads the tablet and learns Reveal */
/* (scAirsRockInsideReadRevealTablet), and a Psynergy Stone in another room */
/* restores the party's PP (scAirsRockInsideCheckPsynergyStone). */

#include "scene/event_layer.h"
#include "field/field_sequence.h"
#include "field/event_inline.h"
#include "macros.h"
#include "field/actor_script.h"
#include "scene/airs_rock.h"
#include "scene/gabomba_statue.h"
#include "graphics/palette_buffers.h"
#include "window/window.h"

/* Set once Move has left the hall's movable statue on half cell (103, 41). */
#define FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED 0xa8e
/* Set once Move has left the Vial chest room's western pillar on (13, 49). */
#define FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED 0x898
/* Set once Move has left its eastern pillar on (41, 49). */
#define FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED 0x897
/* Set once Move has left the Clarity Circlet chest room's first pillar on (33,
 */
/* 79). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED 0x89b
/* Its second, on (77, 71). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED 0x89c
/* Its third, on (59, 65). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED 0x89d
/* Set once Move has left the tightropes room's pillar on (81, 37). */
#define FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED 0x89e
/* Set once a gust has dropped the Elixir chest room's pillar below its bridge
 */
/* (scAirsRockInsideBlowGust). The set-up sets it on every entry, so the room */
/* always shows the pillar dropped and its Move trigger never runs. */
#define FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED 0x89f
/* Set once Move has left the paired stakes room's pillar on (43, 87). */
#define FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED 0x8d0
/* Set once Move has left the face vents room's first pillar on (35, 59). */
#define FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED 0x8d1
/* Its second, on (45, 105). */
#define FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED 0x8d2
/* Its third, on (23, 77). */
#define FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED 0xa8f
/* Set once Move has left the coins chest room's pillar on (45, 33). */
#define FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED 0x8d3

/* Story flags: the statues of the hall and the Vial chest room woken by */
/* Whirlwind, and the Reveal tablet read. */
#define FLAG_AIRS_ROCK_HALL_STATUE_WOKEN	   0x89a
#define FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN 0x899
#define FLAG_REVEAL_TABLET_READ				   0x8d4

/* The paired stakes room's stakes, each set while it is down; B and C go */
/* down as the party comes in. */
#define FLAG_STAKE_A_DOWN FLAG_SCENE_TEMP_00
#define FLAG_STAKE_B_DOWN FLAG_SCENE_TEMP_01
#define FLAG_STAKE_C_DOWN FLAG_SCENE_TEMP_02
#define FLAG_STAKE_D_DOWN FLAG_SCENE_TEMP_03

/* The face vents room's vents kept still by where Move leaves a pillar */
/* (scAirsRockInsideFinishPushingPillar): the vent beside each puffs in its */
/* turn (scAirsRockInsidePuffVents). */
#define FLAG_SOUTHEAST_VENT_STILL	FLAG_SCENE_TEMP_00
#define FLAG_MIDDLE_WEST_VENT_STILL FLAG_SCENE_TEMP_01
#define FLAG_NORTHEAST_VENT_STILL	FLAG_SCENE_TEMP_02

/* The Reveal tablet room: set once a stepping stone has vanished, and one */
/* flag for each stone from the southernmost, set once it has. */
#define FLAG_STONE_VANISHED		  FLAG_SCENE_TEMP_00
#define FLAG_FIRST_STONE_VANISHED FLAG_SCENE_TEMP_02

/* What the gusts leave for the rest of the visit: the cells a gust clears on */
/* its path in the tightropes room (the set-up sets it on every entry, so the */
/* room always shows them cleared), a turning bridge turned from its first lie,
 */
/* the Elixir chest room's pillar left on its bridge, and a golden barrier */
/* blown away in the paired stakes and coins chest rooms. */
#define FLAG_TIGHTROPES_GUST_PATH_CLEARED			FLAG_AREA_TEMP_00
#define FLAG_TIGHTROPES_BRIDGE_TURNED				FLAG_AREA_TEMP_01
#define FLAG_ELIXIR_CHEST_BRIDGE_TURNED				FLAG_AREA_TEMP_02
#define FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE			FLAG_AREA_TEMP_03
#define FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_04
#define FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_05
#define FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY			FLAG_AREA_TEMP_06

/* The Elixir chest room's pillar column, in half cells, that stands it on */
/* the turning bridge. */
#define ELIXIR_CHEST_PILLAR_ON_BRIDGE_X 65

/* The hall's entrance from the Reveal tablet's room, and where the party */
/* lands after dropping from the Vial chest room (scAirsRockInsideLandInHall);
 */
/* the Vial chest room's entrance on the lift from the hall */
/* (scAirsRockInsideRiseOnLift). */
enum {
	ENTRANCE_HALL_FROM_REVEAL_TABLET = 16,
	ENTRANCE_HALL_LANDING = 11,
	ENTRANCE_VIAL_CHEST_ON_THE_LIFT = 3
};

/* The hall's entrances by the part of it they open on: up to the first of */
/* these the great hall itself, then its northeast room, and past the second */
/* its east room. ENTRANCE_HALL_FROM_REVEAL_TABLET, past both, takes the great
 */
/* hall's set-up and the east room's. */
enum {
	HALL_LAST_GREAT_HALL_ENTRANCE = 11,
	HALL_LAST_NORTHEAST_ROOM_ENTRANCE = 13
};

/* The collision codes this scene reads by name. On the overhead layer a */
/* whirlwind ends where it reaches WHIRLWIND_END; on the ground a raised */
/* stake's top is STAKE_TOP; in the hall the tower's door is TOWER_DOOR, and */
/* the Vial chest room is left by jumping off VIAL_CHEST_LEDGE. The */
/* Reveal tablet room's stepping stones run north from STONES_SOUTH_END to */
/* STONES_NORTH_END on the overhead layer, the five between them vanishing, */
/* and its tablet, as the Psynergy Stone room's stone, is examined on its */
/* cell. */
typedef enum AIRS_ROCK_INSIDE_CELL_BEHAVIOR {
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END = 1,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE = 2,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END = 7,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE = 20,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR = 30,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET = 30,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE = 30,
	/* The stepping stones Felix is drawn standing on: two kinds, a stack of */
	/* two and the same one row nearer, and the Reveal tablet room's stones that
	 */
	/* light gold under him (scAirsRockInsideStampCellUnderLeader and the rest).
	 */
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE = 80,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE = 90,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END = 212,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP = 212
} AIRS_ROCK_INSIDE_CELL_BEHAVIOR;

/* The face vents room's six vents, by the cell in front of each on the ground
 */
/* layer: south of the room the west and east one, in its middle the west and */
/* east one, in its north the west and east one. A vent's puff reaching the */
/* leader there blows him down to the place below it */
/* (scAirsRockInsideRideFaceVent). */
typedef enum AIRS_ROCK_INSIDE_FACE_VENTS_VENT {
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT = 50,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHEAST_VENT,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_WEST_VENT,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_EAST_VENT,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHWEST_VENT,
	CELL_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHEAST_VENT
} AIRS_ROCK_INSIDE_FACE_VENTS_VENT;

/* Where a gust stops on the overhead layer (scAirsRockInsideBlowGust), its */
/* code less CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP: up to LAST_BARRIER it */
/* blows on, clearing what blocks its path, as the golden barriers of the */
/* paired stakes and coins chest rooms; a turning bridge reads */
/* BRIDGE_NORTH_SOUTH while it runs north and south, and BRIDGE_EAST_WEST while
 */
/* it lies east and west, and the gust turns it the other way. */
typedef enum AIRS_ROCK_INSIDE_GUST_STOP {
	GUST_STOP_PAIRED_STAKES_NORTH_BARRIER = 0,
	GUST_STOP_LAST_BARRIER = 4,
	GUST_STOP_BRIDGE_NORTH_SOUTH = 5,
	GUST_STOP_BRIDGE_EAST_WEST = 6
} AIRS_ROCK_INSIDE_GUST_STOP;
#define CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP                               \
	30 /* the first stop's code                                                \
		*/

/* The triggers this scene raises (FIELD_STATE.raisedTrigger) besides a statue's
 */
/* whirlwind carrying the leader (RAISED_RIDE_WHIRLWIND), each room reading */
/* them its own way: a vent's puff reaching him, and in the Reveal tablet room,
 */
/* a cell he has left, as TRIGGER_LEFT_CELL plus its collision code. */
typedef enum AIRS_ROCK_INSIDE_RAISED_TRIGGER {
	TRIGGER_PUFF_REACHES_LEADER = 201,
	TRIGGER_LEFT_CELL = 200,
	TRIGGER_FIRST_VANISHING_STONE =
		TRIGGER_LEFT_CELL + CELL_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE
} AIRS_ROCK_INSIDE_RAISED_TRIGGER;

/* The actors of each inner room, numbering its placement table. The statues */
/* (SPRITE_WIND_STATUE) breathe whirlwinds once Whirlwind on their room's */
/* twister rock wakes them; Whirlwind on a Wind Stone (SPRITE_WIND_STONE) */
/* sends a gust; stone pillars (SPRITE_STONE_PILLAR) are Moved. */
typedef enum AIRS_ROCK_INSIDE_HALL_ACTOR_ID {
	ACTOR_HALL_WEST_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_HALL_EAST_STATUE,
	/* The statue the hall's own twister rock wakes. */
	ACTOR_HALL_TWISTER_STATUE,
	ACTOR_HALL_TWISTER_ROCK,
	/* The statue in the hall's east room, which Move pushes onto its mark. */
	ACTOR_HALL_MOVABLE_STATUE,
	/* The three whirlwinds the movable statue breathes in turn. */
	ACTOR_HALL_FIRST_WHIRLWIND
} AIRS_ROCK_INSIDE_HALL_ACTOR_ID;

/* The Vial chest's room (AirsRockInside_VialChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID {
	ACTOR_VIAL_CHEST_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_VIAL_CHEST_EAST_PILLAR,
	ACTOR_VIAL_CHEST_TWISTER_ROCK,
	ACTOR_VIAL_CHEST_WEST_PILLAR
} AIRS_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID;

/* The Clarity Circlet chest's room */
/* (AirsRockInside_ClarityCircletChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST_ACTOR_ID {
	ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
	ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C
} AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST_ACTOR_ID;

/* The tightropes' room (AirsRockInside_TightropesPlacements). */
typedef enum AIRS_ROCK_INSIDE_TIGHTROPES_ACTOR_ID {
	ACTOR_TIGHTROPES_NORTH_WIND_STONE = ACTOR_FIRST_PLACED,
	ACTOR_TIGHTROPES_PILLAR,
	ACTOR_TIGHTROPES_SOUTH_WIND_STONE,
	/* The posts of its five tightropes, two to a rope. */
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
} AIRS_ROCK_INSIDE_TIGHTROPES_ACTOR_ID;

/* The Reveal tablet (SPRITE_SLATE), which rises as Sheba reads it. */
#define ACTOR_REVEAL_TABLET ACTOR_FIRST_PLACED

/* The Elixir chest's room (AirsRockInside_ElixirChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_ELIXIR_CHEST_ACTOR_ID {
	/* The stone pillar scAirsRockInsideMoveElixirChestPillar moves. */
	ACTOR_ELIXIR_CHEST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE,
	ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE,
	ACTOR_ELIXIR_CHEST_POST_A,
	ACTOR_ELIXIR_CHEST_POST_B,
	ACTOR_ELIXIR_CHEST_POST_C,
	ACTOR_ELIXIR_CHEST_POST_D,
	ACTOR_ELIXIR_CHEST_POST_E,
	ACTOR_ELIXIR_CHEST_POST_F
} AIRS_ROCK_INSIDE_ELIXIR_CHEST_ACTOR_ID;

/* The paired stakes' room (AirsRockInside_PairedStakesPlacements). */
typedef enum AIRS_ROCK_INSIDE_PAIRED_STAKES_ACTOR_ID {
	ACTOR_PAIRED_STAKES_PILLAR = ACTOR_FIRST_PLACED,
	/* The three pillars pushed among the stakes. */
	ACTOR_PAIRED_STAKES_PILLAR_A,
	ACTOR_PAIRED_STAKES_PILLAR_B,
	ACTOR_PAIRED_STAKES_PILLAR_C,
	/* The four stakes (SPRITE_STAKE), in two pairs: as one sinks, the other */
	/* rises. */
	ACTOR_PAIRED_STAKES_STAKE_A,
	ACTOR_PAIRED_STAKES_STAKE_B,
	ACTOR_PAIRED_STAKES_STAKE_C,
	ACTOR_PAIRED_STAKES_STAKE_D,
	ACTOR_PAIRED_STAKES_NORTH_WIND_STONE,
	ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE
} AIRS_ROCK_INSIDE_PAIRED_STAKES_ACTOR_ID;

/* The room of the face vents (AirsRockInside_FaceVentsPlacements). */
typedef enum AIRS_ROCK_INSIDE_FACE_VENTS_ACTOR_ID {
	ACTOR_FACE_VENTS_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_FACE_VENTS_SECOND_PILLAR,
	ACTOR_FACE_VENTS_WIND_STONE,
	ACTOR_FACE_VENTS_STATUE,
	ACTOR_FACE_VENTS_THIRD_PILLAR,
	ACTOR_FACE_VENTS_PILLAR_A,
	ACTOR_FACE_VENTS_PILLAR_B
} AIRS_ROCK_INSIDE_FACE_VENTS_ACTOR_ID;

/* The coins chest's room (AirsRockInside_CoinsChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_COINS_CHEST_ACTOR_ID {
	ACTOR_COINS_CHEST_WIND_STONE = ACTOR_FIRST_PLACED,
	ACTOR_COINS_CHEST_PILLAR,
	ACTOR_COINS_CHEST_STATUE
} AIRS_ROCK_INSIDE_COINS_CHEST_ACTOR_ID;

/* The cell stampers keep their last cell and the block they copy. */
extern s32 AirsRockInside_StampedCell;
s32 AirsRockInside_StampedWidth BSS_DATA;
s32 AirsRockInside_StampedHeight BSS_DATA;
s32 AirsRockInside_StampedX BSS_DATA;
s32 AirsRockInside_StampedZ BSS_DATA;

#include "scene/scene.h"
#include "field/call.h"
#include "game/game.h"
#include "version.h"

struct ACTOR;

extern ACTOR_PLACE AirsRockInside_Placements[];
extern ACTOR_PLACE AirsRockInside_HallPlacements[];
extern ACTOR_PLACE AirsRockInside_VialChestPlacements[];
extern ACTOR_PLACE AirsRockInside_ClarityCircletChestPlacements[];
extern ACTOR_PLACE AirsRockInside_RevealTabletPlacements[];

void scAirsRockInsideRestoreCellBehindLeader(void);

void scAirsRockInsideBurstDust(s32 x, s32 z);
void scAirsRockInsideScatterSparks(s32 x, s32 z);

extern s16 AirsRockInside_HallPillars[];
extern s16 AirsRockInside_VialChestPillars[];
extern s16 AirsRockInside_ClarityCircletChestPillars[];
extern s16 AirsRockInside_TightropesPillars[];
extern s16 AirsRockInside_PairedStakesPillars[];
extern s16 AirsRockInside_FaceVentsPillars[];
extern s16 AirsRockInside_CoinsChestPillars[];

void scAirsRockInsideStampCellUnderLeader(void);
void scAirsRockInsideReadRevealTablet(void);

extern TRIGGER AirsRockInside_HallTriggers[];
extern TRIGGER AirsRockInside_VialChestTriggers[];
extern TRIGGER AirsRockInside_ClarityCircletChestTriggers[];
extern TRIGGER AirsRockInside_TightropesTriggers[];
extern TRIGGER AirsRockInside_RevealTabletTriggers[];
extern TRIGGER AirsRockInside_ElixirChestTriggers[];
extern TRIGGER AirsRockInside_PairedStakesTriggers[];
extern TRIGGER AirsRockInside_FaceVentsTriggers[];
extern TRIGGER AirsRockInside_CoinsChestTriggers[];
extern TRIGGER AirsRockInside_PsynergyStoneTriggers[];

void scAirsRockInsideStartRestoringCells(void);
void scAirsRockInsideSetLedgeCellsByLevel(void);
void scAirsRockInsideSpawnMotes(void);

void scAirsRockInsideRiseOnLift(void);

void scAirsRockInsidePlaceStakes(u16 *list);
void scAirsRockInsideTakeOverWhirlwind(void);
void scAirsRockInsideRideWhirlwind(void);
void scAirsRockInsideBlowGust(void);

void scAirsRockInsideStampOtherCellUnderLeader(void);
void scAirsRockInsideStampTallCellUnderLeader(void);
void scAirsRockInsideStampTallCellUnderLeaderNearer(void);
void scAirsRockInsideWakeHallStatue(void);
void scAirsRockInsideOpenTabletRoomDoor(void);
void scAirsRockInsideStartPushingPillar(void);
void scAirsRockInsideFinishPushingPillar(void);
void scAirsRockInsideWakeVialChestStatue(void);
void scAirsRockInsideJumpDownLedge(void);
void scAirsRockInsideRideClarityCircletVent(void);
void scAirsRockInsideStepOffVanishingStone(void);
void scAirsRockInsideStampWideCellUnderLeader(void);
void scAirsRockInsideHideRevealedDoors(void);
void scAirsRockInsideRevealDoors(void);
void scAirsRockInsideWalkToRevealTablet(void);
void scAirsRockInsideReadTabletFacingWest(void);
void scAirsRockInsideReadTabletFacingEast(void);
void scAirsRockInsideMoveElixirChestPillar(void);
void scAirsRockInsidePoundPairedStakesStake(s32 mode, s32 step);
void scAirsRockInsideStepOntoStake(void);
void scAirsRockInsideRideFaceVent(void);
void scAirsRockInsideCheckPsynergyStone(void);
void scAirsRockInsideIgnoreStep(void);

extern u32 FieldEffect_MediumScript[];
extern u32 AirsRockInside_WindStoneScript[];

void fdActorPlaySndInView(ACTOR *actor, s32 cue);

void scAirsRockInsideSetStakeCells(ACTOR *actor);

/* Unused: places the pillars of a list as shared/pillar_place.h does, reading
 */
/* its halfwords as signed. */
UNUSED void scPillarPlace(s16 *list) {
	s16 *entry;

	if((u16)list[0] == ACTOR_LIST_END)
		return;
	for(entry = list; (u16)entry[0] != ACTOR_LIST_END; entry += 3) {
		s32 id = (u16)entry[0];
		ACTOR *actor = fdActorGetById(id);
		s32 kind = entry[1];
		s32 flag = entry[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
	}
}

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern ACTOR_PLACE AirsRockInside_TightropesPlacements[];
extern ACTOR_PLACE AirsRockInside_ElixirChestPlacements[];
extern ACTOR_PLACE AirsRockInside_PairedStakesPlacements[];
extern ACTOR_PLACE AirsRockInside_FaceVentsPlacements[];
extern ACTOR_PLACE AirsRockInside_CoinsChestPlacements[];

/* A mote of light moves on to a random spot, x 176 to 431 pixels and y and z */
/* up to 31 and 127 pixels past the leader's, to drift there for its timer's */
/* frames, swelling slowly or quickly by its lifetime. */
void scAirsRockInsideScatterMote(ACTOR *actor) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(syRandom() & 0xff) {
		actor->x.fixed = INTtoFIXED(syRandom() & 0xff) + toFIXED(176);
		actor->y.fixed = leader->y.fixed + INTtoFIXED(syRandom() & 31);
		actor->z.fixed = leader->z.fixed + INTtoFIXED(syRandom() & 127);
	}
	actor->userValue = actor->tmr;
	if(actor->tmr == PARTICLE_SLOW_LIFETIME)
		fdActorSelectAnimation(actor, ANIM_PARTICLE_SLOW_SWELL);
	else
		fdActorSelectAnimation(actor, ANIM_PARTICLE_QUICK_SWELL);
}

/* A mote's update: after its start delay it sways as it drifts upward, a */
/* sway every 64 pixels of depth and every 128 of height, scattering again */
/* when its time is up; it and the map's animations stop while a menu, */
/* Psynergy or an item is in use. */
void scAirsRockInsideDriftMote(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0) {
		fdActorSetAnimationSpeed(actor, 0);
		fdTilesetAnimSuspendAll();
		return;
	}
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL);
	fdTilesetAnimResumeAll();
	if(actor->sceneByte != 0) {
		if((FrameCount & 7) == 0)
			actor->sceneByte--;
		return;
	}
	if(actor->userValue == 0)
		scAirsRockInsideScatterMote(actor);
	actor->userValue--;
	actor->x.fixed += syCos((FIXEDtoINT_FOUR_TIMES(actor->z.fixed) +
							 FIXEDtoINT_TWICE(actor->y.fixed))
							<< 8) /
					  2;
	actor->y.fixed += actor->speedLimit;
}

/* Nothing visible happens of its own as Felix starts casting Whirlwind on a */
/* twister rock or a Wind Stone inside the rock; the game clears what is */
/* pending so that the cast can go on to the rock's or stone's own whirlwind */
/* (scAirsRockInsideTakeOverWhirlwind). */
void scAirsRockInsideStartWhirlwind(void) { fdEventEndLastingPsynergy(0); }

/* Nothing visible happens of its own as Felix casts Whirlwind on a twister */
/* rock or a Wind Stone inside the rock; the game tells Whirlwind to leave out
 */
/* its usual little whirlwind, so that what follows is the rock's or the */
/* stone's own (scAirsRockInsideBlowGust, scAirsRockInsideWakeHallStatue and */
/* scAirsRockInsideWakeVialChestStatue). */
void scAirsRockInsideTakeOverWhirlwind(void) {
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->sceneVariant = 1;
}

#include "shared/map_cell_set.c"

#include "../field/actor_wait_until_landed.c"

#include "../field/map_cell_get.c"

/* Sets a stake's cells by its flag (its timer), set once it has sunk: a sunk */
/* stake's cell is open ground and its height drops 32 pixels unless it stands
 */
/* on its ground, drawn under the leader; a raised one is a wall on the */
/* overhead layer with its top (CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP) on */
/* the ground layer, 32 pixels higher. */
void scAirsRockInsideSetStakeCells(ACTOR *actor) {
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 ground = fdMapCellGetAttributes(MAPGRID_LAYER_GROUND, x, z);
	s32 cell = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z);
	s32 height = CELL_HEIGHT_STEPS(
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));
	s32 set = syGameFlagGet(actor->tmr);
	u8 *flags = &actor->priFlg;
	s32 px = FIELD_CELL_ORIGIN(x);
	s32 pz = FIELD_CELL_ORIGIN(z);

	if(set) {
		cell &= ~CELL_CODE_BITS;
		ground = 0;
		if(CELL_HEIGHT_STEPS(actor->groundHeight) != height)
			height -= 4;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		scSetActorSprPriority(actor, 3);
	}
	else {
		cell |= CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
		ground |= CELL_ATTRIBUTES(CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP,
								  MAP_CELL_JUMP);
		*flags &= ~ACTOR_PRIORITY_UNDERFOOT;
		height += 4;
		scSetActorSprPriority(actor, 1);
	}
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, x, z, ground);
	scMapCellSetAttributes(MAPGRID_LAYER_OVERHEAD, x, z, cell);
	fdMapGridSetCellHeight(px, pz, actor->layer, height);
}

/* A stake's picture in OBJ VRAM (stake.png): 32 by 64 pixels of 256-colour */
/* tiles, four to each 8-pixel row, the stake itself within the first three */
/* of them. A partner rising is drawn from its target's picture: the top */
/* STAKE_TOP_ROWS rows at once, then a row a step. */
enum { STAKE_ROW_TILES = 4, STAKE_DRAWN_TILES = 3, STAKE_TOP_ROWS = 3 };
/* The bytes of one 8-pixel row of a stake's picture. */
#define STAKE_ROW_BYTES (STAKE_ROW_TILES * TILE_SIZE_8BPP)

/* One step of Pound on a stake: the target sinks and its partner, the other */
/* of its pair, rises. On step 0 the partner's picture is cleared and the top */
/* of the target's drawn over it, 28 pixels up; on steps 1 to 5 a row more of */
/* it is drawn as it comes down; through step 7 the target flattens. Once the */
/* fist has landed (POUND_STAGE_LANDED) both settle, the target's flag is set */
/* and the partner's cleared, they take their next poses and their cells are */
/* set again (scAirsRockInsideSetStakeCells). */
void scAirsRockInsidePoundStake(s32 mode, s32 step) {
	s16 *pair = &((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->tgtId;
	s32 even = *pair - ((*(u16 *)pair & 1) << 1);
	ACTOR *first = fdActorGetById(*pair);
	ACTOR *second = fdActorGetById(even + 1);
	FIELD_SPR *firstSprite, *secondSprite;
	VRAM_CACHE_ENTRY *entry;

	if(step == 0) {
		firstSprite = first->spr;
		secondSprite = second->spr;
		second->motionFlg = 0;
		second->y.fixed -= toFIXED(28);
		sySynch(1);
		entry = &VramCacheEntries[secondSprite->slot];
		second->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
		entry = &VramCacheEntries[firstSprite->slot];
		first->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
		btDmaFill32(step, second->watchedActor, entry->size);
		syDMAXCopy(first->watchedActor, second->watchedActor,
				   DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4),
				   REG_DMA3);
		first->watchedActor = (void *)((u8 *)first->watchedActor +
									   STAKE_TOP_ROWS * STAKE_ROW_BYTES);
		second->watchedActor = (void *)((u8 *)second->watchedActor +
										STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	}
	else if(step <= 5) {
		second->y.fixed += toFIXED(26) / 5;
		syDMAXCopy(first->watchedActor, second->watchedActor,
				   DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4),
				   REG_DMA3);
		first->watchedActor =
			(void *)((u8 *)first->watchedActor + STAKE_ROW_BYTES);
		second->watchedActor =
			(void *)((u8 *)second->watchedActor + STAKE_ROW_BYTES);
	}
	if(step <= 7)
		first->scaleY += -(FIXED_ONE * 31 / 256);
	if(mode == POUND_STAGE_LANDED) {
		first->scaleY = FIXED_ONE;
		second->y.fixed = second->groundHeight;
		syGameFlagSet(first->tmr);
		syGameFlagClear(second->tmr);
		fdActorSelectAnimation(first, ((FIELD_SPR *)first->spr)->lastNo + 1);
		fdActorSelectAnimation(second, ((FIELD_SPR *)second->spr)->lastNo - 1);
		scAirsRockInsideSetStakeCells(first);
		scAirsRockInsideSetStakeCells(second);
	}
}

/* Places the stakes of a list ended by ACTOR_LIST_END, each entry three */
/* halfwords: the stake, the first of its poses and the flag that says whether
 */
/* it has sunk, which picks between them. The stake stands still, no longer */
/* solid, and keeps its flag in its timer (scAirsRockInsideSetStakeCells). */
void scAirsRockInsidePlaceStakes(u16 *list) {
	ACTOR *actor;
	s32 flag, base;
	s16 pose;
	u16 *entry;

	if(*list == ACTOR_LIST_END)
		return;
	for(entry = list; *entry != ACTOR_LIST_END; entry += 3) {
		actor = fdActorGetById(*entry);
		pose = ((s16 *)entry)[1];
		flag = ((s16 *)entry)[2];
		fdActorSetSprFlags(actor, 0);
		{
			s32 choice = syGameFlagGet((u16)flag);

			base = (u16)pose;
			base += base << 1;
			base += choice;
		}
		fdActorSelectAnimation(actor, base);
		actor->motionFlg = 0;
		actor->collisionFlg &= ~ACTOR_SOLID;
		actor->tmr = flag;
		scAirsRockInsideSetStakeCells(actor);
	}
}

/* The leader rides a statue's whirlwind along its path until it fades. */
void scAirsRockInsideRideWhirlwind(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *ride = &leader->watchedActor->actor;
	ACTOR *statue, *dust;
	s32 rise;
	FIXED dx, dz;
	s32 moved, frames, height;
	u16 facing;
	s32 angle;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	leader->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlags(leader, 0);
	syGameFlagSet(FLAG_RIDING_WHIRLWIND);
	sySndPlay(SE_KIRIN);
	rise = 0;
	while(ride->sceneByte != 0) {
		leader->x.fixed = ride->x.fixed;
		leader->y.fixed = ride->y.fixed + rise;
		leader->z.fixed = ride->z.fixed;
		if(rise < toFIXED(12))
			rise += toFIXED(1) / 10;
		leader->facing = (u8)FrameCount * FACING_STEP;
		leader->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (FrameCount & 1);
		sySynch(1);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	/* The hall's whirlwind at x 312 lifts him into the Vial chest room. */
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL &&
	   FIELD_HALF_CELL(leader->x.fixed) == 39) {
		fdEventSetExitRequest(
			AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_VIAL_CHEST_SOUTH_B);
		fdEventEnd();
		return;
	}
	facing = ride->facing;
	switch(facing) {
	case FACING_NORTH:
		statue = &ride->watchedActor->actor;
		dx = 0;
		dz = 0;
		leader->priFlg = ACTOR_PRIORITY_OVERHEAD;
		leader->x.fixed =
			FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->x.fixed) & ~1) + 1);
		leader->y.fixed =
			FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->y.fixed) & ~1) + 3);
		leader->z.fixed =
			FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(leader->z.fixed) & ~1) + 1);
		for(frames = 0; frames != 60; frames++) {
			moved = 0;
			if(InputWork.held & KEY_UP) {
				dx = 0;
				dz = -toFIXED(16);
				leader->facing = FACING_NORTH;
				moved = 1;
			}
			if(InputWork.held & KEY_RIGHT) {
				dx = toFIXED(16);
				dz = 0;
				leader->facing = FACING_EAST;
				moved = 1;
			}
			if(InputWork.held & KEY_LEFT) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
				dz = 0;
				moved = 1;
			}
			if(moved &&
			   CELL_CODE(fdMapCellGetAttributes(
				   MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed + dx),
				   FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) !=
				   CELL_BEHAVIOR_WALL)
				goto found;
			sySynch(1);
		}
		dx = 0;
		dz = -toFIXED(16);
		leader->facing = FACING_NORTH;
		if((u16)CELL_CODE(fdMapCellGetAttributes(
			   MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed),
			   FIELD_CELL(ride->z.fixed - ride->y.fixed + dz))) ==
		   CELL_BEHAVIOR_WALL) {
			dx = toFIXED(16);
			dz = 0;
			leader->facing = FACING_EAST;
			if((u16)CELL_CODE(fdMapCellGetAttributes(
				   MAPGRID_LAYER_OVERHEAD, FIELD_CELL(ride->x.fixed + dx),
				   FIELD_CELL(ride->z.fixed - ride->y.fixed))) ==
			   CELL_BEHAVIOR_WALL) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
			}
		}
	found:
		/* The statue's sceneCnt holds the layer of the ledge he lands on.
		 */
		height = fdMapGetTerrainHeight(statue->sceneCnt, ride->x.fixed + dx,
									   ride->z.fixed + dz);
		leader->y.fixed = height;
		leader->groundHeight = height;
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(2);
		leader->acceleration = toFIXED(1) / 2;
		leader->velY = toFIXED(4);
		leader->motionFlg = ACTOR_FALLS;
		leader->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		leader->layer = statue->sceneCnt;
		fdActorSetMoveTarget(leader, leader->x.fixed + dx, leader->y.fixed,
							 leader->z.fixed + dz);
		fdActorWaitUntilLanded(leader);
		fdActorWaitForMove(leader);
		fdActorSetSprFlags(leader, 1);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		break;
	default:
		angle = ride->facing;
		leader->x.fixed =
			FIELD_HALF_CELL_ORIGIN((FIELD_HALF_CELL(ride->x.fixed) & ~1) +
								   (angle = FACING_EIGHTH_INDEX(angle)) - 1);
		leader->z.fixed = FIELD_HALF_CELL_ORIGIN(
			(FIELD_HALF_CELL(ride->z.fixed) & ~1) - (angle & 2) + 1);
		fdActorSetSprFlags(leader, 1);
		leader->velY = toFIXED(6);
		leader->priFlg =
			ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_AUTOMATIC;
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdCameraMoveToFX(-1, -1, -1, 0);
		dust = fdCameraGetActor();
		dust->speedLimit = toFIXED(8);
		dust->acceleration = toFIXED(1);
		dust->motionFlg = 0;
		fdActorSetMoveTarget(
			dust, leader->x.fixed,
			fdMapGetTerrainHeight(0, leader->x.fixed, leader->z.fixed),
			leader->z.fixed);
		fdActorWaitUntilLanded(leader);
		fdActorWaitForMove(dust);
		break;
	}
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syGameFlagClear(FLAG_RIDING_WHIRLWIND);
	sySynch(10);
	fdEventEnd();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern u32 AirsRockInside_WhirlwindFadesScript[];
extern u32 AirsRockInside_WhirlwindRisesScript[];

/* How near a whirlwind comes to catch the leader above him while others move */
/* him, in his cell's row, in pixels; on his level it is WHIRLWIND_REACH. */
enum { WHIRLWIND_REACH_ABOVE = 16 };

/* A statue's whirlwind: it moves 1.5 pixels a frame the way it faces, upward */
/* when it faces north, and where the overhead layer reads */
/* CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END it fades (facing north) or */
/* rises. The first time it comes within reach of the leader it carries him */
/* along (RAISED_RIDE_WHIRLWIND). It stops while the leader moves as sand, a */
/* menu or Psynergy is up, or an item is in use. */
void scAirsRockInsideMoveWhirlwind(ACTOR *shot) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 near;
	s32 *leaderPosition;
	s32 *shotPosition;
	FIXED speed = toFIXED(3) / 2;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0 ||
	   MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SAND) {
		fdActorSetAnimationSpeed(shot, 0);
		return;
	}
	fdActorSetAnimationSpeed(shot, ANIM_STEP_NORMAL);
	if(CELL_CODE(fdMapCellGetAttributes(
		   MAPGRID_LAYER_OVERHEAD, FIELD_CELL(shot->x.fixed),
		   FIELD_CELL(shot->z.fixed - shot->y.fixed))) ==
	   CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
		shot->sceneByte = 0;
		shot->update = 0;
		Call2((void (*)())fdActorPlaySndInView, (s32)shot, SE_SAND_EMERGE);
		if(shot->facing == FACING_NORTH)
			fdActorStartScript(shot, AirsRockInside_WhirlwindFadesScript);
		else
			fdActorStartScript(shot, AirsRockInside_WhirlwindRisesScript);
		return;
	}
	if(shot->sceneCnt == 0) {
		near = 0;
		leaderPosition = &leader->x.fixed;
		shotPosition = &shot->x.fixed;
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
			if(scPosDistanceAbove(leaderPosition, shotPosition) <=
				   WHIRLWIND_REACH_ABOVE &&
			   FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
				near = 1;
		}
		else {
			if(scPosFlatDistance(leaderPosition, shotPosition) <=
				   WHIRLWIND_REACH &&
			   FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
				near = 1;
		}
		if(near && !fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
			leader->facing = shot->facing;
			leader->watchedActor = (FIELD_OBJECT *)shot;
			leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)leader->spr)->pri = ((FIELD_SPR *)shot->spr)->pri;
			state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			shot->sceneCnt = 1;
			shot->sceneByte = 1;
		}
	}
	if(shot->facing == FACING_NORTH) {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->y.fixed -= syMulFX(sySin(shot->facing), speed);
	}
	else {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->z.fixed += syMulFX(sySin(shot->facing), speed);
	}
}

/* A woken statue's update: while no menu, Psynergy or item is in use it counts
 */
/* frames in its timer, and every WHIRLWIND_BREATH_FRAMES breathes a new */
/* whirlwind 8 pixels ahead of it, with a puff of dust below and a flare unless
 */
/* the leader is riding one. */
void scAirsRockInsideStatueBreathesWhirlwind(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *shot;
	VECTOR3 position;
	EFFECT_OPTIONS options;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0)
		return;
	if(actor->tmr == WHIRLWIND_BREATH_FRAMES) {
		if(!syGameFlagGet(FLAG_RIDING_WHIRLWIND))
			fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		if(actor->facing == FACING_NORTH) {
			position.x = actor->x.fixed;
			position.z = actor->z.fixed;
		}
		else {
			position.x =
				actor->x.fixed + syMathMulFX(toFIXED(8), syCos(actor->facing));
			position.z =
				actor->z.fixed + syMathMulFX(toFIXED(8), sySin(actor->facing));
		}
		shot = (ACTOR *)syImportActorCreate(SPRITE_WHIRLWIND, position.x,
											actor->y.fixed, position.z);
		fdActorSelectAnimation(shot, 2);
		fdActorSetSprFlags(shot, 0);
		((FIELD_SPR *)shot->spr)->pri = 1;
		shot->motionFlg = 0;
		shot->sceneCnt = 0;
		shot->sceneByte = 0;
		shot->facing = actor->facing;
		shot->update = scAirsRockInsideMoveWhirlwind;
		shot->watchedActor = (FIELD_OBJECT *)actor;
		actor->tmr = 0;
		options.pri = 1;
		options.pal = 7;
		scFieldEffectSpawn(position.x, actor->y.fixed, position.z - toFIXED(6),
						   0, 0, 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
						   &options);
	}
	actor->tmr++;
}

/* Wakes a statue unless the party keeps its place: it breathes its first */
/* whirlwind once its timer reaches WHIRLWIND_BREATH_FRAMES, and its whirlwinds
 */
/* set the leader down on the ledge of layer `landingLayer`. */
void scAirsRockInsideWakeStatue(ACTOR *actor, s16 timer, u8 landingLayer) {
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		actor->steeringFlg = 0;
		actor->tmr = timer;
		actor->sceneCnt = landingLayer;
		actor->update = (s32)scAirsRockInsideStatueBreathesWhirlwind;
	}
}

extern u32 AirsRockInside_PooledWhirlwindFadesScript[];

/* A whirlwind of the hall's movable statue, one of three it uses in turn: as */
/* scAirsRockInsideMoveWhirlwind, but it also holds still until the room's */
/* opening transition has begun (FIELD_STATE.transitionActive). */
void scAirsRockInsideMovePooledWhirlwind(ACTOR *shot) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 near;
	FIXED speed = toFIXED(3) / 2;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0 || state->transitionActive == 0) {
		fdActorSetAnimationSpeed(shot, 0);
		return;
	}
	fdActorSetAnimationSpeed(shot, ANIM_STEP_NORMAL);
	if(CELL_CODE(fdMapCellGetAttributes(
		   MAPGRID_LAYER_OVERHEAD, FIELD_CELL(shot->x.fixed),
		   FIELD_CELL(shot->z.fixed - shot->y.fixed))) ==
	   CELL_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
		Call2((void (*)())fdActorPlaySndInView, (s32)shot, SE_SAND_EMERGE);
		shot->sceneByte = 0;
		shot->update = 0;
		fdActorStartScript(shot, AirsRockInside_PooledWhirlwindFadesScript);
		return;
	}
	if(shot->sceneCnt == 0) {
		near = 0;
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
			if(scPosDistanceAbove(&leader->x.fixed, &shot->x.fixed) <=
				   WHIRLWIND_REACH_ABOVE &&
			   FIELD_CELL(leader->z.fixed) == FIELD_CELL(shot->z.fixed))
				near = 1;
		}
		else {
			if(scPosFlatDistance(&leader->x.fixed, &shot->x.fixed) <=
				   WHIRLWIND_REACH &&
			   FIELD_CELL(leader->y.fixed) == FIELD_CELL(shot->y.fixed))
				near = 1;
		}
		if(near && !fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
			leader->facing = shot->facing;
			leader->watchedActor = (FIELD_OBJECT *)shot;
			leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)leader->spr)->pri = ((FIELD_SPR *)shot->spr)->pri;
			state->raisedTrigger = RAISED_RIDE_WHIRLWIND;
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			shot->sceneCnt = 1;
			shot->sceneByte = 1;
		}
	}
	if(shot->facing == FACING_NORTH) {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->y.fixed -= syMulFX(sySin(shot->facing), speed);
	}
	else {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->z.fixed += syMulFX(sySin(shot->facing), speed);
	}
}

/* The movable statue's update: as scAirsRockInsideStatueBreathesWhirlwind, but
 */
/* breathing the first of its three whirlwinds that is not on its way. */
void scAirsRockInsideStatueBreathesPooledWhirlwind(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *shot;
	VECTOR3 position;
	EFFECT_OPTIONS options;
	s32 i;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0)
		return;
	/* DEAD: the three whirlwinds looked up, results unused; the ROM keeps the
	 */
	/* three calls. */
	for(i = 0; i != 3;)
		fdActorGetById(ACTOR_HALL_FIRST_WHIRLWIND + i++);
	if(actor->tmr == WHIRLWIND_BREATH_FRAMES) {
		shot = 0;
		if(!syGameFlagGet(FLAG_RIDING_WHIRLWIND))
			fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		if(actor->facing == FACING_NORTH) {
			position.x = actor->x.fixed;
			position.z = actor->z.fixed;
		}
		else {
			position.x =
				actor->x.fixed + syMathMulFX(toFIXED(8), syCos(actor->facing));
			position.z =
				actor->z.fixed + syMathMulFX(toFIXED(8), sySin(actor->facing));
		}
		for(i = 0; i != 3; i++) {
			shot = fdActorGetById(ACTOR_HALL_FIRST_WHIRLWIND + i);
			if(shot->update == 0)
				break;
		}
		shot->x.fixed = position.x;
		shot->y.fixed = actor->y.fixed;
		shot->z.fixed = position.z;
		fdActorSelectAnimation(shot, 2);
		fdActorSetSprFlags(shot, 0);
		((FIELD_SPR *)shot->spr)->pri = 1;
		shot->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
		shot->motionFlg = 0;
		shot->sceneCnt = 0;
		shot->sceneByte = 0;
		shot->facing = actor->facing;
		shot->watchedActor = (FIELD_OBJECT *)actor;
		shot->update = scAirsRockInsideMovePooledWhirlwind;
		actor->tmr = 0;
		options.pri = 1;
		options.pal = 7;
		scFieldEffectSpawn(position.x, actor->y.fixed, position.z - toFIXED(6),
						   0, 0, 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
						   &options);
	}
	actor->tmr++;
}

/* Wakes the hall's movable statue as scAirsRockInsideWakeStatue does. */
void scAirsRockInsideWakeMovableStatue(ACTOR *actor, s16 timer,
									   u8 landingLayer) {
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		actor->steeringFlg = 0;
		actor->tmr = timer;
		actor->sceneCnt = landingLayer;
		actor->update = (s32)scAirsRockInsideStatueBreathesPooledWhirlwind;
		fdActorSetSprFlags(actor, 0);
	}
}

/* A vent's puff of steam: it drifts and grows until its time runs out; */
/* reaching the leader it bounces back and carries him */
/* (TRIGGER_PUFF_REACHES_LEADER), and it bounces off the room's pillars. */
void scAirsRockInsideUpdateVentPuff(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *other;
	s32 *pos;
	u8 i;

	if(((PALETTE_FADE *)Ram_HeapSlots->palFade)->duration != 0)
		return;
	if(state->transitionActive == 0)
		return;
	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0) {
		fdActorSetAnimationSpeed(actor, 0);
		return;
	}
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL);
	if(actor->tmr == 0) {
		if(actor->x.fixed == 0) {
			fdActorRelease(actor);
		}
		else {
			actor->z.fixed = 0;
			actor->x.fixed = 0;
		}
		return;
	}
	if(scPosFlatDistance(&leader->x.fixed, pos = &actor->x.fixed) <= 8 &&
	   leader->y.part[0].integer == actor->y.part[0].integer) {
		state->raisedTrigger = TRIGGER_PUFF_REACHES_LEADER;
		actor->sceneWord = 0;
		actor->speedLimit = -actor->speedLimit >> 1;
		actor->z.fixed = leader->z.fixed - toFIXED(8);
	}
	if(MainRoutineStateWork.next ==
	   SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		other = fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR);
		if(scPosFlatDistance(&other->x.fixed, pos) <= 8) {
			actor->speedLimit = -actor->speedLimit >> 1;
			actor->sceneWord = 0;
			actor->z.fixed = other->z.fixed - toFIXED(8);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		for(i = 0; i != 3; i++) {
			ACTOR *pillar = fdActorGetById(i + ACTOR_FACE_VENTS_THIRD_PILLAR);

			if(scPosFlatDistance(&pillar->x.fixed, pos) <= 8) {
				actor->speedLimit = -actor->speedLimit >> 1;
				actor->sceneWord = 0;
				actor->z.fixed = pillar->z.fixed - toFIXED(8);
			}
		}
	}
	actor->tmr--;
	actor->z.fixed += actor->speedLimit;
	actor->x.fixed += actor->acceleration;
	actor->speedLimit -= actor->sceneWord;
	actor->scaleX += FIXED_ONE / 20;
	actor->scaleY += FIXED_ONE / 20;
}

/* A puff of steam blown from a vent. */
void scAirsRockInsidePuffVent(s32 x, s32 y, s32 z) {
	ACTOR *actor = syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);

	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdActorSetSprCol(actor, SPRITE_COLOR_GREY);
	fdActorSetBlendMode(actor, 1);
	actor->tmr = 27;
	actor->motionFlg = 0;
	actor->steeringFlg = 0;
	actor->scaleX = FIXED_ONE / 2;
	actor->speedLimit = toFIXED(4);
	actor->acceleration = ((syRandom() & 0x7fff) << 1) - (toFIXED(1) / 2 - 1);
	actor->sceneWord = toFIXED(7) / 32;
	actor->update = (s32)scAirsRockInsideUpdateVentPuff;
}

/* A task: on two frames in eight, while no menu, Psynergy or item is in use, a
 */
/* vent puffs steam (scAirsRockInsidePuffVent). In the Clarity Circlet chest */
/* room the west vent puffs, or on odd frames the east one until its pillar is
 */
/* placed; in the face vents room a vent near the leader puffs, the scene's */
/* flags passing over a vent a pillar has blocked. */
void scAirsRockInsidePuffVents(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	u32 frame;
	s32 scene;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
	   state->item != 0)
		return;
	frame = FrameCount;
	if((frame & 6) != 0)
		return;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		FIXED x;

		if((frame & 1) &&
		   !syGameFlagGet(
			   FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
			x = toFIXED(376);
		else
			x = toFIXED(280);
		scAirsRockInsidePuffVent(x, 0, toFIXED(616));
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		if(frame & 1) {
			if(FIELD_HALF_CELL(
				   fdActorGetById(MainRoutineStateWork.leaderActorId)
					   ->z.fixed) > 41) {
				if(FIELD_HALF_CELL(
					   fdActorGetById(MainRoutineStateWork.leaderActorId)
						   ->x.fixed) <= 44)
				puffSouthwestVent:
					scAirsRockInsidePuffVent(toFIXED(168), 0, toFIXED(603));
				else if(!syGameFlagGet(FLAG_MIDDLE_WEST_VENT_STILL))
					scAirsRockInsidePuffVent(toFIXED(504), 0, toFIXED(456));
				else
					goto puffMiddleEastVent;
			}
			else {
			puffNorthwestVent:
				scAirsRockInsidePuffVent(toFIXED(616), -toFIXED(32),
										 toFIXED(136));
			}
		}
		else {
			if(FIELD_HALF_CELL(
				   fdActorGetById(MainRoutineStateWork.leaderActorId)
					   ->z.fixed) > 41) {
				if(FIELD_HALF_CELL(
					   fdActorGetById(MainRoutineStateWork.leaderActorId)
						   ->x.fixed) > 49)
				puffMiddleEastVent:
					scAirsRockInsidePuffVent(toFIXED(552), 0, toFIXED(456));
				else if(syGameFlagGet(FLAG_SOUTHEAST_VENT_STILL))
					goto puffSouthwestVent;
				else
					scAirsRockInsidePuffVent(toFIXED(264), 0, toFIXED(600));
			}
			else if(syGameFlagGet(FLAG_NORTHEAST_VENT_STILL)) {
				goto puffNorthwestVent;
			}
			else {
				scAirsRockInsidePuffVent(toFIXED(648), -toFIXED(32),
										 toFIXED(136));
			}
		}
	}
}

/* An actor a gust carries: it rides 8 pixels above the gust, its draw order */
/* flickering. */
void scAirsRockInsideFollowGust(ACTOR *actor) {
	ACTOR *target = &actor->watchedActor->actor;

	actor->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (FrameCount & 1);
	SET_ACTOR_POSITION(actor, target->x.fixed, target->y.fixed + toFIXED(8),
					   target->z.fixed);
}

void scAirsRockInsideSpiralGust(ACTOR *actor);

/* The actor the gust is cast at: the field effect's target. */
#define TARGET(effect) ((effect)->tgt)

/* The half cell where a gust drops what it carries: x in the high half and z */
/* in the low. */
#define GUST_DROP(x, z)	  (((x) / 8 << 16) | (z) / 8)
#define GUST_DROP_X(drop) ((drop) >> 16)

/* The gust picks up an actor it passes over, to drop it at `at` */
/* (GUST_DROP). */
#define PICK_UP(actor, at)                                                     \
	do {                                                                       \
		(actor)->watchedActor = (FIELD_OBJECT *)gust;                          \
		(actor)->update = scAirsRockInsideFollowGust;                          \
		(actor)->motionFlg = 0;                                                \
		(actor)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;                          \
		fdActorSetSprFlags(actor, 0);                                          \
		((FIELD_SPR *)(actor)->spr)->pri = ((FIELD_SPR *)gust->spr)->pri;      \
		gust->watchedActor = (FIELD_OBJECT *)(actor);                          \
		carried = (at);                                                        \
	} while(0)

/* One shake of the walls, with the grinding of a gate. */
#define SHAKE()                                                                \
	(sySndPlay(SE_GATE_MOVE), fdMapGridSetShake(5, 5, 1),                      \
	 fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9))

/* Whirlwind on a Wind Stone: a gust rises from it and blows away from the */
/* leader until a wall or something on the overhead layer stops it, carrying */
/* the room's pillar or statue it passes (GUST_DROP). A golden barrier in its */
/* way is blown away, and the turning bridge it reaches turns with a shake, */
/* unless the Elixir chest room's pillar stands on it. */
void scAirsRockInsideBlowGust(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	ACTOR *gust, *spark, *actor;
	VECTOR3 velocity;
	EFFECT_OPTIONS glints, dust;
	CELL dir, cell;
	s32 i, steps = 0, carried;
	s32 x, z;
	s32 hit = 0;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_RUSH);
	gust = syImportActorCreate(SPRITE_WHIRLWIND, leader->x.fixed,
							   leader->y.fixed, leader->z.fixed);
	fdActorSelectAnimation(gust, 2);
	fdActorSetSprFlags(gust, 0);
	gust->scaleX = FIXED_ONE * 17 / 10;
	gust->scaleY = FIXED_ONE / 5;
	gust->motionFlg = 0;
	((FIELD_SPR *)gust->spr)->pri = ((FIELD_SPR *)TARGET(effect)->spr)->pri;
	fdActorSetMoveTarget(gust, TARGET(effect)->x.fixed, TARGET(effect)->y.fixed,
						 TARGET(effect)->z.fixed - toFIXED(1));
	gust->groundHeight = fdMapGetTerrainHeight(0, gust->x.fixed, gust->z.fixed);
	while(gust->scaleY < FIXED_ONE * 13 / 10) {
		gust->scaleY += FIXED_ONE / 10;
		syFieldEventWait(1);
	}
	fdActorWaitForMove(gust);
	gust->z.fixed = TARGET(effect)->z.fixed - toFIXED(1);
	while(gust->scaleY < FIXED_ONE * 18 / 10) {
		if((FrameCount & 3) == 0) {
			spark = syImportActorCreate(
				SPRITE_PARTICLE, TARGET(effect)->x.fixed,
				TARGET(effect)->y.fixed, TARGET(effect)->z.fixed);
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->groundHeight =
				fdMapGetTerrainHeight(0, spark->x.fixed, spark->z.fixed);
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(4) / 5;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralGust;
		}
		if(FrameCount & 1) {
			gust->scaleX += FIXED_ONE / 100;
			gust->scaleY += FIXED_ONE / 100;
			fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_HIDDEN);
		}
		else {
			fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_GBR);
		}
		syFieldEventWait(1);
	}
	fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_NORMAL);
	sySndPlay(SE_WHOOSH);
	cell.x = FIELD_CELL(TARGET(effect)->x.fixed);
	cell.z = FIELD_CELL(TARGET(effect)->z.fixed);
	if(FIELD_CELL(leader->x.fixed) < cell.x) {
		dir.x = 1;
		dir.z = 0;
	}
	else if(FIELD_CELL(leader->x.fixed) > cell.x) {
		dir.x = -1;
		dir.z = 0;
	}
	else if(FIELD_CELL(leader->z.fixed) < cell.z) {
		dir.x = 0;
		dir.z = 1;
	}
	else {
		dir.x = 0;
		dir.z = -1;
	}
	do {
		hit = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, cell.x, cell.z);
		hit >>= 8;
		if(fdMapGetTerrainHeight(2, FIELD_CELL_ORIGIN(cell.x),
								 FIELD_CELL_ORIGIN(cell.z)) !=
		   gust->groundHeight)
			hit = 0;
		cell.x += dir.x;
		cell.z += dir.z;
		steps += 2;
	} while(hit == 0);
	fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, cell.x, cell.z);
	steps -= 2;
	x = FIXEDtoINT(TARGET(effect)->x.fixed) + ((steps * dir.x) << 3);
	z = FIXEDtoINT(TARGET(effect)->z.fixed) + ((steps * dir.z) << 3);
	((FIELD_SPR *)gust->spr)->pri = 1;
	/* cell takes the gust's target in pixels: the ROM stores both words in */
	/* cell's slots before the move, though nothing reads them. */
	cell.x = x;
	cell.z = z;
	gust->speedLimit = toFIXED(4);
	gust->acceleration = toFIXED(4);
	fdActorSetMoveTarget(gust, INTtoFIXED(cell.x), gust->y.fixed,
						 INTtoFIXED(cell.z));
	fdCameraSetSpeedAndAcceleration(5, 5);
	fdCameraMoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
	carried = 0;
	while(fdActorHasArrived(gust) == 0) {
		if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
			actor = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR);
			if(FIELD_HALF_CELL(gust->x.fixed) ==
				   FIELD_HALF_CELL(actor->x.fixed) &&
			   carried == 0)
				PICK_UP(actor, GUST_DROP(312, 296));
		}
		else if(MainRoutineStateWork.next ==
				SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
			actor = fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR);
			if(FIELD_CELL(gust->x.fixed) == FIELD_CELL(actor->x.fixed) &&
			   FIELD_CELL(gust->z.fixed) == FIELD_CELL(actor->z.fixed) &&
			   carried == 0)
				PICK_UP(actor, GUST_DROP(328, 840));
		}
		else if(MainRoutineStateWork.next ==
				SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
			actor = fdActorGetById(ACTOR_COINS_CHEST_STATUE);
			if(FIELD_CELL(gust->x.fixed) == FIELD_CELL(actor->x.fixed) &&
			   FIELD_CELL(gust->z.fixed) == FIELD_CELL(actor->z.fixed) &&
			   carried == 0)
				PICK_UP(actor, GUST_DROP(120, 408));
		}
		if((FrameCount & 1) == 0) {
			glints.pri = 1;
			glints.type = SPRITE_GLINT;
			glints.pal = 2;
			velocity.x = syCos((FrameCount & 0x1ff) * (TRIG_TURN / 16)) << 1;
			velocity.y = gust->y.fixed + INTtoFIXED(syRandom() & 31);
			velocity.z = sySin((FrameCount & 0x1ff) * (TRIG_TURN / 16));
			scFieldEffectSpawn(gust->x.fixed, velocity.y, gust->z.fixed,
							   velocity.x, 0, velocity.z,
							   EFFECT_USE_TYPE | EFFECT_USE_PRIORITY |
								   EFFECT_USE_PALETTE,
							   &glints);
		}
		syFieldEventWait(1);
	}
	if(carried != 0) {
		actor = &gust->watchedActor->actor;
		actor->x.fixed = FIELD_HALF_CELL_ORIGIN(GUST_DROP_X(carried));
		/* The x half shifts out, leaving the z one. */
		actor->z.fixed = FIELD_HALF_CELL_ORIGIN(carried);
		actor->velY = toFIXED(8);
		actor->update = 0;
		actor->motionFlg = ACTOR_FALLS;
		actor->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		actor->priFlg &= ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
		fdActorSetSprFlags(actor, 1);
		scSetActorSprPriority(actor, 2);
		if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
			actor->groundHeight = -toFIXED(32);
			fdMapGridCopyCellAttributes(19, 16, 1, 1, 19, 18);
			syGameFlagSet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
		}
		else if(MainRoutineStateWork.next ==
				SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
			fdMapGridCopyCellAttributes(19, 38, 1, 1, 20, 38);
			fdActorWaitUntilLanded(actor);
			sySynch(1);
			fdActorWaitUntilLanded(actor);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			fdMapGridSetCellHeight(toFIXED(320), toFIXED(832), 0, 4);
		}
		else if(MainRoutineStateWork.next ==
				SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
			fdActorWaitUntilLanded(actor);
			sySynch(1);
			fdActorWaitUntilLanded(actor);
			fdActorSetSprFlags(actor, 0);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			fdMapGridSetCellHeight(toFIXED(120), toFIXED(408), 0, 4);
			if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
				scAirsRockInsideWakeStatue(actor, 60, 0);
		}
	}
	if(hit == CELL_BEHAVIOR_WALL) {
		sySndPlay(SE_WHOOSH);
	}
	else {
		cell.x = FIXEDtoINT(gust->x.fixed) + dir.x * 48;
		cell.z = FIXEDtoINT(gust->z.fixed) + dir.z * 48;
		gust->speedLimit = toFIXED(1) / 2;
		gust->acceleration = toFIXED(1) / 2;
		scFieldActorSetMoveTargetFX(gust, INTtoFIXED(cell.x), gust->y.fixed,
									INTtoFIXED(cell.z));
		hit -= CELL_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP;
		i = 0;
		while(fdActorHasArrived(gust) == 0) {
			if(hit > GUST_STOP_LAST_BARRIER)
				goto shrink;
			if(i == 0) {
				sySndPlay(SE_GUST);
				i = 1;
			}
			if((FrameCount & 1) == 0) {
				dust.pri = 1;
				dust.startScaleX = dust.startScaleY =
					(syRandom() & 0xffff) + FIXED_ONE;
				velocity.x = syCos((FrameCount & 0x1ff) * (TRIG_TURN / 16))
							 << 1;
				velocity.y = toFIXED(1);
				velocity.z = sySin((FrameCount & 0x1ff) * (TRIG_TURN / 16));
				scFieldEffectSpawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
								   velocity.x, velocity.y, velocity.z,
								   EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
								   &dust);
			}
			if((FrameCount & 3) == 0) {
				if(MainRoutineStateWork.next ==
				   SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
					syGameFlagSet(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
					cell.x = FIELD_CELL(gust->x.fixed) + 64;
					cell.z = FIELD_CELL(gust->z.fixed);
					fdMapGridCopyCellTilesIndicesRect(90, 25, cell.x + 1,
													  cell.z, 1, 3);
					fdMapGridCopyCellTilesIndicesRect(21, 88, 24, 89, 1, 2);
					fdMapCopyCellAttributeRect(23, 92, 2, 1, 23, 89);
					fdMapCopyCellAttributeRect(18, 33, 2, 1, 23, 25);
				}
				else if(MainRoutineStateWork.next ==
						SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
					cell.x = FIELD_CELL(gust->x.fixed);
					cell.z = FIELD_CELL(gust->z.fixed - gust->y.fixed) + 64;
					if(hit == GUST_STOP_PAIRED_STAKES_NORTH_BARRIER) {
						syGameFlagSet(
							FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY);
						fdMapGridCopyCellTilesIndicesRect(39, 82, cell.x,
														  cell.z, 1, 1);
						fdMapCopyCellAttributeRect(39, 82, 1, 1, 38, 82);
						fdMapGridCopyCellAttributes(38, 19, 1, 1, 38, 18);
					}
					else {
						syGameFlagSet(
							FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY);
						fdMapGridCopyCellsTo(39, 82, cell.x, cell.z, 1, 1);
						fdMapGridCopyCellAttributes(38, 98, 1, 1, 38, 99);
						fdMapGridCopyCellAttributes(38, 35, 1, 1, 38, 37);
					}
				}
				else if(MainRoutineStateWork.next ==
						SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
					syGameFlagSet(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY);
					cell.x = FIELD_CELL(gust->x.fixed);
					fdMapGridCopyCellsTo(15, 80, cell.x, 77, 1, 3);
					fdMapGridCopyCellAttributes(15, 80, 1, 1, 15, 79);
					fdMapGridCopyCellAttributes(17, 15, 2, 1, 15, 15);
				}
				fdMapGridCenterOnFollowedPoint();
			}
			syFieldEventWait(1);
		}
		sySndPlay(SE_STOP_EXIT_SE);
	}
shrink:
	gust->acceleration = 0;
	gust->speedLimit = 0;
	gust->tgtZ = 0;
	gust->tgtY = 0;
	gust->tgtX = 0;
	for(i = 0; i != 8; i++) {
		gust->scaleX -= FIXED_ONE * 3 / 20;
		gust->scaleY -= FIXED_ONE / 4;
		gust->y.fixed += toFIXED(7) / 2;
		syFieldEventWait(1);
	}
	fdActorRelease(gust);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		if(hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
			SHAKE();
			fdMapGridCopyCellsTo(79, 64, 29, 85, 5, 7);
			syFieldEventWait(30);
			SHAKE();
			fdMapGridCopyCellsTo(69, 64, 29, 85, 5, 7);
			fdMapGridCopyCellAttributes(46, 20, 5, 5, 29, 21);
			fdMapGridCopyCellAttributes(32, 89, 1, 1, 31, 89);
			fdMapGridCopyCellAttributes(50, 86, 1, 1, 33, 87);
			syGameFlagSet(FLAG_TIGHTROPES_BRIDGE_TURNED);
		}
		else if(hit == GUST_STOP_BRIDGE_EAST_WEST) {
			SHAKE();
			fdMapGridCopyCellsTo(74, 64, 29, 85, 5, 7);
			syFieldEventWait(30);
			SHAKE();
			fdMapGridCopyCellsTo(64, 64, 29, 85, 5, 7);
			fdMapGridCopyCellAttributes(52, 20, 5, 5, 29, 21);
			fdMapGridCopyCellAttributes(33, 88, 1, 1, 33, 87);
			fdMapGridCopyCellAttributes(48, 88, 1, 1, 31, 89);
			syGameFlagClear(FLAG_TIGHTROPES_BRIDGE_TURNED);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		if(hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
			SHAKE();
			fdMapGridCopyCellsTo(79, 64, 32, 79, 5, 7);
			if(FIELD_HALF_CELL(
				   fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
			   ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
				velocity.x =
					fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed -
					toFIXED(8);
				velocity.y = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
				velocity.z =
					fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed +
					toFIXED(8);
				fdActorSetSprPriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
				for(i = 0; i != 8; i++) {
					dust.pri = 1;
					scFieldEffectSpawn(velocity.x + INTtoFIXED(syRandom() & 15),
									   velocity.y + INTtoFIXED(syRandom() & 31),
									   velocity.z, 0, 0, -(toFIXED(1) / 2),
									   EFFECT_USE_PRIORITY, &dust);
				}
				sySynch(30);
				SHAKE();
				fdMapGridCopyCellsTo(64, 64, 32, 79, 5, 7);
				fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->priFlg |=
					ACTOR_PRIORITY_AUTOMATIC;
			}
			else {
				syFieldEventWait(30);
				SHAKE();
				fdMapGridCopyCellsTo(69, 64, 32, 79, 5, 7);
				fdMapGridCopyCellAttributes(49, 15, 5, 5, 32, 15);
				fdMapGridCopyCellAttributes(33, 82, 1, 1, 34, 82);
				fdMapGridCopyCellAttributes(52, 81, 1, 1, 36, 81);
				syGameFlagClear(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
			}
		}
		else if(hit == GUST_STOP_BRIDGE_EAST_WEST) {
			SHAKE();
			if(FIELD_HALF_CELL(
				   fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
			   ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
				velocity.x =
					fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed -
					toFIXED(8);
				velocity.y = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
				velocity.z = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed;
				for(i = 0; i != 8; i++) {
					dust.pri = 1;
					scFieldEffectSpawn(
						velocity.x + INTtoFIXED(syRandom() & 15), velocity.y,
						velocity.z - INTtoFIXED(syRandom() & 31), 0, 0,
						-(toFIXED(1) / 2), EFFECT_USE_PRIORITY, &dust);
				}
				sySynch(20);
			}
			else {
				fdMapGridCopyCellsTo(79, 64, 32, 79, 5, 7);
				sySndPlay(SE_GATE_MOVE);
				syFieldEventWait(30);
				SHAKE();
				fdMapGridCopyCellsTo(64, 64, 32, 79, 5, 7);
				fdMapGridCopyCellAttributes(49, 21, 5, 5, 32, 15);
				fdMapGridCopyCellAttributes(35, 81, 1, 1, 36, 81);
				fdMapGridCopyCellAttributes(50, 82, 1, 1, 34, 82);
				syGameFlagSet(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
			}
		}
	}
	syFieldEventWait(30);
	fdCameraMoveToActorById(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdEventEnd();
}

extern SCENE_ENTRANCE AirsRockInside_Entrances[];
extern SCENE_ENTRANCE AirsRockInside_HallEntrances[];

const SCENE_ENTRANCE *scGetEntrances(void) {
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInside_HallEntrances;
	return AirsRockInside_Entrances;
}

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInside_HallPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
		return AirsRockInside_VialChestPlacements;
	if(MainRoutineStateWork.next ==
	   SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
		return AirsRockInside_ClarityCircletChestPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
		return AirsRockInside_TightropesPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return AirsRockInside_RevealTabletPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
		return AirsRockInside_ElixirChestPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
		return AirsRockInside_PairedStakesPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
		return AirsRockInside_FaceVentsPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
		return AirsRockInside_CoinsChestPlacements;
	return AirsRockInside_Placements;
}

/* Nothing visible of its own: once Felix stands still on a new cell, the */
/* stepping stone he last stood on is drawn as before; in the Reveal tablet */
/* room, leaving a cell with a collision code raises TRIGGER_LEFT_CELL plus the
 */
/* code, which for a vanishing stone is its trigger */
/* (TRIGGER_FIRST_VANISHING_STONE on). */
void scAirsRockInsideRestoreCellBehindLeader(void) {
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 key = CELL_KEY(x, z);
	s32 last;
	s32 value;
	FIELD_STATE *state;

	if(actor->velX != 0 || actor->velZ != 0)
		return;
	last = AirsRockInside_StampedCell;
	if(last == -1 || key == last)
		return;
	x = CELL_KEY_X(last);
	z = CELL_KEY_Z(last);
	AirsRockInside_StampedCell = -1;
	fdMapGridCopyCellsLayered(73, 72, AirsRockInside_StampedWidth,
							  AirsRockInside_StampedHeight,
							  AirsRockInside_StampedX, AirsRockInside_StampedZ);
	if(MainRoutineStateWork.next != SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return;
	state = Ram_HeapSlots->fieldState;
	value = CELL_CODE(fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z));
	if(value != 0) {
		state->raisedTrigger = TRIGGER_LEFT_CELL + value;
	}
}

/* Starts scAirsRockInsideRestoreCellBehindLeader with no cell stamped yet. */
void scAirsRockInsideStartRestoringCells(void) {
	s32 value = -1;

	AirsRockInside_StampedCell = value;
	sySetEvent(scAirsRockInsideRestoreCellBehindLeader, TASK_PASS_FRAME);
}

/* As Felix steps onto one of the yellow stepping stones marked with a */
/* diamond, in the great hall and the rooms around it, the stone is drawn */
/* again under his feet, in front of what lies behind it, so that he is seen */
/* standing on it; nothing else changes. The stone goes back to its own */
/* picture once he has left it (scAirsRockInsideRestoreCellBehindLeader). */
/* */
/* The cells under him are saved at (73, 72) and the stone's picture kept at */
/* (70, 72) is copied in their place. */
void scAirsRockInsideStampCellUnderLeader(void) {
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 y = FIELD_CELL(actor->y.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 cell = CELL_KEY(x, z);
	s32 scene;

	if(cell == AirsRockInside_StampedCell)
		return;
	AirsRockInside_StampedCell = cell;
	AirsRockInside_StampedWidth = 1;
	AirsRockInside_StampedHeight = 2;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
	   scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST ||
	   scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		AirsRockInside_StampedX = x;
		AirsRockInside_StampedZ = z - y + 64;
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
			scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST ||
			scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES ||
			scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		AirsRockInside_StampedX = x + 64;
		AirsRockInside_StampedZ = z - y;
	}
	fdMapGridCopyCellsLayered(AirsRockInside_StampedX, AirsRockInside_StampedZ,
							  AirsRockInside_StampedWidth,
							  AirsRockInside_StampedHeight, 73, 72);
	fdMapGridCopyCellsLayered(70, 72, AirsRockInside_StampedWidth,
							  AirsRockInside_StampedHeight,
							  AirsRockInside_StampedX, AirsRockInside_StampedZ);
}

/* The same for the stepping stones of a second kind, in the hall, the Vial */
/* chest room, the Reveal tablet's room and the Elixir chest room: the stone */
/* is drawn again under Felix's feet from the picture kept at (71, 72). */
void scAirsRockInsideStampOtherCellUnderLeader(void) {
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 y = FIELD_CELL(actor->y.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 cell = CELL_KEY(x, z);
	s32 scene;

	if(cell == AirsRockInside_StampedCell)
		return;
	AirsRockInside_StampedCell = cell;
	AirsRockInside_StampedWidth = 1;
	AirsRockInside_StampedHeight = 2;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
	   scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		AirsRockInside_StampedX = x;
		AirsRockInside_StampedZ = z - y + 64;
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
			scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		AirsRockInside_StampedX = x + 64;
		AirsRockInside_StampedZ = z - y;
	}
	fdMapGridCopyCellsLayered(AirsRockInside_StampedX, AirsRockInside_StampedZ,
							  AirsRockInside_StampedWidth,
							  AirsRockInside_StampedHeight, 73, 72);
	fdMapGridCopyCellsLayered(71, 72, AirsRockInside_StampedWidth,
							  AirsRockInside_StampedHeight,
							  AirsRockInside_StampedX, AirsRockInside_StampedZ);
}

/* As Felix steps onto a stepping stone stacked on another, three cells of */
/* the picture under him are saved at (73, 72) and the stack's picture kept */
/* at (72, 72) is drawn in their place. */
#define DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(name, n1)                    \
	void name(void) {                                                          \
		ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());                \
		s32 x = FIELD_CELL(actor->x.fixed);                                    \
		s32 y = FIELD_CELL(actor->y.fixed);                                    \
		s32 z = FIELD_CELL(actor->z.fixed);                                    \
		s32 key = CELL_KEY(x, z);                                              \
                                                                               \
		if(key != AirsRockInside_StampedCell) {                                \
			AirsRockInside_StampedCell = key;                                  \
			AirsRockInside_StampedWidth = 1;                                   \
			AirsRockInside_StampedHeight = 3;                                  \
			AirsRockInside_StampedX = x;                                       \
			AirsRockInside_StampedZ = z - y + n1;                              \
			fdMapGridCopyCellsLayered(x, AirsRockInside_StampedZ, 1, 3, 73,    \
									  72);                                     \
			fdMapGridCopyCellsLayered(72, 72, AirsRockInside_StampedWidth,     \
									  AirsRockInside_StampedHeight,            \
									  AirsRockInside_StampedX,                 \
									  AirsRockInside_StampedZ);                \
		}                                                                      \
	}

/* As Felix steps onto one of the stacked stepping stones in the great hall, */
/* the stack is drawn again under his feet, in front of the wall behind it, */
/* so that he is seen standing on top of it; nothing else changes. */
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
	scAirsRockInsideStampTallCellUnderLeader, 64)

/* The same for the stacked stepping stones one row nearer the screen. */
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_CELL(
	scAirsRockInsideStampTallCellUnderLeaderNearer, 63)

/* A burst of eight puffs of dust around x, z, and the walls shake. */
void scAirsRockInsideBurstDust(s32 x, s32 z) {
	EFFECT_OPTIONS options;
	VECTOR3 spawn;
	s32 i;

	options.pri = 1;
	options.startScaleX = FIXED_ONE * 2;
	options.startScaleY = FIXED_ONE * 2;
	options.tgtScaleX = FIXED_ONE / 2;
	options.tgtScaleY = FIXED_ONE / 2;
	for(i = 0; i != 8; i++) {
		spawn.x = x + INTtoFIXED(syRandom() & 63) - toFIXED(32);
		spawn.y = (syRandom() & 15) << 12;
		spawn.z = z + INTtoFIXED(syRandom() & 7) - toFIXED(16);
		scFieldEffectSpawn(spawn.x, 0, spawn.z, 0, spawn.y, 0,
						   EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PRIORITY | 1,
						   &options);
	}
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/* The sparks scAirsRockInsideScatterSparks raises, 5 frames apart. */
enum { SCATTERED_SPARKS = 12 };

/* Sparks rise one after another around x, z. */
void scAirsRockInsideScatterSparks(s32 x, s32 z) {
	EFFECT_OPTIONS options;
	VECTOR3 position;
	s32 i;

	options.type = SPRITE_PARTICLE;
	options.pri = 1;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 3;
	for(i = 0; i != SCATTERED_SPARKS; i++) {
		position.x = x + INTtoFIXED(syRandom() & 31) - toFIXED(16);
		position.y = toFIXED(4);
		position.z = z;
		scFieldEffectSpawn(position.x, 0, position.z, 0, position.y, 0,
						   EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PRIORITY,
						   &options);
		syFieldEventWait(5);
	}
}

/* A gust's spark: circles the centre kept in bounce and sceneWord, a lap every
 */
/* 32 frames, its radius growing as it rises above the ground. */
void scAirsRockInsideSpiralGust(ACTOR *actor) {
	FIXED radius = ((actor->y.fixed - actor->groundHeight) >> 1) + toFIXED(16);

	actor->x.fixed =
		actor->bounce +
		syMulFX(syCos((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius);
	actor->z.fixed =
		actor->sceneWord +
		syMulFX(sySin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius) /
			3;
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
}

/* A spark: circles the centre kept in bounce and sceneWord on an ellipse twice
 */
/* as wide as deep, the radius growing with its height; it rises by its gravity
 */
/* each frame and turns blue (SPRITE_COLOR_BLUE_TINT) and back every two */
/* frames. */
void scAirsRockInsideSpiralSpark(ACTOR *actor) {
	FIXED y = actor->y.fixed;
	s32 phase;
	FIXED radius;

	phase = actor->speedLimit;
	radius = toFIXED(12) + (y >> 2);
	actor->x.fixed =
		actor->bounce +
		syMulFX(syCos((phase & 0xff) * (TRIG_TURN / 32)), radius) * 2;
	actor->z.fixed =
		actor->sceneWord +
		syMulFX(sySin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius);
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
	fdActorSetSprCol(actor, ((FrameCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/* A Whirlwind target's update while the gust gathers: it flickers to the */
/* colour mode in sceneCnt (a SPR_COL_MODE), at the rate sceneByte */
/* sets. */
void scAirsRockInsideFlickerTarget(ACTOR *actor) {
	s32 bit = ((FrameCount & 3) >> actor->sceneByte) & 1;

	fdActorSetSprCol(actor, (u8)(bit * actor->sceneCnt));
}

/* Felix casts Whirlwind on the twister rock in the great hall: it glows, a */
/* ball of purple light flies north up the hall to the great stone face, */
/* whose eyes light red as its mouth opens with a rumble and rocks fall, and */
/* from then on its statues breathe whirlwinds the party can ride. The rock */
/* turns grey, spent. */
void scAirsRockInsideWakeHallStatue(void) {
	u16 *buffer = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	u16 *cell;
	ACTOR *spark;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIoWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES),
						 buffer);
	syDMAXCopy(buffer, (void *)BG_CHAR_BLOCK(0),
			   DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buffer);
	btDmaFill16(0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK),
				BG_SCREEN_SIZE);
	buffer =
		syHeapAlloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP),
						 buffer);
	cell = buffer;
	/* The map's tile numbers less 512. */
	for(i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
		*cell &= 0xfcff;
		cell++;
	}
	cell = buffer;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(cell,
				   (void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
							i * MAP_ROW_BYTES),
				   DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4),
				   REG_DMA3);
		cell += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buffer);
	sySndPlay(SE_GLITTER);
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->sceneCnt = SPRITE_COLOR_BLUE_TINT;
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->sceneByte = 1;
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->update =
		scAirsRockInsideFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((FrameCount & 3) == 0) {
			spark = syImportActorCreate(
				SPRITE_PARTICLE,
				fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->x.fixed,
				fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->y.fixed,
				fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->z.fixed);
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->spr)
					->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralSpark;
		}
		syFieldEventWait(1);
	}
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeedFX(toFIXED(1), toFIXED(1) / 8);
	fdCameraMoveToFX(toFIXED(312), -1, toFIXED(680), 1);
	fdCameraWaitForArrival();
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->update = 0;
	fdActorSetSprCol(fdActorGetById(ACTOR_HALL_TWISTER_ROCK),
					 SPRITE_COLOR_GREY);
	REG_BLDALPHA = BLEND_ALPHA(16, 16);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	BgScrollWork[0].x = -104;
	BgScrollWork[0].y = 42;
	sySynch(30);
	sySndPlay(SE_ITEM_BREAK);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdPaletteFadeSetSource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(8);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridCopyCellTilesIndicesRect(64, 78, 18, 105, 3, 3);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	for(i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
		REG_BLDALPHA =
			BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	fdPaletteFadeSetSource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	sySynch(30);
	sySndPlay(SE_BOSS_ROAR);
	scAirsRockInsideBurstDust(toFIXED(312), toFIXED(720));
	fdMapGridCopyCellsTo(64, 78, 18, 105, 3, 5);
	fdCameraSetSpeedParms(toFIXED(3) / 2, toFIXED(3) / 16);
	fdCameraMoveToActorById(ACTOR_HALL_TWISTER_STATUE, 1);
	fdCameraWaitForArrival();
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetSprColById(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_BLINK);
	scAirsRockInsideScatterSparks(
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->x.fixed,
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->z.fixed);
	fdActorConfigureSprCol(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(fdActorGetById(ACTOR_HALL_TWISTER_STATUE),
					 SPRITE_COLOR_WHITE);
	sySynch(3);
	fdActorSetSprCol(fdActorGetById(ACTOR_HALL_WEST_STATUE),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(fdActorGetById(ACTOR_HALL_EAST_STATUE),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(fdActorGetById(ACTOR_HALL_TWISTER_STATUE),
					 SPRITE_COLOR_NORMAL);
	sySynch(40);
	scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
	syWindowLoadGraphics();
	syDebugTextClear();
	mnWindowColApply(MainRoutineStateWork.winHue,
					 MainRoutineStateWork.winBrightness);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	syGameFlagSet(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN);
	fdEventEnd();
}

/* Walking north into the door at the top of the hall's tower: it opens with a
 */
/* thump, and the leader walks through into the Reveal tablet's room. */
void scAirsRockInsideOpenTabletRoomDoor(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 width;
	s32 height;

	if(CELL_CODE(fdMapCellGetAttributes(MAPGRID_LAYER_MIDDLE,
										FIELD_CELL(leader->x.fixed),
										FIELD_CELL(leader->z.fixed) - 1)) !=
	   CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR)
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_THUMP);
	width = 1;
	height = 2;
	fdMapGridCopyCellTilesIndicesRect(65, 76, 19, 92, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellTilesIndicesRect(66, 76, 19, 92, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellTilesIndicesRect(67, 76, 19, 92, width, height);
	syFieldEventWait(8);
	fdMapGridCopyCellTilesIndicesRect(62, 76, 19, 92, width, height);
	syFieldEventWait(15);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorGetById(MainRoutineStateWork.leaderActorId)->speedLimit =
		fdActorGetById(MainRoutineStateWork.leaderActorId)->acceleration =
			toFIXED(1) / 2;
	sySndPlay(SE_MAP_EXIT);
	fdActorWalkToById(MainRoutineStateWork.leaderActorId, 312, 792);
	syFieldEventWait(5);
	fdEventSetExitRequest(
		AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_REVEAL_TABLET);
	fdEventEnd();
}

/* Nothing visible happens of its own as Felix starts to push a pillar or a */
/* statue inside the rock, or casts Move on one; the game takes it off the */
/* cells it was blocking so that it can slide. In the paired statues room it */
/* also smooths two cells' ground, and in the coins chest room the statue */
/* being moved stops breathing whirlwinds. */
void scAirsRockInsideStartPushingPillar(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL) {
		fdActorLiftTargetBlock(AirsRockInside_HallPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		fdActorLiftTargetBlock(AirsRockInside_VialChestPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		fdActorLiftTargetBlock(AirsRockInside_ClarityCircletChestPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		fdActorLiftTargetBlock(AirsRockInside_TightropesPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		syFieldMapGridSetCellHeight(toFIXED(608), toFIXED(336), 2, 0);
		fdMapGridSetCellHeight(toFIXED(624), toFIXED(336), 2, 0);
		fdActorLiftTargetBlock(AirsRockInside_PairedStakesPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		fdActorLiftTargetBlock(AirsRockInside_FaceVentsPillars);
	}
	else if(scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		fdActorGetById(ACTOR_COINS_CHEST_STATUE)->tmr = 0;
		fdActorLiftTargetBlock(AirsRockInside_CoinsChestPillars);
	}
}

/* Once Felix has pushed or moved a pillar or a statue inside the rock: one */
/* left over lower ground drops onto it with a thump, and one left in the */
/* place it belongs stays there on later visits, as a shortcut or a step. */
/* */
/* In the paired statues' room and the face vents room the cells along the */
/* pillar's rows are marked or cleared again, which in the face vents room */
/* decides which vents it keeps still. */
void scAirsRockInsideFinishPushingPillar(void) {
	ACTOR *pushed = ((FIELD_STATE *)Ram_HeapSlots->fieldState)->movedBlock;
	s32 x, z;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorPlaceMovedBlock();
	if(fdMapGetTerrainHeight(2, pushed->x.fixed, pushed->z.fixed) !=
	   pushed->y.fixed) {
		pushed->layer = 2;
		pushed->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorWaitUntilLanded(pushed);
		sySndPlay(SE_THUMP);
		fdActorWaitUntilLanded(pushed);
		pushed->motionFlg = 0;
		pushed->layer = 0;
	}
	x = FIELD_HALF_CELL(pushed->x.fixed);
	z = FIELD_HALF_CELL(pushed->z.fixed);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
		if(x == 103 && z == 41)
			syGameFlagSet(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		if(x == 13 && z == 49)
			syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED);
		else if(x == 41 && z == 49)
			syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED);
	}
	else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		if(x == 33 && z == 79)
			syGameFlagSet(
				FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED);
		else if(x == 77 && z == 71)
			syGameFlagSet(
				FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED);
		else if(x == 59 && z == 65)
			syGameFlagSet(
				FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		if(x == 81 && z == 37)
			syGameFlagSet(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		syFieldMapGridSetCellHeight(toFIXED(608), toFIXED(336), 2, -4);
		syFieldMapGridSetCellHeight(toFIXED(624), toFIXED(336), 2, -4);
		if(x == 43 && z == 87)
			syGameFlagSet(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED);
		if((FIELD_HALF_CELL(
				fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_A)->x.fixed) == 73 &&
			FIELD_HALF_CELL(
				fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_A)->z.fixed) == 43) ||
		   (FIELD_HALF_CELL(
				fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_B)->x.fixed) == 73 &&
			FIELD_HALF_CELL(
				fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_B)->z.fixed) == 43))
			fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneByte = 1;
		else
			fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneByte = 0;
		if(pushed->x.fixed ==
			   fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_C)->x.fixed &&
		   pushed->z.fixed ==
			   fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_C)->z.fixed) {
			if(x == 83 && z == 43)
				fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneByte = 1;
			else
				fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneByte = 0;
		}
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		if(x == 35 && z == 59)
			syGameFlagSet(FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED);
		if(x == 45 && z == 105)
			syGameFlagSet(FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED);
		if(x == 23 && z == 77) {
			syGameFlagSet(FLAG_SOUTHEAST_VENT_STILL);
			syGameFlagSet(FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED);
		}
		if(z == 59 && (x == 67 || x == 71 || x == 73))
			syGameFlagSet(FLAG_MIDDLE_WEST_VENT_STILL);
		if(z == 19 && (x == 75 || x == 79))
			syGameFlagSet(FLAG_NORTHEAST_VENT_STILL);
		if(x == 69 && z == 59)
			syGameFlagClear(FLAG_MIDDLE_WEST_VENT_STILL);
		if(x == 77 && z == 19)
			syGameFlagClear(FLAG_NORTHEAST_VENT_STILL);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed),
			0);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed),
			0);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		if(x == 45 && z == 33)
			syGameFlagSet(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
	}
	fdEventEnd();
}

/* Felix casts Whirlwind on the twister rock in the Vial chest room: it */
/* glows, a ball of purple light flies north to the room's great stone face, */
/* whose eyes light red and whose mouth opens with a roar (the cells from */
/* (12, 77) down), and the statue below it wakes and starts breathing */
/* whirlwinds. The rock turns grey, spent. */
void scAirsRockInsideWakeVialChestStatue(void) {
	u16 *buffer = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	u16 *cell;
	ACTOR *spark;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIoWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES),
						 buffer);
	syDMAXCopy(buffer, (void *)BG_CHAR_BLOCK(0),
			   DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buffer);
	btDmaFill16(0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK),
				BG_SCREEN_SIZE);
	buffer =
		syHeapAlloc(LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(u16));
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP),
						 buffer);
	cell = buffer;
	/* The map's tile numbers less 512. */
	for(i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
		*cell &= 0xfcff;
		cell++;
	}
	cell = buffer;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(cell,
				   (void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
							i * MAP_ROW_BYTES),
				   DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(u16) / 4),
				   REG_DMA3);
		cell += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buffer);
	sySndPlay(SE_GLITTER);
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneCnt =
		SPRITE_COLOR_BLUE_TINT;
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneByte = 1;
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update =
		scAirsRockInsideFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((FrameCount & 3) == 0) {
			spark = syImportActorCreate(
				SPRITE_PARTICLE,
				fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->x.fixed,
				fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->y.fixed,
				fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->z.fixed);
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)
					 ->spr)
					->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			fdActorSelectAnimation(spark, 7);
			fdActorStartScript(spark, FieldEffect_MediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralSpark;
		}
		syFieldEventWait(1);
	}
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeedFX(toFIXED(1), toFIXED(1) / 8);
	fdCameraMoveToFX(toFIXED(216), -1, toFIXED(224), 1);
	fdCameraWaitForArrival();
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update = 0;
	fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK),
					 SPRITE_COLOR_GREY);
	REG_BLDALPHA = BLEND_ALPHA(16, 16);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	BgScrollWork[0].x = -104;
	BgScrollWork[0].y = 42;
	sySynch(30);
	sySndPlay(SE_ITEM_BREAK);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdPaletteFadeSetSource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(8);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridCopyCellTilesIndicesRect(68, 84, 12, 77, 3, 3);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	for(i = 0; i != BLEND_WEIGHT_MAX * 2; i++) {
		REG_BLDALPHA =
			BLEND_ALPHA(BLEND_WEIGHT_MAX - (i >> 1), BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	fdPaletteFadeSetSource(PALETTE_TINT_WARM, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	sySynch(30);
	sySndPlay(SE_BOSS_ROAR);
	scAirsRockInsideBurstDust(toFIXED(216), toFIXED(301));
	fdMapGridCopyCellsTo(68, 84, 12, 77, 3, 6);
	fdCameraSetSpeedParms(toFIXED(3) / 2, toFIXED(3) / 16);
	fdCameraMoveToActorById(ACTOR_VIAL_CHEST_STATUE, 1);
	fdCameraWaitForArrival();
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetSprColById(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_BLINK);
	scAirsRockInsideScatterSparks(
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE)->x.fixed,
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE)->z.fixed + toFIXED(32));
	fdActorConfigureSprCol(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_STATUE),
					 SPRITE_COLOR_WHITE);
	sySynch(3);
	fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_STATUE),
					 SPRITE_COLOR_NORMAL);
	sySynch(40);
	scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_VIAL_CHEST_STATUE), 8, 0);
	syWindowLoadGraphics();
	syDebugTextClear();
	mnWindowColApply(MainRoutineStateWork.winHue,
					 MainRoutineStateWork.winBrightness);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN);
	fdEventEnd();
}

/* Walking south off the Vial chest room's ledge: the leader jumps down, to */
/* land in the hall below (scAirsRockInsideLandInHall). */
void scAirsRockInsideJumpDownLedge(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorCreateSilhouette(toFIXED(216), -toFIXED(32), toFIXED(544),
							SPRITE_CRATE);
	fdActorJumpDownLedge(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING,
						 0);
	fdEventSetExitRequest(
		AIRS_ROCK_INSIDE_VIAL_CHEST_TO_AIRS_ROCK_INSIDE_HALL_SOUTH);
	fdEventEnd();
}

/* A puff of steam from one of the vents in the Clarity Circlet chest room */
/* catches Felix north of the vents and, with a chime, blows him back south */
/* to cell (23, 41) in front of them, without turning him round. South of */
/* them the puff does nothing. */
void scAirsRockInsideRideClarityCircletVent(void) {

	if(fdActorGetById(MainRoutineStateWork.leaderActorId)->z.part[0].integer >
	   653)
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetSpeedParms(MainRoutineStateWork.leaderActorId, toFIXED(4),
						 toFIXED(2));
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 376, 668);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdEventEnd();
}

/* A stepping stone the leader has crossed fades with a drip and is gone. */
void scAirsRockInsideVanishStone(s32 x, s32 y) {
	ACTOR *marker;
	FIELD_SPR *sprite;
	s32 i, cellX, cellY;

	marker = syImportActorCreate(SPRITE_STACKED_CRATES, INTtoFIXED(x), 0,
								 INTtoFIXED(y));
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	scSetActorSprPriority(marker, 0);
	fdActorSetSprFlags(marker, 0);
	sprite = marker->spr;
	sprite->objMode = OBJ_MODE_WINDOW;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
				 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_ALL;
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	sySndPlay(SE_DRIP);
	for(i = 0; i != BLEND_WEIGHT_MAX / 2; i++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, i * 2);
		sySynch(1);
	}
	for(i = 0; i != BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX - i, BLEND_WEIGHT_MAX);
		sySynch(1);
	}
	cellX = x >> 4;
	cellY = (y >> 4) - 1;
	fdMapGridCopyCellsTo(65, 1, cellX + 64, cellY, 1, 2);
	scMapCellSetAttributes(MAPGRID_LAYER_GROUND, cellX, cellY,
						   CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
	fdActorRelease(marker);
	fdEventEnd();
	REG_BLDCNT = 0;
	REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

/* Felix steps off one of the stepping stones that cross the Reveal tablet's */
/* room, and it fades away behind him with a drip, leaving a gap, for good. */
/* Stepping from a stone back onto the floor at either end of the row, the */
/* stone goes only once another already has. */
/* */
/* The floor at the ends is read as */
/* CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END and _NORTH_END on the */
/* overhead layer; FLAG_STONE_VANISHED says a stone has gone, and each stone */
/* has its own flag. */
void scAirsRockInsideStepOffVanishingStone(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader;
	s32 cell, row = 54;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	cell = fdMapCellGetAttributes(MAPGRID_LAYER_OVERHEAD,
								  FIELD_CELL(leader->x.fixed),
								  FIELD_CELL(leader->z.fixed));
	/* The stone's row in half cells: the first's is 50, each next four north.
	 */
	row -= (state->raisedTrigger - TRIGGER_FIRST_VANISHING_STONE + 1) << 2;
	cell = CELL_CODE(cell);
	if(cell != CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END &&
	   cell != CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END) {
		scAirsRockInsideStampCellUnderLeader();
		scAirsRockInsideVanishStone(264, row * 8 + 10);
		if(!fdEventGetFlag(FLAG_STONE_VANISHED))
			syGameFlagSet(FLAG_STONE_VANISHED);
		syGameFlagSet(state->raisedTrigger - TRIGGER_FIRST_VANISHING_STONE +
					  FLAG_FIRST_STONE_VANISHED);
	}
	else if(syGameFlagGet(FLAG_STONE_VANISHED)) {
		scAirsRockInsideVanishStone(264, row * 8 + 10);
	}
}

/* As Felix steps onto one of the stepping stones on row 19 of the Reveal */
/* tablet's room, beside its two hidden doorways, it lights up gold under his */
/* feet, and goes back to yellow once he has left it. */
/* */
/* Two cells of the overhead layer under him are saved at (73, 72) and the */
/* lit stone's picture kept at (70, 74) is drawn in their place. */
void scAirsRockInsideStampWideCellUnderLeader(void) {
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 y = FIELD_CELL(actor->y.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	s32 key = CELL_KEY(x, z);

	if(key != AirsRockInside_StampedCell) {
		AirsRockInside_StampedCell = key;
		AirsRockInside_StampedWidth = 1;
		AirsRockInside_StampedHeight = 2;
		AirsRockInside_StampedX = x + 64;
		AirsRockInside_StampedZ = z - y;
		fdMapGridCopyCellsLayered(AirsRockInside_StampedX,
								  AirsRockInside_StampedZ, 1, 2, 73, 72);
		fdMapGridCopyCellsLayered(
			70, 74, AirsRockInside_StampedWidth, AirsRockInside_StampedHeight,
			AirsRockInside_StampedX, AirsRockInside_StampedZ);
	}
}

extern u32 OrbitSpark_Script[];

/* Defines a function that sets the Reveal tablet room's two hidden doorways, */
/* at cells (8, 19) and (24, 19), from the spare cells of row 17: their ground
 */
/* from column groundColumn, their pictures from column pictureColumn. */
#define DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(name, groundColumn, pictureColumn)   \
	void name(void) {                                                          \
		fdMapCopyCellAttributeRect(groundColumn, 17, 1, 1, 8, 19);             \
		fdMapCopyCellAttributeRect(groundColumn, 17, 1, 1, 24, 19);            \
		fdMapGridCopyCellsTo(pictureColumn, 17, 72, 19, 1, 2);                 \
		fdMapGridCopyCellTilesIndicesRect(pictureColumn, 17, 88, 19, 1, 2);    \
	}

/* Nothing visible of its own: as Reveal fades, the Reveal tablet room's two */
/* hidden doorways, at cells (8, 19) and (24, 19), are walls again. */
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(scAirsRockInsideHideRevealedDoors, 26, 90)

/* Felix casts Reveal in the Reveal tablet's room, and two hidden doorways */
/* show in its walls, at cells (8, 19) and (24, 19), for as long as it */
/* lasts. */
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(scAirsRockInsideRevealDoors, 25, 89)

#include "shared/orbit_spark_spawn.h"

/* An orbiter's update: circles 8 pixels above its anchor, closing in 4 pixels
 */
/* a step and leaving sparks, until its countdown ends; 1 once it is 2 pixels */
/* out. */
s32 scAirsRockInsideOrbitAnchor(ORBITER *orbiter) {
	ACTOR *anchor = orbiter->anchor;

	orbiter->ang += (s16)orbiter->speed >> 1;
	orbiter->x = anchor->x.fixed;
	orbiter->y = anchor->y.fixed + toFIXED(8);
	orbiter->z = anchor->z.fixed;
	syFieldVectorAddPolarOffsetFX(orbiter->orbitRadius, orbiter->ang,
								  (s32)&orbiter->x);
	if(--orbiter->countdown == 0)
		return 0;
	if(orbiter->orbitRadius == toFIXED(2))
		return 1;
	syOrbitSparkSpawn(orbiter);
	orbiter->orbitRadius -= toFIXED(4);
	orbiter->ang += (s16)orbiter->speed >> 1;
	orbiter->x = anchor->x.fixed;
	orbiter->y = anchor->y.fixed + toFIXED(8);
	orbiter->z = anchor->z.fixed;
	syVectorAddPolarOffset(orbiter->orbitRadius, orbiter->ang, &orbiter->x);
	syOrbitSparkSpawn(orbiter);
	orbiter->orbitRadius -= toFIXED(4);
	return 1;
}

#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_sparkles.h"

extern u32 Lesson_TabletRiseScript[];
extern u32 AirsRockInside_RisingSparkScript[];
extern s32 AirsRockInside_OrbitingSparkScript[];
extern u32 Lesson_AdeptRiseScript[];
extern s32 Lesson_SeekingSparkScript[];
extern s32 Lesson_TabletSettleScript[];
extern s32 Lesson_AdeptSettleScript[];

/* The sparks that rise from the tablet as it lifts. */
enum { TABLET_SPARKS = 16 };
/* The sparks that spiral in on Sheba, one set out every ORBITING_SPARK_FRAMES.
 */
enum { ORBITING_SPARKS = 16, ORBITING_SPARK_FRAMES = 8 };
/* The sparks that then close in on her start every 2 pixels up her, each */
/* circling out as far as the sine of its height in 64ths of a turn */
/* (syOrbitUpdate's phase), short of SEEKING_SPARK_PHASES, half a turn, where */
/* the sine is 0 again. */
enum { SEEKING_SPARK_PHASES = 32 };

/* The tablet rises in a rumble, sparks circle Sheba and she learns Reveal. */
void scAirsRockInsideShebaLearnsReveal(s32 tabletId, s32 shebaId) {
	ACTOR *tablet = fdActorGetById(tabletId);
	ACTOR *sheba = fdActorGetById(shebaId);
	s32 i;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	s32 unused[3];

	sySndPlay(SOUND_FADE_OUT_BGM);
	syFieldEventWait(30);
	fdCameraSetSpeedParms(toFIXED(1) / 2, toFIXED(1) / 16);
	fdCameraMoveWithinMapBounds(INTtoFIXED(tablet->x.part[0].integer),
								INTtoFIXED(tablet->y.part[0].integer),
								INTtoFIXED(tablet->z.part[0].integer), 1);
	sySndPlay(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	tablet->motionFlg = 0;
	tablet->acceleration = toFIXED(1) / 5;
	tablet->speedLimit = toFIXED(1) / 5;
	fdActorSetMovementTypeById(tabletId, Lesson_TabletRiseScript);
	sySndPlay(SE_TABLET_RISE);
	syFieldEventWait(45);
	fdActorConfigureSprCol(tabletId, SPRITE_COLOR_BLINK);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	for(i = 0; i != TABLET_SPARKS; i++) {
		ACTOR *spark = syImportActorCreate(SPRITE_PARTICLE, tablet->x.fixed,
										   tablet->y.fixed, tablet->z.fixed);
		sySndPlay(SE_GLITTER);
		spark->x.fixed += INTtoFIXED((syRandom() & 31) - 16);
		spark->y.fixed += INTtoFIXED((syRandom() & 15) - 8);
		spark->speedLimit = toFIXED(8);
		spark->acceleration = toFIXED(1) / 5;
		spark->motionFlg = 0;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		scSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		fdActorStartScript(spark, AirsRockInside_RisingSparkScript);
		REG_BLDALPHA = BLEND_ALPHA(4, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(16, 16);
		sySynch(2);
		REG_BLDALPHA = BLEND_ALPHA(10, 16);
		sySynch(2);
	}
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	sySynch(30);
	for(i = 0; i != ORBITING_SPARKS; i++) {
		ACTOR *spark = scFieldActorCreate(SPRITE_PARTICLE, sheba->x.fixed,
										  sheba->y.fixed, sheba->z.fixed);
		sySndPlay(SE_SPARKS);
		spark->tmr = (syRandom() & 0x7fff) + DEGtoANG(180);
		spark->userValue = ((i & 1) * 2 - 1) << ((syRandom() & 3) + 9);
		spark->motionFlg = 0;
		spark->sceneWord = toFIXED(122);
		spark->watchedActor = (FIELD_OBJECT *)sheba;
		/* All spirals end together, 15 frames after the loop's last wait. */
		spark->sceneCnt = (ORBITING_SPARKS - i) * ORBITING_SPARK_FRAMES + 15;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		scSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		syFieldVectorAddPolarOffsetFX(spark->sceneWord, spark->tmr,
									  (s32)&spark->x.fixed);
		fdActorStartScript(spark, AirsRockInside_OrbitingSparkScript);
		if(i == 3) {
			fdActorSetSprColById(shebaId, SPRITE_COLOR_BLINK);
			sheba->motionFlg = 0;
			sheba->acceleration = toFIXED(1) / 5;
			sheba->speedLimit = toFIXED(1) / 5;
			fdActorSetMovementTypeById(shebaId, Lesson_AdeptRiseScript);
		}
		sySynch(ORBITING_SPARK_FRAMES);
	}
	sySndPlay(SE_SHIMMER);
	sySynch(16);
	for(i = 2; i != SEEKING_SPARK_PHASES; i += 2) {
		ACTOR *spark = scFieldActorCreate(
			SPRITE_PARTICLE, sheba->x.fixed,
			sheba->y.fixed + toFIXED(i) - toFIXED(8), sheba->z.fixed);
		spark->tmr = syRandom();
		spark->userValue = TRIG_TURN / 32;
		spark->sceneCnt = i;
		spark->sceneByte = 1;
		spark->bounce = spark->x.fixed;
		spark->sceneWord = 0;
		spark->gravity = spark->z.fixed;
		spark->watchedActor = (FIELD_OBJECT *)sheba;
		spark->motionFlg = 0;
		spark->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		fdActorStartScript(spark, Lesson_SeekingSparkScript);
	}
	sySndPlay(SE_SHIMMER);
	sySynch(50);
	fdPaletteFadeSetTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(8);
	sySynch(16);
	fdMapGridSetShake(0, 0, 0);
	fdActorConfigureSprCol(tabletId, SPRITE_COLOR_NORMAL);
	fdActorConfigureSprCol(shebaId, SPRITE_COLOR_NORMAL);
	sySndPlay(SE_FLASH);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(80);
	fdActorSetMovementTypeById(tabletId, Lesson_TabletSettleScript);
	fdActorSetMovementTypeById(shebaId, Lesson_AdeptSettleScript);
	sheba->update = fdActorEmitSparkles;
	sySynch(120);
	sheba->update = 0;
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	scPlayMusic();
	sySynch(1);
}

#include "text/msg_ids.h"

/* Checking the tablet shows its message; the first time */
/* (FLAG_REVEAL_TABLET_READ) Sheba learns Reveal, as Magma Rock's tablet does */
/* for Blaze. */
void scAirsRockInsideReadRevealTablet(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "Felix checked the tablet. It has strange characters written on it." */
	/* "Wielder of Wind's might... Lay your hands upon this stone." */
	/* "We bestow upon thee the power to see the truth unclouded..." */
	mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
	if(fdEventGetFlag(FLAG_REVEAL_TABLET_READ) != 0) {
		fdEventEnd();
	}
	else {
		fdEventSetMessageId((s32)MsgCheckedTheTablet);
		FieldStateWork->msg++;
		fdActorSetSpeedFX(MainRoutineStateWork.leaderActorId, toFIXED(7) / 10,
						  toFIXED(7) / 20);
		fdActorSetSpeedFX(ACTOR_SHEBA, toFIXED(7) / 10, toFIXED(7) / 20);
		fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
		syFieldEventWait(1);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, 256, 136);
		fdActorWalkToById(ACTOR_SHEBA, 272, 136);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdActorWaitForMoveById(ACTOR_SHEBA);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_EAST, 0);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_WEST, 0);
		syFieldEventWait(10);
		/* "Do you think I ought to touch it?" */
		fdEventShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldEventWait(30);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 256, 152);
		fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
		syFieldEventWait(20);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
		syFieldEventWait(20);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
		syFieldEventWait(30);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
		syFieldEventWait(20);
		fdActorWalkToAndWait(ACTOR_SHEBA, 264, 137);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
		syFieldEventWait(20);
		/* "Sheba touched the tablet." */
		mnFieldMessageShow(MsgCheckedTheTablet + 2, FIELD_MESSAGE_ZOOM);
		fdMapGridSetShake(5, 7, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridWaitForShakeToSettle();
		scAirsRockInsideShebaLearnsReveal(ACTOR_REVEAL_TABLET, ACTOR_SHEBA);
		fdPartyLearnAbility(SHEBA, ABILITY_REVEAL);
		syGameFlagSet(FLAG_REVEAL_TABLET_READ);
		fdActorTurnToFaceEachOther(MainRoutineStateWork.leaderActorId,
								   ACTOR_SHEBA, 0);
		syFieldEventWait(10);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
		fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldEventWait(20);
		fdActorSetSpeedFX(ACTOR_SHEBA, toFIXED(6) / 5, toFIXED(3) / 5);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_WALK);
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(actor != 0)
			fdActorSetDestinationById(ACTOR_SHEBA, actor->x.part[0].integer,
									  actor->z.part[0].integer);
		fdActorWaitForMoveById(ACTOR_SHEBA);
		fdActorSetHorizontalPosWithTerrain(ACTOR_SHEBA, 0, 0);
		fdActorSetMovementType(fdActorGetById(ACTOR_REVEAL_TABLET),
							   MOVEMENT_TYPE_TAKE_LEADER_LAYER);
		fdEventEnd();
	}
}

/* Felix examines the Reveal tablet from the south: read already, */
/* it says so; otherwise he walks round to its front and Sheba reads it. */
void scAirsRockInsideWalkToRevealTablet(void) {
	if(syGameFlagGet(FLAG_REVEAL_TABLET_READ)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
		fdEventEnd();
	}
	else {
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 248, 107);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 248, 136);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 264, 136);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_NORTH, 0);
		syFieldEventWait(10);
		scAirsRockInsideReadRevealTablet();
	}
}

/* Defines a function for examining the Reveal tablet from one side: once it */
/* has been read the game only says so; before, Felix walks from sideX round */
/* to its front, faces it, and Sheba reads it. */
#define DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(name, sideX)                       \
	void name(void) {                                                          \
		if(syGameFlagGet(FLAG_REVEAL_TABLET_READ)) {                           \
			fdEventStart();                                                    \
			fdEventEndLastingPsynergy(0);                                      \
			mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);       \
			fdEventEnd();                                                      \
		}                                                                      \
		else {                                                                 \
			fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId,       \
									 sideX, 136);                              \
			fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 264,  \
									 136);                                     \
			fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,       \
									 FACING_NORTH, 0);                         \
			syFieldEventWait(10);                                              \
			scAirsRockInsideReadRevealTablet();                                \
		}                                                                      \
	}

/* Felix examines the Reveal tablet from its east side, facing west: he walks */
/* round to its front and Sheba reads it, as from the south */
/* (scAirsRockInsideWalkToRevealTablet). */
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(scAirsRockInsideReadTabletFacingWest, 280)

/* Felix examines the Reveal tablet from its west side, facing east: the same.
 */
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(scAirsRockInsideReadTabletFacingEast, 248)

/* Nothing happens as Felix walks west into the ledge of the Reveal tablet's */
/* room. */
void scAirsRockInsideIgnoreStep(void) {}

/* Felix casts Move on the pillar in the Elixir chest room; nothing more than */
/* the move shows, but the game remembers whether it now stands on the */
/* turning bridge, which then cannot turn under a gust. */
void scAirsRockInsideMoveElixirChestPillar(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(FIELD_HALF_CELL(fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
	   ELIXIR_CHEST_PILLAR_ON_BRIDGE_X)
		syGameFlagSet(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
	else
		syGameFlagClear(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
	fdEventEnd();
}

/* Felix casts Pound on one of the four stakes in the paired stakes room: it */
/* sinks into the floor and the other stake of its pair rises */
/* (scAirsRockInsidePoundStake). */
void scAirsRockInsidePoundPairedStakesStake(s32 mode, s32 step) {
	scAirsRockInsidePoundStake(mode, step);
}

extern u16 AirsRockInside_PairedStakesStakes[];
void scAirsRockInsideSetStakeCells(ACTOR *actor);

/* Stepping onto a raised stake's top: unless a pillar stands on its pair, the
 */
/* stake sinks under the leader and its partner rises (as */
/* scAirsRockInsidePoundStake draws it). */
void scAirsRockInsideStepOntoStake(void) {
	ACTOR *first = 0, *second = 0, *leader;
	FIELD_SPR *firstSprite, *secondSprite;
	VRAM_CACHE_ENTRY *entry;
	u16 *pair;
	u32 id, i;
	s32 busy;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	pair = AirsRockInside_PairedStakesStakes;
	if(*pair != ACTOR_LIST_END) {
		do {
			id = *pair;
			first = fdActorGetById(id);
			if(FIELD_CELL(first->x.fixed) == FIELD_CELL(leader->x.fixed) &&
			   FIELD_CELL(first->z.fixed) == FIELD_CELL(leader->z.fixed)) {
				second = fdActorGetById(id - ((id & 1) << 1) + 1);
				break;
			}
			pair += 3;
		} while(*pair != ACTOR_LIST_END);
	}
	busy = first->sceneByte | second->sceneByte;
	if(busy != 0)
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	firstSprite = first->spr;
	secondSprite = second->spr;
	second->motionFlg = busy;
	second->y.fixed -= toFIXED(28);
	sySynch(1);
	entry = &VramCacheEntries[secondSprite->slot];
	second->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
	entry = &VramCacheEntries[firstSprite->slot];
	first->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
	btDmaFill32(busy, second->watchedActor, entry->size);
	syDMAXCopy(first->watchedActor, second->watchedActor,
			   DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4), REG_DMA3);
	first->watchedActor =
		(void *)((u8 *)first->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	second->watchedActor =
		(void *)((u8 *)second->watchedActor + STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	leader->motionFlg = busy;
	for(i = 0; i != 8; i++) {
		if(i <= 4) {
			second->y.fixed += toFIXED(26) / 5;
			syDMAXCopy(first->watchedActor, second->watchedActor,
					   DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4),
					   REG_DMA3);
			first->watchedActor =
				(void *)((u8 *)first->watchedActor + STAKE_ROW_BYTES);
			second->watchedActor =
				(void *)((u8 *)second->watchedActor + STAKE_ROW_BYTES);
		}
		leader->y.fixed += -(toFIXED(7) / 2);
		leader->groundHeight = leader->y.fixed;
		first->scaleY += -(FIXED_ONE * 31 / 256);
		sySynch(1);
	}
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	first->scaleY = FIXED_ONE;
	second->y.fixed = second->groundHeight;
	syGameFlagSet(first->tmr);
	syGameFlagClear(second->tmr);
	fdActorSelectAnimation(first, ((FIELD_SPR *)first->spr)->lastNo + 1);
	fdActorSelectAnimation(second, ((FIELD_SPR *)second->spr)->lastNo - 1);
	scAirsRockInsideSetStakeCells(first);
	scAirsRockInsideSetStakeCells(second);
	syFieldEventWait(10);
	fdEventEnd();
}

/* A vent's puff has reached the leader in the face vents room: standing in */
/* front of a vent, he is blown down to the place below it with a chime. */
void scAirsRockInsideRideFaceVent(void) {
	s16 *vent;
	s32 x, z;

	x = 0;
	z = 0;
	vent = &FieldStateWork->stepTrigger;
	if(*vent == 0)
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetSpeedParms(MainRoutineStateWork.leaderActorId, toFIXED(4),
						 toFIXED(2));
	switch(*vent) {
	case CELL_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT:
		/* DEAD: the leader looked up twice, both results unused; the ROM keeps
		 */
		/* both calls. */
		fdActorGetById(MainRoutineStateWork.leaderActorId);
		fdActorGetById(MainRoutineStateWork.leaderActorId);
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
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, x, z);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdEventEnd();
}

/* Checking the Psynergy Stone restores the party's PP. */
void scAirsRockInsideCheckPsynergyStone(void) {
	const u8 *message;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	message = MsgCheckedThePsynergy;
	/* "Felix checked the Psynergy Stone." */
	mnFieldMessageShow(message, FIELD_MESSAGE_ZOOM);
	sySndPlay(SE_RECOVERY);
	fdPartyAddPp(PARTY_PP_REPLENISH, 0);
	syFieldEventWait(10);
	syGameFlagClear(FLAG_NOTHING_HAPPENS);
	/* "Everyone's PP was replenished!" */
	mnFieldMessageShow(message + 1, FIELD_MESSAGE_ZOOM);
	fdEventEnd();
}

/* The trigger table for the current room. */
const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInside_HallTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
		return AirsRockInside_VialChestTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
		return AirsRockInside_ClarityCircletChestTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
		return AirsRockInside_TightropesTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return AirsRockInside_RevealTabletTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
		return AirsRockInside_ElixirChestTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
		return AirsRockInside_PairedStakesTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
		return AirsRockInside_FaceVentsTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
		return AirsRockInside_CoinsChestTriggers;
	if(scene == SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE)
		return AirsRockInside_PsynergyStoneTriggers;
	return AirsRockInside_VialChestTriggers;
}

/* The leader rises 80 pixels on a lift into the room. */
void scAirsRockInsideRiseOnLift(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *prepared, *lift;
#if TLA_PATCH_1
	u8 priorityFlags;
#endif
	s32 height;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
#if TLA_PATCH_1
	priorityFlags = leader->priFlg;
#endif
	prepared = fdActorCreateSilhouette(toFIXED(216), -toFIXED(32), toFIXED(544),
									   SPRITE_CRATE);
	leader->motionFlg = 0;
	fdActorSetSprFlags(leader, 0);
	leader->speedLimit = toFIXED(3) / 2;
	leader->acceleration = toFIXED(3) / 2;
	lift = syImportActorCreate(SPRITE_WHIRLWIND, leader->x.fixed,
							   leader->y.fixed - toFIXED(8), leader->z.fixed);
	fdActorSelectAnimation(lift, 2);
	fdActorSetSprFlags(lift, 0);
	lift->motionFlg = 0;
	lift->speedLimit = toFIXED(7) / 5;
	lift->acceleration = toFIXED(7) / 5;
	fdActorSetMoveTarget(leader, leader->x.fixed, leader->y.fixed + toFIXED(80),
						 leader->z.fixed);
	fdActorSetMoveTarget(lift, lift->x.fixed, lift->y.fixed + toFIXED(72),
						 lift->z.fixed);
	sySndPlay(SE_KIRIN);
	fdEventStartScrTransition();
	while(!fdActorHasArrived(leader)) {
		leader->facing = (u8)FrameCount * FACING_STEP;
		leader->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (FrameCount & 1);
		sySynch(1);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	syFieldEventWait(10);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH,
							 0);
	height =
		fdMapGetTerrainHeight(0, lift->x.fixed, lift->z.fixed - toFIXED(16));
	leader->y.fixed = height;
	leader->groundHeight = height;
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	sySynch(6);
	sySndPlay(SE_JUMP);
	fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
	leader->speedLimit = toFIXED(2);
	leader->acceleration = toFIXED(1) / 2;
	leader->velY = toFIXED(4);
	leader->motionFlg = ACTOR_FALLS;
	leader->priFlg = ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
						 leader->z.fixed - toFIXED(16));
	fdActorWaitUntilLanded(leader);
	fdActorWaitForMove(leader);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorSelectAnimation(lift, 5);
	sySynch(12);
	fdActorRelease(lift);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->groundHeight = leader->y.fixed;
	fdActorSetSprFlags(leader, 1);
#if TLA_PATCH_1
	leader->priFlg = priorityFlags;
#endif
	fdActorRelease(prepared);
	sySynch(1);
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* Eight motes of light drift about the hall, each starting a little after the
 */
/* last; the odd ones swell slowly, the even ones quickly. */
void scAirsRockInsideSpawnMotes(void) {
	ACTOR *actor;
	s32 i;
	FIELD_SPR *sprite;

	for(i = 0; i != 8; i++) {
		actor = syImportActorCreate(SPRITE_PARTICLE, 0, 0, 0);
		sprite = actor->spr;
		sprite->pri = 1;
		actor->sceneByte = i << 4;
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->acceleration = actor->speedLimit =
			i * (toFIXED(1) / 16) + (toFIXED(1) / 2);
		actor->update = scAirsRockInsideDriftMote;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_RBG);
		if(i & 1) {
			actor->tmr = PARTICLE_SLOW_LIFETIME;
			actor->userValue = 0;
		}
		else {
			actor->tmr = PARTICLE_QUICK_LIFETIME;
			actor->userValue = 0;
		}
		fdActorSetBlendMode(actor, 1);
	}
}

extern s32 AirsRockInside_LeaderLevel;

/* A task in the great hall: as the leader climbs, two blocks of cells at the */
/* hall's west and east sides take the collision codes kept for the level he */
/* is at, the three levels 96 pixels apart. */
void scAirsRockInsideSetLedgeCellsByLevel(void) {
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 level;

	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(actor->y.fixed + toFIXED(32) >= toFIXED(312))
		level = 2;
	else if(actor->y.fixed + toFIXED(32) >= toFIXED(216))
		level = 1;
	else
		level = 0;
	if(AirsRockInside_LeaderLevel == level)
		return;
	switch(level) {
	case 2: {
		s32 x = 7, z = 39;
		fdMapCopyCellAttributeRect(64, 64, 6, 4, x, z);
	}
		fdMapCopyCellAttributeRect(70, 64, 6, 4, 26, 39);
		break;
	case 1: {
		s32 x = 7, z = 39;
		fdMapCopyCellAttributeRect(64, 68, 6, 4, x, z);
	}
		fdMapCopyCellAttributeRect(70, 68, 6, 4, 26, 39);
		break;
	case 0: {
		s32 x = 7, z = 39;
		fdMapCopyCellAttributeRect(64, 72, 6, 4, x, z);
	}
		fdMapCopyCellAttributeRect(70, 68, 6, 4, 26, 39);
		break;
	}
	AirsRockInside_LeaderLevel = level;
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

/* Dropping from the Vial chest room: the leader falls 130 pixels into the hall
 */
/* and lands in a ring of dust. */
void scAirsRockInsideLandInHall(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	u32 i;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdCameraMoveToFX(-1, -1, -1, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	sySndPlay(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldEventWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = FIXED_ONE * 4 / 5;
	options.startScaleY = FIXED_ONE * 4 / 5;
	/* A puff each sixteenth of a turn, the last over the first. */
	for(i = 0; i <= TRIG_TURN / FACING_STEP; i++) {
		s32 angle;
		FIXED velocity[3];

		angle = i * FACING_STEP;
		velocity[0] = syCos(angle);
		velocity[1] = 0;
		velocity[2] = sySin(angle);
		velocity[0] += velocity[0] >> 1;
		velocity[0] += syRandomBelow(toFIXED(3) / 4);
		velocity[0] += -(toFIXED(3) / 8);
		velocity[2] += syRandomBelow(toFIXED(3) / 8) + -(toFIXED(3) / 16);
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
						   velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
							   EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
						   &options);
	}
	sySndPlay(SE_THUMP);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = FIXED_ONE / 4;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

void scAirsRockInsidePuffVents(void);

s32 scInit(void) {
	ACTOR *actor;
	s32 i, z;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	syGameFlagSet(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
	syGameFlagSet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
		scAirsRockInsideStartRestoringCells();
		/* The Japanese edition settles the leader here whatever */
		/* FLAG_KEEP_PARTY_POSITION says. */
		if(MainRoutineStateWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET &&
		   (!TLA_PATCH_1 || !syGameFlagGet(FLAG_KEEP_PARTY_POSITION))) {
			fdActorGetById(MainRoutineStateWork.leaderActorId)->layer = 1;
			sySynch(8);
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
		}
		if(MainRoutineStateWork.entrance <= HALL_LAST_GREAT_HALL_ENTRANCE ||
		   MainRoutineStateWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET) {
			sySetEvent(scAirsRockInsideSetLedgeCellsByLevel, TASK_PASS_FRAME);
			REG_BLDALPHA = BLEND_ALPHA(8, 16);
			REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			scAirsRockInsideSpawnMotes();
		}
		if(MainRoutineStateWork.entrance > HALL_LAST_NORTHEAST_ROOM_ENTRANCE) {
			if(syGameFlagGet(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED))
				fdActorSetPosFX(ACTOR_HALL_MOVABLE_STATUE, toFIXED(824),
								toFIXED(328));
			fdActorPlaceBlocks(AirsRockInside_HallPillars);
		}
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->motionFlg = 0;
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->y.fixed = 0;
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->priFlg =
			ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprPriority(ACTOR_HALL_TWISTER_STATUE, 3);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN)) {
			fdActorSetSprCol(fdActorGetById(ACTOR_HALL_TWISTER_STATUE),
							 SPRITE_COLOR_GREY);
		}
		else {
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
			fdActorSetSprCol(fdActorGetById(ACTOR_HALL_TWISTER_ROCK),
							 SPRITE_COLOR_GREY);
			fdMapGridCopyCellsTo(64, 78, 18, 105, 3, 5);
		}
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
			fdActorSetSprCol(fdActorGetById(ACTOR_HALL_WEST_STATUE),
							 SPRITE_COLOR_GREY);
			fdActorSetSprCol(fdActorGetById(ACTOR_HALL_EAST_STATUE),
							 SPRITE_COLOR_GREY);
			fdActorSetSprCol(fdActorGetById(ACTOR_HALL_MOVABLE_STATUE),
							 SPRITE_COLOR_GREY);
		}
		else {
			scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_HALL_WEST_STATUE),
									   180, 0);
			scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_HALL_EAST_STATUE),
									   60, 0);
			scAirsRockInsideWakeMovableStatue(
				fdActorGetById(ACTOR_HALL_MOVABLE_STATUE), 60, 0);
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_WEST_STATUE), 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_EAST_STATUE), 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_MOVABLE_STATUE), 0);
		if(MainRoutineStateWork.entrance == ENTRANCE_HALL_LANDING &&
		   !syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
			scAirsRockInsideLandInHall();
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		REG_BLDALPHA = BLEND_ALPHA(8, 16);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		scAirsRockInsideStartRestoringCells();
		if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_VIAL_CHEST_EAST_PILLAR, toFIXED(328),
							toFIXED(392));
		if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_VIAL_CHEST_WEST_PILLAR, toFIXED(104),
							toFIXED(392));
		fdActorPlaceBlocks(AirsRockInside_VialChestPillars);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
			fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_STATUE),
							 SPRITE_COLOR_GREY);
		}
		else {
			scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_VIAL_CHEST_STATUE),
									   60, 0);
			fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK),
							 SPRITE_COLOR_GREY);
			fdMapGridCopyCellsTo(68, 84, 12, 77, 3, 6);
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_VIAL_CHEST_STATUE), 0);
		if(MainRoutineStateWork.entrance == ENTRANCE_VIAL_CHEST_ON_THE_LIFT &&
		   !syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
			scAirsRockInsideRiseOnLift();
	}
	else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		scAirsRockInsideStartRestoringCells();
		scQueueMusic(SE_AMBIENT_STREAM);
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		sySetEvent(scAirsRockInsidePuffVents, TASK_PASS_FRAME);
		if(syGameFlagGet(
			   FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
							toFIXED(264), toFIXED(632));
		if(syGameFlagGet(
			   FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
							toFIXED(616), toFIXED(568));
		if(syGameFlagGet(
			   FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
							toFIXED(472), toFIXED(520));
		fdActorPlaceBlocks(AirsRockInside_ClarityCircletChestPillars);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
			fdActorSetSprCol(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
				SPRITE_COLOR_GREY);
			fdActorSetSprCol(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
				SPRITE_COLOR_GREY);
			fdActorSetSprCol(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
				SPRITE_COLOR_GREY);
		}
		else {
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A), 60, 0);
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B), 180, 0);
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C), 200, 0);
		}
		fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A)->steeringFlg = 0;
		fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B)->steeringFlg = 0;
		fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C)->steeringFlg = 0;
		fdActorSetSprFlags(fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A),
						   0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B),
						   0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C),
						   0);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		fdTightropeInit();
		fdTightropePlace(0, ACTOR_TIGHTROPES_POST_A, ACTOR_TIGHTROPES_POST_B);
		fdTightropePlace(1, ACTOR_TIGHTROPES_POST_C, ACTOR_TIGHTROPES_POST_D);
		fdTightropePlace(2, ACTOR_TIGHTROPES_POST_E, ACTOR_TIGHTROPES_POST_F);
		fdTightropePlace(3, ACTOR_TIGHTROPES_POST_G, ACTOR_TIGHTROPES_POST_H);
		fdTightropePlace(4, ACTOR_TIGHTROPES_POST_I, ACTOR_TIGHTROPES_POST_J);
		fdActorGetById(ACTOR_TIGHTROPES_POST_H)->sceneCnt = 1;
		if(syGameFlagGet(FLAG_TIGHTROPES_BRIDGE_TURNED)) {
			fdMapGridCopyCellsTo(69, 64, 29, 85, 5, 7);
			fdMapGridCopyCellAttributes(46, 20, 5, 5, 29, 21);
			fdMapGridCopyCellAttributes(32, 89, 1, 1, 31, 89);
			fdMapGridCopyCellAttributes(50, 86, 1, 1, 33, 87);
		}
		else {
			fdMapGridCopyCellsTo(64, 64, 29, 85, 5, 7);
			fdMapGridCopyCellAttributes(52, 20, 5, 5, 29, 21);
			fdMapGridCopyCellAttributes(33, 88, 1, 1, 33, 87);
			fdMapGridCopyCellAttributes(48, 88, 1, 1, 31, 89);
		}
		if(syGameFlagGet(FLAG_TIGHTROPES_GUST_PATH_CLEARED)) {
			fdMapGridCopyCellsTo(90, 25, 86, 25, 4, 3);
			fdMapGridCopyCellsTo(21, 88, 24, 89, 1, 2);
			fdMapGridCopyCellAttributes(23, 92, 2, 1, 23, 89);
			fdMapGridCopyCellAttributes(18, 33, 2, 1, 23, 25);
		}
		if(syGameFlagGet(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_TIGHTROPES_PILLAR, toFIXED(648),
							toFIXED(296));
		fdActorPlaceBlocks(AirsRockInside_TightropesPillars);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET) {
		syGameFlagSet(FLAG_RETREAT_BLOCKED);
		scAirsRockInsideStartRestoringCells();
		for(i = 0; i != 6; i++) {
			if(syGameFlagGet(FLAG_FIRST_STONE_VANISHED + i)) {
				z = 24 - (i << 1);
				fdMapGridCopyCellsTo(65, 1, 80, z, 1, 2);
				syFieldMapCellSetAttributes(
					MAPGRID_LAYER_GROUND, 16, z,
					CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
			}
		}
		fdActorGetById(ACTOR_REVEAL_TABLET)->motionFlg = 0;
		actor = fdActorGetById(ACTOR_REVEAL_TABLET);
		fdActorGetById(ACTOR_REVEAL_TABLET)->groundHeight = toFIXED(16);
		actor->y.fixed = toFIXED(16);
		fdActorSetMovementType(fdActorGetById(ACTOR_REVEAL_TABLET),
							   MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		scAirsRockInsideStartRestoringCells();
		fdTightropeInit();
		fdTightropePlace(0, ACTOR_ELIXIR_CHEST_POST_A,
						 ACTOR_ELIXIR_CHEST_POST_B);
		fdTightropePlace(1, ACTOR_ELIXIR_CHEST_POST_C,
						 ACTOR_ELIXIR_CHEST_POST_D);
		fdTightropePlace(2, ACTOR_ELIXIR_CHEST_POST_E,
						 ACTOR_ELIXIR_CHEST_POST_F);
		if(!syGameFlagGet(FLAG_ELIXIR_CHEST_BRIDGE_TURNED)) {
			fdMapGridCopyCellsTo(69, 64, 32, 79, 5, 7);
			fdMapGridCopyCellAttributes(49, 15, 5, 5, 32, 15);
			fdMapGridCopyCellAttributes(33, 82, 1, 1, 34, 82);
			fdMapGridCopyCellAttributes(52, 81, 1, 1, 36, 81);
		}
		else {
			fdMapGridCopyCellsTo(64, 64, 32, 79, 5, 7);
			fdMapGridCopyCellAttributes(49, 21, 5, 5, 32, 15);
			fdMapGridCopyCellAttributes(35, 81, 1, 1, 36, 81);
			fdMapGridCopyCellAttributes(50, 82, 1, 1, 34, 82);
		}
		if(syGameFlagGet(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE))
			fdActorSetPosFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(520),
							toFIXED(296));
		if(syGameFlagGet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED)) {
			fdActorSetPosFX(ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(312),
							toFIXED(296));
			fdActorSetSprPriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
			fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->motionFlg = ACTOR_FALLS;
			fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->groundHeight =
				-toFIXED(32);
			fdMapGridCopyCellAttributes(19, 16, 1, 1, 19, 18);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		scAirsRockInsideStartRestoringCells();
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_A)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_D)->sceneCnt = 1;
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			syGameFlagSet(FLAG_STAKE_B_DOWN);
			syGameFlagSet(FLAG_STAKE_C_DOWN);
		}
		scAirsRockInsidePlaceStakes(AirsRockInside_PairedStakesStakes);
		if(syGameFlagGet(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_PAIRED_STAKES_PILLAR, toFIXED(344),
							toFIXED(696));
		fdActorPlaceBlocks(AirsRockInside_PairedStakesPillars);
		if(syGameFlagGet(FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyCellsTo(39, 82, 38, 82, 1, 3);
			fdMapGridCopyCellAttributes(39, 82, 1, 1, 38, 82);
			fdMapGridCopyCellAttributes(38, 19, 1, 1, 38, 18);
		}
		if(syGameFlagGet(FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyCellsTo(39, 82, 38, 99, 1, 3);
			fdMapGridCopyCellAttributes(38, 98, 1, 1, 38, 99);
			fdMapGridCopyCellAttributes(38, 35, 1, 1, 38, 37);
		}
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		scQueueMusic(SE_AMBIENT_STREAM);
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		sySetEvent(scAirsRockInsidePuffVents, TASK_PASS_FRAME);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_FACE_VENTS_FIRST_PILLAR, toFIXED(280),
							toFIXED(472));
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_FACE_VENTS_SECOND_PILLAR, toFIXED(360),
							toFIXED(840));
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED)) {
			fdActorSetPosFX(ACTOR_FACE_VENTS_THIRD_PILLAR, toFIXED(184),
							toFIXED(616));
			syGameFlagSet(FLAG_SOUTHEAST_VENT_STILL);
		}
		fdActorSetSprPriority(ACTOR_FACE_VENTS_SECOND_PILLAR, 2);
		fdActorPlaceBlocks(AirsRockInside_FaceVentsPillars);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed),
			0);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed),
			0);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
			fdActorSetSprCol(fdActorGetById(ACTOR_FACE_VENTS_STATUE),
							 SPRITE_COLOR_GREY);
		else
			scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_FACE_VENTS_STATUE),
									   8, 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_FACE_VENTS_STATUE), 0);
	}
	else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		scAirsRockInsideStartRestoringCells();
		if(syGameFlagGet(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED))
			fdActorSetPosFX(ACTOR_COINS_CHEST_PILLAR, toFIXED(360),
							toFIXED(264));
		fdActorPlaceBlocks(AirsRockInside_CoinsChestPillars);
		scMapCellSetAttributes(
			MAPGRID_LAYER_OVERHEAD,
			FIELD_CELL(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
			FIELD_CELL(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->z.fixed), 0);
		fdActorGetById(ACTOR_COINS_CHEST_STATUE)->steeringFlg = 0;
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
			fdActorSetSprCol(fdActorGetById(ACTOR_COINS_CHEST_STATUE),
							 SPRITE_COLOR_GREY);
		else
			scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_COINS_CHEST_STATUE),
									   60, 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_COINS_CHEST_STATUE), 0);
		if(syGameFlagGet(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyCellsTo(15, 80, 15, 77, 2, 3);
			fdMapGridCopyCellAttributes(15, 80, 1, 1, 15, 79);
			fdMapGridCopyCellAttributes(17, 15, 2, 1, 15, 15);
		}
	}
	return 0;
}

s32 scPrepareMap(void) {
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		scAirsRockInsideSetLedgeCellsByLevel();
	return 0;
}

void scAirsRockInsideStartWhirlwind(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_LongScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The blocks Move pushes in each room (fdActorPlaceBlocks), ended by -1: the */
/* hall's statue. */
s16 AirsRockInside_HallPillars[] = {
    ACTOR_HALL_MOVABLE_STATUE, -1,
};

/* The Vial chest room's two pillars. */
s16 AirsRockInside_VialChestPillars[] = {
    ACTOR_VIAL_CHEST_EAST_PILLAR, ACTOR_VIAL_CHEST_WEST_PILLAR, -1,
};

/* The Clarity Circlet chest room's three pillars and its statue. */
s16 AirsRockInside_ClarityCircletChestPillars[] = {
    ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
    ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
    -1,
};

/* The tightropes room's pillar. */
s16 AirsRockInside_TightropesPillars[] = {
    ACTOR_TIGHTROPES_PILLAR, -1,
};

/* The paired stakes room's four pillars. */
s16 AirsRockInside_PairedStakesPillars[] = {
    ACTOR_PAIRED_STAKES_PILLAR,   ACTOR_PAIRED_STAKES_PILLAR_A,
    ACTOR_PAIRED_STAKES_PILLAR_B, ACTOR_PAIRED_STAKES_PILLAR_C,
    -1,
};

/* The face vents room's five pillars. */
s16 AirsRockInside_FaceVentsPillars[] = {
    ACTOR_FACE_VENTS_THIRD_PILLAR, ACTOR_FACE_VENTS_FIRST_PILLAR,
    ACTOR_FACE_VENTS_SECOND_PILLAR, ACTOR_FACE_VENTS_PILLAR_A,
    ACTOR_FACE_VENTS_PILLAR_B, -1,
};

/* The coins chest room's pillar and statue. */
s16 AirsRockInside_CoinsChestPillars[] = {
    ACTOR_COINS_CHEST_PILLAR, ACTOR_COINS_CHEST_STATUE, -1,
};

u32 OrbitSpark_Script[] = {
    SC_WAIT(6),
    SC_RELEASE,
    SC_END,
};

u32 AirsRockInside_RisingSparkScript[] = {
    SC_MOVE_BY(0, 160, 0),
    SC_SYNC,
    SC_RELEASE,
    SC_END,
};

s32 AirsRockInside_OrbitingSparkScript[] = {
    SC_CALL(scAirsRockInsideOrbitAnchor),
    SC_RELEASE,
    SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

const s32 *FieldEffect_DurationScripts[] = {
    FieldEffect_ShortScript,
    FieldEffect_MediumScript,
    FieldEffect_LongScript,
};

u32 AirsRockInside_WhirlwindRisesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 AirsRockInside_WhirlwindFadesScript[] = {
    SC_WAIT(60),
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
    SC_END,
};

u32 AirsRockInside_PooledWhirlwindFadesScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

u32 AirsRockInside_WindStoneScript[] = {
    SC_SET(SW_SCALE_Y, FIXED_ONE * 3 / 2),
    SC_SET(SW_STEERING, 0),
    SC_ANIMATION(2),
    SC_END,
};

/* The paired stakes room's four stakes, as scAirsRockInsidePlaceStakes reads */
/* them: each stake, its first pose and the flag set while it is sunk. */
u16 AirsRockInside_PairedStakesStakes[] = {
    ACTOR_PAIRED_STAKES_STAKE_A, 1, FLAG_STAKE_A_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_B, 1, FLAG_STAKE_B_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_C, 1, FLAG_STAKE_C_DOWN,
    ACTOR_PAIRED_STAKES_STAKE_D, 1, FLAG_STAKE_D_DOWN,
    ACTOR_LIST_END, 0,
};

SCENE_ENTRANCE AirsRockInside_Entrances[] = {
    {0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AirsRockInside_HallEntrances[] = {
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

u32 SceneExits[] = {
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

ACTOR_PLACE AirsRockInside_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_HallPlacements[] = {
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

ACTOR_PLACE AirsRockInside_VialChestPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_ClarityCircletChestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_TightropesPlacements[] = {
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
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

ACTOR_PLACE AirsRockInside_RevealTabletPlacements[] = {
    {SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_ElixirChestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_PairedStakesPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_FaceVentsPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInside_CoinsChestPlacements[] = {
    {SPRITE_WIND_STONE, CONDITION_ALWAYS, (s32)AirsRockInside_WindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

s32 AirsRockInside_StampedCell = -1;

TRIGGER AirsRockInside_HallTriggers[] = {
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, EVENT(scAirsRockInsideOpenTabletRoomDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampTallCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampTallCellUnderLeaderNearer)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, EVENT(scAirsRockInsideWakeHallStatue)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideWhirlwind)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE, CONDITION_ALWAYS, EVENT(scAirsRockInsideJumpDownLedge)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampOtherCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, EVENT(scAirsRockInsideWakeVialChestStatue)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideWhirlwind)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_ClarityCircletChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideWhirlwind)},
    {TRIGGER_RAISED, TRIGGER_PUFF_REACHES_LEADER, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideClarityCircletVent)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_TightropesTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_RevealTabletTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampOtherCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampWideCellUnderLeader)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(scAirsRockInsideIgnoreStep)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(scAirsRockInsideReadRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(scAirsRockInsideWalkToRevealTablet)},
    {EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(scAirsRockInsideReadTabletFacingWest)},
    {EXAMINE_TRIGGER_FACING(FACING_EAST), CELL_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, EVENT(scAirsRockInsideReadTabletFacingEast)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideRevealDoors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideHideRevealedDoors)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 1, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 2, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 3, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOffVanishingStone)},
    {TRIGGER_RAISED, TRIGGER_FIRST_VANISHING_STONE + 4, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOffVanishingStone)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_ElixirChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampOtherCellUnderLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELIXIR_CHEST_PILLAR, FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED, EVENT(scAirsRockInsideMoveElixirChestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_PairedStakesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP, CONDITION_ALWAYS, EVENT(scAirsRockInsideStepOntoStake)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_A, FLAG_STAKE_A_DOWN, EVENT(scAirsRockInsidePoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_B, FLAG_STAKE_B_DOWN, EVENT(scAirsRockInsidePoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_C, FLAG_STAKE_C_DOWN, EVENT(scAirsRockInsidePoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_D, FLAG_STAKE_D_DOWN, EVENT(scAirsRockInsidePoundPairedStakesStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_FaceVentsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_RAISED, TRIGGER_PUFF_REACHES_LEADER, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideFaceVent)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_CoinsChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStampCellUnderLeader)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(scAirsRockInsideRideWhirlwind)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(scAirsRockInsideStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, EVENT(scAirsRockInsideFinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideStartWhirlwind)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideTakeOverWhirlwind)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideBlowGust)},
    {TRIGGER_TABLE_END},
};

TRIGGER AirsRockInside_PsynergyStoneTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE, CONDITION_ALWAYS, EVENT(scAirsRockInsideCheckPsynergyStone)},
    {TRIGGER_TABLE_END},
};

/* The great hall's level the leader stood at when its ledge cells were last */
/* set; -1 before. */
s32 AirsRockInside_LeaderLevel = -1;
#undef fdActorSetHorizontalPosWithTerrain
