// The upper rooms of Jupiter Lighthouse's right tower, the last before its top. First a hall
// floored with cracked tiles, where two stone faces in the walls blow whirlwinds across the floor
// at Felix as he passes (scJupiterLighthouseRaiseWhirlwind), and a cracked tile gives way under a
// second step, dropping the party to the room below (scJupiterLighthouseStepOnUpperCrackedTile).
// Then, south and up, the hall of Hover pads, where Felix can Hover almost anywhere: two lines of
// dark cracked tiles run up it, and four stone statues in its middle blow whirlwinds along the two
// rows in front of them at Felix as he floats past (scJupiterLighthouseRaiseUpperWhirlwind). Its
// west passage leads to the Phaeton's Blade's chest, its east one on towards the top; the stairs
// down between the statues are walled off while he Hovers (scJupiterLighthouseStartHovering).

// This unit declares its own form of fdMapGridGetMetatileBehavior, below.
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/metatile_target.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

// Where a stone face's whirlwind starts: an unseen actor moved to the face that blows, which the
// whirlwind takes its place and heading from.
enum
{
	ACTOR_WHIRLWIND_SOURCE = ACTOR_FIRST_PLACED
};

// The step cues before the stone faces that blow whirlwinds: two rows of the cracked hall's floor,
// (8, 37) to (17, 37) blown west and (9, 42) to (18, 42) blown east, and four short rows before the
// statues in the Hover hall, at rows 11 and 17, the west pair blown west and the east pair east.
enum
{
	CUE_FLOOR_WHIRLWIND_WEST      = 30,
	CUE_FLOOR_WHIRLWIND_EAST      = 31,
	CUE_UPPER_WHIRLWIND_SOUTHWEST = 50,
	CUE_UPPER_WHIRLWIND_NORTHWEST,
	CUE_UPPER_WHIRLWIND_SOUTHEAST,
	CUE_UPPER_WHIRLWIND_NORTHEAST
};

// A cracked tile's metatile behavior in the cracked hall, which takes the next one up as it cracks;
// the Hover hall's are METATILE_BEHAVIOR_CRACKED_TILE.
enum
{
	METATILE_BEHAVIOR_UPPER_CRACKED_TILE = 43
};

// Through a cracked tile of the cracked hall to the room below, on the same map: the exit and the
// entrance it lands at share the number, as THROUGH_CRACKED_TILE's do.
enum
{
	THROUGH_UPPER_CRACKED_TILE = 14
};

// The entrances scInit tells apart: the cracked hall's two stairs, north and south, and the Hover
// hall's two.
enum
{
	ENTRANCE_LOWER_HALL_NORTH = 3,
	ENTRANCE_LOWER_HALL_SOUTH = 5,
	ENTRANCE_HOVER_HALL_SOUTH = 6,
	ENTRANCE_HOVER_HALL_NORTH = 7
};

// The Hover hall's stairs down, metatile (34, 17), whose metatile takes the exit by its number.
enum
{
	EXIT_HOVER_HALL_STAIRS = 6
};

struct ACTOR;
struct LIGHT_OAM;
void                         scCrackedTilePlaceLanding(Sint32 x, Sint32 z, Sint32 effect);
// The state of the code every Jupiter Lighthouse room's overlay keeps its own copy of: a
// whirlwind's ride (shared/whirlwind_*.h), the palette cycle (shared/palette_cycle_step.h), the lit
// actors' sprites (shared/light_sprites_*.h) and the cracked tiles (shared/cracked_tile_*.h).
Sint32 WhirlwindRiding       BSS_DATA;
Uint16                       PaletteCycleColors[16] BSS_DATA;
// Palette 13's first two colours; it is copied whole, with the unlit colours after them.
Uint16                       PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16                       PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile       BSS_DATA;
Sint16 LightSpriteCount      BSS_DATA;
ACTOR                       *LightSpriteActors[16] BSS_DATA;
LIGHT_OAM                    LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge     BSS_DATA;
Sint16 LightSpritePriority   BSS_DATA;
// The metatiles scCrackedTileStep watches, and the metatile it copies over the one the leader steps
// on.
METATILE_TARGET CrackedTiles BSS_DATA;
void                         scJupiterLighthouseStopHovering(void);

