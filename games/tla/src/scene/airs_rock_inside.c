/* Inside Air's Rock, reached through the doorway the lightning opens at its
   top, with a golden door low on its cliff also leading into the great hall
   (airs_rock.c): the hall and the rooms around it, their doorways marked by
   coloured dots.

   Felix moves pillars with Move or by pushing them, so that they fill gaps or
   block the vents whose puffs of steam blow him off his path; Pound sinks one
   of a pair of stakes and raises the other; Whirlwind on a Wind Stone sends a
   gust that carries pillars, blows golden barriers away and turns bridges, and
   on a twister rock wakes the statues, whose little whirlwinds carry the party.
   The party crosses tightropes and stepping stones that vanish behind them. At
   the heart of the rock, in the room of the Reveal tablet, Sheba reads the
   tablet and learns Reveal (scAirsRockInsideReadRevealTablet), and a Psynergy
   Stone in another room restores the party's PP
   (scAirsRockInsideCheckPsynergyStone). */

#include "scene/script_layer.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "macros.h"
#include "field/actor_script.h"
#include "scene/airs_rock.h"
#include "scene/gabomba_statue.h"
#include "graphics/palette_buffers.h"
#include "window/window.h"

/* Set once Move has left the hall's movable statue on half metatile (103,
   41). */
#define FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED				  0xa8e
/* Set once Move has left the Vial chest room's western pillar on (13, 49). */
#define FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED			  0x898
/* Set once Move has left its eastern pillar on (41, 49). */
#define FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED			  0x897
/* Set once Move has left the Clarity Circlet chest room's first pillar on (33,
   79). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED  0x89b
/* Its second, on (77, 71). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED 0x89c
/* Its third, on (59, 65). */
#define FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED  0x89d
/* Set once Move has left the tightropes room's pillar on (81, 37). */
#define FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED					  0x89e
/* Set once a gust has dropped the Elixir chest room's pillar below its bridge
   (scAirsRockInsideBlowGust). The set-up sets it on every entry, so the room
   always shows the pillar dropped and its Move cue never runs. */
#define FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED				  0x89f
/* Set once Move has left the paired stakes room's pillar on (43, 87). */
#define FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED				  0x8d0
/* Set once Move has left the face vents room's first pillar on (35, 59). */
#define FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED			  0x8d1
/* Its second, on (45, 105). */
#define FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED			  0x8d2
/* Its third, on (23, 77). */
#define FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED			  0xa8f
/* Set once Move has left the coins chest room's pillar on (45, 33). */
#define FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED				  0x8d3

/* Story flags: the statues of the hall and the Vial chest room woken by
   Whirlwind, and the Reveal tablet read. */
#define FLAG_AIRS_ROCK_HALL_STATUE_WOKEN	   0x89a
#define FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN 0x899
#define FLAG_REVEAL_TABLET_READ				   0x8d4

/* The paired stakes room's stakes, each set while it is down; B and C go down
   as the party comes in. */
#define FLAG_STAKE_A_DOWN FLAG_SCENE_TEMP_00
#define FLAG_STAKE_B_DOWN FLAG_SCENE_TEMP_01
#define FLAG_STAKE_C_DOWN FLAG_SCENE_TEMP_02
#define FLAG_STAKE_D_DOWN FLAG_SCENE_TEMP_03

/* The face vents room's vents kept still by where Move leaves a pillar
   (scAirsRockInsideFinishPushingPillar): the vent beside each puffs in its turn
   (scAirsRockInsidePuffVents). */
#define FLAG_SOUTHEAST_VENT_STILL	FLAG_SCENE_TEMP_00
#define FLAG_MIDDLE_WEST_VENT_STILL FLAG_SCENE_TEMP_01
#define FLAG_NORTHEAST_VENT_STILL	FLAG_SCENE_TEMP_02

/* The Reveal tablet room: set once a stepping stone has vanished, and one flag
   for each stone from the southernmost, set once it has. */
#define FLAG_STONE_VANISHED		  FLAG_SCENE_TEMP_00
#define FLAG_FIRST_STONE_VANISHED FLAG_SCENE_TEMP_02

/* What the gusts leave for the rest of the visit: the metatiles a gust clears
   on its path in the tightropes room (the set-up sets it on every entry, so the
   room always shows them cleared), a turning bridge turned from its first lie,
   the Elixir chest room's pillar left on its bridge, and a golden barrier blown
   away in the paired stakes and coins chest rooms. */
#define FLAG_TIGHTROPES_GUST_PATH_CLEARED			FLAG_AREA_TEMP_00
#define FLAG_TIGHTROPES_BRIDGE_TURNED				FLAG_AREA_TEMP_01
#define FLAG_ELIXIR_CHEST_BRIDGE_TURNED				FLAG_AREA_TEMP_02
#define FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE			FLAG_AREA_TEMP_03
#define FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_04
#define FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY FLAG_AREA_TEMP_05
#define FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY			FLAG_AREA_TEMP_06

/* The Elixir chest room's pillar column, in half metatiles, that stands it on
   the turning bridge. */
#define ELIXIR_CHEST_PILLAR_ON_BRIDGE_X 65

/* The hall's entrance from the Reveal tablet's room, and where the party lands
   after dropping from the Vial chest room (scAirsRockInsideLandInHall); the
   Vial chest room's entrance on the lift from the hall
   (scAirsRockInsideRiseOnLift). */
enum {
	ENTRANCE_HALL_FROM_REVEAL_TABLET = 16,
	ENTRANCE_HALL_LANDING = 11,
	ENTRANCE_VIAL_CHEST_ON_THE_LIFT = 3
};

/* The hall's entrances by the part of it they open on: up to the first of these
   the great hall itself, then its northeast room, and past the second its east
   room. ENTRANCE_HALL_FROM_REVEAL_TABLET, past both, takes the great hall's
   set-up and the east room's. */
enum {
	HALL_LAST_GREAT_HALL_ENTRANCE = 11,
	HALL_LAST_NORTHEAST_ROOM_ENTRANCE = 13
};

/* The collision codes this scene reads by name. On the overhead layer a
   whirlwind ends where it reaches WHIRLWIND_END; on the ground a raised stake's
   top is STAKE_TOP; in the hall the tower's door is TOWER_DOOR, and the Vial
   chest room is left by jumping off VIAL_CHEST_LEDGE. The Reveal tablet room's
   stepping stones run north from STONES_SOUTH_END to STONES_NORTH_END on the
   overhead layer, the five between them vanishing, and its tablet, as the
   Psynergy Stone room's stone, is examined on its metatile. */
typedef enum AIRS_ROCK_INSIDE_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END = 1,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE = 2,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END = 7,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE = 20,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR = 30,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET = 30,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE = 30,
	/* The stepping stones Felix is drawn standing on: two kinds, a stack of two
	   and the same one row nearer, and the Reveal tablet room's stones that
	   light gold under him (scAirsRockInsideStampMetatileUnderLeader and the
	   rest). */
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE = 80,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE = 90,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END = 212,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP = 212
} AIRS_ROCK_INSIDE_METATILE_BEHAVIOR;

/* The face vents room's six vents, by the metatile in front of each on the
   ground layer: south of the room the west and east one, in its middle the west
   and east one, in its north the west and east one. A vent's puff reaching the
   leader there blows him down to the place below it
   (scAirsRockInsideRideFaceVent). */
typedef enum AIRS_ROCK_INSIDE_FACE_VENTS_VENT {
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT = 50,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHEAST_VENT,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_WEST_VENT,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_EAST_VENT,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHWEST_VENT,
	METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHEAST_VENT
} AIRS_ROCK_INSIDE_FACE_VENTS_VENT;

/* Where a gust stops on the overhead layer (scAirsRockInsideBlowGust), its code
   less METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP: up to LAST_BARRIER it
   blows on, clearing what blocks its path, as the golden barriers of the paired
   stakes and coins chest rooms; a turning bridge reads BRIDGE_NORTH_SOUTH while
   it runs north and south, and BRIDGE_EAST_WEST while it lies east and west,
   and the gust turns it the other way. */
typedef enum AIRS_ROCK_INSIDE_GUST_STOP {
	GUST_STOP_PAIRED_STAKES_NORTH_BARRIER = 0,
	GUST_STOP_LAST_BARRIER = 4,
	GUST_STOP_BRIDGE_NORTH_SOUTH = 5,
	GUST_STOP_BRIDGE_EAST_WEST = 6
} AIRS_ROCK_INSIDE_GUST_STOP;

/* the first stop's code */
#define METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP 30

/* The cues this scene raises (FIELD_STATE.raisedCue) besides a statue's
   whirlwind carrying the leader (RAISED_RIDE_WHIRLWIND), each room reading them
   its own way: a vent's puff reaching him, and in the Reveal tablet room, a
   metatile he has left, as CUE_LEFT_METATILE plus its collision code. */
typedef enum AIRS_ROCK_INSIDE_RAISED_CUE {
	CUE_PUFF_REACHES_LEADER = 201,
	CUE_LEFT_METATILE = 200,
	CUE_FIRST_VANISHING_STONE = CUE_LEFT_METATILE +
		METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_FIRST_VANISHING_STONE
} AIRS_ROCK_INSIDE_RAISED_CUE;

/* The actors of each inner room, numbering its placement table. The statues
   (SPRITE_WIND_STATUE) breathe whirlwinds once Whirlwind on their room's
   twister rock wakes them; Whirlwind on a Wind Stone (SPRITE_WIND_STONE) sends
   a gust; stone pillars (SPRITE_STONE_PILLAR) are Moved. */
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

/* The Vial chest's room (AirsRockInsideVialChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID {
	ACTOR_VIAL_CHEST_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_VIAL_CHEST_EAST_PILLAR,
	ACTOR_VIAL_CHEST_TWISTER_ROCK,
	ACTOR_VIAL_CHEST_WEST_PILLAR
} AIRS_ROCK_INSIDE_VIAL_CHEST_ACTOR_ID;

/* The Clarity Circlet chest's room
   (AirsRockInsideClarityCircletChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST_ACTOR_ID {
	ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
	ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C
} AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST_ACTOR_ID;

/* The tightropes' room (AirsRockInsideTightropesPlacements). */
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

/* The Elixir chest's room (AirsRockInsideElixirChestPlacements). */
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

/* The paired stakes' room (AirsRockInsidePairedStakesPlacements). */
typedef enum AIRS_ROCK_INSIDE_PAIRED_STAKES_ACTOR_ID {
	ACTOR_PAIRED_STAKES_PILLAR = ACTOR_FIRST_PLACED,
	/* The three pillars pushed among the stakes. */
	ACTOR_PAIRED_STAKES_PILLAR_A,
	ACTOR_PAIRED_STAKES_PILLAR_B,
	ACTOR_PAIRED_STAKES_PILLAR_C,
	/* The four stakes (SPRITE_STAKE), in two pairs: as one sinks, the other
	   rises. */
	ACTOR_PAIRED_STAKES_STAKE_A,
	ACTOR_PAIRED_STAKES_STAKE_B,
	ACTOR_PAIRED_STAKES_STAKE_C,
	ACTOR_PAIRED_STAKES_STAKE_D,
	ACTOR_PAIRED_STAKES_NORTH_WIND_STONE,
	ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE
} AIRS_ROCK_INSIDE_PAIRED_STAKES_ACTOR_ID;

/* The room of the face vents (AirsRockInsideFaceVentsPlacements). */
typedef enum AIRS_ROCK_INSIDE_FACE_VENTS_ACTOR_ID {
	ACTOR_FACE_VENTS_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_FACE_VENTS_SECOND_PILLAR,
	ACTOR_FACE_VENTS_WIND_STONE,
	ACTOR_FACE_VENTS_STATUE,
	ACTOR_FACE_VENTS_THIRD_PILLAR,
	ACTOR_FACE_VENTS_PILLAR_A,
	ACTOR_FACE_VENTS_PILLAR_B
} AIRS_ROCK_INSIDE_FACE_VENTS_ACTOR_ID;

/* The coins chest's room (AirsRockInsideCoinsChestPlacements). */
typedef enum AIRS_ROCK_INSIDE_COINS_CHEST_ACTOR_ID {
	ACTOR_COINS_CHEST_WIND_STONE = ACTOR_FIRST_PLACED,
	ACTOR_COINS_CHEST_PILLAR,
	ACTOR_COINS_CHEST_STATUE
} AIRS_ROCK_INSIDE_COINS_CHEST_ACTOR_ID;

/* The metatile stampers keep their last metatile and the block they copy. */
extern Sint32					   AirsRockInsideStampedMetatile;
Sint32 AirsRockInsideStampedWidth  BSS_DATA;
Sint32 AirsRockInsideStampedHeight BSS_DATA;
Sint32 AirsRockInsideStampedX	   BSS_DATA;
Sint32 AirsRockInsideStampedZ	   BSS_DATA;

#include "scene/scene.h"
#include "field/call.h"
#include "game/game.h"
#include "edition.h"

struct ACTOR;

extern ACTOR_PLACE AirsRockInsidePlacements[];
extern ACTOR_PLACE AirsRockInsideHallPlacements[];
extern ACTOR_PLACE AirsRockInsideVialChestPlacements[];
extern ACTOR_PLACE AirsRockInsideClarityCircletChestPlacements[];
extern ACTOR_PLACE AirsRockInsideRevealTabletPlacements[];

void scAirsRockInsideRestoreMetatileBehindLeader(void);

void scAirsRockInsideBurstDust(Sint32 x, Sint32 z);
void scAirsRockInsideScatterSparks(Sint32 x, Sint32 z);

extern Sint16 AirsRockInsideHallPillars[];
extern Sint16 AirsRockInsideVialChestPillars[];
extern Sint16 AirsRockInsideClarityCircletChestPillars[];
extern Sint16 AirsRockInsideTightropesPillars[];
extern Sint16 AirsRockInsidePairedStakesPillars[];
extern Sint16 AirsRockInsideFaceVentsPillars[];
extern Sint16 AirsRockInsideCoinsChestPillars[];

void scAirsRockInsideStampMetatileUnderLeader(void);
void scAirsRockInsideReadRevealTablet(void);

extern CUE AirsRockInsideHallCues[];
extern CUE AirsRockInsideVialChestCues[];
extern CUE AirsRockInsideClarityCircletChestCues[];
extern CUE AirsRockInsideTightropesCues[];
extern CUE AirsRockInsideRevealTabletCues[];
extern CUE AirsRockInsideElixirChestCues[];
extern CUE AirsRockInsidePairedStakesCues[];
extern CUE AirsRockInsideFaceVentsCues[];
extern CUE AirsRockInsideCoinsChestCues[];
extern CUE AirsRockInsidePsynergyStoneCues[];

void scAirsRockInsideStartRestoringMetatiles(void);
void scAirsRockInsideSetLedgeMetatilesByLevel(void);
void scAirsRockInsideSpawnMotes(void);

void scAirsRockInsideRiseOnLift(void);

void scAirsRockInsidePlaceStakes(Uint16 *list);
void scAirsRockInsideTakeOverWhirlwind(void);
void scAirsRockInsideRideWhirlwind(void);
void scAirsRockInsideBlowGust(void);

void scAirsRockInsideStampOtherMetatileUnderLeader(void);
void scAirsRockInsideStampTallMetatileUnderLeader(void);
void scAirsRockInsideStampTallMetatileUnderLeaderNearer(void);
void scAirsRockInsideWakeHallStatue(void);
void scAirsRockInsideOpenTabletRoomDoor(void);
void scAirsRockInsideStartPushingPillar(void);
void scAirsRockInsideFinishPushingPillar(void);
void scAirsRockInsideWakeVialChestStatue(void);
void scAirsRockInsideJumpDownLedge(void);
void scAirsRockInsideRideClarityCircletVent(void);
void scAirsRockInsideStepOffVanishingStone(void);
void scAirsRockInsideStampWideMetatileUnderLeader(void);
void scAirsRockInsideHideRevealedDoors(void);
void scAirsRockInsideRevealDoors(void);
void scAirsRockInsideWalkToRevealTablet(void);
void scAirsRockInsideReadTabletFacingWest(void);
void scAirsRockInsideReadTabletFacingEast(void);
void scAirsRockInsideMoveElixirChestPillar(void);
void scAirsRockInsidePoundPairedStakesStake(Sint32 mode, Sint32 step);
void scAirsRockInsideStepOntoStake(void);
void scAirsRockInsideRideFaceVent(void);
void scAirsRockInsideCheckPsynergyStone(void);
void scAirsRockInsideIgnoreStep(void);

extern Uint32 FieldEffectMediumScript[];
extern Uint32 AirsRockInsideWindStoneScript[];

void fdActorPlaySndInView(ACTOR *actor, Sint32 snd);

void scAirsRockInsideSetStakeMetatiles(ACTOR *actor);

/* Unused: places the pillars of a list as shared/pillar_place.h does, reading
   its halfwords as signed. */
