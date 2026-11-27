/* The rooms west and south of Jupiter Lighthouse's central room. In the north
   west, the first puzzle up the left path: four fallen columns lie on a raised
   floor, and the Erinyes Tunic's chest stands across a mossy trench to the
   east. Felix pushes the columns about until the middle one of the lower three
   can be pushed east off the floor's edge; it drops into the trench at
   metatiles (16, 11) to (16, 14) (scJupiterLighthousePushColumnOffTheLedge),
   and he hops over it to the chest and back (scJupiterLighthouseJumpForward).
   In the south hall, Cyclone on the purple teleporter pad at metatile (9, 51)
   takes him one level up (scJupiterLighthouseLeaveByCyclone). Later, when he
   comes back with the Blue Key, a stone pillar stands on each of the hall's two
   high ledges beside a hanging chain; Moved one metatile towards its chain,
   each drops into the gap below the ledge and makes a shortcut between the
   tower's two sides (scJupiterLighthouseFinishMovingPillar). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "scene/script_layer.h"
#include "system/field_effect.h"
#include "flags.h"
#include "shared/tables.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "scene/scene.h"
#include "field/metatile_target.h"
#include "macros.h"
#include "field/actor_script.h"

/* Where a pushed object comes to rest, read by the push that follows. */
typedef struct PUSH_TARGET {
	FIXED  x; /* 16.16 fixed-point pixels */
	Sint32 y;
	Sint32 z;
	Sint32 rectangle; /* index into the rectangle table */
	Sint32 id;
	void  *callback;
} PUSH_TARGET;

/* Set once the column has been pushed off the ledge this visit. */
#define FLAG_COLUMN_OFF_LEDGE	 FLAG_SCENE_TEMP_00
/* Set as Move drops the west or east pillar into its gap. */
#define FLAG_LEFT_PILLAR_IN_GAP	 0xa04
#define FLAG_RIGHT_PILLAR_IN_GAP 0xa05

/* The four fallen columns, the third the one Felix pushes off into the trench,
   and the two pillars Move drops into the gaps in the south hall. */
enum {
	ACTOR_FIRST_COLUMN = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_COLUMN,
	ACTOR_LEDGE_COLUMN,
	ACTOR_FOURTH_COLUMN,
	/* The two chains that hang beside the pillars' ledges
	   (fdHangingVinePlace). */
	ACTOR_WEST_CHAIN,
	ACTOR_EAST_CHAIN,
	ACTOR_LEFT_PILLAR = ACTOR_FIRST_PLACED + 6,
	ACTOR_RIGHT_PILLAR
};

/* The metatile at the raised floor's east edge where the pushed column starts
   to fall, and the metatile of the trench it comes to rest in. */
enum {
	LEDGE_EDGE_METATILE_X = 14,
	TRENCH_METATILE_X = 16,
	LEDGE_COLUMN_METATILE_Z = 12
};

/* The gaps below the south hall's west and east ledges that the pillars drop
   into. */
enum { WEST_PILLAR_GAP_X = 20, EAST_PILLAR_GAP_X = 32, PILLAR_GAP_Z = 36 };

/* The entrances at (152, 840) and (696, 840), where Felix arrives in a ring
   (fdActorDepartInRing). */
enum { ENTRANCE_WEST_RING = 12, ENTRANCE_EAST_RING = 13 };

/* The rooms' own metatile behaviors. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_METATILE_BEHAVIOR {
	/* The raised floor round the fallen columns, metatiles (5, 5) to (14, 14),
	   where walking into a column pushes it. */
	METATILE_BEHAVIOR_BESIDE_COLUMNS = 30,
	/* The trench's east bank by the chest, metatiles (18, 11) and (18, 12),
	   where walking west jumps back over the trench. */
	METATILE_BEHAVIOR_TRENCH_EAST_BANK,
	/* The purple teleporter pad in the south hall, metatile (9, 51). */
	METATILE_BEHAVIOR_CYCLONE_TELEPORTER = 40
} JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_METATILE_BEHAVIOR;

