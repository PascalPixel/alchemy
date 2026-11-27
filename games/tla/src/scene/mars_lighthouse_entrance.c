/* The Mars Lighthouse from its doors up to the dragons' hall: the steps outside
   in the storm and the frozen lower floors inside, in seven scenes that share
   this overlay.

   Felix climbs the ladders outside in the rain to the doors. Inside is the main
   hall, a maze of ice cones that Pound hammers flat, with arched doorways along
   its north wall. Up the stairs from one of them, at a dead end, a shaft of ice
   pokes through the floor: Grind on it sends a crack down into the ice wall in
   the small room east of the main hall, and Burst then blows that wall away.
   Behind it a room holds a teleport circle, and west of that a chamber of
   dragon heads breathes fire down at the floor until a white dragon statue,
   pushed all the way east, blocks the last of them. Past black ice where Felix
   slides until something stops him, white urn torches lit by Blaze send
   fireballs that crack a sheet of ice for Burst and melt the ice in their path,
   and Reveal shows a hidden circle in a room of stone posts. Balm, the Mercury
   Djinni, waits at the end of a black ice slide, and the Teleport Lapis lies in
   a chest on a ledge above the first room's circle. Teleport from the hidden
   circle leads on to the Valkyrie Mail rooms, where a dragon face wakes and
   crawls west breathing fire while Felix crosses the ice ahead of it, and holes
   in the floor drop him to the dragons' hall; the jumping puzzle round the Sol
   Blade chest has cracked tiles that do the same. Last of all, Blaze at the
   main hall's westernmost torch cracks its big sheet of ice, on the way to the
   Mars Djinni Fugue. */

#include "graphics/palette_buffers.h"
#include "scene/flame_throwers.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/scene.h"
#include "system/field_effect.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "edition.h"
#include "field/call.h"
#include "macros.h"
#include "shared/script_check_spheres_overlap.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"
#include "window/window.h"

/* Felix has teleported from the north circle once. */
#define FLAG_TELEPORTED_ONCE			0xa36
/* Grind on the shaft of ice cracked the ice sheet a floor below. */
#define FLAG_ICE_SHAFT_GROUND			0xa3e
#define FLAG_LAPIS_SHEET_BURST			0xa3f
#define FLAG_LAPIS_SECOND_SHEET_BURST	0xa47
#define FLAG_LAPIS_SECOND_SHEET_CRACKED 0xa48 /* a fireball cracked it */
#define FLAG_HIDDEN_CIRCLE_ICE_MELTED \
	0xa49 /* the fireball melted the ice in its path */
#define FLAG_HIDDEN_CIRCLE_SHEET_CRACKED   0xa4c
#define FLAG_HIDDEN_CIRCLE_SHEET_BURST	   0xa4d
#define FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES 0xa4e
#define FLAG_LAPIS_STATUE_BLOCKS_FLAMES	   0xa4f
#define FLAG_LAPIS_SHEET_CRACKED		   FLAG_SCENE_TEMP_30
#define FLAG_LAPIS_SECOND_SHEET_SHOWN	   FLAG_SCENE_TEMP_31
#define FLAG_HIDDEN_CIRCLE_SHEET_SHOWN	   FLAG_SCENE_TEMP_32
#define FLAG_DRAGON_FACE_AWAKE			   FLAG_SCENE_TEMP_34

/* The teleport circles that lead within one scene, each the metatile behaviour
   of its circle, the exit Teleport takes from it and the entrance on it. In the
   Teleport Lapis rooms the south circle is the one behind the burst ice wall
   and the north circle the one by the Teleport Lapis chest; in the Valkyrie
   Mail rooms the north circle is in the room by the long hall and the south one
   below it, by a dragon head on the wall. Each circle sends the party to the
   other of its pair. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_CIRCLE {
	LAPIS_SOUTH_CIRCLE = 17,
	LAPIS_NORTH_CIRCLE = 18,
	VALKYRIE_NORTH_CIRCLE = 12,
	VALKYRIE_SOUTH_CIRCLE = 13
} MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_CIRCLE;

/* The other metatile behaviours this overlay's cue rows answer, besides the
   circles' and the holes' (a hole's behaviour is the exit Felix drops through).
   Black ice Felix slides across; the Sol Blade room's cracked tiles, of which
   those with MAP_METATILE_FLOOR_HOLE give way, and its pillar tops; the
   metatile at the head of the Valkyrie Mail rooms' slide that wakes the dragon
   face; the ice wall Grind cracks and the sheet over the lower passage, and the
   main hall's big sheet of ice, each answering Burst. */
enum {
	METATILE_BEHAVIOR_BLACK_ICE = 30,
	METATILE_BEHAVIOR_CRACKED_ICE_TILE = 31,
	METATILE_BEHAVIOR_PILLAR_TOP = 35,
	METATILE_BEHAVIOR_WAKE_DRAGON_FACE = 35,
	METATILE_BEHAVIOR_CRACKED_ICE_WALL = 80,
	METATILE_BEHAVIOR_BIG_ICE_SHEET = 81
};

/* Balm's room's entrance at the north end of its east hall, where the flame
   thrower is, which the set-up reads. */
enum { BALM_EAST_HALL_NORTH_ENTRANCE = 10 };

/* The Valkyrie Mail room's entrances that the set-up reads: onto its southwest
   floor from the southeast and the middle east floors, and onto its north floor
   from the middle west one. */
enum {
	VALKYRIE_SOUTHWEST_FROM_SOUTHEAST = 2,
	VALKYRIE_SOUTHWEST_FROM_MIDDLE_EAST = 4,
	VALKYRIE_NORTH_FROM_MIDDLE_WEST = 11
};

/* The attribute bit that makes a floor metatile here a hole Felix drops through
   to the room below; the field engine reads it elsewhere as
   MAP_METATILE_CLIMBABLE. */
#define MAP_METATILE_FLOOR_HOLE MAP_METATILE_CLIMBABLE

/* The frames Felix takes to shrink and sink out of sight through a hole. */
#define SINK_FRAMES 40

/* The metatile columns where a pushed white dragon statue stands in the flames'
   way: under the last dragon head at the east end of the flame chamber, and
   under the dragon head on the wall of the Valkyrie Mail rooms' urn torch. */
enum { LAPIS_STATUE_BLOCKING_COLUMN = 20, VALKYRIE_STATUE_BLOCKING_COLUMN = 5 };

/* The metatile columns Balm's room's dragon statue stops in on the black ice:
   pushed west from its place, from where it slides on to the ice's west end,
   and pushed east from there, from where it slides back. */
enum { ICE_STATUE_PUSHED_WEST_COLUMN = 11, ICE_STATUE_PUSHED_EAST_COLUMN = 10 };

/* The rooms up the main hall's doorways (Fugue's scene): two ice cones, the
   first placed out of sight at the map's corner, the shaft of ice Grind cracks
   (an unseen marker on it) and Fugue. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_FUGUE_ACTOR_ID {
	ACTOR_FUGUE_ICE_CONE_A = ACTOR_FIRST_PLACED,
	ACTOR_FUGUE_ICE_CONE_B,
	ACTOR_ICE_SHAFT,
	ACTOR_FUGUE
} MARS_LIGHTHOUSE_ENTRANCE_FUGUE_ACTOR_ID;

/* The main hall and the rooms behind it (the Teleport Lapis scene): the white
   dragon statue pushed into the flames, the flame chamber's four dragon heads,
   the urn torches at the main hall's west end (one lit in the same place as an
   unlit one, each shown by whether the lighthouse has thawed), two ice cones,
   the first placed out of sight at the map's corner, and the two ice sheets. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_ACTOR_ID {
	ACTOR_PUSHED_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_LAPIS_FLAME_THROWER_A,
	ACTOR_LAPIS_FLAME_THROWER_B,
	ACTOR_LAPIS_FLAME_THROWER_C,
	ACTOR_LAPIS_FLAME_THROWER_D,
	ACTOR_LAPIS_TORCH_A,
	ACTOR_LAPIS_TORCH_B,
	ACTOR_LIT_TORCH,
	ACTOR_LAPIS_ICE_CONE_A,
	ACTOR_LAPIS_ICE_CONE_B,
	ACTOR_ICE_SHEET,
	ACTOR_UNLIT_TORCH,
	ACTOR_SECOND_ICE_SHEET,
	ACTOR_LAPIS_SPARKLE,  /* the sparkle that leaves the lit torch */
	ACTOR_LAPIS_FIREBALL, /* the fireball the sparkle sets rolling */
	/* The Teleport Lapis chest, the first actor its map's chests make. */
	ACTOR_TELEPORT_LAPIS_CHEST = 64
} MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_ACTOR_ID;

/* Balm's rooms: the dragon head and the urn torch its flames light, which sends
   fireballs down the corridor of ice cones, the two white dragon statues beside
   the doorway onto the black ice, five ice cones and Balm. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_BALM_ACTOR_ID {
	ACTOR_BALM_FLAME_THROWER = ACTOR_FIRST_PLACED,
	ACTOR_BALM_TORCH,
	ACTOR_BALM_WEST_STATUE,
	ACTOR_BALM_EAST_STATUE,
	ACTOR_BALM_ICE_CONE_A,
	ACTOR_BALM_ICE_CONE_B,
	ACTOR_BALM_ICE_CONE_C,
	ACTOR_BALM_ICE_CONE_D,
	ACTOR_BALM_ICE_CONE_E,
	ACTOR_BALM,
	ACTOR_BALM_SPARKLE,		   /* the sparkle that leaves the lit torch */
	ACTOR_BALM_SECOND_SPARKLE, /* placed, and sent nowhere */
	ACTOR_BALM_FIREBALL		   /* the fireball the sparkle sets rolling */
} MARS_LIGHTHOUSE_ENTRANCE_BALM_ACTOR_ID;

/* The hidden circle's rooms: on each side a lit urn torch and the urn torch
   Blaze sends a fireball from, and the ice sheet the west fireball cracks. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_ACTOR_ID {
	ACTOR_WEST_LIT_TORCH = ACTOR_FIRST_PLACED,
	ACTOR_WEST_TORCH,
	ACTOR_EAST_LIT_TORCH,
	ACTOR_EAST_TORCH,
	ACTOR_HIDDEN_CIRCLE_ICE_SHEET,
	ACTOR_HIDDEN_CIRCLE_SPARKLE, /* the sparkle that leaves the lit torch */
	ACTOR_HIDDEN_CIRCLE_FIREBALL /* the fireball the sparkle sets rolling */
} MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_ACTOR_ID;

/* The Valkyrie Mail rooms: the white dragon statue pushed into the flames, the
   dragon head on the wall and its urn torch, the dragon face that wakes and the
   torch below it, six ice cones, and the sparkle and fireball a torch sends. */
typedef enum MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_ACTOR_ID {
	ACTOR_UPPER_FLAME_THROWER = ACTOR_FIRST_PLACED + 1,
	ACTOR_UPPER_TORCH,
	ACTOR_DRAGON_FACE,
	ACTOR_LOWER_TORCH,
	ACTOR_VALKYRIE_ICE_CONE_A,
	ACTOR_VALKYRIE_ICE_CONE_B,
	ACTOR_VALKYRIE_ICE_CONE_C,
	ACTOR_VALKYRIE_ICE_CONE_D,
	ACTOR_VALKYRIE_ICE_CONE_E,
	ACTOR_VALKYRIE_ICE_CONE_F,
	ACTOR_VALKYRIE_SPARKLE, /* the sparkle that leaves the lit torch */
	ACTOR_VALKYRIE_FIREBALL /* the fireball the sparkle sets rolling */
} MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_ACTOR_ID;

/* An urn torch's script call: the torches stand without a shadow. */
Sint32 scMarsLighthouseEntranceSetUpTorch(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* Talking to the Mars Djinni Fugue, east of the shaft of ice in the room at the
   dead end up the main hall's stairs, from its east or south side, starts the
   battle with it. */
void scMarsLighthouseEntranceFightFugue(void)
{
	fdRequestDjinnBattle(ACTOR_FUGUE, DJINNI_FUGUE);
}

/* Talking to the Mercury Djinni Balm, at the west end of the black ice below
   the two white dragon statues, starts the battle with it. */
void scMarsLighthouseEntranceFightBalm(void)
{
	fdRequestDjinnBattle(ACTOR_BALM, DJINNI_BALM);
}

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

extern ACTOR_PLACE MarsLighthouseEntrance_Placements[];
extern ACTOR_PLACE MarsLighthouseEntrance_OutsidePlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_FuguePlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_LapisPlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_BalmPlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_HiddenCirclePlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_ValkyriePlacements[];
extern ACTOR_PLACE MarsLighthouseEntrance_SolBladePlacements[];

/* The waking dragon face in the Valkyrie Mail rooms' long hall has crawled to
   its west end and stops breathing fire. */
Sint32 scMarsLighthouseEntranceDryDragonFace(void)
{
	scFlameThrowerSet(0, FLAME_THROWER_DRY | 1);
	return 0;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_OUTSIDE)
		return MarsLighthouseEntrance_OutsidePlacements;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE)
		return MarsLighthouseEntrance_FuguePlacements;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS)
		return MarsLighthouseEntrance_LapisPlacements;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM)
		return MarsLighthouseEntrance_BalmPlacements;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE)
		return MarsLighthouseEntrance_HiddenCirclePlacements;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST)
		return MarsLighthouseEntrance_ValkyriePlacements;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST)
		return MarsLighthouseEntrance_SolBladePlacements;
	return MarsLighthouseEntrance_Placements;
}

