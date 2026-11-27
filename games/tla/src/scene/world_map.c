/* The World Map (SceneId_WorldMap), the perspective map that joins the areas,
   and the only scene of this overlay.

   It lets the party into the areas as The Broken Seal's does. On it the party
   meets Echo and six more Djinn, sails and later flies the Lemurian Ship, gazes
   into a pond and finds the Golden Boots, and the story plays several of its
   scenes here: Venus Lighthouse lit and falling as Idejima breaks away and
   drifts north on a tidal wave, the first voyage at the tiller, Loho's men
   bringing the cannon aboard, the bandit and thieves from Vault after a linked
   game, the party carried in sparkles to Aqua Rock, and Jupiter's beacon seen
   lit from the sea. */

#include "item_ids.h"
#include "macros.h"
#include "edition.h"
#include "field/field_mode.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "field/field_sprite.h"
#include "field/map.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/staff_credit.h"
#include "window/window.h"
#include "text/msg_ids.h"

void scWorldMapLoadTidalWave(Sint32 unused);

#include "shared/ship_flight.h"

struct ACTOR;

/* A FIELD_SPR read as its sprite attribute 1, whose top two bits are the sprite
   size where FIELD_SPR names them its flips. */
typedef struct WORLD_MAP_OAM {
	/* FIELD_SPR's unused bytes and attribute 0, not read here */
	Uint8  filler[6];
	Uint16 x : 9;
	Uint16 affineIdx : 5;
	Uint16 size : 2;
} WORLD_MAP_OAM;

/* The World Map's own actors, numbered in the order of WorldMapActorPlacements
   from ACTOR_FIRST_PLACED; a placement wearing a party member's sprite is that
   member, and takes no number. */
typedef enum WORLD_MAP_ACTOR_ID {
	/* Piers's ship, drawn from the map's tiles (WorldMap_TileSpriteScript). */
	ACTOR_SHIP = ACTOR_FIRST_PLACED,
	/* The Venus Djinni Echo, circling south of Daila until the party meets
	   him. */
	ACTOR_ECHO,
	ACTOR_IRON,
	ACTOR_SOUR,
	ACTOR_CHILL,
	ACTOR_WHEEZE,
	ACTOR_PETRA,
	ACTOR_CORE,
	/* The men from Loho who bring the cannon to the ship; the first speaks. */
	ACTOR_LOHO_WORKER_A,
	ACTOR_LOHO_WORKER_B,
	ACTOR_LOHO_WORKER_C,
	/* Where the Golden Boots lie once they are shown
	   (scWorldMapShowGoldenBoots). */
	ACTOR_GOLDEN_BOOTS,
	/* The bandit and the two thieves from Vault, after a linked game. */
	ACTOR_BANDIT,
	ACTOR_THIEF_A,
	ACTOR_THIEF_B,
	ACTOR_KRADEN
} WORLD_MAP_ACTOR_ID;

/* WorldMap_CollapsePlacements' and WorldMap_DriftPlacements' actors: Kraden and
   Alex on Idejima, and the piece of Idejima that splits away, then the island
   itself. */
enum { ACTOR_IDEJIMA_KRADEN = ACTOR_FIRST_PLACED, ACTOR_ALEX, ACTOR_IDEJIMA };

/* WorldMap_FirstVoyagePlacements' actors. */
enum { ACTOR_VOYAGE_SHIP = ACTOR_FIRST_PLACED, ACTOR_VOYAGE_KRADEN };

/* WorldMap_DriftingIdejimaPlacements' one actor. */
enum { ACTOR_DRIFTING_IDEJIMA = ACTOR_FIRST_PLACED };

/* The World Map's own metatile behaviors, which the map's metatiles carry: the
   ground around each Djinni met on the map. */
typedef enum WORLD_MAP_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_WORLD_MAP_IRON = 92,
	METATILE_BEHAVIOR_WORLD_MAP_SOUR,
	METATILE_BEHAVIOR_WORLD_MAP_CHILL,
	METATILE_BEHAVIOR_WORLD_MAP_WHEEZE,
	METATILE_BEHAVIOR_WORLD_MAP_PETRA,
	METATILE_BEHAVIOR_WORLD_MAP_CORE,
	/* The landing where the ship moors at Atteka Inlet
	   (scWorldMapMoorAtAttekaInlet). */
	METATILE_BEHAVIOR_WORLD_MAP_ATTEKA_INLET_LANDING = 52,
	/* The ground round the ways out of Gondowan that Jenna will not take before
	   the parties meet at Contigo (scWorldMapRefuseToRunAway). */
	METATILE_BEHAVIOR_WORLD_MAP_AWAY_FROM_CONTIGO = 53,
	/* Where the ship, once it can fly, first takes to the air
	   (scWorldMapTakeOff). */
	METATILE_BEHAVIOR_WORLD_MAP_FIRST_FLIGHT = 40
} WORLD_MAP_METATILE_BEHAVIOR;

/* A block's terrain byte: the party may step ashore from the open sea here. */
#define WORLD_MAP_BLOCK_LANDING (1 << 6)

/* The step cue an area's placed actor raises when touched
   (WorldMap_PlacedActors, from field/world_map.h's STEP_CUE_AREA_BASE), as The
   Broken Seal's; the areas with two ways in choose one by the leader's
   approach. */
#define STEP_CUE_AREA(area) (STEP_CUE_AREA_BASE + (area))

/* Daila's, Yallam's and Prox's slots among WorldMap_PlacedActors. */
enum { PLACED_DAILA = 0, PLACED_YALLAM = 25, PLACED_PROX = 50 };

void scGaugeUpdate(void);

void scWorldMapSelectExitByZ(Sint32 slot, Sint32 exitBefore, Sint32 exitAfter);

void scActorSetFlightMode(ACTOR *actor, Sint32 mode);

void scGaugeInit(void);
void scWorldMapEndFlight(void);
void scGaugeEnd(void);

void scWorldMapFightBanditAndThieves(Sint32 entrance);

void scWorldMapDeliverLohoCannon(void);

void scWorldMapMeetEcho(void);

void scWorldMapShowIdejimaDrifting(void);

void scWorldMapDriftNorthOfContinent(void);

void scWorldMapShowAirsRockFromAlhafra(void);

void scWorldMapTakeTheTiller(void);
void scWorldMapSailToShipDeck(void);
void scWorldMapCarryPartyToAquaRock(void);

/* The lighthouses scWorldMapPlayLighthouseTransition shows lit, each the row of
   WorldMapPalettes its picture takes. */
typedef enum LIGHTHOUSE_VIEW {
	LIGHTHOUSE_VIEW_VENUS = 1,
	LIGHTHOUSE_VIEW_JUPITER = 2
} LIGHTHOUSE_VIEW;

void scWorldMapPlayLighthouseTransition(Sint32 mode);

void scWorldMapMeetSour(void);

void scWorldMapMeetChill(void);

void scWorldMapMeetWheeze(void);

void scWorldMapMeetPetra(void);

void scWorldMapMeetCore(void);

void scWorldMapSetUpFlyingShip(void);

void scWorldMapSetUpShip(void);
void scWorldMapMoorAtAttekaInlet(void);

void scWorldMapRefuseToRunAway(void);

void scWorldMapGazeIntoPond(void);

void scWorldMapPickUpGoldenBoots(void);

void scWorldMapTakeOff(void);

extern Sint32 WorldMap_TileSpriteScript[];
extern Sint32 WorldMap_IslandScript[];
extern Sint32 WorldMap_TidalWaveScript[];

extern Uint32 WorldMap_GlowColorModes[];

/* The party members hidden as the ship first takes off, -1 for none. */
Sint32 WorldMap_HiddenMembers[8] BSS_DATA;

ACTOR					   *WorldMap_PlacedActors[128] BSS_DATA;
Uint16 WorldMapBlend		BSS_DATA;
Sint16 Gauge_Shown BSS_DATA ALIGNED(4);
Sint16 Gauge_Vram			BSS_DATA;
Sint16 Gauge_Countdown		BSS_DATA;
Uint32						Gauge_Sprite[3] BSS_DATA;
Uint8						Gauge_Tiles[8][32] BSS_DATA;
Uint8						Gauge_SourceTiles[12][32] BSS_DATA;

/* The tidal wave's tiles, pictures and palettes, tagged LZ. */
extern Uint8 WorldMapPackedTilesB[];
extern Uint8 WorldMapPackedPicturesB[];
extern Uint8 WorldMapPalettesB[];

void scWorldMapSpawnTwinSparks(ACTOR *actor);

void scWorldMapWatchVenusLighthouseCollapse(void);

/* Once the party has walked nine tenths of the encounter threshold, a coin toss
   either requests the Djinni's battle now or makes the party walk the whole
   threshold first. From the international editions on, only while the party
   walks the World Map, not aboard the ship or as sand. */
#if !DJINNI_ENCOUNTER_ON_FOOT_ONLY
#define WORLD_MAP_TRY_ENCOUNTER(actorId, djinni) \
	{ \
		FIELD_STATE		   *state; \
		MAIN_ROUTINE_STATE *party; \
\
		state = Ram_HeapSlots->fieldState; \
		party = &MainRoutineStateWork; \
		if(party->encounterSteps >= state->encounterThreshold * 9 / 10) { \
			if(syRandom() < 32768) { \
				fdScriptRequestDjinnBattle(actorId, djinni); \
				state->encounterBias = 0; \
			} else { \
				party->encounterSteps = state->encounterThreshold; \
			} \
		} \
	}
#else
#define WORLD_MAP_TRY_ENCOUNTER(actorId, djinni) \
	{ \
		MAIN_ROUTINE_STATE *party; \
		FIELD_STATE		   *state; \
\
		party = &MainRoutineStateWork; \
		state = Ram_HeapSlots->fieldState; \
		if(party->movementMode == LEADER_MOVEMENT_WORLD_MAP) { \
			if(party->encounterSteps >= state->encounterThreshold * 9 / 10) { \
				if(syRandom() < 32768) { \
					fdScriptRequestDjinnBattle(actorId, djinni); \
					state->encounterBias = 0; \
				} else { \
					party->encounterSteps = state->encounterThreshold; \
				} \
			} \
		} \
	}
#endif

/* Walking about the patch of ground east of Madra, over the small bridge and
   down by the water, may meet the Venus Djinni Iron and start his battle, as a
   random battle would (WORLD_MAP_TRY_ENCOUNTER); if he escapes, walking on
   there meets him again. */
void scWorldMapMeetIron(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_IRON, DJINNI_IRON)
}

/* Walking about the field between Mikasalla and Osenia Cavern may meet the
   Mercury Djinni Sour and start his battle. */
void scWorldMapMeetSour(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_SOUR, DJINNI_SOUR)
}

/* Walking about the field near the beach far south of Naribwe may meet the
   Mercury Djinni Chill and start his battle. */
void scWorldMapMeetChill(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CHILL, DJINNI_CHILL)
}

/* Walking about the dead-end inlet south-west of Tundaria Tower may meet the
   Jupiter Djinni Wheeze and start his battle. */
void scWorldMapMeetWheeze(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_WHEEZE, DJINNI_WHEEZE)
}

/* Walking about the patch of ground north of Hesperia may meet the Venus Djinni
   Petra and start her battle. */
void scWorldMapMeetPetra(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_PETRA, DJINNI_PETRA)
}

/* Walking about the clearings in the east of Atteka, the lower continent beyond
   its western beach, may meet the Mars Djinni Core and start his battle. */
void scWorldMapMeetCore(void)
{
	WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CORE, DJINNI_CORE)
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

#include "shared/tile_sprite_reset.h"

/* The marker blinks and bobs up and down on a circle. As The Broken Seal's
   scWorldMapCircleEcho. */
void scWorldMapCircleEcho(BOBBING_MARKER *marker)
{
	Sint16 *angle;
	FIXED	y;
	FIXED	z;
	FIXED	radius;
	FIXED	rise;

	if(FrameCount & 2)
		fdActorSetSprColMode(marker, SPRITE_COLOR_RED_TINT);
	else
		fdActorSetSprColMode(marker, SPRITE_COLOR_WHITE);
	if(marker->stopped == 0) {
		angle = &marker->ang;
		marker->x.fixed = toFIXED(10120);
		y = syMathMulFX(syMathSin(*angle << 3), toFIXED(4));
		z = toFIXED(8356);

		rise = toFIXED(16);
		marker->y.fixed = y + rise;
		marker->z.fixed = z;
		radius = rise;
		syFieldVectorAddPolarOffsetFX(radius, *angle, &marker->x);
		marker->facing = *angle + DEGtoANG(90);
		*angle += TRIG_TURN / 64;
	}
}

/* Until the party has Echo or has learnt about Djinn, Echo circles south of
   Daila (scWorldMapCircleEcho), at half size. */
Sint32 scWorldMapSetUpEcho(ACTOR *actor)
{
	if(fdScriptGetFlag(FLAG_DJINNI(DJINNI_ECHO)) != 0) {
		return 0;
	}
	if(fdScriptGetFlag(FLAG_DJINN_LESSON) != 0) {
		return 0;
	}
	actor->update = (OBJECT_UPDATE)scWorldMapCircleEcho;
	actor->motionFlg = 0;
	actor->tmr = 0;
	actor->userValue = 0;
	actor->scaleX = toFIXED(1) / 2;
	actor->scaleY = toFIXED(1) / 2;
	return 0;
}

