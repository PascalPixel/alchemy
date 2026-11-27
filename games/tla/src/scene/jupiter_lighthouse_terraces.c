/* The terraces high outside Jupiter Lighthouse, over the clouds, which link its
   main tower, by the red and blue doors, with its left and right towers. On the
   walkway west to the left tower, a whirlwind blows Felix back east if he walks
   into its path (scJupiterLighthouseRideUpperWhirlwind). Outside the upper
   doorway, the grey block pushed one metatile east onto a trapdoor and the
   purple button in front of the doorway make a shortcut: standing on the button
   opens the trapdoor and the block drops through
   (scJupiterLighthouseOpenFloor). Pushed into its gap, the energy block draws a
   bolt of lightning that lights the path to a Hover pad
   (scJupiterLighthouseFinishMovingBlock); Hovering from it, Felix is caught by
   a whirlwind that takes him on towards the right tower
   (scJupiterLighthouseRideWhirlwind). Once both halves of the well's lid are
   lifted, on the way back to the aerie the party finds Isaac's party: Garet
   hangs from the ledge, Mia unable to pull him up, while Isaac and Ivan fight
   Karst and Agatio (scJupiterLighthouseStartTheFight); the party arrives to
   watch (scJupiterLighthousePartyArrivesAtTheFight), and Isaac and Ivan are
   hurt (scJupiterLighthouseIsaacAndIvanAreHurt) and fall
   (scJupiterLighthouseIsaacAndIvanFall). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "system/field_effect.h"
#include "field/script_inline.h"
#include "item_ids.h"
#include "field/call.h"
#include "scene/scene.h"
#include "edition.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/actor_script.h"
#include "macros.h"
#include "shop/shop.h"

void scLeaderCenterInMetatile(void);

/* The energy block pushed into its gap: lightning has struck it, and the path's
   grooves light up to the Hover pad beyond. */
#define FLAG_ENERGY_BLOCK_IN_GAP		  0xa0c
/* The grey block pushed over the trapdoor has dropped through it, a shortcut to
   the rooms below. */
#define FLAG_GRAY_BLOCK_DROPPED			  0xa0d
/* For this visit: Lash has tied the rope between the two tightrope posts. */
#define FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED FLAG_SCENE_TEMP_00
/* Set while Felix stands on the button by the upper doorway, which holds the
   trapdoor open. */
#define FLAG_TRAPDOOR_OPEN				  FLAG_SCENE_TEMP_02

struct METATILE_TARGET;
struct METATILE_TARGET_ENTRY;

void scPaletteCycleSetPausesInScripts(Sint32 value);

extern Uint32 JupiterLighthouseGaretHangsScript[];
extern Uint32 JupiterLighthouseMiaScript[];
extern Uint32 JupiterLighthouseFelixLooksAroundScript[];
extern Uint32 JupiterLighthouseKradenArrivesWestScript[];
extern Uint32 JupiterLighthouseKradenArrivesEastScript[];
extern Uint32 JupiterLighthousePiersArrivesWestScript[];
extern Uint32 JupiterLighthouseJennaArrivesEastScript[];
extern Uint32 JupiterLighthouseShebaArrivesWestScript[];

extern Uint32 JupiterLighthouseAgatioStepsUpScript[];
extern Uint32 JupiterLighthouseKarstTurnsScript[];
extern Uint32 JupiterLighthouseKarstFacesSouthScript[];
extern Uint32 JupiterLighthouseAgatioFacesEastScript[];
extern Uint32 JupiterLighthouseIsaacFacesWestScript[];
extern Uint32 JupiterLighthouseIvanFacesNorthScript[];

extern Uint32 JupiterLighthouseGatherAtFelixScript[];

extern Uint32 JupiterLighthouseFelixWaitsScript[];
extern Uint32 JupiterLighthouseKradenWaitsScript[];
extern Uint32 JupiterLighthousePiersStepsUpScript[];
extern Uint32 JupiterLighthouseShebaStepsUpScript[];
extern Uint32 JupiterLighthouseJennaStepsUpScript[];

extern Uint32 JupiterLighthouseWalkToAerieScript[];
extern Uint32 JupiterLighthouseLeaveForAerieScript[];

void scJupiterLighthouseRideUpperWhirlwind(void);

void scJupiterLighthouseRideWhirlwind(void);

void scJupiterLighthouseOpenFloor(void);