/* Unused: as the dragon hall's, makes the next cast of Blaze a long spray
   (FIELD_STATE.blazeLongSpray): cast at a target, Blaze then takes
   BLAZE_LONG_SPRAY over BLAZE_SPRAY, and each cast clears it as it starts. */
UNUSED void scMarsLighthouseEntranceStartBlazeOnStatue(void)
{
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	event->blazeLongSpray = 1;
}

extern MAP_METATILE MapGridMetatileBuffer[];

#include "../field/map_metatile_get.c"

#include "shared/map_metatile_set.c"

#include "../field/actor_set_sprite_priority.c"

void fdTeleportPlayWhirl(void);
void fdTeleportPlaySparks(void);
/* Set while a whirlwind carries the leader in the two Jupiter Lighthouse rooms
   that raise one (jupiter_lighthouse_terraces.c and
   jupiter_lighthouse_room_phaetons_blade_chest.c); shared/whirlwind_update.h
   clears it as the ride ends. The rooms of this overlay have no whirlwind, and
   nothing here but scInit's clear touches it. */
Sint32 WhirlwindRiding BSS_DATA;
/* The teleport circle's shimmer, a palette cycle that fdPaletteCycleFade
   steps. */
Uint16 PaletteCycleBright							   BSS_DATA;
Uint16 *PaletteCycleSlots							   BSS_DATA;
Uint16 PaletteCycleTimer							   BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA					   ALIGNED(4);
Uint16 *PaletteSlots								   BSS_DATA;
Uint16 *PaletteColors								   BSS_DATA;
Uint16 *PaletteCycleColors							   BSS_DATA;
Uint16 PaletteCycleLength							   BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA					   ALIGNED(4);
Sint16 MarsLighthouseEntrance_DragonFireTimer BSS_DATA ALIGNED(4);
Uint16 MarsLighthouseEntrance_HoleMaskVram			   BSS_DATA;
Uint32 MarsLighthouseEntrance_HoleMaskSprite[3] BSS_DATA;
volatile Sint16 SlideOnIce_Stopped BSS_DATA;
METATILE_TARGET CrackedTiles	   BSS_DATA;

Sint32 fdMapMetatileGetAttributes();
Sint32 fdMapGetTerrainHeight();
void   fdSetActorSprPriority();

/* Sets the metatiles under an ice cone: standing, it blocks the way, the ground
   there is raised and it is drawn in front; flattened (its flag, kept in its
   timer, set), the metatiles are clear and the ground is lowered again. */
void scMarsLighthouseEntranceSetIceConeMetatiles(ACTOR *actor)
{
	Sint32 z, upperMetatile, height, groundMetatile, x;
	ACTOR *cone;
	Uint8 *layer;

	cone = actor;
	x = FIELD_METATILE(cone->x.fixed);
	z = FIELD_METATILE(cone->z.fixed);
	groundMetatile = fdMapMetatileGetAttributes(0, x, z);
	upperMetatile = fdMapMetatileGetAttributes(2, x, z);
	layer = &cone->layer;
	height = METATILE_HEIGHT_STEPS(
		fdMapGetTerrainHeight(cone->layer, cone->x.fixed, cone->z.fixed));
	if(fdScriptGetFlag(cone->tmr)) {
		upperMetatile &= ~METATILE_CODE_BITS;
		groundMetatile &= ~MAP_METATILE_JUMP;
		if(METATILE_HEIGHT_STEPS(cone->groundHeight) != height)
			height -= 4;
	} else {
		upperMetatile &= METATILE_ATTRIBUTE_BITS;
		upperMetatile |= METATILE_ATTRIBUTES(METATILE_BEHAVIOR_WALL, 0);
		groundMetatile |= MAP_METATILE_JUMP;
		height += 4;
		fdSetActorSprPriority(cone, 2);
	}
	scMapMetatileSetAttributes(0, x, z, groundMetatile);
	scMapMetatileSetAttributes(2, x, z, upperMetatile);
	fdMapGridSetMetatileHeight(
		FIELD_METATILE_ORIGIN(x), FIELD_METATILE_ORIGIN(z), *layer, height);
}

/* Places the ice cones of a list ended by ACTOR_LIST_END, each row three
   halfwords: the cone, its standing pose and the flag set once Pound has
   flattened it, which picks the flattened pose after it. */
void scMarsLighthouseEntrancePlaceIceCones(Uint16 *table)
{
	Uint16		*row;
	FIELD_ACTOR *object;
	Sint16		 flag;
	Sint16		 animation;

	if(table[0] == ACTOR_LIST_END)
		return;
	for(row = table; row[0] != ACTOR_LIST_END; row += 3) {
		object = (FIELD_ACTOR *)fdActorGet(row[0]);
		animation = row[1];
		flag = row[2];

		fdActorSetSprFlags(object, 0);
		fdActorSelectAnimation(
			object, (Uint16)animation + fdScriptGetFlag((Uint16)flag));
		object->motionFlg = 0;
		object->collisionFlg &= ~ACTOR_SOLID;
		object->tmr = flag;
		scMarsLighthouseEntranceSetIceConeMetatiles(object);
	}
}

/* One blow of Pound's hammer on an ice cone: for the first eight the cone grows
   shorter and a chip of blue ice flies off it, lower each time; when Pound is
   done the cone is gone, its flag set and its metatiles cleared
   (scMarsLighthouseEntranceSetIceConeMetatiles), so Felix can walk over the
   spot. */
void scMarsLighthouseEntrancePoundIceCone(Sint32 mode, Sint32 index)
{
	FIELD_EFFECT_STATE *holder = Ram_HeapSlots->fieldEffState;
	ACTOR			   *actor = fdActorGet(holder->tgtId);

	if(index <= 7) {
		EFFECT_OPTIONS options;

		options.pri = 0;
		options.type = SPRITE_BLUE_CRYSTAL;
		options.startScaleX = options.startScaleY = toFIXED(1);
		options.tgtScaleX = options.tgtScaleY = toFIXED(1) / 2;
		options.rotation = (syRandom() & 15) << 12;
		scFieldEffectSpawn(actor->x.fixed,
			actor->y.fixed - index * 3 * toFIXED(1) + toFIXED(16),
			actor->z.fixed, ((syRandom() & 15) - 8) << 13, 0, 0,
			EFFECT_USE_ROTATION | EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
				EFFECT_USE_PRIORITY,
			&options);
		actor->scaleY -= toFIXED(31) / 256;
	}
	if(mode == 1) {
		actor->scaleY = toFIXED(1);
		fdScriptSetFlag(actor->tmr);
		scMarsLighthouseEntranceSetIceConeMetatiles(fdActorGet(holder->tgtId));
		actor->x.fixed = actor->y.fixed = actor->z.fixed = 0;
	}
}

/* Felix casts Pound on one of the ice cones that stand on the lighthouse's
   frozen floors (one up the main hall's stairs, one in the main hall, five in
   the corridor of Balm's rooms, six in the Valkyrie Mail rooms' long hall): a
   great hammer beats it down into the floor, chips of ice flying, until it is
   gone and the way is clear. Each blow comes here with the phase and its count,
   and scMarsLighthouseEntrancePoundIceCone does the work. */
void scMarsLighthouseEntranceStartPoundIceCone(Sint32 mode, Sint32 index)
{
	scMarsLighthouseEntrancePoundIceCone(mode, index);
}

extern Uint32 Debris_Script[];

/* Throws six chips of ice from an actor as its ice cracks or breaks, as the
   star room's scMarsLighthouseStarRoomScatterIceChips does; each overlay keeps
   its own, this one creating them through its other import row. */
void scMarsLighthouseEntranceScatterIceChips(Sint32 id)
{
	ACTOR *source = fdActorGet(id);
	Sint32 i;

	for(i = 0; i < 6; i++) {
		FIXED  x = source->x.fixed + (syRandom() << 4) - (syRandom() << 4);
		ACTOR *actor = scFieldActorCreate(SPRITE_WIND_STONE, x,
			(syRandom() << 3) + source->y.fixed + toFIXED(4),
			source->z.fixed + (syRandom() << 4) - (syRandom() << 4));

		if(actor != NULL) {
			Sint32 heading;
			Sint32 speed;
			Sint32 scale;

			heading = syRandom();
			speed = syRandom();
			scale = (syRandom() >> 1) + toFIXED(1) / 4;
			fdActorStartScript(actor, Debris_Script);
			fdActorSetSprFlags(actor, 0);
			actor->velY = heading * 4 + toFIXED(2);
			actor->velZ = syMulFX(speed * 8, syCos(heading));
			actor->velX = syMulFX(speed * 8, syMathSin(heading));
			actor->acceleration = 0;
			actor->gravity = toFIXED(33) / 100;
			actor->scaleX = scale;
			actor->scaleY = scale;
			actor->bounce = toFIXED(1) / 2;
			fdActorSetSprColMode(actor, SPRITE_COLOR_GRB);
		}
	}
}

/* Felix casts Grind on the shaft of ice poking up through the floor of the
   dead-end room up the main hall's stairs: it crumbles away in bursts of ice
   chips, and the scene cuts to the small room east of the main hall below,
   where the floor shakes and a crack runs through the ice wall, ready for
   Burst, before Felix is brought back. */
void scMarsLighthouseEntranceGrindIce(void)
{
	Sint32 x;
	Sint32 count;

	fdScriptPlaySnd(SE_CRUMBLE);
	x = 32;
	for(count = 0; count < 7; count++) {
		fdMapGridCopyMetatilesLayered(x, 125, 3, 3, 39, 85);
		scMarsLighthouseEntranceScatterIceChips(ACTOR_ICE_SHAFT);
		syWait(10);
		x += 3;
	}
	fdActorTableSnapshot();
	syFieldScriptSetExitRequest(
		MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_B);
}

/* Felix examines the shaft of ice in the dead-end room up the main hall's
   stairs: "Felix checked the ice." and "A shaft of ice is poking up through the
   floor." */
void scMarsLighthouseEntranceCheckIce(void)
{
	/* "Felix checked the ice." */
	mnFieldMessageShow(
		(Sint32)MsgMarsLighthouseEntranceCheckedIce, FIELD_MESSAGE_ZOOM);
	/* "A shaft of ice is poking up through the floor." */
	mnFieldMessageShow(
		(Sint32)MsgMarsLighthouseEntranceCheckedIce + 1, FIELD_MESSAGE_ZOOM);
	FieldStateWork->fieldMenu = 1;
}

/* Felix casts Teleport on the north circle, beside the Teleport Lapis chest:
   the screen turns grey, sparks swirl round him and he appears on the south
   circle in the room behind the burst ice wall. The first time, the game moves
   him and the camera there itself; after that it takes the exit between the two
   circles. */
void scMarsLighthouseEntranceTeleportFromLapisNorthCircle(void)
{
	if(!fdScriptGetFlag(FLAG_TELEPORTED_ONCE)) {
		scTeleportStartShimmer();
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdTeleportPlaySparks();
		fdActorGet(MainRoutineStateWork.leaderActorId)->update = NULL;
		scFieldActorSetSprCol(fdActorGet(MainRoutineStateWork.leaderActorId),
			SPRITE_COLOR_NORMAL);
		fdActorSetPosFX(
			MainRoutineStateWork.leaderActorId, toFIXED(688), toFIXED(360));
		fdCameraMoveTo(688, CAMERA_KEEP, 360, 1);
		fdCameraWaitForArrival();
		fdTeleportPlayWhirl();
		syFieldScriptWait(40);
		scTeleportStopShimmer();
		fdPsynergyFadeFromCastCol();
		syWait(10);
		fdScriptEnd();
		fdScriptSetFlag(FLAG_TELEPORTED_ONCE);
	} else {
		scTeleportStartShimmer();
		fdTeleportPlaySparks();
		MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
		syFieldScriptSetExitRequest(LAPIS_NORTH_CIRCLE);
		syFieldScriptWait(40);
		scTeleportStopShimmer();
	}
}

/* Felix casts Teleport on the south circle, in the room behind the burst ice
   wall: in a swirl of sparks he appears on the north circle, on the ledge
   beside the Teleport Lapis chest. */