/* Turns the island's sprite with the World Map's view and shows it. */
Sint32 scWorldMapSetIslandTile(ACTOR *actor)
{
	Uint16	  *yaw = &((WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid)->yaw;
	FIELD_SPR *sprite = actor->spr;

	sprite->rotation = *yaw;
	sprite->flg = 0;
	return 1;
}

/* Takes the tidal wave's shadow away (its sprite flags cleared). */
Sint32 scWorldMapSetUpWave(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* Once the scene's first temporary flag is set, the tidal wave loses its shadow
   and rolls slowly 30 pixels west and 8 south. */
Bool scWorldMapSendWaveIn(ACTOR *actor)
{
	FIXED x, z;

	if(!fdScriptGetFlag(FLAG_SCENE_TEMP_00))
		return TRUE;
	fdActorSetSprFlags(actor, 0);
	x = actor->x.fixed;
	actor->acceleration = toFIXED(1) / 16;
	actor->speedLimit = toFIXED(1) / 16;
	z = actor->z.fixed;
	scFieldActorSetMoveTargetFX(
		actor, x - toFIXED(30), actor->y.fixed, z + toFIXED(8));
	return FALSE;
}

extern ACTOR_PLACE WorldMapActorPlacements[];
extern ACTOR_PLACE WorldMap_CollapsePlacements[];
extern ACTOR_PLACE WorldMap_DriftPlacements[];
extern ACTOR_PLACE WorldMap_FirstVoyagePlacements[];
extern ACTOR_PLACE WorldMap_DriftingIdejimaPlacements[];

/* The story's scenes the world map plays on arrival that no other scene's exit
   leads to; the others it tests by scene_exits.h's names. */
typedef enum WORLD_MAP_ENTRANCE {
	WORLD_MAP_PIERS_DJINN = 70,
	WORLD_MAP_BANDIT_BEATEN = 73, /* after the battle with the bandit */
	WORLD_MAP_BANDIT_LOST = 74,
	WORLD_MAP_DEBUG_FLIGHT = 98, /* the debug menu's: the ship flying */
	WORLD_MAP_DEBUG_SAILING = 99
} WORLD_MAP_ENTRANCE;

/* The exit the Venus Lighthouse's collapse leaves by, to the world map's own
   entrance where Idejima drifts; the story's other exits are
   scene_exits.h's. */
typedef enum WORLD_MAP_EXIT {
	WORLD_MAP_TO_IDEJIMA_DRIFTING = 64
} WORLD_MAP_EXIT;

void scShipTakeFlight(void);

const ACTOR_PLACE *scGetActorPlacements(void)
{
	switch(MainRoutineStateWork.entrance) {
	case WORLD_MAP_VENUS_COLLAPSE:
		return WorldMap_CollapsePlacements;
	case WORLD_MAP_DRIFT_NORTH:
		return WorldMap_DriftPlacements;
	case WORLD_MAP_FIRST_VOYAGE:
		if(!fdScriptGetFlag(FLAG_SHIP_CAN_SAIL))
			return WorldMap_FirstVoyagePlacements;
	case WORLD_MAP_IDEJIMA_DRIFTING:
		return WorldMap_DriftingIdejimaPlacements;
	default:
		return WorldMapActorPlacements;
	}
}

/* When the ship takes to the air over the World Map, "Hover" is shown while it
   rises, and the PP gauge appears, since flying uses the party's PP. */
void scWorldMapSetUpFlyingShip(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*leader;

	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_FLYING_SHIP;
	if(fdScriptGetFlag(FLAG_SHIP_HOVERING)) {
		fdWorldMapFreeze();
		mnMessageArgumentPush(ABILITY_HOVER, MESSAGE_ARGUMENT_ABILITY);
		/* "Hover" */
		mnMessageShowCentered((Sint32)MsgCastAbilityName, 4);
	}
	if((leader = fdActorGet(syFieldPartyGetLeaderActor())) != NULL)
		fdActorStartMovementModeScript(leader);
	scShipTakeFlight();
	if(fdScriptGetFlag(FLAG_SHIP_HOVERING))
		state->climbing = 0;
	else
		state->climbing = 1;
	if(fdScriptGetFlag(FLAG_SHIP_HOVERING)) {
		mnMessageCloseFinishedWindows();
		fdWorldMapReloadPendingAreaGraphics();
	}
	scGaugeInit();
	state->fieldMenu = 0;
	state->pauseMenu = 0;
	state->aButton = 0;
	state->encounter = 0;
	state->psynergy = 0;
}

/* The ship sails the map again, out of flight: the PP gauge goes, and a party
   with no PP left at all gives the leader 1 back. */
void scWorldMapSetUpShip(void)
{
	Sint32 count;
	Sint32 pp;
	Sint32 i;

	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP;
	scWorldMapEndFlight();
	scGaugeEnd();
	fdActorSetSprFlags(fdActorGet(ACTOR_SHIP), 0);
	count = gmPartyCountMembers();
	pp = 0;
	for(i = 0; i < count; i++) {
		/* One member's PP. */
		pp += syUnitGet(MainRoutineStateWork.party[i])->pp;
	}
	if(pp == 0)
		gmUnitAddPp(MainRoutineStateWork.leaderActorId, 1);
}

/* The leader jumps aboard the ship when it is the actor ahead of them or under
   16 pixels away. */
void scWorldMapBoardShip(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*leader;
	ACTOR		*ship;
	Sint32		 ahead, dx, dz, distanceSquared;

	if(syGameFlagGet(FLAG_SHIP_CAN_SAIL) == 0)
		return;
	ahead = fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ship = fdActorGet(ACTOR_SHIP);
	dx = FIXEDtoINT(leader->x.fixed - ship->x.fixed);
	dz = FIXEDtoINT(leader->z.fixed - ship->z.fixed);
	distanceSquared = dx * dx + dz * dz;
	if(ahead != ACTOR_SHIP && distanceSquared > 255)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP;
	MainRoutineStateWork.ownerOverride = 1;
	fdScriptSetFlag(FLAG_SAILING);
	MainRoutineStateWork.shipX = ship->x.fixed;
	MainRoutineStateWork.shipZ = ship->z.fixed;
	MainRoutineStateWork.shipFacing = ship->facing;
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	syWait(6);
	fdScriptPlaySnd(SE_JUMP);
	fdActorSetSprFlags(leader, 0);
	fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
	leader->speedLimit = toFIXED(3);
	leader->acceleration = toFIXED(2);
	leader->velY = toFIXED(4);
	leader->groundHeight = toFIXED(4);
	fdActorStartIdle(leader);
	scFieldActorSetMoveTargetFX(leader, ship->x.fixed, 0, ship->z.fixed);
	scFieldActorWaitForMove(leader);
	fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
	syWait(6);
	leader->groundHeight = 0;
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdChooseMusic();
	gmPlayMusic();
	fdScriptEnd();
	state->aButton = 0;
#if BOARDING_SHIP_CLEARS_STEP_TRIGGER
	state->stepCue = 0;
#endif
	syFieldWorldMapStartZoom(toFIXED(1), 30);
}

/* The leader jumps ashore from the ship, where the shore ahead lets them
   land. */
void scWorldMapLeaveShip(void)
{
	ACTOR		*leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR		*ship = fdActorGetById(ACTOR_SHIP);
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 metatile = fdWorldMapGetCollision(&ship->x.fixed);
	FIXED		 position[3];
	Sint32		 shore = 0;
	Sint32		 blocked;
	WORLD_MAP_BLOCK	 *block;
	WORLD_MAP_BLOCKS *blocks = (WORLD_MAP_BLOCKS *)MetatileTiles;
	Uint8			  flags;

	position[0] = ship->x.fixed;
	position[1] = ship->y.fixed;
	position[2] = ship->z.fixed;
	syVectorAddPolarOffset(toFIXED(24), ship->facing, position);
	blocked = 0;
	block = &blocks->blocks[(position[0] / toFIXED(WORLD_MAP_METATILE_PIXELS) &
								WORLD_MAP_VIEW_METATILES - 1) +
		(position[2] / toFIXED(WORLD_MAP_METATILE_PIXELS) &
			WORLD_MAP_VIEW_METATILES - 1) *
			WORLD_MAP_VIEW_METATILES];
	if(metatile > WORLD_MAP_COLLISION_WALKABLE_LAST) {
		blocked = 1;
	} else {
		/* Collisions 1 to 3, the open sea (BATTLE_ENVIRONMENT_SEA's), short of
		   WORLD_MAP_COLLISION_SEA_LAST. */
		if((Uint32)(metatile - WORLD_MAP_COLLISION_SEA_FIRST) <= 2)
			shore = 1;
		if((block->unusedAfterPlace & WORLD_MAP_BLOCK_LANDING) == 0 &&
			shore != 0) {
			blocked = 1;
		} else {
			position[0] += toFIXED(6);
			blocked |= fdWorldMapIsWalkable(ship, position);
			position[0] += -toFIXED(12);
			blocked |= fdWorldMapIsWalkable(ship, position);
			position[0] += toFIXED(6);
			position[1] += toFIXED(6);
			blocked |= fdWorldMapIsWalkable(ship, position);
			position[1] += -toFIXED(12);
			blocked |= fdWorldMapIsWalkable(ship, position);
			position[1] += toFIXED(6);
			blocked |= fdWorldMapIsWalkable(ship, position);
		}
	}
	if(blocked == 0) {
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldWorldMapStartZoom(toFIXED(1), 10);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WORLD_MAP;
		MainRoutineStateWork.ownerOverride = 0;
		fdScriptClearFlag(FLAG_SAILING);
		MainRoutineStateWork.shipX = ship->x.fixed;
		MainRoutineStateWork.shipZ = ship->z.fixed;
		MainRoutineStateWork.shipFacing = ship->facing;
		flags = leader->motionFlg;
		leader->x.fixed = ship->x.fixed;
		leader->y.fixed = toFIXED(4);
		leader->z.fixed = ship->z.fixed;
		leader->motionFlg = 0;
		fdActorSetSprFlags(leader, 0);
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		syWait(6);
		fdScriptPlaySnd(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velY = toFIXED(4);
		leader->motionFlg = flags;
		fdActorStartIdle(leader);
		scFieldActorSetMoveTargetFX(leader, position[0], 0, position[2]);
		scFieldActorWaitForMove(leader);
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		syWait(6);
		fdScriptEnd();
		fdChooseMusic();
		gmPlayMusic();
		state->aButton = 0;
	}
}

/* Sailing up to the landing at Atteka Inlet moors the ship there: the screen
   fades and the party goes ashore into the inlet, by the ship's cabins. Where
   the ship lay is kept, to find it there again; a flying ship comes down to the
   water first. */
void scWorldMapMoorAtAttekaInlet(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*ship = fdActorGet(ACTOR_SHIP);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_FLYING_SHIP)
		scWorldMapSetUpShip();
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	MainRoutineStateWork.shipX = ship->x.fixed;
	MainRoutineStateWork.shipZ = ship->z.fixed;
	MainRoutineStateWork.shipFacing = 0;
	MainRoutineStateWork.ownerOverride = 0;
	fdScriptClearFlag(FLAG_SAILING);
	fdScriptPlaySnd(SE_MAP_EXIT);
	syFieldScriptSetExitRequest(state->stepCue);
}

/* The actor walks into the leader and is taken off the stage, as a party member
   falls back in. */
static __inline__ void scActorJoinLeader(Sint32 actor)
{
	ACTOR *leader;

	fdActorSetAnimation(actor, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0)
		fdActorSetDestination(
			actor, leader->x.part[0].integer, leader->z.part[0].integer);
	fdActorWaitForMoveById(actor);
	fdActorSetPos(actor, 0, 0);
}

/* Once Jupiter Lighthouse is lit and before the parties meet at Contigo, Jenna
   stops Felix leaving by any other exit: they must go to Contigo. */
void scWorldMapRefuseToRunAway(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;
	Sint32 southwest = FACING_SOUTHWEST;
	Sint32 west = FACING_WEST;

	if(fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT) &&
		!fdScriptGetFlag(FLAG_REUNION_AT_CONTIGO)) {
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		x = actor->x.fixed;
		z = actor->z.fixed;
		x >>= 20;
		z >>= 20;
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(fdScriptGetFlag(FLAG_JENNA_POINTED_TO_CONTIGO)) {
			syFieldWorldMapStartZoom(toFIXED(7) / 5, 6);
			fdActorPlaceOnActor(
				ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
			syWait(1);
			fdActorWalkToAndWait(ACTOR_JENNA, (x << 4) + 24, (z << 4) - 12);
			fdActorFaceDirection(ACTOR_JENNA, southwest, 0);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_NORTHEAST);
			fdWorldMapFreeze();
			syFieldScriptSetMessageId((Sint32)MsgWeShouldnT);
			/* "We shouldn't run from Isaac anymore, Felix." */
			fdScriptShowMessage(ACTOR_JENNA, 0);
			fdWorldMapReloadPendingAreaGraphics();
			scActorJoinLeader(ACTOR_JENNA);
			syWait(1);
			fdActorWalkToAndWait(
				MainRoutineStateWork.leaderActorId, 7312, 8192);
			syFieldWorldMapStartZoom(toFIXED(1), 6);
		} else {
			Sint32 besideZ;

			syFieldWorldMapStartZoom(toFIXED(7) / 5, 6);
			fdActorPlaceOnActor(
				ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
			syWait(1);
			x <<= 4;
			besideZ = (z << 4) - 12;
			fdActorWalkToAndWait(ACTOR_JENNA, x + 24, besideZ);
			fdActorFaceDirection(ACTOR_JENNA, southwest, 0);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_NORTHEAST);
			fdWorldMapFreeze();
			syFieldScriptSetMessageId((Sint32)MsgYouReNot);
			/* "You're not planning on running away from Isaac again, are
			   you?" */
			fdScriptShowMessage(ACTOR_JENNA, 0);
			fdWorldMapReloadPendingAreaGraphics();
			fdActorFaceDirection(ACTOR_JENNA, west, 0);
			fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorWalkToAndWait(
				MainRoutineStateWork.leaderActorId, x + 12, besideZ);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_EAST);
			fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
			fdWorldMapFreeze();
			/* "We can't run anymore, Felix. I promised him that we
			   wouldn't." */
			fdScriptShowMessage(ACTOR_JENNA, 0);
			fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
			/* "Besides, they want to talk, not fight..." */
			fdScriptShowMessage(ACTOR_JENNA, 0);
			fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
			/* "They're waiting for us. Let's go to Contigo." */
			fdScriptShowMessage(ACTOR_JENNA, 0);
			fdWorldMapReloadPendingAreaGraphics();
			scActorJoinLeader(ACTOR_JENNA);
			syWait(1);
			fdActorWalkToAndWait(
				MainRoutineStateWork.leaderActorId, 7312, 8192);
			fdScriptSetFlag(FLAG_JENNA_POINTED_TO_CONTIGO);
			syFieldWorldMapStartZoom(toFIXED(1), 6);
		}
		syWait(8);
		fdScriptEnd();
	} else {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdScriptPlaySnd(SE_MAP_EXIT);
		syFieldScriptSetExitRequest(state->stepCue);
	}
}

/* The actor slot the pond's coins take when they show
   (scWorldMapGazeIntoPond). */
enum { ACTOR_POND_COINS = 79 };

/* Searching the open sea ahead of the ship, which the text calls the pond: one
   time in sixteen there are coins in the shallows, one in sixteen the boat
   rocks, and otherwise one of the pond's lines. No checking message comes first
   (CUE_SUBJECT_OWN_EFFECT). */
void scWorldMapGazeIntoPond(void)
{
	Sint32 roll = syRandomBelow(16);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	/* "Felix gazed into the pond..." */
	mnMessageShowCentered((Sint32)MsgGazedIntoThe, 1);
	if(roll == 0) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;
		Sint32		 slot = ACTOR_POND_COINS;
		ACTOR		*actor = fdActorGet(syFieldPartyGetLeaderActor());
		UNIT		*unit = syUnitGet(MainRoutineStateWork.leaderActorId);
		Sint32		 amount =
			(((unit->level * syRandom()) << 1) >> 16) + SCOOPED_COINS + 1;
		FIXED position[3];

		/* "There are some coins in the shallows!" */
		mnMessageShowCentered((Sint32)MsgThereAreSome, 1);
		position[0] = actor->x.fixed;
		position[1] = actor->y.fixed;
		position[2] = actor->z.fixed;
		syFieldVectorAddPolarOffset(16, actor->facing, position);
		actor = fdActorGet(ACTOR_POND_COINS);
		if(actor == NULL) {
			actor = scFieldActorCreate(
				SPRITE_UNSEEN_MARKER, position[0], position[1], position[2]);
			if(actor == NULL)
				return;
			state->actors[slot] = actor;
		} else {
			actor->x.fixed = position[0];
			actor->y.fixed = position[1];
			actor->z.fixed = position[2];
		}
		scFieldActorShowItemIconById(ACTOR_POND_COINS, amount);
		state->scoopedItem = amount;
		fdTalkToActor(FIELD_TALK_SILENT | slot);
	} else if(roll == 1) {
		fdMapGridSetShake(3, 5, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		syFieldScriptWait(30);
		/* "The boat's rocking! This thing's dangerous! I almost fell out!" */
		mnMessageShowCentered((Sint32)MsgTheBoatS, 1);
	} else {
		mnMessageShowCentered((Sint32)MsgTheBoatS + roll / 2, 1);
	}
	fdScriptEnd();
}

/* Searching the Golden Boots picks them up. */
void scWorldMapPickUpGoldenBoots(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(Value3((Sint32 (*)())fdScriptPickUpItem, ACTOR_GOLDEN_BOOTS,
		   ITEM_GOLDEN_BOOTS, -1) == 0)
		fdScriptClearFlag(FLAG_GOLDEN_BOOTS_ON_MAP);
	fdScriptEnd();
}

/* Places the Golden Boots on the map as an item icon
   (FLAG_GOLDEN_BOOTS_ON_MAP). */
void scWorldMapShowGoldenBoots(void)
{
	ACTOR	  *actor;
	FIELD_SPR *sprite;
	Sint32	   a, id;

	scFieldActorShowItemIconById(ACTOR_GOLDEN_BOOTS, ITEM_GOLDEN_BOOTS);
	actor = fdActorGet(ACTOR_GOLDEN_BOOTS);
	sprite = actor->spr;
	sprite->retainCacheEntry = 1;
	fdActorSetSprFlags(fdActorGet(ACTOR_GOLDEN_BOOTS), 0);
	a = FACING_SOUTH;
	id = ACTOR_GOLDEN_BOOTS;
	fdActorSetPosAndFacing(id, toFIXED(10168), toFIXED(9648), a);
	fdScriptSetFlag(FLAG_GOLDEN_BOOTS_ON_MAP);
}

/* Walking onto Prox from the World Map: coming from the north, the party enters
   by its northern way in (SceneId_ProxMold), and from the south by its south
   gate. Yallam and Daila choose their way in the same way. */
void scWorldMapEnterProx(Sint32 id)
{
	scWorldMapSelectExitByZ(
		id, WORLD_MAP_TO_PROX_MOLD, WORLD_MAP_TO_PROX_SOUTH_GATE);
}

/* Walking onto Yallam from the World Map: coming from the north, the party
   enters by its north side, and from the south by its south side. */
void scWorldMapEnterYallam(Sint32 id)
{
	scWorldMapSelectExitByZ(
		id, WORLD_MAP_TO_YALLAM_NORTH, WORLD_MAP_TO_YALLAM_SOUTH);
}

/* Walking onto Daila from the World Map: coming from the north, the party
   enters by its north side, and from the south by its south side. */