UNUSED void scPillarPlace(Sint16 *list)
{
	Sint16 *entry;

	if((Uint16)list[0] == ACTOR_LIST_END)
		return;
	for(entry = list; (Uint16)entry[0] != ACTOR_LIST_END; entry += 3) {
		Sint32 id = (Uint16)entry[0];
		ACTOR *actor = fdActorGetById(id);
		Sint32 kind = entry[1];
		Sint32 flg = entry[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetMetatileOpen(id, (Uint16)kind, (Uint16)flg, actor->layer);
	}
}

#include "shared/pound_sink_target.h"

#include "shared/actor_set_metatile_open.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern ACTOR_PLACE AirsRockInsideTightropesPlacements[];
extern ACTOR_PLACE AirsRockInsideElixirChestPlacements[];
extern ACTOR_PLACE AirsRockInsidePairedStakesPlacements[];
extern ACTOR_PLACE AirsRockInsideFaceVentsPlacements[];
extern ACTOR_PLACE AirsRockInsideCoinsChestPlacements[];

/* A mote of light moves on to a random spot, x 176 to 431 pixels and y and z up
   to 31 and 127 pixels past the leader's, to drift there for its timer's
   ticks, swelling slowly or quickly by its lifetime. */
void scAirsRockInsideScatterMote(ACTOR *actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(syRandom() & 0xff) {
		actor->x.fixed = toFIXED(syRandom() & 0xff) + toFIXED(176);
		actor->y.fixed = leader->y.fixed + toFIXED(syRandom() & 31);
		actor->z.fixed = leader->z.fixed + toFIXED(syRandom() & 127);
	}
	actor->userValue = actor->tmr;
	if(actor->tmr == PARTICLE_SLOW_LIFETIME)
		fdActorSelectAnimation(actor, ANIM_PARTICLE_SLOW_SWELL);
	else
		fdActorSelectAnimation(actor, ANIM_PARTICLE_QUICK_SWELL);
}

/* A mote's update: after its start delay it sways as it drifts upward, a sway
   every 64 pixels of depth and every 128 of height, scattering again when its
   time is up; it and the map's animations stop while a menu, Psynergy or an
   item is in use. */
void scAirsRockInsideDriftMote(ACTOR *actor)
{
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
		if((TickCount & 7) == 0)
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

/* Nothing visible happens of its own as Felix starts casting Whirlwind on a
   twister rock or a Wind Stone inside the rock; the game clears what is pending
   so that the cast can go on to the rock's or stone's own whirlwind
   (scAirsRockInsideTakeOverWhirlwind). */
void scAirsRockInsideStartWhirlwind(void)
{
	fdScriptEndLastingPsynergy(0);
}

/* Nothing visible happens of its own as Felix casts Whirlwind on a twister rock
   or a Wind Stone inside the rock; the game tells Whirlwind to leave out its
   usual little whirlwind, so that what follows is the rock's or the stone's own
   (scAirsRockInsideBlowGust, scAirsRockInsideWakeHallStatue and
   scAirsRockInsideWakeVialChestStatue). */
void scAirsRockInsideTakeOverWhirlwind(void)
{
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->sceneVariant = 1;
}

#include "shared/map_metatile_set.c"

#include "../field/actor_wait_until_landed.c"

#include "../field/map_metatile_get.c"

/* Sets a stake's metatiles by its flag (its timer), set once it has sunk: a
   sunk stake's metatile is open ground and its height drops 32 pixels unless it
   stands on its ground, drawn under the leader; a raised one is a wall on the
   overhead layer with its top (METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP) on
   the ground layer, 32 pixels higher. */
void scAirsRockInsideSetStakeMetatiles(ACTOR *actor)
{
	Sint32 x = FIELD_METATILE(actor->x.fixed);
	Sint32 z = FIELD_METATILE(actor->z.fixed);
	Sint32 ground = fdMapMetatileGetAttributes(MAPGRID_LAYER_GROUND, x, z);
	Sint32 metatile = fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z);
	Sint32 height = METATILE_HEIGHT_STEPS(
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));
	Sint32 set = syGameFlagGet(actor->tmr);
	Uint8 *flg = &actor->priFlg;
	Sint32 px = FIELD_METATILE_ORIGIN(x);
	Sint32 pz = FIELD_METATILE_ORIGIN(z);

	if(set) {
		metatile &= ~METATILE_CODE_BITS;
		ground = 0;
		if(METATILE_HEIGHT_STEPS(actor->groundHeight) != height)
			height -= 4;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdSetActorSprPriority(actor, 3);
	} else {
		metatile |= METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, 0);
		ground |= METATILE_ATTRIBUTES(
			METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP, MAP_METATILE_JUMP);
		*flg &= ~ACTOR_PRIORITY_UNDERFOOT;
		height += 4;
		fdSetActorSprPriority(actor, 1);
	}
	scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, x, z, ground);
	scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD, x, z, metatile);
	fdMapGridSetMetatileHeight(px, pz, actor->layer, height);
}

/* A stake's picture in OBJ VRAM (stake.png): 32 by 64 pixels of 256-colour
   tiles, four to each 8-pixel row, the stake itself within the first three of
   them. A partner rising is drawn from its target's picture: the top
   STAKE_TOP_ROWS rows at once, then a row a step. */
enum { STAKE_ROW_TILES = 4, STAKE_DRAWN_TILES = 3, STAKE_TOP_ROWS = 3 };

/* The bytes of one 8-pixel row of a stake's picture. */
#define STAKE_ROW_BYTES (STAKE_ROW_TILES * TILE_SIZE_8BPP)

/* One step of Pound on a stake: the target sinks and its partner, the other of
   its pair, rises. On step 0 the partner's picture is cleared and the top of
   the target's drawn over it, 28 pixels up; on steps 1 to 5 a row more of it is
   drawn as it comes down; through step 7 the target flattens. Once the fist has
   landed (POUND_STAGE_LANDED) both settle, the target's flag is set and the
   partner's cleared, they take their next poses and their metatiles are set
   again (scAirsRockInsideSetStakeMetatiles). */
void scAirsRockInsidePoundStake(Sint32 mode, Sint32 step)
{
	Sint16 *pair = &((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->tgtId;
	Sint32	even = *pair - ((*(Uint16 *)pair & 1) << 1);
	ACTOR  *first = fdActorGetById(*pair);
	ACTOR  *second = fdActorGetById(even + 1);
	FIELD_SPR		 *firstSpr, *secondSpr;
	VRAM_CACHE_ENTRY *entry;

	if(step == 0) {
		firstSpr = first->spr;
		secondSpr = second->spr;
		second->motionFlg = 0;
		second->y.fixed -= toFIXED(28);
		sySynch(1);
		entry = &VramCacheEntries[secondSpr->slot];
		second->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
		entry = &VramCacheEntries[firstSpr->slot];
		first->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
		DMA_FILL32(step, second->watchedActor, entry->size);
		syDMAXCopy(first->watchedActor, second->watchedActor,
			DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4), REG_DMA3);
		first->watchedActor = (void *)((Uint8 *)first->watchedActor +
			STAKE_TOP_ROWS * STAKE_ROW_BYTES);
		second->watchedActor = (void *)((Uint8 *)second->watchedActor +
			STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	} else if(step <= 5) {
		second->y.fixed += toFIXED(26) / 5;
		syDMAXCopy(first->watchedActor, second->watchedActor,
			DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
		first->watchedActor =
			(void *)((Uint8 *)first->watchedActor + STAKE_ROW_BYTES);
		second->watchedActor =
			(void *)((Uint8 *)second->watchedActor + STAKE_ROW_BYTES);
	}
	if(step <= 7)
		first->scaleY += -(toFIXED(31) / 256);
	if(mode == POUND_STAGE_LANDED) {
		first->scaleY = toFIXED(1);
		second->y.fixed = second->groundHeight;
		syGameFlagSet(first->tmr);
		syGameFlagClear(second->tmr);
		fdActorSelectAnimation(first, ((FIELD_SPR *)first->spr)->lastNo + 1);
		fdActorSelectAnimation(second, ((FIELD_SPR *)second->spr)->lastNo - 1);
		scAirsRockInsideSetStakeMetatiles(first);
		scAirsRockInsideSetStakeMetatiles(second);
	}
}

/* Places the stakes of a list ended by ACTOR_LIST_END, each entry three
   halfwords: the stake, the first of its poses and the flag that says whether
   it has sunk, which picks between them. The stake stands still, no longer
   solid, and keeps its flag in its timer
   (scAirsRockInsideSetStakeMetatiles). */
void scAirsRockInsidePlaceStakes(Uint16 *list)
{
	ACTOR  *actor;
	Sint32	flag, base;
	Sint16	pose;
	Uint16 *entry;

	if(*list == ACTOR_LIST_END)
		return;
	for(entry = list; *entry != ACTOR_LIST_END; entry += 3) {
		actor = fdActorGetById(*entry);
		pose = ((Sint16 *)entry)[1];
		flag = ((Sint16 *)entry)[2];
		fdActorSetSprFlags(actor, 0);
		{
			Sint32 choice = syGameFlagGet((Uint16)flag);

			base = (Uint16)pose;
			base += base << 1;
			base += choice;
		}
		fdActorSelectAnimation(actor, base);
		actor->motionFlg = 0;
		actor->collisionFlg &= ~ACTOR_SOLID;
		actor->tmr = flag;
		scAirsRockInsideSetStakeMetatiles(actor);
	}
}

/* The leader rides a statue's whirlwind along its path until it fades. */
void scAirsRockInsideRideWhirlwind(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *ride = &leader->watchedActor->actor;
	ACTOR *statue, *dust;
	Sint32 rise;
	FIXED  dx, dz;
	Sint32 moved, ticks, height;
	Uint16 facing;
	Sint32 angle;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
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
		leader->facing = (Uint8)TickCount * FACING_STEP;
		leader->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
		sySynch(1);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	/* The hall's whirlwind at x 312 lifts him into the Vial chest room. */
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL &&
		FIELD_HALF_METATILE(leader->x.fixed) == 39) {
		fdScriptSetExitRequest(
			AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_VIAL_CHEST_SOUTH_B);
		fdScriptEnd();
		return;
	}
	facing = ride->facing;
	switch(facing) {
	case FACING_NORTH:
		statue = &ride->watchedActor->actor;
		dx = 0;
		dz = 0;
		leader->priFlg = ACTOR_PRIORITY_OVERHEAD;
		leader->x.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(leader->x.fixed) & ~1) + 1);
		leader->y.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->y.fixed) & ~1) + 3);
		leader->z.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(leader->z.fixed) & ~1) + 1);
		for(ticks = 0; ticks != 60; ticks++) {
			moved = 0;
			if(PerWork.data & KEY_UP) {
				dx = 0;
				dz = -toFIXED(16);
				leader->facing = FACING_NORTH;
				moved = 1;
			}
			if(PerWork.data & KEY_RIGHT) {
				dx = toFIXED(16);
				dz = 0;
				leader->facing = FACING_EAST;
				moved = 1;
			}
			if(PerWork.data & KEY_LEFT) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
				dz = 0;
				moved = 1;
			}
			if(moved &&
				METATILE_CODE(fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
					FIELD_METATILE(ride->x.fixed + dx),
					FIELD_METATILE(ride->z.fixed - ride->y.fixed + dz))) !=
					METATILE_BEHAVIOR_WALL)
				goto found;
			sySynch(1);
		}
		dx = 0;
		dz = -toFIXED(16);
		leader->facing = FACING_NORTH;
		if((Uint16)METATILE_CODE(fdMapMetatileGetAttributes(
			   MAPGRID_LAYER_OVERHEAD, FIELD_METATILE(ride->x.fixed),
			   FIELD_METATILE(ride->z.fixed - ride->y.fixed + dz))) ==
			METATILE_BEHAVIOR_WALL) {
			dx = toFIXED(16);
			dz = 0;
			leader->facing = FACING_EAST;
			if((Uint16)METATILE_CODE(fdMapMetatileGetAttributes(
				   MAPGRID_LAYER_OVERHEAD, FIELD_METATILE(ride->x.fixed + dx),
				   FIELD_METATILE(ride->z.fixed - ride->y.fixed))) ==
				METATILE_BEHAVIOR_WALL) {
				dx = -toFIXED(16);
				leader->facing = FACING_WEST;
			}
		}
	found:
		/* The statue's sceneCnt holds the layer of the ledge he lands on. */
		height = fdMapGetTerrainHeight(
			statue->sceneCnt, ride->x.fixed + dx, ride->z.fixed + dz);
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
		leader->x.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->x.fixed) & ~1) +
			(angle = FACING_EIGHTH_INDEX(angle)) - 1);
		leader->z.fixed = FIELD_HALF_METATILE_ORIGIN(
			(FIELD_HALF_METATILE(ride->z.fixed) & ~1) - (angle & 2) + 1);
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
		fdActorSetMoveTarget(dust, leader->x.fixed,
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
	fdScriptEnd();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern Uint32 AirsRockInsideWhirlwindFadesScript[];
extern Uint32 AirsRockInsideWhirlwindRisesScript[];

/* How near a whirlwind comes to catch the leader above him while others move
   him, in his metatile's row, in pixels; on his level it is WHIRLWIND_REACH. */
enum { WHIRLWIND_REACH_ABOVE = 16 };

/* A statue's whirlwind: it moves 1.5 pixels a tick the way it faces, upward
   when it faces north, and where the overhead layer reads
   METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END it fades (facing north) or
   rises. The first time it comes within reach of the leader it carries him
   along (RAISED_RIDE_WHIRLWIND). It stops while the leader moves as sand, a
   menu or Psynergy is up, or an item is in use. */
void scAirsRockInsideMoveWhirlwind(ACTOR *shot)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 near;
	Sint32		*leaderPos;
	Sint32		*shotPos;
	FIXED		 speed = toFIXED(3) / 2;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0 ||
		MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SAND) {
		fdActorSetAnimationSpeed(shot, 0);
		return;
	}
	fdActorSetAnimationSpeed(shot, ANIM_STEP_NORMAL);
	if(METATILE_CODE(fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
		   FIELD_METATILE(shot->x.fixed),
		   FIELD_METATILE(shot->z.fixed - shot->y.fixed))) ==
		METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
		shot->sceneByte = 0;
		shot->update = 0;
		Call2((void (*)())fdActorPlaySndInView, (Sint32)shot, SE_SAND_EMERGE);
		if(shot->facing == FACING_NORTH)
			fdActorStartScript(shot, AirsRockInsideWhirlwindFadesScript);
		else
			fdActorStartScript(shot, AirsRockInsideWhirlwindRisesScript);
		return;
	}
	if(shot->sceneCnt == 0) {
		near = 0;
		leaderPos = &leader->x.fixed;
		shotPos = &shot->x.fixed;
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
			if(scPosDistanceAbove(leaderPos, shotPos) <=
					WHIRLWIND_REACH_ABOVE &&
				FIELD_METATILE(leader->z.fixed) ==
					FIELD_METATILE(shot->z.fixed))
				near = 1;
		} else {
			if(scPosFlatDistance(leaderPos, shotPos) <= WHIRLWIND_REACH &&
				FIELD_METATILE(leader->y.fixed) ==
					FIELD_METATILE(shot->y.fixed))
				near = 1;
		}
		if(near && !fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
			leader->facing = shot->facing;
			leader->watchedActor = (FIELD_OBJECT *)shot;
			leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)leader->spr)->pri = ((FIELD_SPR *)shot->spr)->pri;
			state->raisedCue = RAISED_RIDE_WHIRLWIND;
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			shot->sceneCnt = 1;
			shot->sceneByte = 1;
		}
	}
	if(shot->facing == FACING_NORTH) {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->y.fixed -= syMulFX(sySin(shot->facing), speed);
	} else {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->z.fixed += syMulFX(sySin(shot->facing), speed);
	}
}

/* A woken statue's update: while no menu, Psynergy or item is in use it counts
   ticks in its timer, and every WHIRLWIND_BREATH_TICKS breathes a new
   whirlwind 8 pixels ahead of it, with a puff of dust below and a flare unless
   the leader is riding one. */
void scAirsRockInsideStatueBreathesWhirlwind(ACTOR *actor)
{
	FIELD_STATE	  *state = Ram_HeapSlots->fieldState;
	ACTOR		  *shot;
	VECTOR3		   pos;
	EFFECT_OPTIONS options;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0)
		return;
	if(actor->tmr == WHIRLWIND_BREATH_TICKS) {
		if(!syGameFlagGet(FLAG_RIDING_WHIRLWIND))
			fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		if(actor->facing == FACING_NORTH) {
			pos.x = actor->x.fixed;
			pos.z = actor->z.fixed;
		} else {
			pos.x =
				actor->x.fixed + syMathMulFX(toFIXED(8), syCos(actor->facing));
			pos.z =
				actor->z.fixed + syMathMulFX(toFIXED(8), sySin(actor->facing));
		}
		shot = (ACTOR *)syImportActorCreate(
			SPRITE_WHIRLWIND, pos.x, actor->y.fixed, pos.z);
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
		scFieldEffectSpawn(pos.x, actor->y.fixed, pos.z - toFIXED(6), 0, 0, 0,
			EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
	}
	actor->tmr++;
}