void scMarsLighthouseEntranceTeleportFromLapisSouthCircle(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(LAPIS_SOUTH_CIRCLE);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

/* Felix casts Burst on the cracked ice wall in the small room east of the main
   hall (metatiles (51, 41) to (53, 41)): it blows apart in fiery blasts, chips
   of ice flying, and the arched doorway behind it is clear. */
void scMarsLighthouseEntranceBurstIceSheet(Sint32 state, Sint32 frame)
{
	if(state == 1) {
		fdScriptPlaySnd(SE_FLASH);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeInterpolate(60);
		fdMapGridCopyMetatilesLayered(47, 94, 10, 6, 47, 100);
		fdMapGridCopyMetatileAttributes(110, 40, 12, 4, 46, 40);
		fdActorSetPos(ACTOR_ICE_SHEET, 0, 0);
		fdScriptSetFlag(FLAG_LAPIS_SHEET_BURST);
	}
	if(frame > 96 && frame < 272 && frame % 30 == 0)
		scMarsLighthouseEntranceScatterIceChips(ACTOR_ICE_SHEET);
	if(frame == 276) {
		fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
		fdPaletteFadeInterpolate(20);
	}
}

/* Felix casts Blaze at the urn torch at the west end of the main hall: a
   fireball sets off east along the hall toward the big sheet of ice. */
void scMarsLighthouseEntranceBlazeLapisTorch(void)
{
	scMarsLighthouseEntranceShootFireballAtLapisIceSheet();
}

/* As the fireball from the main hall's west torch rolls east, the camera
   follows it along the hall to the sheet of ice it cracks and holds there. */
void scMarsLighthouseEntranceWatchFireballCrackIce(void)
{
	FieldStateWork->psynergy = 0;
	fdScriptClearFlag(FLAG_MENU_OPEN);
	fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(1);
	fdCameraMoveToFX(toFIXED(168), 0, toFIXED(664), 1);
	fdCameraWaitForArrival();
	syWait(10);
	fdCameraSetSpeed(4);
	fdCameraMoveToFX(toFIXED(472), 0, toFIXED(664), 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(90);
	fdScriptEnd();
}

/* Draws the main hall's big sheet of ice cracked, on both floors. */
void scMarsLighthouseEntranceDrawLapisCrackedSheet(void)
{
	fdMapGridCopyMetatileAttributes(14, 55, 14, 3, 14, 41);
	fdMapGridCopyMetatilesLayered(64, 64, 14, 3, 78, 41);
	fdMapGridCopyMetatilesLayered(64, 64, 14, 3, 14, 104);
	fdMapGridCopyMetatileAttributes(64, 64, 14, 3, 78, 41);
	fdMapGridCopyMetatileAttributes(64, 64, 14, 3, 14, 105);
}

/* The fireball from the main hall's west torch reaches the big sheet of ice and
   cracks it, with a crack of ice. */
void scMarsLighthouseEntranceCrackLapisIceSheet(void)
{
	ACTOR *actor = fdActorGet(ACTOR_SECOND_ICE_SHEET);

	if(!fdScriptGetFlag(FLAG_LAPIS_SECOND_SHEET_BURST)) {
		fdScriptPlaySnd(SE_ICE_CRACK);
		fdScriptSetFlag(FLAG_LAPIS_SECOND_SHEET_SHOWN);
		actor->x.fixed = toFIXED(actor->homeX);
		actor->z.fixed = toFIXED(actor->homeZ);
		fdScriptSetFlag(FLAG_LAPIS_SECOND_SHEET_CRACKED);
		scMarsLighthouseEntranceDrawLapisCrackedSheet();
	}
}

/* Defines what Burst does to a sheet of ice: as it breaks, the screen flashes;
   eight columns from x are drawn broken at row z, rows high, from the metatiles
   at row brokenZ, and take the ground of the metatiles at column groundSourceX,
   groundRows high from row groundZ; the sheet's actor goes and flag is set.
   Chips of ice scatter from it every half second, and the screen flashes white
   at the end. */
#define DEFINE_BURST_ICE_SHEET(name, x, brokenZ, rows, z, groundSourceX, \
	groundZ, groundRows, sheet, flag) \
	void name(Sint32 state, Sint32 frame) \
	{ \
		if(state == 1) { \
			fdScriptPlaySnd(SE_FLASH); \
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL); \
			fdPaletteFadeInterpolate(60); \
			fdMapGridCopyMetatilesLayered(x, brokenZ, 8, rows, x, z); \
			fdMapGridCopyMetatileAttributes( \
				groundSourceX, groundZ, 8, groundRows, x, groundZ); \
			fdActorSetPos(sheet, 0, 0); \
			fdScriptSetFlag(flag); \
		} \
		if(frame > 96 && frame < 272 && frame % 30 == 0) \
			scMarsLighthouseEntranceScatterIceChips(sheet); \
		if(frame == 276) { \
			fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL); \
			fdPaletteFadeInterpolate(20); \
		} \
	}

/* Felix casts Burst on the big sheet of ice in the east of the main hall
   (metatiles (29, 43) and (30, 43)), once the west torch's fireball has cracked
   it: it blows apart in fiery blasts and the way past it is clear. */
DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstLapisIceSheet, 28, 92, 8,
	100, 92, 38, 8, ACTOR_SECOND_ICE_SHEET, FLAG_LAPIS_SECOND_SHEET_BURST)

/* Each time Felix pushes or Moves the white dragon statue along the flame
   chamber, west of the room behind the burst ice wall, the game checks where it
   stands. Once it is under the last dragon head at the chamber's east end
   (metatile (20, 7)), it settles a pixel south into the head's flames, blocks
   them, and stays there from then on. */
void scMarsLighthouseEntrancePushStatueIntoFlames(Sint32 unused, Sint32 id)
{
	if(id == ACTOR_PUSHED_STATUE) {
		ACTOR *actor = fdActorGet(ACTOR_PUSHED_STATUE);

		if(FIELD_METATILE(actor->x.fixed) == LAPIS_STATUE_BLOCKING_COLUMN) {
			syFieldScriptWait(5);
			actor->z.fixed += toFIXED(1);
			*(Uint8 *)&actor->sceneCnt = 1;
			fdScriptSetFlag(FLAG_LAPIS_STATUE_BLOCKS_FLAMES);
		}
	}
}

/* Felix examines the Teleport Lapis chest, on the ledge above the north circle;
   besides the chest opening as usual, Retreat stops working, and casting it
   answers "Retreat... It doesn't work here." Retreat works again once the party
   enters another scene: every ordinary arrival clears the flag (gmArrive), but
   walking on within this scene does not. */
void scMarsLighthouseEntranceBlockRetreatAtChest(void)
{
	syGameFlagSet(FLAG_RETREAT_BLOCKED);
}

/* Felix casts Teleport on the circle in the room of the Orihalcon chest: in a
   swirl of sparks he appears on the hidden circle among the stone posts. */