void scWorldMapEnterDaila(Sint32 id)
{
	scWorldMapSelectExitByZ(
		id, WORLD_MAP_TO_DAILA_NORTH, WORLD_MAP_TO_DAILA_SOUTH);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

extern Sint32 WorldMap_EchoScript[];

Sint32 scInit(void)
{
	FIELD_STATE *state;

	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0 &&
		syGameFlagGet(FLAG_SHIP_CAN_SAIL) != 0 &&
		MainRoutineStateWork.ownerOverride == 0 &&
		MainRoutineStateWork.shipX != 0) {
		ACTOR *ship = fdActorGetById(ACTOR_SHIP);

		ship->x.fixed = MainRoutineStateWork.shipX;
		ship->z.fixed = MainRoutineStateWork.shipZ;
		ship->facing = MainRoutineStateWork.shipFacing;
		ship->tmr = 0;
		ship->userValue = 0;
	}
	if(MainRoutineStateWork.ownerOverride != 0)
		syFieldWorldMapStartZoom(toFIXED(1), 1);
	syGameFlagSet(FLAG_RETREAT_BLOCKED);
	state = Ram_HeapSlots->fieldState;
	state->transition =
		SCENE_TRANSITION(TRANSITION_CENTRE_LINE, CENTRE_LINE_STEADY);
	state->transitionFrm = 16;
	sySynch(1);
	fdCameraSetSpeed(8);
	syGameFlagClear(FLAG_SHOW_AREA_NAME);
	scWorldMapStartPlacedActors();
	if(syGameFlagGet(FLAG_ALHAFRA_SHIP_GONE))
		fdSetWorldMapWindowMetatileTile(0, 240, 336, 288);
	if(syGameFlagGet(FLAG_JUPITER_BEACON_LIT)) {
		fdSetWorldMapWindowMetatileTile(0, 224, 208, 256);
		fdSetWorldMapWindowMetatileTile(16, 224, 224, 256);
	}
	if(syGameFlagGet(FLAG_GOLDEN_BOOTS_ON_MAP))
		scWorldMapShowGoldenBoots();
	switch(MainRoutineStateWork.entrance) {
	case WORLD_MAP_FROM_GAIA_ROCK_SUSA:
		if(syGameFlagGet(FLAG_SERPENT_SLAIN))
			syGameFlagSet(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE);
		break;
	case WORLD_MAP_FROM_MADRA:
	case WORLD_MAP_BANDIT_BEATEN:
	case WORLD_MAP_BANDIT_LOST:
		if(syGameFlagGet(FLAG_BANDIT_ON_WORLD_MAP))
			scWorldMapFightBanditAndThieves(MainRoutineStateWork.entrance);
		break;
	case WORLD_MAP_IDEJIMA_DRIFTING:
		scWorldMapShowIdejimaDrifting();
		break;
	case WORLD_MAP_CARRIED_TO_AQUA_ROCK:
		scWorldMapCarryPartyToAquaRock();
		break;
	case WORLD_MAP_FROM_LOHO:
		if(!fdScriptGetFlag(FLAG_SHIP_CARRIES_CANNON) &&
			syGameFlagGet(FLAG_LOHO_WALL_BREACHED) != 0) {
			syGameFlagSet(FLAG_SHIP_CARRIES_CANNON);
			scWorldMapDeliverLohoCannon();
		}
		break;
	case WORLD_MAP_VENUS_COLLAPSE:
		scWorldMapWatchVenusLighthouseCollapse();
		break;
	case WORLD_MAP_DRIFT_NORTH:
		scWorldMapDriftNorthOfContinent();
		break;
	case WORLD_MAP_PIERS_DJINN:
		gmPartyGivePiersDjinn();
		break;
	case WORLD_MAP_FROM_DAILA_SOUTH:
		if(syGameFlagGet(FLAG_DJINNI(DJINNI_ECHO)) == 0) {
			fdActorSetScript(ACTOR_ECHO, WorldMap_EchoScript);
			scWorldMapMeetEcho();
		}
		break;
	case WORLD_MAP_ALHAFRA_MANOR_VIEW:
		scWorldMapShowAirsRockFromAlhafra();
		break;
	case WORLD_MAP_FIRST_VOYAGE:
		scWorldMapTakeTheTiller();
		break;
	case WORLD_MAP_SAIL_TO_DECK:
		scWorldMapSailToShipDeck();
		break;
	case WORLD_MAP_JUPITER_BEACON:
		scWorldMapPlayLighthouseTransition(LIGHTHOUSE_VIEW_JUPITER);
		scFieldActorSetSprCol(fdActorGetById(ACTOR_PIERS), SPRITE_COLOR_HIDDEN);
		fdActorSetPos(ACTOR_PIERS, 7277, 7300);
		sySynch(1);
		syFieldScriptSetMessageId((Sint32)MsgIVeHeard);
		/* "I've heard tales of this..." */
		fdScriptShowMessage(ACTOR_PIERS, 0);
		syFieldScriptWait(20);
		/* "But I had no idea it would be so spectacular." */
		fdScriptShowMessage(ACTOR_PIERS, 0);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionFrames();
		fdScriptSetDestination(SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON, 9);
		break;
	case WORLD_MAP_DEBUG_FLIGHT:
		MainRoutineStateWork.ownerOverride = 1;
		fdScriptSetFlag(FLAG_SHIP_CAN_SAIL);
		fdScriptSetFlag(FLAG_SHIP_FLIES);
		fdScriptSetFlag(FLAG_DEBUG_STEPS_OFF);
		/* Flag 0x11d changes nothing: no code of this game reads it, and only
		   these two debug arrivals set it. */
		fdScriptSetFlag(0x11d);
		fdScriptSetDestination(
			SCENE_WORLD_MAP, WORLD_MAP_FROM_NORTHERN_REACHES_NORTH);
		break;
	case WORLD_MAP_DEBUG_SAILING:
		syGameFlagSet(FLAG_DEBUG_STEPS_OFF);
		syGameFlagSet(0x11d);
		break;
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Echo's lesson on Djinn: "After bringing a Djinni into your party, you must
   set it to somebody." and the Djinn menu, then "Next, I'll show you how you
   can use us in battle." and the menu again, and a battle. With
   FLAG_ALREADY_KNOWS_DJINN set he says the lesson's second copy of its
   lines. */
void scWorldMapExplainGivingDjinn(void)
{
	fdWorldMapReloadPendingAreaGraphics();
	syFieldWorldMapStartZoom(toFIXED(1), 6);
	fdScriptWaitForZoomTransition();
	fdWorldMapFreeze();
	fdActorRunRepeatedMotion(ACTOR_ECHO, 2);
	syFieldScriptSetMessageId(fdScriptGetFlag(FLAG_ALREADY_KNOWS_DJINN) *
			((Sint32)MsgDjinnGiveTutorialB - (Sint32)MsgDjinnGiveTutorialA) +
		(Sint32)MsgAfterBringingA);
	fdScriptShowMessage(ACTOR_ECHO, 0);
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_MENU_CURSOR_MOVE);
	mnAnimateSelectionToEntry(0, 2);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	scFieldActorJump(ACTOR_ECHO, 4, 30);
	{
		Sint32 flag = fdScriptGetFlag(FLAG_ALREADY_KNOWS_DJINN);
		Sint32 base = (Sint32)MsgNextILl;

		syFieldScriptSetMessageId(flag *
				((Sint32)MsgDjinnGiveTutorialB -
					(Sint32)MsgDjinnGiveTutorialA) +
			base);
	}
	fdScriptShowMessage(ACTOR_ECHO, 0);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	syFieldScriptWait(30);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_EARLY_STORY, 5);
}

