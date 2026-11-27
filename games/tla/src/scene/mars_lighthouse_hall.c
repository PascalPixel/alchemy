/* @file */
/* The Mars Lighthouse's central hall, the four ledges in the rain that lead */
/* from it to the element towers, and the beacon ledge at the top of each */
/* tower, all sharing this overlay. */
/* */
/* Felix reaches the hall up the stairs from the lighthouse's entrance. The */
/* great dragon statue at its head speaks as he steps in: without the Mars */
/* Star it tells him to begone; with it, once the lighthouse has thawed, it */
/* bids him set the four spirits aflame. Four inscriptions hang beside the */
/* statue, two on the floor by its chin and two on the gallery by its head, */
/* naming the peoples who ruled earth, water, fire and wind. Each tower comes */
/* out on a beacon ledge in the rain, where Blaze at the torch lights that */
/* element's flame. Each flame lit burns on one of the hall's four robed */
/* statues and makes its element's inscription glow. With all four lit the */
/* statue says the heavens await, a fireball from its mouth breaks the floor */
/* before it to show a round carving, and Teleport there carries the party up */
/* to the aerie. Once the lighthouse has thawed, the gallery's arched doorways
 */
/* light purple as Felix walks up to them. */

#include "graphics/palette_buffers.h"
#include "item_ids.h"
#include "scene/flame_throwers.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "version.h"
#include "shared/event_check_spheres_overlap.h"
#include "window/window.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/* All four flames lit: the floor before the statue has broken open. */
#define FLAG_WAY_TO_AERIE_OPEN 0xa31
#define FLAG_VENUS_FLAME_LIT   0xa32
#define FLAG_MERCURY_FLAME_LIT 0xa33
#define FLAG_MARS_FLAME_LIT	   0xa34
#define FLAG_JUPITER_FLAME_LIT 0xa35
/* "If you wish to scale the heavens, set the four spirits aflame!" */
#define FLAG_TOLD_TO_LIGHT_FLAMES 0xa2e
/* "If you do not hold the star, you shall not ascend to the heavens... */
/* Begone!" */
#define FLAG_TOLD_TO_BEGONE 0xa2f

/* The hall's own entrances, through its four doorways: arriving from the */
/* passage toward each element's tower, the party comes in under that */
/* doorway's arch with its light shown. */
enum {
	ENTRANCE_THROUGH_VENUS_DOORWAY = 8,
	ENTRANCE_THROUGH_MERCURY_DOORWAY = 10,
	ENTRANCE_THROUGH_MARS_DOORWAY = 12,
	ENTRANCE_THROUGH_JUPITER_DOORWAY = 14
};

/* The step cells before the hall's doorways, two to each in the doorways' */
/* order: the cell under the arch, which fades its light in, then the cells */
/* beside it, which fade it out. */
enum {
	CELL_BEHAVIOR_VENUS_DOORWAY = 20,
	CELL_BEHAVIOR_MERCURY_DOORWAY = 22,
	CELL_BEHAVIOR_MARS_DOORWAY = 24,
	CELL_BEHAVIOR_JUPITER_DOORWAY = 26
};

/* The hall's doorways, in the order of their step cells */
/* (MarsLighthouseHall_Doorway). */
enum { DOORWAY_VENUS, DOORWAY_MERCURY, DOORWAY_MARS, DOORWAY_JUPITER };

/* The hall's other cells: the carving Teleport is cast on, the four */
/* inscriptions, and the steps up from the stairs where the dragon statue */
/* speaks. */
enum {
	CELL_AERIE_CARVING = 1,
	CELL_EARTH_INSCRIPTION = 30,
	CELL_WATER_INSCRIPTION,
	CELL_FIRE_INSCRIPTION,
	CELL_WIND_INSCRIPTION,
	CELL_BEFORE_DRAGON_STATUE
};

/* The beacon ledge's doorway back into its tower. */
#define CELL_LEDGE_DOORWAY 1

/* The inscriptions' colours: BG palettes 8 and 9 hold six for each element, */
/* the hall's four inscriptions glowing in them once that element's flame is */
/* lit, and the ledge's inscription takes its tower's six into palette 13. */
#define INSCRIPTION_FIRST_PALETTE 8
#define LEDGE_INSCRIPTION_PALETTE 13
/* The hall's palette whose last six colours are an unlit inscription's. */
#define UNLIT_INSCRIPTION_PALETTE 2
/* An inscription's glow: a run of INSCRIPTION_GLOW_COLORS colours that turns */
/* round, one colour on every INSCRIPTION_GLOW_STEP_FRAMES frames. */
#define INSCRIPTION_GLOW_COLORS		 6
#define INSCRIPTION_GLOW_STEP_FRAMES 8
/* The first of each element's six colours in palettes 8 and 9, and of the */
/* ledge inscription's in palette 13. */
enum {
	JUPITER_GLOW = 4,
	VENUS_GLOW = 10,
	MERCURY_GLOW = PALETTE_COLORS + 4,
	MARS_GLOW = PALETTE_COLORS + 10,
	LEDGE_GLOW = 10
};

/* Each element's flame among the flame sprite's animations. */
enum {
	FLAME_ANIMATION_MARS = 2,
	FLAME_ANIMATION_JUPITER = 3,
	FLAME_ANIMATION_VENUS = 4,
	FLAME_ANIMATION_MERCURY = 5
};

/* The beacon ledge: the torch Blaze is cast at, tinted to its tower's element,
 */
/* its flame, the point above it Blaze reaches, and two dragon statues. */
enum {
	ACTOR_LEDGE_TORCH = 8,
	ACTOR_LEDGE_FLAME = 9,
	ACTOR_LEDGE_BLAZE_TARGET = 10
};

/* The hall: four robed statues (actors 8 to 11), the four elements' flames on
 */
/* them, shown once lit, and the dragon statue's mouth, which lets fall the */
/* fireball that breaks the floor. */
enum {
	ACTOR_VENUS_FLAME = 12,
	ACTOR_MERCURY_FLAME = 13,
	ACTOR_MARS_FLAME = 14,
	ACTOR_JUPITER_FLAME = 15,
	ACTOR_STATUE_MOUTH = 16,
	ACTOR_FIREBALL = 18
};

struct ACTOR;
struct VECTOR3;

s32 fdLeaderKnockBack(void);

s32 scFireballCarryLeader(void);
void fdActorLaunchAhead(ACTOR *self);

void scTeleportStartShimmer(void);

void scMarsLighthouseHallFadeDoorway(void);

void scMarsLighthouseHallBreakFloorTile(s32 x, s32 z, s32 sourceX, s32 sourceY,
										s32 cellX, s32 cellY);

void scMarsLighthouseHallCycleLedgeInscription(void);

void scMarsLighthouseHallCycleHallInscriptions(void);

void scMarsLighthouseHallStartDoorways(s32 entrance);

void scFlameThrowersCreate(void);

void scMarsLighthouseHallFlashDoorways(void);

void fdActorBlink(ACTOR *actor);