const SCENE_ENTRANCE        *scGetEntrances(void)
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

// The step handlers below pass on the metatile behavior their cue hands them; scCrackedTileStep
// finds the leader's metatile itself and never reads it.
Bool   scCrackedTileStep();
ACTOR *scWhirlwindSpawn(ACTOR *src);

// Nothing visible happens when Felix starts to Hover; while he floats, the Hover hall's stairs down
// between the statues, metatile (34, 17), are walled off, so that he cannot drift down them.
void   scJupiterLighthouseStartHovering(void)
{
	if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_HOVER)
		syFieldMapGridSetMetatileBehavior(
			toFIXED(544), toFIXED(272), MAPGRID_LAYER_GROUND, METATILE_BEHAVIOR_WALL);
}

// Nothing visible happens when Felix's Hover ends; the Hover hall's stairs down, metatile (34, 17),
// lead down again.
void scJupiterLighthouseStopHovering(void)
{
	fdMapGridSetMetatileBehavior(
		toFIXED(544), toFIXED(272), MAPGRID_LAYER_GROUND, EXIT_HOVER_HALL_STAIRS);
}

// Felix walks across the row in front of a stone face in the cracked hall's wall, metatiles (8, 37)
// to (17, 37) or (9, 42) to (18, 42): the face blows a big whirlwind at him that catches him and
// carries him spinning across the hall, west along the first row or east along the second, and
// drops him (fdWhirlwindCarryLeader). Nothing happens while a whirlwind is already carrying him.
void scJupiterLighthouseRaiseWhirlwind(Sint32 id)
{
	ACTOR *actor;

	if (WhirlwindRiding == 0)
	{
		actor = fdActorGet(ACTOR_WHIRLWIND_SOURCE);
		if (id == CUE_FLOOR_WHIRLWIND_WEST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 296, 600);
			actor->y.fixed = 0;
			actor->facing  = FACING_WEST;
		}
		else if (id == CUE_FLOOR_WHIRLWIND_EAST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 136, 680);
			actor->y.fixed = 0;
			actor->facing  = FACING_EAST;
		}
		scWhirlwindSpawn(actor);
		WhirlwindRiding = 1;
	}
}

// Felix, Hovering in the Hover hall, floats across the row in front of one of the four stone
// statues in its middle, at row 11 or 17: the statue blows a slower whirlwind at him, west from the
// west pair or east from the east pair, over the line of dark cracked tiles, and it carries him off
// (fdWhirlwindCarryLeader). Walking there without Hover, or while a whirlwind is already carrying
// him, does nothing.
void scJupiterLighthouseRaiseUpperWhirlwind(Sint32 id)
{
	ACTOR *actor;
	ACTOR *motion;

	if (WhirlwindRiding == 0 && fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed > 0)
	{
		actor = fdActorGet(ACTOR_WHIRLWIND_SOURCE);
		if (id == CUE_UPPER_WHIRLWIND_SOUTHWEST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 488, 280);
			actor->y.fixed = toFIXED(32);
			actor->facing  = FACING_WEST;
		}
		else if (id == CUE_UPPER_WHIRLWIND_NORTHWEST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 472, 184);
			actor->y.fixed = toFIXED(32);
			actor->facing  = FACING_WEST;
		}
		else if (id == CUE_UPPER_WHIRLWIND_SOUTHEAST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 616, 280);
			actor->y.fixed = toFIXED(32);
			actor->facing  = FACING_EAST;
		}
		else if (id == CUE_UPPER_WHIRLWIND_NORTHEAST)
		{
			fdActorSetPos(ACTOR_WHIRLWIND_SOURCE, 632, 184);
			actor->y.fixed = toFIXED(32);
			actor->facing  = FACING_EAST;
		}
		motion = scWhirlwindSpawn(actor);
		if (motion != NULL)
			motion->speedLimit = toFIXED(1);
		WhirlwindRiding = 1;
	}
}