void scMarsLighthouseEntranceTeleportFromBalm(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(
		MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

/* Felix Moves the west one of the two white dragon statues beside the doorway
   onto Balm's black ice: pushed west, it slides along the ice to the wall (x
   152), where it will stop Felix's own slides; pushed east from there, it
   slides back to its place by the doorway (x 200). */
void scMarsLighthouseEntranceSlideStatueOnIce(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if(FIELD_METATILE(actor->x.fixed) == ICE_STATUE_PUSHED_WEST_COLUMN) {
		actor->speedLimit = toFIXED(1) / 4;
		actor->acceleration = toFIXED(1) / 8;
		scFieldActorSetMoveTargetFX(
			actor, toFIXED(152), actor->y.fixed, toFIXED(792));
	}
	if(FIELD_METATILE(actor->x.fixed) == ICE_STATUE_PUSHED_EAST_COLUMN) {
		actor->speedLimit = toFIXED(1) / 4;
		actor->acceleration = toFIXED(1) / 8;
		scFieldActorSetMoveTargetFX(
			actor, toFIXED(200), actor->y.fixed, toFIXED(792));
	}
}

/* Felix casts Blaze at the urn torch at the west end of the room beyond the
   first black ice: the camera moves to the torch as it sends a fireball east
   toward the sheet of ice over the lower passage. */
void scMarsLighthouseEntranceBlazeHiddenCircleWestTorch(void)
{
	fdCameraSetSpeed(1);
	fdCameraMoveTo(88, CAMERA_KEEP, 696, 1);
	scMarsLighthouseEntranceShootFireballAtHiddenCircleIceSheet();
}

/* The camera holds for two seconds on the sheet of ice the west torch's
   fireball cracks. */
void scMarsLighthouseEntranceWatchFireballCrackHiddenCircleIce(void)
{
	FieldStateWork->psynergy = 0;
	fdScriptClearFlag(FLAG_MENU_OPEN);
	fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraWaitForArrival();
	syFieldScriptWait(120);
	fdScriptEnd();
}

/* The west torch's fireball reaches the sheet of ice before the hidden circle's
   rooms, which cracks with a crack of ice, unless Burst has broken it
   already. */
void scMarsLighthouseEntranceCrackHiddenCircleIceSheet(void)
{
	ACTOR *actor = fdActorGet(ACTOR_HIDDEN_CIRCLE_ICE_SHEET);

	if(!fdScriptGetFlag(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
		fdScriptPlaySnd(SE_ICE_CRACK);
		fdScriptSetFlag(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN);
		actor->x.fixed = toFIXED(actor->homeX);
		actor->z.fixed = toFIXED(actor->homeZ);
		fdScriptSetFlag(FLAG_HIDDEN_CIRCLE_SHEET_CRACKED);
	}
}

/* Felix casts Blaze at the urn torch under the great carved dragon face in the
   right-hand room of ice cones: the torch sends a fireball rolling south
   through the ice. */
void scMarsLighthouseEntranceBlazeHiddenCircleEastTorch(void)
{
	scMarsLighthouseEntranceShootFireballThroughIce();
}

/* The camera follows the east torch's fireball south as it melts the ice in its
   path, and holds on the cleared way. */
void scMarsLighthouseEntranceWatchFireballMeltIce(void)
{
	FieldStateWork->psynergy = 0;
	fdScriptClearFlag(FLAG_MENU_OPEN);
	fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(1);
	fdCameraMoveToFX(toFIXED(872), -1, toFIXED(168), 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	fdCameraMoveTo(872, CAMERA_KEEP, 264, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(120);
	fdScriptEnd();
}

/* Felix casts Teleport on the hidden circle among the stone posts: in a swirl
   of sparks he appears on the circle in the room of the Orihalcon chest. */
void scMarsLighthouseEntranceTeleportFromHiddenCircle(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(
		MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

/* While Felix's Reveal lasts, the teleport circle hidden in the floor among the
   room's six stone posts shows (metatiles (53, 48) to (56, 50)). */
void scMarsLighthouseEntranceRevealTeleportCircle(void)
{
	fdMapGridCopyMetatiles(53, 30, 4, 3, 53, 48);
	fdMapGridCopyMetatilesLayered(117, 30, 4, 3, 117, 48);
}

/* As Reveal fades, the hidden teleport circle among the stone posts is covered
   again. */
void scMarsLighthouseEntranceHideTeleportCircle(void)
{
	fdMapGridCopyMetatiles(53, 40, 4, 3, 53, 48);
	fdMapGridCopyMetatilesLayered(117, 45, 4, 3, 117, 48);
}

/* Felix casts Burst on the sheet of ice the west torch's fireball cracked
   (metatiles (14, 42) and (15, 42)): it blows apart in fiery blasts and the
   lower passage is open. */
DEFINE_BURST_ICE_SHEET(MarsLighthouseEntrance_BurstHiddenCircleIceSheet, 12, 95,
	6, 101, 76, 40, 5, ACTOR_HIDDEN_CIRCLE_ICE_SHEET,
	FLAG_HIDDEN_CIRCLE_SHEET_BURST)

/* The east torch's fireball melts the ice in its path and opens the way south
   to the doorway. */
void scMarsLighthouseEntranceMeltIceInPath(void)
{
	fdMapGridCopyMetatileAttributes(35, 11, 3, 6, 53, 11);
	fdMapGridCopyMetatilesLayered(35, 75, 3, 6, 117, 11);
	fdMapGridCopyMetatilesLayered(35, 75, 3, 6, 53, 75);
	fdScriptSetFlag(FLAG_HIDDEN_CIRCLE_ICE_MELTED);
}

/* The fireball in the Valkyrie Mail rooms' long hall has passed: the metatiles
   of its row are cleared. */
void scMarsLighthouseEntranceClearFireballRow(void)
{
	fdMapGridCopyMetatileAttributes(25, 9, 11, 3, 26, 9);
}

/* Felix casts Teleport on the north circle of the Valkyrie Mail rooms
   (metatiles (12, 25) to (15, 27)): in a swirl of sparks he appears on the
   south circle in the room below, by the dragon head on its west wall. */
void scMarsLighthouseEntranceTeleportFromValkyrieNorth(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(VALKYRIE_NORTH_CIRCLE);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

/* Felix casts Teleport on the south circle of the Valkyrie Mail rooms
   (metatiles (11, 44) to (14, 46)), by the dragon head on its west wall: in a
   swirl of sparks he appears on the north circle. */
void scMarsLighthouseEntranceTeleportFromValkyrieSouth(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(VALKYRIE_SOUTH_CIRCLE);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

/* Felix Moves the white dragon statue on the ledge at the west of the south
   circle's room: pushed one metatile east (metatile (5, 43)), it stands under
   the small dragon head on the wall and blocks the flames it breathes at the
   urn torch, and stays there from then on. */
void scMarsLighthouseEntrancePushStatueIntoValkyrieFlames(
	Sint32 unused, Sint32 id)
{
	if(id == ACTOR_PUSHED_STATUE) {
		ACTOR *actor = fdActorGet(ACTOR_PUSHED_STATUE);

		if(FIELD_METATILE(actor->x.fixed) == VALKYRIE_STATUE_BLOCKING_COLUMN) {
			syFieldScriptWait(5);
			actor->z.fixed += toFIXED(1);
			fdScriptSetFlag(FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES);
		}
	}
}

/* Each frame of a fall through the floor: the mask sprite is drawn over Felix's
   metatile, so that he sinks out of sight through the hole. */
void scMarsLighthouseEntranceDrawHoleMask(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   x = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	Sint32	   cameraZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	Sint32	   originY = map->origin[1];
	Sint32	   tile =
		VramCacheEntries[MarsLighthouseEntrance_HoleMaskVram].ofs >> 5;
	ACTOR  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32	y, z, depth, screenX, screenY;
	Uint32 *word;

	x = FIELD_METATILE_ORIGIN(FIELD_METATILE(leader->x.fixed)) - x;
	y = fdMapGetTerrainHeight(0, leader->x.fixed, leader->z.fixed) - originY;
	z = FIELD_METATILE_ORIGIN(FIELD_METATILE(leader->z.fixed)) - cameraZ -
		originY;
	screenY = z - y;
	screenX = FIXEDtoINT(x);
	screenY = FIXEDtoINT(screenY) + 16;
	depth = FIXEDtoINT(y + z) + SCENE_SPRITE_DEPTH;
	if((Uint32)(screenX + 15) > 334 || screenY < -15 || screenY > 239)
		return;
	screenX &= OBJ_X_MASK;
	screenY &= OBJ_Y_MASK;
	word = MarsLighthouseEntrance_HoleMaskSprite;
	*word++ = 0;
	*word++ =
		screenY | OBJ_X(screenX) | OBJ_SIZE_16X32 | OBJ_MODE(OBJ_MODE_WINDOW);
	*word = tile | OBJ_PRIORITY(1);
	syOAMAddSprAtDepthClamped(MarsLighthouseEntrance_HoleMaskSprite, depth);
}

/* Hides Felix's sprite below the floor as he drops through it: a sprite of
   colour 1 in a VRAM block of its own, drawn each frame in the OBJ window over
   his metatile (scMarsLighthouseEntranceDrawHoleMask). */
void scMarsLighthouseEntranceStartHoleMask(void)
{
	Uint32 *buffer = syHeapAlloc(HOLE_MASK_BYTES);
	Uint32 *word = buffer;
	Sint32	count;

	for(count = 0; count < HOLE_MASK_BYTES / 4; count++) {
		*word++ = PIXELS_4BPP(1);
	}
	MarsLighthouseEntrance_HoleMaskVram = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(MarsLighthouseEntrance_HoleMaskVram, HOLE_MASK_BYTES, buffer);
	syHeapRelease(buffer);
	sySetFrameEvent(scMarsLighthouseEntranceDrawHoleMask, EVENT_PASS_FRAME);
}

/* Shows every layer and the sprites outside the windows, and every layer inside
   the sprite window. */
#define SET_SPRITE_WINDOW_LAYERS() \
	do { \
		Uint16 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | \
			WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | \
			WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | \
			WINOUT_WINOBJ_BG3; \
\
		REG_WINOUT = winout; \
	} while(0)

/* Felix walks onto one of the purple holes on the upper floor of the Valkyrie
   Mail rooms (metatiles (15, 43) to (16, 46) east of the south circle, and (54,
   8) to (55, 11) in the long hall): startled, he raises his arms and sinks out
   of sight, the screen closes in on him, and he drops into a room of the
   dragons' hall below, landing in a puff of dust. The metatile's behaviour is
   the exit he takes. */
void scMarsLighthouseEntranceFallIntoHole(Sint32 arg)
{
	ACTOR		 *actor;
	MAP_STATE	 *map;
	MAP_METATILE *metatiles;
	MAP_METATILE *buffer;
	Sint32		  x, z;
	Sint32		  i;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	actor->update = NULL;
	{
		Sint32 value = EMOTE_IN_FRONT | EMOTE_EXCLAMATION;

		scFieldActorShowEmote(MainRoutineStateWork.leaderActorId, value, 0);
	}
	fdActorSelectAnimation(actor, ANIM_LEADER_WALK_BACKWARDS);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
	syWait(30);
	actor->facing = FACING_SOUTH;
	actor->motionFlg = 0;
	buffer = MapGridMetatileBuffer;
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed);
	map = Ram_HeapSlots->mapGrid;
	if(map != NULL)
		metatiles = map->layers[actor->layer & 3].metatiles;
	else
		metatiles = buffer;
	metatiles += x + (z << 7) + MAPGRID_SIZE;
	if(!(metatiles->attributeB & MAP_METATILE_FLOOR_HOLE)) {
		SET_SPRITE_WINDOW_LAYERS();
		REG_DISPCNT |= DISPCNT_OBJWIN_ON;
		scMarsLighthouseEntranceStartHoleMask();
	}
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(16);
	for(i = 0; i < SINK_FRAMES; i++) {
		if(i == 5)
			fdScriptPlaySnd(SE_DUST_PUFF);
		actor->scaleX -= toFIXED(1) / 64;
		actor->scaleY -= toFIXED(3) / 256;
		actor->y.fixed -= toFIXED(6) / 10;
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(arg);
}

extern Sint32 MarsLighthouseEntrance_DragonFaceGoesLeft[];
extern CUE	  MarsLighthouseEntrance_Cues[];
extern CUE	  MarsLighthouseEntrance_OutsideCues[];
extern CUE	  MarsLighthouseEntrance_FugueCues[];
extern CUE	  MarsLighthouseEntrance_LapisCues[];
extern CUE	  MarsLighthouseEntrance_BalmCues[];
extern CUE	  MarsLighthouseEntrance_HiddenCircleCues[];
extern CUE	  MarsLighthouseEntrance_ValkyrieCues[];
extern CUE	  MarsLighthouseEntrance_SolBladeCues[];

/* Felix steps onto metatile (52, 8) in the Valkyrie Mail rooms' long hall, at
   the head of the slide down the ice: with a click the dragon face wakes, and
   the game can no longer be saved. Great fireballs roll east along the ice
   towards him, shattering the ice blocks in their way and bursting into flames
   at the hall's east end, and the face crawls along the hall's north wall,
   coming east almost to Felix, then turning and crawling back west out of
   sight, still spitting fire. Felix has to cross the ice ahead of it. While he
   stays at the head of the slide nothing else happens to him. */
void scMarsLighthouseEntranceWakeDragonFace(void)
{
	fdScriptPlaySnd(SE_BUTTON_PRESS);
	fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
	fdActorSetScript(
		ACTOR_DRAGON_FACE, MarsLighthouseEntrance_DragonFaceGoesLeft);
	scFlameThrowerSet(0, FLAME_FORWARD | 3);
	fdScriptSetFlag(FLAG_DRAGON_FACE_AWAKE);
}

/* Stepping onto the black ice in the Valkyrie Mail rooms' long hall, Felix
   slides on across it; if a fireball has hit him he rides along with it. */
void scMarsLighthouseEntranceSlideFromEdge(void)
{
	scLeaderSlideOnIceFromEdge();
	scMarsLighthouseEntranceRideFireballIfHit();
}

void syGameFlagSetByte(Sint32 flag, Sint32 value);

/* Felix drops through a cracked tile of the Sol Blade room: startled, he raises
   his arms and sinks out of sight, and lands in the room below. The metatile he
   fell through is kept, so the landing room can place him under it. */
void scMarsLighthouseEntranceFallThroughCrack(const MAP_METATILE *metatile)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIXED  z = leader->z.fixed;
	Sint32 i = FIELD_METATILE(leader->x.fixed);

	syGameFlagSetByte(FLAG_BYTE_FALL_COLUMN, i);
	syGameFlagSetByte(FLAG_BYTE_FALL_ROW, FIELD_METATILE(z));
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	leader->update = NULL;
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
	fdActorSetAnimationSpeed(leader, ANIM_STEP_NORMAL * 3);
	syWait(30);
	leader->facing = FACING_SOUTH;
	leader->motionFlg = 0;
	if((metatile->attributeB & MAP_METATILE_FLOOR_HOLE) == 0) {
		{
			Uint16 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 |
				WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ |
				WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 |
				WINOUT_WINOBJ_BG3;

			REG_WINOUT = winout;
		}
		REG_DISPCNT |= DISPCNT_OBJWIN_ON;
		scMarsLighthouseEntranceStartHoleMask();
	}
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(16);
	for(i = 0; i < SINK_FRAMES; i++) {
		if(i == 5)
			fdScriptPlaySnd(SE_DUST_PUFF);
		leader->scaleX -= toFIXED(1) / 64;
		leader->scaleY -= toFIXED(3) / 256;
		leader->y.fixed -= toFIXED(6) / 10;
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(
		MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_B);
}

/* Felix walks onto one of the pillar tops of the Sol Blade room's jumping
   puzzle (metatiles (13, 28), (7, 30), (11, 34), (8, 36) and (14, 37)): he hops
   on across the gap the way he is walking where there is somewhere to land, and
   otherwise slides on as on ice. */
void scMarsLighthouseEntranceJumpOrSlide(void)
{
	fdTryJumpForward();
	fdLeaderSlideOnIceAhead();
}

/* Felix steps on one of the pale cracked tiles among the pillar tops of the Sol
   Blade room (on its upper floor, metatiles (8, 29) to (14, 38)): those that
   give way drop him through to the room below, as
   scMarsLighthouseEntranceFallThroughCrack shows; the others hold. */
void scMarsLighthouseEntranceCheckCrackedFloor(void)
{
	MAP_STATE		   *map = Ram_HeapSlots->mapGrid;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR			   *actor;
	Sint32				x;
	Sint32				z;
	MAP_METATILE	   *metatile;

	actor = fdActorGet(party->leaderActorId);
	x = FIELD_METATILE(actor->x.fixed);
	z = FIELD_METATILE(actor->z.fixed);
	metatile = MapGridMetatileBuffer;
	if(map != NULL)
		metatile = map->layers[actor->layer & 3].metatiles;
	metatile += x + (z << 7);
	if(metatile->attributeB & MAP_METATILE_FLOOR_HOLE)
		scMarsLighthouseEntranceFallThroughCrack(metatile + MAPGRID_SIZE);
}

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_OUTSIDE)
		return MarsLighthouseEntrance_OutsideCues;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE)
		return MarsLighthouseEntrance_FugueCues;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS)
		return MarsLighthouseEntrance_LapisCues;
	if(MainRoutineStateWork.next == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM)
		return MarsLighthouseEntrance_BalmCues;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE)
		return MarsLighthouseEntrance_HiddenCircleCues;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST)
		return MarsLighthouseEntrance_ValkyrieCues;
	if(MainRoutineStateWork.next ==
		SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST)
		return MarsLighthouseEntrance_SolBladeCues;
	return MarsLighthouseEntrance_Cues;
}

#include "shared/actor_freeze.h"
#undef SET_SPRITE_WINDOW_LAYERS

extern Sint16 MarsLighthouseEntrance_DragonFireOn;

/* The dragon face at the west end of the long hall breathes fire for a second
   and stops for four, in turn. */
void scMarsLighthouseEntrancePulseDragonFire(void)
{
	if(MarsLighthouseEntrance_DragonFireTimer < 60) {
		if(MarsLighthouseEntrance_DragonFireOn == 0) {
			scFlameThrowerSet(0, FLAME_FORWARD | 3);
			MarsLighthouseEntrance_DragonFireOn = 1;
		}
	} else {
		if(MarsLighthouseEntrance_DragonFireOn == 1) {
			scFlameThrowerSet(0, FLAME_THROWER_DRY | 3);
			MarsLighthouseEntrance_DragonFireOn = 0;
		}
	}
	if(++MarsLighthouseEntrance_DragonFireTimer > 300)
		MarsLighthouseEntrance_DragonFireTimer = 0;
}

/* Each frame the waking dragon face flickers: tinted red for two frames in
   eight. */
void scMarsLighthouseEntranceFlickerDragonFace(ACTOR *actor)
{
	switch(FrameCount & 7) {
	case 0:
		fdActorSetSprColMode(actor, 0);
		break;
	case 6:
		fdActorSetSprColMode(actor, SPRITE_COLOR_RED_TINT);
		break;
	}
}

/* An ice cone the dragon face's fireball reaches spins and flies off east,
   shrinking. */
void scMarsLighthouseEntranceFlingIceCone(ACTOR *actor)
{
	FIELD_SPR *sprite = actor->spr;

	sprite->rotation += TRIG_TURN / 32;
	if(++actor->tmr == 8)
		actor->velY = toFIXED(4);
	if(actor->tmr > 9) {
		actor->x.fixed += toFIXED(4);
		actor->scaleX -= toFIXED(1) / 32;
		actor->scaleY -= toFIXED(1) / 32;
	}
	if(actor->tmr == 32) {
		actor->x.fixed = 0;
		actor->z.fixed = 0;
		actor->update = NULL;
	}
}

/* An ice cone waits until the dragon face's fireball comes within 32 pixels
   across, then is flung away. */
void scMarsLighthouseEntranceWaitForDragonFace(ACTOR *actor)
{
	if(ABS(actor->x.fixed - actor->watchedActor->actor.x.fixed) > toFIXED(32))
		return;
	actor->tmr = 0;
	actor->update = (OBJECT_UPDATE)scMarsLighthouseEntranceFlingIceCone;
}

/* Nothing visible happens of itself. In the Valkyrie Mail rooms the dragon
   face's fireball hits Felix on the ice and he sits blinking on it, red, as it
   carries him east across the ice to the hole at its end, where "!" shows over
   him and he falls through to the room below; each frame of that ride the game
   holds his own slide on the ice stopped, so that only the fireball moves
   him. */
void scMarsLighthouseEntranceStopSlideWhileRiding(void)
{
	if(scFlameThrowersHasHitLeader())
		scLeaderSetSlideStopped(1);
}

void scFlameThrowersStart(
	Uint16 *targets, Uint16 *stops, Sint32 onStopped, Sint32 hitCue);
void scFlameThrowerSetPuffsOffScreen(Sint32 index, Sint16 value);
void scPaletteSetColsFade(Uint16 *slots, Uint16 *colours, Uint16 *a, Uint16 *b);
void scMarsLighthouseEntranceShootValkyrieUpperFireball(void);
extern Sint32 MarsLighthouseEntrance_DragonFaceAtLeft[];
extern Uint16 MarsLighthouseEntrance_FugueIceCones[];
extern Uint16 MarsLighthouseEntrance_LapisBeamTargets[];
extern Uint16 MarsLighthouseEntrance_LapisBeamStops[];
extern Uint16 MarsLighthouseEntrance_LapisIceCones[];
extern Uint16 MarsLighthouseEntrance_BalmBeamTargets[];
extern Uint16 MarsLighthouseEntrance_BalmBeamStops[];
extern Uint16 MarsLighthouseEntrance_BalmIceCones[];
extern Uint16 MarsLighthouseEntrance_ValkyrieUpperBeamTargets[];
extern Uint16 MarsLighthouseEntrance_ValkyrieUpperBeamStops[];
extern Uint16 MarsLighthouseEntrance_ValkyrieLowerBeamTargets[];
extern Uint16 MarsLighthouseEntrance_ValkyrieLowerBeamStops[];
extern Uint16 MarsLighthouseEntrance_ValkyrieIceCones[];
extern Uint16 TeleportCircle_PaletteSlots[];
extern Uint16 TeleportCircle_Colors[];
extern Uint16 TeleportCircle_ShimmerColors[];
extern Uint16 TeleportCircle_ShimmerSlots[];

Sint32 scInit(void)
{
	Sint32 scene = MainRoutineStateWork.next;
	Sint32 entrance = MainRoutineStateWork.entrance;
	ACTOR *actor;
	Sint32 i;

	WhirlwindRiding = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdScriptSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
	scFlameThrowersCreate();
	if(scene == SCENE_MARS_LIGHTHOUSE_OUTSIDE) {
		fdWeatherStartRain(1);
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_FUGUE) {
		fdEffectStartBlankOverlay(0);
#if !FUGUE_NEW_POSITION
		scFieldActorSetSprPriority(ACTOR_FUGUE, 3);
#endif
		scActorFreeze(ACTOR_ICE_SHAFT);
		if(fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
			fdMapGridCopyMetatilesLayered(35, 41, 24, 10, 3, 53);
			fdMapGridCopyMetatilesLayered(35, 105, 24, 10, 3, 117);
		}
		scMarsLighthouseEntrancePlaceIceCones(
			(Sint32)MarsLighthouseEntrance_FugueIceCones);
		if(fdScriptGetFlag(FLAG_ICE_SHAFT_GROUND))
			fdMapGridCopyMetatilesLayered(50, 125, 3, 3, 39, 85);
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS) {
		fdEffectStartBlankOverlay(0);
		scFlameThrowersStart((Sint32)MarsLighthouseEntrance_LapisBeamTargets,
			(Sint32)MarsLighthouseEntrance_LapisBeamStops, 0,
			STEP_CUE_FLAME_HIT);
		scMarsLighthouseEntrancePlaceIceCones(
			(Sint32)MarsLighthouseEntrance_LapisIceCones);
		if(fdScriptGetFlag(FLAG_LAPIS_SHEET_BURST)) {
			fdMapGridCopyMetatilesLayered(47, 94, 10, 6, 47, 100);
			fdMapGridCopyMetatileAttributes(110, 40, 12, 4, 46, 40);
			fdActorSetPos(ACTOR_ICE_SHEET, 0, 0);
		} else if(fdScriptGetFlag(FLAG_ICE_SHAFT_GROUND)) {
			fdScriptSetFlag(FLAG_LAPIS_SHEET_CRACKED);
			fdActorGet(ACTOR_ICE_SHEET)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		} else {
			fdActorSetPos(ACTOR_ICE_SHEET, 0, 0);
		}
		fdActorGet(ACTOR_SECOND_ICE_SHEET)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		scFieldActorSetSprPriority(ACTOR_SECOND_ICE_SHEET, 2);
		if(fdScriptGetFlag(FLAG_LAPIS_SECOND_SHEET_BURST)) {
			fdMapGridCopyMetatilesLayered(28, 92, 8, 8, 28, 100);
			fdMapGridCopyMetatileAttributes(92, 38, 8, 8, 28, 38);
			scMarsLighthouseEntranceDrawLapisCrackedSheet();
			fdActorSetPos(ACTOR_SECOND_ICE_SHEET, 0, 0);
		} else if(fdScriptGetFlag(FLAG_LAPIS_SECOND_SHEET_CRACKED)) {
			scMarsLighthouseEntranceDrawLapisCrackedSheet();
			fdScriptSetFlag(FLAG_LAPIS_SECOND_SHEET_SHOWN);
		} else {
			fdActorSetPos(ACTOR_SECOND_ICE_SHEET, 0, 0);
		}
		if(fdScriptGetFlag(FLAG_LAPIS_STATUE_BLOCKS_FLAMES)) {
			fdActorSetPos(ACTOR_PUSHED_STATUE, 328, 120);
			fdActorGet(ACTOR_PUSHED_STATUE)->sceneCnt = 1;
		}
		if(fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
			fdActorSetPos(ACTOR_UNLIT_TORCH, 0, 0);
		else
			fdActorSetPos(ACTOR_LIT_TORCH, 0, 0);
		if(entrance ==
			MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST) {
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdMapGridSetShake(0, 2, 1);
			fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
			fdActorSetPos(ACTOR_ICE_SHEET, 0, 0);
			fdActorGet(ACTOR_ICE_SHEET)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionFrames();
			actor = fdActorGet(ACTOR_ICE_SHEET);
			fdActorSetPos(ACTOR_ICE_SHEET, actor->tmr, actor->userValue);
			fdScriptPlaySnd(SE_ICE_CRACK);
			for(i = 0; i < 3; i++) {
				scMarsLighthouseEntranceScatterIceChips(ACTOR_ICE_SHEET);
				syWait(1);
			}
			fdScriptSetFlag(FLAG_ICE_SHAFT_GROUND);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			syFieldScriptWait(120);
			syFieldScriptSetExitRequest(
				MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST);
			fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
		}
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_BALM) {
		fdEffectStartBlankOverlay(0);
		if(entrance == BALM_EAST_HALL_NORTH_ENTRANCE ||
			entrance ==
				MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH) {
			scFlameThrowersStart((Sint32)MarsLighthouseEntrance_BalmBeamTargets,
				(Sint32)MarsLighthouseEntrance_BalmBeamStops,
				(Sint32)scMarsLighthouseEntranceShootBalmFireball,
				STEP_CUE_FLAME_HIT);
			scFlameThrowerSetPuffsOffScreen(0, 1);
		}
		scMarsLighthouseEntrancePlaceIceCones(
			(Sint32)MarsLighthouseEntrance_BalmIceCones);
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE) {
		fdEffectStartBlankOverlay(0);
		fdActorGet(ACTOR_HIDDEN_CIRCLE_ICE_SHEET)->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT;
		if(fdScriptGetFlag(FLAG_HIDDEN_CIRCLE_SHEET_BURST)) {
			fdMapGridCopyMetatilesLayered(12, 95, 8, 6, 12, 101);
			fdMapGridCopyMetatileAttributes(76, 40, 8, 5, 12, 40);
			fdActorSetPos(ACTOR_HIDDEN_CIRCLE_ICE_SHEET, 0, 0);
		} else if(fdScriptGetFlag(FLAG_HIDDEN_CIRCLE_SHEET_CRACKED)) {
			fdScriptSetFlag(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN);
		} else {
			fdActorSetPos(ACTOR_HIDDEN_CIRCLE_ICE_SHEET, 0, 0);
		}
		if(fdScriptGetFlag(FLAG_HIDDEN_CIRCLE_ICE_MELTED))
			scMarsLighthouseEntranceMeltIceInPath();
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST) {
		fdEffectStartBlankOverlay(0);
		if(entrance == VALKYRIE_SOUTHWEST_FROM_SOUTHEAST ||
			entrance == VALKYRIE_SOUTHWEST_FROM_MIDDLE_EAST ||
			entrance ==
				MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST ||
			entrance == VALKYRIE_SOUTH_CIRCLE) {
			scFlameThrowersStart(
				(Sint32)MarsLighthouseEntrance_ValkyrieUpperBeamTargets,
				(Sint32)MarsLighthouseEntrance_ValkyrieUpperBeamStops,
				(Sint32)scMarsLighthouseEntranceShootValkyrieUpperFireball,
				STEP_CUE_FLAME_HIT);
			scFlameThrowerSetPuffsOffScreen(0, 1);
		}
		if(entrance ==
				MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM ||
			entrance ==
				MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST ||
			entrance == VALKYRIE_NORTH_FROM_MIDDLE_WEST) {
			ACTOR *target;

			scFlameThrowersStart(
				(Sint32)MarsLighthouseEntrance_ValkyrieLowerBeamTargets,
				(Sint32)MarsLighthouseEntrance_ValkyrieLowerBeamStops,
				(Sint32)scMarsLighthouseEntranceShootValkyrieLowerFireball,
				STEP_CUE_FLAME_HIT);
			scFlameThrowerSetPuffsOffScreen(0, 1);
			if(entrance == VALKYRIE_NORTH_FROM_MIDDLE_WEST) {
				fdActorSetScript(
					ACTOR_DRAGON_FACE, MarsLighthouseEntrance_DragonFaceAtLeft);
				scFlameThrowerSet(0, FLAME_FORWARD | 3);
				sySetFrameEvent(
					scMarsLighthouseEntrancePulseDragonFire, EVENT_PASS_FRAME);
				fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
			}
			sySetFrameEvent(
				scMarsLighthouseEntranceStopSlideWhileRiding, EVENT_PASS_FRAME);
			fdActorGet(ACTOR_DRAGON_FACE)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceFlickerDragonFace;
			scFlameThrowersSetMode(1);
			scMarsLighthouseEntrancePlaceIceCones(
				(Sint32)MarsLighthouseEntrance_ValkyrieIceCones);
			target = fdActorGet(ACTOR_VALKYRIE_FIREBALL);
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_A)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_B)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_C)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_D)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_E)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_F)->update =
				(OBJECT_UPDATE)scMarsLighthouseEntranceWaitForDragonFace;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_A)->watchedActor =
				(FIELD_OBJECT *)target;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_B)->watchedActor =
				(FIELD_OBJECT *)target;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_C)->watchedActor =
				(FIELD_OBJECT *)target;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_D)->watchedActor =
				(FIELD_OBJECT *)target;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_E)->watchedActor =
				(FIELD_OBJECT *)target;
			fdActorGet(ACTOR_VALKYRIE_ICE_CONE_F)->watchedActor =
				(FIELD_OBJECT *)target;
		}
		fdActorGet(ACTOR_UPPER_FLAME_THROWER)->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT;
		if(fdScriptGetFlag(FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES))
			fdActorSetPos(ACTOR_PUSHED_STATUE, 88, 696);
	} else if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST) {
		fdEffectStartBlankOverlay(0);
	}
	if(scene == SCENE_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST &&
		entrance == VALKYRIE_SOUTH_CIRCLE)
		fdScriptClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
	scPaletteSetColsFade((Sint32)TeleportCircle_PaletteSlots,
		(Sint32)TeleportCircle_Colors, (Sint32)TeleportCircle_ShimmerColors,
		(Sint32)TeleportCircle_ShimmerSlots);
	return 0;
}