extern u32 Crumbs_Script[];
extern u32 IceChips_Script[];

extern s32 MarsLighthouseHall_TorchScript[];

void fdTeleportPlaySparks(void);
/* The colours of BG palettes 8 and 9. */
typedef struct HALL_COL {
	u16 cols[2 * PALETTE_COLORS];
} HALL_COL;
/* The teleport circle's shimmer, a palette cycle that fdPaletteCycleFade */
/* steps. */
u16 PaletteCycleBright BSS_DATA;
u16 *PaletteCycleSlots BSS_DATA;
u16 PaletteCycleTimer BSS_DATA;
u16 PaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *PaletteSlots BSS_DATA;
u16 *PaletteColors BSS_DATA;
u16 *PaletteCycleColors BSS_DATA;
u16 PaletteCycleLength BSS_DATA;
u16 PaletteCycleIndex BSS_DATA ALIGNED(4);
/* The beacon ledge inscription's colours: the element palettes as loaded, the
 */
/* palette the glow is built in, and the inscription's own palette as loaded. */
u16 MarsLighthouseHall_LedgeElementColors[2 * PALETTE_COLORS] BSS_DATA
	ALIGNED(4);
u16 MarsLighthouseHall_LedgePalette[2 * PALETTE_COLORS] BSS_DATA;
u16 MarsLighthouseHall_LedgeSavedPalette[PALETTE_COLORS] BSS_DATA;
HALL_COL MarsLighthouseHall_HallElementColors BSS_DATA;
HALL_COL MarsLighthouseHall_HallPalette BSS_DATA;
/* BG palette 2's first ten colours; it is saved whole, with the unlit */
/* inscription colours after them. */
u16 MarsLighthouseHall_HallSavedPalette[10] BSS_DATA;
u16 MarsLighthouseHall_UnlitInscriptionColors[6] BSS_DATA;
s16 MarsLighthouseHall_DoorwayBlend BSS_DATA;
s16 MarsLighthouseHall_DoorwayBlendTarget BSS_DATA;
s16 MarsLighthouseHall_Doorway BSS_DATA;
s16 MarsLighthouseHall_DoorwayStep BSS_DATA;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

void scTeleportStopShimmer(void);

/* A script call of the beacon ledge's torch and of Blaze's mark above it: */
/* they stand without a shadow. */
s32 scMarsLighthouseHallSetUpTorch(ACTOR *actor) {
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* Shows a flame on its torch, 14 pixels up, in its element's animation. */
s32 scMarsLighthouseHallShowFlame(s32 id, s32 animation) {
	ACTOR *actor = fdActorGet(id);

	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(14);
	fdActorSetSprFlags(actor, 0);
	fdActorSelectAnimation(actor, animation);
#ifdef UBFIX
	return 0;
#endif
}

void syFlameUpdateForward(ACTOR *actor);
void fdActorSetupScriptedPart(ACTOR *actor, s32 layer);
void syFlameUpdateSideways(ACTOR *actor);

extern s32 ScriptedPart_Script[];
extern ACTOR_PLACE MarsLighthouseHall_Placements[];
extern ACTOR_PLACE MarsLighthouseHall_FlamePlacements[];
extern ACTOR_PLACE MarsLighthouseHall_InscriptionsPlacements[];

extern TRIGGER MarsLighthouseHall_Triggers[];
extern TRIGGER MarsLighthouseHall_FlameTriggers[];
extern TRIGGER MarsLighthouseHall_InscriptionsTriggers[];
extern TRIGGER MarsLighthouseHall_LedgesTriggers[];
extern s32 MarsLighthouseHall_FlamePuffSwellScript[];
extern s32 MarsLighthouseHall_FlamePuffThrownScript[];

s32 syListCountEntries(u16 *list);
void fdPaletteCycleFade(void);
void scTeleportStopShimmer(void);
void scTeleportStartFastShimmer(void);
s32 fdActorIsAbove(ACTOR *a, ACTOR *b);
void scFlameThrowerUpdateSpark(ACTOR *actor);
void fdActorSetupScripted(ACTOR *actor);
void scMarsLighthouseHallSpawnFlamePuff(s32 x, s32 z, s32 kind);
void scActorHoldDuringEvent(ACTOR *actor);
void fdTeleportPlayWhirl(void);

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE)
		return MarsLighthouseHall_FlamePlacements;
	if(scene == SCENE_MARS_LIGHTHOUSE_HALL)
		return MarsLighthouseHall_InscriptionsPlacements;
	return MarsLighthouseHall_Placements;
}

extern s32 MarsLighthouseHall_LedgeCycleFrame;