// Felix steps on a cracked tile in the Hover hall with his feet on the floor: it gives way under
// him (a tile not yet cracked takes one step first) and the party falls to the cracked hall below.
void scJupiterLighthouseStepOnLowerCrackedTile(Sint32 metatileBehavior)
{
	if (fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0
	    && scCrackedTileStep(metatileBehavior) != 0)
		syFieldScriptSetExitRequest(THROUGH_CRACKED_TILE);
}

// Felix steps on a cracked tile in the cracked hall: it cracks, and at the second step it gives way
// and the party falls to the room below. Two of the tiles lie just north of the great white
// wind-face statues carved into the hall's walls, metatile (18, 34) against the east wall and
// metatile (8, 39) against the west; on those he is drawn behind the statue's head
// (`scFieldActorSetSpritePriority`) as he sinks through the hole.
void scJupiterLighthouseStepOnUpperCrackedTile(Sint32 metatileBehavior)
{
	ACTOR *leader;
	Sint32 x;
	Sint32 z;

	if (fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed == 0)
	{
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		x      = FIELD_METATILE(leader->x.fixed);
		z      = FIELD_METATILE(leader->z.fixed);
		if (x == 18 && z == 34)
			scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
		if (x == 8 && z == 39)
			scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 3);
		if (scCrackedTileStep(metatileBehavior) != 0)
			syFieldScriptSetExitRequest(THROUGH_UPPER_CRACKED_TILE);
	}
}

