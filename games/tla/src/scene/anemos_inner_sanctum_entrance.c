/* The Anemos Inner Sanctum's hall under Contigo, which the party reaches by
   Teleport from the circle in the middle of the town, and in the same map the
   Sanctum's last room (SceneId_AnemosInnerSanctumEntrance).

   In the last room six grey stone blocks, each with a glowing symbol on top,
   stand around a dark square. Felix pushes them or moves them with Move until
   all six stand on the square; their symbols light up, and Hover cast standing
   on them floats the party north across the gap to the shadow who keeps the
   light. He challenges them, and the battle with the Dullahan begins; once he
   is beaten, his light rises in his place. */
#include "../field/actor_set_sprite_priority.c"
#include "field/actor_script.h"
#include "field/field_mode.h"

#include "../field/actor_create_silhouette.c"
#include "graphics/palette_buffers.h"

/* Set once the Dullahan, the shadow keeping the light, has been beaten; he is
   gone. */
#define FLAG_DULLAHAN_BEATEN 0xa9f

/* Set once the six blocks stand on the dark square: their symbols are lit, for
   the rest of the visit. */
#define FLAG_SYMBOLS_LIT	 FLAG_SCENE_TEMP_00
/* Set as the battle with the Dullahan starts, for the light to rise when the
   party returns. */
#define FLAG_DULLAHAN_FOUGHT FLAG_SCENE_TEMP_01

/* The last room's metatile behaviours: its floor, where walking into a block
   pushes it, and the metatiles where Hover, cast once the six blocks stand in
   their places, floats the party across the gap. */
enum {
	METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_BLOCK_FLOOR = 33,
	METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER = 77
};

/* A glow's value with this bit drawn mirrored left to right; its low three bits
   are its frame. */
#define LIGHT_SPRITE_MIRRORED (1 << 7)

/* Where a glow joins the depth list: its actor's y + z plus this, where the
   actor's own sprite goes at + 62. */
enum { GLOW_DEPTH = 58 };

void scAnemosInnerSanctumEntrancePlaceBlocks(void);
void scAnemosInnerSanctumEntranceSetLightPicture(Uint16 picture);
#include "field/script_inline.h"
#include "scene/scene.h"
#include "game/game.h"
#include "edition.h"
#include "macros.h"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

/* The hall's and the last room's actors
   (AnemosInnerSanctumEntrance_Placements). */
typedef enum ANEMOS_INNER_SANCTUM_ENTRANCE_ACTOR_ID {
	/* "I am the shadow, the keeper of light." */
	ACTOR_SHADOW = ACTOR_FIRST_PLACED,
	/* The six grey stone blocks, each with a glowing symbol on top, that Move
	   sets in place before the way opens. */
	ACTOR_SYMBOL_BLOCK_A,
	ACTOR_SYMBOL_BLOCK_B,
	ACTOR_SYMBOL_BLOCK_C,
	ACTOR_SYMBOL_BLOCK_D,
	ACTOR_SYMBOL_BLOCK_E,
	ACTOR_SYMBOL_BLOCK_F,
	/* The light the shadow keeps, a blue orb: once the Dullahan is beaten it
	   flashes white, swells over him and rises. */
	ACTOR_LIGHT,
	/* The fourteen coiled dragon statues, drawn without shadows by their
	   script. */
	ACTOR_FIRST_DRAGON_STATUE
} ANEMOS_INNER_SANCTUM_ENTRANCE_ACTOR_ID;

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern Sint32 AnemosInnerSanctumEntrance_LightGrowsScript[];
extern Sint32 AnemosInnerSanctumEntrance_LightShrinksScript[];
extern Sint32 AnemosInnerSanctumEntrance_ShadowScript[];
extern Sint8  AnemosInnerSanctumEntrance_BlockOrder[];
void		  scAnemosInnerSanctumEntranceStartLightSprs(void);
void		  scAnemosInnerSanctumEntranceSetLightPriority(Uint16 priority);
void		  scAnemosInnerSanctumEntranceOpenTheWay(void);

extern Sint32 AnemosInnerSanctumEntrance_DragonStatueScript[];

struct ACTOR;