extern Sint32 MarsLighthouseEntrance_ValkyrieUpperFireballPath[];
extern Sint32 MarsLighthouseEntrance_ValkyrieLowerFireballPath[];
extern Sint32 MarsLighthouseEntrance_BalmFireballPath[];
extern Sint32 MarsLighthouseEntrance_FireballToHiddenCircleIceSheet[];
extern Sint32 MarsLighthouseEntrance_FireballThroughIce[];
extern Sint32 MarsLighthouseEntrance_FireballToLapisIceSheet[];

Sint32 scPrepareMap(void)
{
	if(fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
		fdMapCopyPalette(
			MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
			(Sint32)BG_PLTT);
	return 0;
}

/* The flames of the Valkyrie Mail rooms' dragon head reach the urn torch below
   it, and its fireball sets off east. */
void scMarsLighthouseEntranceShootValkyrieUpperFireball(void)
{
	scActorAttach(ACTOR_UPPER_TORCH, ACTOR_VALKYRIE_SPARKLE,
		ACTOR_VALKYRIE_FIREBALL, FACING_EAST,
		MarsLighthouseEntrance_ValkyrieUpperFireballPath);
}

/* The flames reach the urn torch at the west end of the long hall, and its
   fireball sets off east along the hall. */
void scMarsLighthouseEntranceShootValkyrieLowerFireball(void)
{
	scActorAttach(ACTOR_LOWER_TORCH, ACTOR_VALKYRIE_SPARKLE,
		ACTOR_VALKYRIE_FIREBALL, FACING_EAST,
		MarsLighthouseEntrance_ValkyrieLowerFireballPath);
}

/* The dragon head's flames reach the urn torch at the head of the corridor of
   ice cones in Balm's rooms, and its fireball sets off south down it. */
void scMarsLighthouseEntranceShootBalmFireball(void)
{
	scActorAttach(ACTOR_BALM_TORCH, ACTOR_BALM_SPARKLE, ACTOR_BALM_FIREBALL,
		FACING_SOUTH, MarsLighthouseEntrance_BalmFireballPath);
}

/* The west urn torch's fireball sets off east for the sheet of ice. */
void scMarsLighthouseEntranceShootFireballAtHiddenCircleIceSheet(void)
{
	scActorAttach(ACTOR_WEST_TORCH, ACTOR_HIDDEN_CIRCLE_SPARKLE,
		ACTOR_HIDDEN_CIRCLE_FIREBALL, FACING_EAST,
		MarsLighthouseEntrance_FireballToHiddenCircleIceSheet);
}

/* The urn torch under the dragon face sends its fireball south through the
   ice. */
void scMarsLighthouseEntranceShootFireballThroughIce(void)
{
	scActorAttach(ACTOR_EAST_TORCH, ACTOR_HIDDEN_CIRCLE_SPARKLE,
		ACTOR_HIDDEN_CIRCLE_FIREBALL, FACING_SOUTH,
		MarsLighthouseEntrance_FireballThroughIce);
}

/* The main hall's west torch sends its fireball east for the big sheet of
   ice. */
void scMarsLighthouseEntranceShootFireballAtLapisIceSheet(void)
{
	scActorAttach(ACTOR_LAPIS_TORCH_A, ACTOR_LAPIS_SPARKLE,
		ACTOR_LAPIS_FIREBALL, FACING_EAST,
		MarsLighthouseEntrance_FireballToLapisIceSheet);
}

void scFlameThrowerUpdateSpark(ACTOR *actor);

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/flame_update_forward.h"

extern Sint32 ScriptedPart_Script[];

void scFlameThrowerUpdateSpark(ACTOR *actor);

#include "shared/actor_reset_scripted_for.h"

#include "shared/flame_thrower_send_forward.c"

#include "shared/flame_update_sideways.h"

extern Sint32 ScriptedPart_Script[];

void scFlameThrowerUpdateSpark(ACTOR *actor);

#include "shared/actor_setup_scripted_part.h"

#include "shared/flame_thrower_send_backward.c"

#include "shared/flame_thrower_send_east.c"

#include "shared/flame_thrower_send_west.c"

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted_for.h"

#include "shared/flame_thrower_send_spark.c"

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

/* Releases the kept flame. */
UNUSED void scFlameThrowersRelease(void)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	fdActorRelease(state->sharedFlame);
	state->sharedFlame = NULL;
}