/* Wakes a statue unless the party keeps its place: it breathes its first
   whirlwind once its timer reaches WHIRLWIND_BREATH_TICKS, and its whirlwinds
   set the leader down on the ledge of layer `landingLayer`. */
void scAirsRockInsideWakeStatue(ACTOR *actor, Sint16 timer, Uint8 landingLayer)
{
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		actor->steeringFlg = 0;
		actor->tmr = timer;
		actor->sceneCnt = landingLayer;
		actor->update = (Sint32)scAirsRockInsideStatueBreathesWhirlwind;
	}
}

extern Uint32 AirsRockInsidePooledWhirlwindFadesScript[];

/* A whirlwind of the hall's movable statue, one of three it uses in turn: as
   scAirsRockInsideMoveWhirlwind, but it also holds still until the room's
   opening transition has begun (FIELD_STATE.transitionActive). */
void scAirsRockInsideMovePooledWhirlwind(ACTOR *shot)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 near;
	FIXED		 speed = toFIXED(3) / 2;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0 || state->transitionActive == 0) {
		fdActorSetAnimationSpeed(shot, 0);
		return;
	}
	fdActorSetAnimationSpeed(shot, ANIM_STEP_NORMAL);
	if(METATILE_CODE(fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
		   FIELD_METATILE(shot->x.fixed),
		   FIELD_METATILE(shot->z.fixed - shot->y.fixed))) ==
		METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_WHIRLWIND_END) {
		Call2((void (*)())fdActorPlaySndInView, (Sint32)shot, SE_SAND_EMERGE);
		shot->sceneByte = 0;
		shot->update = 0;
		fdActorStartScript(shot, AirsRockInsidePooledWhirlwindFadesScript);
		return;
	}
	if(shot->sceneCnt == 0) {
		near = 0;
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS) {
			if(scPosDistanceAbove(&leader->x.fixed, &shot->x.fixed) <=
					WHIRLWIND_REACH_ABOVE &&
				FIELD_METATILE(leader->z.fixed) ==
					FIELD_METATILE(shot->z.fixed))
				near = 1;
		} else {
			if(scPosFlatDistance(&leader->x.fixed, &shot->x.fixed) <=
					WHIRLWIND_REACH &&
				FIELD_METATILE(leader->y.fixed) ==
					FIELD_METATILE(shot->y.fixed))
				near = 1;
		}
		if(near && !fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
			leader->facing = shot->facing;
			leader->watchedActor = (FIELD_OBJECT *)shot;
			leader->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			((FIELD_SPR *)leader->spr)->pri = ((FIELD_SPR *)shot->spr)->pri;
			state->raisedCue = RAISED_RIDE_WHIRLWIND;
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
			shot->sceneCnt = 1;
			shot->sceneByte = 1;
		}
	}
	if(shot->facing == FACING_NORTH) {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->y.fixed -= syMulFX(sySin(shot->facing), speed);
	} else {
		shot->x.fixed += syMulFX(syCos(shot->facing), speed);
		shot->z.fixed += syMulFX(sySin(shot->facing), speed);
	}
}

/* The movable statue's update: as scAirsRockInsideStatueBreathesWhirlwind, but
   breathing the first of its three whirlwinds that is not on its way. */
void scAirsRockInsideStatueBreathesPooledWhirlwind(ACTOR *actor)
{
	FIELD_STATE	  *state = Ram_HeapSlots->fieldState;
	ACTOR		  *shot;
	VECTOR3		   pos;
	EFFECT_OPTIONS options;
	Sint32		   i;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0)
		return;
	/* DEAD: the three whirlwinds looked up, results unused; the ROM keeps the
	   three calls. */
	for(i = 0; i != 3;)
		fdActorGetById(ACTOR_HALL_FIRST_WHIRLWIND + i++);
	if(actor->tmr == WHIRLWIND_BREATH_TICKS) {
		shot = 0;
		if(!syGameFlagGet(FLAG_RIDING_WHIRLWIND))
			fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		if(actor->facing == FACING_NORTH) {
			pos.x = actor->x.fixed;
			pos.z = actor->z.fixed;
		} else {
			pos.x =
				actor->x.fixed + syMathMulFX(toFIXED(8), syCos(actor->facing));
			pos.z =
				actor->z.fixed + syMathMulFX(toFIXED(8), sySin(actor->facing));
		}
		for(i = 0; i != 3; i++) {
			shot = fdActorGetById(ACTOR_HALL_FIRST_WHIRLWIND + i);
			if(shot->update == 0)
				break;
		}
		shot->x.fixed = pos.x;
		shot->y.fixed = actor->y.fixed;
		shot->z.fixed = pos.z;
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
		scFieldEffectSpawn(pos.x, actor->y.fixed, pos.z - toFIXED(6), 0, 0, 0,
			EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
	}
	actor->tmr++;
}

/* Wakes the hall's movable statue as scAirsRockInsideWakeStatue does. */
void scAirsRockInsideWakeMovableStatue(
	ACTOR *actor, Sint16 timer, Uint8 landingLayer)
{
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		actor->steeringFlg = 0;
		actor->tmr = timer;
		actor->sceneCnt = landingLayer;
		actor->update = (Sint32)scAirsRockInsideStatueBreathesPooledWhirlwind;
		fdActorSetSprFlags(actor, 0);
	}
}

/* A vent's puff of steam: it drifts and grows until its time runs out; reaching
   the leader it bounces back and carries him (CUE_PUFF_REACHES_LEADER), and it
   bounces off the room's pillars. */
void scAirsRockInsideUpdateVentPuff(ACTOR *actor)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR		*other;
	Sint32		*pos;
	Uint8		 i;

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
		} else {
			actor->z.fixed = 0;
			actor->x.fixed = 0;
		}
		return;
	}
	if(scPosFlatDistance(&leader->x.fixed, pos = &actor->x.fixed) <= 8 &&
		leader->y.part[0].integer == actor->y.part[0].integer) {
		state->raisedCue = CUE_PUFF_REACHES_LEADER;
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
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
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
	actor->scaleX += toFIXED(1) / 20;
	actor->scaleY += toFIXED(1) / 20;
}

/* A puff of steam blown from a vent. */
void scAirsRockInsidePuffVent(Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR *actor = syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);

	fdActorSelectAnimation(actor, 1);
	fdActorSetSprFlags(actor, 0);
	fdActorSetSprCol(actor, SPRITE_COLOR_GREY);
	fdActorSetBlendMode(actor, 1);
	actor->tmr = 27;
	actor->motionFlg = 0;
	actor->steeringFlg = 0;
	actor->scaleX = toFIXED(1) / 2;
	actor->speedLimit = toFIXED(4);
	actor->acceleration = ((syRandom() & 0x7fff) << 1) - (toFIXED(1) / 2 - 1);
	actor->sceneWord = toFIXED(7) / 32;
	actor->update = (Sint32)scAirsRockInsideUpdateVentPuff;
}

/* An event: on two ticks in eight, while no menu, Psynergy or item is in use,
   a vent puffs steam (scAirsRockInsidePuffVent). In the Clarity Circlet chest
   room the west vent puffs, or on odd ticks the east one until its pillar is
   placed; in the face vents room a vent near the leader puffs, the scene's
   flags passing over a vent a pillar has blocked. */
void scAirsRockInsidePuffVents(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Uint32		 frm;
	Sint32		 scene;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0 ||
		state->item != 0)
		return;
	frm = TickCount;
	if((frm & 6) != 0)
		return;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		FIXED x;

		if((frm & 1) &&
			!syGameFlagGet(
				FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED))
			x = toFIXED(376);
		else
			x = toFIXED(280);
		scAirsRockInsidePuffVent(x, 0, toFIXED(616));
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		if(frm & 1) {
			if(FIELD_HALF_METATILE(
				   fdActorGetById(MainRoutineStateWork.leaderActorId)
					   ->z.fixed) > 41) {
				if(FIELD_HALF_METATILE(
					   fdActorGetById(MainRoutineStateWork.leaderActorId)
						   ->x.fixed) <= 44)
				puffSouthwestVent:
					scAirsRockInsidePuffVent(toFIXED(168), 0, toFIXED(603));
				else if(!syGameFlagGet(FLAG_MIDDLE_WEST_VENT_STILL))
					scAirsRockInsidePuffVent(toFIXED(504), 0, toFIXED(456));
				else
					goto puffMiddleEastVent;
			} else {
			puffNorthwestVent:
				scAirsRockInsidePuffVent(
					toFIXED(616), -toFIXED(32), toFIXED(136));
			}
		} else {
			if(FIELD_HALF_METATILE(
				   fdActorGetById(MainRoutineStateWork.leaderActorId)
					   ->z.fixed) > 41) {
				if(FIELD_HALF_METATILE(
					   fdActorGetById(MainRoutineStateWork.leaderActorId)
						   ->x.fixed) > 49)
				puffMiddleEastVent:
					scAirsRockInsidePuffVent(toFIXED(552), 0, toFIXED(456));
				else if(syGameFlagGet(FLAG_SOUTHEAST_VENT_STILL))
					goto puffSouthwestVent;
				else
					scAirsRockInsidePuffVent(toFIXED(264), 0, toFIXED(600));
			} else if(syGameFlagGet(FLAG_NORTHEAST_VENT_STILL)) {
				goto puffNorthwestVent;
			} else {
				scAirsRockInsidePuffVent(
					toFIXED(648), -toFIXED(32), toFIXED(136));
			}
		}
	}
}

/* An actor a gust carries: it rides 8 pixels above the gust, its draw order
   flickering. */
void scAirsRockInsideFollowGust(ACTOR *actor)
{
	ACTOR *tgt = &actor->watchedActor->actor;

	actor->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
	SET_ACTOR_POSITION(
		actor, tgt->x.fixed, tgt->y.fixed + toFIXED(8), tgt->z.fixed);
}

void scAirsRockInsideSpiralGust(ACTOR *actor);

/* The actor the gust is cast at: the field effect's target. */
#define TARGET(effect) ((effect)->tgt)

/* The half metatile where a gust drops what it carries: x in the high half and
   z in the low. */
#define GUST_DROP(x, z)	  (((x) / 8 << 16) | (z) / 8)
#define GUST_DROP_X(drop) ((drop) >> 16)

/* The gust picks up an actor it passes over, to drop it at `at` (GUST_DROP). */
#define PICK_UP(actor, at) \
	do { \
		(actor)->watchedActor = (FIELD_OBJECT *)gust; \
		(actor)->update = scAirsRockInsideFollowGust; \
		(actor)->motionFlg = 0; \
		(actor)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC; \
		fdActorSetSprFlags(actor, 0); \
		((FIELD_SPR *)(actor)->spr)->pri = ((FIELD_SPR *)gust->spr)->pri; \
		gust->watchedActor = (FIELD_OBJECT *)(actor); \
		carried = (at); \
	} while(0)

/* One shake of the walls, with the grinding of a gate. */
#define SHAKE() \
	(sySndPlay(SE_GATE_MOVE), fdMapGridSetShake(5, 5, 1), \
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9))

/* Whirlwind on a Wind Stone: a gust rises from it and blows away from the
   leader until a wall or something on the overhead layer stops it, carrying the
   room's pillar or statue it passes (GUST_DROP). A golden barrier in its way is
   blown away, and the turning bridge it reaches turns with a shake, unless the
   Elixir chest room's pillar stands on it. */