void scJupiterLighthouseCloseFloor(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

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

void scCrackedTileCrack(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry);

void scCrackedTileFall(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry);

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#include "text/msg_ids.h"
#include "scene/lighthouse_palettes.h"

/* An action script's call: clears the actor's sprite flags. */
Sint32 scJupiterLighthouseClearScriptedActorSprFlags(Sint32 object)
{
	fdActorSetSprFlags(object, 0);
	return 0;
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

/* Where the battle this room starts returns the party, won or lost; and an
   entrance no exit or battle leads to, which plays Isaac and Ivan's fall again
   without its flags. */
enum { ENTRANCE_AFTER_THE_FIGHT = 77, ENTRANCE_ISAAC_AND_IVAN_FALL = 99 };

/* The poses of the fight on the terrace, by each sheet: Isaac hunched and
   battered, then on his back; Ivan on one knee, then down; Mia kneeling; Agatio
   bent over, catching his breath; Garet seen from behind, hanging by his hands
   from the ledge. */
enum {
	ANIM_ISAAC_STAGGER = 38,
	ANIM_ISAAC_LIE_BEATEN = 39,
	ANIM_IVAN_KNEEL = 31,
	ANIM_IVAN_LIE_BEATEN = 32,
	ANIM_MIA_KNEEL = 9,
	ANIM_AGATIO_CATCH_BREATH = 8,
	ANIM_GARET_HANG = 14
};

/* The energy block's gap: the metatile it drops into and lights the path
   from. */
enum { ENERGY_BLOCK_GAP_METATILE_X = 63, ENERGY_BLOCK_GAP_METATILE_Z = 15 };

/* The path from the energy block's gap lit, and the Hover pad at its end. */
#define scJupiterLighthouseLightPathToHoverPad() \
	(fdMapGridCopyMetatilesLayered(104, 42, 5, 11, 61, 42), \
		fdMapGridCopyMetatilesLayered(106, 52, 1, 1, 63, 84), \
		syFieldMapGridSetMetatileBehavior(toFIXED(1040), toFIXED(160), \
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_HOVER))

/* The trapdoor's four columns in front of the upper doorway, west to east. */
enum { TRAPDOOR_WEST_COLUMN = 49 };

/* The trapdoor shown open, and its metatiles left open to fall through. */
#define scJupiterLighthouseLeaveTrapdoorOpen() \
	(fdMapGridCopyMetatilesLayered(49, 106, 4, 3, 49, 76), \
		fdMapGridCopyMetatileAttributes(49, 45, 4, 1, 49, 44))

/* The terraces' own metatile behaviors. */
typedef enum JUPITER_LIGHTHOUSE_TERRACES_METATILE_BEHAVIOR {
	/* The west walkway's metatiles (13, 9) to (20, 9), in the west whirlwind's
	   path. */
	METATILE_BEHAVIOR_WEST_WHIRLWIND_PATH = 30,
	/* Metatile (65, 42), past the Hover pad the energy block lights, in the
	   east whirlwind's path. */
	METATILE_BEHAVIOR_EAST_WHIRLWIND_PATH,
	/* The purple button in front of the upper doorway, metatile (41, 39), and
	   the eight metatiles round it. */
	METATILE_BEHAVIOR_TRAPDOOR_BUTTON,
	METATILE_BEHAVIOR_OFF_TRAPDOOR_BUTTON
} JUPITER_LIGHTHOUSE_TERRACES_METATILE_BEHAVIOR;

/* The bits of a lightning map entry that number the map's tiles from 512;
   cleared, the map draws the tiles at the start of character block 0. */
#define LIGHTNING_MAP_TILE_BASE_MASK 0x300

/* More HP or PP than anyone has, so that taking it away leaves none; and the HP
   Isaac is left with for the fight that cannot be won. */
enum { ALL_HP_OR_PP = 10000, ISAAC_HP_IN_THE_FIGHT = 173 };

/* The actors this file moves by number: what the placements put where, and the
   people at the fight (ActorPlacements's last rows). */
enum {
	/* The stone block Move pushes into its gap, which the lightning strikes
	   (scJupiterLighthouseFlashSky). */
	ACTOR_ENERGY_BLOCK = 20,
	/* The two tightrope posts Lash ties the rope between. */
	ACTOR_EAST_ROPE_POST = 21,
	ACTOR_WEST_ROPE_POST = 22,
	/* The grey block pushed onto the trapdoor
	   (scJupiterLighthouseOpenFloor). */
	ACTOR_GRAY_BLOCK = 23,
	/* The unseen points the west and east whirlwinds blow out from. */
	ACTOR_WEST_UPPER_GUST = 24,
	ACTOR_EAST_UPPER_GUST = 25,
	/* "And I'm Agatio." */
	ACTOR_AGATIO = 26,
	/* "Me? Why, I'm Karst." */
	ACTOR_KARST = 27,
	ACTOR_KRADEN = 28,
	/* The point by Garet's hand on the ledge, where he strains every 128 ticks
	   (scJupiterLighthouseKeepGaretHanging). */
	ACTOR_GARETS_GRIP = 29
};

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

ACTOR *scWhirlwindSpawn(ACTOR *src);

/* Felix walks into the west whirlwind's path, on the walkway towards the left
   tower at metatiles (13, 9) to (20, 9): a whirlwind blows out at him, catches
   him and carries him spinning back east along the walkway, over the clouds, to
   the doorway (fdWhirlwindCarryLeader). Nothing happens while a whirlwind is
   already carrying him. */
void scJupiterLighthouseRideUpperWhirlwind(void)
{
	if(WhirlwindRiding == 0) {
		scWhirlwindSpawn(fdActorGet(ACTOR_WEST_UPPER_GUST));
		WhirlwindRiding = 1;
	}
}

/* Felix casts Hover on the pad the energy block lights, at metatile (65, 42),
   and rises in its glow: the east whirlwind catches him and carries him, still
   in Hover's rings, east along the chained walkway over the clouds to the right
   tower, and sets him down on the walkway's east end by the tower wall, beside
   the eye-shaped switch by its door (fdWhirlwindCarryLeader). His Hover's
   movement ends as he is caught. Walking there without Hover does nothing. */
void scJupiterLighthouseRideWhirlwind(void)
{
	if(WhirlwindRiding == 0) {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		if(party->movementMode == LEADER_MOVEMENT_HOVER) {
			ACTOR *actor = fdActorGet(party->leaderActorId);

			party->movementMode = LEADER_MOVEMENT_NONE;
			actor->speedLimit = 0;
			actor->acceleration = 0;
			scWhirlwindSpawn(fdActorGet(ACTOR_EAST_UPPER_GUST));
			WhirlwindRiding = 1;
		}
	}
}

Sint32 scActorEmitSmoke(ACTOR *actor);

/* Lightning strikes the energy block in a white flash, the ground shakes, Felix
   jumps back, sparks rise from the block, and the path lights up to the Hover
   pad beyond the gap, at metatile (65, 10). The bolt is a picture drawn on the
   first background layer, faded out over the scene. */
void scJupiterLighthouseFlashSky(void)
{
	Uint16 *buf;
	Uint16 *p;
	Uint32	i;

	{
		Uint8 *enabled = &CpuLoadDisplayEnabled;

		*enabled = 0;
	}
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syIOWrite16(BGCNT_SCREEN_BASE(LIGHTNING_SCREEN_BLOCK), &REG_BG0CNT);
	buf = syHeapAlloc(LIGHTNING_TILES * TILE_SIZE_4BPP);
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LIGHTNING_TILES), buf);
	syDMAXCopy(buf, (void *)BG_CHAR_BLOCK(0),
		DMA_COPY_WORDS(LIGHTNING_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buf);
	DMA_FILL16(0, BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK), BG_SCREEN_SIZE);
	buf = syHeapAlloc(
		LIGHTNING_MAP_ROWS * LIGHTNING_MAP_COLUMNS * sizeof(Uint16));
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_LIGHTNING_MAP), buf);
	p = buf;
	for(i = 0; i != LIGHTNING_MAP_COLUMNS * LIGHTNING_MAP_ROWS; i++)
		*p++ &= ~LIGHTNING_MAP_TILE_BASE_MASK;
	p = buf;
	for(i = 0; i != LIGHTNING_MAP_ROWS; i++) {
		syDMAXCopy(p,
			BG_SCREEN_BLOCK(LIGHTNING_SCREEN_BLOCK) +
				i * BG_SCREEN_WIDTH * sizeof(Uint16),
			DMA_COPY_WORDS(LIGHTNING_MAP_COLUMNS * sizeof(Uint16) / 4),
			REG_DMA3);
		p += LIGHTNING_MAP_COLUMNS;
	}
	syHeapRelease(buf);
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	BgScrollWork[0].x = -104;
	BgScrollWork[0].y = 16;
	syWait(30);
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, 0);
	fdPaletteFadeApplyTarget(RGB_WHITE, 0);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	fdPaletteFadeApplySource(RGB_WHITE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 0);
	fdPaletteFadeInterpolate(8);
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdScriptPlaySnd(SE_CLATTER);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	scFieldActorJump(ACTOR_FELIX, 6, 0);
	fdActorGet(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_FELIX, -16, 0);
	fdActorGet(ACTOR_ENERGY_BLOCK)->update = scActorEmitSmoke;
	for(i = 0; i != 2 * BLEND_WEIGHT_MAX; i++) {
		REG_BLDALPHA =
			(BLEND_WEIGHT_MAX - (i >> 1)) | BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
		syWait(1);
	}
	scJupiterLighthouseLightPathToHoverPad();
	fdPaletteFadeApplySource(PALETTE_TINT_WARM, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeInterpolate(60);
	syWindowLoadGraphics();
	syPrintClear();
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	REG_BLDALPHA = BLEND_ALPHA(8, 16);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	syWait(1);
	CpuLoadDisplayEnabled = 1;
	fdMapGridSetShake(0, 0, 0);
}

/* Nothing visible happens as Felix starts to push the energy block, by walking
   into it or with Move. The metatile the block stands on is kept at its top, 48
   pixels down, a step above the walkway 64 pixels down (metatile (60, 15) as
   the room is entered); as the push starts the game sets that metatile back to
   the walkway's 64 pixels down, so the metatile the block leaves is plain
   walkway again. Where the block stops, scJupiterLighthouseFinishMovingBlock
   raises its new metatile to 48 pixels down. */
void scJupiterLighthouseStartMovingBlock(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	Call4(fdMapGridSetMetatileHeight, actor->x.fixed, actor->z.fixed, 0, -8);
}

/* fdMapGridSetMetatileHeight, as the block's landing calls it. */
static __inline__ void scJupiterLighthouseSetMetatileHeight(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 height)
{
	fdMapGridSetMetatileHeight(x, z, layer, height);
}

/* The energy block, pushed or Moved three metatiles east into the gap at
   metatile (63, 15), drops in with a heavy thud. Felix steps back west of the
   gap, the light turns warm, he looks about with a question mark and leans
   back, and lightning strikes the block (scJupiterLighthouseFlashSky); the
   camera then follows the lit path to the Hover pad as the puzzle-solved jingle
   plays. Anywhere else, the block simply stands where it is put. */
void scJupiterLighthouseFinishMovingBlock(Sint32 unused, Sint32 id)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *actor = fdActorGetById(id);
	Sint32 x;
	Sint32 ticks;

	if(FIELD_METATILE(actor->x.fixed) == ENERGY_BLOCK_GAP_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == ENERGY_BLOCK_GAP_METATILE_Z) {
		fdScriptSetFlag(FLAG_ENERGY_BLOCK_IN_GAP);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		syWait(2);
		ticks = 0;
		if(actor->y.fixed > actor->groundHeight || actor->velY != 0) {
			do {
				syWait(1);
				ticks++;
				if(ticks > 29)
					break;
			} while(actor->y.fixed > actor->groundHeight || actor->velY != 0);
		}
		actor->motionFlg = 0;
		fdScriptPlaySnd(SE_HEAVY_IMPACT);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdCameraMoveTo(1016, CAMERA_KEEP, 232, 1);
		fdCameraWaitForArrival();
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
		x = FIELD_METATILE(leader->x.fixed);
		if(x == ENERGY_BLOCK_GAP_METATILE_X + 1) {
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 1032, 232);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 1000, 232);
		} else if(x == ENERGY_BLOCK_GAP_METATILE_X) {
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 1000, 232);
		}
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 1000, 248);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_EAST);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 0);
		fdPaletteFadeInterpolate(60);
		syFieldScriptWait(50);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST);
		syFieldScriptWait(20);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST);
		syFieldScriptWait(20);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST);
		syFieldScriptWait(20);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST);
		syFieldScriptWait(20);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		syFieldScriptWait(20);
		scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
			EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_EAST);
		syFieldScriptWait(20);
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
		syFieldScriptWait(40);
		scPaletteCycleSetPausesInScripts(0);
		scJupiterLighthouseFlashSky();
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->steeringFlg |=
			ACTOR_FACES_MOTION;
		scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
		fdActorSetAttachedEffect(
			ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		syFieldScriptWait(40);
		syFieldScriptWait(60);
		fdCameraSetSpeed(3);
		fdCameraMoveTo(1048, 0, 174, 1);
		fdCameraWaitForArrival();
		syFieldScriptWait(10);
		actor->update = 0;
		fdScriptPlaySnd(MUS_PUZZLE_SOLVED);
		mnSndWaitForJingle();
		syFieldScriptWait(10);
		fdCameraMoveTo(984, -64, 248, 1);
		fdCameraWaitForArrival();
		syFieldScriptWait(10);
		scPaletteCycleSetPausesInScripts(1);
		fdScriptEnd();
	}
	scJupiterLighthouseSetMetatileHeight(actor->x.fixed, actor->z.fixed, 0, -6);
}