Sint16 LightSpriteTile	BSS_DATA;
Sint16 LightSpriteCount BSS_DATA;
/* The actors that carry a glow, LightSpriteCount of them. */
ACTOR *LightSpriteActors[16] BSS_DATA;
/* The glows' sprite attributes, built each frame by scLightSprsDraw. */
LIGHT_OAM									   LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge					   BSS_DATA;
Sint16 LightSpritePriority					   BSS_DATA;
Sint16 AnemosInnerSanctumEntrance_LightPicture BSS_DATA;

#include "text/msg_ids.h"

/* The call AnemosInnerSanctumEntrance_DragonStatueScript makes: the actor's
   shadow is no longer drawn. */
Sint32 scAnemosInnerSanctumEntranceRemoveShadow(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
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

extern LATE_DATA ACTOR_PLACE AnemosInnerSanctumEntrance_EmptyPlacements[];
extern ACTOR_PLACE			 AnemosInnerSanctumEntrance_Placements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == (Sint32)&SceneId_AnemosInnerSanctumEntrance)
		return AnemosInnerSanctumEntrance_Placements;
	return AnemosInnerSanctumEntrance_EmptyPlacements;
}

/* As the blocks set the party down before him, the shadow at the door speaks,
   the room's colours warm, and the battle with the Dullahan starts, one the
   party cannot run from. */
void scAnemosInnerSanctumEntranceShadowChallenges(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStartRepeatedMotion(ACTOR_SHADOW, 2);
	fdScriptWait(20);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeSetTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeStartInterpolation(60);
	fdScriptWait(60);
	/* "I am the shadow, the keeper of light. If you want the sun's power, show
	   me your own." */
	mnFieldMessageShowCentered((Sint32)&MsgIAmTheShadow, 0, 6);
	MainRoutineStateWork.pendingDjinni = IF_FLAG_SET(FLAG_DULLAHAN_BEATEN);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_HIDDEN_BOSSES, HIDDEN_BOSS_ANEMOS);
	syGameFlagSet(FLAG_DULLAHAN_FOUGHT);
}

/* Back from beating the Dullahan, the shadow and his light flash white, and the
   light swells and rises over him while he shrinks away. */
void scAnemosInnerSanctumEntranceLightRises(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_LIGHT);

	actor->motionFlg = 0;
	actor->scaleX = toFIXED(1) / 4;
	actor->scaleY = toFIXED(1) / 4;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	sySndPlay(SE_LIGHT_APPEAR);
	fdActorSetSprCol(fdActorGetById(ACTOR_SHADOW), SPRITE_COLOR_WHITE);
	fdActorSetSprCol(fdActorGetById(ACTOR_LIGHT), SPRITE_COLOR_WHITE);
	fdScriptWait(20);
	sySndPlay(SE_WARP);
	fdActorPlaceOnActor(ACTOR_LIGHT, ACTOR_SHADOW);
	sySynch(1);
	fdActorSetMovementTypeById(
		ACTOR_SHADOW, AnemosInnerSanctumEntrance_ShadowScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_LIGHT, AnemosInnerSanctumEntrance_LightGrowsScript);
	fdActorSetMovementTypeAndWaitById(
		ACTOR_LIGHT, AnemosInnerSanctumEntrance_LightShrinksScript);
	fdScriptWait(10);
	fdScriptEnd();
}

/* Nothing visible: leaves the six blocks standing where they are, no longer
   pushed. */
void scAnemosInnerSanctumEntrancePlaceBlocks(void)
{
	Sint32 i;

	for(i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
		ACTOR *actor = fdActorGetById(i);

		actor->sceneCnt = 1;
		actor->update = 0;
	}
}

/* A floating block's shadow shows on the ground while the ground is near under
   it, and not over the gap. */
void scAnemosInnerSanctumEntranceShadowByHeight(ACTOR *actor)
{
	if((actor->groundHeight = fdMapGetTerrainHeight(
			0, actor->x.fixed, actor->z.fixed)) >= -toFIXED(32))
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	else
		fdActorSetSprFlags(actor, 0);
}

extern Sint8 AnemosInnerSanctumEntrance_BlockSpots[];

/* Felix moves one of the six symbol blocks in the last room, with Move or by
   pushing it. Once all six stand together on the dark square in the middle of
   the room (metatiles (11, 27) to (13, 28)), their symbols light up and flicker
   with a rising sound, and the blocks stay put for the rest of the visit.

   Each block has its own spot, in AnemosInnerSanctumEntrance_BlockSpots. */