void scAirsRockInsideBlowGust(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	ACTOR			   *gust, *spark, *actor;
	VECTOR3				vel;
	EFFECT_OPTIONS		glints, dust;
	CELL				dir, metatile;
	Sint32				i, steps = 0, carried;
	Sint32				x, z;
	Sint32				hit = 0;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_RUSH);
	gust = syImportActorCreate(
		SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	fdActorSelectAnimation(gust, 2);
	fdActorSetSprFlags(gust, 0);
	gust->scaleX = toFIXED(17) / 10;
	gust->scaleY = toFIXED(1) / 5;
	gust->motionFlg = 0;
	((FIELD_SPR *)gust->spr)->pri = ((FIELD_SPR *)TARGET(effect)->spr)->pri;
	fdActorSetMoveTarget(gust, TARGET(effect)->x.fixed, TARGET(effect)->y.fixed,
		TARGET(effect)->z.fixed - toFIXED(1));
	gust->groundHeight = fdMapGetTerrainHeight(0, gust->x.fixed, gust->z.fixed);
	while(gust->scaleY < toFIXED(13) / 10) {
		gust->scaleY += toFIXED(1) / 10;
		syFieldScriptWait(1);
	}
	fdActorWaitForMove(gust);
	gust->z.fixed = TARGET(effect)->z.fixed - toFIXED(1);
	while(gust->scaleY < toFIXED(18) / 10) {
		if((TickCount & 3) == 0) {
			spark =
				syImportActorCreate(SPRITE_PARTICLE, TARGET(effect)->x.fixed,
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
			fdActorStartScript(spark, FieldEffectMediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralGust;
		}
		if(TickCount & 1) {
			gust->scaleX += toFIXED(1) / 100;
			gust->scaleY += toFIXED(1) / 100;
			fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_HIDDEN);
		} else {
			fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_GBR);
		}
		syFieldScriptWait(1);
	}
	fdActorSetSprCol(TARGET(effect), SPRITE_COLOR_NORMAL);
	sySndPlay(SE_WHOOSH);
	metatile.x = FIELD_METATILE(TARGET(effect)->x.fixed);
	metatile.z = FIELD_METATILE(TARGET(effect)->z.fixed);
	if(FIELD_METATILE(leader->x.fixed) < metatile.x) {
		dir.x = 1;
		dir.z = 0;
	} else if(FIELD_METATILE(leader->x.fixed) > metatile.x) {
		dir.x = -1;
		dir.z = 0;
	} else if(FIELD_METATILE(leader->z.fixed) < metatile.z) {
		dir.x = 0;
		dir.z = 1;
	} else {
		dir.x = 0;
		dir.z = -1;
	}
	do {
		hit = fdMapMetatileGetAttributes(
			MAPGRID_LAYER_OVERHEAD, metatile.x, metatile.z);
		hit >>= 8;
		if(fdMapGetTerrainHeight(2, FIELD_METATILE_ORIGIN(metatile.x),
			   FIELD_METATILE_ORIGIN(metatile.z)) != gust->groundHeight)
			hit = 0;
		metatile.x += dir.x;
		metatile.z += dir.z;
		steps += 2;
	} while(hit == 0);
	fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD, metatile.x, metatile.z);
	steps -= 2;
	x = FIXEDtoINT(TARGET(effect)->x.fixed) + ((steps * dir.x) << 3);
	z = FIXEDtoINT(TARGET(effect)->z.fixed) + ((steps * dir.z) << 3);
	((FIELD_SPR *)gust->spr)->pri = 1;
	/* metatile takes the gust's target in pixels: the ROM stores both words in
	   metatile's slots before the move, though nothing reads them. */
	metatile.x = x;
	metatile.z = z;
	gust->speedLimit = toFIXED(4);
	gust->acceleration = toFIXED(4);
	fdActorSetMoveTarget(
		gust, INTtoFIXED(metatile.x), gust->y.fixed, INTtoFIXED(metatile.z));
	fdCameraSetSpeedAndAcceleration(5, 5);
	fdCameraMoveToFX(toFIXED(x), -1, toFIXED(z), 1);
	carried = 0;
	while(fdActorHasArrived(gust) == 0) {
		if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
			actor = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR);
			if(FIELD_HALF_METATILE(gust->x.fixed) ==
					FIELD_HALF_METATILE(actor->x.fixed) &&
				carried == 0)
				PICK_UP(actor, GUST_DROP(312, 296));
		} else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
			actor = fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR);
			if(FIELD_METATILE(gust->x.fixed) ==
					FIELD_METATILE(actor->x.fixed) &&
				FIELD_METATILE(gust->z.fixed) ==
					FIELD_METATILE(actor->z.fixed) &&
				carried == 0)
				PICK_UP(actor, GUST_DROP(328, 840));
		} else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
			actor = fdActorGetById(ACTOR_COINS_CHEST_STATUE);
			if(FIELD_METATILE(gust->x.fixed) ==
					FIELD_METATILE(actor->x.fixed) &&
				FIELD_METATILE(gust->z.fixed) ==
					FIELD_METATILE(actor->z.fixed) &&
				carried == 0)
				PICK_UP(actor, GUST_DROP(120, 408));
		}
		if((TickCount & 1) == 0) {
			glints.pri = 1;
			glints.type = SPRITE_GLINT;
			glints.pal = 2;
			vel.x = syCos((TickCount & 0x1ff) * (TRIG_TURN / 16)) << 1;
			vel.y = gust->y.fixed + toFIXED(syRandom() & 31);
			vel.z = sySin((TickCount & 0x1ff) * (TRIG_TURN / 16));
			scFieldEffectSpawn(gust->x.fixed, vel.y, gust->z.fixed, vel.x, 0,
				vel.z,
				EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
				&glints);
		}
		syFieldScriptWait(1);
	}
	if(carried != 0) {
		actor = &gust->watchedActor->actor;
		actor->x.fixed = FIELD_HALF_METATILE_ORIGIN(GUST_DROP_X(carried));
		/* The x half shifts out, leaving the z one. */
		actor->z.fixed = FIELD_HALF_METATILE_ORIGIN(carried);
		actor->velY = toFIXED(8);
		actor->update = 0;
		actor->motionFlg = ACTOR_FALLS;
		actor->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
		actor->priFlg &= ~(ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_OVERHEAD);
		fdActorSetSprFlags(actor, 1);
		fdSetActorSprPriority(actor, 2);
		if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
			actor->groundHeight = -toFIXED(32);
			fdMapGridCopyMetatileAttributes(19, 16, 1, 1, 19, 18);
			syGameFlagSet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
		} else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
			fdMapGridCopyMetatileAttributes(19, 38, 1, 1, 20, 38);
			fdActorWaitUntilLanded(actor);
			sySynch(1);
			fdActorWaitUntilLanded(actor);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			fdMapGridSetMetatileHeight(toFIXED(320), toFIXED(832), 0, 4);
		} else if(MainRoutineStateWork.next ==
			SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
			fdActorWaitUntilLanded(actor);
			sySynch(1);
			fdActorWaitUntilLanded(actor);
			fdActorSetSprFlags(actor, 0);
			actor->motionFlg = 0;
			actor->groundHeight = 0;
			actor->y.fixed = 0;
			fdMapGridSetMetatileHeight(toFIXED(120), toFIXED(408), 0, 4);
			if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
				scAirsRockInsideWakeStatue(actor, 60, 0);
		}
	}
	if(hit == METATILE_BEHAVIOR_WALL) {
		sySndPlay(SE_WHOOSH);
	} else {
		metatile.x = FIXEDtoINT(gust->x.fixed) + dir.x * 48;
		metatile.z = FIXEDtoINT(gust->z.fixed) + dir.z * 48;
		gust->speedLimit = toFIXED(1) / 2;
		gust->acceleration = toFIXED(1) / 2;
		scFieldActorSetMoveTargetFX(gust, INTtoFIXED(metatile.x), gust->y.fixed,
			INTtoFIXED(metatile.z));
		hit -= METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_GUST_STOP;
		i = 0;
		while(fdActorHasArrived(gust) == 0) {
			if(hit > GUST_STOP_LAST_BARRIER)
				goto shrink;
			if(i == 0) {
				sySndPlay(SE_GUST);
				i = 1;
			}
			if((TickCount & 1) == 0) {
				dust.pri = 1;
				dust.startScaleX = dust.startScaleY =
					(syRandom() & 0xffff) + toFIXED(1);
				vel.x = syCos((TickCount & 0x1ff) * (TRIG_TURN / 16)) << 1;
				vel.y = toFIXED(1);
				vel.z = sySin((TickCount & 0x1ff) * (TRIG_TURN / 16));
				scFieldEffectSpawn(gust->x.fixed, gust->y.fixed, gust->z.fixed,
					vel.x, vel.y, vel.z,
					EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
			}
			if((TickCount & 3) == 0) {
				if(MainRoutineStateWork.next ==
					SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
					syGameFlagSet(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
					metatile.x = FIELD_METATILE(gust->x.fixed) + 64;
					metatile.z = FIELD_METATILE(gust->z.fixed);
					fdMapGridCopyMetatileTilesIndicesRect(
						90, 25, metatile.x + 1, metatile.z, 1, 3);
					fdMapGridCopyMetatileTilesIndicesRect(21, 88, 24, 89, 1, 2);
					fdMapCopyMetatileAttributeRect(23, 92, 2, 1, 23, 89);
					fdMapCopyMetatileAttributeRect(18, 33, 2, 1, 23, 25);
				} else if(MainRoutineStateWork.next ==
					SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
					metatile.x = FIELD_METATILE(gust->x.fixed);
					metatile.z =
						FIELD_METATILE(gust->z.fixed - gust->y.fixed) + 64;
					if(hit == GUST_STOP_PAIRED_STAKES_NORTH_BARRIER) {
						syGameFlagSet(
							FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY);
						fdMapGridCopyMetatileTilesIndicesRect(
							39, 82, metatile.x, metatile.z, 1, 1);
						fdMapCopyMetatileAttributeRect(39, 82, 1, 1, 38, 82);
						fdMapGridCopyMetatileAttributes(38, 19, 1, 1, 38, 18);
					} else {
						syGameFlagSet(
							FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY);
						fdMapGridCopyMetatilesTo(
							39, 82, metatile.x, metatile.z, 1, 1);
						fdMapGridCopyMetatileAttributes(38, 98, 1, 1, 38, 99);
						fdMapGridCopyMetatileAttributes(38, 35, 1, 1, 38, 37);
					}
				} else if(MainRoutineStateWork.next ==
					SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
					syGameFlagSet(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY);
					metatile.x = FIELD_METATILE(gust->x.fixed);
					fdMapGridCopyMetatilesTo(15, 80, metatile.x, 77, 1, 3);
					fdMapGridCopyMetatileAttributes(15, 80, 1, 1, 15, 79);
					fdMapGridCopyMetatileAttributes(17, 15, 2, 1, 15, 15);
				}
				fdMapGridCenterOnFollowedPoint();
			}
			syFieldScriptWait(1);
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
		gust->scaleX -= toFIXED(3) / 20;
		gust->scaleY -= toFIXED(1) / 4;
		gust->y.fixed += toFIXED(7) / 2;
		syFieldScriptWait(1);
	}
	fdActorRelease(gust);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		if(hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
			SHAKE();
			fdMapGridCopyMetatilesTo(79, 64, 29, 85, 5, 7);
			syFieldScriptWait(30);
			SHAKE();
			fdMapGridCopyMetatilesTo(69, 64, 29, 85, 5, 7);
			fdMapGridCopyMetatileAttributes(46, 20, 5, 5, 29, 21);
			fdMapGridCopyMetatileAttributes(32, 89, 1, 1, 31, 89);
			fdMapGridCopyMetatileAttributes(50, 86, 1, 1, 33, 87);
			syGameFlagSet(FLAG_TIGHTROPES_BRIDGE_TURNED);
		} else if(hit == GUST_STOP_BRIDGE_EAST_WEST) {
			SHAKE();
			fdMapGridCopyMetatilesTo(74, 64, 29, 85, 5, 7);
			syFieldScriptWait(30);
			SHAKE();
			fdMapGridCopyMetatilesTo(64, 64, 29, 85, 5, 7);
			fdMapGridCopyMetatileAttributes(52, 20, 5, 5, 29, 21);
			fdMapGridCopyMetatileAttributes(33, 88, 1, 1, 33, 87);
			fdMapGridCopyMetatileAttributes(48, 88, 1, 1, 31, 89);
			syGameFlagClear(FLAG_TIGHTROPES_BRIDGE_TURNED);
		}
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		if(hit == GUST_STOP_BRIDGE_NORTH_SOUTH) {
			SHAKE();
			fdMapGridCopyMetatilesTo(79, 64, 32, 79, 5, 7);
			if(FIELD_HALF_METATILE(
				   fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
				ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
				vel.x = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed -
					toFIXED(8);
				vel.y = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
				vel.z = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed +
					toFIXED(8);
				fdActorSetSprPriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
				for(i = 0; i != 8; i++) {
					dust.pri = 1;
					scFieldEffectSpawn(vel.x + toFIXED(syRandom() & 15),
						vel.y + toFIXED(syRandom() & 31), vel.z, 0, 0,
						-(toFIXED(1) / 2), EFFECT_USE_PRIORITY, &dust);
				}
				sySynch(30);
				SHAKE();
				fdMapGridCopyMetatilesTo(64, 64, 32, 79, 5, 7);
				fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->priFlg |=
					ACTOR_PRIORITY_AUTOMATIC;
			} else {
				syFieldScriptWait(30);
				SHAKE();
				fdMapGridCopyMetatilesTo(69, 64, 32, 79, 5, 7);
				fdMapGridCopyMetatileAttributes(49, 15, 5, 5, 32, 15);
				fdMapGridCopyMetatileAttributes(33, 82, 1, 1, 34, 82);
				fdMapGridCopyMetatileAttributes(52, 81, 1, 1, 36, 81);
				syGameFlagClear(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
			}
		} else if(hit == GUST_STOP_BRIDGE_EAST_WEST) {
			SHAKE();
			if(FIELD_HALF_METATILE(
				   fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
				ELIXIR_CHEST_PILLAR_ON_BRIDGE_X) {
				vel.x = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed -
					toFIXED(8);
				vel.y = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->y.fixed;
				vel.z = fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->z.fixed;
				for(i = 0; i != 8; i++) {
					dust.pri = 1;
					scFieldEffectSpawn(vel.x + toFIXED(syRandom() & 15), vel.y,
						vel.z - toFIXED(syRandom() & 31), 0, 0,
						-(toFIXED(1) / 2), EFFECT_USE_PRIORITY, &dust);
				}
				sySynch(20);
			} else {
				fdMapGridCopyMetatilesTo(79, 64, 32, 79, 5, 7);
				sySndPlay(SE_GATE_MOVE);
				syFieldScriptWait(30);
				SHAKE();
				fdMapGridCopyMetatilesTo(64, 64, 32, 79, 5, 7);
				fdMapGridCopyMetatileAttributes(49, 21, 5, 5, 32, 15);
				fdMapGridCopyMetatileAttributes(35, 81, 1, 1, 36, 81);
				fdMapGridCopyMetatileAttributes(50, 82, 1, 1, 34, 82);
				syGameFlagSet(FLAG_ELIXIR_CHEST_BRIDGE_TURNED);
			}
		}
	}
	syFieldScriptWait(30);
	fdCameraMoveToActorById(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

extern SCENE_ENTRANCE AirsRockInsideEntrances[];
extern SCENE_ENTRANCE AirsRockInsideHallEntrances[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInsideHallEntrances;
	return AirsRockInsideEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInsideHallPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
		return AirsRockInsideVialChestPlacements;
	if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
		return AirsRockInsideClarityCircletChestPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
		return AirsRockInsideTightropesPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return AirsRockInsideRevealTabletPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
		return AirsRockInsideElixirChestPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
		return AirsRockInsidePairedStakesPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
		return AirsRockInsideFaceVentsPlacements;
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
		return AirsRockInsideCoinsChestPlacements;
	return AirsRockInsidePlacements;
}

/* Nothing visible of its own: once Felix stands still on a new metatile, the
   stepping stone he last stood on is drawn as before; in the Reveal tablet
   room, leaving a metatile with a collision code raises CUE_LEFT_METATILE plus
   the code, which for a vanishing stone is its cue (CUE_FIRST_VANISHING_STONE
   on). */
void scAirsRockInsideRestoreMetatileBehindLeader(void)
{
	ACTOR		*actor = fdActorGetById(fdPartyGetLeaderActor());
	Sint32		 x = FIELD_METATILE(actor->x.fixed);
	Sint32		 z = FIELD_METATILE(actor->z.fixed);
	Sint32		 key = METATILE_KEY(x, z);
	Sint32		 last;
	Sint32		 value;
	FIELD_STATE *state;

	if(actor->velX != 0 || actor->velZ != 0)
		return;
	last = AirsRockInsideStampedMetatile;
	if(last == -1 || key == last)
		return;
	x = METATILE_KEY_X(last);
	z = METATILE_KEY_Z(last);
	AirsRockInsideStampedMetatile = -1;
	fdMapGridCopyMetatilesLayered(73, 72, AirsRockInsideStampedWidth,
		AirsRockInsideStampedHeight, AirsRockInsideStampedX,
		AirsRockInsideStampedZ);
	if(MainRoutineStateWork.next != SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return;
	state = Ram_HeapSlots->fieldState;
	value =
		METATILE_CODE(fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD, x, z));
	if(value != 0) {
		state->raisedCue = CUE_LEFT_METATILE + value;
	}
}

/* Starts scAirsRockInsideRestoreMetatileBehindLeader with no metatile stamped
   yet. */
void scAirsRockInsideStartRestoringMetatiles(void)
{
	Sint32 value = -1;

	AirsRockInsideStampedMetatile = value;
	sySetEvent(scAirsRockInsideRestoreMetatileBehindLeader, EVENT_PASS_TICK);
}

/* As Felix steps onto one of the yellow stepping stones marked with a diamond,
   in the great hall and the rooms around it, the stone is drawn again under his
   feet, in front of what lies behind it, so that he is seen standing on it;
   nothing else changes. The stone goes back to its own picture once he has left
   it (scAirsRockInsideRestoreMetatileBehindLeader).

   The metatiles under him are saved at (73, 72) and the stone's picture kept at
   (70, 72) is copied in their place. */
void scAirsRockInsideStampMetatileUnderLeader(void)
{
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	Sint32 x = FIELD_METATILE(actor->x.fixed);
	Sint32 y = FIELD_METATILE(actor->y.fixed);
	Sint32 z = FIELD_METATILE(actor->z.fixed);
	Sint32 metatile = METATILE_KEY(x, z);
	Sint32 scene;

	if(metatile == AirsRockInsideStampedMetatile)
		return;
	AirsRockInsideStampedMetatile = metatile;
	AirsRockInsideStampedWidth = 1;
	AirsRockInsideStampedHeight = 2;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
		scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST ||
		scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		AirsRockInsideStampedX = x;
		AirsRockInsideStampedZ = z - y + 64;
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
		scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST ||
		scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES ||
		scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		AirsRockInsideStampedX = x + 64;
		AirsRockInsideStampedZ = z - y;
	}
	fdMapGridCopyMetatilesLayered(AirsRockInsideStampedX,
		AirsRockInsideStampedZ, AirsRockInsideStampedWidth,
		AirsRockInsideStampedHeight, 73, 72);
	fdMapGridCopyMetatilesLayered(70, 72, AirsRockInsideStampedWidth,
		AirsRockInsideStampedHeight, AirsRockInsideStampedX,
		AirsRockInsideStampedZ);
}

/* The same for the stepping stones of a second kind, in the hall, the Vial
   chest room, the Reveal tablet's room and the Elixir chest room: the stone is
   drawn again under Felix's feet from the picture kept at (71, 72). */
void scAirsRockInsideStampOtherMetatileUnderLeader(void)
{
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	Sint32 x = FIELD_METATILE(actor->x.fixed);
	Sint32 y = FIELD_METATILE(actor->y.fixed);
	Sint32 z = FIELD_METATILE(actor->z.fixed);
	Sint32 metatile = METATILE_KEY(x, z);
	Sint32 scene;

	if(metatile == AirsRockInsideStampedMetatile)
		return;
	AirsRockInsideStampedMetatile = metatile;
	AirsRockInsideStampedWidth = 1;
	AirsRockInsideStampedHeight = 2;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL ||
		scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		AirsRockInsideStampedX = x;
		AirsRockInsideStampedZ = z - y + 64;
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET ||
		scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		AirsRockInsideStampedX = x + 64;
		AirsRockInsideStampedZ = z - y;
	}
	fdMapGridCopyMetatilesLayered(AirsRockInsideStampedX,
		AirsRockInsideStampedZ, AirsRockInsideStampedWidth,
		AirsRockInsideStampedHeight, 73, 72);
	fdMapGridCopyMetatilesLayered(71, 72, AirsRockInsideStampedWidth,
		AirsRockInsideStampedHeight, AirsRockInsideStampedX,
		AirsRockInsideStampedZ);
}

/* As Felix steps onto a stepping stone stacked on another, three metatiles of
   the picture under him are saved at (73, 72) and the stack's picture kept at
   (72, 72) is drawn in their place. */
#define DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_METATILE(name, n1) \
	void name(void) \
	{ \
		ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor()); \
		Sint32 x = FIELD_METATILE(actor->x.fixed); \
		Sint32 y = FIELD_METATILE(actor->y.fixed); \
		Sint32 z = FIELD_METATILE(actor->z.fixed); \
		Sint32 key = METATILE_KEY(x, z); \
\
		if(key != AirsRockInsideStampedMetatile) { \
			AirsRockInsideStampedMetatile = key; \
			AirsRockInsideStampedWidth = 1; \
			AirsRockInsideStampedHeight = 3; \
			AirsRockInsideStampedX = x; \
			AirsRockInsideStampedZ = z - y + n1; \
			fdMapGridCopyMetatilesLayered( \
				x, AirsRockInsideStampedZ, 1, 3, 73, 72); \
			fdMapGridCopyMetatilesLayered(72, 72, AirsRockInsideStampedWidth, \
				AirsRockInsideStampedHeight, AirsRockInsideStampedX, \
				AirsRockInsideStampedZ); \
		} \
	}

/* As Felix steps onto one of the stacked stepping stones in the great hall, the
   stack is drawn again under his feet, in front of the wall behind it, so that
   he is seen standing on top of it; nothing else changes. */
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_METATILE(
	scAirsRockInsideStampTallMetatileUnderLeader, 64)

/* The same for the stacked stepping stones one row nearer the screen. */
DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_METATILE(
	scAirsRockInsideStampTallMetatileUnderLeaderNearer, 63)

/* A burst of eight puffs of dust around x, z, and the walls shake. */
void scAirsRockInsideBurstDust(Sint32 x, Sint32 z)
{
	EFFECT_OPTIONS options;
	VECTOR3		   spawn;
	Sint32		   i;

	options.pri = 1;
	options.startScaleX = toFIXED(2);
	options.startScaleY = toFIXED(2);
	options.tgtScaleX = toFIXED(1) / 2;
	options.tgtScaleY = toFIXED(1) / 2;
	for(i = 0; i != 8; i++) {
		spawn.x = x + toFIXED(syRandom() & 63) - toFIXED(32);
		spawn.y = (syRandom() & 15) << 12;
		spawn.z = z + toFIXED(syRandom() & 7) - toFIXED(16);
		scFieldEffectSpawn(spawn.x, 0, spawn.z, 0, spawn.y, 0,
			EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
				EFFECT_USE_PRIORITY | 1,
			&options);
	}
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/* The sparks scAirsRockInsideScatterSparks raises, 5 ticks apart. */
enum { SCATTERED_SPARKS = 12 };

/* Sparks rise one after another around x, z. */
void scAirsRockInsideScatterSparks(Sint32 x, Sint32 z)
{
	EFFECT_OPTIONS options;
	VECTOR3		   pos;
	Sint32		   i;

	options.type = SPRITE_PARTICLE;
	options.pri = 1;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(3);
	for(i = 0; i != SCATTERED_SPARKS; i++) {
		pos.x = x + toFIXED(syRandom() & 31) - toFIXED(16);
		pos.y = toFIXED(4);
		pos.z = z;
		scFieldEffectSpawn(pos.x, 0, pos.z, 0, pos.y, 0,
			EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
			&options);
		syFieldScriptWait(5);
	}
}

/* A gust's spark: circles the centre kept in bounce and sceneWord, a lap every
   32 ticks, its radius growing as it rises above the ground. */
void scAirsRockInsideSpiralGust(ACTOR *actor)
{
	FIXED radius = ((actor->y.fixed - actor->groundHeight) >> 1) + toFIXED(16);

	actor->x.fixed = actor->bounce +
		syMulFX(syCos((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius);
	actor->z.fixed = actor->sceneWord +
		syMulFX(sySin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius) /
			3;
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
}

/* A spark: circles the centre kept in bounce and sceneWord on an ellipse twice
   as wide as deep, the radius growing with its height; it rises by its gravity
   each tick and turns blue (SPRITE_COLOR_BLUE_TINT) and back every two
   ticks. */
void scAirsRockInsideSpiralSpark(ACTOR *actor)
{
	FIXED  y = actor->y.fixed;
	Sint32 phase;
	FIXED  radius;

	phase = actor->speedLimit;
	radius = toFIXED(12) + (y >> 2);
	actor->x.fixed = actor->bounce +
		syMulFX(syCos((phase & 0xff) * (TRIG_TURN / 32)), radius) * 2;
	actor->z.fixed = actor->sceneWord +
		syMulFX(sySin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius);
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
	fdActorSetSprCol(actor, ((TickCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/* A Whirlwind target's update while the gust gathers: it flickers to the colour
   mode in sceneCnt (a SPR_COL_MODE), at the rate sceneByte sets. */
void scAirsRockInsideFlickerTarget(ACTOR *actor)
{
	Sint32 bit = ((TickCount & 3) >> actor->sceneByte) & 1;

	fdActorSetSprCol(actor, (Uint8)(bit * actor->sceneCnt));
}

/* Felix casts Whirlwind on the twister rock in the great hall: it glows, a ball
   of purple light flies north up the hall to the great stone face, whose eyes
   light red as its mouth opens with a rumble and rocks fall, and from then on
   its statues breathe whirlwinds the party can ride. The rock turns grey,
   spent. */
void scAirsRockInsideWakeHallStatue(void)
{
	Uint16 *buffer = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	Uint16 *metatile;
	ACTOR  *spark;
	Uint32	i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIOWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES), buffer);
	syDMAXCopy(buffer, (void *)BG_CHAR_BLOCK(0),
		DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buffer);
	DMA_FILL16(
		0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK), BG_SCREEN_SIZE);
	buffer = syHeapAlloc(
		LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(Uint16));
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP), buffer);
	metatile = buffer;
	/* The map's tile numbers less 512. */
	for(i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
		*metatile &= 0xfcff;
		metatile++;
	}
	metatile = buffer;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(metatile,
			(void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
				i * MAP_ROW_BYTES),
			DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(Uint16) / 4),
			REG_DMA3);
		metatile += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buffer);
	sySndPlay(SE_GLITTER);
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->sceneCnt = SPRITE_COLOR_BLUE_TINT;
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->sceneByte = 1;
	fdActorGetById(ACTOR_HALL_TWISTER_ROCK)->update =
		scAirsRockInsideFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((TickCount & 3) == 0) {
			spark = syImportActorCreate(SPRITE_PARTICLE,
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
			fdActorStartScript(spark, FieldEffectMediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralSpark;
		}
		syFieldScriptWait(1);
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
	fdActorSetSprCol(
		fdActorGetById(ACTOR_HALL_TWISTER_ROCK), SPRITE_COLOR_GREY);
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
	fdMapGridCopyMetatileTilesIndicesRect(64, 78, 18, 105, 3, 3);
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
	fdMapGridCopyMetatilesTo(64, 78, 18, 105, 3, 5);
	fdCameraSetSpeedParms(toFIXED(3) / 2, toFIXED(3) / 16);
	fdCameraMoveToActorById(ACTOR_HALL_TWISTER_STATUE, 1);
	fdCameraWaitForArrival();
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetSprColById(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_BLINK);
	scAirsRockInsideScatterSparks(
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->x.fixed,
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->z.fixed);
	fdActorConfigureSprCol(ACTOR_HALL_TWISTER_STATUE, SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE), SPRITE_COLOR_WHITE);
	sySynch(3);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_HALL_WEST_STATUE), SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_HALL_EAST_STATUE), SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE), SPRITE_COLOR_NORMAL);
	sySynch(40);
	scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
	syWindowLoadGraphics();
	syPrintClear();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	syGameFlagSet(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN);
	fdScriptEnd();
}