/* Felix steps onto the purple button in front of the upper doorway, metatile
   (41, 39): the camera turns to the trapdoor of two green panels nearby, which
   swing open. If the grey block has been pushed one metatile east onto it, it
   falls through, the first time only, a few levels down to the rooms below with
   a crash and a shake: over the trapdoor's middle it first lands on a ledge
   with a shake and slides off sideways. Once the well's lid is lifted the
   button does nothing. */
void scJupiterLighthouseOpenFloor(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_GRAY_BLOCK);
	Sint32 spot = 0;
	Sint32 x;
	Sint32 ticks;

	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED))
		return;
	syGameFlagSet(FLAG_TRAPDOOR_OPEN);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_CHEST_OPEN);
	fdCameraMoveTo(776, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	fdScriptWait(6);
	sySndPlay(SE_FLOOR_SWITCH);
	fdMapGridCopyMetatilesLayered(49, 103, 4, 3, 49, 76);
	fdScriptWait(6);
	fdMapGridCopyMetatilesLayered(49, 106, 4, 3, 49, 76);
	x = FIELD_METATILE(actor->x.fixed);
	if(x == TRAPDOOR_WEST_COLUMN)
		spot = 1;
	if(x == TRAPDOOR_WEST_COLUMN + 1)
		spot = 2;
	if(x == TRAPDOOR_WEST_COLUMN + 2)
		spot = 3;
	if(x == TRAPDOOR_WEST_COLUMN + 3)
		spot = 4;
	if(actor->y.fixed != 0)
		spot = 0;
	if(fdScriptGetFlag(FLAG_GRAY_BLOCK_DROPPED))
		spot = 0;
	if(spot != 0)
		syGameFlagSet(FLAG_GRAY_BLOCK_DROPPED);
	if((Uint32)(spot - 2) <= 1) {
		fdCameraSetSpeed(4);
		syScriptCameraFollowActor(ACTOR_GRAY_BLOCK, 1);
		fdCameraWaitForArrival();
		sySndPlay(SE_FALL);
		actor->gravity = toFIXED(1) / 2;
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = toFIXED(-80);
		ticks = 0;
		if(actor->y.fixed > actor->groundHeight) {
			do {
				syWait(1);
				ticks++;
				if(ticks > 29)
					break;
			} while(actor->y.fixed > actor->groundHeight);
		}
		fdMapGridSetShake(3, 3, 0.95);
		sySndPlay(SE_HEAVY_IMPACT);
		actor->gravity = toFIXED(1);
		actor->velY = toFIXED(6);
		fdActorSetSpeed(ACTOR_GRAY_BLOCK, 1.5);
		if(spot == 2)
			fdActorSetDestinationOffset(ACTOR_GRAY_BLOCK, -32, 0);
		else
			fdActorSetDestinationOffset(ACTOR_GRAY_BLOCK, 32, 0);
		fdScriptWait(7);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		actor->gravity = toFIXED(1) / 2;
		actor->groundHeight = toFIXED(-240);
		fdScriptWait(80);
		fdMapGridSetShake(5, 5, 1);
		sySndPlay(SE_CLATTER);
		fdScriptWait(20);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdActorSetPos(ACTOR_GRAY_BLOCK, 0, 0);
		fdScriptWait(10);
	} else if(spot == 1 || spot == 4) {
		scFieldActorSetSprPriority(ACTOR_GRAY_BLOCK, 1);
		fdCameraSetSpeed(4);
		syScriptCameraFollowActor(ACTOR_GRAY_BLOCK, 1);
		fdCameraWaitForArrival();
		sySndPlay(SE_FALL);
		actor->gravity = toFIXED(1) / 2;
		actor->motionFlg = ACTOR_FALLS;
		actor->groundHeight = toFIXED(-240);
		fdScriptWait(20);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdScriptWait(60);
		fdMapGridSetShake(5, 5, 1);
		sySndPlay(SE_CLATTER);
		fdScriptWait(20);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdActorSetPos(ACTOR_GRAY_BLOCK, 0, 0);
		fdScriptWait(10);
	}
	fdScriptWait(20);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

/* Felix steps off the purple button in front of the upper doorway: the camera
   turns to the trapdoor, which swings shut with a thud. Once the well's lid is
   lifted the trapdoor stays as it is. */
void scJupiterLighthouseCloseFloor(void)
{
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED) != 0)
		return;
	fdScriptClearFlag(FLAG_TRAPDOOR_OPEN);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_CHEST_CLOSE);
	fdCameraMoveTo(776, CAMERA_KEEP, 208, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(6);
	fdMapGridCopyMetatilesLayered(49, 103, 4, 3, 49, 76);
	syFieldScriptWait(6);
	fdScriptPlaySnd(SE_IMPACT);
	fdMapGridCopyMetatilesLayered(49, 100, 4, 3, 49, 76);
	syFieldScriptWait(20);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Talking to Isaac, Ivan, Kraden, Jenna or Sheba once Isaac and Ivan have
   fallen: each answers with a line of their own. */
void scJupiterLighthouseTalkAfterTheFight(Sint32 actor)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	switch(actor) {
	case ACTOR_ISAAC:
		fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
		syFieldScriptSetMessageId(MsgNoWhyAre);
		/* "Felix... No! Why are you helping them?" */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		break;
	case ACTOR_IVAN:
		fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
		syFieldScriptSetMessageId(MsgDonTWorry);
		/* "Don't worry about us... Help Garet and Mia first." */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		break;
	case ACTOR_KRADEN:
		syFieldScriptSetMessageId(MsgWeHaveTo);
		/* "We have to help Garet. He can't hold on for much longer." */
		fdScriptShowMessage(ACTOR_KRADEN, 0);
		break;
	case ACTOR_JENNA:
		syFieldScriptSetMessageId(MsgIDonT);
		/* "Isaac! I don't know how you survived that attack, but I'm so
		   glad..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		break;
	case ACTOR_SHEBA:
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		syFieldScriptWait(33);
		syFieldScriptSetMessageId(MsgYouVeBeen);
		/* "Ivan... You've been injured. Don't worry about the others.
		   Let's..." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
		break;
	}
	fdScriptEnd();
}

/* Garet's sprite's rotation swings by the sine over this: up to a 128th of a
   turn either way. */
#define GARET_SWING_DIVISOR 128

/* A tick event while Garet hangs from the ledge: his sprite, pivoting on his
   hand, swings on a sine about a tilt of a 32nd of a turn, and every 128 ticks
   his grip strains. */
Sint32 scJupiterLighthouseKeepGaretHanging(void)
{
	ACTOR *actor = fdActorGet(ACTOR_GARET);

	if(actor != NULL) {
		SPR *spr = actor->spr;

		if((TickCount & 127) == 0)
			fdActorSetAttachedEffect(ACTOR_GARETS_GRIP,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		if(spr != NULL) {
			/* Drawn down and to the left of his point, below his grip. */
			spr->ofsY = 30;
			spr->ofsX = -4;
			spr->rotation =
				syMathSin(TickCount * DEGtoANG(4)) / GARET_SWING_DIVISOR -
				TRIG_TURN / 32;
		}
	}
	return 1;
}

/* An action script's call during the fight: the sound of one of five blows, at
   random. */
Sint32 scJupiterLighthousePlayRandomBlowSnd(void)
{
	switch(syRandom() & 7) {
	case 0:
		fdScriptPlaySnd(SE_RECOVERY);
		break;
	case 1:
		fdScriptPlaySnd(SE_POWER_UP);
		break;
	case 2:
		fdScriptPlaySnd(SE_HEAVY_IMPACT);
		break;
	case 3:
	case 5:
	case 6:
	default:
		fdScriptPlaySnd(SE_IMPACT);
		break;
	case 4:
	case 7:
		fdScriptPlaySnd(SE_CAST);
		break;
	}
	return 0;
}

/* An action script's call: stops the actor turning to face where it moves. */
Sint32 scJupiterLighthouseStopFacingMotion(ACTOR *actor)
{
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	return 0;
}

extern Uint32 JupiterLighthouseIsaacFightsScript[];
extern Uint32 JupiterLighthouseAgatioFightsScript[];
extern Uint32 JupiterLighthouseIvanFightsScript[];
extern Uint32 JupiterLighthouseKarstFightsScript[];

/* Places an actor at (x, z), in pixels, facing a direction. */
#define PLACE(id, x, z, facing) \
	{ \
		Sint32 facing_ = (facing); \
		fdActorSetPosAndFacing(id, toFIXED(x), toFIXED(z), facing_); \
	}

/* Sets four actors' speeds, places them and starts their action scripts. */
void scJupiterLighthouseStartTheFight(void)
{
	fdActorSetSpeed(ACTOR_AGATIO, 3);
	fdActorSetSpeed(ACTOR_KARST, 3);
	fdActorSetSpeed(ACTOR_ISAAC, 3);
	fdActorSetSpeed(ACTOR_IVAN, 3);
	PLACE(ACTOR_ISAAC, 776, 202, FACING_WEST);
	PLACE(ACTOR_IVAN, 731, 232, FACING_NORTH);
	PLACE(ACTOR_KARST, 731, 213, FACING_SOUTH);
	PLACE(ACTOR_AGATIO, 737, 202, FACING_EAST);
	fdActorSetScript(ACTOR_ISAAC, JupiterLighthouseIsaacFightsScript);
	fdActorSetScript(ACTOR_IVAN, JupiterLighthouseIvanFightsScript);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseAgatioFightsScript);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseKarstFightsScript);
	fdActorSetScript(ACTOR_MIA, JupiterLighthouseMiaScript);
}

/* Places Isaac, Ivan and Mia where the fight is, and Garet hanging from the
   ledge by his grip, swaying. */
void scJupiterLighthousePlaceForTheFight(void)
{
	PLACE(ACTOR_IVAN, 757, 216, FACING_SOUTHEAST);
	PLACE(ACTOR_ISAAC, 761, 219, FACING_SOUTHEAST);
	PLACE(ACTOR_MIA, 816, 285, FACING_SOUTHEAST);
	PLACE(ACTOR_GARET, 824, 302, FACING_NORTH);
	PLACE(ACTOR_GARETS_GRIP, 821, 302, FACING_NORTH);
	fdActorSetSprFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorSetScript(ACTOR_GARET, JupiterLighthouseGaretHangsScript);
	{
		Sint32 priority = EVENT_PASS_TICK;

		syFieldSetTickEvent(
			(void (*)(void))scJupiterLighthouseKeepGaretHanging, priority);
	}
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_ISAAC_STAGGER);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_IVAN_KNEEL);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
}