void scAnemosInnerSanctumEntranceMoveBlock(Sint32 unused, Sint32 block)
{
	Sint32 i;

	const Sint8 *x = AnemosInnerSanctumEntrance_BlockSpots;
	const Sint8 *z = AnemosInnerSanctumEntrance_BlockSpots + 1;

	for(i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
		ACTOR *actor = fdActorGetById(i);

		if(FIELD_METATILE(actor->x.fixed) != *x)
			return;
		{
			Sint32 metatileZ = *z;

			z += 2;
			x += 2;
			if(FIELD_METATILE(actor->z.fixed) != metatileZ)
				return;
		}
	}
	syGameFlagSet(FLAG_SYMBOLS_LIT);
	sySndPlay(SE_ASCEND);
	scAnemosInnerSanctumEntranceSetLightPicture(1);
	scAnemosInnerSanctumEntrancePlaceBlocks();
}

/* Felix walks into one of the six symbol blocks on the last room's floor and
   pushes it a metatile ahead, as scAnemosInnerSanctumEntranceMoveBlock. With no
   block to push, he jumps two metatiles ahead where the ground allows
   (fdTryJumpForward), but only while he stands less than 28 pixels below the
   room's ground: never on the sunken floor itself, which lies 32 pixels down
   (metatiles (7, 25) to (17, 30)); only up on the blocks, which he walks on as
   platforms, or on the step at metatile (12, 25) and the landing at metatile
   (12, 9) north of the gap, each 16 pixels down. */
void scAnemosInnerSanctumEntranceStepWhilePushing(Sint32 behavior)
{
	MAIN_ROUTINE_STATE *party;
	Sint32				held = fdLeaderCanPushObjectAhead();

	party = &MainRoutineStateWork;
	if(held) {
		if(fdFindNearbyTalkTarget(party->leaderActorId) != -1) {
			fdLeaderPushObjectAhead();
			scAnemosInnerSanctumEntranceMoveBlock(0, behavior);
		}
	} else {
		if(fdActorGetById(party->leaderActorId)->y.fixed > -toFIXED(28))
			fdTryJumpForward();
	}
}

/* Felix casts Hover standing on the six blocks once their symbols are lit: one
   after another they hop up, then float the party 19 metatiles north across the
   gap (from metatile (12, 28) to (12, 9)) and settle with a thump. If the
   Dullahan is not yet beaten, the shadow then challenges the party
   (scAnemosInnerSanctumEntranceShadowChallenges). Cast again on the far side,
   it floats them back south. */
void scAnemosInnerSanctumEntranceOpenTheWay(void)
{
	ACTOR *actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
	Sint32 shift = toFIXED(304);
	Sint32 i;

	if(FIELD_METATILE(actor->z.fixed) <= 9)
		shift = -shift;
	fdScriptEndLastingPsynergy(0);
	if(!syGameFlagGet(FLAG_SYMBOLS_LIT))
		return;
	if(fdMapGridGetMetatileBehaviorAt(
		   actor->layer, actor->x.fixed, actor->z.fixed) !=
		METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER)
		return;
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
#if ANEMOS_HOVER_CENTRES_LEADER
	scFieldActorSetMoveTargetFX(actor,
		(actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8), actor->y.fixed,
		(actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
#endif
	{
		Sint32 id = MainRoutineStateWork.leaderActorId;

		if(shift > 0)
			fdActorFaceDirectionById(id, FACING_NORTH, 0);
		else
			fdActorFaceDirectionById(id, FACING_SOUTH, 0);
	}
	sySynch(10);
	i = 0;
	/* BUG: the loop runs seven times, reading one byte past the six blocks of
	   AnemosInnerSanctumEntrance_BlockOrder; BUGFIX stops at the sixth. */
	do {
		actor = fdActorGetById(AnemosInnerSanctumEntrance_BlockOrder[i]);
		actor->motionFlg = 0;
		actor->y.fixed += toFIXED(1) / 2;
		actor->acceleration = toFIXED(1) / 16;
		actor->speedLimit = toFIXED(2);
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
		fdActorSetMoveTarget(
			actor, actor->x.fixed, -toFIXED(8), actor->z.fixed);
		actor->update = scAnemosInnerSanctumEntranceShadowByHeight;
		sySndPlay(SE_JUMP);
		sySynch((syRandomBelow(4)) + 3);
#if FIX_ANEMOS_BLOCK_HOP_OVERRUN
	} while(++i < 6);
#else
	} while(++i <= 6);
#endif
	sySynch(40);
	sySndPlay(SE_KOLIMA_BRIDGE_POST);
	for(i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
		actor = fdActorGetById(i);
		{
			FIXED z = actor->z.fixed - shift;

			fdActorSetMoveTarget(actor, actor->x.fixed, -toFIXED(8), z);
		}
	}
	fdActorWaitForMove(fdActorGetById(ACTOR_SYMBOL_BLOCK_A));
	sySynch(40);
	for(i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
		actor = fdActorGetById(i);
		actor->z.fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->update = 0;
	}
	sySynch(3);
	sySndPlay(SE_THUMP);
	sySynch(10);
	if(!syGameFlagGet(FLAG_DULLAHAN_BEATEN) && shift > 0)
		scAnemosInnerSanctumEntranceShadowChallenges();
}

/* Unused: does nothing. */
void scAnemosInnerSanctumEntranceNoOperation(void)
{
}

extern CUE AnemosInnerSanctumEntrance_EmptyCues[];
extern CUE AnemosInnerSanctumEntrance_Cues[];

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == (Sint32)&SceneId_AnemosInnerSanctumEntrance)
		return AnemosInnerSanctumEntrance_Cues;
	return AnemosInnerSanctumEntrance_EmptyCues;
}