/* The bandit and the thieves from Vault, after a linked game, want Isaac. */
void scWorldMapFightBanditAndThieves(Sint32 entrance)
{
	Sint32 south, north, facingSouth, southB, northB;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(entrance == WORLD_MAP_FROM_MADRA) {
		north = FACING_NORTH;
		fdActorSetPosAndFacing(
			ACTOR_BANDIT, toFIXED(10168), toFIXED(9000), north);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdWorldMapFreeze();
		fdScriptSetMessageId((Sint32)MsgHeyWaitA);
		/* "Hey! Wait a minute!" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdWorldMapReloadPendingAreaGraphics();
		syFieldWorldMapStartZoom(toFIXED(31) / 20, 16);
		fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdActorWalkByAndWait(ACTOR_FELIX, 0, 32);
		fdActorFaceDirection(ACTOR_FELIX, north, 0);
		fdScriptWaitForZoomTransition();
		fdActorWaitForMoveById(ACTOR_FELIX);
		fdWorldMapFreeze();
		fdActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, north);
		fdActorWaitForScriptEndById(ACTOR_KRADEN);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, north);
		south = FACING_SOUTH;
		fdScriptWait(10);
		fdActorSetPosAndFacing(
			ACTOR_BANDIT, toFIXED(10168), toFIXED(9600), south);
		fdActorSetPosAndFacing(
			ACTOR_THIEF_A, toFIXED(10168), toFIXED(9600), south);
		fdActorSetPosAndFacing(
			ACTOR_THIEF_B, toFIXED(10168), toFIXED(9600), south);
		fdActorWalkBy(ACTOR_BANDIT, 0, 16);
		fdActorWalkBy(ACTOR_THIEF_A, -16, 16);
		fdActorWalkByAndWait(ACTOR_THIEF_B, 16, 16);
		fdActorFaceDirection(ACTOR_BANDIT, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_THIEF_A, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_THIEF_B, FACING_SOUTH_SOUTHEAST, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdActorWaitForMoveById(ACTOR_BANDIT);
		fdActorWaitForMoveById(ACTOR_THIEF_A);
		fdActorWaitForMoveById(ACTOR_THIEF_B);
		fdWorldMapFreeze();
		/* "Who are you?" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		fdScriptWait(5);
		/* "You were talking about Isaac, weren't you?" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorTurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
		fdScriptWait(30);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_THIEF_A, 2);
		fdActorFaceDirection(ACTOR_FELIX, north, 0);
		fdActorFaceDirection(ACTOR_KRADEN, north, 0);
		fdScriptWait(5);
		/* "Don't play dumb... We all heard you." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "Yes, we know Isaac. What about him?" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Well, why don't you tell us where we can find him?" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "Why should we tell you anything?" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		/* "Isaac took good care of us when we were in Vault, and we'd like
		   to..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "Ah, yes... Well... I'm afraid we don't know where you can find
		   Isaac..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		/* "Oh, is that so? Well, I guess we'll just have to give you the
		   reward..." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorTurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
		fdScriptWait(30);
		/* "You've got a grudge against Isaac, and you're going to take it
		   out..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		{
			Sint32 facing = FACING_NORTH;

			fdActorFaceDirection(ACTOR_FELIX, facing, 0);
			fdActorFaceDirection(ACTOR_KRADEN, facing, 0);
		}
		/* "Oh yeah, you could call it a grudge... A serious grudge." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorWalkBy(ACTOR_FELIX, 8, -8);
		fdActorWalkBy(ACTOR_KRADEN, -8, 8);
		fdActorWaitForMoveById(ACTOR_FELIX);
		fdActorWaitForMoveById(ACTOR_KRADEN);
		{
			Sint32 facing = FACING_NORTH;

			fdActorFaceDirection(ACTOR_FELIX, facing, 0);
			fdActorFaceDirection(ACTOR_KRADEN, facing, 0);
		}
		{
			Sint32 emote = EMOTE_IN_FRONT | EMOTE_WINCE;

			fdActorShowEmote(ACTOR_FELIX, emote, 30);
		}
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		/* "But don't worry... We're just going to give you a little sample
		   of..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
		syFieldScriptSetAfterBattleDestination(
			SCENE_WORLD_MAP, WORLD_MAP_BANDIT_BEATEN);
		fdScriptSetDefeatDestination(SCENE_WORLD_MAP, WORLD_MAP_BANDIT_LOST);
		fdWorldMapReloadPendingAreaGraphics();
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 3);
	} else if(entrance == WORLD_MAP_BANDIT_LOST) {
		fdScriptClearFlag(FLAG_BANDIT_ON_WORLD_MAP);
		facingSouth = FACING_SOUTH;
		fdActorSetPosAndFacing(
			ACTOR_BANDIT, toFIXED(10168), toFIXED(9616), facingSouth);
		fdActorSetPosAndFacing(
			ACTOR_THIEF_A, toFIXED(10152), toFIXED(9616), facingSouth);
		fdActorSetPosAndFacing(
			ACTOR_THIEF_B, toFIXED(10184), toFIXED(9616), facingSouth);
		fdActorSetPosAndFacing(
			ACTOR_KRADEN, toFIXED(10152), toFIXED(9664), FACING_NORTHEAST);
		fdActorSetPosAndFacing(
			ACTOR_FELIX, toFIXED(10168), toFIXED(9648), FACING_NORTHWEST);
		syScriptCameraFollowActor(ACTOR_FELIX, 0);
		syFieldWorldMapStartZoom(toFIXED(31) / 20, 1);
		fdActorSetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
		fdActorSetSprFlags(fdActorGet(ACTOR_FELIX), 0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdWorldMapFreeze();
		fdScriptSetMessageId((Sint32)MsgItLooksLike);
		fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
		fdScriptWait(5);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "It looks like Isaac runs with a pretty feeble crowd..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_THIEF_A, facingSouth);
		fdScriptWait(10);
		/* "Well, we have been working out. Maybe Isaac won't be so tough
		   this..." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_THIEF_B, facingSouth);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_THIEF_B, 2);
		fdScriptWait(5);
		/* "So you think we just got a lot stronger?" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_THIEF_A, ANIM_NOD);
		fdScriptWait(10);
		fdActorFaceDirectionAndWait(ACTOR_BANDIT, facingSouth);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		fdScriptWait(5);
		/* "You let Isaac know we're looking for him!" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		/* "The band of thieves he busted in Vault are coming to repay the
		   favor!" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_THIEF_A, ANIM_NOD);
		fdScriptWait(10);
		/* "And we're not messing around!" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Be sure you tell him! And send him this way!" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		fdScriptWait(40);
		fdScriptSetDestination((Sint16)MainRoutineStateWork.restartScene,
			(Sint16)MainRoutineStateWork.restartEntrance);
		FieldStateWork->transition = 0;
	} else {
		fdScriptClearFlag(FLAG_BANDIT_ON_WORLD_MAP);
		southB = FACING_SOUTH;
		fdActorSetPosAndFacing(
			ACTOR_BANDIT, toFIXED(10168), toFIXED(9632), southB);
		northB = FACING_NORTH;
		fdActorSetPosAndFacing(
			ACTOR_THIEF_A, toFIXED(10152), toFIXED(9632), southB);
		fdActorSetPosAndFacing(
			ACTOR_THIEF_B, toFIXED(10184), toFIXED(9632), southB);
		fdActorSetPosAndFacing(
			ACTOR_KRADEN, toFIXED(10160), toFIXED(9680), northB);
		fdActorSetPosAndFacing(
			ACTOR_FELIX, toFIXED(10176), toFIXED(9680), northB);
		syScriptCameraFollowActor(ACTOR_FELIX, 0);
		syFieldWorldMapStartZoom(toFIXED(3) / 2, 1);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdWorldMapFreeze();
		fdScriptSetMessageId((Sint32)MsgWeLostAgain);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		fdScriptWait(5);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "We lost again..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "So, boss... You, uh... You think maybe it was a mistake going
		   after..." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "This means we're going to jail again, doesn't it?" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		fdScriptWait(5);
		/* "Aw... Why do we have to go back to jail. I hate it there..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_THIEF_A, FACING_EAST);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "It's not like we really hurt anyone... After all, we lost!" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_BANDIT, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		/* "Yeah... And it's not like we stole anything...this time..." */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorFaceDirectionAndWait(ACTOR_THIEF_B, FACING_WEST);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "We're not gonna have to go to jail, are we?" */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorFaceDirection(ACTOR_BANDIT, southB, 0);
		fdActorFaceDirection(ACTOR_THIEF_A, southB, 0);
		fdActorFaceDirection(ACTOR_THIEF_B, southB, 0);
		/* "You're thieves!" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetAnimation(ACTOR_BANDIT, ANIM_NOD);
		fdActorSetAnimation(ACTOR_THIEF_A, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_THIEF_B, ANIM_NOD);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "If we asked around, we could probably find a dozen reasons to
		   have..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdScriptWait(40);
		fdActorTurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_A, 0);
		fdActorFaceDirection(ACTOR_THIEF_B, FACING_WEST, 0);
		fdActorShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdActorFaceDirectionAndWait(ACTOR_BANDIT, southB);
		fdScriptWait(20);
		/* "Are you guys, like, wandering sheriffs or something?" */
		fdScriptOpenMessage(ACTOR_BANDIT, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			fdActorShowEmote(
				ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
			/* "Because we could be really useful to you if you are. We know
			   all..." */
			syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
			(FieldStateWork->msg)++;
		} else {
			fdActorShowEmote(
				ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
			(FieldStateWork->msg)++;
			/* "And even if you're not, we could be really useful to
			   you......" */
			syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		}
		fdActorTurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
		fdScriptWait(40);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "You can't honestly believe we would let you join us!" */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_THIEF_A, 2);
		fdActorFaceDirection(ACTOR_THIEF_A, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_THIEF_B, FACING_SOUTH_SOUTHWEST, 0);
		fdScriptWait(5);
		/* "Hey, come on! We haven't done anything wrong, really! Not yet..." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
		fdActorSetAnimationAndWait(ACTOR_THIEF_B, ANIM_SHAKE_HEAD);
		/* "Yeah! We didn't have enough time to do anything bad before we
		   ran..." */
		syFieldScriptShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		scWorldMapShowGoldenBoots();
		fdScriptWait(5);
		fdActorShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
		/* "Yeah, so give us a break!" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorTurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_A, 0);
		fdScriptWait(40);
		fdActorSetAnimation(ACTOR_THIEF_A, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		fdActorTurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_B, 0);
		fdScriptWait(40);
		fdActorSetAnimation(ACTOR_THIEF_B, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_BANDIT, ANIM_NOD);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_BANDIT, 2);
		fdActorFaceDirection(ACTOR_THIEF_A, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_THIEF_B, FACING_SOUTH, 0);
		fdActorFaceDirectionAndWait(ACTOR_BANDIT, FACING_SOUTH);
		fdScriptWait(5);
		/* "So, uh... Bye! See you later!" */
		syFieldScriptShowMessageThenWait(ACTOR_BANDIT, 0, 5);
		fdActorWalkBy(ACTOR_BANDIT, -32, 0);
		fdActorWalkBy(ACTOR_THIEF_A, -32, 0);
		fdActorWalkByAndWait(ACTOR_THIEF_B, -32, 0);
		fdActorWalkBy(ACTOR_BANDIT, -64, -64);
		fdActorWalkBy(ACTOR_THIEF_A, -64, -64);
		fdActorWalkByAndWait(ACTOR_THIEF_B, -64, -64);
		fdActorSetPos(ACTOR_BANDIT, 0, 0);
		fdActorSetPos(ACTOR_THIEF_A, 0, 0);
		fdActorSetPos(ACTOR_THIEF_B, 0, 0);
		fdScriptWait(10);
		fdActorFaceDirectionAndWait(ACTOR_BANDIT, FACING_SOUTH);
		fdScriptWait(10);
		fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
		fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdScriptWait(10);
		/* "Will you promise to stop following Isaac and trying to get
		   revenge..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdScriptWait(10);
		fdActorSetSpeed(ACTOR_KRADEN, 1.2);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
		{
			ACTOR *actor = fdActorGet(ACTOR_FELIX);

			if(actor != NULL)
				fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
					actor->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_KRADEN);
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		fdWorldMapReloadPendingAreaGraphics();
		syFieldWorldMapStartZoom(toFIXED(1), 15);
		fdScriptEnd();
	}
}

extern Uint32 WorldMap_LohoWorkerLeaveScriptA[];
extern Uint32 WorldMap_LohoWorkerLeaveScriptB[];
extern Uint32 WorldMap_LohoWorkerLeaveScriptC[];

/* Loho's men carry the cannon aboard the ship and go home. */
void scWorldMapDeliverLohoCannon(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorGet(ACTOR_LOHO_WORKER_A)->motionFlg = ACTOR_FALLS;
	fdActorGet(ACTOR_LOHO_WORKER_B)->motionFlg = ACTOR_FALLS;
	fdActorGet(ACTOR_LOHO_WORKER_C)->motionFlg = ACTOR_FALLS;
	fdActorSetPosAndFacing(
		ACTOR_LOHO_WORKER_A, toFIXED(8913), toFIXED(5853), FACING_NORTHEAST);
	fdActorSetPosAndFacing(
		ACTOR_LOHO_WORKER_B, toFIXED(8929), toFIXED(5869), FACING_NORTHEAST);
	fdActorSetPosAndFacing(
		ACTOR_LOHO_WORKER_C, toFIXED(8929), toFIXED(5885), FACING_NORTHEAST);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_LOHO_WORKER_A, 0.2);
	fdActorSetSpeed(ACTOR_LOHO_WORKER_B, 0.2);
	fdActorSetSpeed(ACTOR_LOHO_WORKER_C, 0.2);
	fdActorWalkBy(ACTOR_LOHO_WORKER_A, 10, 0);
	fdActorWalkBy(ACTOR_LOHO_WORKER_B, 10, 0);
	fdActorWalkBy(ACTOR_LOHO_WORKER_C, 10, 0);
	fdScriptWaitTransitionFrames();
	syFieldWorldMapStartZoom(toFIXED(12) / 10, 6);
	fdScriptWaitForZoomTransition();
	fdWorldMapFreeze();
	scFieldActorWaitForMoveById(ACTOR_LOHO_WORKER_A);
	syFieldScriptWait(40);
	fdActorSetAnimation(ACTOR_LOHO_WORKER_A, ANIM_NOD);
	syFieldScriptSetMessageId((Sint32)MsgYouDonT);
	fdActorFaceDirection(ACTOR_LOHO_WORKER_B, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_LOHO_WORKER_C, FACING_NORTH_NORTHEAST, 0);
	/* "You don't have anything to worry about. We got the cannon onto
	   your..." */
	fdScriptShowMessage(ACTOR_LOHO_WORKER_A, 0);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldScriptWait(30);
	/* "Now, we're going to start excavating the ground north of the wall..." */
	fdScriptShowMessage(ACTOR_LOHO_WORKER_A, 0);
	fdActorSetAnimation(ACTOR_LOHO_WORKER_A, ANIM_NOD);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	syFieldScriptWait(60);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_LOHO_WORKER_C);
	fdActorSetSpeed(ACTOR_LOHO_WORKER_A, 0.4);
	fdActorSetSpeed(ACTOR_LOHO_WORKER_B, 0.4);
	fdActorSetSpeed(ACTOR_LOHO_WORKER_C, 0.4);
	fdActorSetScript(ACTOR_LOHO_WORKER_A, WorldMap_LohoWorkerLeaveScriptA);
	syFieldScriptWait(40);
	fdActorSetScript(ACTOR_LOHO_WORKER_B, WorldMap_LohoWorkerLeaveScriptB);
	fdActorSetScript(ACTOR_LOHO_WORKER_C, WorldMap_LohoWorkerLeaveScriptC);
	scFieldActorWaitForScriptEndById(ACTOR_LOHO_WORKER_A);
	scFieldActorWaitForScriptEndById(ACTOR_LOHO_WORKER_B);
	scFieldActorWaitForScriptEndById(ACTOR_LOHO_WORKER_C);
	fdWorldMapReloadPendingAreaGraphics();
	syFieldScriptWait(30);
	syFieldWorldMapStartZoom(toFIXED(1), 6);
	fdScriptEnd();
}

/* A message of Echo's Djinn lesson: the second set (from
   MsgDjinnGiveTutorialB), for a player who knows Djinn already, when secondSet
   is 1. */
#define TUTORIAL_SET_MESSAGE(secondSet, base) \
	((secondSet) * \
			((Sint32)MsgDjinnGiveTutorialB - (Sint32)MsgDjinnGiveTutorialA) + \
		(Sint32)(base))

/* A message of Echo's Djinn lesson, from its second set when the player held Up
   as the lesson began and Echo says "You look like you already know a little
   about us, huh?". */
#define TUTORIAL_MESSAGE(base) \
	TUTORIAL_SET_MESSAGE(fdScriptGetFlag(FLAG_ALREADY_KNOWS_DJINN), base)

/* The first Djinni, Echo, joins south of Daila, however often he is refused. */
void scWorldMapMeetEcho(void)
{
	ACTOR *djinni;
	ACTOR *leader;
	FIXED  x;
	FIXED  z;
	Sint32 owner;
	Sint32 count;
	Sint32 i;
	Sint32 flag;
	/* Echo is a Venus Djinni. */
	Sint32 element = ELEMENT_VENUS;

	djinni = fdActorGetById(ACTOR_ECHO);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	x = (leader->x.fixed - toFIXED(10120)) / 2 + toFIXED(10120);
	z = (leader->z.fixed - toFIXED(8356)) / 2 + toFIXED(8356);
	flag = fdScriptGetFlag(FLAG_DJINN_LESSON);
	if(flag == 0) {
		if(PerWork.data & KEY_UP)
			fdScriptSetFlag(FLAG_ALREADY_KNOWS_DJINN);
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		/* Echo's lesson is given to Felix, Jenna and Sheba alone. */
		gmPartyRemoveMember(ACTOR_ISAAC);
		gmPartyRemoveMember(ACTOR_GARET);
		gmPartyRemoveMember(ACTOR_IVAN);
		gmPartyRemoveMember(ACTOR_MIA);
		gmPartyRemoveMember(ACTOR_FELIX);
		gmPartyRemoveMember(ACTOR_JENNA);
		gmPartyRemoveMember(ACTOR_SHEBA);
		gmPartyRemoveMember(ACTOR_PIERS);
		gmPartyAddMember(ACTOR_FELIX);
		gmPartyAddMember(ACTOR_JENNA);
		gmPartyAddMember(ACTOR_SHEBA);
		owner = MainRoutineStateWork.leaderActorId;
		gmPartyRestoreHpPp(1);
		fdScriptSetFlag(FLAG_DJINN_LESSON);
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		gmDjinnGive(owner, element, DJINNI_INDEX(DJINNI_ECHO));
		gmDjinnRecoveryAddStandby(owner, element, DJINNI_INDEX(DJINNI_ECHO));
		fdWorldMapFreeze();
		fdActorFaceActorById(owner, ACTOR_ECHO, 0);
		fdScriptWait(10);
		scFieldActorShowEmote(owner, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		djinni->userValue = 1;
		fdActorFaceActorById(ACTOR_ECHO, owner, 0);
		sySynch(16);
		flag = fdScriptGetFlag(FLAG_ALREADY_KNOWS_DJINN);
		fdScriptSetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey));
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		fdWorldMapReloadPendingAreaGraphics();
		syFieldWorldMapStartZoom(toFIXED(12) / 10, 6);
		fdScriptWaitForZoomTransition();
		fdWorldMapFreeze();
		djinni->motionFlg = ACTOR_FALLS;
		djinni->gravity = toFIXED(1) / 4;
		djinni->speedLimit = toFIXED(1);
		djinni->acceleration = toFIXED(1);
		djinni->velY = 0;
		djinni->groundHeight = 0;
		scFieldActorSetMoveTarget(djinni, 10120, 0, 8356);
		for(i = 0; i < 16; i++) {
			djinni->scaleX += toFIXED(1) / 32;
			djinni->scaleY += toFIXED(1) / 32;
			sySynch(1);
		}
		fdActorFaceActorById(ACTOR_ECHO, owner, 0);
		fdActorFaceActorById(owner, ACTOR_ECHO, 0);
		sySynch(16);
		djinni->update = 0;
		fdActorSetSprColMode(djinni, SPRITE_COLOR_NORMAL);
		djinni->gravity = toFIXED(1);
		fdMessageShowAtActor(ACTOR_ECHO, 0);
		if(fdScriptAskYesNoWithActorAnimation(owner, 0) == 1) {
			fdActorJump(ACTOR_ECHO, 2, 20);
			fdActorJump(ACTOR_ECHO, 2, 20);
			fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren));
			fdMessageShowAtActor(ACTOR_ECHO, 0);
			if(fdScriptAskYesNoWithActorAnimation(owner, 0) == 1) {
				fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
				fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
			} else {
				fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
				fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
			}
		} else {
			fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgSeeYouKnow));
			fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		}
		scFieldActorShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdActorJump(ACTOR_ECHO, 2, 20);
		fdActorJump(ACTOR_ECHO, 2, 20);
		fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
		i = TRIG_TURN / FACING_STEP - 1;
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		scFieldActorShowEmote(owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		scFieldActorShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		fdActorRunRepeatedMotion(owner, 2);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		fdActorJump(ACTOR_ECHO, 2, 30);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		djinni->motionFlg = 0;
		scFieldActorSetMoveTargetFX(djinni, x, toFIXED(16), z);
		/* A whole turn as he rises, then half a turn as he drops. */
		while(i >= 0) {
			djinni->facing += FACING_STEP;
			sySynch(1);
			i--;
		}
		fdActorSetAnimationById(owner, ANIM_STAND);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		djinni->motionFlg = ACTOR_FALLS;
		djinni->velY = 0;
		djinni->groundHeight = 0;
		i = TRIG_TURN / 2 / FACING_STEP - 1;
		while(i >= 0) {
			djinni->facing += FACING_STEP;
			sySynch(1);
			i--;
		}
		fdActorSetAnimationById(owner, ANIM_LEADER_LEAN_BACK);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		fdActorShowEmoteById(ACTOR_ECHO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdActorFaceActorById(ACTOR_ECHO, owner, 0);
		fdActorRunRepeatedMotion(ACTOR_ECHO, 2);
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
		fdActorJump(ACTOR_ECHO, 2, 30);
		fdMessageShowAtActor(ACTOR_ECHO, 0);
		count = 0;
	plead: {
		i = fdScriptAskYesNoWithActorAnimation(owner, 0);

		if(count == 7) {
			if(i == 0) {
				fdActorJump(ACTOR_ECHO, 2, 20);
				fdActorJump(ACTOR_ECHO, 2, 20);
				fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgOhAllRight));
				fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
				goto joined;
			}
		} else if(i == 1) {
			fdActorJump(ACTOR_ECHO, 2, 20);
			fdActorJump(ACTOR_ECHO, 2, 20);
			fdScriptSetMessageId(count + TUTORIAL_MESSAGE(MsgOhComeOn));
			fdMessageShowAtActor(ACTOR_ECHO, 0);
			count++;
			goto plead;
		}
	}
		fdActorSetAnimationById(owner, ANIM_LEADER_LEAN_BACK);
		fdActorJump(ACTOR_ECHO, 2, 20);
		fdActorJump(ACTOR_ECHO, 4, 20);
		fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgIMSo));
		fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	joined:
		mnMessageArgumentPush(ABILITY_ECHO, MESSAGE_ARGUMENT_ABILITY);
		sySndPlay(MUS_A_NEW_ALLY);
		mnFieldMessageShow(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
		fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
		fdActorJump(ACTOR_ECHO, 2, 20);
		fdMessageShowAtActor(ACTOR_ECHO, 0);
		if(fdScriptAskYesNoWithActorAnimation(owner, 0) == 1)
			goto refused;
		if(fdScriptGetFlag(FLAG_ALREADY_KNOWS_DJINN))
			sySndPlay(MUS_CHILDRENS_SONG);
		else
			sySndPlay(MUS_BRIGGS_STEALS_SHIP);
		goto tutorial;
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdActorPlaceOnActor(ACTOR_ECHO, owner);
	djinni->velY = toFIXED(10);
	scFieldActorSetMoveTargetFX(djinni, x, 0, z);
	fdScriptWait(30);
	fdWorldMapFreeze();
	fdActorFaceActorById(ACTOR_ECHO, owner, 0);
	fdActorFaceActorById(owner, ACTOR_ECHO, 0);
	fdActorSetAnimationById(owner, ANIM_LEADER_LEAN_BACK);
	fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgSeeWeDjinn));
	fdActorJump(ACTOR_ECHO, 2, 20);
	fdActorJump(ACTOR_ECHO, 2, 20);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdActorRunRepeatedMotion(ACTOR_ECHO, 2);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	sySndPlay(SE_MENU_CURSOR_MOVE);
	mnAnimateSelectionToEntry(0, 2);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
	scFieldActorSetMoveTarget(djinni, 10120, 0, 8356);
	fdScriptWait(30);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdActorFaceActorById(ACTOR_ECHO, owner, 0);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdMessageShowAtActor(ACTOR_ECHO, 0);
	if(fdScriptAskYesNoWithActorAnimation(owner, 0) != 1)
		goto refused;
	fdActorSetAnimationById(owner, ANIM_LEADER_LEAN_BACK);
	fdActorRunRepeatedMotion(ACTOR_ECHO, 2);
	fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgHmmmmDoYou));
	fdMessageShowAtActor(ACTOR_ECHO, 0);
	if(fdScriptAskYesNoWithActorAnimation(owner, 0) == 1)
		goto refused;
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdActorWalkToAndWait(ACTOR_ECHO, FIXEDtoINT(x), FIXEDtoINT(z));
tutorial:
	scWorldMapExplainGivingDjinn();
	fdWorldMapReloadPendingAreaGraphics();
	return;
refused:
	fdActorSetAnimationById(owner, ANIM_LEADER_LEAN_BACK);
	fdScriptSetMessageId(TUTORIAL_MESSAGE(MsgWellThatS));
	fdActorJump(ACTOR_ECHO, 2, 20);
	fdActorJump(ACTOR_ECHO, 2, 20);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdActorShowEmoteById(ACTOR_ECHO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdActorJump(ACTOR_ECHO, 2, 20);
	fdScriptShowMessageAndWait(ACTOR_ECHO, 0);
	fdWorldMapReloadPendingAreaGraphics();
	syFieldDjinnJoinParty(ACTOR_ECHO, element, DJINNI_INDEX(DJINNI_ECHO));
	gmPlayMusic();
	fdScriptEnd();
	fdScriptClearFlag(FLAG_DJINN_LESSON);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_B);
}