extern Uint32 JupiterLighthouseFelixArrivesWestScript[];
extern Uint32 JupiterLighthouseFelixArrivesEastScript[];
extern Uint32 JupiterLighthousePiersArrivesEastScript[];
extern Uint32 JupiterLighthouseJennaArrivesWestScript[];
extern Uint32 JupiterLighthouseShebaArrivesEastScript[];
extern Uint32 JupiterLighthouseKarstStepsUpScript[];

/* Felix comes out by the aerie to find Jenna, Sheba, Piers and Kraden watching
   the fight, and the party is placed to watch with them. `west` brings them out
   on the west side of the terrace; the scene only ever passes it set, so the
   east side's scripts go unused. */
void scJupiterLighthousePartyArrivesAtTheFight(Sint32 west)
{
	ACTOR *actor;
	Sint32 ang;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId(MsgAhhhh);
	fdScriptPlaySnd(SE_CHEST_OPEN);
	syFieldScriptWait(26);
	fdScriptPlaySnd(SE_FLOOR_SWITCH);
	fdActorSetScript(ACTOR_FELIX, JupiterLighthouseFelixLooksAroundScript);
	/* Mia's and Garet's cries, then Isaac's and Ivan's, in windows on the
	   screen's right (tile column 20 or 21) when the party is on the west side,
	   else on its left (column 5): the first speaker's at tile row 9, the
	   second's at row 2, each portrait 5 columns left of its window and a row
	   above. */
	if(west != 0) {
		/* "Ahhhh!!!" and "Mia!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_MIA, 20, 9, 15, 8, ACTOR_GARET, 20, 2, 15, 1, 25);
		fdScriptPlaySnd(SE_FALL);
		syFieldScriptWait(30);
		fdScriptPlaySnd(SE_CLATTER);
		/* "Noooo!!!" and "Oof!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_MIA, 21, 9, 16, 8, ACTOR_GARET, 21, 2, 16, 1, 0);
		/* "Mia!" and "Garet!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_ISAAC, 20, 9, 15, 8, ACTOR_IVAN, 20, 2, 15, 1, 0);
	} else {
		/* "Ahhhh!!!" and "Mia!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_MIA, 5, 9, 0, 8, ACTOR_GARET, 5, 2, 0, 1, 25);
		fdScriptPlaySnd(SE_FALL);
		syFieldScriptWait(30);
		fdScriptPlaySnd(SE_CLATTER);
		/* "Noooo!!!" and "Oof!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_MIA, 5, 9, 0, 8, ACTOR_GARET, 5, 2, 0, 1, 0);
		/* "Mia!" and "Garet!" */
		syFieldScriptShowTwoMessagesAndWait(
			ACTOR_ISAAC, 5, 9, 0, 8, ACTOR_IVAN, 5, 2, 0, 1, 0);
	}
	scFieldActorStop(ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_WEST);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_EAST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, 24, FACING_WEST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 24, FACING_EAST);
	syScriptCameraFollowActor(ACTOR_JENNA, 1);
	scFieldActorWaitForMoveById(ACTOR_JENNA);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Is that... It's Isaac and the others!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
	/* "Isaac? Isn't he a friend of yours, Felix?" Yes: "They both come from
	   Vale..." No: "Don't be that way! You two have much in common..." */
	fdScriptOpenMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
		syFieldScriptWait(20);
		syFieldScriptWait(10);
		/* "They both come from Vale, and regardless of what Isaac
		   believes..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
		(FieldStateWork->msg)++;
	} else {
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		syFieldScriptWait(33);
		(FieldStateWork->msg)++;
		/* "Don't be that way! You two have much in common, even if you are
		   at..." */
		syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	}
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	syFieldScriptWait(5);
	/* "But Isaac and the others came all this way just to try and stop us!" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "But didn't you hear that? It sounded like a cry for help... What
	   if..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetSpeed(ACTOR_FELIX, 1.2);
	fdActorSetScript(ACTOR_FELIX,
		west != 0 ? JupiterLighthouseFelixArrivesWestScript
				  : JupiterLighthouseFelixArrivesEastScript);
	syFieldScriptWait(30);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	syFieldScriptWait(30);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetScript(ACTOR_KRADEN,
		west != 0 ? JupiterLighthouseKradenArrivesWestScript
				  : JupiterLighthouseKradenArrivesEastScript);
	fdActorSetScript(ACTOR_PIERS,
		west != 0 ? JupiterLighthousePiersArrivesWestScript
				  : JupiterLighthousePiersArrivesEastScript);
	fdActorSetScript(ACTOR_JENNA,
		west != 0 ? JupiterLighthouseJennaArrivesWestScript
				  : JupiterLighthouseJennaArrivesEastScript);
	fdActorSetScript(ACTOR_SHEBA,
		west != 0 ? JupiterLighthouseShebaArrivesWestScript
				  : JupiterLighthouseShebaArrivesEastScript);
	fdCameraSetSpeed(0.5);
	if(west != 0) {
		fdCameraMoveTo(677, -64, 287, 1);
		fdCameraWaitForArrival();
	} else {
		fdCameraMoveTo(950, -64, 287, 1);
		fdCameraWaitForArrival();
	}
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	scFieldActorWaitForScriptEndById(ACTOR_PIERS);
	scFieldActorWaitForScriptEndById(ACTOR_JENNA);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syFieldScriptWait(20);
	ang = FACING_SOUTHEAST;
	fdActorSetPosAndFacing(ACTOR_IVAN, toFIXED(741), toFIXED(232), ang);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(800, 0, 270, 1);
	fdCameraWaitForArrival();
	fdScriptPlaySnd(MUS_ALEX);
	syFieldScriptWait(50);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	/* "Hang on, Mia! Don't worry, Garet! Everything will be fine!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STILL);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
	/* "Don't worry about me... I'm fine, but Garet..." */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_MIA, ang);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	/* "Garet, you're only using one arm to hang on... What's the matter?" */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	syFieldScriptWait(5);
	/* "I don't know... I think I hurt my arm when I fell... It's totally..." */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 5);
	scFieldActorStartRepeatedMotion(ACTOR_MIA, 3);
	fdActorSetAttachedEffect(
		ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(40);
	/* "You shouldn't have tried to save me when I fell into that hole..." */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	syFieldScriptWait(5);
	/* "Oh, Mia, don't blame yourself... I wasn't going to leave you down
	   there!" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 5);
	fdActorGet(ACTOR_MIA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_MIA, 0.1);
	fdActorSetDestinationOffset(ACTOR_MIA, 4, 2);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetDestinationOffset(ACTOR_MIA, -2, 2);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetDestinationOffset(ACTOR_MIA, 2, 1);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetDestinationOffset(ACTOR_MIA, -2, -1);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	syFieldScriptWait(40);
	fdActorSetDestinationOffset(ACTOR_MIA, 0, -2);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetSpeed(ACTOR_MIA, 2);
	fdActorAnimateThenWait(ACTOR_MIA, ANIM_SHAKE_HEAD, 33);
	/* "It's no use! I can't pull you up, Garet! I'm not strong enough!" */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 5);
	fdActorGet(ACTOR_MIA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetDestinationOffset(ACTOR_MIA, -2, -8);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "Isaac, please! You've got to save Garet!" */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 5);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "This doesn't look good... We have to save Garet, and fast!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorAnimateThenWait(ACTOR_IVAN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorSetScript(ACTOR_MIA, JupiterLighthouseMiaScript);
	fdActorSetPosAndFacing(ACTOR_AGATIO, toFIXED(665), toFIXED(128), ang);
	fdActorSetPosAndFacing(ACTOR_KARST, toFIXED(641), toFIXED(128), ang);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "No, I'm afraid you won't be doing that..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
	syFieldScriptWait(10);
	fdCameraMoveTo(720, 0, 210, 1);
	fdActorSetSpeedFX(ACTOR_AGATIO, toFIXED(1) / 4, ang);
	fdActorSetSpeedFX(ACTOR_KARST, toFIXED(1) / 4, ang);
	fdActorWalkTo(ACTOR_AGATIO, 714, 193);
	fdActorWalkTo(ACTOR_KARST, 698, 209);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseAgatioStepsUpScript);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseKarstStepsUpScript);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Who are you!?" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdScriptPlaySnd(MUS_KARST_AND_AGATIO);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Me? Why, I'm Karst." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "And I'm Agatio." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "If you're not here to help my friend, then get out of my way, so
	   I..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "Your friends will have to take care of themselves. You have a
	   little..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Debt? What are you talking about? We've never even seen you before!" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Regardless, you have done Karst here a great wrong, and you're..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
	/* "Wait a second... You two look familiar somehow... Do you know..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "So, you're not such a fool after all, Isaac! You are right. We
	   are..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Prox? Never heard of it." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "That doesn't surprise me. Our town hangs on the brink of extinction." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "And the seal placed on Alchemy is responsible!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "What are you saying? That doesn't make any sense." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	syFieldScriptWait(5);
	/* "It matters not! For soon, Prox will recover its lost power..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, ang);
	syFieldScriptWait(10);
	/* "We shall bring Prox back from the edge, and then all the people
	   of..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You think we'd let you, after hearing all that nonsense?" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "See! You're a hindrance!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "And Felix was trying to help you?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Actually...we don't know what Felix's objectives are." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "And we don't care, as long as he lights the beacon on Jupiter..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	syFieldScriptWait(5);
	/* "Then get out of our way! We're here to stop Felix!" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorSetAnimation(ACTOR_KARST, ANIM_SHAKE_HEAD);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Move, or we'll have to move you ourselves!" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Did you hear that, Karst? They actually want to fight?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "You mean to tell us that you would leave your friend...hanging?" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, ang);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_ISAAC_STAGGER);
	fdCameraMoveTo(784, 0, 274, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(60);
	fdCameraMoveTo(720, 0, 210, 1);
	fdCameraWaitForArrival();
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_STAND);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "So this was all part of your plan?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorFaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Cowards! Stop playing dirty and fight fair!" */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorFaceDirection(ACTOR_AGATIO, ang, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, ang);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "If you really are the brats who killed Saturos and Menardi, then..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "But answer me this: would you still cry foul if you were fighting
	   us..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "That's why you were waiting up here? To set a trap and make Mia
	   and..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "We didn't plan on snaring both of them, though... That was a..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* "So that's the deal, is it? You came up here to fight us?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Oh, did you figure that out all on your own? Impressive! I'm
	   quite..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHEAST);
	syFieldScriptWait(10);
	/* "I hope you don't think we're afraid of a fair fight, two on two..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "A fair fight?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "I'm sorry if I misled you, but we have a third... A Water Adept...
	   Alex!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTHWEST);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorSetSpeed(ACTOR_KARST, 2);
	fdActorWalkByAndWait(ACTOR_KARST, -24, -24);
	syFieldScriptWait(5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_WEST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_EAST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Alex? ...Where is he?" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	/* "What's wrong, Karst?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, ang);
	syFieldScriptWait(10);
	/* "It's Alex! He's gone!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "What!?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceEachOther(ACTOR_ISAAC, ACTOR_IVAN, 0);
	syFieldScriptWait(10);
	/* "Alex!" and "Alex?" */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_ISAAC, 20, 7, 15, 6, ACTOR_IVAN, 17, 13, 12, 12, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Did anyone see where he went?" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Forget about him! Honestly, I was planning to do the same to him..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, ang);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "My only regret is that I won't be able to wipe that sneering
	   smile..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorWalkBy(ACTOR_KARST, 24, 24);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseKarstTurnsScript);
	/* "Well, take out all of your aggressions on these brats... You'll
	   feel..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorFaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 30);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorSetSpeed(ACTOR_AGATIO, 3);
	fdActorSetSpeed(ACTOR_KARST, 3);
	fdActorSetSpeed(ACTOR_ISAAC, 3);
	fdActorSetSpeed(ACTOR_IVAN, 3);
	fdActorWalkTo(ACTOR_AGATIO, 737, 202);
	fdActorWalkTo(ACTOR_KARST, 731, 213);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseKarstFacesSouthScript);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseAgatioFacesEastScript);
	syFieldScriptWait(5);
	fdActorWalkTo(ACTOR_ISAAC, 776, 202);
	fdActorWalkTo(ACTOR_IVAN, 731, 232);
	fdActorSetScript(ACTOR_ISAAC, JupiterLighthouseIsaacFacesWestScript);
	fdActorSetScript(ACTOR_IVAN, JupiterLighthouseIvanFacesNorthScript);
	scFieldActorWaitForScriptEndById(ACTOR_ISAAC);
	scFieldActorWaitForScriptEndById(ACTOR_IVAN);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	syFieldScriptWait(40);
	scJupiterLighthouseStartTheFight();
	syFieldScriptWait(200);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(5);
	/* "Felix! Agatio and Karst are attacking Isaac and the others!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Do you think Isaac and Ivan can handle them without Mia and
	   Garet's..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I'm disgusted that Karst would set a trap for them. It's
	   dishonorable!" */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "What if Garet loses his grip while they're still fighting?" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH);
	syFieldScriptWait(10);
	/* "There's still time! We have to help Isaac!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestination(
			ACTOR_PIERS, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestination(
			ACTOR_SHEBA, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestination(
			ACTOR_JENNA, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != NULL)
		fdActorSetDestination(
			ACTOR_KRADEN, actor->x.part[0].integer, actor->z.part[0].integer);
	fdActorSetScript(ACTOR_PIERS, JupiterLighthouseGatherAtFelixScript);
	fdActorSetScript(ACTOR_SHEBA, JupiterLighthouseGatherAtFelixScript);
	fdActorSetScript(ACTOR_JENNA, JupiterLighthouseGatherAtFelixScript);
	fdActorSetScript(ACTOR_KRADEN, JupiterLighthouseGatherAtFelixScript);
	scFieldActorWaitForScriptEndById(ACTOR_PIERS);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	scFieldActorWaitForScriptEndById(ACTOR_JENNA);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	fdScriptEnd();
}

/* gmUnitAddPp as a script calls it, beside system_call.h's scFieldUnitAddHp. */
static inline Sint32 scFieldUnitAddPp(Sint32 owner, Sint32 delta)
{
	return gmUnitAddPp(owner, delta);
}