void scSetArrivalFlags(Sint32 a);
Bool scPushTargetFillRectangle(Sint32 id);
Bool scPushTargetFindStop(PUSH_TARGET *target);
void scPushTargetPush(PUSH_TARGET target);

void				   scJupiterLighthouseStartMovingPillar(void);
void				   scJupiterLighthouseLeaveByCyclone(void);
void				   scJupiterLighthouseJumpForward(void);
Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16					   PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16					   PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile	   BSS_DATA;
Sint16 LightSpriteCount	   BSS_DATA;
ACTOR					  *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM				   LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge   BSS_DATA;
Sint16 LightSpritePriority BSS_DATA;
/* The metatiles scCrackedTileStep watches, and the metatile it copies over the
   one the leader steps on. */
METATILE_TARGET CrackedTiles BSS_DATA;
ACTOR *scPushTargetFindAhead(Sint32 *direction, Sint32 *slot, Sint32 *kind);

extern Uint16 PushKinds[PUSH_KINDS];
void		  fdPaletteCycleStep(void);

extern Sint32 PaletteCycleTick;

/* Sets the palette cycle's tick (PaletteCycleTick). */
static inline void scPaletteCycleSetTick(Sint32 value)
{
	PaletteCycleTick = value;
}

extern Sint32 PaletteCyclePausesInScripts;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
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
	return ActorPlacements;
}

/* The pushed column's update while Felix pushes it: once it reaches metatile
   (14, 12), at the raised floor's east edge, it is drawn a step further back
   (`fdActorSetSpritePriorityDirect`) for its fall into the trench. */
void scJupiterLighthouseDrawColumnBelowLedge(ACTOR *actor)
{
	if(FIELD_METATILE(actor->x.fixed) == LEDGE_EDGE_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == LEDGE_COLUMN_METATILE_Z)
		fdActorSetSpritePriorityDirect(actor, 2);
}

/* Felix walks into one of the fallen columns on the raised floor and pushes it;
   it slides until something stops it. The middle one of the lower three, pushed
   east to the floor's edge, falls into the mossy trench with a heavy thud and
   lies across it at metatiles (16, 11) to (16, 14), so that Felix can hop over
   it to the Erinyes Tunic's chest. */
void scJupiterLighthousePushColumnOffTheLedge(void)
{
	ACTOR	   *actor = fdActorGet(ACTOR_LEDGE_COLUMN);
	PUSH_TARGET target;
	Sint32		i;
	FIXED		x;

	actor->update = (OBJECT_UPDATE)scJupiterLighthouseDrawColumnBelowLedge;
	if(scPushTargetFindStop(&target))
		scPushTargetPush(target);
	if(FIELD_METATILE(actor->x.fixed) == TRENCH_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == LEDGE_COLUMN_METATILE_Z &&
		fdScriptGetFlag(FLAG_COLUMN_OFF_LEDGE) == 0) {
		fdScriptSetFlag(FLAG_COLUMN_OFF_LEDGE);
		scFieldActorSetSpritePriority(ACTOR_LEDGE_COLUMN, 3);
		actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		syWait(5);
		fdScriptPlaySound(SE_HEAVY_IMPACT);
		actor->motionFlags = ACTOR_FALLS;
		for(i = 0, x = toFIXED(176); i <= 3; i++, x += toFIXED(16)) {
			syFieldMapGridSetMetatileHeight(toFIXED(256), x, 0, 0);
			fdMapGridSetMetatileBehavior(toFIXED(256), x, 0, 0);
			if(i <= 1)
				fdMapGridSetMetatileAttributesAt(
					toFIXED(256), x, 0, MAP_METATILE_JUMP);
		}
	}
	actor->update = 0;
}

/* Nothing visible happens as Move takes hold of a pillar on a ledge in the
   south hall; these rooms need no metatiles changed before the push. */