/* Walking north into the door at the top of the hall's tower: it opens with a
   thump, and the leader walks through into the Reveal tablet's room. */
void scAirsRockInsideOpenTabletRoomDoor(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 width;
	Sint32 height;

	if(METATILE_CODE(fdMapMetatileGetAttributes(MAPGRID_LAYER_MIDDLE,
		   FIELD_METATILE(leader->x.fixed),
		   FIELD_METATILE(leader->z.fixed) - 1)) !=
		METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_THUMP);
	width = 1;
	height = 2;
	fdMapGridCopyMetatileTilesIndicesRect(65, 76, 19, 92, width, height);
	syFieldScriptWait(8);
	fdMapGridCopyMetatileTilesIndicesRect(66, 76, 19, 92, width, height);
	syFieldScriptWait(8);
	fdMapGridCopyMetatileTilesIndicesRect(67, 76, 19, 92, width, height);
	syFieldScriptWait(8);
	fdMapGridCopyMetatileTilesIndicesRect(62, 76, 19, 92, width, height);
	syFieldScriptWait(15);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlg = 0;
	fdActorGetById(MainRoutineStateWork.leaderActorId)->speedLimit =
		fdActorGetById(MainRoutineStateWork.leaderActorId)->acceleration =
			toFIXED(1) / 2;
	sySndPlay(SE_MAP_EXIT);
	fdActorWalkToById(MainRoutineStateWork.leaderActorId, 312, 792);
	syFieldScriptWait(5);
	fdScriptSetExitRequest(
		AIRS_ROCK_INSIDE_HALL_TO_AIRS_ROCK_INSIDE_REVEAL_TABLET);
	fdScriptEnd();
}

/* Nothing visible happens of its own as Felix starts to push a pillar or a
   statue inside the rock, or casts Move on one; the game takes it off the
   metatiles it was blocking so that it can slide. In the paired statues room it
   also smooths two metatiles' ground, and in the coins chest room the statue
   being moved stops breathing whirlwinds. */
void scAirsRockInsideStartPushingPillar(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL) {
		fdActorLiftTargetBlock(AirsRockInsideHallPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		fdActorLiftTargetBlock(AirsRockInsideVialChestPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		fdActorLiftTargetBlock(AirsRockInsideClarityCircletChestPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		fdActorLiftTargetBlock(AirsRockInsideTightropesPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		syFieldMapGridSetMetatileHeight(toFIXED(608), toFIXED(336), 2, 0);
		fdMapGridSetMetatileHeight(toFIXED(624), toFIXED(336), 2, 0);
		fdActorLiftTargetBlock(AirsRockInsidePairedStakesPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		fdActorLiftTargetBlock(AirsRockInsideFaceVentsPillars);
	} else if(scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		fdActorGetById(ACTOR_COINS_CHEST_STATUE)->tmr = 0;
		fdActorLiftTargetBlock(AirsRockInsideCoinsChestPillars);
	}
}

/* Once Felix has pushed or moved a pillar or a statue inside the rock: one left
   over lower ground drops onto it with a thump, and one left in the place it
   belongs stays there on later visits, as a shortcut or a step.

   In the paired statues' room and the face vents room the metatiles along the
   pillar's rows are marked or cleared again, which in the face vents room
   decides which vents it keeps still. */
void scAirsRockInsideFinishPushingPillar(void)
{
	ACTOR *pushed = ((FIELD_STATE *)Ram_HeapSlots->fieldState)->movedBlock;
	Sint32 x, z;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
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
	x = FIELD_HALF_METATILE(pushed->x.fixed);
	z = FIELD_HALF_METATILE(pushed->z.fixed);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
		if(x == 103 && z == 41)
			syGameFlagSet(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED);
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		if(x == 13 && z == 49)
			syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED);
		else if(x == 41 && z == 49)
			syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED);
	} else if(MainRoutineStateWork.next ==
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
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		if(x == 81 && z == 37)
			syGameFlagSet(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED);
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		syFieldMapGridSetMetatileHeight(toFIXED(608), toFIXED(336), 2, -4);
		syFieldMapGridSetMetatileHeight(toFIXED(624), toFIXED(336), 2, -4);
		if(x == 43 && z == 87)
			syGameFlagSet(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED);
		if((FIELD_HALF_METATILE(
				fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_A)->x.fixed) == 73 &&
			   FIELD_HALF_METATILE(
				   fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_A)->z.fixed) ==
				   43) ||
			(FIELD_HALF_METATILE(
				 fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_B)->x.fixed) == 73 &&
				FIELD_HALF_METATILE(
					fdActorGetById(ACTOR_PAIRED_STAKES_PILLAR_B)->z.fixed) ==
					43))
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
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
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
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed),
			0);
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed),
			0);
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		if(x == 45 && z == 33)
			syGameFlagSet(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED);
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
			FIELD_METATILE(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->z.fixed),
			0);
	}
	fdScriptEnd();
}

/* Felix casts Whirlwind on the twister rock in the Vial chest room: it glows, a
   ball of purple light flies north to the room's great stone face, whose eyes
   light red and whose mouth opens with a roar (the metatiles from (12, 77)
   down), and the statue below it wakes and starts breathing whirlwinds. The
   rock turns grey, spent. */
void scAirsRockInsideWakeVialChestStatue(void)
{
	Uint16 *buf = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	Uint16 *metatile;
	ACTOR  *spark;
	Uint32	i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIOWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES), buf);
	syDMAXCopy(buf, (void *)BG_CHAR_BLOCK(0),
		DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buf);
	DMA_FILL16(
		0, (void *)BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK), BG_SCREEN_SIZE);
	buf = syHeapAlloc(
		LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(Uint16));
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP), buf);
	metatile = buf;
	/* The map's tile numbers less 512. */
	for(i = 0; i != LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS; i++) {
		*metatile &= 0xfcff;
		metatile++;
	}
	metatile = buf;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(metatile,
			(void *)(BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
				i * MAP_ROW_BYTES),
			DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(Uint16) / 4),
			REG_DMA3);
		metatile += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buf);
	sySndPlay(SE_GLITTER);
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneCnt =
		SPRITE_COLOR_BLUE_TINT;
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->sceneByte = 1;
	fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK)->update =
		scAirsRockInsideFlickerTarget;
	for(i = 0; i != 45; i++) {
		if((TickCount & 3) == 0) {
			spark = syImportActorCreate(SPRITE_PARTICLE,
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
			fdActorStartScript(spark, FieldEffectMediumScript);
			fdActorSetSprFlags(spark, 0);
			fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scAirsRockInsideSpiralSpark;
		}
		syFieldScriptWait(1);
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
	fdActorSetSprCol(
		fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK), SPRITE_COLOR_GREY);
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
	fdMapGridCopyMetatileTilesIndicesRect(68, 84, 12, 77, 3, 3);
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
	fdMapGridCopyMetatilesTo(68, 84, 12, 77, 3, 6);
	fdCameraSetSpeedParms(toFIXED(3) / 2, toFIXED(3) / 16);
	fdCameraMoveToActorById(ACTOR_VIAL_CHEST_STATUE, 1);
	fdCameraWaitForArrival();
	sySndPlay(SE_ENEMY_CRY);
	fdActorSetSprColById(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_BLINK);
	scAirsRockInsideScatterSparks(
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE)->x.fixed,
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE)->z.fixed + toFIXED(32));
	fdActorConfigureSprCol(ACTOR_VIAL_CHEST_STATUE, SPRITE_COLOR_NORMAL);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE), SPRITE_COLOR_WHITE);
	sySynch(3);
	fdActorSetSprCol(
		fdActorGetById(ACTOR_VIAL_CHEST_STATUE), SPRITE_COLOR_NORMAL);
	sySynch(40);
	scAirsRockInsideWakeStatue(fdActorGetById(ACTOR_VIAL_CHEST_STATUE), 8, 0);
	syWindowLoadGraphics();
	syPrintClear();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	syGameFlagSet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN);
	fdScriptEnd();
}

/* Walking south off the Vial chest room's ledge: the leader jumps down, to land
   in the hall below (scAirsRockInsideLandInHall). */
void scAirsRockInsideJumpDownLedge(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorCreateSilhouette(
		toFIXED(216), -toFIXED(32), toFIXED(544), SPRITE_CRATE);
	fdActorJumpDownLedge(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING, 0);
	fdScriptSetExitRequest(
		AIRS_ROCK_INSIDE_VIAL_CHEST_TO_AIRS_ROCK_INSIDE_HALL_SOUTH);
	fdScriptEnd();
}

/* A puff of steam from one of the vents in the Clarity Circlet chest room
   catches Felix north of the vents and, with a chime, blows him back south to
   metatile (23, 41) in front of them, without turning him round. South of them
   the puff does nothing. */
void scAirsRockInsideRideClarityCircletVent(void)
{

	if(fdActorGetById(MainRoutineStateWork.leaderActorId)->z.part[0].integer >
		653)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetSpeedParms(
		MainRoutineStateWork.leaderActorId, toFIXED(4), toFIXED(2));
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 376, 668);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdScriptEnd();
}

/* A stepping stone the leader has crossed fades with a drip and is gone. */
void scAirsRockInsideVanishStone(Sint32 x, Sint32 y)
{
	ACTOR	  *marker;
	FIELD_SPR *spr;
	Sint32	   i, metatileX, metatileY;

	marker =
		syImportActorCreate(SPRITE_STACKED_CRATES, toFIXED(x), 0, toFIXED(y));
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	fdSetActorSprPriority(marker, 0);
	fdActorSetSprFlags(marker, 0);
	spr = marker->spr;
	spr->objMode = OBJ_MODE_WINDOW;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
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
	metatileX = x >> 4;
	metatileY = (y >> 4) - 1;
	fdMapGridCopyMetatilesTo(65, 1, metatileX + 64, metatileY, 1, 2);
	scMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, metatileX, metatileY,
		METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, 0));
	fdActorRelease(marker);
	fdScriptEnd();
	REG_BLDCNT = 0;
	REG_DISPCNT &= ~DISPCNT_OBJWIN_ON;
}

/* Felix steps off one of the stepping stones that cross the Reveal tablet's
   room, and it fades away behind him with a drip, leaving a gap, for good.
   Stepping from a stone back onto the floor at either end of the row, the stone
   goes only once another already has.

   The floor at the ends is read as
   METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END and _NORTH_END on the
   overhead layer; FLAG_STONE_VANISHED says a stone has gone, and each stone has
   its own flag. */
void scAirsRockInsideStepOffVanishingStone(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*leader;
	Sint32		 metatile, row = 54;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	metatile = fdMapMetatileGetAttributes(MAPGRID_LAYER_OVERHEAD,
		FIELD_METATILE(leader->x.fixed), FIELD_METATILE(leader->z.fixed));
	/* The stone's row in half metatiles: the first's is 50, each next four
	   north. */
	row -= (state->raisedCue - CUE_FIRST_VANISHING_STONE + 1) << 2;
	metatile = METATILE_CODE(metatile);
	if(metatile != METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_SOUTH_END &&
		metatile != METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STONES_NORTH_END) {
		scAirsRockInsideStampMetatileUnderLeader();
		scAirsRockInsideVanishStone(264, row * 8 + 10);
		if(!fdScriptGetFlag(FLAG_STONE_VANISHED))
			syGameFlagSet(FLAG_STONE_VANISHED);
		syGameFlagSet(state->raisedCue - CUE_FIRST_VANISHING_STONE +
			FLAG_FIRST_STONE_VANISHED);
	} else if(syGameFlagGet(FLAG_STONE_VANISHED)) {
		scAirsRockInsideVanishStone(264, row * 8 + 10);
	}
}