extern Uint32 WorldMap_CollapseDustScript[];

/* A cloud of dust thrown up at a random spot as the earthquake that fells Venus
   Lighthouse shakes the land, at a random scale. */
void scWorldMapSpawnCollapseDust(void)
{
	ACTOR	  *dust;
	FIELD_SPR *sprite;
	Uint8	  *motionFlags;
	FIXED	   x;
	FIXED	   z;
	Sint32	   scale;

	x = toFIXED(syRandomBelow(40));
	z = toFIXED(syRandomBelow(30));
	dust = (scFieldActorCreate(
		SPRITE_DUST_CLOUD, x + toFIXED(1976), 0, z + toFIXED(7260)));
	if(dust != NULL) {
		sprite = dust->spr;
		scale = syRandomBelow(32768) + toFIXED(6) / 5;
		sprite->flg = 0;
		sprite->pri = 2;
		motionFlags = &dust->motionFlg;
		*motionFlags = 0;
		dust->scaleX = scale;
		dust->scaleY = scale;
		fdActorSelectAnimation(dust, 1);
		fdActorStartScript(dust, WorldMap_CollapseDustScript);
	}
}

/* Moves the actor by whole pixels. */
void scWorldMapShiftActor(Sint32 id, Sint32 dx, Sint32 dz)
{
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL)
		fdActorSetPos(
			id, actor->x.part[0].integer + dx, actor->z.part[0].integer + dz);
}

/* Breaks a piece of Idejima's tiles away and moves the party on it with the
   piece. */
void scWorldMapSplitIdejima(Sint32 x, Sint32 z, Sint32 dx, Sint32 dz)
{
	syFieldWorldMapSetWindowMetatileTileAndDraw(x, z, 48, 224);
	scWorldMapShiftActor(ACTOR_JENNA, dx, dz);
	scWorldMapShiftActor(ACTOR_IDEJIMA_KRADEN, dx, dz);
	scWorldMapShiftActor(ACTOR_ALEX, dx, dz);
	scWorldMapShiftActor(ACTOR_IDEJIMA, dx, dz);
	syFieldWorldMapSetWindowMetatileTileAndDraw(x + 16, z, 64, 224);
}

/* Venus Lighthouse is lit and falls; the earthquake breaks Idejima away with
   Jenna, Kraden and Alex on it. */
void scWorldMapWatchVenusLighthouseCollapse(void)
{
	scWorldMapPlayLighthouseTransition(LIGHTHOUSE_VIEW_VENUS);
	syFieldScriptSetMessageId((Sint32)MsgTheyVeDone);
	/* "They've done it, Jenna! They've lit the beacon!" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	fdMapGridSetShake(4, 4, 1);
	syFieldScriptWait(30);
	/* "What's happening, Kraden!? Why won't the ground stop shaking?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdWorldMapReloadPendingAreaGraphics();
	fdScriptPlaySnd(SE_CRASH);
	fdPaletteFadeApplySource(RGB_WHITE, 0);
	fdPaletteFadeApplyTarget(toFIXED(1), 0);
	fdPaletteFadeInterpolate(60);
	sySetFrameEvent(scWorldMapSpawnCollapseDust, EVENT_PASS_FRAME);
	fdMapGridSetShake(5, 5, 1);
	syFieldScriptWait(30);
	fdMapGridSetShake(7, 6, 1);
	syFieldScriptWait(30);
	fdWorldMapFreeze();
	fdMapGridSetShake(3, 3, 1);
	/* "Strange... I think it might actually be getting worse!" */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	fdWorldMapReloadPendingAreaGraphics();
	fdMapGridSetShake(7, 7, 1);
	fdCameraMoveTo(1988, CAMERA_KEEP, 7553, 1);
	syFieldWorldMapStartZoom(toFIXED(3) / 5, 60);
	fdCameraWaitForArrival();
	syFieldScriptWait(30);
	fdWorldMapFreeze();
	/* "I think the lighthouse is collapsing! Look! The ground around it
	   is..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "Mercury Lighthouse didn't react this way... What makes Venus
	   different?" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	fdWorldMapReloadPendingAreaGraphics();
	fdScriptPlaySnd(SE_CRASH);
	scWorldMapSplitIdejima(0, 128, 0, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_IDEJIMA_KRADEN, 5);
	fdActorSetAnimation(ACTOR_ALEX, 7);
	syWait(4);
	fdScriptPlaySnd(SE_CRASH);
	scWorldMapSplitIdejima(32, 128, 32, 0);
	syWait(4);
	scWorldMapSplitIdejima(64, 128, 32, 0);
	syWait(4);
	fdCameraMoveTo(2052, CAMERA_KEEP, 7553, 1);
	fdCameraWaitForArrival();
	fdWorldMapFreeze();
	fdMapGridSetShake(3, 3, 1);
	/* "That last tremor tore a rift in the Suhalla Range!" */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	/* "How are we going to get back to Gondowan now!?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdWorldMapReloadPendingAreaGraphics();
	fdMapGridSetShake(3, 3, 1);
	scWorldMapSplitIdejima(96, 128, 32, 0);
	syWait(20);
	scWorldMapSplitIdejima(0, 160, 32, 0);
	syWait(20);
	scWorldMapSplitIdejima(32, 160, 32, 0);
	fdCameraMoveTo(2148, CAMERA_KEEP, 7553, 1);
	fdCameraWaitForArrival();
	fdWorldMapFreeze();
	/* "Idejima... It's drifting away from the mainland!" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	/* "But...what will happen to us?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(WORLD_MAP_TO_IDEJIMA_DRIFTING);
	syFieldScriptWait(120);
}

extern Sint32 WorldMap_IdejimaWakeScript[];

/* Every sixteenth frame, a puff of spray behind the drifting Idejima. */
void scWorldMapSpawnIdejimaWake(void)
{
	ACTOR	  *island;
	ACTOR	  *spray;
	FIELD_SPR *sprite;
	Uint32	   offset;

	if((FrameCount & 15) != 0)
		return;
	island = fdActorGet(ACTOR_DRIFTING_IDEJIMA);
	spray = scFieldActorCreate(SPRITE_DUST_CLOUD, island->x.fixed - toFIXED(32),
		island->y.fixed, island->z.fixed - toFIXED(16));
	if(spray == NULL)
		return;
	spray->scaleX = toFIXED(1) / 2;
	spray->scaleY = toFIXED(1) / 2;
	sprite = spray->spr;
	if(syRandomBelow(2)) {
		FIXED back = (syRandomBelow(48)) << 16;

		spray->x.fixed -= back >> 1;
		spray->z.fixed -= back;
	} else {
		offset = syRandomBelow(32) << 16;
		spray->x.fixed += offset;
		offset = (Sint32)offset >> 1;
		spray->z.fixed += offset;
	}
	sprite->flg = 0;
	sprite->pri = ((FIELD_SPR *)island->spr)->pri;
	spray->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	spray->motionFlg = island->motionFlg;
	scFieldActorSetSprCol(spray, SPRITE_COLOR_BLUE_TINT);
	fdActorSelectAnimation(spray, 2);
	fdActorStartScript(spray, WorldMap_IdejimaWakeScript);
}

extern Uint32 WorldMap_IdejimaDriftScript[];

/* The camera follows Idejima as it drifts away north. */
void scWorldMapShowIdejimaDrifting(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_DRIFTING_IDEJIMA);
	syFieldScriptWait(60);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldWorldMapStartZoom(toFIXED(3) / 5, 1);
	actor->scaleX = toFIXED(12) / 10;
	actor->scaleY = toFIXED(12) / 10;
	syScriptCameraFollowActor(ACTOR_DRIFTING_IDEJIMA, 1);
	syWait(1);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_DRIFTING_IDEJIMA), 0);
	fdActorSetSpeed(ACTOR_DRIFTING_IDEJIMA, 0.4);
	actor->tmr = 0;
	fdActorSetScript(ACTOR_DRIFTING_IDEJIMA, WorldMap_IdejimaDriftScript);
	sySetFrameEvent(scWorldMapSpawnIdejimaWake, EVENT_PASS_FRAME);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	syFieldScriptWait(120);
	syFieldWorldMapStartZoom(toFIXED(7) / 5, 300);
	syFieldScriptWait(270);
	FieldStateWork->transitionFrm = 16;
	BG_PLTT_COLOR(0, 0) = RGB_WHITE;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(WORLD_MAP_TO_IDEJIMA_NORTH_B);
}

/* Idejima drifts north; a tidal wave rises behind it and carries it away. */
void scWorldMapDriftNorthOfContinent(void)
{
	ACTOR		   *island = fdActorGet(ACTOR_IDEJIMA);
	WORLD_MAP_VIEW *view = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	Sint32			i;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldWorldMapSetWindowMetatileTileAndDraw(64, 176, 304, 240);
	syFieldWorldMapSetWindowMetatileTileAndDraw(80, 176, 320, 240);
	island->scaleX = toFIXED(12) / 10;
	island->scaleY = toFIXED(12) / 10;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_IDEJIMA), 0);
	fdActorSetSpeed(ACTOR_IDEJIMA, 0.4);
	island->tmr = 0;
	island->acceleration = toFIXED(1) / 8;
	island->speedLimit = toFIXED(1) / 8;
	scFieldActorSetMoveTargetFX(
		island, toFIXED(10239), island->y.fixed, toFIXED(7884));
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdWorldMapFreeze();
	syFieldScriptSetMessageId((Sint32)MsgWeReSaved);
	/* "We're saved!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "So it would seem..." */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	scFieldActorSetMoveTargetFX(
		island, toFIXED(10255), island->y.fixed, toFIXED(7868));
	/* "Wait..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "What is it?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "We're going to pass north of the continent..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "She's right!" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	/* "I don't think we're going to make it!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "Oh, Kraden... Felix... What are we going to do?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdScriptPlaySnd(SE_WAVE);
	/* "???" */
	fdScriptShowMessage(MainRoutineStateWork.leaderActorId, 0);
	/* "Oh... This can't be good." */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	fdScriptPlaySnd(MUS_FATED_CONNECTION_WITH_SWORN_ENEMIES);
	fdWorldMapReloadPendingAreaGraphics();
	fdCameraSetSpeed(4);
	fdCameraMoveTo(10303, CAMERA_KEEP, 7820, 1);
	for(i = 0; i < 32; i++) {
		view->yaw -= TRIG_TURN / 512;
		view->pitch += TRIG_TURN / 512;
		fdActorGet(ACTOR_IDEJIMA)->scaleY += -(toFIXED(3) / 256);
		syWait(1);
	}
	fdWorldMapFreeze();
	fdScriptSetFlag(FLAG_SCENE_TEMP_00);
	/* "What...is it?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "It's...a tidal wave." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "A tidal wave?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "The earthquake must have caused it!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "Oh... Oh my... It's coming right at us!" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	fdMapGridSetShake(1, 2, 1);
	fdWorldMapReloadPendingAreaGraphics();
	syFieldWorldMapStartZoom(toFIXED(3) / 5, 1);
	view->yaw = 0;
	view->pitch = -DEGtoANG(45);
	syWait(1);
	fdCameraMoveTo(3655, CAMERA_KEEP, 7632, 0);
	fdWorldMapUpdateCurrentTileBlock();
	fdActorSetPos(ACTOR_IDEJIMA_KRADEN, 3690, 7616);
	fdActorSetPos(ACTOR_ALEX, 3646, 7679);
	fdActorSetPos(ACTOR_FELIX, 3669, 7664);
	fdActorSetPos(ACTOR_JENNA, 3657, 7621);
	fdActorSetPos(ACTOR_SHEBA, 3703, 7651);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 1);
	syWait(1);
	fdWorldMapFreeze();
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Alex... How can you stay so calm at a time like this?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "At times like this, where would be the good in panicking?" */
	fdScriptShowMessage(ACTOR_ALEX, 0);
	scFieldActorJump(ACTOR_SHEBA, 6, 15);
	scFieldActorJump(ACTOR_SHEBA, 6, 23);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
	/* "We'll be washed away!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAttachedEffect(ACTOR_IDEJIMA_KRADEN,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	/* "Help!!!" */
	fdScriptShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
	fdActorSetSpeed(ACTOR_IDEJIMA_KRADEN, 1);
	fdActorWalkTo(ACTOR_IDEJIMA_KRADEN, 3608, 7695);
	fdScriptPlaySnd(SE_WAVE_HIGH);
	scWorldMapLoadTidalWave(0);
	syFieldScriptSetExitRequest(WORLD_MAP_TO_IDEJIMA_NORTHEAST);
}

/* When the man gazing out from Alhafra's manor shows Felix Air's Rock, the
   World Map's view glides over to the rock, Felix hidden; pressing any button
   returns him to the manor. */
void scWorldMapShowAirsRockFromAlhafra(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId)->scaleX = 0;
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(20);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(11890, CAMERA_KEEP, 10090, 1);
	while(PerWork.data == 0)
		sySynch(1);
	fdScriptSetDestination(
		SCENE_ALHAFRA_MANOR, ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW);
}