void scJupiterLighthouseStartMovingPillar(void)
{
}

/* The stone pillar on the south hall's west or east ledge, Moved one metatile
   towards the hanging chain beside it, drops off the ledge into the gap below
   (metatile (20, 36) in the west, (32, 36) in the east) with a heavy thud and
   stays there, its top level with the floor, a shortcut across. */
void scJupiterLighthouseFinishMovingPillar(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);
	Sint32 placed = 0;
	Sint32 i;
	if(FIELD_METATILE(actor->x.fixed) == WEST_PILLAR_GAP_X &&
		FIELD_METATILE(actor->z.fixed) == PILLAR_GAP_Z) {
		fdScriptSetFlag(FLAG_LEFT_PILLAR_IN_GAP);
		placed = 1;
	}
	if(FIELD_METATILE(actor->x.fixed) == EAST_PILLAR_GAP_X &&
		FIELD_METATILE(actor->z.fixed) == PILLAR_GAP_Z) {
		fdScriptSetFlag(FLAG_RIGHT_PILLAR_IN_GAP);
		placed = 1;
	}
	if(placed) {
		syWait(10);
		fdScriptPlaySound(SE_HEAVY_IMPACT);
		for(i = 0; i < 30; i++) {
			if(actor->y.fixed <= actor->groundHeight && actor->velocityY == 0)
				break;
			syWait(1);
		}
		actor->motionFlags = 0;
		fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, 0);
	}
}

/* Felix casts Cyclone on the purple teleporter pad in the south hall, metatile
   (9, 51): the whirlwind lifts the party one level up, into the room of the
   locked doors, where they arrive in a ring. */
void scJupiterLighthouseLeaveByCyclone(void)
{
	syImportScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets up the rooms as the party enters: the two chains, and the ring the party
   arrives in by a teleporter. Returns 0. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdHangingVinePlace(ACTOR_WEST_CHAIN, 1);
	fdHangingVinePlace(ACTOR_EAST_CHAIN, 1);
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
		MainRoutineStateWork.entrance == ENTRANCE_WEST_RING)
		fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
		MainRoutineStateWork.entrance == ENTRANCE_EAST_RING)
		fdActorDepartInRing(MainRoutineStateWork.leaderActorId);
	return 0;
}

/* Sets the scene's arrival flags; puts the pillars back in their gaps, makes
   the four columns block their metatiles, and lays the pushed column across the
   trench again once it has fallen in. Returns 0. */
Sint32 scPrepareMap(void)
{
	ACTOR *actor;
	Sint32 i;
	FIXED  x;

	scSetArrivalFlags(0);
	if(fdScriptGetFlag(FLAG_LEFT_PILLAR_IN_GAP)) {
		actor = fdActorGet(ACTOR_LEFT_PILLAR);
		actor->motionFlags = 0;
		actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
		actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
		fdActorSetPos(ACTOR_LEFT_PILLAR, 328, 584);
		fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, 0);
	}
	if(fdScriptGetFlag(FLAG_RIGHT_PILLAR_IN_GAP)) {
		actor = fdActorGet(ACTOR_RIGHT_PILLAR);
		actor->motionFlags = 0;
		actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
		actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
		fdActorSetPos(ACTOR_RIGHT_PILLAR, 520, 584);
		fdMapGridSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, 0);
	}
	fdActorGet(ACTOR_FIRST_COLUMN)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_SECOND_COLUMN)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_LEDGE_COLUMN)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_FOURTH_COLUMN)->priorityFlags |=
		ACTOR_PRIORITY_IGNORES_RISES;
	scFieldActorSetSpritePriority(ACTOR_FIRST_COLUMN, 3);
	scFieldActorSetSpritePriority(ACTOR_LEDGE_COLUMN, 3);
	scFieldActorSetSpritePriority(ACTOR_SECOND_COLUMN, 3);
	scFieldActorSetSpritePriority(ACTOR_FOURTH_COLUMN, 3);
	fdActorGet(ACTOR_LEDGE_COLUMN)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorDisableById(ACTOR_FIRST_COLUMN);
	fdActorDisableById(ACTOR_SECOND_COLUMN);
	fdActorDisableById(ACTOR_LEDGE_COLUMN);
	fdActorDisableById(ACTOR_FOURTH_COLUMN);
	scPushTargetFillRectangle(ACTOR_FIRST_COLUMN);
	scPushTargetFillRectangle(ACTOR_SECOND_COLUMN);
	scPushTargetFillRectangle(ACTOR_LEDGE_COLUMN);
	scPushTargetFillRectangle(ACTOR_FOURTH_COLUMN);
	if(fdScriptGetFlag(FLAG_COLUMN_OFF_LEDGE)) {
		for(i = 0, x = toFIXED(176); i <= 3; i++, x += toFIXED(16)) {
			syFieldMapGridSetMetatileHeight(toFIXED(256), x, 0, 0);
			fdMapGridSetMetatileBehavior(toFIXED(256), x, 0, 0);
			if(i <= 1)
				fdMapGridSetMetatileAttributesAt(
					toFIXED(256), x, 0, MAP_METATILE_JUMP);
		}
	}
	return 0;
}