/* Sets whether the flame throwers keep puffing while Psynergy is used. */
void scFlameThrowersSetMode(Sint16 value)
{
	FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

void scFlameThrowersPuff(void);

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

/* The flame throwers' counter, which scFlameThrowersAdvanceCounter steps. */
UNUSED Sint32 scFlameThrowersGetCounter(void)
{
	return FLAME_THROWERS_STATE->cnt;
}

#include "shared/flame_throwers_advance_counter.h"

#include "shared/flame_thrower_set.h"

#include "shared/flame_thrower_set_puffs_off_screen.c"

/* Flame thrower `index`'s flags: which flame it sends and how far. */
UNUSED Sint32 scFlameThrowerGetFlags(Sint32 index)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + index)->flg;
}

#include "shared/flame_throwers_set_stop.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

#include "shared/leader_knock_back.c"

/* Hit by the flames, Felix is knocked back. */
void scMarsLighthouseEntranceKnockBackIfHit(void)
{
	if(FLAME_THROWERS_STATE->active)
		fdLeaderKnockBack();
}

/* A rolling fireball hits Felix in Balm's corridor of ice cones or the Valkyrie
   Mail rooms: the party takes damage, and he sits blinking on the fireball and
   is carried along with it until it stops (FLAME_HIT_STATE.car), kept to a
   metatile's centre over rounded ground. */