/* The first voyage: the party takes the ship's tiller and learns to sail. */
void scWorldMapTakeTheTiller(void)
{
	Sint32 scale;
	Sint32 seen = fdScriptGetFlag(FLAG_SHIP_CAN_SAIL);

	if(seen == 0) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		scale = fdActorGetById(MainRoutineStateWork.leaderActorId)->scaleX;
		fdActorGetById(MainRoutineStateWork.leaderActorId)->scaleX = 0;
		fdActorSetSprFlags(
			fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdWorldMapFreeze();
		syFieldScriptSetMessageId((Sint32)MsgTakeTheTiller);
		fdScriptWait(40);
		/* "Take the tiller, Felix!" */
		syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
		fdActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "?!?" */
		syFieldScriptShowMessageThenWait(ACTOR_FELIX, 0, 10);
		/* "If you hope to reach Lemuria, you'll have to learn a few
		   things..." */
		syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
		fdActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "Kraden, shouldn't we be heading straight for Lemuria?" */
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		/* "Well, I suppose if we have to learn to sail, it's better to do
		   it..." */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		fdActorShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Then I want to go to Lalivero! And Tolbi! Oh! And Vale! Can we go
		   to..." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		/* "I'd love to see your hometown, Jenna! Shall we? Let's do it! To
		   Vale..." */
		fdMessageShowAtActor(ACTOR_PIERS, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
			fdActorShowEmote(
				ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
			/* "I don't think that's a good idea." */
			syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
			FieldStateWork->msg++;
		} else {
			fdActorShowEmote(
				ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
			FieldStateWork->msg++;
			/* "I'm with Felix. I don't think that would be a wise course of
			   action." */
			syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		}
		scFieldActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "Come, now! Why not?" */
		syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
		/* "Think about the roads we have traveled to get where we are..." */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		scFieldActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "I can't... I was never any good at geography." */
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		/* "Then allow me to explain... The coastline of Angara, the
		   continent..." */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		fdActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "Oh... You don't think Piers's boat could land there?" */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		/* "And as for Gondowan, I would guess the people from Lalivero..." */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		/* "He's right. Saturos and Menardi went a little wild in Lalivero,
		   and..." */
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		scFieldActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Oh... But Sheba can explain the situation for us. Faran and
		   the..." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		fdActorShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "And then they'd make me stay in Lalivero! Would you just leave
		   me..." */
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		/* "Oh yeah... Maybe that wouldn't be such a good idea after all..." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		/* "Then it's decided!" */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		fdActorShowEmote(
			ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "What's decided?" */
		syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
		fdActorShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "The sea is vast... We can go wherever we please! So, let's explore
		   a..." */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		/* "That's a great idea! We've already met werewolves and discovered
		   new..." */
		syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		/* "Wow! This is going to be great!" */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
		/* "Hmm... The seas can be quite dangerous. It won't be my fault
		   if..." */
		syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
		/* "To the open sea, Felix!" */
		syFieldScriptShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
		fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP;
		MainRoutineStateWork.ownerOverride = 1;
		fdWorldMapReloadPendingAreaGraphics();
		fdScriptSetFlag(FLAG_SHIP_CAN_SAIL);
		fdActorSetPos(ACTOR_PIERS, 0, 0);
		fdActorSetPos(ACTOR_SHEBA, 0, 0);
		fdActorSetPos(ACTOR_VOYAGE_KRADEN, 0, 0);
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->scaleX = scale;
		fdScriptEnd();
	}
}

/* The ship sails to its berth and the party goes down to the deck. */
void scWorldMapSailToShipDeck(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdActorMoveToAndWait(ACTOR_SHIP, 10720, 9075);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syFieldScriptSetExitRequest(WORLD_MAP_TO_LEMURIAN_SHIP_TILLER_A);
}

/* The ship's first flight once it has wings (FLAG_SHIP_TOOK_OFF). */
void scWorldMapTakeOff(void)
{
	Sint32 x, z, i;

	if(syGameFlagGet(FLAG_SHIP_FLIES) == 0)
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptSetFlag(FLAG_SHIP_TOOK_OFF);
	fdActorMoveToAndWait(ACTOR_SHIP, 7158, 8532);
	scWorldMapSetUpShip();
	x = fdActorGet(ACTOR_SHIP)->x.fixed / toFIXED(1);
	z = fdActorGet(ACTOR_SHIP)->z.fixed / toFIXED(1);
	for(i = 0; i <= 7; i++) {
		if(fdActorGet(i) != NULL) {
			WorldMap_HiddenMembers[i] = i;
			fdActorGet(i)->scaleX = 0;
			fdActorSetSprFlags(fdActorGet(i), 0);
			fdActorSetPos(i, x, z);
		} else {
			WorldMap_HiddenMembers[i] = -1;
		}
	}
	fdActorGet(ACTOR_KRADEN)->scaleX = 0;
	fdActorSetSprFlags(fdActorGet(ACTOR_KRADEN), 0);
	fdActorSetPos(ACTOR_KRADEN, x, z);
	fdWorldMapFreeze();
	syFieldScriptSetMessageId((Sint32)MsgWeDidIt);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	/* "We did it! The ship took off without a hitch!" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorShowEmote(ACTOR_SHIP, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Yeah, but...shouldn't it be flying a little higher than this?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "Hamma told us that we wouldn't be able to fly very high until we..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	/* "Let's see how far we can take this thing!" */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	syFieldScriptWait(30);
	/* "Sheba, what's wrong? You don't look happy..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "Do you remember on Idejima, when I told you I had my own reasons
	   for..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "You were so secretive! Are you going to tell us now?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syFieldScriptWait(50);
	/* "... ..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "You've gone quiet again... It's OK... You don't have to tell us
	   if..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	syFieldScriptWait(30);
	fdScriptPlaySnd(MUS_SHEBAS_PAST);
	/* "I thought that if I went to Jupiter Lighthouse, I would learn who
	   I..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	syFieldScriptWait(20);
	/* "I remember the people of Lalivero talking about how Sheba fell
	   from..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	syFieldScriptWait(20);
	/* "All my life, I've been looking for the answer... Where was I born..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "You knew Felix was going to Jupiter Lighthouse eventually, and so..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	syFieldScriptWait(20);
	/* "But you didn't find any answers, did you, Sheba?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	syFieldScriptWait(30);
	/* "I'm sorry, Sheba... I was so excited to find Contigo that I
	   didn't..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	syFieldScriptWait(20);
	/* "What? Why is everyone so down all of a sudden?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "... ..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "Faran raised Sheba like his own daughter. He's cared for her ever..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "Yes, that's true, but..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "But you can never be his real daughter, is that it?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "Garet! You don't have to be so blunt about it!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "No, he's right... I'd want to know my real parents, no matter how..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	/* "You think so? If it were me, I doubt it'd bother me at all..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	syFieldScriptWait(40);
	/* "Sheba, you may not know this, but you and I are very similar..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "What do you mean?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "Well, it's not exactly the same situation, but..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(20);
	/* "I was born in a poor village. My memories are hazy, but I still..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(20);
	/* "I was only four when Babi took me under his wing." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(20);
	/* "You see, even though I was young, I was quite intelligent. Babi
	   had..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "And Babi took you to Tolbi to further your education, to raise
	   you..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	syFieldScriptWait(20);
	/* "I was separated from my parents so early in life... I've never
	   known..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "Faran has always been like a father to me... I must have been
	   very..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "I should say so." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(60);
	/* "Thank you, Kraden... I feel better now..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "I'm glad I could help." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(40);
	/* "Well, we should be going... Let's hurry on, Felix!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	gmPlayMusic();
	fdWorldMapReloadPendingAreaGraphics();
	for(i = 0; i <= 7; i++) {
		if(WorldMap_HiddenMembers[i] >= 0) {
			fdActorSetSprFlags(fdActorGet(WorldMap_HiddenMembers[i]), 1);
			fdActorGet(WorldMap_HiddenMembers[i])->scaleX = toFIXED(1);
			fdActorSetPos(WorldMap_HiddenMembers[i], 0, 0);
		}
	}
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdScriptEnd();
}

/* A sparkle of the party's path to Aqua Rock: each frame, one time in four, it
   takes one of the eight colour modes at random. */
Sint32 scWorldMapChangeSparkleCol(ACTOR *actor)
{
	if(syRandom() < 16384)
		fdActorSetSprColMode(actor, syRandomBelow(8));
	return 0;
}

extern Sint32 WorldMap_AquaRockSparkleScript[];

/* The hidden leader's update as the party is carried to Aqua Rock: every eighth
   frame it leaves a sparkle where it is, which lasts about two seconds. */
void scWorldMapSpawnAquaRockSparkle(ACTOR *parent)
{
	if((FrameCount & 7) == 0) {
		FIXED  x = parent->x.fixed;
		FIXED  y = parent->y.fixed;
		FIXED  z = parent->z.fixed;
		ACTOR *actor = (scFieldActorCreate(SPRITE_SPARKLE, x, y, z));

		if(actor != NULL) {
			fdActorStartScript(actor, WorldMap_AquaRockSparkleScript);
			fdActorSelectAnimation(actor, 1);
			fdActorSetSprFlags(actor, 0);
			actor->motionFlg = 0;
			actor->scaleX = toFIXED(2);
			actor->scaleY = toFIXED(2);
		}
	}
}

/* The party, hidden in a trail of sparkles, is carried across the sea to Aqua
   Rock. */
void scWorldMapCarryPartyToAquaRock(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->update =
		scWorldMapSpawnAquaRockSparkle;
	fdActorGet(MainRoutineStateWork.leaderActorId)->acceleration =
		toFIXED(1) / 8;
	fdActorGet(MainRoutineStateWork.leaderActorId)->speedLimit = toFIXED(2);
	syWait(5);
	syFieldWorldMapStartZoom(toFIXED(3) / 4, 60);
	fdScriptStartScreenTransition();
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14537, 8577);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14465, 8662);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14452, 8766);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14387, 8828);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14331, 8888);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14284, 8904);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14255, 8870);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14255, 8831);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14255, 8772);
	syFieldWorldMapStartZoom(toFIXED(5) / 4, 120);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14275, 8736);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 14257, 8720);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptEnd();
	syFieldScriptSetExitRequest(WORLD_MAP_TO_AQUA_ROCK_MOAI_NORTH);
}

/* The same function as scWorldMapSetIslandTile, under another name. Turns a
   placed actor's sprite with the World Map's view and shows it. */
Sint32 scWorldMapSetPlacedActorTile(ACTOR *actor)
{
	Uint16	  *yaw = &((WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid)->yaw;
	FIELD_SPR *sprite = actor->spr;

	sprite->rotation = *yaw;
	sprite->flg = 0;
	return 1;
}

#include "shared/world_map_objects.c"

/* The ship's flicker and its sparks while it powers up (flight mode 1). */
void scWorldMapFlashPoweringUp(ACTOR *actor)
{
	if((FrameCount & 2) != 0) {
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((FrameCount & 15) == 0) {
		scWorldMapSpawnSparkPair(actor);
	}
}

/* The step through WorldMap_GlowColorModes, every other frame. */
extern Sint32 WorldMap_GlowColorStep;

/* The ship's glow and its trail in flight (flight mode 2): the colour cycles
   every other frame, and a particle every fourth. */
void scWorldMapGlowInFlight(ACTOR *actor)
{
	if(FrameCount & 1) {
		fdActorSetSprColMode(
			actor, WorldMap_GlowColorModes[WorldMap_GlowColorStep]);
		WorldMap_GlowColorStep = (WorldMap_GlowColorStep + 1) & 3;
	}
	if(!(FrameCount & 3)) {
		scWorldMapSpawnParticle(actor);
	}
}

/* Sets the ship's look by flight mode (SHIP_FLIGHT_MODE). */
void scActorSetFlightMode(ACTOR *actor, Sint32 mode)
{
	if(mode == SHIP_LANDED) {
		actor->update = 0;
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
		fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	} else if(mode == SHIP_POWERING_UP) {
		actor->update = (OBJECT_UPDATE)scWorldMapFlashPoweringUp;
	} else {
		actor->update = (OBJECT_UPDATE)scWorldMapGlowInFlight;
		fdScriptPlaySnd(SE_TAKE_FLIGHT);
	}
}

#include "shared/ship_take_flight.h"

/* Brings the leader's ship down out of flight. */
void scWorldMapEndFlight(void)
{
	scActorSetFlightMode(fdActorGetById(fdPartyGetLeaderActor()), SHIP_LANDED);
}

/* Run each frame of the lighthouse transition: every other two frames the blend
   alternates between two strengths. */
void scWorldMapUpdateTransitionBlend(void)
{

	scQueueIOWrite16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	if(FrameCount & 2)
		scQueueIOWrite16(
			(Uint32)&REG_BLDALPHA, WorldMapBlend | (BLEND_WEIGHT_MAX - 4));
	else
		scQueueIOWrite16(
			(Uint32)&REG_BLDALPHA, WorldMapBlend | BLEND_WEIGHT_MAX);
}

/* Shakes the camera diagonally by one pixel every three frames. */
#include "shared/camera_jitter.h"

/* The view of a lighthouse lit, seen from the World Map; Jupiter's beacon also
   rumbles and shakes the ground. */
void scWorldMapPlayLighthouseTransition(Sint32 mode)
{
	Uint8 *buffer;

	syWait(1);
	fdScriptClearFlag(FLAG_KEEP_PARTY_POSITION);
	FieldStateWork->transition = 0;
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	FieldStateWork->transitionFrm = 16;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	if(mode == LIGHTHOUSE_VIEW_JUPITER) {
		fdMapGridSetShake(2, 3, 1);
		fdScriptPlaySnd(SE_RUMBLE);
	}
	fdWorldMapFreeze();
	buffer = syHeapAllocEWRAM(16384);
	syDecompressTaggedLZ(WorldMapPackedTiles, buffer);
	syDecompressTaggedLZ(
		WorldMapPackedPictures, buffer + PICTURE_FIRST_TILE * TILE_SIZE_4BPP);
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
		(Uint32)(WorldMapPalettes + mode * PALETTE_COLORS * 2),
		DMA_COPY_HALFWORDS(PALETTE_COLORS));
	IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (Uint32)buffer,
		DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
	sySetFrameEvent(scWorldMapUpdateTransitionBlend, EVENT_PASS_FRAME);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_GLITTER);
	WorldMapBlend = BLEND_ALPHA(0, 14);
	PUSH_PICTURE(6);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 13);
	PUSH_PICTURE(5);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 12);
	PUSH_PICTURE(4);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 11);
	PUSH_PICTURE(3);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 10);
	PUSH_PICTURE(2);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 9);
	PUSH_PICTURE(1);
	syFieldScriptWait(2);
	WorldMapBlend = BLEND_ALPHA(0, 8);
	PUSH_PICTURE(0);
	syFieldScriptWait(140);
	PUSH_PICTURE(1);
	syFieldScriptWait(4);
	PUSH_PICTURE(2);
	syFieldScriptWait(4);
	PUSH_PICTURE(3);
	syFieldScriptWait(4);
	WorldMapBlend = BLEND_ALPHA(0, 9);
	PUSH_PICTURE(4);
	syFieldScriptWait(4);
	WorldMapBlend = BLEND_ALPHA(0, 10);
	PUSH_PICTURE(5);
	syFieldScriptWait(4);
	WorldMapBlend = BLEND_ALPHA(0, 11);
	PUSH_PICTURE(6);
	syFieldScriptWait(4);
	WorldMapBlend = BLEND_ALPHA(0, 12);
	PUSH_PICTURE(7);
	if(mode == LIGHTHOUSE_VIEW_VENUS) {
		fdMapGridSetShake(2, 2, 1);
		fdScriptPlaySnd(SE_RUMBLE);
		WorldMapBlend = BLEND_ALPHA(0, 13);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, 14);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, 15);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		syFieldScriptWait(45);
		syFieldCloseEvent(scWorldMapUpdateTransitionBlend);
		syPrintClear();
		scQueueIOWrite16((Uint32)&REG_BLDALPHA, WorldMapBlend);
		scQueueIOWrite16((Uint32)&REG_BLDCNT, 0);
	} else {
		WorldMapBlend = BLEND_ALPHA(0, 13);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, 14);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, 15);
		syFieldScriptWait(4);
		WorldMapBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		syFieldScriptWait(45);
		syFieldCloseEvent(scWorldMapUpdateTransitionBlend);
		syPrintClear();
		scQueueIOWrite16((Uint32)&REG_BLDALPHA, WorldMapBlend);
		scQueueIOWrite16((Uint32)&REG_BLDCNT, 0);
	}
	syHeapRelease(buffer);
	fdScriptSetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/* The tidal wave picture's tiles, loaded after the picture layer's own. */
#define WAVE_TILES 256

/* Loads the tidal wave's tiles, pictures and palette and sets its background
   up. */
void scWorldMapLoadTidalWave(Sint32 unused)
{
	Uint8 *buffer = syHeapAllocEWRAM(16384);
	Uint8 *frames;
	Uint32 x;
	Sint32 y, i, scrollX;
	Uint16 scrollY;
	Sint32 mode;

	syDecompressTaggedLZ(WorldMapPackedTilesB, buffer);
	frames = buffer + 8192;
	syDecompressTaggedLZ(WorldMapPackedPicturesB, frames);
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
		(Uint32)WorldMapPalettesB, DMA_COPY_HALFWORDS(PALETTE_COLORS));
	IO_WRITE_QUEUE_PUSH(VRAM, (Uint32)buffer,
		DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
	IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (Uint32)frames,
		DMA_COPY_WORDS(WAVE_TILES * TILE_SIZE_4BPP / 4));
	scQueueIOWrite16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	scQueueIOWrite16(
		(Uint32)&REG_BLDALPHA, BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX));
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 352;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG0CNT,
		BGCNT_SIZE(3) | BGCNT_SCREEN_BASE(2), IO_WRITE_HALFWORD);
	y = 11616;
	x = 561;
	for(i = 0; i < 96; i++) {
		scrollX = (Sint32)(x * 16) / 128;
		scrollY = -y / 128 + 352;
		BgScrollWork[0].x = scrollX;
		BgScrollWork[0].y = scrollY;
		syWait(1);
		y += 352;
		x += 17;
	}
	fdScriptPlaySnd(SE_SCATTER);
	syWait(120);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	/* FAKEMATCH(temporary): mode, a local standing for the fade's 0; as the
	 * literal. */
	mode = 0;
	fdPaletteFadeApplyTarget(RGB_WHITE, mode);
	fdPaletteFadeInterpolate(1);
	scQueueIOWrite16((Uint32)&REG_BLDALPHA, BLEND_ALPHA(0, BLEND_WEIGHT_MAX));
	syWait(1);
	fdPaletteFadeApplyTarget(0, 0);
	fdPaletteFadeInterpolate(60);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syWait(60);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	syWait(120);
	Ram_HeapSlots->actorSystem->suspended = 1;
	syWait(1);
	syWindowLoadGraphics();
	syPrintClear();
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
}