/* Walking west off the trench's east bank by the Erinyes Tunic's chest,
   metatile (18, 11) or (18, 12), Felix jumps back over the trench, once the
   column lies in it to land on. */
void scJupiterLighthouseJumpForward(void)
{
	fdTryJumpForward();
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_scripts.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_metatile.h"

#include "shared/whirlwind_carry_leader_lower.h"

/* The distance between two positions across the ground (x and z), in whole
   pixels. */
Sint32 scPosFlatDistance(FIXED *a, FIXED *b)
{
	Sint32 dx = FIXEDtoINT(a[0] - b[0]);
	Sint32 dz = FIXEDtoINT(a[2] - b[2]);
	Sint32 dxsq = dx * dx;
	Sint32 dzsq = dz * dz;

	return sySquartIWRAM(dxsq + dzsq);
}

extern Sint32 WhirlwindEndScript[];

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#include "shared/whirlwind_update_lower.h"

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern Sint8 PushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_metatile_fill.c"

extern Sint32 PushLeaderSteps[];

static inline Sint32 scPushTargetCheckCollision(ACTOR *actor, Sint32 *pos)
{
	return fdMapGridCheckCollision(actor, pos);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

void scActorStopAtPushTarget(ACTOR *actor);

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSpritesSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

/* clang-format off */
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"
/* clang-format on */

#include "../field/field_effect_move.c"

#undef FLAG_COLUMN_OFF_LEDGE
#undef FLAG_LEFT_PILLAR_IN_GAP
#undef FLAG_RIGHT_PILLAR_IN_GAP

void fdCrackedTileUpdate(void);

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

void syGameFlagSetByte(Sint32 flag, Sint32 value);

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack();
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Uint32 FieldEffectLongScript[];

/* clang-format off */
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseRoomErinyesTunicChest),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_NORTH, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHWEST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHEAST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
	SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 680, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 680, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

LATE_DATA CUE SceneCues[] = {
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_NORTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_NORTH},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHWEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_TERRACES_SOUTHEAST},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_BESIDE_COLUMNS, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePushColumnOffTheLedge)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_TRENCH_EAST_BANK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseJumpForward)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEST_CHAIN, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_EAST_CHAIN, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartMovingPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseFinishMovingPillar)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_CYCLONE_TELEPORTER, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLeaveByCyclone)},
	{CUE_TABLE_END},
};

LATE_DATA Sint32 PaletteCycleTick = -1;

LATE_DATA Sint32 PaletteCyclePausesInScripts = 1;

LATE_DATA Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
/* clang-format on */

LATE_DATA const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};