int scMarsLighthouseEntranceRideFireball(void)
{
	FIELD_STATE		*event = Ram_HeapSlots->fieldState;
	FLAME_HIT_STATE *ride;
	ACTOR			*leader;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Sint32 frame[3];

	if(event->transitionActive == 0) {
		fdScreenTransitionStart(event->transition, event->transitionFrm);
		event->transitionActive = 1;
	}
	fdScriptClearFlag(FLAG_MENU_OPEN);
	event->psynergy = 0;
	event->fieldMenu = 0;
	event->pauseMenu = 0;
	event->aButton = 0;
	event->encounter = 0;
#if FIX_FUNCTION_SIGNATURES
	gmPartyTakeFallDamage();
#else
	/* UB: the fall damage takes no arguments and returns nothing; it is passed
	   the arguments it gives fdPartyAddHp. */
	((Sint32 (*)())gmPartyTakeFallDamage)(-FALL_DAMAGE, 0);
#endif
	fdCameraSetSpeed(7);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ride = FLAME_THROWERS_STATE;
	fdScriptSetFlag(FLAG_MENU_OPEN);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptClearFlag(FLAG_MENU_OPEN);
	leader->update = (OBJECT_UPDATE)fdActorBlink;
	leader->facing = FACING_SOUTH;
	fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
	while(ride->car->tmr == 0) {
		if(fdMapGridGetTerrainKindOrBumpAt(
			   0, leader->x.fixed, leader->z.fixed) != TERRAIN_ROUNDED_SQUARE) {
			leader->x.fixed += ride->car->stepX;
			leader->z.fixed += ride->car->stepZ;
		} else {
			if(ride->car->stepX != 0)
				leader->x.fixed =
					(leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
			if(ride->car->stepZ != 0)
				leader->z.fixed =
					(leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		}
		ride->active = 0;
		syWait(1);
	}
	syFieldScriptWait(30);
	leader->update = NULL;
	fdActorSetSprColMode(leader, 0);
	ride->active = 0;
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Stepping onto the south circle of the Valkyrie Mail rooms while a fireball
   has hit him, Felix is carried along with it
   (scMarsLighthouseEntranceRideFireball); otherwise nothing happens. The slide
   from the ice's edge asks the same, ignoring the answer. */
Bool scMarsLighthouseEntranceRideFireballIfHit(void)
{
	if(FLAME_THROWERS_STATE->active) {
		scMarsLighthouseEntranceRideFireball();
		return TRUE;
	}
	return FALSE;
}

/* Whether a flame or fireball has hit Felix. */
Sint32 scFlameThrowersHasHitLeader(void)
{
	return FLAME_THROWERS_STATE->active;
}

extern Uint32 Crumbs_Script[];

#include "shared/actor_scatter_crumbs.c"

extern Uint32 IceChips_Script[];

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_script.c"

#include "shared/actor_roll_and_crumble.c"

/* Sends a sparkle from a torch along its path, at whose end it sets a fireball
   rolling toward `facing` (scActorPark). Its callers take it as returning a
   word; it returns none. */
Sint32 scActorAttach(Sint32 torchId, Sint32 sparkleId, Sint32 fireballId,
	Sint32 facing, const void *path)
{
	ACTOR *source = fdActorGet(torchId);
	ACTOR *actor = fdActorGet(sparkleId);

	scFieldActorSetPosAndResetMotion(
		actor, source->x.fixed, source->y.fixed, source->z.fixed);
	fdActorStartScript(actor, path);
	fdActorSetSprFlags(actor, 0);
	actor->motionFlg = 0;
	fdActorSelectAnimation(actor, 1);
	actor->watchedActor = (FIELD_OBJECT *)source;
	actor->tmr = 0;
	actor->userValue = facing;
	actor->launchedActorId = fireballId;
	actor->update = (OBJECT_UPDATE)scActorHoldDuringScript;
	/* UB: no value is returned, though the callers take one. */
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

#include "shared/actor_launch_ahead.h"

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

extern Sint16 SlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

Sint32		  fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);
extern Sint16 SlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

void   fdActorSteerByPad(ACTOR *actor);
Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

#include "shared/slide_on_ice_from_edge.c"

/* Stops the leader's slide on the ice where it is (nonzero), or lets it run. */
void scLeaderSetSlideStopped(Sint32 value)
{
	SlideOnIce_Stopped = value;
}

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/field_effect_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_BURST_ICE_SHEET
extern Sint32 FieldEffect_DurationScripts[];

/* Puts a short-lived effect sprite, dust unless `options` names another, at a
   point, moving off at the velocity given and drawn at the leader's priority:
   this overlay's own copy of field_effect_spawn.c. */
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options)
{
	ACTOR		 *party = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	FIELD_SPR	 *sprite;
	const Sint32 *script;

	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	sprite = effect->spr;
	fdActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(
		effect, FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	fdActorSetSprFlags(effect, 0);
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	fdEffectSetSprPriority(effect, ((FIELD_SPR *)party->spr)->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flags & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flags & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		fdEffectSetSprPriority(effect, options->pri);
	}
	if((flags & EFFECT_USE_START_SCALE) != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flags & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED  target;
		Sint32 frames;

		script = FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
		target = options->tgtScaleX;
		frames = script[3];
		if((flags & EFFECT_USE_START_SCALE) != 0) {
			effect->scaleRateX = (target - effect->scaleX) / frames;
			effect->scaleRateY =
				(options->tgtScaleY - effect->scaleY) / script[3];
		} else {
			effect->scaleRateX = (target - toFIXED(1)) / frames;
			effect->scaleRateY = (options->tgtScaleY - toFIXED(1)) / script[3];
		}
	}
	if((flags & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation(effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flags & EFFECT_USE_ROTATION) != 0)
		sprite->rotation = options->rotation;
	if((flags & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flags & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

/* A cracked tile gives way under Felix, who drops through it. */
void scCrackedTileFall(METATILE_TARGET *table, METATILE_TARGET_ENTRY *entry)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x;
	Sint32 z;
	Sint32 mask;

	mask = MAPGRID_SIZE / 2;
	x = FIELD_METATILE(actor->x.fixed);
	x |= (Sint16)(table->x & mask);
	z = FIELD_METATILE(actor->z.fixed);
	z |= (Sint16)(table->z & mask);
	scLeaderCenterInMetatile();
	fdScriptPlaySnd(SE_SPLASH);
	fdMapGridCopyMetatiles(table->x, table->z + 2, 1, 1, x, z);
	syFieldScriptWait(12);
	actor->facing = FACING_SOUTH;
	actor->motionFlg = 0;
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	{
		Uint32 winout = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
			WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
			WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3;

		REG_WINOUT = winout;
	}
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	table->shown = 1;
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(16);
	for(x = 0; x < SINK_FRAMES; x++) {
		if(x == 5)
			fdScriptPlaySnd(SE_DUST_PUFF);
		actor->scaleX -= toFIXED(1) / 64;
		actor->scaleY -= toFIXED(3) / 256;
		actor->y.fixed -= toFIXED(6) / 10;
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	table->shown = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
}

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY
void fdTeleportPlayWhirl(void);

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flag, Sint32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

#include "shared/teleport_shimmer.c"

#include "shared/actors_set_metatiles_open.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_metatile_open.h"

/* clang-format off */
/* The flame chamber's two moving dragon heads, each sliding along the north */
/* wall and pausing at each end. */
Sint32 MarsLighthouseEntrance_LapisWestFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(200, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(104, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 MarsLighthouseEntrance_LapisEastFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(232, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(296, 0, 112),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The dragon head in Balm's rooms, raised on its wall, sliding back and */
/* forth above the urn torch. */
Sint32 MarsLighthouseEntrance_BalmFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(48)),
	SC_SET(SW_GROUND, toFIXED(48)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(904, 0, 528),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(840, 0, 528),
	SC_SYNC,
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The Valkyrie Mail rooms' dragon heads: the one on the south circle's */
/* room's wall slides back and forth over the urn torch; the long hall's */
/* dragon face stays raised on its wall until it wakes. */
Sint32 MarsLighthouseEntrance_ValkyrieWestFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_Y, toFIXED(48)),
	SC_SET(SW_GROUND, toFIXED(48)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_WAIT(10),
	SC_MOVE(88, 0, 688),
	SC_SYNC,
	SC_WAIT(30),
	SC_LABEL(1),
	SC_MOVE(152, 0, 688),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(88, 0, 688),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

Sint32 MarsLighthouseEntrance_ValkyrieEastFlameThrowerRoute[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, toFIXED(32)),
	SC_SET(SW_GROUND, toFIXED(32)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_END,
};

/* The woken dragon face crawls west along the long hall's north wall, rising */
/* and dropping as it goes, and stops breathing fire at the west end. */
Sint32 MarsLighthouseEntrance_DragonFaceGoesLeft[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, toFIXED(32)),
	SC_SET(SW_GROUND, toFIXED(32)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(808, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(808, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(728, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(728, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(712, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(712, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(616, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(616, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(552, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(552, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(536, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(536, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(520, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(520, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(296, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(296, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(264, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(264, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(136, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(136, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(120, 32, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(120, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_MOVE(56, 48, 128),
	SC_SYNC,
	SC_WAIT(60),
	SC_CALL(scMarsLighthouseEntranceDryDragonFace),
	SC_END,
};

/* The dragon face already at the long hall's west end, as Felix comes back. */
Sint32 MarsLighthouseEntrance_DragonFaceAtLeft[] = {
	SC_SET(SW_STEERING, 0),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_X, toFIXED(56)),
	SC_SET(SW_Z, toFIXED(128)),
	SC_SET(SW_Y, toFIXED(48)),
	SC_SET(SW_GROUND, toFIXED(48)),
	SC_SPEED(0.5),
	SC_ACCELERATION(0.03125),
	SC_MOVE(56, 48, 128),
	SC_SYNC,
	SC_WAIT(20),
	SC_END,
};

#include "shared/debris_script.c"

/* Each room's ice cones (scMarsLighthouseEntrancePlaceIceCones), and its */
/* flame throwers for scFlameThrowersStart, each with its flame's kind and */
/* reach, with the actors their flames stop at, 1 where reaching one calls */
/* the function scFlameThrowersStart is given. */
Uint16 MarsLighthouseEntrance_FugueIceCones[] = {
	ACTOR_FUGUE_ICE_CONE_A, 7, FLAG_SCENE_TEMP_00,
	ACTOR_FUGUE_ICE_CONE_B, 7, FLAG_SCENE_TEMP_01,
	ACTOR_LIST_END,
};
Uint16 MarsLighthouseEntrance_LapisBeamTargets[] = {
	ACTOR_LAPIS_FLAME_THROWER_A, FLAME_FORWARD | 3, ACTOR_LAPIS_FLAME_THROWER_B, FLAME_FORWARD | 3,
	ACTOR_LAPIS_FLAME_THROWER_C, FLAME_FORWARD | 3, ACTOR_LAPIS_FLAME_THROWER_D, FLAME_FORWARD | 3,
	0,
};
Uint16 MarsLighthouseEntrance_LapisBeamStops[] = {
	ACTOR_PUSHED_STATUE, 0, 0,
};
Uint16 MarsLighthouseEntrance_LapisIceCones[] = {
	ACTOR_LAPIS_ICE_CONE_A, 7, FLAG_SCENE_TEMP_02,
	ACTOR_LAPIS_ICE_CONE_B, 7, FLAG_SCENE_TEMP_03,
	ACTOR_LIST_END,
};
Uint16 MarsLighthouseEntrance_BalmBeamTargets[] = {
	ACTOR_BALM_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};
Uint16 MarsLighthouseEntrance_BalmBeamStops[] = {
	ACTOR_BALM_TORCH, 1, 0,
};
Uint16 MarsLighthouseEntrance_BalmIceCones[] = {
	ACTOR_BALM_ICE_CONE_A, 7, FLAG_SCENE_TEMP_04,
	ACTOR_BALM_ICE_CONE_B, 7, FLAG_SCENE_TEMP_05,
	ACTOR_BALM_ICE_CONE_C, 7, FLAG_SCENE_TEMP_06,
	ACTOR_BALM_ICE_CONE_D, 7, FLAG_SCENE_TEMP_07,
	ACTOR_BALM_ICE_CONE_E, 7, FLAG_SCENE_TEMP_08,
	ACTOR_LIST_END,
};
Uint16 MarsLighthouseEntrance_ValkyrieUpperBeamTargets[] = {
	ACTOR_UPPER_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};
Uint16 MarsLighthouseEntrance_ValkyrieUpperBeamStops[] = {
	ACTOR_PUSHED_STATUE, 0, ACTOR_UPPER_TORCH, 1, 0,
};
Uint16 MarsLighthouseEntrance_ValkyrieLowerBeamTargets[] = {
	ACTOR_DRAGON_FACE, FLAME_THROWER_DRY | 3, 0,
};
Uint16 MarsLighthouseEntrance_ValkyrieLowerBeamStops[] = {
	ACTOR_LOWER_TORCH, 1, 0,
};
Uint16 MarsLighthouseEntrance_ValkyrieIceCones[] = {
	ACTOR_VALKYRIE_ICE_CONE_A, 7, FLAG_SCENE_TEMP_10,
	ACTOR_VALKYRIE_ICE_CONE_B, 7, FLAG_SCENE_TEMP_11,
	ACTOR_VALKYRIE_ICE_CONE_C, 7, FLAG_SCENE_TEMP_12,
	ACTOR_VALKYRIE_ICE_CONE_D, 7, FLAG_SCENE_TEMP_13,
	ACTOR_VALKYRIE_ICE_CONE_E, 7, FLAG_SCENE_TEMP_14,
	ACTOR_VALKYRIE_ICE_CONE_F, 7, FLAG_SCENE_TEMP_15,
	ACTOR_LIST_END,
};

/* The sparkles' ways from each urn torch to where its fireball sets off; at */
/* the end of some, the function the fireball's arrival calls. */
Sint32 MarsLighthouseEntrance_ValkyrieUpperFireballPath[] = {
	SC_CALL(scActorPopUpSlow),
	SC_MOVE(112, 32, 728),
	SC_SYNC,
	SC_MOVE(112, 22, 728),
	SC_SYNC,
	SC_MOVE(128, 22, 728),
	SC_SYNC,
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseEntrance_ValkyrieLowerFireballPath[] = {
	SC_CALL(scActorPopUpSlow),
	SC_MOVE(80, 32, 168),
	SC_SYNC,
	SC_MOVE(80, 22, 168),
	SC_SYNC,
	SC_MOVE(96, 22, 168),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseEntranceClearFireballRow),
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseEntrance_BalmFireballPath[] = {
	SC_CALL(scActorPopUpSlow),
	SC_MOVE(872, 32, 576),
	SC_SYNC,
	SC_MOVE(872, 30, 576),
	SC_SYNC,
	SC_MOVE(872, 30, 584),
	SC_SYNC,
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseEntrance_FireballToHiddenCircleIceSheet[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(96, 32, 680),
	SC_SYNC,
	SC_MOVE(96, 22, 680),
	SC_SYNC,
	SC_MOVE(112, 22, 680),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseEntranceCrackHiddenCircleIceSheet),
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseEntrance_FireballThroughIce[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(856, 32, 136),
	SC_SYNC,
	SC_MOVE(872, 32, 136),
	SC_SYNC,
	SC_MOVE(872, 22, 136),
	SC_SYNC,
	SC_MOVE(872, 22, 152),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseEntranceMeltIceInPath),
	SC_CALL(scActorPark),
	SC_END,
};

Sint32 MarsLighthouseEntrance_FireballToLapisIceSheet[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(112, 32, 680),
	SC_SYNC,
	SC_MOVE(112, 22, 680),
	SC_SYNC,
	SC_MOVE(128, 22, 680),
	SC_SYNC,
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseEntranceCrackLapisIceSheet),
	SC_CALL(scActorPark),
	SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/field_effect_duration_scripts.c"

#define TELEPORT_CIRCLE_FIRST_ENTRY 137
#include "shared/teleport_circle_palette.c"

Sint32 MarsLighthouseEntrance_TorchScript[] = {
	SC_CALL(scMarsLighthouseEntranceSetUpTorch),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(MarsLighthouseOutside),
	SCENE_EXIT(MARS_LIGHTHOUSE_OUTSIDE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MARS_LIGHTHOUSE_OUTSIDE),
	SCENE_EXIT(MARS_LIGHTHOUSE_OUTSIDE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_OUTSIDE),
	SCENE_EXITS(MarsLighthouseEntranceFugue),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_WEST, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE),
	SCENE_EXIT(3, MarsLighthouseEntranceFugue, 4),
	SCENE_EXIT(4, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_A),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_WEST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_EAST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST),
	SCENE_EXIT(7, MarsLighthouseEntranceFugue, 8),
	SCENE_EXIT(8, MarsLighthouseEntranceFugue, 7),
	SCENE_EXIT(9, MarsLighthouseEntranceFugue, 10),
	SCENE_EXIT(10, MarsLighthouseEntranceFugue, 9),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_FUGUE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_B, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST),
	SCENE_EXITS(MarsLighthouseEntranceTeleportLapis),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_WEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_OUTSIDE, MarsLighthouseOutside, MARS_LIGHTHOUSE_OUTSIDE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTH),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTHEAST, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTHEAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS),
	SCENE_EXIT(6, MarsLighthouseEntranceTeleportLapis, 7),
	SCENE_EXIT(7, MarsLighthouseEntranceTeleportLapis, 6),
	SCENE_EXIT(8, MarsLighthouseEntranceTeleportLapis, 9),
	SCENE_EXIT(9, MarsLighthouseEntranceTeleportLapis, 8),
	SCENE_EXIT(10, MarsLighthouseEntranceTeleportLapis, 11),
	SCENE_EXIT(11, MarsLighthouseEntranceTeleportLapis, 10),
	SCENE_EXIT(12, MarsLighthouseEntranceTeleportLapis, 13),
	SCENE_EXIT(13, MarsLighthouseEntranceTeleportLapis, 12),
	SCENE_EXIT(14, MarsLighthouseEntranceTeleportLapis, 15),
	SCENE_EXIT(15, MarsLighthouseEntranceTeleportLapis, 14),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_SOUTH),
	SCENE_EXIT(17, MarsLighthouseEntranceTeleportLapis, 18),
	SCENE_EXIT(18, MarsLighthouseEntranceTeleportLapis, 17),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_EAST_A),
	SCENE_EXITS(MarsLighthouseEntranceBalm),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_A, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_A),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTH, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_NORTHEAST, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTHEAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_WEST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_WEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE_EAST, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_EAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM),
	SCENE_EXIT(8, MarsLighthouseEntranceBalm, 9),
	SCENE_EXIT(9, MarsLighthouseEntranceBalm, 8),
	SCENE_EXIT(10, MarsLighthouseEntranceBalm, 11),
	SCENE_EXIT(11, MarsLighthouseEntranceBalm, 10),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B, MarsLighthouseEntranceHiddenCircle, MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_SOUTH, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH),
	SCENE_EXITS(MarsLighthouseEntranceHiddenCircle),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_SOUTH, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_SOUTH),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_A, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_A),
	SCENE_EXIT(3, MarsLighthouseEntranceHiddenCircle, 4),
	SCENE_EXIT(4, MarsLighthouseEntranceHiddenCircle, 3),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS, MarsLighthouseEntranceTeleportLapis, MARS_LIGHTHOUSE_ENTRANCE_TELEPORT_LAPIS_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE),
	SCENE_EXIT(6, MarsLighthouseEntranceHiddenCircle, 7),
	SCENE_EXIT(7, MarsLighthouseEntranceHiddenCircle, 6),
	SCENE_EXIT(8, MarsLighthouseEntranceHiddenCircle, 9),
	SCENE_EXIT(9, MarsLighthouseEntranceHiddenCircle, 8),
	SCENE_EXIT(10, MarsLighthouseEntranceHiddenCircle, 11),
	SCENE_EXIT(11, MarsLighthouseEntranceHiddenCircle, 10),
	SCENE_EXIT(12, MarsLighthouseEntranceHiddenCircle, 13),
	SCENE_EXIT(13, MarsLighthouseEntranceHiddenCircle, 12),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B),
	SCENE_EXITS(MarsLighthouseEntranceValkyrieMailChest),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST),
	SCENE_EXIT(2, MarsLighthouseEntranceValkyrieMailChest, 3),
	SCENE_EXIT(3, MarsLighthouseEntranceValkyrieMailChest, 2),
	SCENE_EXIT(4, MarsLighthouseEntranceValkyrieMailChest, 5),
	SCENE_EXIT(5, MarsLighthouseEntranceValkyrieMailChest, 4),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM, MarsLighthouseEntranceBalm, MARS_LIGHTHOUSE_ENTRANCE_BALM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_EAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_EAST),
	SCENE_EXIT(10, MarsLighthouseEntranceValkyrieMailChest, 11),
	SCENE_EXIT(11, MarsLighthouseEntranceValkyrieMailChest, 10),
	SCENE_EXIT(12, MarsLighthouseEntranceValkyrieMailChest, 13),
	SCENE_EXIT(13, MarsLighthouseEntranceValkyrieMailChest, 12),
	SCENE_EXIT(14, MarsLighthouseEntranceValkyrieMailChest, 15),
	SCENE_EXIT(15, MarsLighthouseEntranceValkyrieMailChest, 14),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_WEST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_WEST),
	SCENE_EXITS(MarsLighthouseEntranceSolBladeChest),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST),
	SCENE_EXIT(2, MarsLighthouseEntranceSolBladeChest, 3),
	SCENE_EXIT(3, MarsLighthouseEntranceSolBladeChest, 2),
	SCENE_EXIT(4, MarsLighthouseEntranceSolBladeChest, 6),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST),
	SCENE_EXIT(6, MarsLighthouseEntranceSolBladeChest, 4),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A),
	SCENE_EXIT(MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_B, MarsLighthouseStarRoom, MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_B),
	SCENE_EXITS(MarsLighthouseStarRoom),
	SCENE_EXIT(1, MarsLighthouseStarRoom, 2),
	SCENE_EXIT(2, MarsLighthouseStarRoom, 1),
	SCENE_EXIT(3, MarsLighthouseStarRoom, 4),
	SCENE_EXIT(4, MarsLighthouseStarRoom, 3),
	SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
	SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A, MarsLighthouseEntranceSolBladeChest, MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A),
	SCENE_EXIT(MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST, MarsLighthouseEntranceValkyrieMailChest, MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST),
	SCENE_EXITS_END,
};

Sint32 MarsLighthouseEntrance_IceConeScript[] = {
	SC_ANIMATION(7),
	SC_END,
};

ACTOR_PLACE MarsLighthouseEntrance_Placements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseEntrance_OutsidePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 280, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 280, FACING_WEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

ACTOR_PLACE MarsLighthouseEntrance_FuguePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
		MarsLighthouseEntrance_IceConeScript, 0, 0, 0, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
		MarsLighthouseEntrance_IceConeScript, 136, 0, 424, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, FLAG_AREA_TEMP_20,
		MOVEMENT_TYPE_STAND, 648, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
#if !FUGUE_NEW_POSITION
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FUGUE),
		MOVEMENT_TYPE_STAND, 664, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
#else
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FUGUE),
		MOVEMENT_TYPE_STAND, 728, 0, 408, FACING_NORTHWEST, TALK_FACING_PARTY,
		0),
#endif
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE MarsLighthouseEntrance_LapisPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseEntrance_LapisWestFlameThrowerRoute, 104, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseEntrance_LapisEastFlameThrowerRoute, 280, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 88, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 552, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 552, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 840, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 704, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE MarsLighthouseEntrance_BalmPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseEntrance_BalmFlameThrowerRoute, 840, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 872, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 792, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 792, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 840, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 904, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 840, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 936, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 904, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_BALM), MOVEMENT_TYPE_STAND, 88, 0, 904, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE MarsLighthouseEntrance_HiddenCirclePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 72, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 72, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 904, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 856, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 240, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE MarsLighthouseEntrance_ValkyriePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseEntrance_ValkyrieWestFlameThrowerRoute, 120, 0, 688, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 88, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MarsLighthouseEntrance_ValkyrieEastFlameThrowerRoute, 792, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (Sint32)MarsLighthouseEntrance_TorchScript, 56, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 200, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 424, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 664, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MarsLighthouseEntrance_IceConeScript, 760, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

/* clang-format off */
ACTOR_PLACE MarsLighthouseEntrance_SolBladePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE MarsLighthouseEntrance_Cues[] = {
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_OutsideCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_TABLE_END},
};
/* clang-format on */

CUE MarsLighthouseEntrance_FugueCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT, 16, CONDITION_ALWAYS, 16},
	{TALK_CUE_FACING(FACING_WEST), ACTOR_FUGUE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceFightFugue)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_FUGUE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceFightFugue)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_FUGUE_ICE_CONE_A, FLAG_SCENE_TEMP_00,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_FUGUE_ICE_CONE_B, FLAG_SCENE_TEMP_01,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_ICE_SHAFT, FLAG_ICE_SHAFT_GROUND,
		SCRIPT_REF(scMarsLighthouseEntranceGrindIce)},
	{CUE_TALK, ACTOR_ICE_SHAFT, FLAG_ICE_SHAFT_GROUND,
		SCRIPT_REF(scMarsLighthouseEntranceCheckIce)},
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_LapisCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 16, CONDITION_ALWAYS, 16},
	{CUE_STEP, STEP_CUE_FLAME_HIT, CONDITION_ALWAYS,
		SCRIPT_REF(fdLeaderKnockBack)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_LAPIS_ICE_CONE_A, FLAG_SCENE_TEMP_02,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_LAPIS_ICE_CONE_B, FLAG_SCENE_TEMP_03,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		LAPIS_SOUTH_CIRCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceTeleportFromLapisSouthCircle)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		LAPIS_NORTH_CIRCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceTeleportFromLapisNorthCircle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
		FLAG_LAPIS_STATUE_BLOCKS_FLAMES,
		SCRIPT_REF(scMarsLighthouseEntrancePushStatueIntoFlames)},
	{CUE_PUSHED, 0, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntrancePushStatueIntoFlames)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_CRACKED_ICE_WALL,
		IF_FLAG_SET(FLAG_LAPIS_SHEET_CRACKED),
		SCRIPT_REF(scMarsLighthouseEntranceBurstIceSheet)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_BIG_ICE_SHEET,
		IF_FLAG_SET(FLAG_LAPIS_SECOND_SHEET_SHOWN),
		SCRIPT_REF(MarsLighthouseEntrance_BurstLapisIceSheet)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_LIT_TORCH, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
		ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceBlazeLapisTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
		ACTOR_LAPIS_TORCH_A, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceWatchFireballCrackIce)},
	{CUE_TALK, ACTOR_TELEPORT_LAPIS_CHEST, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceBlockRetreatAtChest)},
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_BalmCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
	{CUE_STEP, METATILE_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
		SCRIPT_REF(fdLeaderSlideOnIce)},
	{CUE_STEP, STEP_CUE_FIREBALL_HIT, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceRideFireball)},
	{CUE_TALK, ACTOR_BALM, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceFightBalm)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_WEST_STATUE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceSlideStatueOnIce)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BALM_EAST_STATUE, CONDITION_ALWAYS,
		0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_ICE_CONE_A, FLAG_SCENE_TEMP_04,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_ICE_CONE_B, FLAG_SCENE_TEMP_05,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_ICE_CONE_C, FLAG_SCENE_TEMP_06,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_ICE_CONE_D, FLAG_SCENE_TEMP_07,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_BALM_ICE_CONE_E, FLAG_SCENE_TEMP_08,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		MARS_LIGHTHOUSE_ENTRANCE_BALM_TO_MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_NORTH_B,
		CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseEntranceTeleportFromBalm)},
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_HiddenCircleCues[] = {
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_STEP, METATILE_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
		SCRIPT_REF(fdLeaderSlideOnIce)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_WEST_LIT_TORCH, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
		ACTOR_WEST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceBlazeHiddenCircleWestTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
		ACTOR_WEST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceWatchFireballCrackHiddenCircleIce)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_EAST_LIT_TORCH, CONDITION_ALWAYS, 0},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
		ACTOR_EAST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceBlazeHiddenCircleEastTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
		ACTOR_EAST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceWatchFireballMeltIce)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_BEHAVIOR_CRACKED_ICE_WALL,
		IF_FLAG_SET(FLAG_HIDDEN_CIRCLE_SHEET_SHOWN),
		SCRIPT_REF(MarsLighthouseEntrance_BurstHiddenCircleIceSheet)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		MARS_LIGHTHOUSE_ENTRANCE_HIDDEN_CIRCLE_TO_MARS_LIGHTHOUSE_ENTRANCE_BALM_NORTH_B,
		CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceTeleportFromHiddenCircle)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceRevealTeleportCircle)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceHideTeleportCircle)},
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_ValkyrieCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 15, CONDITION_ALWAYS, 15},
	{CUE_STEP,
		MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_EAST,
		CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseEntranceFallIntoHole)},
	{CUE_STEP,
		MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_TO_MARS_LIGHTHOUSE_STAR_ROOM_WEST,
		CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseEntranceFallIntoHole)},
	{CUE_STEP, METATILE_BEHAVIOR_WAKE_DRAGON_FACE, FLAG_DRAGON_FACE_AWAKE,
		SCRIPT_REF(scMarsLighthouseEntranceWakeDragonFace)},
	{CUE_STEP, METATILE_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceSlideFromEdge)},
	{CUE_STEP, STEP_CUE_FIREBALL_HIT, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceRideFireball)},
	{CUE_STEP, VALKYRIE_SOUTH_CIRCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceRideFireballIfHit)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_A, FLAG_SCENE_TEMP_10,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_B, FLAG_SCENE_TEMP_11,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_C, FLAG_SCENE_TEMP_12,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_D, FLAG_SCENE_TEMP_13,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_E, FLAG_SCENE_TEMP_14,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
		ACTOR_VALKYRIE_ICE_CONE_F, FLAG_SCENE_TEMP_15,
		SCRIPT_REF(scMarsLighthouseEntranceStartPoundIceCone)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		VALKYRIE_NORTH_CIRCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceTeleportFromValkyrieNorth)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
		VALKYRIE_SOUTH_CIRCLE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceTeleportFromValkyrieSouth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE,
		FLAG_VALKYRIE_STATUE_BLOCKS_FLAMES,
		SCRIPT_REF(scMarsLighthouseEntrancePushStatueIntoValkyrieFlames)},
	{CUE_TABLE_END},
};

CUE MarsLighthouseEntrance_SolBladeCues[] = {
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT, 16, CONDITION_ALWAYS, 16},
	{CUE_STEP, METATILE_BEHAVIOR_BLACK_ICE, CONDITION_ALWAYS,
		SCRIPT_REF(fdLeaderSlideOnIce)},
	{CUE_STEP, METATILE_BEHAVIOR_CRACKED_ICE_TILE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceCheckCrackedFloor)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_PILLAR_TOP, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseEntranceJumpOrSlide)},
	{CUE_TABLE_END},
};

/* 1 while the dragon face at the west end of the long hall breathes fire. */
Sint16 MarsLighthouseEntrance_DragonFireOn = 0;

Sint32 FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};