/* As Felix steps onto one of the stepping stones on row 19 of the Reveal
   tablet's room, beside its two hidden doorways, it lights up gold under his
   feet, and goes back to yellow once he has left it.

   Two metatiles of the overhead layer under him are saved at (73, 72) and the
   lit stone's picture kept at (70, 74) is drawn in their place. */
void scAirsRockInsideStampWideMetatileUnderLeader(void)
{
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	Sint32 x = FIELD_METATILE(actor->x.fixed);
	Sint32 y = FIELD_METATILE(actor->y.fixed);
	Sint32 z = FIELD_METATILE(actor->z.fixed);
	Sint32 key = METATILE_KEY(x, z);

	if(key != AirsRockInsideStampedMetatile) {
		AirsRockInsideStampedMetatile = key;
		AirsRockInsideStampedWidth = 1;
		AirsRockInsideStampedHeight = 2;
		AirsRockInsideStampedX = x + 64;
		AirsRockInsideStampedZ = z - y;
		fdMapGridCopyMetatilesLayered(
			AirsRockInsideStampedX, AirsRockInsideStampedZ, 1, 2, 73, 72);
		fdMapGridCopyMetatilesLayered(70, 74, AirsRockInsideStampedWidth,
			AirsRockInsideStampedHeight, AirsRockInsideStampedX,
			AirsRockInsideStampedZ);
	}
}

extern Uint32 OrbitSparkScript[];

/* Defines a function that sets the Reveal tablet room's two hidden doorways, at
   metatiles (8, 19) and (24, 19), from the spare metatiles of row 17: their
   ground from column groundColumn, their pictures from column pictureColumn. */
#define DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(name, groundColumn, pictureColumn) \
	void name(void) \
	{ \
		fdMapCopyMetatileAttributeRect(groundColumn, 17, 1, 1, 8, 19); \
		fdMapCopyMetatileAttributeRect(groundColumn, 17, 1, 1, 24, 19); \
		fdMapGridCopyMetatilesTo(pictureColumn, 17, 72, 19, 1, 2); \
		fdMapGridCopyMetatileTilesIndicesRect( \
			pictureColumn, 17, 88, 19, 1, 2); \
	}

/* Nothing visible of its own: as Reveal fades, the Reveal tablet room's two
   hidden doorways, at metatiles (8, 19) and (24, 19), are walls again. */
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(scAirsRockInsideHideRevealedDoors, 26, 90)

/* Felix casts Reveal in the Reveal tablet's room, and two hidden doorways show
   in its walls, at metatiles (8, 19) and (24, 19), for as long as it lasts. */
DEFINE_AIRS_ROCK_INSIDE_SET_DOORS(scAirsRockInsideRevealDoors, 25, 89)

#include "shared/orbit_spark_spawn.h"

/* An orbiter's update: circles 8 pixels above its anchor, closing in 4 pixels a
   step and leaving sparks, until its countdown ends; 1 once it is 2 pixels
   out. */
Bool scAirsRockInsideOrbitAnchor(ORBITER *orbiter)
{
	ACTOR *anchor = orbiter->anchor;

	orbiter->ang += (Sint16)orbiter->speed >> 1;
	orbiter->x = anchor->x.fixed;
	orbiter->y = anchor->y.fixed + toFIXED(8);
	orbiter->z = anchor->z.fixed;
	syFieldVectorAddPolarOffsetFX(
		orbiter->orbitRadius, orbiter->ang, (Sint32)&orbiter->x);
	if(--orbiter->countdown == 0)
		return FALSE;
	if(orbiter->orbitRadius == toFIXED(2))
		return TRUE;
	scOrbitSparkSpawn(orbiter);
	orbiter->orbitRadius -= toFIXED(4);
	orbiter->ang += (Sint16)orbiter->speed >> 1;
	orbiter->x = anchor->x.fixed;
	orbiter->y = anchor->y.fixed + toFIXED(8);
	orbiter->z = anchor->z.fixed;
	syVectorAddPolarOffset(orbiter->orbitRadius, orbiter->ang, &orbiter->x);
	scOrbitSparkSpawn(orbiter);
	orbiter->orbitRadius -= toFIXED(4);
	return TRUE;
}

#include "shared/orbit_update.h"

#include "shared/actor_aim_at_watched.h"

#include "shared/actor_step_to_watched.h"

#include "shared/actor_emit_sparkles.h"

extern Uint32 LessonTabletRiseScript[];
extern Uint32 AirsRockInsideRisingSparkScript[];
extern Sint32 AirsRockInsideOrbitingSparkScript[];
extern Uint32 LessonAdeptRiseScript[];
extern Sint32 LessonSeekingSparkScript[];
extern Sint32 LessonTabletSettleScript[];
extern Sint32 LessonAdeptSettleScript[];

/* The sparks that rise from the tablet as it lifts. */
enum { TABLET_SPARKS = 16 };

/* The sparks that spiral in on Sheba, one set out every
   ORBITING_SPARK_TICKS. */
enum { ORBITING_SPARKS = 16, ORBITING_SPARK_TICKS = 8 };

/* The sparks that then close in on her start every 2 pixels up her, each
   circling out as far as the sine of its height in 64ths of a turn
   (syOrbitUpdate's phase), short of SEEKING_SPARK_PHASES, half a turn, where
   the sine is 0 again. */
enum { SEEKING_SPARK_PHASES = 32 };

/* The tablet rises in a rumble, sparks circle Sheba and she learns Reveal. */
void scAirsRockInsideShebaLearnsReveal(Sint32 tabletId, Sint32 shebaId)
{
	ACTOR *tablet = fdActorGetById(tabletId);
	ACTOR *sheba = fdActorGetById(shebaId);
	Sint32 i;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Sint32 unused[3];

	sySndPlay(SOUND_FADE_OUT_BGM);
	syFieldScriptWait(30);
	fdCameraSetSpeedParms(toFIXED(1) / 2, toFIXED(1) / 16);
	fdCameraMoveWithinMapBounds(toFIXED(tablet->x.part[0].integer),
		toFIXED(tablet->y.part[0].integer), toFIXED(tablet->z.part[0].integer),
		1);
	sySndPlay(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	tablet->motionFlg = 0;
	tablet->acceleration = toFIXED(1) / 5;
	tablet->speedLimit = toFIXED(1) / 5;
	fdActorSetMovementTypeById(tabletId, LessonTabletRiseScript);
	sySndPlay(SE_TABLET_RISE);
	syFieldScriptWait(45);
	fdActorConfigureSprCol(tabletId, SPRITE_COLOR_BLINK);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	for(i = 0; i != TABLET_SPARKS; i++) {
		ACTOR *spark = syImportActorCreate(
			SPRITE_PARTICLE, tablet->x.fixed, tablet->y.fixed, tablet->z.fixed);
		sySndPlay(SE_GLITTER);
		spark->x.fixed += toFIXED((syRandom() & 31) - 16);
		spark->y.fixed += toFIXED((syRandom() & 15) - 8);
		spark->speedLimit = toFIXED(8);
		spark->acceleration = toFIXED(1) / 5;
		spark->motionFlg = 0;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		fdSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		fdActorStartScript(spark, AirsRockInsideRisingSparkScript);
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
		ACTOR *spark = scFieldActorCreate(
			SPRITE_PARTICLE, sheba->x.fixed, sheba->y.fixed, sheba->z.fixed);
		sySndPlay(SE_SPARKS);
		spark->tmr = (syRandom() & 0x7fff) + DEGtoANG(180);
		spark->userValue = ((i & 1) * 2 - 1) << ((syRandom() & 3) + 9);
		spark->motionFlg = 0;
		spark->sceneWord = toFIXED(122);
		spark->watchedActor = (FIELD_OBJECT *)sheba;
		/* All spirals end together, 15 ticks after the loop's last wait. */
		spark->sceneCnt = (ORBITING_SPARKS - i) * ORBITING_SPARK_TICKS + 15;
		fdActorSetSprCol(spark, SPRITE_COLOR_RBG);
		fdSetActorSprPriority(spark, 1);
		fdActorSelectAnimation(spark, 7);
		fdActorSetSprFlags(spark, 0);
		fdActorSetBlendMode(spark, 1);
		syFieldVectorAddPolarOffsetFX(
			spark->sceneWord, spark->tmr, (Sint32)&spark->x.fixed);
		fdActorStartScript(spark, AirsRockInsideOrbitingSparkScript);
		if(i == 3) {
			fdActorSetSprColById(shebaId, SPRITE_COLOR_BLINK);
			sheba->motionFlg = 0;
			sheba->acceleration = toFIXED(1) / 5;
			sheba->speedLimit = toFIXED(1) / 5;
			fdActorSetMovementTypeById(shebaId, LessonAdeptRiseScript);
		}
		sySynch(ORBITING_SPARK_TICKS);
	}
	sySndPlay(SE_SHIMMER);
	sySynch(16);
	for(i = 2; i != SEEKING_SPARK_PHASES; i += 2) {
		ACTOR *spark = scFieldActorCreate(SPRITE_PARTICLE, sheba->x.fixed,
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
		fdActorStartScript(spark, LessonSeekingSparkScript);
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
	fdActorSetMovementTypeById(tabletId, LessonTabletSettleScript);
	fdActorSetMovementTypeById(shebaId, LessonAdeptSettleScript);
	sheba->update = scActorEmitSparkles;
	sySynch(120);
	sheba->update = 0;
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	gmPlayMusic();
	sySynch(1);
}

#include "text/msg_ids.h"

/* Checking the tablet shows its message; the first time
   (FLAG_REVEAL_TABLET_READ) Sheba learns Reveal, as Magma Rock's tablet does
   for Blaze. */
void scAirsRockInsideReadRevealTablet(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	/* "Felix checked the tablet. It has strange characters written on it."
	   "Wielder of Wind's might... Lay your hands upon this stone." "We bestow
	   upon thee the power to see the truth unclouded..." */
	mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
	if(fdScriptGetFlag(FLAG_REVEAL_TABLET_READ) != 0) {
		fdScriptEnd();
	} else {
		fdScriptSetMessageId((Sint32)MsgCheckedTheTablet);
		FieldStateWork->msg++;
		fdActorSetSpeedFX(MainRoutineStateWork.leaderActorId, toFIXED(7) / 10,
			toFIXED(7) / 20);
		fdActorSetSpeedFX(ACTOR_SHEBA, toFIXED(7) / 10, toFIXED(7) / 20);
		fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
		syFieldScriptWait(1);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, 256, 136);
		fdActorWalkToById(ACTOR_SHEBA, 272, 136);
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		fdActorWaitForMoveById(ACTOR_SHEBA);
		fdActorFaceDirectionById(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_WEST, 0);
		syFieldScriptWait(10);
		/* "Do you think I ought to touch it?" */
		fdScriptShowMessageAndWait(ACTOR_SHEBA, 0);
		fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldScriptWait(30);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 256, 152);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
		syFieldScriptWait(20);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
		syFieldScriptWait(20);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
		syFieldScriptWait(30);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
		syFieldScriptWait(20);
		fdActorWalkToAndWait(ACTOR_SHEBA, 264, 137);
		fdActorFaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
		syFieldScriptWait(20);
		/* "Sheba touched the tablet." */
		mnFieldMessageShow(MsgCheckedTheTablet + 2, FIELD_MESSAGE_ZOOM);
		fdMapGridSetShake(5, 7, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridWaitForShakeToSettle();
		scAirsRockInsideShebaLearnsReveal(ACTOR_REVEAL_TABLET, ACTOR_SHEBA);
		fdPartyLearnAbility(SHEBA, ABILITY_REVEAL);
		syGameFlagSet(FLAG_REVEAL_TABLET_READ);
		fdActorTurnToFaceEachOther(
			MainRoutineStateWork.leaderActorId, ACTOR_SHEBA, 0);
		syFieldScriptWait(10);
		fdActorSetAnimationById(ACTOR_SHEBA, ANIM_NOD);
		fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldScriptWait(20);
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
		fdScriptEnd();
	}
}

/* Felix examines the Reveal tablet from the south: read already, it says so;
   otherwise he walks round to its front and Sheba reads it. */
void scAirsRockInsideWalkToRevealTablet(void)
{
	if(syGameFlagGet(FLAG_REVEAL_TABLET_READ)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM);
		fdScriptEnd();
	} else {
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 248, 107);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 248, 136);
		fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 264, 136);
		fdActorFaceDirectionById(
			MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
		syFieldScriptWait(10);
		scAirsRockInsideReadRevealTablet();
	}
}

/* Defines a function for examining the Reveal tablet from one side: once it has
   been read the game only says so; before, Felix walks from sideX round to its
   front, faces it, and Sheba reads it. */
#define DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(name, sideX) \
	void name(void) \
	{ \
		if(syGameFlagGet(FLAG_REVEAL_TABLET_READ)) { \
			fdScriptStart(); \
			fdScriptEndLastingPsynergy(0); \
			mnFieldMessageShow(MsgCheckedTheTablet, FIELD_MESSAGE_ZOOM); \
			fdScriptEnd(); \
		} else { \
			fdActorWalkToAndWaitById( \
				MainRoutineStateWork.leaderActorId, sideX, 136); \
			fdActorWalkToAndWaitById( \
				MainRoutineStateWork.leaderActorId, 264, 136); \
			fdActorFaceDirectionById( \
				MainRoutineStateWork.leaderActorId, FACING_NORTH, 0); \
			syFieldScriptWait(10); \
			scAirsRockInsideReadRevealTablet(); \
		} \
	}

/* Felix examines the Reveal tablet from its east side, facing west: he walks
   round to its front and Sheba reads it, as from the south
   (scAirsRockInsideWalkToRevealTablet). */
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(scAirsRockInsideReadTabletFacingWest, 280)

/* Felix examines the Reveal tablet from its west side, facing east: the
   same. */
DEFINE_AIRS_ROCK_INSIDE_READ_TABLET(scAirsRockInsideReadTabletFacingEast, 248)

/* Nothing happens as Felix walks west into the ledge of the Reveal tablet's
   room. */
void scAirsRockInsideIgnoreStep(void)
{
}

/* Felix casts Move on the pillar in the Elixir chest room; nothing more than
   the move shows, but the game remembers whether it now stands on the turning
   bridge, which then cannot turn under a gust. */