/* Each frame on the beacon ledge: once its flame is lit, the inscription on */
/* the wall behind it glows in its element's colours, turning round. */
void scMarsLighthouseHallCycleLedgeInscription(void) {
	s32 entrance = MainRoutineStateWork.entrance;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 base;
	s32 i;
	s8 busy;

	if(state->psynergy != 0)
		return;
	busy = state->revealActive;
	if(busy != 0)
		return;
	if(MarsLighthouseHall_LedgeCycleFrame == -1) {
		syWordsCopyEntry(MarsLighthouseHall_LedgeSavedPalette,
						 (void *)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
						 sizeof(MarsLighthouseHall_LedgeSavedPalette));
		syWordsCopyEntry(MarsLighthouseHall_LedgeElementColors,
						 (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
						 sizeof(MarsLighthouseHall_LedgeElementColors));
		syWordsCopyEntry(MarsLighthouseHall_LedgePalette,
						 MarsLighthouseHall_LedgeElementColors,
						 sizeof(MarsLighthouseHall_LedgeElementColors));
		MarsLighthouseHall_LedgeCycleFrame = 0;
	}
	base = MarsLighthouseHall_LedgeCycleFrame / INSCRIPTION_GLOW_STEP_FRAMES %
		   INSCRIPTION_GLOW_COLORS;
	syWordsCopyEntry(MarsLighthouseHall_LedgePalette,
					 MarsLighthouseHall_LedgeSavedPalette,
					 sizeof(MarsLighthouseHall_LedgeSavedPalette));
	if(entrance ==
		   MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER &&
	   fdEventGetFlag(FLAG_VENUS_FLAME_LIT)) {
		for(i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
			MarsLighthouseHall_LedgePalette[LEDGE_GLOW + i] =
				MarsLighthouseHall_LedgeElementColors
					[VENUS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
	}
	if(entrance ==
		   MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER &&
	   fdEventGetFlag(FLAG_MERCURY_FLAME_LIT)) {
		for(i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
			MarsLighthouseHall_LedgePalette[LEDGE_GLOW + i] =
				MarsLighthouseHall_LedgeElementColors
					[MERCURY_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
	}
	if(entrance ==
		   MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER &&
	   fdEventGetFlag(FLAG_MARS_FLAME_LIT)) {
		for(i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
			MarsLighthouseHall_LedgePalette[LEDGE_GLOW + i] =
				MarsLighthouseHall_LedgeElementColors
					[MARS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
	}
	if(entrance ==
		   MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER &&
	   fdEventGetFlag(FLAG_JUPITER_FLAME_LIT)) {
		for(i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
			MarsLighthouseHall_LedgePalette[LEDGE_GLOW + i] =
				MarsLighthouseHall_LedgeElementColors
					[JUPITER_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
	}
	IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
						(u32)MarsLighthouseHall_LedgePalette,
						DMA_COPY_WORDS(8));
	MarsLighthouseHall_LedgeCycleFrame++;
}

/* Walking back into the doorway at the top of the beacon ledge's steps */
/* (cell (31, 17)) takes Felix back into the tower he came up through. */
void scMarsLighthouseHallLeaveLedge(void) {
	fdEventPlaySnd(SE_MAP_EXIT);
	syFieldEventSetExitRequest(MainRoutineStateWork.entrance);
}

/* Nothing visible happens yet when Felix casts Blaze at the torch on the */
/* beacon ledge; the game only marks this cast's jet of flame as the long one,
 */
/* so that it carries past the torch up to the beacon flame. */
void scMarsLighthouseHallStartBlazeOnTorch(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	event->blazeLongSpray = 1;
}

/* As Felix's Blaze on the beacon ledge's torch flies on past it, the camera */
/* follows the fire up to the unlit beacon. */
void scMarsLighthouseHallWatchBlazeReachFlame(void) {
	fdCameraMoveToActorById(ACTOR_LEDGE_BLAZE_TARGET, 1);
	fdCameraWaitForArrival();
}

/* Blaze reaches the beacon above the ledge's torch: the screen flashes white */
/* with a chime, and the beacon of the tower Felix came up through is lit. */
void scMarsLighthouseHallLightFlame(void) {
	FIELD_STATE *state;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeInterpolate(30);
	syFieldEventWait(30);
	fdEventPlaySnd(SE_WARP);
	switch(MainRoutineStateWork.entrance) {
	case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
		fdEventSetFlag(FLAG_VENUS_FLAME_LIT);
		break;
	case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
		fdEventSetFlag(FLAG_MERCURY_FLAME_LIT);
		break;
	case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
		fdEventSetFlag(FLAG_MARS_FLAME_LIT);
		break;
	case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
		fdEventSetFlag(FLAG_JUPITER_FLAME_LIT);
		break;
	}
	state = Ram_HeapSlots->fieldState;
	state->psynergy = 0;
	syFieldEventWait(30);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(30);
	syFieldEventWait(30);
	syFieldEventWait(10);
	fdEventEnd();
}

extern s32 MarsLighthouseHall_HallCycleFrame;

/* Each frame in the hall: the inscription of each element whose flame is lit */
/* glows in its colours, turning round, and the others show unlit. */
void scMarsLighthouseHallCycleHallInscriptions(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 base;
	s32 i;
	s8 busy;

	if(state->psynergy != 0)
		return;
	busy = state->revealActive;
	if(busy != 0)
		return;
	if(MarsLighthouseHall_HallCycleFrame == -1) {
		syWordsCopyEntry(MarsLighthouseHall_HallSavedPalette,
						 (void *)&BG_PLTT_COLOR(UNLIT_INSCRIPTION_PALETTE, 0),
						 PALETTE_COLORS * 2);
		syWordsCopyEntry(MarsLighthouseHall_HallElementColors.cols,
						 (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
						 sizeof(MarsLighthouseHall_HallElementColors));
		syWordsCopyEntry(MarsLighthouseHall_HallPalette.cols,
						 MarsLighthouseHall_HallElementColors.cols,
						 sizeof(MarsLighthouseHall_HallElementColors));
		MarsLighthouseHall_HallCycleFrame = 0;
	}
	base = MarsLighthouseHall_HallCycleFrame / INSCRIPTION_GLOW_STEP_FRAMES %
		   INSCRIPTION_GLOW_COLORS;
	for(i = 0; i < INSCRIPTION_GLOW_COLORS; i++) {
		MarsLighthouseHall_HallPalette.cols[JUPITER_GLOW + i] =
			MarsLighthouseHall_HallElementColors
				.cols[JUPITER_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
		MarsLighthouseHall_HallPalette.cols[VENUS_GLOW + i] =
			MarsLighthouseHall_HallElementColors
				.cols[VENUS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
		MarsLighthouseHall_HallPalette.cols[MERCURY_GLOW + i] =
			MarsLighthouseHall_HallElementColors
				.cols[MERCURY_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
		MarsLighthouseHall_HallPalette.cols[MARS_GLOW + i] =
			MarsLighthouseHall_HallElementColors
				.cols[MARS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
	}
	if(!fdEventGetFlag(FLAG_VENUS_FLAME_LIT))
		syWordsCopyEntry(&MarsLighthouseHall_HallPalette.cols[VENUS_GLOW],
						 MarsLighthouseHall_UnlitInscriptionColors,
						 sizeof(MarsLighthouseHall_UnlitInscriptionColors));
	if(!fdEventGetFlag(FLAG_MERCURY_FLAME_LIT))
		syWordsCopyEntry(&MarsLighthouseHall_HallPalette.cols[MERCURY_GLOW],
						 MarsLighthouseHall_UnlitInscriptionColors,
						 sizeof(MarsLighthouseHall_UnlitInscriptionColors));
	if(!fdEventGetFlag(FLAG_MARS_FLAME_LIT))
		syWordsCopyEntry(&MarsLighthouseHall_HallPalette.cols[MARS_GLOW],
						 MarsLighthouseHall_UnlitInscriptionColors,
						 sizeof(MarsLighthouseHall_UnlitInscriptionColors));
	if(!fdEventGetFlag(FLAG_JUPITER_FLAME_LIT))
		syWordsCopyEntry(&MarsLighthouseHall_HallPalette.cols[JUPITER_GLOW],
						 MarsLighthouseHall_UnlitInscriptionColors,
						 sizeof(MarsLighthouseHall_UnlitInscriptionColors));
	IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
						(u32)MarsLighthouseHall_HallPalette.cols,
						DMA_COPY_WORDS(16));
	MarsLighthouseHall_HallCycleFrame++;
}

/* Each frame, the purple light in the arched doorway Felix last stepped up to
 */
/* fades one step in or out. Fully lit, the doorway opens a way through the */
/* wall (the cells under its arch are copied from the open template); faded */
/* out, the wall closes again. */
void scMarsLighthouseHallFadeDoorway(void) {
	s32 step;

	if(MarsLighthouseHall_DoorwayBlend == MarsLighthouseHall_DoorwayBlendTarget)
		return;
	step =
		MarsLighthouseHall_DoorwayBlendTarget - MarsLighthouseHall_DoorwayBlend;
	if(step < 0)
		step = -1;
	else
		step = 1;
	MarsLighthouseHall_DoorwayBlend += step;
	IO_WRITE_QUEUE_PUSH(
		(u32)&REG_BLDALPHA,
		BLEND_ALPHA(MarsLighthouseHall_DoorwayBlend,
					BLEND_WEIGHT_MAX - MarsLighthouseHall_DoorwayBlend),
		IO_WRITE_HALFWORD);
	if(MarsLighthouseHall_DoorwayBlend == 0) {
		if(MarsLighthouseHall_Doorway == DOORWAY_VENUS)
			fdMapGridCopyCellAttributes(34, 8, 3, 3, 17, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_MERCURY)
			fdMapGridCopyCellAttributes(30, 8, 3, 3, 8, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_MARS)
			fdMapGridCopyCellAttributes(34, 8, 3, 3, 17, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_JUPITER)
			fdMapGridCopyCellAttributes(30, 8, 3, 3, 8, 8);
	}
	if(MarsLighthouseHall_DoorwayBlend == BLEND_WEIGHT_MAX) {
		if(MarsLighthouseHall_Doorway == DOORWAY_VENUS)
			fdMapGridCopyCellAttributes(34, 0, 3, 3, 17, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_MERCURY)
			fdMapGridCopyCellAttributes(30, 4, 3, 3, 8, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_MARS)
			fdMapGridCopyCellAttributes(34, 4, 3, 3, 17, 8);
		if(MarsLighthouseHall_Doorway == DOORWAY_JUPITER)
			fdMapGridCopyCellAttributes(30, 0, 3, 3, 8, 8);
	}
}

/* Once the lighthouse has thawed, stepping under one of the hall's four */
/* arched doorways, two each side of the dragon statue, lights its opening */
/* purple with a hum, and stepping to the cells beside it fades it out again. */
/* The arch under Venus's doorway is cell (18, 10), Mercury's (9, 11), Mars's */
/* (18, 11) and Jupiter's (9, 10); the last doorway lit is put away first. */
void scMarsLighthouseHallStepTowardDoorway(s32 state) {
	s32 next;
	s32 odd;
	u16 *flash;

	if(state == MarsLighthouseHall_DoorwayStep)
		return;
	if((state ^ MarsLighthouseHall_DoorwayStep) & 1)
		fdEventPlaySnd(SE_MARS_LIGHTHOUSE_DOORWAY);
	MarsLighthouseHall_DoorwayStep = state;
	odd = state & 1;
	flash = (u16 *)&MarsLighthouseHall_DoorwayBlendTarget;
	next = state - CELL_BEHAVIOR_VENUS_DOORWAY;
	if(odd)
		*flash = 0;
	else
		*flash = BLEND_WEIGHT_MAX;
	if(MarsLighthouseHall_Doorway == DOORWAY_VENUS)
		fdMapGridCopyCellsLayered(0, 122, 3, 3, 17, 67);
	if(MarsLighthouseHall_Doorway == DOORWAY_MERCURY)
		fdMapGridCopyCellsLayered(0, 122, 3, 3, 8, 72);
	if(MarsLighthouseHall_Doorway == DOORWAY_MARS)
		fdMapGridCopyCellsLayered(0, 122, 3, 3, 17, 72);
	if(MarsLighthouseHall_Doorway == DOORWAY_JUPITER)
		fdMapGridCopyCellsLayered(0, 122, 3, 3, 8, 67);
	MarsLighthouseHall_Doorway = next / 2;
	if(MarsLighthouseHall_Doorway == DOORWAY_VENUS)
		fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 67);
	if(MarsLighthouseHall_Doorway == DOORWAY_MERCURY)
		fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 72);
	if(MarsLighthouseHall_Doorway == DOORWAY_MARS)
		fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 72);
	if(MarsLighthouseHall_Doorway == DOORWAY_JUPITER)
		fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 67);
}

/* As the dragon statue speaks, the four arched doorways light up purple */
/* together, flicker and fade away. */
void scMarsLighthouseHallFlashDoorways(void) {
	fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 67);
	fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 72);
	fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 72);
	fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 67);
	syWait(1);
	MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
	syWait(12);
	MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
	syWait(4);
	MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
	syWait(4);
	MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
	syWait(4);
	MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
	syWait(4);
	MarsLighthouseHall_DoorwayBlendTarget = 0;
	syWait(12);
	fdMapGridCopyCellsLayered(0, 122, 3, 3, 17, 67);
	fdMapGridCopyCellsLayered(0, 122, 3, 3, 8, 72);
	fdMapGridCopyCellsLayered(0, 122, 3, 3, 17, 72);
	fdMapGridCopyCellsLayered(0, 122, 3, 3, 8, 67);
	syWait(1);
}

/* On arriving in the hall through one of its arched doorways, that doorway */
/* is shown lit purple behind Felix (one step short of full, so the next fade */
/* opens its way); arriving any other way, all four are dark. */
void scMarsLighthouseHallStartDoorways(s32 entrance) {
	s32 cell = -1;
	s16 *blend;
	fdBlendScriptSuspend();
	blend = &MarsLighthouseHall_DoorwayBlend;
	if(entrance == ENTRANCE_THROUGH_VENUS_DOORWAY)
		cell = CELL_BEHAVIOR_VENUS_DOORWAY;
	if(entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY)
		cell = CELL_BEHAVIOR_MERCURY_DOORWAY;
	if(entrance == ENTRANCE_THROUGH_MARS_DOORWAY)
		cell = CELL_BEHAVIOR_MARS_DOORWAY;
	if(entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY)
		cell = CELL_BEHAVIOR_JUPITER_DOORWAY;
	if(cell == -1) {
		*blend = 0;
		MarsLighthouseHall_DoorwayBlendTarget = 0;
		MarsLighthouseHall_Doorway = cell;
		MarsLighthouseHall_DoorwayStep = cell;
	}
	else {
		s32 step;
		*blend = BLEND_WEIGHT_MAX - 1;
		MarsLighthouseHall_DoorwayBlendTarget = BLEND_WEIGHT_MAX;
		step = (cell - CELL_BEHAVIOR_VENUS_DOORWAY) / 2;
		MarsLighthouseHall_Doorway = step;
		MarsLighthouseHall_DoorwayStep = cell;
		if(step == DOORWAY_VENUS)
			fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 67);
		if(MarsLighthouseHall_Doorway == DOORWAY_MERCURY)
			fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 72);
		if(MarsLighthouseHall_Doorway == DOORWAY_MARS)
			fdMapGridCopyCellsLayered(0, 125, 3, 3, 17, 72);
		if(MarsLighthouseHall_Doorway == DOORWAY_JUPITER)
			fdMapGridCopyCellsLayered(0, 125, 3, 3, 8, 67);
	}
	IO_WRITE_QUEUE_PUSH(
		(u32)&REG_BLDALPHA,
		BLEND_ALPHA(MarsLighthouseHall_DoorwayBlend,
					BLEND_WEIGHT_MAX - MarsLighthouseHall_DoorwayBlend),
		IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
						BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
						IO_WRITE_HALFWORD);
	syTaskAddFrame(scMarsLighthouseHallFadeDoorway, TASK_PASS_FRAME);
}

/* Felix casts Teleport on the round carving the dragon statue's fireball */
/* uncovered in the floor before it (cells (12, 12) to (15, 14)): the screen */
/* turns red, sparks swirl round the party, and they arrive on the carved */
/* octagon of the Mars Lighthouse Aerie in the storm. */
void scMarsLighthouseHallTeleportToAerie(void) {
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldEventSetExitRequest(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_AERIE);
	syFieldEventWait(40);
	scTeleportStopShimmer();
	syFieldTaskRemove(scMarsLighthouseHallFadeDoorway);
}

const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE)
		return MarsLighthouseHall_FlameTriggers;
	if(scene == SCENE_MARS_LIGHTHOUSE_HALL)
		return MarsLighthouseHall_InscriptionsTriggers;
	if(scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_MARS_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE)
		return MarsLighthouseHall_LedgesTriggers;
	return MarsLighthouseHall_Triggers;
}