/* The fight goes against Isaac and Ivan: Ivan can take no more, Agatio mocks
   Isaac's concern for him, and the battle that cannot be won begins, the two
   the whole party, Ivan with no HP, Isaac with ISAAC_HP_IN_THE_FIGHT, and
   neither with PP. */
void scJupiterLighthouseIsaacAndIvanAreHurt(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	PLACE(ACTOR_ISAAC, 776, 202, FACING_WEST);
	PLACE(ACTOR_IVAN, 731, 232, FACING_NORTHWEST);
	PLACE(ACTOR_KARST, 731, 213, FACING_SOUTH);
	PLACE(ACTOR_AGATIO, 737, 202, FACING_EAST);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_STAND);
	fdActorSetAnimation(ACTOR_KARST, ANIM_STAND);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId(MsgIMSorry);
	/* "Isaac... I'm sorry... I can't take any more..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdCameraMoveTo(705, 0, 177, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(30);
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_IVAN_LIE_BEATEN);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHWEST);
	/* "Keep it together, Ivan!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Ha! You're a fool! Never let concern distract you in battle!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorWalkBy(ACTOR_AGATIO, 24, 0);
	syFieldScriptWait(20);
	SET_BATTLE_TYPE(BATTLE_TYPE_LOST);
	syFieldScriptSetAfterBattleDestination(
		SCENE_JUPITER_LIGHTHOUSE_TERRACES, ENTRANCE_AFTER_THE_FIGHT);
	fdScriptSetDefeatDestination(
		SCENE_JUPITER_LIGHTHOUSE_TERRACES, ENTRANCE_AFTER_THE_FIGHT);
	gmPartyRemoveMember(FELIX);
	gmPartyRemoveMember(PIERS);
	gmPartyRemoveMember(JENNA);
	gmPartyRemoveMember(SHEBA);
	gmPartyRemoveMember(IVAN);
	gmPartyRemoveMember(ISAAC);
	gmPartyAddMember(IVAN);
	gmPartyAddMember(ISAAC);
	gmPartyRemoveMember(MIA);
	gmPartyRemoveMember(GARET);
	scFieldUnitAddHp(IVAN, -ALL_HP_OR_PP);
	scFieldUnitAddHp(ISAAC, -ALL_HP_OR_PP);
	gmUnitCalc(ISAAC);
	gmUnitCalc(IVAN);
	scFieldUnitAddHp(ISAAC, ISAAC_HP_IN_THE_FIGHT);
	scFieldUnitAddPp(IVAN, -ALL_HP_OR_PP);
	gmUnitAddPp(ISAAC, -ALL_HP_OR_PP);
	MainRoutineStateWork.leaderActorId = ACTOR_ISAAC;
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
}

/* Places Isaac and Ivan lying hurt after the fight, with Jenna, Sheba and
   Kraden beside them. */