/* The actor flashes white, with a pair of sparks every sixteenth frame. */
void scWorldMapFlashWithSparks(ACTOR *actor)
{
	if((FrameCount & 2) != 0) {
		fdActorSetSprColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((FrameCount & 15) == 0) {
		scWorldMapSpawnTwinSparks(actor);
	}
}

/* Animates an actor, running scWorldMapSpawnTwinSparks every sixteenth
   frame. */
void scActorAnimateAndEmit(ACTOR *actor)
{
	Uint32 frame = FrameCount;

	if(frame & 1)
		fdActorSetSprColMode(actor, (frame >> 1) % 6);
	if((FrameCount & 15) == 0)
		scWorldMapSpawnTwinSparks(actor);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

/* Two golden rings rise from the actor and grow away. */
void scWorldMapSpawnTwinSparks(ACTOR *parent)
{
	ACTOR				*pair[2];
	ACTOR				*child;
	FIELD_SPR			*sprite;
	LINKED_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	Sint32				 i;

	for(i = 0; i < 2; ++i) {
		child = (scFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
			parent->y.fixed, parent->z.fixed));
		pair[i] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			sprite = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (union FIELD_OBJECT *)parent;
			if(sprite != NULL) {
				fdSprStartAnimation(sprite, 0);
				sprite->flg = 0;
				syVRAMCacheFreeEntry(sprite->slot);
				sprite->slot = state->vramBlock;
				sprite->retainCacheEntry = 1;
				sprite->tile =
					(VramCacheEntries[sprite->slot].ofs >> 5) & OBJ_TILE_MASK;
				sprite->fullCol = 0;
				sprite->shape = 1;
				((WORLD_MAP_OAM *)sprite)->size = 2;
				((SPR_ANIMATION *)sprite->record)->picture = 0;
			}
		}
	}
	{
		ACTOR	  *p = pair[0];
		FIELD_SPR *sp = p->spr;

		p->update = (void *)scActorGrowDown;
		sp->pri = 1;
	}
	{
		ACTOR	  *p = pair[1];
		FIELD_SPR *sp = p->spr;

		sp->pri = 1;
		p->update = (void *)scActorGrowUp;
		p->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}

#include "shared/world_map_gauge.c"

/* clang-format off */
Uint32 WorldMap_LohoWorkerLeaveScriptA[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(8961, 5837),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_END,
};

Uint32 WorldMap_LohoWorkerLeaveScriptB[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(8945, 5853),
	SC_SYNC,
	SC_MOVE_XZ(8961, 5837),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_END,
};

/* The same script as WorldMap_LohoWorkerLeaveScriptB, under another name. */
Uint32 WorldMap_LohoWorkerLeaveScriptC[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(8945, 5853),
	SC_SYNC,
	SC_MOVE_XZ(8961, 5837),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_END,
};

Uint32 WorldMap_IdejimaDriftScript[] = {
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_LOOP(10, SC_TOP),
	SC_SET(SW_TIMER, 1),
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_END,
};

Sint32 WorldMap_IdejimaWakeScript[] = {
	SC_ADD(SW_X, -toFIXED(0.1)),
	SC_ADD(SW_Z, toFIXED(0.1)),
	SC_ADD(SW_SCALE_X, toFIXED(7) / 128),
	SC_ADD(SW_SCALE_Y, -384),
	SC_WAIT(1),
	SC_LOOP(48, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 WorldMap_AquaRockSparkleScript[] = {
	SC_CALL(scWorldMapChangeSparkleCol),
	SC_WAIT(1),
	SC_LOOP(120, SC_TOP),
	SC_RELEASE,
};

Sint32 WorldMap_SparkScript[] = {
	SC_CALL((Sint32)scWorldMapRiseFromAnchor),
	SC_RELEASE,
};

#include "shared/world_map_particle_script.c"

/* The colour modes the ship's glow steps through, one every other frame. */
Uint32 WorldMap_GlowColorModes[] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_BLUE_TINT,
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_GREY,
};
/* clang-format on */

#undef WORLD_MAP_TRY_ENCOUNTER
#undef TUTORIAL_SET_MESSAGE
#undef TUTORIAL_MESSAGE

void scWorldMapMeetIron(void);
void scWorldMapBoardShip(void);
void scWorldMapLeaveShip(void);

LATE_DATA SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_IDEJIMA_SOUTHWEST, CONDITION_ALWAYS, 10240, 0, 7974,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_DAILA_SOUTH, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_DAILA_NORTH, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KANDOREAN_TEMPLE_GATE, CONDITION_ALWAYS, 10137, 0, 8602,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHRINE_OF_THE_SEA_GOD_ROPE, CONDITION_ALWAYS, 10423, 0,
		8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST, CONDITION_ALWAYS,
		10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_DEHKAN_PLATEAU_TIGHTROPES, CONDITION_ALWAYS, 10578, 0, 8952,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LEMURIAN_SHIP_MAIN_DECK_NORTH, CONDITION_ALWAYS, 10635, 0,
		9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MADRA, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MADRA_DRAWBRIDGE_SOUTH, CONDITION_ALWAYS, 10270, 0, 9522,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MADRA_DRAWBRIDGE_NORTH, CONDITION_ALWAYS, 10271, 0, 9425,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_OSENIA_CLIFFS_WEST, CONDITION_ALWAYS, 10380, 0, 9370,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_OSENIA_CLIFFS_EAST, CONDITION_ALWAYS, 10440, 0, 9399,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MIKASALLA, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GAROH_WAY_UP, CONDITION_ALWAYS, 12070, 0, 10470,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_AIRS_ROCK_STORM_BRAND_CHEST, CONDITION_ALWAYS, 11880, 0,
		10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_YAMPI_DESERT_GUARDIAN_RING_CHEST, CONDITION_ALWAYS, 10976,
		0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS, 11736,
		0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE, CONDITION_ALWAYS, 11910,
		0, 9729, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ALHAFRA, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CLIFFS_A, CONDITION_ALWAYS, 9244, 0, 9528,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CLIFFS_B, CONDITION_ALWAYS, 9328, 0, 9528,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NARIBWE_SOUTH, CONDITION_ALWAYS, 9148, 0, 9048,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NARIBWE_NORTH, CONDITION_ALWAYS, 9148, 0, 8988,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST, CONDITION_ALWAYS, 9199, 0,
		8410, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KIBOMBO_MOUNTAINS_WAFT, CONDITION_ALWAYS, 9060, 0, 8290,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KIBOMBO, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_WEST_INDRA_ISLET, CONDITION_ALWAYS, 9546, 0, 8316,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_EAST_TUNDARIA_ISLET, CONDITION_ALWAYS, 12770, 0, 11570,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NORTH_OSENIA_ISLET_A, CONDITION_ALWAYS, 11724, 0, 8064,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SOUTHEAST_ANGARA_ISLET, CONDITION_ALWAYS, 11680, 0, 6678,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SEA_OF_TIME_ISLET, CONDITION_ALWAYS, 13486, 0, 7482,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_YALLAM_NORTH, CONDITION_ALWAYS, 12923, 0, 9681,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_YALLAM_SOUTH, CONDITION_ALWAYS, 12921, 0, 9752,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TAOPO_SWAMP_ENTRANCE, CONDITION_ALWAYS, 12680, 0, 10297,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_APOJII_ISLANDS, CONDITION_ALWAYS, 14478, 0, 8410,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_AQUA_ROCK_MOAI_SOUTH, CONDITION_ALWAYS, 14250, 0, 8728,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_IZUMO, CONDITION_ALWAYS, 13376, 0, 6068, FACING_WEST, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_GAIA_ROCK_SUSA, CONDITION_ALWAYS, 13452, 0, 6204,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CHAMPA, CONDITION_ALWAYS, 10903, 0, 6285, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ANKOHL_RUINS_ENTRANCE, CONDITION_ALWAYS, 11201, 0, 6105,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SEA_OF_TIME_SWIRLS, CONDITION_ALWAYS, 12461, 0, 7566,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SEA_OF_TIME_TO_LEMURIA_NORTHEAST, CONDITION_ALWAYS, 12090,
		0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SEA_OF_TIME_TO_LEMURIA_NORTH, CONDITION_ALWAYS, 13010, 0,
		1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LEMURIA_ENTRANCE, CONDITION_ALWAYS, 13088, 0, 1090,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TUNDARIA_TOWER_ENTRANCE, CONDITION_ALWAYS, 9490, 0, 10960,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SOUTHWEST_ATTEKA_ISLET, CONDITION_ALWAYS, 5836, 0, 10120,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_SETTLEMENT, CONDITION_ALWAYS, 8582, 0, 7420,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_HESPERIA_SETTLEMENT, CONDITION_ALWAYS, 6314, 0, 6223,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALT_ISLAND, CONDITION_ALWAYS, 8847, 0, 4629, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MAGMA_ROCK_SLIDE, CONDITION_ALWAYS, 8970, 0, 7500,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ATTEKA_INLET_EAST, CONDITION_ALWAYS, 7256, 0, 8325,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ATTEKA_INLET_GEODE_NORTH, CONDITION_ALWAYS, 7280, 0, 8180,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CONTIGO, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_JUPITER_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, 7277, 0,
		7330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_WEST, CONDITION_ALWAYS, 7383, 0, 5753,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHAMAN_VILLAGE_CAVE_EAST, CONDITION_ALWAYS, 7504, 0, 5650,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHAMAN_VILLAGE, CONDITION_ALWAYS, 7663, 0, 5535,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LOHO, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_NORTHERN_REACHES_SOUTH, CONDITION_ALWAYS, 8933, 0, 3704,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NORTHERN_REACHES_NORTH, CONDITION_ALWAYS, 8916, 0, 1640,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_PROX_SOUTH_GATE, CONDITION_ALWAYS, 9056, 0, 1250,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_PROX_MOLD, CONDITION_ALWAYS, 9056, 0, 1175, FACING_NORTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_VENUS_COLLAPSE, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_DRIFT_NORTH, CONDITION_ALWAYS, 10239, 0, 7884, FACING_EAST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_ALHAFRA_MANOR_VIEW, CONDITION_ALWAYS, 11633, 0, 9406,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FIRST_VOYAGE, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_SAIL_TO_DECK, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LEMURIAN_SHIP_TILLER_B, CONDITION_ALWAYS, 10720, 0, 9075,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_JUPITER_BEACON, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_BANDIT_BEATEN, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_BANDIT_LOST, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_IDEJIMA_DRIFTING, CONDITION_ALWAYS, 12096, 0, 8325, FACING_EAST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_CARRIED_TO_AQUA_ROCK, CONDITION_ALWAYS, 14558, 0, 8457,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NORTH_OSENIA_ISLET_B, CONDITION_ALWAYS, 11754, 0, 8030,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_NORTH_OSENIA_ISLET_C, CONDITION_ALWAYS, 11700, 0, 8034,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CAVERNS_OSENIA, CONDITION_ALWAYS, 11596, 0, 10204,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CAVERNS_INDRA, CONDITION_ALWAYS, 10240, 0, 9142,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MARS_LIGHTHOUSE_OUTSIDE, CONDITION_ALWAYS, 9020, 0, 882,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_NORTH, CONDITION_ALWAYS, 9288, 0, 9434,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CLIFFS_PASS_SOUTH, CONDITION_ALWAYS, 9288, 0, 9640,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TREASURE_ISLE_ENTRANCE, CONDITION_ALWAYS, 12640, 0, 4390,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CAVERNS_ATTEKA, CONDITION_ALWAYS, 7868, 0, 9120,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CAVERNS_ANGARA, CONDITION_ALWAYS, 9479, 0, 5060,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_DEBUG_FLIGHT, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_DEBUG_SAILING, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(WorldMap),
	SCENE_EXIT(
		WORLD_MAP_TO_IDEJIMA_DRIFTING, WorldMap, WORLD_MAP_IDEJIMA_DRIFTING),
	SCENE_EXIT(
		WORLD_MAP_TO_IDEJIMA_NORTHEAST, Idejima, IDEJIMA_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_LEMURIAN_SHIP_TILLER_A, LemurianShipTiller,
		LEMURIAN_SHIP_TILLER_FROM_WORLD_MAP_C),
	SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_NORTH_B, Idejima, IDEJIMA_AFTER_DRIFT),
	SCENE_EXIT(WORLD_MAP_TO_AQUA_ROCK_MOAI_NORTH, AquaRockMoai,
		AQUA_ROCK_MOAI_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_SOUTHWEST, Idejima,
		IDEJIMA_FROM_WORLD_MAP_SOUTHWEST),
	SCENE_EXIT_UNLESS(WORLD_MAP_TO_DAILA_SOUTH, DailaFlooded,
		DAILA_FLOODED_FROM_WORLD_MAP_SOUTH, FLAG_TAVI_RESCUED),
	SCENE_EXIT_UNLESS(WORLD_MAP_TO_DAILA_NORTH, DailaFlooded,
		DAILA_FLOODED_FROM_WORLD_MAP_NORTH, FLAG_TAVI_RESCUED),
	SCENE_EXIT(WORLD_MAP_TO_DAILA_SOUTH, Daila, DAILA_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_DAILA_NORTH, Daila, DAILA_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_KANDOREAN_TEMPLE_GATE, KandoreanTempleGate,
		KANDOREAN_TEMPLE_GATE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope,
		SHRINE_OF_THE_SEA_GOD_ROPE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST,
		DehkanPlateauFullMetalVestChest,
		DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES, DehkanPlateauTightropes,
		DEHKAN_PLATEAU_TIGHTROPES_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_LEMURIAN_SHIP_MAIN_DECK_NORTH, LemurianShipMainDeck,
		LEMURIAN_SHIP_MAIN_DECK_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_MADRA, Madra, MADRA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH, MadraDrawbridge,
		MADRA_DRAWBRIDGE_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH, MadraDrawbridge,
		MADRA_DRAWBRIDGE_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_OSENIA_CLIFFS_WEST, OseniaCliffs,
		OSENIA_CLIFFS_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_OSENIA_CLIFFS_EAST, OseniaCliffs,
		OSENIA_CLIFFS_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_MIKASALLA, Mikasalla, MIKASALLA_FROM_WORLD_MAP),
	SCENE_EXIT(
		WORLD_MAP_TO_GAROH_WAY_UP, GarohWayUp, GAROH_WAY_UP_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_AIRS_ROCK_STORM_BRAND_CHEST,
		AirsRockStormBrandChest, AIRS_ROCK_STORM_BRAND_CHEST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST,
		YampiDesertGuardianRingChest,
		YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST,
		YampiDesertTrainersWhipChest,
		YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE,
		YampiDesertHiddenPsynergyStone,
		YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_A, GondowanCliffs,
		GONDOWAN_CLIFFS_FROM_WORLD_MAP_A),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_B, GondowanCliffs,
		GONDOWAN_CLIFFS_FROM_WORLD_MAP_B),
	SCENE_EXIT(
		WORLD_MAP_TO_NARIBWE_SOUTH, Naribwe, NARIBWE_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST,
		KibomboMountainsDiskAxeChest,
		KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KIBOMBO_MOUNTAINS_WAFT, KibomboMountainsWaft,
		KIBOMBO_MOUNTAINS_WAFT_FROM_WORLD_MAP),
	SCENE_EXIT_UNLESS(WORLD_MAP_TO_KIBOMBO, KibomboCeremony,
		KIBOMBO_CEREMONY_FROM_WORLD_MAP, FLAG_GABOMBA_MESSAGE_CARRIED),
	SCENE_EXIT_UNLESS(
		WORLD_MAP_TO_KIBOMBO, Kibombo, KIBOMBO_FROM_WORLD_MAP, EXIT_FLAG_NONE),
	SCENE_EXIT(WORLD_MAP_TO_WEST_INDRA_ISLET, WestIndraIslet,
		WEST_INDRA_ISLET_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_EAST_TUNDARIA_ISLET, EastTundariaIslet,
		EAST_TUNDARIA_ISLET_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_NORTH_OSENIA_ISLET_A, NorthOseniaIslet,
		NORTH_OSENIA_ISLET_FROM_WORLD_MAP_A),
	SCENE_EXIT(WORLD_MAP_TO_SOUTHEAST_ANGARA_ISLET, SoutheastAngaraIslet,
		SOUTHEAST_ANGARA_ISLET_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_ISLET, SeaOfTimeIslet,
		SEA_OF_TIME_ISLET_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_YALLAM_SOUTH, Yallam, YALLAM_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_YALLAM_NORTH, Yallam, YALLAM_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_TAOPO_SWAMP_ENTRANCE, TaopoSwampEntrance,
		TAOPO_SWAMP_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_APOJII_ISLANDS, ApojiiIslands,
		APOJII_ISLANDS_FROM_WORLD_MAP_A),
	SCENE_EXIT(WORLD_MAP_TO_AQUA_ROCK_MOAI_SOUTH, AquaRockMoai,
		AQUA_ROCK_MOAI_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_IZUMO, Izumo, IZUMO_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GAIA_ROCK_SUSA, GaiaRockSusa,
		GAIA_ROCK_SUSA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_CHAMPA, Champa, CHAMPA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_ANKOHL_RUINS_ENTRANCE, AnkohlRuinsEntrance,
		ANKOHL_RUINS_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_SWIRLS, SeaOfTimeSwirls,
		SEA_OF_TIME_SWIRLS_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST,
		SeaOfTimeToLemuria, SEA_OF_TIME_TO_LEMURIA_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH, SeaOfTimeToLemuria,
		SEA_OF_TIME_TO_LEMURIA_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_LEMURIA_ENTRANCE, LemuriaEntrance,
		LEMURIA_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_TUNDARIA_TOWER_ENTRANCE, TundariaTowerEntrance,
		TUNDARIA_TOWER_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SOUTHWEST_ATTEKA_ISLET, SouthwestAttekaIslet,
		SOUTHWEST_ATTEKA_ISLET_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_SETTLEMENT, GondowanSettlement,
		GONDOWAN_SETTLEMENT_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_HESPERIA_SETTLEMENT, HesperiaSettlement,
		HESPERIA_SETTLEMENT_FROM_WORLD_MAP),
	SCENE_EXIT(
		WORLD_MAP_TO_KALT_ISLAND, KaltIsland, KALT_ISLAND_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_MAGMA_ROCK_SLIDE, MagmaRockSlide,
		MAGMA_ROCK_SLIDE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_ATTEKA_INLET_EAST, AttekaInlet,
		ATTEKA_INLET_FROM_LEMURIAN_SHIP_CABINS),
	SCENE_EXIT(WORLD_MAP_TO_ATTEKA_INLET_GEODE_NORTH, AttekaInletGeode,
		ATTEKA_INLET_GEODE_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_CONTIGO, Contigo, CONTIGO_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_JUPITER_LIGHTHOUSE_ENTRANCE,
		JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_WEST, ShamanVillageCave,
		SHAMAN_VILLAGE_CAVE_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_EAST, ShamanVillageCave,
		SHAMAN_VILLAGE_CAVE_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE, ShamanVillage,
		SHAMAN_VILLAGE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_LOHO, Loho, LOHO_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_NORTHERN_REACHES_SOUTH, NorthernReaches,
		NORTHERN_REACHES_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_NORTHERN_REACHES_NORTH, NorthernReaches,
		NORTHERN_REACHES_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_PROX_SOUTH_GATE, ProxSouthGate,
		PROX_SOUTH_GATE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_PROX_MOLD, ProxMold, PROX_MOLD_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_CAVERNS_OSENIA, CavernsOsenia,
		CAVERNS_OSENIA_FROM_WORLD_MAP),
	SCENE_EXIT(
		WORLD_MAP_TO_CAVERNS_INDRA, CavernsIndra, CAVERNS_INDRA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE, MarsLighthouseOutside,
		MARS_LIGHTHOUSE_OUTSIDE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH, GondowanCliffsPass,
		GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH, GondowanCliffsPass,
		GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_TREASURE_ISLE_ENTRANCE, TreasureIsleEntrance,
		TREASURE_ISLE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_CAVERNS_ATTEKA, CavernsAtteka,
		CAVERNS_ATTEKA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_CAVERNS_ANGARA, CavernsAngara,
		CAVERNS_ANGARA_FROM_WORLD_MAP),
	SCENE_EXITS_END,
};