// Felix steps on one of the dark cracked tiles in the two lines running up the Hover hall, columns
// 25 and 43. With his feet on the floor it gives way at once and the party falls to the cracked
// hall below. Hovering over it at row 11 or 17, in front of a statue, he is blown away as on the
// rows before the statues (scJupiterLighthouseRaiseUpperWhirlwind).
void scJupiterLighthouseStepOnCrackedTileOrWhirlwind(Sint32 metatileBehavior)
{
	ACTOR *leader;
	Sint32 x;
	Sint32 z;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if (leader->y.fixed <= 0)
	{
		scJupiterLighthouseStepOnLowerCrackedTile(metatileBehavior);
	}
	else
	{
		x = FIELD_METATILE(leader->x.fixed);
		z = FIELD_METATILE(leader->z.fixed);
		if (x == 25)
		{
			if (z == 17)
				scJupiterLighthouseRaiseUpperWhirlwind(CUE_UPPER_WHIRLWIND_SOUTHWEST);
			if (z == 11)
				scJupiterLighthouseRaiseUpperWhirlwind(CUE_UPPER_WHIRLWIND_NORTHWEST);
		}
		if (x == 43)
		{
			if (z == 17)
				scJupiterLighthouseRaiseUpperWhirlwind(CUE_UPPER_WHIRLWIND_SOUTHEAST);
			if (z == 11)
				scJupiterLighthouseRaiseUpperWhirlwind(CUE_UPPER_WHIRLWIND_NORTHEAST);
		}
	}
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

// Sets up the rooms: the Phaeton's Blade's chest set back in the drawing order
// (`scFieldActorSetSpritePriority`), the cracked tiles, and the landing when the party fell through
// a crack from the hall above.
Sint32 scInit(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	state->transition  = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scFieldActorSetSpritePriority(FIELD_TREASURE_ACTOR, 3);
	WhirlwindRiding                                 = 0;
	fdActorGet(ACTOR_WHIRLWIND_SOURCE)->motionFlags = 0;
	if (MainRoutineStateWork.entrance >= ENTRANCE_HOVER_HALL_SOUTH
	    && MainRoutineStateWork.entrance <= ENTRANCE_HOVER_HALL_NORTH)
	{
		scCrackedTileFind(
			METATILE_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 48, 1);
		scCrackedTilePlaceLanding(
			0, 0, SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
	}
	else if (MainRoutineStateWork.entrance == ENTRANCE_LOWER_HALL_NORTH
	         || MainRoutineStateWork.entrance == ENTRANCE_LOWER_HALL_SOUTH
	         || MainRoutineStateWork.entrance == THROUGH_CRACKED_TILE)
	{
		scCrackedTileFind(
			METATILE_BEHAVIOR_UPPER_CRACKED_TILE, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 49, 1);
		scCrackedTilePlaceLanding(
			-21, 32, SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
	}
	else
	{
		scCrackedTileFind(
			METATILE_BEHAVIOR_UPPER_CRACKED_TILE, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 0, 0);
		scCrackedTilePlaceLanding(
			-2, -27, SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
	}
	return 0;
}

// Sets the scene's arrival flags and loads the lighthouse's palettes before the map is built.
// Returns 0.
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

void          syGameFlagSetByte(Sint32 flag, Sint32 value);

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32      PaletteCycleTick;

// Sets the palette cycle's tick (PaletteCycleTick).
static inline void scPaletteCycleSetTick(Sint32 value)
{
	PaletteCycleTick = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_metatile.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];

// The ROM's callers here are compiled against this form, which returns an Sint32 where the
// definition returns a Uint8.
Sint32        fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#include "shared/whirlwind_update.h"

void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

// Sets the priority the lit actors' sprites are drawn at.
void scLightSpritesSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

extern Uint32 FieldEffectLongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseRoomPhaetonsBladeChest),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL, JupiterLighthouseWhorl, JUPITER_LIGHTHOUSE_WHORL_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
	SCENE_EXIT(2, JupiterLighthouseRoomPhaetonsBladeChest, 3),
	SCENE_EXIT(3, JupiterLighthouseRoomPhaetonsBladeChest, 2),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
	SCENE_EXIT(5, JupiterLighthouseRoomPhaetonsBladeChest, 6),
	SCENE_EXIT(6, JupiterLighthouseRoomPhaetonsBladeChest, 5),
	SCENE_EXIT(7, JupiterLighthouseRoomPhaetonsBladeChest, 8),
	SCENE_EXIT(8, JupiterLighthouseRoomPhaetonsBladeChest, 7),
	SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomPhaetonsBladeChest, THROUGH_CRACKED_TILE),
	SCENE_EXIT(THROUGH_UPPER_CRACKED_TILE, JupiterLighthouseRoomPhaetonsBladeChest, THROUGH_UPPER_CRACKED_TILE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, -8, 0, -8, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 680, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 424, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 456, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 472, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 488, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 520, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 552, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 568, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 584, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 600, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 632, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 648, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 664, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 696, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_STEP, CUE_FLOOR_WHIRLWIND_WEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseWhirlwind)},
	{CUE_STEP, CUE_FLOOR_WHIRLWIND_EAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseWhirlwind)},
	{CUE_STEP, CUE_UPPER_WHIRLWIND_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseUpperWhirlwind)},
	{CUE_STEP, CUE_UPPER_WHIRLWIND_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseUpperWhirlwind)},
	{CUE_STEP, CUE_UPPER_WHIRLWIND_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseUpperWhirlwind)},
	{CUE_STEP, CUE_UPPER_WHIRLWIND_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseUpperWhirlwind)},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(fdWhirlwindCarryLeader)},
	{CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnLowerCrackedTile)},
	{CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnCrackedTileOrWhirlwind)},
	{CUE_STEP, METATILE_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnLowerCrackedTile)},
	{CUE_STEP, METATILE_BEHAVIOR_UPPER_CRACKED_TILE, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnUpperCrackedTile)},
	{CUE_STEP, METATILE_BEHAVIOR_UPPER_CRACKED_TILE + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnUpperCrackedTile)},
	{CUE_STEP, METATILE_BEHAVIOR_UPPER_CRACKED_TILE + 2, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStepOnUpperCrackedTile)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartHovering)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStopHovering)},
	{CUE_TABLE_END},
};

Sint32 PaletteCycleTick = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
// clang-format on

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

#include "../field/field_effect_move.c"

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE   fdActorSelectAnimation
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

void          scCrackedTileCrack(METATILE_TARGET *table, METATILE_TARGET_ENTRY *entry);
extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flag, Sint32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