/* Opens with the window transition. In the last room, gives the six blocks
   their symbols, lets the leader stand on them, and once they are placed this
   visit leaves them placed and flickering. Back from the Dullahan's battle, the
   light rises. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next ==
		(Sint32)&SceneId_AnemosInnerSanctumEntrance) {
		scAnemosInnerSanctumEntranceStartLightSprs();
		scAnemosInnerSanctumEntranceSetLightPriority(2);
		/* scLightSprsAdd, defined below, is called with no prototype in
		   scope. */
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_A, 0);
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_B, 1);
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_C, 2);
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_D, 3);
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_E, LIGHT_SPRITE_MIRRORED | 0);
		scLightSprsAdd(ACTOR_SYMBOL_BLOCK_F, LIGHT_SPRITE_MIRRORED | 1);
		fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_A)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS | ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_B)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_C)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_D)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_E)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorGetById(ACTOR_SYMBOL_BLOCK_F)->priFlg |= ACTOR_PRIORITY_PLATFORM;
		if(syGameFlagGet(FLAG_SYMBOLS_LIT)) {
			scAnemosInnerSanctumEntranceSetLightPicture(1);
			scAnemosInnerSanctumEntrancePlaceBlocks();
		}
	}
	if(fdScriptGetFlag(FLAG_DULLAHAN_FOUGHT)) {
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		scAnemosInnerSanctumEntranceLightRises();
		syGameFlagClear(FLAG_DULLAHAN_FOUGHT);
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Draws each listed actor's glow: a small 16-pixel glow on its frame, or, in
   the large mode, a 32 by 64 one. */
void scLightSprsDraw(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Sint32	   viewX, viewZ, picture, tile, viewY;
	LIGHT_OAM *oam;
	Sint32	   i;

	viewX = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
	viewZ = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
	tile = VramCacheEntries[LightSpriteTile].ofs >> 5;
	viewY = map->origin[1];
	oam = LightSpriteOams;
	picture = AnemosInnerSanctumEntrance_LightPicture *
		((FrameCount >> 2) % 5 * 32 + 32);

	for(i = 0; i < LightSpriteCount; i++) {
		ACTOR *actor = LightSpriteActors[i];

		if(actor != 0 && actor->x.fixed != 0) {
			Sint32	screenX = actor->x.fixed - viewX;
			FIXED	y = actor->y.fixed - viewY + toFIXED(8);
			FIXED	z = actor->z.fixed - viewZ - viewY;
			Sint32	screenY = z - y;
			Sint32	depth = FIXEDtoINT(y + z) + GLOW_DEPTH;
			Sint32	mode = LightSpritesLarge;
			Uint32 *word;

			if(mode == 0) {
				Sint32 frame = actor->tmr;
				Sint32 pixelX = FIXEDtoINT(screenX);
				Sint32 pixelY = FIXEDtoINT(screenY);
				screenX = pixelX - 8;
				screenY = pixelY - 16;
				if((Uint32)(pixelX + 7) > 334 || screenY <= -16 ||
					screenY > 239)
					continue;
				screenX &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				word = (Uint32 *)oam;
				*word++ = 0;
				*word++ = screenY | OBJ_X(screenX) |
					(frame & LIGHT_SPRITE_MIRRORED) *
						(OBJ_HFLIP / LIGHT_SPRITE_MIRRORED) |
					OBJ_SIZE_16X16 | OBJ_256_COLORS;
				*word = (tile + picture + (frame & 7) * 8) | OBJ_PRIORITY(2);
				if(LightSpritePriority == -1)
					oam->pri = fdActorGetSprPriority(actor);
				else
					oam->pri = LightSpritePriority;
				syOAMAddSprAtDepthClamped(oam, depth);
				oam++;
			} else {
				Sint32 frame = actor->tmr;
				Sint32 pixelX = FIXEDtoINT(screenX);
				Sint32 pixelY = FIXEDtoINT(screenY);
				screenX = pixelX - 8;
				screenY = pixelY - 64;
				if((Uint32)(pixelX + 23) > 350 || screenY <= -64 ||
					screenY > 175)
					continue;
				screenX &= OBJ_X_MASK;
				screenY &= OBJ_Y_MASK;
				word = (Uint32 *)oam;
				*word++ = 0;
				*word++ =
					screenY | OBJ_X(screenX) | OBJ_SIZE_32X64 | OBJ_256_COLORS;
				*word = (tile + frame * 8) | OBJ_PRIORITY(2);
				if(LightSpritePriority == -1)
					oam->pri = fdActorGetSprPriority(actor);
				else
					oam->pri = LightSpritePriority;
				syOAMAddSprAtDepthClamped(oam, depth);
				oam++;
			}
		}
	}
}

#include "shared/light_sprites_init_a.h"

/* Sets up the glow sprites from the field engine's cached tiles D, with no
   picture row chosen yet. */
void scAnemosInnerSanctumEntranceStartLightSprs(void)
{
	Uint8 *buffer = syHeapCallAlternatePool(syHeapAlloc, 6144);

	syWordsClearEntry(LightSpriteActors, sizeof(LightSpriteActors));
	syDecompressTaggedLZ(Field_CachedTilesD, buffer);
	LightSpriteTile = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(LightSpriteTile, 6144, buffer);
	syHeapRelease(buffer);
	sySetEvent(scLightSprsDraw, EVENT_PASS_FRAME);
	LightSpriteCount = 0;
	LightSpritesLarge = 0;
	LightSpritePriority = -1;
	AnemosInnerSanctumEntrance_LightPicture = 0;
}

/* Gives actor `id` a glow: `glow` is its frame, with LIGHT_SPRITE_MIRRORED to
   draw it mirrored. */
void scLightSprsAdd(Sint32 id, Sint32 glow)
{
	ACTOR *actor = fdActorGetById(id);

	if(actor != NULL) {
		actor->tmr = glow;
		LightSpriteActors[LightSpriteCount] = actor;
		LightSpriteCount++;
	}
}

/* Sets the glow sprites' priority; -1 takes each actor's own. */
void scAnemosInnerSanctumEntranceSetLightPriority(Uint16 priority)
{
	LightSpritePriority = priority;
}

/* Chooses the glows' picture: 0 the still glow, 1 the five-frame flicker. */
void scAnemosInnerSanctumEntranceSetLightPicture(Uint16 picture)
{
	AnemosInnerSanctumEntrance_LightPicture = picture;
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The light's rise: it grows and climbs for 40 frames. */
Sint32 AnemosInnerSanctumEntrance_LightGrowsScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
	SC_ADD(SW_Y, -toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(40, SC_TOP),
	SC_END,
};

/* The light shrinking back as it sinks, then gone. */
Sint32 AnemosInnerSanctumEntrance_LightShrinksScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_ADD(SW_Y, toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(24, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

/* The shadow shrinking away for 40 frames, then gone. */
Sint32 AnemosInnerSanctumEntrance_ShadowScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(40, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};
/* clang-format on */

/* The metatiles, x then z, where the six blocks must stand before Hover opens
   the way: the dark square in the middle of the room. */
Sint8 AnemosInnerSanctumEntrance_BlockSpots[] = {
	11, 27, 11, 28, 12, 27, 12, 28, 13, 27, 13, 28};

/* The order the blocks hop up in when the way opens. */
Sint8 AnemosInnerSanctumEntrance_BlockOrder[] = {
	ACTOR_SYMBOL_BLOCK_D,
	ACTOR_SYMBOL_BLOCK_F,
	ACTOR_SYMBOL_BLOCK_A,
	ACTOR_SYMBOL_BLOCK_C,
	ACTOR_SYMBOL_BLOCK_E,
	ACTOR_SYMBOL_BLOCK_B,
};

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

LATE_DATA const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The coiled dragon statues' script, from ACTOR_FIRST_DRAGON_STATUE on: hides
   each one's sprite, then stops. */
LATE_DATA Sint32 AnemosInnerSanctumEntrance_DragonStatueScript[] = {
	SC_CALL(scAnemosInnerSanctumEntranceRemoveShadow),
	SC_END,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(AnemosInnerSanctumEntrance),
	SCENE_EXIT_UNLESS(
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTHEAST,
		ContigoGeoglyph,
		CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST,
		AnemosInnerSanctumDragonSkinChest,
		ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTH,
		ContigoGeoglyph,
		CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_SOUTH,
		ContigoGeoglyph,
		CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH,
		EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST,
		AnemosInnerSanctumOrihalconChest,
		ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE,
		EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* The scene's other room's actors: none. */
LATE_DATA ACTOR_PLACE AnemosInnerSanctumEntrance_EmptyPlacements[] = {
	{SCENE_TABLE_END},
};

/* The hall's and the last room's actors, in
   ANEMOS_INNER_SANCTUM_ENTRANCE_ACTOR_ID's order. */
LATE_DATA ACTOR_PLACE AnemosInnerSanctumEntrance_Placements[] = {
	ACTOR_PLACEMENT(SPRITE_DULLAHAN, FLAG_DULLAHAN_BEATEN, MOVEMENT_TYPE_STAND,
		200, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 424, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 456, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 472, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 424, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 456, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 472, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 152, 0, 168,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 152, 0, 216,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 248, 0, 168,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 248, 0, 216,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 696, 0, 240,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 744, 0, 240,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 424, 0, 232,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 424, 0, 288,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 504, 0, 232,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 504, 0, 288,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 424, 0, 712,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 424, 0, 768,
		FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 504, 0, 712,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
		(Sint32)AnemosInnerSanctumEntrance_DragonStatueScript, 504, 0, 768,
		FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

/* The scene's other room's cues: none. */
LATE_DATA CUE AnemosInnerSanctumEntrance_EmptyCues[] = {
	{CUE_TABLE_END},
};

/* The scene's exits, the last room's floor for pushing, Move on each block and
   Hover on the blocks' metatiles. */
LATE_DATA CUE AnemosInnerSanctumEntrance_Cues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTHEAST},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST},
	{CUE_EXIT, 3, CONDITION_ALWAYS,
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTH},
	{CUE_EXIT, 4, CONDITION_ALWAYS,
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_SOUTH},
	{CUE_EXIT, 5, CONDITION_ALWAYS,
		ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST},
	{STEP_CUE_WALKING,
		METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_BLOCK_FLOOR,
		CONDITION_ALWAYS,
		SCRIPT_REF(scAnemosInnerSanctumEntranceStepWhilePushing)},
	{STEP_CUE_WALKING, METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
		CONDITION_ALWAYS,
		SCRIPT_REF(scAnemosInnerSanctumEntranceStepWhilePushing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_A, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_B, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_C, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_D, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_E, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_F, FLAG_SYMBOLS_LIT,
		SCRIPT_REF(scAnemosInnerSanctumEntranceMoveBlock)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST),
		METATILE_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
		IF_FLAG_SET(FLAG_SYMBOLS_LIT),
		SCRIPT_REF(scAnemosInnerSanctumEntranceOpenTheWay)},
	{CUE_TABLE_END},
};