void scAirsRockInsideMoveElixirChestPillar(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(FIELD_HALF_METATILE(
		   fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->x.fixed) ==
		ELIXIR_CHEST_PILLAR_ON_BRIDGE_X)
		syGameFlagSet(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
	else
		syGameFlagClear(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE);
	fdScriptEnd();
}

/* Felix casts Pound on one of the four stakes in the paired stakes room: it
   sinks into the floor and the other stake of its pair rises
   (scAirsRockInsidePoundStake). */
void scAirsRockInsidePoundPairedStakesStake(Sint32 mode, Sint32 step)
{
	scAirsRockInsidePoundStake(mode, step);
}

extern Uint16 AirsRockInsidePairedStakesStakes[];
void		  scAirsRockInsideSetStakeMetatiles(ACTOR *actor);

/* Stepping onto a raised stake's top: unless a pillar stands on its pair, the
   stake sinks under the leader and its partner rises (as
   scAirsRockInsidePoundStake draws it). */
void scAirsRockInsideStepOntoStake(void)
{
	ACTOR			 *first = 0, *second = 0, *leader;
	FIELD_SPR		 *firstSpr, *secondSpr;
	VRAM_CACHE_ENTRY *entry;
	Uint16			 *pair;
	Uint32			  id, i;
	Sint32			  busy;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	pair = AirsRockInsidePairedStakesStakes;
	if(*pair != ACTOR_LIST_END) {
		do {
			id = *pair;
			first = fdActorGetById(id);
			if(FIELD_METATILE(first->x.fixed) ==
					FIELD_METATILE(leader->x.fixed) &&
				FIELD_METATILE(first->z.fixed) ==
					FIELD_METATILE(leader->z.fixed)) {
				second = fdActorGetById(id - ((id & 1) << 1) + 1);
				break;
			}
			pair += 3;
		} while(*pair != ACTOR_LIST_END);
	}
	busy = first->sceneByte | second->sceneByte;
	if(busy != 0)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	firstSpr = first->spr;
	secondSpr = second->spr;
	second->motionFlg = busy;
	second->y.fixed -= toFIXED(28);
	sySynch(1);
	entry = &VramCacheEntries[secondSpr->slot];
	second->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
	entry = &VramCacheEntries[firstSpr->slot];
	first->watchedActor = (void *)(entry->ofs + OBJ_VRAM0);
	DMA_FILL32(busy, second->watchedActor, entry->size);
	syDMAXCopy(first->watchedActor, second->watchedActor,
		DMA_COPY_WORDS(STAKE_TOP_ROWS * STAKE_ROW_BYTES / 4), REG_DMA3);
	first->watchedActor = (void *)((Uint8 *)first->watchedActor +
		STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	second->watchedActor = (void *)((Uint8 *)second->watchedActor +
		STAKE_TOP_ROWS * STAKE_ROW_BYTES);
	leader->motionFlg = busy;
	for(i = 0; i != 8; i++) {
		if(i <= 4) {
			second->y.fixed += toFIXED(26) / 5;
			syDMAXCopy(first->watchedActor, second->watchedActor,
				DMA_COPY_WORDS(STAKE_DRAWN_TILES * TILE_SIZE_8BPP / 4),
				REG_DMA3);
			first->watchedActor =
				(void *)((Uint8 *)first->watchedActor + STAKE_ROW_BYTES);
			second->watchedActor =
				(void *)((Uint8 *)second->watchedActor + STAKE_ROW_BYTES);
		}
		leader->y.fixed += -(toFIXED(7) / 2);
		leader->groundHeight = leader->y.fixed;
		first->scaleY += -(toFIXED(31) / 256);
		sySynch(1);
	}
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	first->scaleY = toFIXED(1);
	second->y.fixed = second->groundHeight;
	syGameFlagSet(first->tmr);
	syGameFlagClear(second->tmr);
	fdActorSelectAnimation(first, ((FIELD_SPR *)first->spr)->lastNo + 1);
	fdActorSelectAnimation(second, ((FIELD_SPR *)second->spr)->lastNo - 1);
	scAirsRockInsideSetStakeMetatiles(first);
	scAirsRockInsideSetStakeMetatiles(second);
	syFieldScriptWait(10);
	fdScriptEnd();
}

/* A vent's puff has reached the leader in the face vents room: standing in
   front of a vent, he is blown down to the place below it with a chime. */
void scAirsRockInsideRideFaceVent(void)
{
	Sint16 *vent;
	Sint32	x, z;

	x = 0;
	z = 0;
	vent = &FieldStateWork->stepCue;
	if(*vent == 0)
		return;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_SOL_SANCTUM_LIGHT_UP);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetSpeedParms(
		MainRoutineStateWork.leaderActorId, toFIXED(4), toFIXED(2));
	switch(*vent) {
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHWEST_VENT:
		/* DEAD: the leader looked up twice, both results unused; the ROM keeps
		   both calls. */
		fdActorGetById(MainRoutineStateWork.leaderActorId);
		fdActorGetById(MainRoutineStateWork.leaderActorId);
		x = 148;
		z = 632;
		break;
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_SOUTHEAST_VENT:
		x = 264;
		z = 652;
		break;
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_WEST_VENT:
		x = 504;
		z = 508;
		break;
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_MIDDLE_EAST_VENT:
		x = 552;
		z = 508;
		break;
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHWEST_VENT:
		x = 616;
		z = 188;
		break;
	case METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_NORTHEAST_VENT:
		x = 648;
		z = 188;
		break;
	}
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, x, z);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	fdScriptEnd();
}

/* Checking the Psynergy Stone restores the party's PP. */
void scAirsRockInsideCheckPsynergyStone(void)
{
	const Uint8 *msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	msg = MsgCheckedThePsynergy;
	/* "Felix checked the Psynergy Stone." */
	mnFieldMessageShow(msg, FIELD_MESSAGE_ZOOM);
	sySndPlay(SE_RECOVERY);
	fdPartyAddPp(PARTY_PP_REPLENISH, 0);
	syFieldScriptWait(10);
	syGameFlagClear(FLAG_NOTHING_HAPPENS);
	/* "Everyone's PP was replenished!" */
	mnFieldMessageShow(msg + 1, FIELD_MESSAGE_ZOOM);
	fdScriptEnd();
}

/* The cue table for the current room. */
const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_AIRS_ROCK_INSIDE_HALL)
		return AirsRockInsideHallCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST)
		return AirsRockInsideVialChestCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST)
		return AirsRockInsideClarityCircletChestCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES)
		return AirsRockInsideTightropesCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET)
		return AirsRockInsideRevealTabletCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST)
		return AirsRockInsideElixirChestCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES)
		return AirsRockInsidePairedStakesCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS)
		return AirsRockInsideFaceVentsCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST)
		return AirsRockInsideCoinsChestCues;
	if(scene == SCENE_AIRS_ROCK_INSIDE_PSYNERGY_STONE)
		return AirsRockInsidePsynergyStoneCues;
	return AirsRockInsideVialChestCues;
}

/* The leader rises 80 pixels on a lift into the room. */
void scAirsRockInsideRiseOnLift(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *prepared, *lift;
#if LIFT_RESTORES_LEADER_PRIORITY
	Uint8 priFlg;
#endif
	Sint32 height;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
#if LIFT_RESTORES_LEADER_PRIORITY
	priFlg = leader->priFlg;
#endif
	prepared = fdActorCreateSilhouette(
		toFIXED(216), -toFIXED(32), toFIXED(544), SPRITE_CRATE);
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
	fdActorSetMoveTarget(
		lift, lift->x.fixed, lift->y.fixed + toFIXED(72), lift->z.fixed);
	sySndPlay(SE_KIRIN);
	fdScriptStartScreenTransition();
	while(!fdActorHasArrived(leader)) {
		leader->facing = (Uint8)TickCount * FACING_STEP;
		leader->priFlg ^= ACTOR_PRIORITY_UNDERFOOT << (TickCount & 1);
		sySynch(1);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	syFieldScriptWait(10);
	fdActorFaceDirectionById(
		MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
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
#if LIFT_RESTORES_LEADER_PRIORITY
	leader->priFlg = priFlg;
#endif
	fdActorRelease(prepared);
	sySynch(1);
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}

/* Eight motes of light drift about the hall, each starting a little after the
   last; the odd ones swell slowly, the even ones quickly. */
void scAirsRockInsideSpawnMotes(void)
{
	ACTOR	  *actor;
	Sint32	   i;
	FIELD_SPR *spr;

	for(i = 0; i != 8; i++) {
		actor = syImportActorCreate(SPRITE_PARTICLE, 0, 0, 0);
		spr = actor->spr;
		spr->pri = 1;
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
		} else {
			actor->tmr = PARTICLE_QUICK_LIFETIME;
			actor->userValue = 0;
		}
		fdActorSetBlendMode(actor, 1);
	}
}

extern Sint32 AirsRockInsideLeaderLevel;

/* An event in the great hall: as the leader climbs, two blocks of metatiles at
   the hall's west and east sides take the collision codes kept for the level he
   is at, the three levels 96 pixels apart. */
void scAirsRockInsideSetLedgeMetatilesByLevel(void)
{
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	Sint32 level;

	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	if(actor->y.fixed + toFIXED(32) >= toFIXED(312))
		level = 2;
	else if(actor->y.fixed + toFIXED(32) >= toFIXED(216))
		level = 1;
	else
		level = 0;
	if(AirsRockInsideLeaderLevel == level)
		return;
	switch(level) {
	case 2: {
		Sint32 x = 7, z = 39;
		fdMapCopyMetatileAttributeRect(64, 64, 6, 4, x, z);
	}
		fdMapCopyMetatileAttributeRect(70, 64, 6, 4, 26, 39);
		break;
	case 1: {
		Sint32 x = 7, z = 39;
		fdMapCopyMetatileAttributeRect(64, 68, 6, 4, x, z);
	}
		fdMapCopyMetatileAttributeRect(70, 68, 6, 4, 26, 39);
		break;
	case 0: {
		Sint32 x = 7, z = 39;
		fdMapCopyMetatileAttributeRect(64, 72, 6, 4, x, z);
	}
		fdMapCopyMetatileAttributeRect(70, 68, 6, 4, 26, 39);
		break;
	}
	AirsRockInsideLeaderLevel = level;
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_AIRS_ROCK_INSIDE_SET_DOORS
#undef DEFINE_AIRS_ROCK_INSIDE_READ_TABLET
#undef DEFINE_AIRS_ROCK_INSIDE_TRACK_LEADER_METATILE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef TARGET
#undef PICK_UP
#undef SHAKE

/* Dropping from the Vial chest room: the leader falls 130 pixels into the hall
   and lands in a ring of dust. */
void scAirsRockInsideLandInHall(void)
{
	ACTOR		  *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	EFFECT_OPTIONS options;
	Uint32		   i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraMoveToFX(-1, -1, -1, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	leader->y.fixed = toFIXED(130);
	leader->gravity = toFIXED(1) / 2;
	leader->bounce = 0;
	leader->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySndPlay(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(24);
	options.pal = SPRITE_COLOR_WHITE;
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	/* A puff each sixteenth of a turn, the last over the first. */
	for(i = 0; i <= TRIG_TURN / FACING_STEP; i++) {
		Sint32 ang;
		FIXED  vel[3];

		ang = i * FACING_STEP;
		vel[0] = syCos(ang);
		vel[1] = 0;
		vel[2] = sySin(ang);
		vel[0] += vel[0] >> 1;
		vel[0] += syRandomBelow(toFIXED(3) / 4);
		vel[0] += -(toFIXED(3) / 8);
		vel[2] += syRandomBelow(toFIXED(3) / 8) + -(toFIXED(3) / 16);
		scFieldEffectSpawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
			vel[0], vel[1], vel[2],
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
				EFFECT_SCRIPT_MEDIUM,
			&options);
	}
	sySndPlay(SE_THUMP);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationById(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
	fdMapGridSetShake(5, 5, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	leader->gravity = toFIXED(1);
	leader->bounce = toFIXED(1) / 4;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptEnd();
}

void scAirsRockInsidePuffVents(void);

Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 i, z;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	syGameFlagSet(FLAG_TIGHTROPES_GUST_PATH_CLEARED);
	syGameFlagSet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED);
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL) {
		scAirsRockInsideStartRestoringMetatiles();
		/* The Japanese edition settles the leader here whatever
		   FLAG_KEEP_PARTY_POSITION says. */
		if(MainRoutineStateWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET &&
			(!AIRS_ROCK_REVEAL_ENTRANCE_KEEPS_POSITION ||
				!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))) {
			fdActorGetById(MainRoutineStateWork.leaderActorId)->layer = 1;
			sySynch(8);
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
		}
		if(MainRoutineStateWork.entrance <= HALL_LAST_GREAT_HALL_ENTRANCE ||
			MainRoutineStateWork.entrance == ENTRANCE_HALL_FROM_REVEAL_TABLET) {
			sySetEvent(
				scAirsRockInsideSetLedgeMetatilesByLevel, EVENT_PASS_TICK);
			REG_BLDALPHA = BLEND_ALPHA(8, 16);
			REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
			scAirsRockInsideSpawnMotes();
		}
		if(MainRoutineStateWork.entrance > HALL_LAST_NORTHEAST_ROOM_ENTRANCE) {
			if(syGameFlagGet(FLAG_AIRS_ROCK_HALL_MOVABLE_STATUE_PLACED))
				fdActorSetPosFX(
					ACTOR_HALL_MOVABLE_STATUE, toFIXED(824), toFIXED(328));
			fdActorPlaceBlocks(AirsRockInsideHallPillars);
		}
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->motionFlg = 0;
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->y.fixed = 0;
		fdActorGetById(ACTOR_HALL_TWISTER_STATUE)->priFlg =
			ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprPriority(ACTOR_HALL_TWISTER_STATUE, 3);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_HALL_STATUE_WOKEN)) {
			fdActorSetSprCol(
				fdActorGetById(ACTOR_HALL_TWISTER_STATUE), SPRITE_COLOR_GREY);
		} else {
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_HALL_TWISTER_STATUE), 8, 0);
			fdActorSetSprCol(
				fdActorGetById(ACTOR_HALL_TWISTER_ROCK), SPRITE_COLOR_GREY);
			fdMapGridCopyMetatilesTo(64, 78, 18, 105, 3, 5);
		}
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
			fdActorSetSprCol(
				fdActorGetById(ACTOR_HALL_WEST_STATUE), SPRITE_COLOR_GREY);
			fdActorSetSprCol(
				fdActorGetById(ACTOR_HALL_EAST_STATUE), SPRITE_COLOR_GREY);
			fdActorSetSprCol(
				fdActorGetById(ACTOR_HALL_MOVABLE_STATUE), SPRITE_COLOR_GREY);
		} else {
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_HALL_WEST_STATUE), 180, 0);
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_HALL_EAST_STATUE), 60, 0);
			scAirsRockInsideWakeMovableStatue(
				fdActorGetById(ACTOR_HALL_MOVABLE_STATUE), 60, 0);
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_WEST_STATUE), 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_EAST_STATUE), 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_HALL_MOVABLE_STATUE), 0);
		if(MainRoutineStateWork.entrance == ENTRANCE_HALL_LANDING &&
			!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
			scAirsRockInsideLandInHall();
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_VIAL_CHEST) {
		REG_BLDALPHA = BLEND_ALPHA(8, 16);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		scAirsRockInsideStartRestoringMetatiles();
		if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_VIAL_CHEST_EAST_PILLAR, toFIXED(328), toFIXED(392));
		if(syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_VIAL_CHEST_WEST_PILLAR, toFIXED(104), toFIXED(392));
		fdActorPlaceBlocks(AirsRockInsideVialChestPillars);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN)) {
			fdActorSetSprCol(
				fdActorGetById(ACTOR_VIAL_CHEST_STATUE), SPRITE_COLOR_GREY);
		} else {
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_VIAL_CHEST_STATUE), 60, 0);
			fdActorSetSprCol(fdActorGetById(ACTOR_VIAL_CHEST_TWISTER_ROCK),
				SPRITE_COLOR_GREY);
			fdMapGridCopyMetatilesTo(68, 84, 12, 77, 3, 6);
		}
		fdActorSetSprFlags(fdActorGetById(ACTOR_VIAL_CHEST_STATUE), 0);
		if(MainRoutineStateWork.entrance == ENTRANCE_VIAL_CHEST_ON_THE_LIFT &&
			!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
			scAirsRockInsideRiseOnLift();
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_CLARITY_CIRCLET_CHEST) {
		scAirsRockInsideStartRestoringMetatiles();
		fdQueueMusic(SE_AMBIENT_STREAM);
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		sySetEvent(scAirsRockInsidePuffVents, EVENT_PASS_TICK);
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
		fdActorPlaceBlocks(AirsRockInsideClarityCircletChestPillars);
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
		} else {
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
		fdActorSetSprFlags(
			fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_A), 0);
		fdActorSetSprFlags(
			fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B), 0);
		fdActorSetSprFlags(
			fdActorGetById(ACTOR_CLARITY_CIRCLET_CHEST_STATUE_C), 0);
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_TIGHTROPES) {
		fdTightropeInit();
		fdTightropePlace(0, ACTOR_TIGHTROPES_POST_A, ACTOR_TIGHTROPES_POST_B);
		fdTightropePlace(1, ACTOR_TIGHTROPES_POST_C, ACTOR_TIGHTROPES_POST_D);
		fdTightropePlace(2, ACTOR_TIGHTROPES_POST_E, ACTOR_TIGHTROPES_POST_F);
		fdTightropePlace(3, ACTOR_TIGHTROPES_POST_G, ACTOR_TIGHTROPES_POST_H);
		fdTightropePlace(4, ACTOR_TIGHTROPES_POST_I, ACTOR_TIGHTROPES_POST_J);
		fdActorGetById(ACTOR_TIGHTROPES_POST_H)->sceneCnt = 1;
		if(syGameFlagGet(FLAG_TIGHTROPES_BRIDGE_TURNED)) {
			fdMapGridCopyMetatilesTo(69, 64, 29, 85, 5, 7);
			fdMapGridCopyMetatileAttributes(46, 20, 5, 5, 29, 21);
			fdMapGridCopyMetatileAttributes(32, 89, 1, 1, 31, 89);
			fdMapGridCopyMetatileAttributes(50, 86, 1, 1, 33, 87);
		} else {
			fdMapGridCopyMetatilesTo(64, 64, 29, 85, 5, 7);
			fdMapGridCopyMetatileAttributes(52, 20, 5, 5, 29, 21);
			fdMapGridCopyMetatileAttributes(33, 88, 1, 1, 33, 87);
			fdMapGridCopyMetatileAttributes(48, 88, 1, 1, 31, 89);
		}
		if(syGameFlagGet(FLAG_TIGHTROPES_GUST_PATH_CLEARED)) {
			fdMapGridCopyMetatilesTo(90, 25, 86, 25, 4, 3);
			fdMapGridCopyMetatilesTo(21, 88, 24, 89, 1, 2);
			fdMapGridCopyMetatileAttributes(23, 92, 2, 1, 23, 89);
			fdMapGridCopyMetatileAttributes(18, 33, 2, 1, 23, 25);
		}
		if(syGameFlagGet(FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_TIGHTROPES_PILLAR, toFIXED(648), toFIXED(296));
		fdActorPlaceBlocks(AirsRockInsideTightropesPillars);
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_REVEAL_TABLET) {
		syGameFlagSet(FLAG_RETREAT_BLOCKED);
		scAirsRockInsideStartRestoringMetatiles();
		for(i = 0; i != 6; i++) {
			if(syGameFlagGet(FLAG_FIRST_STONE_VANISHED + i)) {
				z = 24 - (i << 1);
				fdMapGridCopyMetatilesTo(65, 1, 80, z, 1, 2);
				syFieldMapMetatileSetAttributes(MAPGRID_LAYER_GROUND, 16, z,
					METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, 0));
			}
		}
		fdActorGetById(ACTOR_REVEAL_TABLET)->motionFlg = 0;
		actor = fdActorGetById(ACTOR_REVEAL_TABLET);
		fdActorGetById(ACTOR_REVEAL_TABLET)->groundHeight = toFIXED(16);
		actor->y.fixed = toFIXED(16);
		fdActorSetMovementType(fdActorGetById(ACTOR_REVEAL_TABLET),
			MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_ELIXIR_CHEST) {
		scAirsRockInsideStartRestoringMetatiles();
		fdTightropeInit();
		fdTightropePlace(
			0, ACTOR_ELIXIR_CHEST_POST_A, ACTOR_ELIXIR_CHEST_POST_B);
		fdTightropePlace(
			1, ACTOR_ELIXIR_CHEST_POST_C, ACTOR_ELIXIR_CHEST_POST_D);
		fdTightropePlace(
			2, ACTOR_ELIXIR_CHEST_POST_E, ACTOR_ELIXIR_CHEST_POST_F);
		if(!syGameFlagGet(FLAG_ELIXIR_CHEST_BRIDGE_TURNED)) {
			fdMapGridCopyMetatilesTo(69, 64, 32, 79, 5, 7);
			fdMapGridCopyMetatileAttributes(49, 15, 5, 5, 32, 15);
			fdMapGridCopyMetatileAttributes(33, 82, 1, 1, 34, 82);
			fdMapGridCopyMetatileAttributes(52, 81, 1, 1, 36, 81);
		} else {
			fdMapGridCopyMetatilesTo(64, 64, 32, 79, 5, 7);
			fdMapGridCopyMetatileAttributes(49, 21, 5, 5, 32, 15);
			fdMapGridCopyMetatileAttributes(35, 81, 1, 1, 36, 81);
			fdMapGridCopyMetatileAttributes(50, 82, 1, 1, 34, 82);
		}
		if(syGameFlagGet(FLAG_ELIXIR_CHEST_PILLAR_ON_BRIDGE))
			fdActorSetPosFX(
				ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(520), toFIXED(296));
		if(syGameFlagGet(FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED)) {
			fdActorSetPosFX(
				ACTOR_ELIXIR_CHEST_PILLAR, toFIXED(312), toFIXED(296));
			fdActorSetSprPriority(ACTOR_ELIXIR_CHEST_PILLAR, 2);
			fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->motionFlg = ACTOR_FALLS;
			fdActorGetById(ACTOR_ELIXIR_CHEST_PILLAR)->groundHeight =
				-toFIXED(32);
			fdMapGridCopyMetatileAttributes(19, 16, 1, 1, 19, 18);
		}
	} else if(MainRoutineStateWork.next ==
		SCENE_AIRS_ROCK_INSIDE_PAIRED_STAKES) {
		scAirsRockInsideStartRestoringMetatiles();
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_A)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_B)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_C)->sceneCnt = 1;
		fdActorGetById(ACTOR_PAIRED_STAKES_STAKE_D)->sceneCnt = 1;
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			syGameFlagSet(FLAG_STAKE_B_DOWN);
			syGameFlagSet(FLAG_STAKE_C_DOWN);
		}
		scAirsRockInsidePlaceStakes(AirsRockInsidePairedStakesStakes);
		if(syGameFlagGet(FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_PAIRED_STAKES_PILLAR, toFIXED(344), toFIXED(696));
		fdActorPlaceBlocks(AirsRockInsidePairedStakesPillars);
		if(syGameFlagGet(FLAG_PAIRED_STAKES_NORTH_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyMetatilesTo(39, 82, 38, 82, 1, 3);
			fdMapGridCopyMetatileAttributes(39, 82, 1, 1, 38, 82);
			fdMapGridCopyMetatileAttributes(38, 19, 1, 1, 38, 18);
		}
		if(syGameFlagGet(FLAG_PAIRED_STAKES_SOUTH_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyMetatilesTo(39, 82, 38, 99, 1, 3);
			fdMapGridCopyMetatileAttributes(38, 98, 1, 1, 38, 99);
			fdMapGridCopyMetatileAttributes(38, 35, 1, 1, 38, 37);
		}
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_FACE_VENTS) {
		fdQueueMusic(SE_AMBIENT_STREAM);
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		sySetEvent(scAirsRockInsidePuffVents, EVENT_PASS_TICK);
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_FACE_VENTS_FIRST_PILLAR, toFIXED(280), toFIXED(472));
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_FACE_VENTS_SECOND_PILLAR, toFIXED(360), toFIXED(840));
		if(syGameFlagGet(FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED)) {
			fdActorSetPosFX(
				ACTOR_FACE_VENTS_THIRD_PILLAR, toFIXED(184), toFIXED(616));
			syGameFlagSet(FLAG_SOUTHEAST_VENT_STILL);
		}
		fdActorSetSprPriority(ACTOR_FACE_VENTS_SECOND_PILLAR, 2);
		fdActorPlaceBlocks(AirsRockInsideFaceVentsPillars);
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->x.fixed),
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_FIRST_PILLAR)->z.fixed),
			0);
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->x.fixed),
			FIELD_METATILE(
				fdActorGetById(ACTOR_FACE_VENTS_SECOND_PILLAR)->z.fixed),
			0);
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
			fdActorSetSprCol(
				fdActorGetById(ACTOR_FACE_VENTS_STATUE), SPRITE_COLOR_GREY);
		else
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_FACE_VENTS_STATUE), 8, 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_FACE_VENTS_STATUE), 0);
	} else if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_COINS_CHEST) {
		scAirsRockInsideStartRestoringMetatiles();
		if(syGameFlagGet(FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED))
			fdActorSetPosFX(
				ACTOR_COINS_CHEST_PILLAR, toFIXED(360), toFIXED(264));
		fdActorPlaceBlocks(AirsRockInsideCoinsChestPillars);
		scMapMetatileSetAttributes(MAPGRID_LAYER_OVERHEAD,
			FIELD_METATILE(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->x.fixed),
			FIELD_METATILE(fdActorGetById(ACTOR_COINS_CHEST_STATUE)->z.fixed),
			0);
		fdActorGetById(ACTOR_COINS_CHEST_STATUE)->steeringFlg = 0;
		if(!syGameFlagGet(FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN))
			fdActorSetSprCol(
				fdActorGetById(ACTOR_COINS_CHEST_STATUE), SPRITE_COLOR_GREY);
		else
			scAirsRockInsideWakeStatue(
				fdActorGetById(ACTOR_COINS_CHEST_STATUE), 60, 0);
		fdActorSetSprFlags(fdActorGetById(ACTOR_COINS_CHEST_STATUE), 0);
		if(syGameFlagGet(FLAG_COINS_CHEST_BARRIER_BLOWN_AWAY)) {
			fdMapGridCopyMetatilesTo(15, 80, 15, 77, 2, 3);
			fdMapGridCopyMetatileAttributes(15, 80, 1, 1, 15, 79);
			fdMapGridCopyMetatileAttributes(17, 15, 2, 1, 15, 15);
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_AIRS_ROCK_INSIDE_HALL)
		scAirsRockInsideSetLedgeMetatilesByLevel();
	return 0;
}