void scJupiterLighthousePlaceTheHurt(void)
{
	PLACE(ACTOR_ISAAC, 761, 219, FACING_NORTHWEST);
	PLACE(ACTOR_IVAN, 741, 232, FACING_NORTHWEST);
	fdActorSetSprFlags(fdActorGet(ACTOR_ISAAC), 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_ISAAC_LIE_BEATEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_IVAN_LIE_BEATEN);
	PLACE(ACTOR_JENNA, 761, 203, FACING_SOUTH);
	PLACE(ACTOR_SHEBA, 725, 232, FACING_EAST);
	PLACE(ACTOR_KRADEN, 729, 203, FACING_SOUTHEAST);
	fdActorSetScript(ACTOR_MIA, JupiterLighthouseMiaScript);
}

/* After the battle with Karst and Agatio: the two stand over the fallen Isaac
   and Ivan until Felix and the others step in, Felix promises to light the
   beacon, and the Mythril Bag holding the Jupiter Star is swapped for a plain
   one (ITEM_MYTHRIL_BAG_B) that Felix equips. */
void scJupiterLighthouseIsaacAndIvanFall(void)
{
	Sint32 idx;
	Sint32 answer;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(737, 0, 209, 0);
	MainRoutineStateWork.leaderActorId = ACTOR_FELIX;
	gmPartyRemoveMember(ISAAC);
	gmPartyRemoveMember(IVAN);
	gmPartyRemoveMember(JENNA);
	gmPartyRemoveMember(SHEBA);
	gmPartyRemoveMember(PIERS);
	gmPartyRemoveMember(MIA);
	gmPartyRemoveMember(GARET);
	gmPartyAddMember(FELIX);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetPosAndFacing(
		ACTOR_FELIX, toFIXED(664), toFIXED(88), FACING_SOUTH);
	fdActorSetPosAndFacing(
		ACTOR_KARST, toFIXED(717), toFIXED(208), FACING_SOUTHEAST);
	fdActorSetPosAndFacing(
		ACTOR_AGATIO, toFIXED(737), toFIXED(195), FACING_SOUTHEAST);
	fdActorSetPosAndFacing(
		ACTOR_ISAAC, toFIXED(761), toFIXED(219), FACING_NORTHWEST);
	fdActorSetPosAndFacing(
		ACTOR_IVAN, toFIXED(741), toFIXED(232), FACING_NORTHWEST);
	fdActorSetScript(ACTOR_MIA, (Sint32)JupiterLighthouseMiaScript);
	fdActorSetSprFlags(fdActorGet(ACTOR_ISAAC), 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_ISAAC_LIE_BEATEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_IVAN_LIE_BEATEN);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId((Sint32)MsgWeDidItAgatio);
	/* "We did it, Agatio!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_AGATIO_CATCH_BREATH);
	syFieldScriptWait(40);
	/* "It was close, but we did it." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_EAST);
	syFieldScriptWait(10);
	/* "Heh... He's no warrior. His concern for his friend gave us the..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	scFieldActorStartRepeatedMotion(ACTOR_ISAAC, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	syFieldScriptWait(5);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "What's this? He's still alive!? Even after that blow?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTHEAST);
	syFieldScriptWait(10);
	/* "It would be a shame if you and I ever had to regret letting him
	   live..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_STAND);
	syFieldScriptWait(30);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH_SOUTHWEST);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	syFieldScriptWait(10);
	syScriptCameraGetActor()->motionFlg = 0;
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	syFieldScriptWait(20);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 0, 0, FACING_SOUTH);
	syScriptCameraFollowActor(ACTOR_SHEBA, 1);
	scFieldActorLaunchFromLeader(ACTOR_PIERS, 16, 0, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 24, FACING_SOUTHEAST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_SOUTHEAST);
	fdActorWalkByAndWait(ACTOR_FELIX, 16, 24);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Isaac! Watch out!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	/* "He's still alive! If we're going to act, Felix, we have to act now!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 35);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorWalkTo(ACTOR_FELIX, 683, 186);
	fdActorWalkTo(ACTOR_KRADEN, 667, 202);
	fdActorSetScript(ACTOR_FELIX, (Sint32)JupiterLighthouseFelixWaitsScript);
	fdActorSetScript(ACTOR_KRADEN, (Sint32)JupiterLighthouseKradenWaitsScript);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KARST, FACING_WEST, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "They're going to kill Isaac!" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
	/* "No, they aren't! I'm not about to let that happen!" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_JENNA, 0);
	/* "None of us are, Jenna!" */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorWalkBy(ACTOR_PIERS, 0, 40);
	fdActorWalkBy(ACTOR_SHEBA, 0, 40);
	fdActorWalkBy(ACTOR_JENNA, 0, 40);
	fdActorSetScript(ACTOR_PIERS, (Sint32)JupiterLighthousePiersStepsUpScript);
	fdActorSetScript(ACTOR_SHEBA, (Sint32)JupiterLighthouseShebaStepsUpScript);
	fdActorSetScript(ACTOR_JENNA, (Sint32)JupiterLighthouseJennaStepsUpScript);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Felix? What's going on?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Be grateful! We just did you a favor!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
	/* "I understand that when Venus was lit, the ground shook so
	   fiercely..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "You haven't lit the beacon yet, have you? And after all we've
	   done..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(5);
	/* "We're going to light the beacon right now..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "If that's true, then you'd better go do it!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
	/* "But we're not leaving Isaac behind." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Oh, great... Are you going to betray us now?" Yes: "Typical. And you're
	   going to try to stop us..." No: "Don't lie... We can see it in your
	   eyes." */
	fdScriptOpenMessage(ACTOR_KARST, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	syFieldScriptWait(10);
	answer = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(answer == 0) {
		fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "Typical. And you're going to try to stop us from finishing them
		   off..." */
		syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		FieldStateWork->msg++;
		/* "Don't lie... We can see it in your eyes. We can see what you
		   truly..." */
		syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	}
	fdActorFaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "If you're only here to clear the way for us to light the beacon..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
	/* "Isaac has proven to be a greater foe than we'd imagined. We can't..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Consider how powerful these two were without the aid of a Water..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
	/* "If you want them, you're going to have to face us first!" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorFaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_KARST, 0);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	fdActorFaceActor(ACTOR_AGATIO, ACTOR_KRADEN, 0);
	fdActorFaceActor(ACTOR_KARST, ACTOR_KRADEN, 0);
	/* "You're both quite badly injured... You're in no shape to fight
	   all..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	syFieldScriptWait(60);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "We haven't much choice..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "Karst, no!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "Agatio, if we fight Felix now...we'll be destroyed." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Isaac killed your sister! Where is your anger!?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I am filled with rage, but not so much as to be blinded by it." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "What, then? Do we give up?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_WEST);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Fine! But this is not the end." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_NORTHWEST);
	syFieldScriptWait(10);
	/* "Do with Isaac as you will." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
	fdActorFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
	fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_KRADEN, ACTOR_AGATIO, 0);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "But swear this oath to us: if we leave now, you will light the beacon's
	   fire." Yes: "I don't care if you want to help Garet or not!..." No: "We
	   do not have time to wait for you to save Garet!" */
	fdScriptOpenMessage(ACTOR_AGATIO, 0);
	answer = syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
	if(answer == 0) {
		fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
		/* "I don't care if you want to help Garet or not! The lighthouse
		   comes..." */
		syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
		FieldStateWork->msg++;
		/* "We do not have time to wait for you to save Garet!" */
		syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	}
	fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_FELIX, 0);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Go, Felix. I will tend to Garet." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(40);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(30);
	fdActorFaceEachOther(ACTOR_AGATIO, ACTOR_KARST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceActor(ACTOR_AGATIO, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_KARST, 0);
	fdActorWalkToAndWait(ACTOR_KARST, 699, 202);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "And be sure to bring with you the Mars Star that Isaac carries on
	   him." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	/* "How did you know about the Mars Star?" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Did you think this was about nothing more than revenge? Alex told
	   us..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorFaceEachOther(ACTOR_FELIX, ACTOR_PIERS, 0);
	fdActorFaceEachOther(ACTOR_SHEBA, ACTOR_JENNA, 0);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	scFieldActorWaitForScriptEndById(ACTOR_JENNA);
	syFieldScriptWait(40);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_PIERS, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_SHEBA, ACTOR_AGATIO, 0);
	fdActorFaceActor(ACTOR_JENNA, ACTOR_AGATIO, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	fdActorFaceDirection(ACTOR_KARST, FACING_EAST, 0);
	/* "Take the Mars Star, or we'll be forced to make you!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorSetSpeed(ACTOR_PIERS, 4);
	fdActorSetSpeed(ACTOR_KARST, 2);
	fdActorWalkBy(ACTOR_PIERS, 8, 24);
	fdActorWalkByAndWait(ACTOR_KARST, 8, 4);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	syFieldScriptWait(5);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	fdActorFaceActor(ACTOR_AGATIO, ACTOR_PIERS, 0);
	fdActorFaceActor(ACTOR_KARST, ACTOR_PIERS, 0);
	/* "Do not force our hand on this!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 5);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* "Go on, Felix. Take it." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	fdActorFaceActor(ACTOR_AGATIO, ACTOR_ISAAC, 0);
	fdActorFaceActor(ACTOR_KARST, ACTOR_ISAAC, 0);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* "I don't know why you're doing this, but I trust you... Take the
	   Mars..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 5);
	syFieldScriptWait(40);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	scFieldActorWatchAndSetScript(ACTOR_AGATIO, ACTOR_FELIX);
	scFieldActorWatchAndSetScript(ACTOR_KARST, ACTOR_FELIX);
	fdActorWalkByAndWait(ACTOR_FELIX, 5, 32);
	fdActorWalkByAndWait(ACTOR_FELIX, 56, -2);
	syFieldScriptWait(30);
	scFieldActorStartRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(4);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	gmPartyInventoryRemove(ITEM_MYTHRIL_BAG_JUPITER_STAR);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG_B);
	/* "Felix took the Mars Star." */
	mnMessageShowCentered((Sint16)FieldStateWork->msg++, 5);
	idx = 0;
	for(idx = 0; idx <= INVENTORY_SLOTS - 1; idx++) {
		if((syUnitGet(FELIX)->inventory[idx] & INVENTORY_ITEM_ID_MASK) ==
			ITEM_MYTHRIL_BAG_B) {
			gmInventoryEquip(FELIX, idx);
			break;
		}
	}
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "That's better, Felix. We'll be waiting for you on the aerie." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 5);
	fdActorFaceActor(ACTOR_FELIX, ACTOR_KARST, 0);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_PIERS, 2);
	fdActorSetSpeed(ACTOR_AGATIO, 0.5);
	fdActorSetSpeed(ACTOR_KARST, 0.5);
	fdActorSetSpeed(ACTOR_FELIX, 0.5);
	fdActorSetScript(ACTOR_AGATIO, (Sint32)JupiterLighthouseWalkToAerieScript);
	syFieldScriptWait(40);
	fdActorSetDestinationOffset(ACTOR_PIERS, -16, 0);
	syFieldScriptWait(10);
	fdActorSetScript(ACTOR_KARST, (Sint32)JupiterLighthouseWalkToAerieScript);
	syFieldScriptWait(110);
	fdActorSetScript(ACTOR_FELIX, (Sint32)JupiterLighthouseWalkToAerieScript);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	fdActorSetScript(
		ACTOR_AGATIO, (Sint32)JupiterLighthouseLeaveForAerieScript);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorSetScript(ACTOR_KARST, (Sint32)JupiterLighthouseLeaveForAerieScript);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
	scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	/* "Felix, wait..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	syScriptCameraFollowActor(ACTOR_SHEBA, 1);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "We cannot trust them. Take me with you..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 5);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Good thinking. If something happens up there, you'll be safer
	   with..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	/* "We'll be fine. Take Piers with you..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 5);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Listen to the others... Safety in numbers, as they say..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 5);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorSetScript(ACTOR_PIERS, (Sint32)JupiterLighthouseWalkToAerieScript);
	scFieldActorWaitForScriptEndById(ACTOR_PIERS);
	gmPartyAddMember(PIERS);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	syFieldScriptWait(20);
#if AERIE_PARTY_CLEARS_SHORTCUTS
	fdPartyClearUnusableShortcuts();
#endif
	fdActorWalkToAndWait(ACTOR_FELIX, 664, 70);
	fdScriptEnd();
}

/* Sets up the terraces: the eight hanging chains (the first placed actors) and
   the rope, the trapdoor as it was left open, and once the lid is off the
   reunion as far as it has gone: the fight to watch, the fight itself, Isaac
   and Ivan fallen, or the hurt left behind once they have gone up to the
   aerie. */
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdHangingVinePlace(ACTOR_FIRST_PLACED, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 1, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 2, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 3, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 4, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 5, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 6, 1);
	fdHangingVinePlace(ACTOR_FIRST_PLACED + 7, 1);
	fdRopeInit();
	fdRopePlace(0, FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED, ACTOR_EAST_ROPE_POST,
		ACTOR_WEST_ROPE_POST);
	WhirlwindRiding = 0;
	actor = fdActorGet(ACTOR_WEST_UPPER_GUST);
	actor->motionFlg = 0;
	actor->y.fixed = 0;
	actor->groundHeight = 0;
	actor = fdActorGet(ACTOR_EAST_UPPER_GUST);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(32);
	actor->groundHeight = toFIXED(32);
	if(fdScriptGetFlag(FLAG_TRAPDOOR_OPEN)) {
		scJupiterLighthouseLeaveTrapdoorOpen();
	}
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED)) {
		scJupiterLighthouseLeaveTrapdoorOpen();
		fdActorSetPos(ACTOR_GRAY_BLOCK, 0, 0);
		if(fdScriptGetFlag(FLAG_ISAAC_AND_IVAN_FELL)) {
			if(!fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT)) {
				scJupiterLighthousePlaceForTheFight();
				scJupiterLighthousePlaceTheHurt();
			}
		} else if(!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED)) {
			if(MainRoutineStateWork.entrance ==
				JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST) {
				scJupiterLighthousePlaceForTheFight();
				fdScriptSetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED);
				scJupiterLighthousePartyArrivesAtTheFight(
					MainRoutineStateWork.entrance ==
					JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST);
			}
		} else {
			scJupiterLighthousePlaceForTheFight();
			if(MainRoutineStateWork.entrance ==
				JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST) {
				scJupiterLighthouseIsaacAndIvanAreHurt();
			} else if(MainRoutineStateWork.entrance ==
				ENTRANCE_AFTER_THE_FIGHT) {
				scJupiterLighthouseIsaacAndIvanFall();
				fdScriptSetFlag(FLAG_ISAAC_AND_IVAN_FELL);
				fdScriptClearFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED);
			} else {
				scJupiterLighthouseStartTheFight();
			}
		}
	}
	fdActorGet(ACTOR_GRAY_BLOCK)->motionFlg = 0;
	if(fdScriptGetFlag(FLAG_GRAY_BLOCK_DROPPED))
		fdActorSetPos(ACTOR_GRAY_BLOCK, 0, 0);
	if(MainRoutineStateWork.entrance == ENTRANCE_ISAAC_AND_IVAN_FALL)
		scJupiterLighthouseIsaacAndIvanFall();
	return 0;
}