LATE_DATA Sint32 WorldMap_TileSpriteScript[] = {
	SC_CALL(scTileSprReset),
	SC_END,
};

LATE_DATA Sint32 WorldMap_EchoScript[] = {
	SC_CALL(scWorldMapSetUpEcho),
	SC_END,
};

LATE_DATA ACTOR_PLACE WorldMapActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
		(Sint32)WorldMap_TileSpriteScript, 10640, 0, 9075, FACING_EAST,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_ECHO),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_IRON),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SOUR),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_CHILL),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WHEEZE),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_PETRA),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CORE),
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR,
		IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND, 0, 0, 0,
		FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE,
		IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND, 0, 0, 0,
		FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE,
		IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND, 0, 0, 0,
		FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE WorldMap_CollapsePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1968,
		0, 7488, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1936,
		0, 7472, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1904, 0,
		7488, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
		(Sint32)WorldMap_TileSpriteScript, 1946, 0, 7557, FACING_EAST,
		TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA Sint32 WorldMap_IslandScript[] = {
	SC_CALL(scWorldMapSetIslandTile),
	SC_END,
};

LATE_DATA Sint32 WorldMap_TidalWaveScript[] = {
	SC_CALL(scWorldMapSetUpWave),
	SC_CALL(scWorldMapSendWaveIn),
	SC_END,
};

LATE_DATA ACTOR_PLACE WorldMap_DriftPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
		0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000,
		0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000,
		0, 8000, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
		0, 8000, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
		(Sint32)WorldMap_IslandScript, 10207, 0, 7900, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10524, 0, 7450, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10532, 0, 7480, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10540, 0, 7510, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10548, 0, 7540, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10556, 0, 7570, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10564, 0, 7600, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10572, 0, 7630, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
		(Sint32)WorldMap_TidalWaveScript, 10580, 0, 7660, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
		(Sint32)WorldMap_TileSpriteScript, 3515, 0, 7621, FACING_EAST,
		TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE WorldMap_FirstVoyagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WINGED_SHIP_SMALL, CONDITION_ALWAYS,
		(Sint32)WorldMap_TileSpriteScript, 10640, 0, 9075, FACING_EAST,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550,
		0, 8682, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550,
		0, 9282, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850,
		0, 8682, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850,
		0, 9282, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA ACTOR_PLACE WorldMap_DriftingIdejimaPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
		(Sint32)WorldMap_IslandScript, 12096, 0, 8325, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA CUE SceneCues[] = {
	{CUE_RAISED, WORLD_MAP_RAISED_BOARD_SHIP, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapBoardShip)},
	{CUE_RAISED, WORLD_MAP_RAISED_LEAVE_SHIP, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapLeaveShip)},
	{CUE_RAISED, WORLD_MAP_RAISED_TAKE_OFF, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapSetUpFlyingShip)},
	{CUE_RAISED, WORLD_MAP_RAISED_LAND, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapSetUpShip)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_IRON, FLAG_DJINNI(DJINNI_IRON),
		SCRIPT_REF(scWorldMapMeetIron)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_SOUR, FLAG_DJINNI(DJINNI_SOUR),
		SCRIPT_REF(scWorldMapMeetSour)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_CHILL, FLAG_DJINNI(DJINNI_CHILL),
		SCRIPT_REF(scWorldMapMeetChill)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_WHEEZE, FLAG_DJINNI(DJINNI_WHEEZE),
		SCRIPT_REF(scWorldMapMeetWheeze)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_PETRA, FLAG_DJINNI(DJINNI_PETRA),
		SCRIPT_REF(scWorldMapMeetPetra)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_CORE, FLAG_DJINNI(DJINNI_CORE),
		SCRIPT_REF(scWorldMapMeetCore)},
	{CUE_EXAMINE, CUE_SUBJECT_OWN_EFFECT | SHIP_SEA_EXAMINE_CODE,
		CONDITION_ALWAYS, SCRIPT_REF(scWorldMapGazeIntoPond)},
	{CUE_STEP, STEP_CUE_AREA(PLACED_DAILA), CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapEnterDaila)},
	/* Walking into a place's marker (its slot among WorldMap_PlacedActors)
	   leaves by the exit to that place. */
	{CUE_EXIT, STEP_CUE_AREA(1), CONDITION_ALWAYS,
		WORLD_MAP_TO_KANDOREAN_TEMPLE_GATE},
	{CUE_EXIT, STEP_CUE_AREA(2), CONDITION_ALWAYS,
		WORLD_MAP_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
	{CUE_EXIT, STEP_CUE_AREA(3), CONDITION_ALWAYS,
		WORLD_MAP_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST},
	{CUE_EXIT, STEP_CUE_AREA(4), CONDITION_ALWAYS,
		WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES},
	{CUE_EXIT, STEP_CUE_AREA(5), FLAG_SHIP_CAN_SAIL,
		WORLD_MAP_TO_LEMURIAN_SHIP_MAIN_DECK_NORTH},
	{CUE_EXIT, STEP_CUE_AREA(6), CONDITION_ALWAYS, WORLD_MAP_TO_MADRA},
	{CUE_EXIT, STEP_CUE_AREA(7), CONDITION_ALWAYS,
		WORLD_MAP_TO_OSENIA_CLIFFS_WEST},
	{CUE_EXIT, STEP_CUE_AREA(8), CONDITION_ALWAYS,
		WORLD_MAP_TO_OSENIA_CLIFFS_EAST},
	{CUE_EXIT, STEP_CUE_AREA(9), CONDITION_ALWAYS, WORLD_MAP_TO_MIKASALLA},
	{CUE_EXIT, STEP_CUE_AREA(10), CONDITION_ALWAYS, WORLD_MAP_TO_GAROH_WAY_UP},
	{CUE_EXIT, STEP_CUE_AREA(11), CONDITION_ALWAYS,
		WORLD_MAP_TO_AIRS_ROCK_STORM_BRAND_CHEST},
	{CUE_EXIT, STEP_CUE_AREA(12), CONDITION_ALWAYS,
		WORLD_MAP_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE},
	{CUE_EXIT, STEP_CUE_AREA(13), CONDITION_ALWAYS, WORLD_MAP_TO_ALHAFRA},
	{CUE_EXIT, STEP_CUE_AREA(14), CONDITION_ALWAYS,
		WORLD_MAP_TO_GONDOWAN_CLIFFS_A},
	{CUE_EXIT, STEP_CUE_AREA(15), CONDITION_ALWAYS,
		WORLD_MAP_TO_GONDOWAN_CLIFFS_B},
	{CUE_EXIT, STEP_CUE_AREA(16), CONDITION_ALWAYS, WORLD_MAP_TO_NARIBWE_SOUTH},
	{CUE_EXIT, STEP_CUE_AREA(17), CONDITION_ALWAYS,
		WORLD_MAP_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST},
	{CUE_EXIT, STEP_CUE_AREA(18), CONDITION_ALWAYS,
		WORLD_MAP_TO_KIBOMBO_MOUNTAINS_WAFT},
	{CUE_EXIT, STEP_CUE_AREA(19), CONDITION_ALWAYS, WORLD_MAP_TO_KIBOMBO},
	{CUE_EXIT, STEP_CUE_AREA(20), CONDITION_ALWAYS,
		WORLD_MAP_TO_WEST_INDRA_ISLET},
	{CUE_EXIT, STEP_CUE_AREA(21), CONDITION_ALWAYS,
		WORLD_MAP_TO_EAST_TUNDARIA_ISLET},
	{CUE_EXIT, STEP_CUE_AREA(22), CONDITION_ALWAYS,
		WORLD_MAP_TO_NORTH_OSENIA_ISLET_A},
	{CUE_EXIT, STEP_CUE_AREA(23), CONDITION_ALWAYS,
		WORLD_MAP_TO_SOUTHEAST_ANGARA_ISLET},
	{CUE_EXIT, STEP_CUE_AREA(24), CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_ISLET},
	{CUE_STEP, STEP_CUE_AREA(PLACED_YALLAM), CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapEnterYallam)},
	{CUE_EXIT, STEP_CUE_AREA(26), CONDITION_ALWAYS,
		WORLD_MAP_TO_TAOPO_SWAMP_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(27), CONDITION_ALWAYS,
		WORLD_MAP_TO_APOJII_ISLANDS},
	{CUE_EXIT, STEP_CUE_AREA(28), CONDITION_ALWAYS,
		WORLD_MAP_TO_AQUA_ROCK_MOAI_SOUTH},
	{CUE_EXIT, STEP_CUE_AREA(29), CONDITION_ALWAYS, WORLD_MAP_TO_IZUMO},
	{CUE_EXIT, STEP_CUE_AREA(30), CONDITION_ALWAYS,
		WORLD_MAP_TO_GAIA_ROCK_SUSA},
	{CUE_EXIT, STEP_CUE_AREA(31), CONDITION_ALWAYS, WORLD_MAP_TO_CHAMPA},
	{CUE_EXIT, STEP_CUE_AREA(32), CONDITION_ALWAYS,
		WORLD_MAP_TO_ANKOHL_RUINS_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(33), CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_SWIRLS},
	{CUE_EXIT, STEP_CUE_AREA(34), CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST},
	{CUE_EXIT, STEP_CUE_AREA(36), CONDITION_ALWAYS,
		WORLD_MAP_TO_LEMURIA_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(37), CONDITION_ALWAYS,
		WORLD_MAP_TO_TUNDARIA_TOWER_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(38), CONDITION_ALWAYS,
		WORLD_MAP_TO_SOUTHWEST_ATTEKA_ISLET},
	{CUE_EXIT, STEP_CUE_AREA(39), CONDITION_ALWAYS,
		WORLD_MAP_TO_GONDOWAN_SETTLEMENT},
	{CUE_EXIT, STEP_CUE_AREA(40), CONDITION_ALWAYS,
		WORLD_MAP_TO_HESPERIA_SETTLEMENT},
	{CUE_EXIT, STEP_CUE_AREA(41), CONDITION_ALWAYS, WORLD_MAP_TO_KALT_ISLAND},
	{CUE_EXIT, STEP_CUE_AREA(42), CONDITION_ALWAYS,
		WORLD_MAP_TO_MAGMA_ROCK_SLIDE},
	{CUE_EXIT, STEP_CUE_AREA(43), CONDITION_ALWAYS, WORLD_MAP_TO_CONTIGO},
	{CUE_EXIT, STEP_CUE_AREA(44), CONDITION_ALWAYS,
		WORLD_MAP_TO_JUPITER_LIGHTHOUSE_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(45), CONDITION_ALWAYS,
		WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_WEST},
	{CUE_EXIT, STEP_CUE_AREA(46), CONDITION_ALWAYS,
		WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_EAST},
	{CUE_EXIT, STEP_CUE_AREA(47), CONDITION_ALWAYS,
		WORLD_MAP_TO_SHAMAN_VILLAGE},
	{CUE_EXIT, STEP_CUE_AREA(48), CONDITION_ALWAYS, WORLD_MAP_TO_LOHO},
	{CUE_STEP, STEP_CUE_AREA(PLACED_PROX), CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapEnterProx)},
	{CUE_EXIT, STEP_CUE_AREA(51), CONDITION_ALWAYS,
		WORLD_MAP_TO_CAVERNS_OSENIA},
	{CUE_EXIT, STEP_CUE_AREA(52), CONDITION_ALWAYS, WORLD_MAP_TO_CAVERNS_INDRA},
	{CUE_EXIT, STEP_CUE_AREA(53), CONDITION_ALWAYS,
		WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE},
	{CUE_EXIT, STEP_CUE_AREA(54), CONDITION_ALWAYS,
		WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE},
	{CUE_EXIT, STEP_CUE_AREA(55), CONDITION_ALWAYS,
		WORLD_MAP_TO_TREASURE_ISLE_ENTRANCE},
	{CUE_EXIT, STEP_CUE_AREA(56), CONDITION_ALWAYS,
		WORLD_MAP_TO_CAVERNS_ATTEKA},
	{CUE_EXIT, STEP_CUE_AREA(57), CONDITION_ALWAYS,
		WORLD_MAP_TO_CAVERNS_ANGARA},
	/* XXX: no block of the World Map (games/tla/src/map/world/blocks.tsv) has
	   place 1, so no metatile seems to answer this row; logging the step cue
	   codes as the leader walks the shores round Daila would settle it. */
	{CUE_EXIT, 1, CONDITION_ALWAYS, WORLD_MAP_TO_DAILA_NORTH},
	{CUE_EXIT, WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES, CONDITION_ALWAYS,
		WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES},
	{CUE_EXIT, WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH},
	{CUE_EXIT, WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH},
	{CUE_EXIT, WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST, CONDITION_ALWAYS,
		WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST},
	{CUE_EXIT, WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS,
		WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
	{CUE_EXIT, WORLD_MAP_TO_SEA_OF_TIME_SWIRLS, CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_SWIRLS},
	{CUE_EXIT, WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST, CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST},
	{CUE_EXIT, WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH},
	{CUE_EXIT, WORLD_MAP_TO_LEMURIA_ENTRANCE, CONDITION_ALWAYS,
		WORLD_MAP_TO_LEMURIA_ENTRANCE},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_ATTEKA_INLET_LANDING,
		CONDITION_ALWAYS, SCRIPT_REF(scWorldMapMoorAtAttekaInlet)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_AWAY_FROM_CONTIGO, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapRefuseToRunAway)},
	{CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_FIRST_FLIGHT, FLAG_SHIP_TOOK_OFF,
		SCRIPT_REF(scWorldMapTakeOff)},
	{CUE_EXIT, WORLD_MAP_TO_NORTHERN_REACHES_SOUTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_NORTHERN_REACHES_SOUTH},
	{CUE_EXIT, WORLD_MAP_TO_NORTHERN_REACHES_NORTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_NORTHERN_REACHES_NORTH},
	{CUE_EXIT, WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH},
	{CUE_EXIT, WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH, CONDITION_ALWAYS,
		WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH},
	{CUE_TALK, ACTOR_GOLDEN_BOOTS, CONDITION_ALWAYS,
		SCRIPT_REF(scWorldMapPickUpGoldenBoots)},
	{CUE_TABLE_END},
};

/* A cloud of dust as the earthquake shakes the land, gone after 16 frames. */
LATE_DATA Uint32 WorldMap_CollapseDustScript[] = {
	SC_WAIT(16),
	SC_RELEASE,
};

LATE_DATA Sint32 WorldMap_PlacedActorScript[] = {
	SC_CALL(scWorldMapSetPlacedActorTile),
	SC_END,
	0,
};

LATE_DATA Sint32 WorldMap_GlowColorStep = 0;