void scAirsRockInsideStartWhirlwind(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectLongScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The blocks Move pushes in each room (fdActorPlaceBlocks), ended by -1: the */
/* hall's statue. */
Sint16 AirsRockInsideHallPillars[] = {
	ACTOR_HALL_MOVABLE_STATUE, -1,
};

/* The Vial chest room's two pillars. */
Sint16 AirsRockInsideVialChestPillars[] = {
	ACTOR_VIAL_CHEST_EAST_PILLAR, ACTOR_VIAL_CHEST_WEST_PILLAR, -1,
};

/* The Clarity Circlet chest room's three pillars and its statue. */
Sint16 AirsRockInsideClarityCircletChestPillars[] = {
	ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR,
	ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B,
	-1,
};

/* The tightropes room's pillar. */
Sint16 AirsRockInsideTightropesPillars[] = {
	ACTOR_TIGHTROPES_PILLAR, -1,
};

/* The paired stakes room's four pillars. */
Sint16 AirsRockInsidePairedStakesPillars[] = {
	ACTOR_PAIRED_STAKES_PILLAR,   ACTOR_PAIRED_STAKES_PILLAR_A,
	ACTOR_PAIRED_STAKES_PILLAR_B, ACTOR_PAIRED_STAKES_PILLAR_C,
	-1,
};

/* The face vents room's five pillars. */
Sint16 AirsRockInsideFaceVentsPillars[] = {
	ACTOR_FACE_VENTS_THIRD_PILLAR, ACTOR_FACE_VENTS_FIRST_PILLAR,
	ACTOR_FACE_VENTS_SECOND_PILLAR, ACTOR_FACE_VENTS_PILLAR_A,
	ACTOR_FACE_VENTS_PILLAR_B, -1,
};

/* The coins chest room's pillar and statue. */
Sint16 AirsRockInsideCoinsChestPillars[] = {
	ACTOR_COINS_CHEST_PILLAR, ACTOR_COINS_CHEST_STATUE, -1,
};

Uint32 OrbitSparkScript[] = {
	SC_WAIT(6),
	SC_RELEASE,
	SC_END,
};

Uint32 AirsRockInsideRisingSparkScript[] = {
	SC_MOVE_BY(0, 160, 0),
	SC_SYNC,
	SC_RELEASE,
	SC_END,
};

Sint32 AirsRockInsideOrbitingSparkScript[] = {
	SC_CALL(scAirsRockInsideOrbitAnchor),
	SC_RELEASE,
	SC_END,
};

#include "shared/lesson_seeking_spark_script.c"

#include "shared/lesson_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Uint32 AirsRockInsideWhirlwindRisesScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
	SC_END,
};

Uint32 AirsRockInsideWhirlwindFadesScript[] = {
	SC_WAIT(60),
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
	SC_END,
};

Uint32 AirsRockInsidePooledWhirlwindFadesScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

Uint32 AirsRockInsideWindStoneScript[] = {
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_SET(SW_STEERING, 0),
	SC_ANIMATION(2),
	SC_END,
};

/* The paired stakes room's four stakes, as scAirsRockInsidePlaceStakes reads */
/* them: each stake, its first pose and the flag set while it is sunk. */
Uint16 AirsRockInsidePairedStakesStakes[] = {
	ACTOR_PAIRED_STAKES_STAKE_A, 1, FLAG_STAKE_A_DOWN,
	ACTOR_PAIRED_STAKES_STAKE_B, 1, FLAG_STAKE_B_DOWN,
	ACTOR_PAIRED_STAKES_STAKE_C, 1, FLAG_STAKE_C_DOWN,
	ACTOR_PAIRED_STAKES_STAKE_D, 1, FLAG_STAKE_D_DOWN,
	ACTOR_LIST_END, 0,
};

SCENE_ENTRANCE AirsRockInsideEntrances[] = {
	{0, CONDITION_ALWAYS, 320, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE AirsRockInsideHallEntrances[] = {
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

Uint32 SceneExits[] = {
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

ACTOR_PLACE AirsRockInsidePlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideHallPlacements[] = {
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

ACTOR_PLACE AirsRockInsideVialChestPlacements[] = {
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideClarityCircletChestPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideTightropesPlacements[] = {
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
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

ACTOR_PLACE AirsRockInsideRevealTabletPlacements[] = {
	{SPRITE_SLATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideElixirChestPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsidePairedStakesPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideFaceVentsPlacements[] = {
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

ACTOR_PLACE AirsRockInsideCoinsChestPlacements[] = {
	{SPRITE_WIND_STONE, CONDITION_ALWAYS, (Sint32)AirsRockInsideWindStoneScript, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SCENE_TABLE_END},
};

Sint32 AirsRockInsideStampedMetatile = -1;

CUE AirsRockInsideHallCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, 14},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideOpenTabletRoomDoor)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_TOWER_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampOtherMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampOtherMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampTallMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STACKED_STONES_NEARER, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampTallMetatileUnderLeaderNearer)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_HALL_TWISTER_ROCK, FLAG_AIRS_ROCK_HALL_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideWakeHallStatue)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HALL_MOVABLE_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideWhirlwind)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideVialChestCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_VIAL_CHEST_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideJumpDownLedge)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampOtherMetatileUnderLeader)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_EAST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_EAST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIAL_CHEST_WEST_PILLAR, FLAG_AIRS_ROCK_VIAL_CHEST_WEST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_VIAL_CHEST_TWISTER_ROCK, FLAG_AIRS_ROCK_VIAL_CHEST_STATUE_WOKEN, SCRIPT_REF(scAirsRockInsideWakeVialChestStatue)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideWhirlwind)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideClarityCircletChestCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_FIRST_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_FIRST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_SECOND_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_SECOND_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_THIRD_PILLAR, FLAG_AIRS_ROCK_CLARITY_CIRCLET_CHEST_THIRD_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CLARITY_CIRCLET_CHEST_STATUE_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideWhirlwind)},
	{CUE_RAISED, CUE_PUFF_REACHES_LEADER, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideClarityCircletVent)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideTightropesCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, FLAG_AIRS_ROCK_TIGHTROPES_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_TIGHTROPES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideRevealTabletCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampOtherMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_LIGHTING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampWideMetatileUnderLeader)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideIgnoreStep)},
	{EXAMINE_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideReadRevealTablet)},
	{EXAMINE_CUE_FACING(FACING_SOUTH), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideWalkToRevealTablet)},
	{EXAMINE_CUE_FACING(FACING_WEST), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideReadTabletFacingWest)},
	{EXAMINE_CUE_FACING(FACING_EAST), METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_REVEAL_TABLET, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideReadTabletFacingEast)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRevealDoors)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideHideRevealedDoors)},
	{CUE_RAISED, CUE_FIRST_VANISHING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOffVanishingStone)},
	{CUE_RAISED, CUE_FIRST_VANISHING_STONE + 1, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOffVanishingStone)},
	{CUE_RAISED, CUE_FIRST_VANISHING_STONE + 2, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOffVanishingStone)},
	{CUE_RAISED, CUE_FIRST_VANISHING_STONE + 3, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOffVanishingStone)},
	{CUE_RAISED, CUE_FIRST_VANISHING_STONE + 4, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOffVanishingStone)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideElixirChestCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_OTHER_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampOtherMetatileUnderLeader)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELIXIR_CHEST_PILLAR, FLAG_AIRS_ROCK_ELIXIR_CHEST_PILLAR_MOVED, SCRIPT_REF(scAirsRockInsideMoveElixirChestPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_ELIXIR_CHEST_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRockInsidePairedStakesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STAKE_TOP, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStepOntoStake)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR, FLAG_AIRS_ROCK_PAIRED_STAKES_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PAIRED_STAKES_PILLAR_C, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_A, FLAG_STAKE_A_DOWN, SCRIPT_REF(scAirsRockInsidePoundPairedStakesStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_B, FLAG_STAKE_B_DOWN, SCRIPT_REF(scAirsRockInsidePoundPairedStakesStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_C, FLAG_STAKE_C_DOWN, SCRIPT_REF(scAirsRockInsidePoundPairedStakesStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_PAIRED_STAKES_STAKE_D, FLAG_STAKE_D_DOWN, SCRIPT_REF(scAirsRockInsidePoundPairedStakesStake)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_NORTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_PAIRED_STAKES_SOUTH_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideFaceVentsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_RAISED, CUE_PUFF_REACHES_LEADER, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideFaceVent)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideWhirlwind)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_THIRD_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_THIRD_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_FIRST_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_FIRST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_SECOND_PILLAR, FLAG_AIRS_ROCK_FACE_VENTS_SECOND_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_A, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_VENTS_PILLAR_B, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_VENTS_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRockInsideCoinsChestCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
	{CUE_STEP, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_STEPPING_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStampMetatileUnderLeader)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideRideWhirlwind)},
	{CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideStartPushingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_COINS_CHEST_PILLAR, FLAG_AIRS_ROCK_COINS_CHEST_PILLAR_PLACED, SCRIPT_REF(scAirsRockInsideFinishPushingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideStartWhirlwind)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideTakeOverWhirlwind)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_COINS_CHEST_WIND_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideBlowGust)},
	{CUE_TABLE_END},
};

CUE AirsRockInsidePsynergyStoneCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXAMINE, METATILE_BEHAVIOR_AIRS_ROCK_INSIDE_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scAirsRockInsideCheckPsynergyStone)},
	{CUE_TABLE_END},
};

/* The great hall's level the leader stood at when its ledge metatiles were */
/* last set; -1 before. */
Sint32 AirsRockInsideLeaderLevel = -1;
#undef fdActorSetHorizontalPosWithTerrain