void scLightSprsInitC();
void scLightSprsAdd();

/* Loads the lighthouse's palettes and the energy block's light; once the block
   is in its gap, sets it there with the lit path and the Hover pad beyond. */
Sint32 scPrepareMap(void)
{
	ACTOR *actor;

	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_LIT);
	scLightSprsInitC();
	scLightSprsAdd(ACTOR_ENERGY_BLOCK, 0);
	actor = fdActorGet(ACTOR_ENERGY_BLOCK);
	if(fdScriptGetFlag(FLAG_ENERGY_BLOCK_IN_GAP)) {
		fdActorSetPos(ACTOR_ENERGY_BLOCK, 1016, 248);
		scJupiterLighthouseLightPathToHoverPad();
	}
	actor->motionFlg = 0;
	Call4(fdMapGridSetMetatileHeight, actor->x.fixed, actor->z.fixed, 0, -6);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleTick;

/* Sets the palette cycle's tick counter. */
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

#include "shared/whirlwind_update.h"

void scWhirlwindUpdate(ACTOR *self);
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef PLACE
extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

struct METATILE_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flg, Sint32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

/* clang-format off */
Uint32 JupiterLighthouseAgatioStepsUpScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseKarstStepsUpScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseKarstTurnsScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseKarstFacesSouthScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_END,
};