void scPaletteSetColsFade(u16 *slots, u16 *colours, u16 *a, u16 *b);

extern u16 TeleportCircle_PaletteSlots[];
extern u16 TeleportCircle_Colors[];
extern u16 TeleportCircle_ShimmerColors[];
extern u16 TeleportCircle_ShimmerSlots[];

s32 scInit(void) {
	s32 entrance;
	s32 scene;

	scene = MainRoutineStateWork.next;
	entrance = MainRoutineStateWork.entrance;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
	scPaletteSetColsFade(
		(s32)TeleportCircle_PaletteSlots, (s32)TeleportCircle_Colors,
		(s32)TeleportCircle_ShimmerColors, (s32)TeleportCircle_ShimmerSlots);
	if(scene != SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE &&
	   scene == SCENE_MARS_LIGHTHOUSE_HALL) {
		Call1((void (*)())fdEffectStartBlankOverlay, 0);
		FieldStateWork->transition = SCENE_TRANSITION(
			TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
		if(entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE ||
		   entrance ==
			   MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE ||
		   entrance == ENTRANCE_THROUGH_VENUS_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_MARS_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY) {
			s32 unset = fdEventGetFlag(FLAG_WAY_TO_AERIE_OPEN);

			if(unset == 0 && fdEventGetFlag(FLAG_VENUS_FLAME_LIT) &&
			   fdEventGetFlag(FLAG_MERCURY_FLAME_LIT) &&
			   fdEventGetFlag(FLAG_MARS_FLAME_LIT) &&
			   fdEventGetFlag(FLAG_JUPITER_FLAME_LIT)) {
				ACTOR *actor;
				u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

				fdEventStart();
				syFieldEventEndLastingPsynergy(0);
				fdEventStartScrTransition();
				fdEventWaitTransitionFrames();
				fdEventPlaySnd(SOUND_FADE_OUT_BGM);
				fdCameraSetSpeed(2);
				fdCameraMoveTo(224, 64, 216, 1);
				fdCameraWaitForArrival();
				syFieldEventWait(20);
				fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
				fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
				fdPaletteFadeInterpolate(20);
				syWait(20);
				*color = RGB_WHITE;
				syWait(1);
				syEventMessageShowCentered(
					(s32)MsgMarsLighthouseHallYouHaveProvenYourWorth, 0, 4);
				fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
				fdPaletteFadeInterpolate(20);
				syWait(20);
				fdCameraSetSpeed(1);
				fdCameraMoveTo(224, 16, 216, 1);
				fdCameraWaitForArrival();
				syFieldEventWait(40);
				/* unset is zero here: the mouth stands still and watches
				 * nothing. */
				actor = fdActorGet(ACTOR_STATUE_MOUTH);
				actor->motionFlg = unset;
				actor->y.fixed = toFIXED(16);
				actor->groundHeight = toFIXED(16);
				actor->launchedActorId = ACTOR_FIREBALL;
				actor->userValue = FACING_SOUTH;
				actor->watchedActor = (union FIELD_OBJECT *)unset;
				fdActorLaunchAhead(actor);
				syWait(40);
				fdMapGridSetShake(2, 2, 1);
				scMarsLighthouseHallBreakFloorTile(216, 216, 65, 65, 13, 13);
				scMarsLighthouseHallBreakFloorTile(232, 216, 66, 65, 14, 13);
				scMarsLighthouseHallBreakFloorTile(232, 232, 66, 66, 14, 14);
				scMarsLighthouseHallBreakFloorTile(216, 200, 65, 64, 13, 12);
				scMarsLighthouseHallBreakFloorTile(248, 200, 67, 64, 15, 12);
				scMarsLighthouseHallBreakFloorTile(248, 232, 67, 66, 15, 14);
				scMarsLighthouseHallBreakFloorTile(216, 232, 65, 66, 13, 14);
				scMarsLighthouseHallBreakFloorTile(200, 216, 64, 65, 12, 13);
				scMarsLighthouseHallBreakFloorTile(232, 200, 66, 64, 14, 12);
				scMarsLighthouseHallBreakFloorTile(248, 216, 67, 65, 15, 13);
				scMarsLighthouseHallBreakFloorTile(200, 232, 64, 66, 12, 14);
				scMarsLighthouseHallBreakFloorTile(200, 200, 64, 64, 12, 12);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				fdMapGridCopyCellAttributes(30, 12, 4, 3, 12, 12);
				syFieldEventWait(40);
				fdEventSetFlag(FLAG_WAY_TO_AERIE_OPEN);
				fdEventPlaySnd(MUS_PUZZLE_SOLVED);
				sySndWaitForBgmRestore();
				scPlayMusic();
				fdEventEnd();
			}
		}
	}
	return 0;
}

/* The line scroll's wave across and down that the rain ledges start with the */
/* rain. */
static __inline__ void scMarsLighthouseHallStartRainWave(s32 mode, FIXED freqX,
														 s32 stepX, FIXED ampX,
														 FIXED freqY, s32 stepY,
														 FIXED ampY) {
	fdLineScrollInitWave(mode, freqX, stepX, ampX, freqY, stepY, ampY);
}

s32 scPrepareMap(void) {
	s16 scene = MainRoutineStateWork.next;
	s32 entrance = MainRoutineStateWork.entrance;
	ACTOR *actor;

	if(scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_MARS_LEDGE ||
	   scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE) {
		fdWeatherStartRain(1);
		scMarsLighthouseHallStartRainWave(1, FIXED_ONE, FIXED_ONE * 4,
										  FIXED_ONE * 2, FIXED_ONE,
										  FIXED_ONE / 2, FIXED_ONE / 4);
	}
	if(scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE) {
		switch(entrance) {
		case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
			scFieldActorSetSprCol(fdActorGet(ACTOR_LEDGE_TORCH),
								  SPRITE_COLOR_GRB);
			scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME,
										  FLAME_ANIMATION_VENUS);
			fdTilesetAnimResume(4);
			fdTilesetAnimResume(5);
			break;
		case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
			scFieldActorSetSprCol(fdActorGet(ACTOR_LEDGE_TORCH),
								  SPRITE_COLOR_BGR);
			scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME,
										  FLAME_ANIMATION_MERCURY);
			fdTilesetAnimResume(6);
			fdTilesetAnimResume(7);
			break;
		case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
			scFieldActorSetSprCol(fdActorGet(ACTOR_LEDGE_TORCH),
								  SPRITE_COLOR_NORMAL);
			scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME,
										  FLAME_ANIMATION_MARS);
			fdTilesetAnimResume(0);
			fdTilesetAnimResume(1);
			break;
		case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
			scFieldActorSetSprCol(fdActorGet(ACTOR_LEDGE_TORCH),
								  SPRITE_COLOR_GBR);
			scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME,
										  FLAME_ANIMATION_JUPITER);
			fdTilesetAnimResume(2);
			fdTilesetAnimResume(3);
			break;
		}
		actor = fdActorGet(ACTOR_LEDGE_BLAZE_TARGET);
		actor->motionFlg = 0;
		actor->z.fixed = toFIXED(104);
		syFieldTaskAddFrame(scMarsLighthouseHallCycleLedgeInscription,
							TASK_PASS_FRAME);
	}
	else if(scene == SCENE_MARS_LIGHTHOUSE_HALL) {
		if(entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE ||
		   entrance ==
			   MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE ||
		   entrance == ENTRANCE_THROUGH_VENUS_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_MARS_DOORWAY ||
		   entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY) {
			scFlameThrowersCreate();
			if(fdEventGetFlag(FLAG_WAY_TO_AERIE_OPEN)) {
				fdMapGridCopyCellAttributes(30, 12, 4, 3, 12, 12);
				fdMapGridCopyCellsLayered(64, 64, 4, 3, 76, 12);
			}
			scMarsLighthouseHallStartDoorways(entrance);
			{
				s32 priority = TASK_PASS_FRAME;

				syFieldTaskAddFrame(scMarsLighthouseHallCycleHallInscriptions,
									priority);
			}
			scMarsLighthouseHallShowFlame(ACTOR_VENUS_FLAME,
										  FLAME_ANIMATION_VENUS);
			scMarsLighthouseHallShowFlame(ACTOR_MERCURY_FLAME,
										  FLAME_ANIMATION_MERCURY);
			scMarsLighthouseHallShowFlame(ACTOR_MARS_FLAME,
										  FLAME_ANIMATION_MARS);
			scMarsLighthouseHallShowFlame(ACTOR_JUPITER_FLAME,
										  FLAME_ANIMATION_JUPITER);
			if(!fdEventGetFlag(FLAG_VENUS_FLAME_LIT))
				fdActorSetPos(ACTOR_VENUS_FLAME, 0, 0);
			if(!fdEventGetFlag(FLAG_MERCURY_FLAME_LIT))
				fdActorSetPos(ACTOR_MERCURY_FLAME, 0, 0);
			if(!fdEventGetFlag(FLAG_MARS_FLAME_LIT))
				fdActorSetPos(ACTOR_MARS_FLAME, 0, 0);
			if(!fdEventGetFlag(FLAG_JUPITER_FLAME_LIT))
				fdActorSetPos(ACTOR_JUPITER_FLAME, 0, 0);
		}
	}
	if(fdEventGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
		fdMapCopyPalette(MainRoutineStateWork.mapId +
							 MARS_LIGHTHOUSE_THAWED_MAPS,
						 (s32)BG_PLTT);
	}
	return 0;
}