Uint32 JupiterLighthouseAgatioFacesEastScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_END,
};

Uint32 JupiterLighthouseIsaacFacesWestScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_WEST),
	SC_END,
};

Uint32 JupiterLighthouseIvanFacesNorthScript[] = {
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_END,
};

Uint32 JupiterLighthouseFelixWaitsScript[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseKradenWaitsScript[] = {
	SC_SYNC,
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthousePiersStepsUpScript[] = {
	SC_SYNC,
	SC_MOVE_XZ(699, 170),
	SC_SYNC,
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseShebaStepsUpScript[] = {
	SC_SYNC,
	SC_MOVE_XZ(679, 170),
	SC_SYNC,
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 JupiterLighthouseJennaStepsUpScript[] = {
	SC_SYNC,
	SC_MOVE_XZ(659, 170),
	SC_SYNC,
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};
/* clang-format on */

extern Uint32 FieldEffectLongScript[];
extern Sint32 JupiterLighthouseShowGustScript[];

/* clang-format off */
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

LATE_DATA Sint32 JupiterLighthouseShowGustScript[] = {
	SC_CALL(scJupiterLighthouseClearScriptedActorSprFlags),
	SC_END,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseTerraces),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_TERRACES_NORTH),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_TERRACES_SOUTHWEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_TERRACES_SOUTHEAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_TERRACES_WEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_TERRACES_EAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_TERRACES),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_WHORL, JupiterLighthouseWhorl, JUPITER_LIGHTHOUSE_WHORL_FROM_JUPITER_LIGHTHOUSE_TERRACES),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_TERRACES_WEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_TERRACES_EAST),
	SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1464, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1608, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 648, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 680, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 936, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 952, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 968, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 1144, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 1064, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 200, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 1016, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (Sint32)JupiterLighthouseShowGustScript, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

LATE_DATA CUE SceneCues[] = {
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_WHORL, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_WHORL},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_WEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_TERRACES_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_EAST},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_STEP, METATILE_BEHAVIOR_WEST_WHIRLWIND_PATH, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRideUpperWhirlwind)},
	{CUE_STEP, METATILE_BEHAVIOR_EAST_WHIRLWIND_PATH, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRideWhirlwind)},
	{CUE_STEP, METATILE_BEHAVIOR_TRAPDOOR_BUTTON, FLAG_TRAPDOOR_OPEN, SCRIPT_REF(scJupiterLighthouseOpenFloor)},
	{CUE_STEP, METATILE_BEHAVIOR_OFF_TRAPDOOR_BUTTON, IF_FLAG_SET(FLAG_TRAPDOOR_OPEN), SCRIPT_REF(scJupiterLighthouseCloseFloor)},
	{CUE_TALK, ACTOR_ISAAC, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), SCRIPT_REF(scJupiterLighthouseTalkAfterTheFight)},
	{CUE_TALK, ACTOR_IVAN, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), SCRIPT_REF(scJupiterLighthouseTalkAfterTheFight)},
	{CUE_TALK, ACTOR_SHEBA, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), SCRIPT_REF(scJupiterLighthouseTalkAfterTheFight)},
	{CUE_TALK, ACTOR_JENNA, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), SCRIPT_REF(scJupiterLighthouseTalkAfterTheFight)},
	{CUE_TALK, ACTOR_KRADEN, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), SCRIPT_REF(scJupiterLighthouseTalkAfterTheFight)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseFinishMovingBlock)},
	{CUE_PUSH, -1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartMovingBlock)},
	{CUE_PUSHED, -1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseFinishMovingBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GRAY_BLOCK, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_EAST_ROPE_POST, FLAG_JUPITER_LIGHTHOUSE_ROPE_TIED, 0},
	{CUE_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, SCRIPT_REF(fdWhirlwindCarryLeader)},
	{CUE_TABLE_END},
};

/* Garet hangs from the ledge: in his hanging pose, raised 12 pixels, or */
/* after the Japanese edition set at a height of 12 pixels. */
LATE_DATA Uint32 JupiterLighthouseGaretHangsScript[] = {
#if !GARET_HANG_HEIGHT_ABSOLUTE
	SC_ANIMATION(ANIM_GARET_HANG),
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, toFIXED(12)),
#else
	SC_ANIMATION(ANIM_GARET_HANG),
	SC_SET(SW_MOTION, 0),
	SC_SET(SW_Y, toFIXED(12)),
#endif
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseMiaScript[] = {
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_ANIMATION(9),
	SC_WAIT(120),
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(120),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseIsaacFightsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_CALL(scJupiterLighthouseStopFacingMotion),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(60),
	SC_MOVE_XZ(737, 202),
	SC_ANIMATION(ANIM_WALK),
	SC_WAIT(4),
	SC_MOVE_XZ(776, 202),
	SC_CALL(scJupiterLighthousePlayRandomBlowSnd),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseAgatioFightsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_CALL(scJupiterLighthouseStopFacingMotion),
	SC_SYNC,
	SC_ANIMATION(9),
	SC_WAIT(60),
	SC_MOVE_XZ(776, 202),
	SC_ANIMATION(ANIM_WALK),
	SC_WAIT(4),
	SC_MOVE_XZ(737, 202),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseIvanFightsScript[] = {
	SC_CALL(scJupiterLighthouseStopFacingMotion),
	SC_MOVE_XZ(731, 213),
	SC_ANIMATION(ANIM_WALK),
	SC_WAIT(4),
	SC_MOVE_XZ(731, 232),
	SC_CALL(scJupiterLighthousePlayRandomBlowSnd),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(53),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseKarstFightsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_CALL(scJupiterLighthouseStopFacingMotion),
	SC_MOVE_XZ(731, 232),
	SC_WAIT(4),
	SC_MOVE_XZ(731, 213),
	SC_SYNC,
	SC_ANIMATION(8),
	SC_WAIT(53),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseFelixLooksAroundScript[] = {
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(20),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

LATE_DATA Uint32 JupiterLighthouseFelixArrivesWestScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_MOVE_XZ(744, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseFelixArrivesEastScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_BY(-8, 0, 32),
	SC_SYNC,
	SC_MOVE_XZ(896, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseKradenArrivesWestScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(740, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseKradenArrivesEastScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(900, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthousePiersArrivesWestScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_XZ(724, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthousePiersArrivesEastScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_XZ(916, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseJennaArrivesWestScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_XZ(677, 287),
	SC_SYNC,
	SC_ANIMATION(5),
	SC_MOVE_XZ(740, 344),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseJennaArrivesEastScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_XZ(950, 287),
	SC_SYNC,
	SC_ANIMATION(5),
	SC_MOVE_XZ(900, 344),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseShebaArrivesWestScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_XZ(708, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseShebaArrivesEastScript[] = {
	SC_ANIMATION(5),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_ANIMATION(5),
	SC_MOVE_XZ(924, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

/* Each walks to where Felix stood when the script started. */
LATE_DATA Uint32 JupiterLighthouseGatherAtFelixScript[] = {
	SC_SYNC,
	SC_RELEASE,
};

LATE_DATA Uint32 JupiterLighthouseWalkToAerieScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(706, 172),
	SC_SYNC,
	SC_MOVE_XZ(682, 151),
	SC_SYNC,
	SC_MOVE_XZ(682, 120),
	SC_SYNC,
	SC_MOVE_XZ(664, 106),
	SC_SYNC,
	SC_MOVE_XZ(664, 100),
	SC_SYNC,
	SC_END,
};

LATE_DATA Uint32 JupiterLighthouseLeaveForAerieScript[] = {
	SC_MOVE_XZ(664, 80),
	SC_SYNC,
	SC_RELEASE,
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