/* Felix reads the inscription on the hall's gallery east of the dragon */
/* statue's head: the screen turns yellow-green and "Mankind... With the power
 */
/* of wisdom, they ruled the earth." appears across it. */
void scMarsLighthouseHallReadEarthInscription(void) {
	u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	*color = RGB_WHITE;
	sySynch(1);
	syEventMessageShowCentered(
		(s32)MsgMarsLighthouseHallMankindWithPowerOfWisdom, 0, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdEventEnd();
}

/* Felix reads the inscription on the hall's floor west of the dragon statue's
 */
/* chin: the screen turns yellow-green and "Fish... With cold courage, they */
/* ruled the water." appears across it. */
void scMarsLighthouseHallReadWaterInscription(void) {
	u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	*color = RGB_WHITE;
	sySynch(1);
	syEventMessageShowCentered(
		(s32)MsgMarsLighthouseHallFishWithColdCourageThey, 0, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdEventEnd();
}

/* Felix reads the inscription on the hall's floor east of the dragon statue's
 */
/* chin: the screen turns yellow-green and "Dragons... Burning with might, they
 */
/* ruled the fires." appears across it. */
void scMarsLighthouseHallReadFireInscription(void) {
	u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	*color = RGB_WHITE;
	sySynch(1);
	syEventMessageShowCentered(
		(s32)MsgMarsLighthouseHallDragonsBurningWithMightThey, 0, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdEventEnd();
}

/* Felix reads the inscription on the hall's gallery west of the dragon */
/* statue's head: the screen turns yellow-green and "Bird... On the wings of */
/* truth, they ruled the winds." appears across it. */
void scMarsLighthouseHallReadWindInscription(void) {
	u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	*color = RGB_WHITE;
	sySynch(1);
	syEventMessageShowCentered((s32)MsgMarsLighthouseHallBirdOnWingsOfTruth, 0,
							   0);
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(20);
	sySynch(20);
	fdEventEnd();
}

/* A puff of flame where the fireball breaks the floor: one that swells as it */
/* rises and shrinks back, or one thrown off at random. */
void scMarsLighthouseHallSpawnFlamePuff(s32 x, s32 z, s32 kind) {
	ACTOR *actor;

	actor = scFieldActorCreate(SPRITE_FLAME_PUFF, toFIXED(x), 0, toFIXED(z));
	if(actor != 0) {
		fdActorSetSprFlags(actor, 0);
		fdActorSelectAnimation(actor, 3);
		if(kind != 0) {
			((PLACED_SPR *)actor->spr)->pri = 2;
			fdActorStartScript(actor, MarsLighthouseHall_FlamePuffSwellScript);
		}
		else {
			((PLACED_SPR *)actor->spr)->pri = 1;
			actor->velX = toFIXED(syRandomBelow(8) - 4);
			actor->velY = toFIXED(4);
			fdActorStartScript(actor, MarsLighthouseHall_FlamePuffThrownScript);
		}
	}
}

/* One floor tile before the statue breaks in a burst of flame with a clatter,
 */
/* showing its part of the round carving beneath: the cell (cellX, cellY) */
/* takes the picture at (sourceX, sourceY). */
void scMarsLighthouseHallBreakFloorTile(s32 x, s32 z, s32 sourceX, s32 sourceY,
										s32 cellX, s32 cellY) {
	u32 i;
	s32 px;
	s32 pz;

	fdEventPlaySnd(SE_CLATTER);
	scMarsLighthouseHallSpawnFlamePuff(x, z + 12, 1);
	for(i = 0; i < 3; i++) {
		px = x + (syRandomBelow(12)) - 6;
		pz = z + (syRandomBelow(12)) - 6;
		scMarsLighthouseHallSpawnFlamePuff(px, pz, 0);
	}
	sySynch(4);
	fdMapGridCopyCellsTo(sourceX, sourceY, cellX + 64, cellY, 1, 1);
}

/* The great dragon statue at the head of the hall speaks as Felix steps in */
/* from the stairs (cells (11, 16) to (16, 16)): the camera rises to it and */
/* the screen turns yellow-green. Without the Mars Star it says "If you do not
 */
/* hold the star, you shall not ascend to the heavens... Begone!", once. With */
/* the star it bids the party set the four spirits aflame and the gallery's */
/* four doorways flash, after which it speaks no more; from patch 1 it waits */
/* for the lighthouse to have thawed as well. */
void scMarsLighthouseHallHearDragonStatue(void) {
	if(gmPartyInventoryFindMember(ITEM_MYTHRIL_BAG_MARS_STAR) == -1) {
		if(!fdEventGetFlag(FLAG_TOLD_TO_BEGONE)) {
			u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

			fdEventStart();
			fdEventEndLastingPsynergy(0);
			fdCameraSetSpeed(2);
			fdCameraMoveTo(224, 64, 216, 1);
			fdCameraWaitForArrival();
			syFieldEventWait(20);
			fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
			fdPaletteFadeStartInterpolation(20);
			sySynch(20);
			*color = RGB_WHITE;
			sySynch(1);
			syEventMessageShowCentered((s32)MsgMarsLighthouseHallIfYouDoNotHold,
									   0, 4);
			fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
			fdPaletteFadeStartInterpolation(20);
			sySynch(20);
			fdEventSetFlag(FLAG_TOLD_TO_BEGONE);
			fdEventEnd();
		}
	}
#if !TLA_PATCH_1
	else {
#else
	else if(fdEventGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
#endif
		u16 *color = (u16 *)&CENTERED_TEXT_COLOR;

		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdCameraSetSpeed(2);
		fdCameraMoveTo(224, 64, 216, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(20);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
		fdPaletteFadeStartInterpolation(20);
		sySynch(20);
		*color = RGB_WHITE;
		sySynch(1);
		syEventMessageShowCentered((s32)MsgMarsLighthouseHallYouWhoHoldStarOpen,
								   0, 4);
		sySynch(10);
		syEventMessageShowCentered(
			(s32)MsgMarsLighthouseHallYouWhoHoldStarOpen + 1, 0, 3);
		fdCameraMoveTo(224, 88, 216, 1);
		fdCameraWaitForArrival();
		syFieldEventWait(20);
		fdEventPlaySnd(SE_MARS_LIGHTHOUSE_DOORWAY);
		scMarsLighthouseHallFlashDoorways();
		syFieldEventWait(20);
		fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
		fdPaletteFadeStartInterpolation(20);
		sySynch(40);
		fdEventSetFlag(FLAG_TOLD_TO_LIGHT_FLAMES);
		fdEventEnd();
	}
}

void scTeleportArriveWhirling(void);

s32 fdLeaderKnockBack(void);
s32 scFireballCarryLeader(void);
void fdActorLaunchAhead(ACTOR *self);

void fdTeleportPlaySparks(void);
void scMarsLighthouseHallFadeDoorway(void);

#include "shared/teleport_shimmer.c"

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

s32 fdActorIsTouching(ACTOR *a, ACTOR *b);
s32 fdActorIsBeside(ACTOR *a, ACTOR *b);

#include "shared/flame_update_forward.h"

#include "shared/actor_reset_scripted_for.h"

/* A flame thrower's flame (FLAME_FORWARD), sent south from `position` at */
/* `speed` (syFlameUpdateForward); `offScreen` keeps its sprite's picture */
/* loaded out of sight. */
void scFlameThrowerSendForward(VECTOR3 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position->x,
									  position->y, position->z);

	if(actor != 0) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = speed;
		fdActorResetScriptedFor(actor, 2);
		actor->update = syFlameUpdateForward;
		if(offScreen != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

s32 fdActorIsTouching(ACTOR *a, ACTOR *b);
s32 fdActorIsBeside(ACTOR *a, ACTOR *b);

#include "shared/flame_update_sideways.h"

#include "shared/actor_setup_scripted_part.h"

/* A flame thrower's flame (FLAME_BACKWARD), sent north from `position` at */
/* `speed` (syFlameUpdateForward); `offScreen` keeps its sprite's picture */
/* loaded out of sight. */
void scFlameThrowerSendBackward(VECTOR3 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position->x,
									  position->y, position->z);

	if(actor != 0) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = -speed;
		fdActorSetupScriptedPart(actor, 3);
		actor->update = syFlameUpdateForward;
		if(offScreen != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

/* A flame thrower's flame (FLAME_EAST), sent east from `position` at `speed` */
/* (syFlameUpdateSideways); `offScreen` keeps its sprite's picture loaded out */
/* of sight. */
void scFlameThrowerSendEast(VECTOR3 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position->x,
									  position->y, position->z);

	if(actor != 0) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = speed;
		fdActorSetupScriptedPart(actor, 3);
		actor->update = syFlameUpdateSideways;
		if(offScreen != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

/* A flame thrower's flame (FLAME_WEST), sent west from `position` at `speed` */
/* (syFlameUpdateSideways); `offScreen` keeps its sprite's picture loaded out */
/* of sight. */
void scFlameThrowerSendWest(VECTOR3 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, position->x,
									  position->y, position->z);

	if(actor != 0) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = -speed;
		fdActorSetupScriptedPart(actor, 3);
		actor->update = syFlameUpdateSideways;
		if(offScreen != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted.h"

/* A spark puffed from a flame thrower, drifting off in a random direction. */
void scFlameThrowerSendSpark(VECTOR3 *position, s32 speed, s32 offScreen) {
	ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, position->x,
									  position->y, position->z - toFIXED(2));
	s32 spin;

	if(actor != 0) {
		actor->bounce =
			syCos(syRandomBelow(TRIG_TURN * 3 / 16) + TRIG_TURN * 3 / 16);
		actor->gravity = (u16)syRandom();
		actor->sceneWord = (u16)(syRandom() * 2) + speed;
		spin = (u16)syRandom() - DEGtoANG(180);
		actor->tmr = spin;
		((void (*)())fdActorSetupScripted)(actor, 2);
		actor->update = scFlameThrowerUpdateSpark;
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	}
}

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

#include "shared/flame_throwers_release_flame.h"

/* Sets whether the flame throwers keep puffing while Psynergy is used. */
void scFlameThrowersSetMode(s16 value) {
	FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

/* The flame throwers' counter, which scFlameThrowersAdvanceCounter steps. */
s32 scFlameThrowersGetCounter(void) { return FLAME_THROWERS_STATE->cnt; }

#include "shared/flame_throwers_advance_counter.h"

#include "shared/flame_thrower_set.h"

#include "shared/flame_thrower_set_puffs_off_screen.c"

#include "shared/flame_thrower_get_flags.c"

#include "shared/flame_throwers_set_stop.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

#include "shared/leader_knock_back.c"

/* Hit by the flames, the leader is knocked back. */
void scMarsLighthouseHallKnockBackIfHit(void) {
	if(FLAME_THROWERS_STATE->active)
		fdLeaderKnockBack();
}

/* Hit by a rolling fireball: the leader is hurt, blinks, and is carried along
 */
/* with it until it stops. */
s32 scFireballCarryLeader(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	FLAME_HIT_STATE *ride;
	ACTOR *leader;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED s32 unused[3];

	if(event->transitionActive == 0) {
		fdScrTransitionStart(event->transition, event->transitionFrm);
		event->transitionActive = 1;
	}
	fdEventClearFlag(FLAG_MENU_OPEN);
	event->psynergy = 0;
	event->fieldMenu = 0;
	event->pauseMenu = 0;
	event->aButton = 0;
	event->encounter = 0;
#ifdef UBFIX
	gmPartyTakeFallDamage();
#else
	/* UB: the fall damage takes no arguments and returns nothing; it is passed
	 */
	/* the arguments it gives fdPartyAddHp. */
	((s32 (*)())gmPartyTakeFallDamage)(-FALL_DAMAGE, 0);
#endif
	fdCameraSetSpeed(7);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ride = FLAME_THROWERS_STATE;
	fdEventSetFlag(FLAG_MENU_OPEN);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventClearFlag(FLAG_MENU_OPEN);
	leader->update = (OBJECT_UPDATE)fdActorBlink;
	leader->facing = FACING_SOUTH;
	fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
	while(ride->car->tmr == 0) {
		s32 kind = fdMapGridGetTerrainKindOrBumpAt(0, leader->x.fixed,
												   leader->z.fixed);

		if(kind != TERRAIN_ROUNDED_SQUARE) {
			leader->x.fixed += ride->car->stepX;
			leader->z.fixed += ride->car->stepZ;
		}
		else {
			if(ride->car->stepX != 0)
				leader->x.fixed =
					(leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
			if(ride->car->stepZ != 0)
				leader->z.fixed =
					(leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
		}
		ride->active = 0;
		syWait(1);
	}
	syFieldEventWait(30);
	leader->update = NULL;
	fdActorSetSprColMode(leader, 0);
	ride->active = 0;
#ifdef UBFIX
	return 0;
#endif
}

void fdTeleportPlaySparks(void);

#include "shared/fireball_carry_leader_if_hit.h"

/* Whether a flame or fireball has hit the leader. */
s32 scFlameThrowersHasHitLeader(void) { return FLAME_THROWERS_STATE->active; }

#include "shared/actor_scatter_crumbs.c"

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_event.c"

#include "shared/actor_roll_and_crumble.c"

#include "shared/actor_attach.c"

#include "shared/actor_launch_ahead.h"

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

s32 MarsLighthouseHall_TorchScript[] = {
	SC_CALL(scMarsLighthouseHallSetUpTorch),
	SC_END,
};

/* clang-format off */
#include "shared/mars_lighthouse_part_scripts.c"

#define TELEPORT_CIRCLE_FIRST_ENTRY 201
#include "shared/teleport_circle_palette.c"

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(MarsLighthouseBeaconLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXITS(MarsLighthouseHall),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_AERIE, MarsLighthouseAerie, MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_VENUS_LEDGE, MarsLighthouseVenusLedge, MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_MERCURY_LEDGE, MarsLighthouseMercuryLedge, MARS_LIGHTHOUSE_MERCURY_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_MARS_LEDGE, MarsLighthouseMarsLedge, MARS_LIGHTHOUSE_MARS_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_JUPITER_LEDGE, MarsLighthouseJupiterLedge, MARS_LIGHTHOUSE_JUPITER_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(7, MarsLighthouseHall, 8),
    SCENE_EXIT(8, MarsLighthouseHall, 7),
    SCENE_EXIT(9, MarsLighthouseHall, 10),
    SCENE_EXIT(10, MarsLighthouseHall, 9),
    SCENE_EXIT(11, MarsLighthouseHall, 12),
    SCENE_EXIT(12, MarsLighthouseHall, 11),
    SCENE_EXIT(13, MarsLighthouseHall, 14),
    SCENE_EXIT(14, MarsLighthouseHall, 13),
    SCENE_EXITS(MarsLighthouseMercuryLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXITS(MarsLighthouseJupiterLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXITS(MarsLighthouseMarsLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_MARS_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_MARS_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_MARS_LEDGE_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_MARS_LEDGE),
    SCENE_EXITS(MarsLighthouseVenusLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXITS_END,
};

ACTOR_PLACE MarsLighthouseHall_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseHall_FlamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)MarsLighthouseHall_TorchScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (s32)MarsLighthouseHall_TorchScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseHall_InscriptionsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER MarsLighthouseHall_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/* The beacon ledge inscription's glow, in frames; -1 before it starts. */
s32 MarsLighthouseHall_LedgeCycleFrame = -1;

TRIGGER MarsLighthouseHall_FlameTriggers[] = {
    {TRIGGER_STEP, CELL_LEDGE_DOORWAY, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallLeaveLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEDGE_TORCH, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallStartBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallWatchBlazeReachFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallLightFlame)},
    {TRIGGER_TABLE_END},
};

/* The hall inscriptions' glow, in frames; -1 before it starts. */
s32 MarsLighthouseHall_HallCycleFrame = -1;

TRIGGER MarsLighthouseHall_InscriptionsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_VENUS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MARS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MARS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_JUPITER_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {TRIGGER_STEP, CELL_BEHAVIOR_JUPITER_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), EVENT(scMarsLighthouseHallStepTowardDoorway)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), CELL_AERIE_CARVING, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallTeleportToAerie)},
    {TRIGGER_EXAMINE, CELL_EARTH_INSCRIPTION, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallReadEarthInscription)},
    {TRIGGER_EXAMINE, CELL_WATER_INSCRIPTION, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallReadWaterInscription)},
    {TRIGGER_EXAMINE, CELL_FIRE_INSCRIPTION, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallReadFireInscription)},
    {TRIGGER_EXAMINE, CELL_WIND_INSCRIPTION, CONDITION_ALWAYS, EVENT(scMarsLighthouseHallReadWindInscription)},
    {TRIGGER_STEP, CELL_BEFORE_DRAGON_STATUE, FLAG_TOLD_TO_LIGHT_FLAMES, EVENT(scMarsLighthouseHallHearDragonStatue)},
    {TRIGGER_TABLE_END},
};

TRIGGER MarsLighthouseHall_LedgesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

s32 MarsLighthouseHall_FlamePuffSwellScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 16),
    SC_ADD(SW_Y, -toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 3 / 16),
    SC_ADD(SW_Y, toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 MarsLighthouseHall_FlamePuffThrownScript[] = {
    SC_WAIT(20),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
/* clang-format on */
