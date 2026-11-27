/* Jupiter Lighthouse's aerie and its beacon: Karst and Agatio wait
   (scJupiterLighthouseKarstAndAgatioWait), the party casts the Jupiter Star
   into the well (scJupiterLighthouseCastTheJupiterStar) and the beacon is lit
   (scJupiterLighthouseBeaconIsLit); Karst and Agatio are fought three times,
   Jenna and Sheba coming to help (scJupiterLighthouseJennaJoinsTheFight,
   scJupiterLighthouseShebaJoinsTheFight), and Alex takes them down on the
   elevator whether the party wins (scJupiterLighthouseWeBeatThem) or loses
   (scJupiterLighthouseKarstAndAgatioRetreat). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "item_ids.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/glyph.h"
#include "system/field_effect.h"
#include "edition.h"
#include "field/call.h"
#include "scene/scene.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "window/window.h"
#include "field/actor_script.h"

/* The placed actors this file moves by number, besides the angels, the archers
   and the lid (shared/jupiter_lighthouse_aerie.h). The party's rows take their
   characters' numbers; the rest count on from 8. */
enum {
	/* The chains hanging down the wall below the well (fdHangingVinePlace). */
	ACTOR_WEST_CHAIN = 15,
	ACTOR_EAST_CHAIN,
	/* The six platforms floating over the shafts either side of the well, west
	   to east. */
	ACTOR_FLOATING_PLATFORM_A = 17,
	ACTOR_FLOATING_PLATFORM_B = 18,
	ACTOR_FLOATING_PLATFORM_C = 19,
	ACTOR_FLOATING_PLATFORM_D = 20,
	ACTOR_FLOATING_PLATFORM_E = 21,
	ACTOR_FLOATING_PLATFORM_F = 22,
	ACTOR_AGATIO = 27,
	ACTOR_KARST = 28,
	ACTOR_ALEX = 29,
	ACTOR_KRADEN = 30,
	/* The bottom and top of the column of light that rises out of the well once
	   the Jupiter Star is cast in (scJupiterLighthouseCastTheJupiterStar). */
	ACTOR_BEAM_BOTTOM = 31,
	ACTOR_BEAM_TOP = 32,
	/* The lit beacon, a great swirling orb over the well. */
	ACTOR_BEACON_ORB = 36,
	/* The Jupiter Star's shadow: an unseen marker whose shadow alone shows, at
	   an eighth of its size, following the star from Felix to the well. */
	ACTOR_STAR_SHADOW = 37
};

/* The poses of the battles with Karst and Agatio, by each sheet: facing off,
   weapons ready (Felix, Piers and Jenna seen from behind), and lying beaten on
   the ground. Jenna and Sheba fall as ANIM_COLLAPSE. */
enum {
	ANIM_AGATIO_BATTLE_STANCE = 9,
	ANIM_AGATIO_LIE_BEATEN = 10,
	ANIM_KARST_BATTLE_STANCE = 8,
	ANIM_KARST_LIE_BEATEN = 9,
	ANIM_LEADER_BATTLE_STANCE = 50,
	ANIM_PIERS_BATTLE_STANCE = 12,
	ANIM_PIERS_LIE_BEATEN = 13,
	ANIM_JENNA_BATTLE_STANCE = 27
};

/* Set once Karst and Agatio have met Felix on his way up to the well, so that
   they do not stop him again. */
#define FLAG_KARST_AND_AGATIO_MET_FELIX	 FLAG_SCENE_TEMP_00
/* Sends up the column of light's particles every tick rather than every third
   (scJupiterLighthouseSpawnBeaconParticle); nothing here sets it. */
#define FLAG_BEACON_PARTICLES_EVERY_TICK FLAG_SCENE_TEMP_01
/* Set once the unused scJupiterLighthouseRaiseBeaconLight has finished. */
#define FLAG_BEACON_LIGHT_RISEN			 FLAG_SCENE_TEMP_02

/* The metatiles on the way up to the well where Karst and Agatio meet Felix,
   and the row before the well where he can cast the Jupiter Star. */
enum { METATILE_BEFORE_KARST_AND_AGATIO = 100, METATILE_BEFORE_THE_WELL };

/* The entrances the beacon's scripts arrive by: back from the World Map's view
   of the lit beacon, and after each battle with Karst and Agatio. */
enum {
	ENTRANCE_BEACON_LIT = 9,
	ENTRANCE_AFTER_FIRST_BATTLE,
	ENTRANCE_AFTER_SECOND_BATTLE,
	ENTRANCE_LAST_BATTLE_WON,
	ENTRANCE_LAST_BATTLE_LOST
};

/* An actor-script call: plays the jump sound and drops the actor's shadow as it
   hops onto the next floating platform. */
Sint32 scJupiterLighthouseJumpWithoutShadow(ACTOR *actor)
{
	fdScriptPlaySnd(SE_JUMP);
	fdActorSetSprFlags(actor, 0);
	return 0;
}

/* An actor-script call: gives the actor its shadow back once it has landed. */
Sint32 scJupiterLighthouseShowShadow(ACTOR *actor)
{
	fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

#include "../field/actor_stop_where_it_stands.c"

/* Turns a particle round the column of light's bottom (ACTOR_BEAM_BOTTOM) by
   its timer, a thirty-second of a turn a tick, as far across as its speed and
   two pixels deep. */
void scJupiterLighthouseCircleBeacon(ACTOR *self)
{
	ACTOR *centre = fdActorGet(ACTOR_BEAM_BOTTOM);
	Uint16 ang = self->tmr;
	FIXED  sine;
	FIXED  x;
	FIXED  z;

	self->x.fixed = centre->x.fixed + syCos(ang) * (self->speedLimit + 3);
	sine = syMathSin(ang);
	z = centre->z.fixed;
	x = self->x.fixed;
	z += sine * 2;
	self->z.fixed = z;
	self->tgtX = x;
	self->tgtZ = z;
	self->tmr -= DEGtoANG(45) / 4;
}

/* Turns a spark round the beacon's rising light (ACTOR_BEACON_ORB), wider
   across than deep, its radius from its speed and its sceneCnt. */
void scJupiterLighthouseCircleBeaconWide(ACTOR *self)
{
	ACTOR *centre = fdActorGet(ACTOR_BEACON_ORB);
	Uint16 ang = self->tmr;
	FIXED  cosine = syCos(ang);
	Sint32 radius = self->speedLimit + self->sceneCnt;
	self->x.fixed = centre->x.fixed + cosine * (radius + 6);
	self->z.fixed = centre->z.fixed + syMathSin(ang) * (self->sceneCnt + 4);
	self->tgtX = self->x.fixed;
	self->tgtZ = self->z.fixed;
	self->tmr -= DEGtoANG(45) / 4;
}

#include "../field/actor_step_angle.c"

#include "text/msg_ids.h"

struct METATILE_TARGET;
struct METATILE_TARGET_ENTRY;

void scJupiterLighthouseKarstAndAgatioWait(void);

void fdActorStopWhereItStands(ACTOR *actor);

Sint32 JupiterLighthouseElevatorLevel BSS_DATA;
/* Unused: nothing reads these bytes. */
Uint8 JupiterLighthouseUnusedAfterElevatorLevel[4] BSS_DATA;
Sint32 JupiterLighthouseBeaconRiseTick BSS_DATA;
Sint32 WhirlwindRiding				   BSS_DATA;
Uint16								   PaletteCycleColors[16] BSS_DATA;
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

extern Sint32 JupiterLighthouseBeaconRisesScript[];
extern Sint32 JupiterLighthouseBeaconSparkScript[];

/* Whom scJupiterLighthouseLowerElevator carries down, and where the camera
   goes: Alex, Agatio and Karst with the camera moving south after them; Isaac,
   the camera after him; Felix, carried all the way down; and Alex, Agatio and
   Karst with the camera following Isaac. */
typedef enum ELEVATOR_DESCENT {
	DESCENT_ALEX_AGATIO_KARST,
	DESCENT_ISAAC,
	DESCENT_FELIX,
	DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC
} ELEVATOR_DESCENT;

/* How scJupiterLighthouseRaiseElevator brings the elevator back: put back at
   once, or rising with the camera moving after it. */
typedef enum ELEVATOR_ASCENT { ASCENT_AT_ONCE, ASCENT_RISING } ELEVATOR_ASCENT;

void scJupiterLighthouseLowerElevator(Sint32 mode);
void scJupiterLighthouseRaiseElevator(Sint32 mode);
void scJupiterLighthousePlaceRaisedActors(void);

/* The ROM's callers here are compiled against this form, with no parameter list
   and an Sint32 result where the definition takes three Sint32 values and
   returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior();

/* Unused: one tick of the beacon's rise as The Broken Seal's Venus Lighthouse
   aerie plays it, counted by JupiterLighthouseBeaconRiseTick: the tower
   shakes and flashes, the orb (ACTOR_BEACON_ORB) climbs from below the map
   shedding sparks, and the screen fades to white; FLAG_BEACON_LIGHT_RISEN is
   set once it is done. No script here starts it. */
UNUSED void scJupiterLighthouseRaiseBeaconLight(void)
{
	ACTOR	  *center;
	ACTOR	  *effect;
	FIELD_SPR *spr;
	Sint32	   spawnParticles;
	Uint32	   randomHeight;
	Sint32	   ang;

	center = fdActorGetById(ACTOR_BEACON_ORB);
	spawnParticles = 0;
	switch(JupiterLighthouseBeaconRiseTick) {
	case 0:
		fdScriptPlaySnd(SE_SHIMMER);
		fdMapGridSetShake(6, 6, 1);
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_CUSTOM | RGB(31, 31, 24), PALETTES_BG);
		fdPaletteFadeInterpolate(8);
		break;
	case 8:
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeInterpolate(8);
		break;
	case 16:
		fdMapGridSetShake(1, 1, 1);
		break;
	case 24:
		center->x.fixed = toFIXED(304);
		center->y.fixed = -toFIXED(360);
		center->z.fixed = toFIXED(164);
		center->scaleX = toFIXED(1);
		center->scaleY = toFIXED(1);
		fdActorStopWhereItStands(center);
		fdActorSetScript(ACTOR_BEACON_ORB, JupiterLighthouseBeaconRisesScript);
		break;
	case 25:
		JupiterLighthouseBeaconRiseTick--;
		if(center->y.fixed > 0) {
			fdPaletteFadeApplyTarget(
				PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
			fdPaletteFadeInterpolate(16);
			JupiterLighthouseBeaconRiseTick++;
		} else {
			if((TickCount & 7) == 0)
				fdScriptPlaySnd(SE_GLITTER);
			spawnParticles = 1;
			center->y.fixed += toFIXED(9) / 4;
		}
		break;
	case 26:
		JupiterLighthouseBeaconRiseTick--;
		if(center->y.fixed > toFIXED(40)) {
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeInterpolate(40);
			JupiterLighthouseBeaconRiseTick++;
		} else {
			if((TickCount & 7) == 0)
				fdScriptPlaySnd(SE_GLITTER);
			spawnParticles = 1;
			center->y.fixed += toFIXED(9) / 4;
		}
		break;
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
		spawnParticles = 1;
		break;
	case 36:
		fdScriptPlaySnd(SE_FALLING_LIGHT);
		fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
		fdPaletteFadeInterpolate(12);
		break;
	case 48:
		scFieldActorStop(ACTOR_BEACON_ORB);
		fdScriptSetFlag(FLAG_BEACON_LIGHT_RISEN);
		break;
	}
	if(spawnParticles) {
		randomHeight = toFIXED(syRandomBelow(80));
		effect = scFieldActorCreate(SPRITE_PARTICLE, center->x.fixed,
			center->y.fixed - randomHeight + -toFIXED(8), center->z.fixed);
		if(effect != 0) {
			spr = effect->spr;
			fdActorStartScript(effect, JupiterLighthouseBeaconSparkScript);
			scFieldActorSetSprCol(effect, SPRITE_COLOR_GRB);
			effect->motionFlg = 0;
			ang = syRandom() & 0xffff000;
			effect->tmr = ang;
			effect->userValue = 0;
			{
				Uint32 counter = syRandom() >> 13;

				effect->sceneCnt = counter;
			}
			effect->update = scJupiterLighthouseCircleBeaconWide;
			effect->speedLimit =
				syMathSin((Uint32)(syRandom() * 65535) >> 20) * 24;
			effect->speedLimit = FIXEDtoINT(center->speedLimit);
			spr->flg = 0;
			spr->pri = 1;
		}
	}
	JupiterLighthouseBeaconRiseTick++;
}

/* Walking up towards the well in the aerie's centre (metatiles (33, 21) and
   (34, 21)) once Isaac and Ivan have fallen, Felix is met by Karst and Agatio
   waiting there: "You're late, Felix!" and "Now light the beacon!". */
void scJupiterLighthouseKarstAndAgatioWait(void)
{

	if(fdScriptGetFlag(FLAG_ISAAC_AND_IVAN_FELL)) {
		fdScriptSetFlag(FLAG_KARST_AND_AGATIO_MET_FELIX);
		if(!fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT)) {
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			syFieldScriptSetMessageId((Sint32)MsgYouReLate);
			fdActorShowEmote(
				ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
			/* "You're late, Felix!" */
			fdScriptShowMessage(ACTOR_KARST, 0);
			fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
			/* "Now light the beacon!" */
			fdScriptShowMessage(ACTOR_AGATIO, 0);
			fdScriptEnd();
		}
	}
}

extern Sint32 JupiterLighthouseBeaconParticleScript[];

/* Every third tick, or every tick with FLAG_BEACON_PARTICLES_EVERY_TICK,
   sends up a particle from a random height on the first column of light,
   circling it (scJupiterLighthouseCircleBeacon). */
void scJupiterLighthouseSpawnBeaconParticle(void)
{
	ACTOR	  *center;
	ACTOR	  *effect;
	FIELD_SPR *spr;
	Sint32	   ang;

	if(fdScriptGetFlag(FLAG_BEACON_PARTICLES_EVERY_TICK) == 0 &&
		TickCount % 3 != 0)
		return;
	center = fdActorGet(ACTOR_BEAM_BOTTOM);
	if(fdScriptGetFlag(FLAG_BEACON_PARTICLES_EVERY_TICK) != 0) {
		Uint32 random;

		random = syRandom();
		effect = scFieldActorCreate(SPRITE_PARTICLE, center->x.fixed,
			(Sint32)((Uint32)(random << 8) >> 16 << 16) + center->y.fixed -
				toFIXED(28),
			center->z.fixed);
	} else {
		Uint32 random;

		random = syRandom();
		effect = scFieldActorCreate(SPRITE_PARTICLE, center->x.fixed,
			(Sint32)((Uint32)(random << 6) >> 16 << 16) + center->y.fixed -
				toFIXED(28),
			center->z.fixed);
	}
	if(effect != 0) {
		spr = effect->spr;
		fdActorStartScript(effect, JupiterLighthouseBeaconParticleScript);
		scFieldActorSetSprCol(effect, SPRITE_COLOR_GRB);
		effect->motionFlg = 0;
		ang = syRandom() & 0xffff000;
		effect->tmr = ang;
		effect->userValue = 0;
		effect->update = scJupiterLighthouseCircleBeacon;
		effect->speedLimit =
			syMathSin((Uint32)(syRandom() * 65535) >> 20) * 24 >> 16;
		spr->flg = 0;
		spr->pri = 1;
	}
}

/* The furthest south the view may lie, in pixels, with the lit beacon's light
   drawn over the well. */
#define BEACON_ORB_VIEW_Z_MAX 177

/* The lit beacon's light (ACTOR_BEACON_ORB) pulses over the well while the view
   lies no further south than BEACON_ORB_VIEW_Z_MAX: full size on odd ticks,
   1.3 times the size and 7 pixels lower on even ones. Further south it is put
   away at the origin. */
void scJupiterLighthousePulseBeacon(void)
{
	ACTOR	  *actor = fdActorGet(ACTOR_BEACON_ORB);
	MAP_STATE *map = RamHeapSlots->mapGrid;

	syRandom();
#if BEACON_ORB_NULL_CHECK
	/* In every edition but the Japanese; the Japanese release goes on when the
	   light is missing. */
	if(actor == NULL)
		return;
#endif
	if(FIXEDtoINT(map->view[1]) <= BEACON_ORB_VIEW_Z_MAX) {
		((SPR_ANIMATION *)((FIELD_SPR *)fdActorGet(ACTOR_BEACON_ORB)->spr)
				->record)
			->colMode = SPRITE_COLOR_GRB;
		if(TickCount & 1) {
			fdActorSetPos(ACTOR_BEACON_ORB, 512, 264);
			fdActorGet(ACTOR_BEACON_ORB)->y.fixed = toFIXED(223) / 2;
			fdActorGet(ACTOR_BEACON_ORB)->scaleX = toFIXED(1);
			fdActorGet(ACTOR_BEACON_ORB)->scaleY = toFIXED(1);
		} else {
			fdActorSetPos(ACTOR_BEACON_ORB, 512, 264);
			fdActorGet(ACTOR_BEACON_ORB)->y.fixed = toFIXED(209) / 2;
			fdActorGet(ACTOR_BEACON_ORB)->scaleX = toFIXED(13) / 10;
			fdActorGet(ACTOR_BEACON_ORB)->scaleY = toFIXED(13) / 10;
		}
	} else {
		fdActorSetPos(ACTOR_BEACON_ORB, 0, 0);
	}
}

extern Sint32 JupiterLighthouseBeamOpenScript[];

/* Stepping up to the well in the aerie's centre (metatiles (29, 18) to (34,
   18)) once Isaac and Ivan have fallen, Felix is asked whether to cast the
   Elemental Star into its wide hole. Yes, and he throws the Jupiter Star in,
   its light rises out of the well in two beams and the beacon is lit; the lit
   beacon is shown from the world map, then the party returns to the aerie
   beside it. No, and he steps back from the well.

   The party returns by ENTRANCE_BEACON_LIT, where Karst and Agatio turn on it
   (scJupiterLighthouseBeaconIsLit). */
void scJupiterLighthouseCastTheJupiterStar(void)
{
	Sint32		   answer, state;
	ACTOR		  *felix, *star, *shadow, *bottom, *top;
	FIELD_SPR	  *spr;
	ICON_TRANSFER *glyph;
	Uint8		  *flg;

	if(syGameFlagGet(FLAG_ISAAC_AND_IVAN_FELL) != 0) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		/* "There seems to be a wide hole for the Elemental Star. Cast
		   the..." */
		mnMessageShowCentered((Sint32)MsgThereSeemsTo,
			FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
		answer = fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
		if(answer != 0) {
			fdActorSetSpeed(ACTOR_FELIX, 1);
			fdActorWalkToAndWait(ACTOR_FELIX, 514, 309);
		} else {
			fdScriptSetFlag(FLAG_JUPITER_BEACON_LIT);
			fdScriptSetFlag(FLAG_SHIP_FLIES);
			fdScriptClearFlag(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY);
			fdScriptClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
			fdScriptClearFlag(FLAG_RETREAT_BLOCKED);
			fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
			fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
			fdActorSetSpeed(ACTOR_FELIX, 1);
			fdActorWalkToAndWait(ACTOR_FELIX, 512, 294);
			fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
			syFieldScriptWait(20);
			fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
			fdPaletteFadeInterpolate(20);
			syWait(20);
			gmPartyInventoryRemove(ITEM_MYTHRIL_BAG_B);
			felix = fdActorGetById(ACTOR_FELIX);
			star = scFieldActorCreate(SPRITE_UNSEEN_MARKER, felix->x.fixed,
				felix->y.fixed + toFIXED(8), felix->z.fixed);
			if(star != 0) {
				spr = star->spr;
				spr->cnt = 0;
				spr->fullCol = 0;
				spr->pal = 0;
				spr->pri = 0;
				star->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
				star->motionFlg = 0;
				star->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
				star->speedLimit = toFIXED(2) / 5;
				star->acceleration = toFIXED(1) / 20;
				glyph = syHeapSlotAlloc(
					HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
				gmItemLoadIcon(ITEM_JUPITER_STAR);
				mnVRAMLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
				syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
			}
			scFieldActorSetSprPriority(ACTOR_STAR_SHADOW, 1);
			shadow = fdActorGetById(ACTOR_STAR_SHADOW);
			shadow->x.fixed = felix->x.fixed;
			shadow->y.fixed = toFIXED(96);
			shadow->z.fixed = felix->z.fixed;
			shadow->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			shadow->acceleration = toFIXED(1) / 20;
			shadow->speedLimit = toFIXED(2) / 5;
			shadow->scaleX = toFIXED(1) / 8;
			shadow->scaleY = toFIXED(1) / 8;
			syFieldScriptSetMessageId((Sint32)MsgThereSeemsTo + 1);
			fdScriptPlaySnd(MUS_CONFRONTATION);
			if(star != 0) {
				flg = &star->motionFlg;
				*flg = 0;
				star->gravity = toFIXED(3) / 5;
				star->bounce = toFIXED(4) / 5;
				star->velY = toFIXED(8);
				scFieldActorSetMoveTarget(star, 512, 128, 294);
				scFieldActorSetMoveTargetFX(
					shadow, toFIXED(512), -1, toFIXED(294));
				fdActorPlaceOnActor(
					ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
				syWait(1);
				fdActorSetSpeed(ACTOR_PIERS, 0.8);
				fdActorWalkBy(ACTOR_PIERS, 12, 0);
				fdActorWalkByAndWait(ACTOR_FELIX, -12, 0);
				scFieldActorWaitForMoveById(ACTOR_PIERS);
				scFieldActorStop(ACTOR_PIERS);
				syWait(1);
				fdActorSetAnimation(ACTOR_PIERS, ANIM_STAND);
				syWait(1);
				fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
				fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
				scFieldActorWaitForMove(star);
				state = ACTOR_FLOATS;
				*flg = state;
				syFieldScriptWait(90);
				*flg = 0;
				scFieldActorSetMoveTarget(star, 512, 128, 264);
				scFieldActorSetMoveTargetFX(
					shadow, toFIXED(512), -1, toFIXED(264));
				*flg = state;
				syFieldScriptWait(60);
				*flg = 0;
				shadow->x.fixed = 0;
				shadow->tgtX = ACTOR_NO_TARGET;
				shadow->tgtY = ACTOR_NO_TARGET;
				shadow->tgtZ = ACTOR_NO_TARGET;

				shadow->y.fixed = 0;
				shadow->z.fixed = 0;
				shadow->velX = 0;
				shadow->velY = 0;
				shadow->velZ = 0;
				shadow->tmr = 0;
				scFieldActorWaitForMove(star);
				*flg = state;
				syFieldScriptWait(90);
				*flg = 0;
				scFieldActorSetMoveTarget(star, 512, 92, 264);
				scFieldActorWaitForMove(star);
				star->x.fixed = 0;
				star->tgtX = ACTOR_NO_TARGET;
				star->tgtY = ACTOR_NO_TARGET;
				star->tgtZ = ACTOR_NO_TARGET;
				star->y.fixed = 0;
				star->z.fixed = 0;
				star->velX = 0;
				star->velY = 0;
				star->velZ = 0;
				star->tmr = 0;
			}
			syFieldScriptWait(40);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeInterpolate(20);
			syWait(20);
			fdActorWalkBy(ACTOR_FELIX, 0, -16);
			fdActorWalkBy(ACTOR_PIERS, 0, -16);
			scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
			fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
			fdActorSetAnimation(ACTOR_PIERS, ANIM_STAND);
			syFieldScriptWait(50);
			fdScriptPlaySnd(SE_RUMBLE);
			syFieldScriptWait(30);
			fdMapGridSetShake(1, 1, 1);
			syFieldScriptWait(30);
			fdMapGridSetShake(2, 2, 1);
			syFieldScriptWait(30);
			fdActorGetById(ACTOR_ISAAC)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_IVAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_JENNA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_PIERS)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_SHEBA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_KRADEN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_BEAM_BOTTOM)->priFlg |=
				ACTOR_PRIORITY_AUTOMATIC;
			fdActorGetById(ACTOR_BEAM_TOP)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			fdCameraMoveTo(512, 96, 264, 1);
			fdCameraWaitForArrival();
			scFieldActorSetSprPriority(ACTOR_BEAM_BOTTOM, 1);
			scFieldActorSetSprPriority(ACTOR_BEAM_TOP, 1);
			scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
			scFieldActorSetSprPriority(ACTOR_PIERS, 1);
			syFieldScriptWait(20);
			fdActorSetSprFlags(fdActorGetById(ACTOR_BEAM_BOTTOM), 0);
			scFieldActorSetSprCol(
				fdActorGetById(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_WHITE);
			bottom = fdActorGetById(ACTOR_BEAM_BOTTOM);
			bottom->scaleY = -toFIXED(1);
			bottom->scaleX = toFIXED(1) / 10;
			bottom->motionFlg = 0;
			bottom->x.fixed = toFIXED(512);
			bottom->y.fixed = toFIXED(128);
			bottom->z.fixed = toFIXED(264);
			bottom->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetSprFlags(fdActorGetById(ACTOR_BEAM_TOP), 0);
			scFieldActorSetSprCol(
				fdActorGetById(ACTOR_BEAM_TOP), SPRITE_COLOR_WHITE);
			top = fdActorGetById(ACTOR_BEAM_TOP);
			top->scaleY = -toFIXED(1);
			top->scaleX = toFIXED(1) / 10;
			top->motionFlg = 0;
			top->x.fixed = toFIXED(512);
			top->y.fixed = toFIXED(160);
			top->z.fixed = toFIXED(264);
			top->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetScript(
				ACTOR_BEAM_BOTTOM, JupiterLighthouseBeamOpenScript);
			fdActorSetScript(ACTOR_BEAM_TOP, JupiterLighthouseBeamOpenScript);
			sySetTickEvent(
				scJupiterLighthouseSpawnBeaconParticle, EVENT_PASS_TICK);
			fdActorSetPos(ACTOR_LEFT_ANGEL, 0, 0);
			fdActorSetPos(ACTOR_RIGHT_ANGEL, 0, 0);
			fdActorSetPos(ACTOR_LID, 0, 0);
			fdActorSetPos(ACTOR_UNSEEN_LID, 0, 0);
			fdScriptPlaySnd(SE_CRASH);
			fdMapGridSetShake(6, 6, 1);
			fdPaletteFadeApplyTarget(
				PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
			fdPaletteFadeInterpolate(16);
			syWait(16);
			fdScriptPlaySnd(MUS_ALCHEMY_UNLEASHED);
			syWait(180);
			fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
			fdPaletteFadeInterpolate(60);
			fdScriptPlaySnd(SE_RUMBLE);
			syWait(60);
			syFieldCloseEvent(scJupiterLighthouseSpawnBeaconParticle);
			fdScriptSetFlag(FLAG_KEEP_MUSIC_ONCE);
			syFieldScriptSetExitRequest(
				JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_WORLD_MAP);
		}
		fdScriptEnd();
	}
}

/* Back on the aerie beside the lit beacon, Karst and Agatio turn on Felix and
   Piers, give back the Mythril Bag, and the first battle with them begins. The
   party arrives by ENTRANCE_BEACON_LIT. */
void scJupiterLighthouseBeaconIsLit(void)
{
	FIELD_STATE *state;
	Sint32		 slot;
	Sint32		 facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 120;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(512, 96, 240, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	syFieldScriptSetMessageId((Sint32)MsgWeHaveIgnited);
	fdActorSetPosAndFacing(
		ACTOR_FELIX, toFIXED(508), toFIXED(278), facing = FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(532), toFIXED(278), facing);
	fdScriptPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(4, 4, 1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdMapGridSetShake(3, 3, 1);
	syFieldScriptWait(60);
	fdMapGridSetShake(2, 2, 1);
	syFieldScriptWait(60);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(512, 96, 296, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_NOD, 20);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(20);
	fdActorWalkBy(ACTOR_FELIX, 0, 8);
	fdActorWalkBy(ACTOR_PIERS, 0, 8);
	fdActorWalkBy(ACTOR_AGATIO, 32, -16);
	fdActorWalkBy(ACTOR_KARST, 48, -16);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	scFieldActorWaitForMoveById(ACTOR_PIERS);
	scFieldActorWaitForMoveById(ACTOR_AGATIO);
	scFieldActorWaitForMoveById(ACTOR_KARST);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldScriptWait(5);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "We have ignited the beacon... The third lighthouse is lit... Now
	   let..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	fdScriptPlaySnd(MUS_KARST_AND_AGATIO);
	/* "I don't think so..." */
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
	/* "You see, you've betrayed us once already..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "We simply can't trust you anymore..." */
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	/* "And now that Jupiter Lighthouse is lit, I'm afraid we have little
	   use..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	syFieldScriptWait(5);
	/* "Poor dear... You look puzzled. Would you like to know why we
	   don't..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		/* "I'd be happy to tell you. Mars Lighthouse is in Prox." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg++;
		/* "Figured it out, have you? Yes, Mars Lighthouse is in Prox." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_LEFT | ACTOR_KARST, 0, 0);
	}
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	syFieldScriptWait(10);
	fdActorWalkByAndWait(ACTOR_KARST, -16, -24);
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorGet(ACTOR_KARST)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_KARST)->velY = toFIXED(4);
	fdActorSetDestinationOffset(ACTOR_KARST, 0, 24);
	scFieldActorWaitForMoveById(ACTOR_KARST);
	fdActorGet(ACTOR_KARST)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	/* "What are you doing!?" */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	/* "They stole the Mars Star!" */
	mnMessageShowCentered(
		FieldStateWork->msg++, FIELD_MESSAGE_ZOOM | FIELD_MESSAGE_KEEP_OPEN);
	gmPartyInventoryAdd(ITEM_MYTHRIL_BAG);
	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if((syUnitGet(FELIX)->inventory[slot] & INVENTORY_ITEM_ID_MASK) ==
			ITEM_MYTHRIL_BAG) {
			gmInventoryEquip(FELIX, slot);
			break;
		}
	}
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldScriptWait(5);
	fdActorWalkBy(ACTOR_FELIX, 0, 8);
	fdActorWalkByAndWait(ACTOR_AGATIO, -24, -8);
	fdActorGet(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, -8);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Your job is done now." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	fdActorGet(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
	/* "We're not done yet..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
	/* "We've got the Mars Star! We don't need you anymore!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	syFieldScriptWait(10);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(30);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHWEST, 0);
	/* "You know what that means, of course, don't you?" */
	fdScriptOpenMessage(ACTOR_KARST, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
		/* "Exactly! That means it's time to die, Felix!" */
		syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
		FieldStateWork->msg++;
	} else {
		fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
		FieldStateWork->msg++;
		/* "No? Well, how can I put this? It's time to die!" */
		syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	}
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldScriptWait(5);
	/* "But what will happen to Felix's parents?" */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
	/* "We keep our promises! Once all four lighthouses have been lit, we..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_KARST, FACING_NORTH_NORTHEAST, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_SHAKE_HEAD, 33);
	/* "We would never do anything as terrible as breaking our word... as
	   you..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTH_NORTHWEST, 0);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
	/* "But if you're dead... Who will come to take them home from Prox?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	/* "I guess they'll just have to spend their remaining years in the..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 2);
	fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
	syFieldScriptWait(5);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	/* "Don't worry! They'll be fine! Now prepare for the end!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_AGATIO, 0, 0);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	syFieldScriptSetAfterBattleDestination(
		(Sint32)&SceneIdJupiterLighthouseAerieBeacon,
		ENTRANCE_AFTER_FIRST_BATTLE);
	fdScriptSetDefeatDestination((Sint32)&SceneIdJupiterLighthouseAerieBeacon,
		ENTRANCE_AFTER_FIRST_BATTLE);
	fdScriptSetFlag(FLAG_KEEP_MUSIC_HELD);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MOAPA, 5);
	return;
}

/* The HP the next battle's script takes from Felix when the party lost to Karst
   and Agatio: more than the 1 HP the defeat revived him with
   (gmReturnFromMode), so he goes into it downed. */
#define DEFEAT_REVIVAL_HP_TAKEN 10

/* After the first battle with Karst and Agatio on the aerie, Jenna comes to
   help, joins Felix, and the second battle begins. The party arrives by
   ENTRANCE_AFTER_FIRST_BATTLE. */
void scJupiterLighthouseJennaJoinsTheFight(void)
{
	Sint32 northwest;
	Sint32 southeast;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	northwest = FACING_NORTHWEST;
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(512), toFIXED(312), northwest);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(304), northwest);
	southeast = FACING_SOUTHEAST;
	fdActorSetPosAndFacing(ACTOR_AGATIO, toFIXED(504), toFIXED(304), southeast);
	fdActorSetPosAndFacing(ACTOR_KARST, toFIXED(520), toFIXED(288), southeast);
	fdActorSetPosAndFacing(
		ACTOR_JENNA, toFIXED(552), toFIXED(392), FACING_NORTH);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_AGATIO_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_KARST, ANIM_KARST_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_PIERS_BATTLE_STANCE);
	fdCameraMoveTo(520, 104, 312, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId((Sint32)MsgCallJenna);
	/* "Felix!" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorGet(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_PIERS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_AGATIO)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_KARST)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_FELIX)->velY = toFIXED(3);
	fdActorGet(ACTOR_PIERS)->velY = toFIXED(3);
	fdActorGet(ACTOR_AGATIO)->velY = toFIXED(3);
	fdActorGet(ACTOR_KARST)->velY = toFIXED(3);
	fdActorSetDestinationOffset(ACTOR_FELIX, 8, 4);
	fdActorSetDestinationOffset(ACTOR_PIERS, 8, 4);
	fdActorSetDestinationOffset(ACTOR_AGATIO, -16, -8);
	fdActorSetDestinationOffset(ACTOR_KARST, -16, -8);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	fdActorGet(ACTOR_FELIX)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_PIERS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_AGATIO)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_KARST)->steeringFlg |= ACTOR_FACES_MOTION;
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdActorWalkToAndWait(ACTOR_JENNA, 552, 344);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "I thought you were taking too long, so I came to check on you...." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
	syFieldScriptWait(10);
	/* "They say they don't need us anymore..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Agatio! Felix's sister isn't supposed to be here..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	/* "What do you want me to do? We can't just leave now! We have to..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorFaceDirection(ACTOR_KARST, southeast, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, southeast);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, northwest);
	syFieldScriptWait(10);
	/* "Don't worry, Felix! I'm here to help!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorWalkBy(ACTOR_JENNA, -8, -24);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, southeast);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	fdActorFaceDirection(ACTOR_PIERS, northwest, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, northwest);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, northwest);
	syFieldScriptWait(10);
	fdActorSetDestinationOffset(ACTOR_FELIX, -16, -8);
	fdActorSetDestinationOffset(ACTOR_PIERS, -16, -8);
	fdActorSetDestinationOffset(ACTOR_JENNA, -16, -8);
	fdActorSetDestinationOffset(ACTOR_AGATIO, 16, 8);
	fdActorSetDestinationOffset(ACTOR_KARST, 16, 8);
	fdActorGet(ACTOR_FELIX)->velY = toFIXED(4);
	fdActorGet(ACTOR_PIERS)->velY = toFIXED(4);
	fdActorGet(ACTOR_JENNA)->velY = toFIXED(4);
	fdActorGet(ACTOR_AGATIO)->velY = toFIXED(4);
	fdActorGet(ACTOR_KARST)->velY = toFIXED(4);
	syFieldScriptWait(4);
	if(MainRoutineStateWork.modeResult == BATTLE_OUTCOME_LOST) {
		scFieldUnitAddHp(FELIX, -DEFEAT_REVIVAL_HP_TAKEN);
	}
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	syFieldScriptSetAfterBattleDestination(
		(Sint32)&SceneIdJupiterLighthouseAerieBeacon,
		ENTRANCE_AFTER_SECOND_BATTLE);
	fdScriptSetDefeatDestination((Sint32)&SceneIdJupiterLighthouseAerieBeacon,
		ENTRANCE_AFTER_SECOND_BATTLE);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MOAPA, 6);
}

/* After the second battle on the aerie, Sheba comes to help, joins the party,
   and the last battle with Karst and Agatio begins. The party arrives by
   ENTRANCE_AFTER_SECOND_BATTLE. */
void scJupiterLighthouseShebaJoinsTheFight(void)
{
	Sint32 northwest = FACING_NORTHWEST;
	Sint32 southeast;
	Sint32 north;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(512), toFIXED(312), northwest);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(304), northwest);
	southeast = FACING_SOUTHEAST;
	fdActorSetPosAndFacing(ACTOR_AGATIO, toFIXED(504), toFIXED(304), southeast);
	fdActorSetPosAndFacing(ACTOR_KARST, toFIXED(520), toFIXED(288), southeast);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(528), toFIXED(320), northwest);
	north = FACING_NORTH;
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(544), toFIXED(384), north);
	fdCameraMoveTo(520, 104, 312, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId((Sint32)MsgCallSheba);
	/* "Jenna!" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_AGATIO_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_KARST, ANIM_KARST_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_PIERS_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_JENNA_BATTLE_STANCE);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorGet(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_PIERS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_JENNA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_AGATIO)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_KARST)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_FELIX)->velY = toFIXED(3);
	fdActorGet(ACTOR_PIERS)->velY = toFIXED(3);
	fdActorGet(ACTOR_JENNA)->velY = toFIXED(3);
	fdActorGet(ACTOR_AGATIO)->velY = toFIXED(3);
	fdActorGet(ACTOR_KARST)->velY = toFIXED(3);
	fdActorSetDestinationOffset(ACTOR_FELIX, 8, 4);
	fdActorSetDestinationOffset(ACTOR_PIERS, 8, 4);
	fdActorSetDestinationOffset(ACTOR_JENNA, 8, 4);
	fdActorSetDestinationOffset(ACTOR_AGATIO, -16, -8);
	fdActorSetDestinationOffset(ACTOR_KARST, -16, -8);
	scFieldActorWaitForMoveById(ACTOR_FELIX);
	fdActorGet(ACTOR_FELIX)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_PIERS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_JENNA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_AGATIO)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_KARST)->steeringFlg |= ACTOR_FACES_MOTION;
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdActorWalkToAndWait(ACTOR_SHEBA, 552, 344);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "I got the feeling something was wrong, so I rushed up here. Just
	   in..." */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(5);
	/* "Thank you, Sheba! I'm not looking forward to fighting, but I'm
	   glad..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Another one? Agatio, we're rapidly being outnumbered!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "It's too late to flee now! We have to keep fighting!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, north);
	syFieldScriptWait(10);
	/* "Isaac and the others are safe! Now let's take care of these two
	   rats!" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorWalkBy(ACTOR_SHEBA, 0, -24);
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, southeast);
	fdActorFaceDirection(ACTOR_SHEBA, northwest, 0);
	fdActorAnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
	fdActorFaceDirection(ACTOR_PIERS, northwest, 0);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, northwest);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, northwest);
	syFieldScriptWait(10);
	fdActorSetDestinationOffset(ACTOR_FELIX, -16, -8);
	fdActorSetDestinationOffset(ACTOR_PIERS, -16, -8);
	fdActorSetDestinationOffset(ACTOR_JENNA, -16, -8);
	fdActorSetDestinationOffset(ACTOR_SHEBA, -16, -8);
	fdActorSetDestinationOffset(ACTOR_AGATIO, 16, 8);
	fdActorSetDestinationOffset(ACTOR_KARST, 16, 8);
	fdActorGet(ACTOR_FELIX)->velY = toFIXED(4);
	fdActorGet(ACTOR_PIERS)->velY = toFIXED(4);
	fdActorGet(ACTOR_JENNA)->velY = toFIXED(4);
	fdActorGet(ACTOR_SHEBA)->velY = toFIXED(4);
	fdActorGet(ACTOR_AGATIO)->velY = toFIXED(4);
	fdActorGet(ACTOR_KARST)->velY = toFIXED(4);
	syFieldScriptWait(4);
	if(MainRoutineStateWork.modeResult == BATTLE_OUTCOME_LOST)
		scFieldUnitAddHp(FELIX, -DEFEAT_REVIVAL_HP_TAKEN);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	SET_BATTLE_TYPE(BATTLE_TYPE_NO_ESCAPE);
	syFieldScriptSetAfterBattleDestination(
		(Sint32)&SceneIdJupiterLighthouseAerieBeacon, ENTRANCE_LAST_BATTLE_WON);
	fdScriptSetDefeatDestination((Sint32)&SceneIdJupiterLighthouseAerieBeacon,
		ENTRANCE_LAST_BATTLE_LOST);
	fdScriptClearFlag(FLAG_KEEP_MUSIC_HELD);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_MOAPA, 7);
}

/* The caster's arcs of light reach a fallen actor and it stands up: Mia healing
   Felix's party, or Alex healing Agatio and Karst. */
void scJupiterLighthouseHealActor(Sint32 casterId, Sint32 tgtId)
{
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(
		fdActorGet(casterId), CASTER_LOOK_CYCLING_COLORS);
	fdScriptPlaySnd(SE_ASCEND);
	syFieldScriptWait(60);
	fdBattleEffectSetObjectUpdateMode(fdActorGet(casterId), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdActorSetAnimation(tgtId, ANIM_STAND);
}

extern Sint32 JupiterLighthouseJoinLeaderScript[];
extern Sint32 JupiterLighthouseWalkToPlatformsScript[];
extern Sint32 JupiterLighthouseHopWestOverPlatformsScript[];
extern Sint32 JupiterLighthouseHopEastOverPlatformsScript[];

/* When the party loses the last battle on the aerie, Alex arrives and takes
   Karst and Agatio down on the elevator; Isaac's party comes up, Mia heals
   Felix's party, and both parties leave on the elevator. The party arrives by
   ENTRANCE_LAST_BATTLE_LOST. */
void scJupiterLighthouseKarstAndAgatioRetreat(void)
{
	Sint32 northwest;
	Sint32 southeast;
	Sint32 north;
	ACTOR *actor;

	FIELD_STATE *state;

	void scJupiterLighthouseHealParty(Sint32 clearStatus)
	{
		Sint32 count;
		Sint32 n;
		UNIT  *member;
		Sint16 maxHp;

		count = gmPartyCountMembers();
		for(n = 0; n < count; n++) {
			MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

			member = syUnitGet(party->party[n]);
			maxHp = member->hp = member->hpMax;
			member->hpGauge =
				gmUnitClampGauge((maxHp << UNIT_GAUGE_SHIFT) / maxHp);
			if(member->hpGauge == 0 && member->hp != 0)
				member->hpGauge = 1;
			member->ppGauge = gmUnitClampGauge(
				(member->pp << UNIT_GAUGE_SHIFT) / member->ppMax);
			if(member->ppGauge == 0 && member->pp != 0)
				member->ppGauge = 1;
			if(clearStatus == 1) {
				member->poison = 0;
				member->haunt = 0;
			}
		}
	}

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorGet(ACTOR_ISAAC)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_JENNA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_PIERS)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_SHEBA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_KRADEN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	north = FACING_NORTH;
	northwest = FACING_NORTHWEST;
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(504), toFIXED(304), northwest);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(528), toFIXED(288), northwest);
	southeast = FACING_SOUTHEAST;
	fdActorSetPosAndFacing(ACTOR_AGATIO, toFIXED(488), toFIXED(296), southeast);
	fdActorSetPosAndFacing(ACTOR_KARST, toFIXED(504), toFIXED(280), southeast);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(528), toFIXED(304), northwest);
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(552), toFIXED(288), north);
	fdActorSetPosAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), north);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(592), toFIXED(392), FACING_EAST);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_PIERS_LIE_BEATEN);
	actor = fdActorGet(ACTOR_FELIX);
	actor->scaleX = -actor->scaleX;
	actor = fdActorGet(ACTOR_SHEBA);
	actor->scaleX = -actor->scaleX;
	fdCameraMoveTo(520, 104, 312, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId((Sint32)MsgWasnTThe);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_AGATIO_BATTLE_STANCE);
	fdActorSetAnimation(ACTOR_KARST, ANIM_KARST_BATTLE_STANCE);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	syFieldScriptWait(5);
	/* "Isaac wasn't the only surprise. They were all far more powerful
	   than..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Don't worry, Karst! We'll win in the end..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldScriptWait(5);
	fdActorFaceDirectionAndWait(ACTOR_KARST, southeast);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, southeast);
	syFieldScriptWait(10);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	/* "This one moved... I can't believe they're still alive! The pests!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_SHAKE_HEAD, 33);
	/* "If we stay here much longer, Isaac and the others will come! We..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "I know... But treason deserves death! We must finish them!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	/* "There's no time for petty grudges!" */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_SOUTH);
	syFieldScriptWait(10);
	fdCameraMoveTo(520, 104, 344, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdScriptPlaySnd(MUS_ALEX);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Alex! You..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
	/* "You no longer consider me a friend? I do not care." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "It goes beyond that, Alex! You've betrayed us, and treason
	   deserves..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Yes, I heard you the first time. But shouldn't you heal
	   yourselves..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorSetAnimation(ACTOR_KARST, ANIM_STAND);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
	/* "After fighting both Isaac and Felix, you're in no condition to fight
	   me." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
	/* "And if Isaac and the others come, what chance will you have?" */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	/* "What's keeping Felix and the others?" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KARST, 2);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	fdActorFaceDirection(ACTOR_KARST, southeast, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, southeast);
	syFieldScriptWait(5);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, north);
	/* "There's no time to talk!" */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
	/* "Then what should we do?" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
	/* "We take the elevator! With the beacon lit, it should be fully..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	/* "We have no choice. Let's go." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Good! You're listening to reason! Now follow me!" */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_KARST, ANIM_NOD, 20);
	syFieldScriptWait(10);
	syScriptCameraFollowActor(ACTOR_AGATIO, 1);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseWalkToPlatformsScript);
	syFieldScriptWait(30);
	fdActorSetScript(ACTOR_ALEX, JupiterLighthouseWalkToPlatformsScript);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseWalkToPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ALEX);
	fdActorGet(ACTOR_ALEX)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_ALEX)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_ALEX, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ALEX);
	fdActorWalkToAndWait(ACTOR_ALEX, 311, 256);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	fdActorWalkToAndWait(ACTOR_AGATIO, 332, 266);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorWalkToAndWait(ACTOR_KARST, 290, 266);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
	fdActorGet(ACTOR_ALEX)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "Felix! Jenna!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetPosAndFacing(ACTOR_KRADEN, toFIXED(512), toFIXED(328), north);
	scJupiterLighthouseLowerElevator(DESCENT_ALEX_AGATIO_KARST);
	gmPlayMusic();
	syFieldScriptWait(10);
	fdActorSetPosAndFacing(ACTOR_ISAAC, toFIXED(536), toFIXED(368), north);
	fdActorSetPosAndFacing(ACTOR_GARET, toFIXED(536), toFIXED(384), north);
	fdActorSetPosAndFacing(ACTOR_IVAN, toFIXED(552), toFIXED(368), north);
	fdActorSetPosAndFacing(ACTOR_MIA, toFIXED(552), toFIXED(352), north);
	/* "What's happened?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	syScriptCameraFollowActor(ACTOR_ISAAC, 1);
	fdActorWalkTo(ACTOR_ISAAC, 536, 336);
	fdActorWalkTo(ACTOR_GARET, 536, 352);
	fdActorWalkTo(ACTOR_IVAN, 552, 336);
	fdActorWalkTo(ACTOR_MIA, 552, 320);
	scFieldActorWaitForMoveById(ACTOR_ISAAC);
	scFieldActorWaitForMoveById(ACTOR_GARET);
	scFieldActorWaitForMoveById(ACTOR_IVAN);
	scFieldActorWaitForMoveById(ACTOR_MIA);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
	/* "It seems they fought with Agatio and Karst..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorAnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
	/* "I had a really bad feeling after Sheba left. I hate it when I'm
	   right..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_SOUTH);
	syFieldScriptWait(10);
	/* "Your power of prediction must be even stronger than you'd thought..." */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, northwest);
	fdCameraMoveTo(320, 96, 360, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	/* "Blast! They must have taken the elevator to escape!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdCameraMoveTo(512, 96, 352, 1);
	fdCameraWaitForArrival();
	scJupiterLighthouseRaiseElevator(ASCENT_AT_ONCE);
	fdActorAnimateThenWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 33);
	/* "How could they have done that to Jenna? Those monsters!" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, northwest);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	syFieldScriptWait(5);
	/* "Felix tried to help us... They must have attacked him because he..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "So, does that mean Felix isn't really our enemy?" */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
	syFieldScriptWait(5);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, north);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldScriptWait(5);
	/* "Felix! You're alive!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	syFieldScriptWait(10);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 2);
	syFieldScriptWait(5);
	scFieldActorStartRepeatedMotion(ACTOR_PIERS, 2);
	syFieldScriptWait(5);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(30);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	/* "Mia, he needs healing, quickly! There's still time!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorAnimateThenWait(ACTOR_MIA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_GARET, northwest);
	/* "Don't let 'em die! Felix still has a lot of questions to answer..." */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, southeast);
	syFieldScriptWait(10);
	/* "If you save Felix, we'll tell you everything..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, north);
	fdActorWalkByAndWait(ACTOR_MIA, -48, 0);
	fdActorFaceDirection(ACTOR_MIA, north, 0);
	scJupiterLighthouseHealActor(ACTOR_MIA, ACTOR_FELIX);
	fdActorFaceDirection(ACTOR_FELIX, southeast, 0);
	actor = fdActorGet(ACTOR_FELIX);
	actor->scaleX = -actor->scaleX;
	fdActorWalkByAndWait(ACTOR_MIA, 40, 0);
	fdActorWalkByAndWait(ACTOR_MIA, 8, -16);
	fdActorFaceDirection(ACTOR_MIA, north, 0);
	scJupiterLighthouseHealActor(ACTOR_MIA, ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	actor = fdActorGet(ACTOR_SHEBA);
	actor->scaleX = -actor->scaleX;
	fdActorFaceDirection(ACTOR_MIA, northwest, 0);
	scJupiterLighthouseHealActor(ACTOR_MIA, ACTOR_PIERS);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	scJupiterLighthouseHealActor(ACTOR_MIA, ACTOR_JENNA);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, northwest);
	/* "All right, Felix! We want to know what's going on!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorFaceDirection(ACTOR_KRADEN, southeast, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "It's all quite complicated, Isaac. There are a number of
	   extenuating..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "What are you talking about, Kraden?" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorWalkTo(ACTOR_IVAN, 552, 320);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
	fdActorFaceDirection(ACTOR_GARET, north, 0);
	fdActorAnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* "Look, I don't think any of us are in any shape for another fight..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* "What do you mean, Ivan?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	syFieldScriptWait(5);
	/* "I'm just saying we should get ourselves back to Contigo and sort..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_SHEBA, 0);
	fdActorFaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	/* "That's a sound plan, Ivan... I fully agree." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	syFieldScriptWait(50);
	fdActorAnimateThenWait(ACTOR_GARET, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, north);
	syFieldScriptWait(50);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, northwest);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, southeast);
	if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		fdActorAnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
		syFieldScriptWait(10);
		/* "You're right. We can't fight right now. Let's head back to..." */
		syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	} else {
		fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
		FieldStateWork->msg += 2;
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
		/* "He's right, Felix. It's time we explained ourselves to them.
		   We..." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
		syFieldScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
		syFieldScriptWait(5);
		/* "We'll be there, Isaac. I promise. I hope you can still trust me." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
		fdActorAnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
		syFieldScriptWait(10);
		/* "I've never stopped trusting you, Jenna. I'll be waiting for you
		   in..." */
		syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_ISAAC);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_ISAAC);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_ISAAC);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetScript(ACTOR_IVAN, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_MIA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_GARET, JupiterLighthouseJoinLeaderScript);
	scFieldActorWaitForScriptEndById(ACTOR_IVAN);
	scFieldActorWaitForScriptEndById(ACTOR_MIA);
	scFieldActorWaitForScriptEndById(ACTOR_GARET);
	syScriptCameraFollowActor(ACTOR_ISAAC, 1);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, 0);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, 0);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, 0);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, 0);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_ISAAC, 1.5);
	fdActorWalkToAndWait(ACTOR_ISAAC, 543, 363);
	scFieldActorWaitForMoveById(ACTOR_ISAAC);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* Yes: "We'll be waiting for you... So don't think you can sneak off
	   again!"; No: "We'll be expecting you. Don't think you can sneak out of
	   this!..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorWalkToAndWait(ACTOR_ISAAC, 573, 358);
	fdActorWalkToAndWait(ACTOR_ISAAC, 584, 300);
	fdActorWalkToAndWait(ACTOR_ISAAC, 584, 252);
	fdActorWalkToAndWait(ACTOR_ISAAC, 616, 247);
	fdActorWalkToAndWait(ACTOR_ISAAC, 617, 312);
	fdActorGet(ACTOR_ISAAC)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_ISAAC, JupiterLighthouseHopEastOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ISAAC);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	syFieldScriptWait(30);
	fdActorGet(ACTOR_ISAAC)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	scJupiterLighthouseLowerElevator(DESCENT_ISAAC);
	syFieldScriptWait(100);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	scJupiterLighthouseRaiseElevator(ASCENT_RISING);
	syFieldScriptWait(60);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(60);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	syFieldScriptWait(60);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_PIERS, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_SHEBA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_JENNA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = fdActorGet(ACTOR_FELIX);
	if(actor != 0) {
		fdActorSetDestination(
			ACTOR_KRADEN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetScript(ACTOR_PIERS, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_SHEBA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_JENNA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_KRADEN, JupiterLighthouseJoinLeaderScript);
	scFieldActorWaitForScriptEndById(ACTOR_PIERS);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	scFieldActorWaitForScriptEndById(ACTOR_JENNA);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, 0);
	fdActorSetScript(ACTOR_FELIX, JupiterLighthouseWalkToPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	fdActorGet(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_ALEX)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_FELIX, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	fdActorWalkToAndWait(ACTOR_FELIX, 311, 256);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scJupiterLighthouseLowerElevator(DESCENT_FELIX);
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrm = 24;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	scJupiterLighthouseHealParty(0);
	syFieldScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
	fdScriptEnd();
	return;
}

/* When the party wins the last battle on the aerie, Alex heals Karst and Agatio
   and takes them down on the elevator; Isaac's party comes up, and both parties
   leave on the elevator. The party arrives by ENTRANCE_LAST_BATTLE_WON. */
void scJupiterLighthouseWeBeatThem(void)
{
	Sint32		 northwest;
	Sint32		 southeast;
	Sint32		 north;
	ACTOR		*leader;
	FIELD_STATE *state;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorGet(ACTOR_ISAAC)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_GARET)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_IVAN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_JENNA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_PIERS)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_SHEBA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_KRADEN)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_ALEX)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	northwest = FACING_NORTHWEST;
	fdActorSetPosAndFacing(ACTOR_FELIX, toFIXED(520), toFIXED(304), northwest);
	fdActorSetPosAndFacing(ACTOR_PIERS, toFIXED(536), toFIXED(288), northwest);
	southeast = FACING_SOUTHEAST;
	fdActorSetPosAndFacing(ACTOR_AGATIO, toFIXED(480), toFIXED(296), southeast);
	fdActorSetPosAndFacing(ACTOR_KARST, toFIXED(496), toFIXED(280), southeast);
	fdActorSetPosAndFacing(ACTOR_JENNA, toFIXED(544), toFIXED(304), northwest);
	north = FACING_NORTH;
	fdActorSetPosAndFacing(ACTOR_SHEBA, toFIXED(560), toFIXED(288), north);
	fdActorSetPosAndFacing(ACTOR_ALEX, toFIXED(480), toFIXED(360), north);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(592), toFIXED(392), FACING_EAST);
	fdActorSetAnimation(ACTOR_AGATIO, ANIM_AGATIO_LIE_BEATEN);
	fdActorSetAnimation(ACTOR_KARST, ANIM_KARST_LIE_BEATEN);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(520, 104, 312, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetMessageId((Sint32)MsgWeDidItWe);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_SHAKE_HEAD, 33);
	/* "We did it... We beat them..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Barely..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	syFieldScriptWait(10);
	/* "What should we do with them?" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "Finish us!" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	syFieldScriptWait(5);
	/* "But think: what happens to your parents if neither we nor Saturos..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
	syFieldScriptWait(40);
	fdActorFaceDirection(ACTOR_FELIX, northwest, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, northwest);
	syFieldScriptWait(20);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
	/* "Are you saying your people will kill our parents if you don't
	   return..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	syFieldScriptWait(5);
	/* "Well then, I suppose you'd better finish us off, like I said." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	syFieldScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	/* "What's the matter? Are you going to do it or not?" */
	fdScriptOpenMessage(ACTOR_KARST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		/* "No, Felix! Don't!" */
		syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
		FieldStateWork->msg = (FieldStateWork->msg + 1);
	} else {
		FieldStateWork->msg = (FieldStateWork->msg + 1);
		/* "You've made the better choice, Felix." */
		syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	}
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdCameraMoveTo(520, 104, 352, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(10);
	fdScriptPlaySnd(MUS_ALEX);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
	/* "When did you get here, Alex?" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdCameraMoveTo(520, 104, 312, 1);
	fdActorWalkToAndWait(ACTOR_ALEX, 480, 320);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
	/* "If Karst is not bluffing... If you kill them, all of your efforts..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
	/* "Do you understand, Felix? If that happens, Isaac will suffer the
	   same..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, north);
	fdActorWalkByAndWait(ACTOR_ALEX, 20, -20);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_WEST);
	scJupiterLighthouseHealActor(ACTOR_ALEX, ACTOR_AGATIO);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Alex! What are you doing!?" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, north);
	scJupiterLighthouseHealActor(ACTOR_ALEX, ACTOR_KARST);
	fdActorWalkByAndWait(ACTOR_ALEX, -20, 20);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I have revived them." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	scFieldActorStartRepeatedMotion(ACTOR_AGATIO, 2);
	fdActorRunRepeatedMotion(ACTOR_KARST, 2);
	syFieldScriptWait(5);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Why!? Are you going to make us fight them again?" */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_SHAKE_HEAD, 33);
	/* "Don't worry, Jenna. Look at them... They can barely walk right now." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorAnimateThenWait(ACTOR_PIERS, ANIM_SHAKE_HEAD, 33);
	/* "Still, that doesn't give you the right to..." */
	syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 0);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "If you're not going to finish them off, you'll just be leaving
	   them..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_SOUTH);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You're not going to help us defeat these brats?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_SOUTH);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "You'd better not be expecting a big thank-you for this, Alex!" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 0);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Of course not, Karst. I would never ask for your gratitude." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	/* "What's keeping Felix and the others?" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_KARST, FACING_SOUTHWEST);
	syFieldScriptWait(10);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, FACING_EAST);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "We should be going before Isaac and the others arrive." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_SHEBA, 2);
	syFieldScriptWait(5);
	/* "If they're coming up, how do you plan on avoiding them on your way
	   down?" */
	syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 0);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Do not worry about us... We'll just take the elevator." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	syFieldScriptWait(10);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	/* "Now that the lighthouse has been lit, it should be fully
	   operational..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ALEX, north);
	fdActorAnimateThenWait(ACTOR_ALEX, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_AGATIO, FACING_NORTHEAST);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_KARST, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_AGATIO, ANIM_NOD, 20);
	syFieldScriptWait(10);
	syScriptCameraFollowActor(ACTOR_AGATIO, 1);
	fdActorSetScript(ACTOR_ALEX, JupiterLighthouseWalkToPlatformsScript);
	syFieldScriptWait(30);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseWalkToPlatformsScript);
	syFieldScriptWait(30);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseWalkToPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ALEX);
	fdActorGet(ACTOR_ALEX)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_ALEX)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_ALEX, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	fdActorSetScript(ACTOR_AGATIO, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorSetScript(ACTOR_KARST, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ALEX);
	fdActorWalkToAndWait(ACTOR_ALEX, 311, 256);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_AGATIO);
	fdActorWalkToAndWait(ACTOR_AGATIO, 332, 266);
	fdActorFaceDirection(ACTOR_AGATIO, FACING_SOUTH, 0);
	scFieldActorWaitForScriptEndById(ACTOR_KARST);
	fdActorWalkToAndWait(ACTOR_KARST, 290, 266);
	fdActorFaceDirection(ACTOR_KARST, FACING_SOUTH, 0);
	fdActorGet(ACTOR_ALEX)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_AGATIO)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_KARST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	/* "Oh! You're looking better!" */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorSetPosAndFacing(
		ACTOR_KRADEN, toFIXED(512), toFIXED(328), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_ISAAC, toFIXED(536), toFIXED(336), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_GARET, toFIXED(536), toFIXED(352), FACING_NORTH);
	fdActorSetPosAndFacing(
		ACTOR_IVAN, toFIXED(552), toFIXED(352), FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_MIA, toFIXED(552), toFIXED(368), FACING_NORTH);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	scJupiterLighthouseLowerElevator(DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC);
	gmPlayMusic();
	/* "Felix, Jenna..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Isaac... Garet..." */
	syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
	syFieldScriptWait(20);
	scJupiterLighthouseRaiseElevator(ASCENT_AT_ONCE);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "Man! They got away! What a bunch of jerks!" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* "All right, Felix! We want to know what's going on!" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTHEAST);
	/* "It's all quite complicated, Isaac. There are a number of
	   extenuating..." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "What are you talking about, Kraden?" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	fdActorWalkTo(ACTOR_IVAN, 552, 320);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTH);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
	fdActorAnimateThenWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 33);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* "Look, I don't think any of us are in any shape for another fight..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	syFieldScriptWait(5);
	/* "What do you mean, Ivan?" */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_SOUTHWEST);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	syFieldScriptWait(5);
	/* "I'm just saying we should get ourselves back to Contigo and sort..." */
	syFieldScriptShowMessageThenWait(ACTOR_IVAN, 0, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	fdActorAnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
	/* "That's a sound plan, Ivan... I fully agree." */
	syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	syFieldScriptWait(60);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_GARET, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	syFieldScriptWait(60);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	syFieldScriptWait(30);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	if(!syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
		fdActorAnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
		syFieldScriptWait(10);
		/* "You're right. We can't fight right now. Let's head back to..." */
		syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
		fdActorAnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
		syFieldScriptWait(10);
	} else {
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		fdActorAnimateThenWait(ACTOR_JENNA, ANIM_SHAKE_HEAD, 33);
		FieldStateWork->msg = (FieldStateWork->msg + 2);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
		/* "He's right, Felix. It's time we explained ourselves to them.
		   We..." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
		syFieldScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
		syFieldScriptWait(5);
		/* "We'll be there, Isaac. I promise. I hope you can still trust me." */
		syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 0);
		fdActorAnimateThenWait(ACTOR_ISAAC, ANIM_NOD, 20);
		syFieldScriptWait(10);
		/* "I've never stopped trusting you, Jenna. I'll be waiting for you
		   in..." */
		syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	leader = fdActorGet(ACTOR_ISAAC);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_IVAN, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	leader = fdActorGet(ACTOR_ISAAC);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_MIA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	leader = fdActorGet(ACTOR_ISAAC);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_GARET, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetScript(ACTOR_IVAN, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_MIA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_GARET, JupiterLighthouseJoinLeaderScript);
	scFieldActorWaitForScriptEndById(ACTOR_IVAN);
	scFieldActorWaitForScriptEndById(ACTOR_MIA);
	scFieldActorWaitForScriptEndById(ACTOR_GARET);
	syScriptCameraFollowActor(ACTOR_ISAAC, 1);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, 0);
	scFieldActorWatchAndSetScript(ACTOR_PIERS, 0);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, 0);
	scFieldActorWatchAndSetScript(ACTOR_SHEBA, 0);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, 0);
	syFieldScriptWait(10);
	fdActorSetSpeed(ACTOR_ISAAC, 1.5);
	fdActorWalkToAndWait(ACTOR_ISAAC, 543, 363);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 2);
	syFieldScriptWait(5);
	/* Yes: "We'll be waiting for you... So don't think you can sneak off
	   again!"; No: "We'll be expecting you. Don't think you can sneak out of
	   this!..." */
	syFieldScriptShowMessageThenWait(ACTOR_ISAAC, 0, 0);
	fdActorWalkToAndWait(ACTOR_ISAAC, 573, 358);
	fdActorWalkToAndWait(ACTOR_ISAAC, 584, 300);
	fdActorWalkToAndWait(ACTOR_ISAAC, 584, 252);
	fdActorWalkToAndWait(ACTOR_ISAAC, 616, 247);
	fdActorWalkToAndWait(ACTOR_ISAAC, 617, 312);
	fdActorGet(ACTOR_ISAAC)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_ISAAC, JupiterLighthouseHopEastOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_ISAAC);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	syFieldScriptWait(30);
	fdActorGet(ACTOR_ISAAC)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	scJupiterLighthouseLowerElevator(DESCENT_ISAAC);
	syFieldScriptWait(100);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	scJupiterLighthouseRaiseElevator(ASCENT_RISING);
	syFieldScriptWait(60);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(60);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	syFieldScriptWait(60);
	fdActorAnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorAnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_WALK);
	leader = fdActorGet(ACTOR_FELIX);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_PIERS, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_WALK);
	leader = fdActorGet(ACTOR_FELIX);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_SHEBA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
	leader = fdActorGet(ACTOR_FELIX);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_JENNA, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	leader = fdActorGet(ACTOR_FELIX);
	if(leader != NULL) {
		fdActorSetDestination(
			ACTOR_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	fdActorSetScript(ACTOR_PIERS, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_SHEBA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_JENNA, JupiterLighthouseJoinLeaderScript);
	fdActorSetScript(ACTOR_KRADEN, JupiterLighthouseJoinLeaderScript);
	scFieldActorWaitForScriptEndById(ACTOR_PIERS);
	scFieldActorWaitForScriptEndById(ACTOR_SHEBA);
	scFieldActorWaitForScriptEndById(ACTOR_JENNA);
	scFieldActorWaitForScriptEndById(ACTOR_KRADEN);
	syScriptCameraFollowActor(ACTOR_FELIX, 1);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, 0);
	fdActorSetScript(ACTOR_FELIX, JupiterLighthouseWalkToPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	fdActorGet(ACTOR_FELIX)->priFlg |= ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_ALEX)->motionFlg = ACTOR_FALLS;
	fdActorSetScript(ACTOR_FELIX, JupiterLighthouseHopWestOverPlatformsScript);
	scFieldActorWaitForScriptEndById(ACTOR_FELIX);
	fdActorWalkToAndWait(ACTOR_FELIX, 311, 256);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
	syFieldScriptWait(10);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scJupiterLighthouseLowerElevator(DESCENT_FELIX);
	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrm = 24;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(
		JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Unused: writes to out the ids of the actors standing on the elevator, in
   columns 4 to 8 and the three rows from 72 less the row of its scroll z, as
   The Broken Seal's MercuryLighthouseAerieFindLiftRiders finds its lift's. */
UNUSED void scJupiterLighthouseFindElevatorRiders(Sint32 *out, Sint32 zArg)
{
	Sint32		 z = zArg;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint32		 slot;

	z = 64 - FIELD_METATILE(z);
	for(slot = 0; slot < FIELD_TREASURE_ACTOR; slot++) {
		ACTOR *actor = state->actors[slot];

		if(actor != NULL) {
			Sint32 column = FIELD_METATILE(actor->x.fixed);
			Sint32 row = FIELD_METATILE(actor->z.fixed);

			if(column >= 4 && column <= 8 && z + 8 <= row && row < z + 11)
				*out++ = slot;
		}
	}
}

/* Ends a list of the elevator's riders: the first id past the actors a scene
   places. */
#define ELEVATOR_RIDERS_END FIELD_TREASURE_ACTOR

extern Sint32 JupiterLighthouseFirstElevatorRiders[];
extern Sint32 JupiterLighthouseSecondElevatorRiders[];
extern Sint32 JupiterLighthouseThirdElevatorRiders[];
extern Sint32 JupiterLighthouseFourthElevatorRiders[];
extern Sint32 JupiterLighthouseFloatingPlatforms[];

/* The ticks the elevator takes to sink all the way, the tick of a ride at
   which the camera sets off after it, and the tick at which a rise and every
   descent but DESCENT_FELIX stop. */
#define ELEVATOR_DOWN_TICKS	 228
#define ELEVATOR_CAMERA_TICK 40
#define ELEVATOR_RIDE_TICKS	 120

/* The elevator, drawn on the map's overhead layer, sinks with the riders of its
   descent, speeding up a tenth of a pixel every fourth tick. */
void scJupiterLighthouseLowerElevator(Sint32 mode)
{
	FIXED	   speed;
	Uint32	   cnt;
	Sint32	  *list;
	Sint32	  *p;
	Sint32	  *q;
	Uint32	   i;
	Uint32	   j;
	MAP_LAYER *elevator;

	elevator =
		&((MAP_STATE *)RamHeapSlots->mapGrid)->layers[MAPGRID_LAYER_OVERHEAD];
	speed = toFIXED(1) / 10;
	cnt = 0;
	JupiterLighthouseElevatorLevel = elevator->ofsY;
	switch(mode) {
	case DESCENT_ALEX_AGATIO_KARST:
	case DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC:
		list = JupiterLighthouseFirstElevatorRiders;
		break;
	case DESCENT_ISAAC:
		list = JupiterLighthouseSecondElevatorRiders;
		break;
	case DESCENT_FELIX:
		list = JupiterLighthouseThirdElevatorRiders;
		break;
	default:
		list = JupiterLighthouseFourthElevatorRiders;
		break;
	}

	for(i = 0; i <= 4 && list[i] != ELEVATOR_RIDERS_END; i++) {
		fdActorGet(list[i])->motionFlg = 0;
		scFieldActorSetSprPriority(list[i], 1);
		cnt++;
	}

	for(i = 0; i <= 5; i++) {
		fdActorGet(JupiterLighthouseFloatingPlatforms[i])->priFlg |=
			ACTOR_PRIORITY_IGNORES_RISES;
		scFieldActorSetSprPriority(JupiterLighthouseFloatingPlatforms[i], 0);
	}

	fdScriptPlaySnd(SE_LIFT);

	for(i = 0; i < ELEVATOR_DOWN_TICKS; i++) {
		elevator->ofsY -= speed;
		for(j = 0; j < cnt; j++) {
			ACTOR *actor;

			fdActorGet(list[j])->z.fixed += speed;
			actor = fdActorGet(list[j]);
			actor->tgtZ = fdActorGet(list[j])->z.fixed;
		}
		if((i & 3) == 3)
			speed += toFIXED(1) / 10;
		if(speed > toFIXED(3) / 2 - 1)
			speed = toFIXED(3) / 2;
		syWait(1);
		if(mode == DESCENT_ALEX_AGATIO_KARST) {
			if(i == ELEVATOR_CAMERA_TICK)
				fdCameraMoveTo(512, 104, 304, 1);
			if(i == ELEVATOR_RIDE_TICKS)
				break;
		} else if(mode == DESCENT_ISAAC) {
			if(i == ELEVATOR_CAMERA_TICK)
				fdCameraMoveTo(512, 104, 352, 1);
			if(i == ELEVATOR_RIDE_TICKS)
				break;
		} else if(mode == DESCENT_ALEX_AGATIO_KARST_FOLLOWING_ISAAC) {
			if(i == ELEVATOR_CAMERA_TICK)
				syScriptCameraFollowActor(ACTOR_ISAAC, 1);
			if(i == ELEVATOR_RIDE_TICKS)
				break;
		}
	}

	for(i = 0; i <= 5; i++) {
		fdActorGet(JupiterLighthouseFloatingPlatforms[i])->priFlg &=
			~ACTOR_PRIORITY_IGNORES_RISES;
		scFieldActorSetSprPriority(JupiterLighthouseFloatingPlatforms[i], 1);
	}

	for(i = 0; i < cnt; i++) {
		fdActorSetPos(list[i], 0, 0);
		fdActorGet(list[i])->motionFlg = 0;
	}
}

extern Sint32 JupiterLighthouseRisingElevatorRiders[];

/* The rising elevator keeps (ELEVATOR_DRAG - 1) / ELEVATOR_DRAG of its speed
   every fourth tick once it is less than ELEVATOR_SLOWING_TICKS ticks'
   travel from its level. */
#define ELEVATOR_DRAG		   16
#define ELEVATOR_SLOWING_TICKS 30

/* The elevator rises back to where it was before it sank, slowing as it nears
   it, or with ASCENT_AT_ONCE is only put back. */
void scJupiterLighthouseRaiseElevator(Sint32 mode)
{
	MAP_LAYER *elevator =
		&((MAP_STATE *)RamHeapSlots->mapGrid)->layers[MAPGRID_LAYER_OVERHEAD];
	Sint32	speed = toFIXED(3) / 2;
	Uint32	cnt = 0;
	Uint32	i;
	Sint32 *ids = JupiterLighthouseRisingElevatorRiders;

	for(i = 0; i <= 4 && ids[i] != ELEVATOR_RIDERS_END; i++) {
		fdActorGet(ids[i])->motionFlg = 0;
		cnt++;
	}
	if(mode != ASCENT_AT_ONCE) {
		fdScriptPlaySnd(SE_LIFT);
		i = 0;
		elevator->ofsY += toFIXED(3) / 2;
		while(elevator->ofsY < JupiterLighthouseElevatorLevel) {
			Uint32 j;

			for(j = 0; j < cnt; j++) {
				ACTOR *actor;

				fdActorGet(ids[j])->z.fixed -= speed;
				actor = fdActorGet(ids[j]);
				actor->tgtZ = fdActorGet(ids[j])->z.fixed;
			}
			if((i & 3) == 3 &&
				JupiterLighthouseElevatorLevel <
					speed * ELEVATOR_SLOWING_TICKS + elevator->ofsY)
				speed = speed * (ELEVATOR_DRAG - 1) / ELEVATOR_DRAG;
			if(speed < toFIXED(1) / 20)
				speed = toFIXED(1) / 20;
			if(mode == ASCENT_RISING && i == ELEVATOR_CAMERA_TICK)
				fdCameraMoveTo(288, 104, 368, 1);
			syWait(1);
			i++;
			if(i >= ELEVATOR_RIDE_TICKS)
				break;
			elevator->ofsY += speed;
		}
	}
	elevator->ofsY = JupiterLighthouseElevatorLevel;
	fdMapGridCenterOnFollowedPoint();
	syWait(2);
}

/* Sets up the scene as the party enters: the floating platforms and the lid,
   the beacon's light, and the script of the entrance taken (ENTRANCE_BEACON_LIT
   to ENTRANCE_LAST_BATTLE_LOST). */
Sint32 scInit(void)
{

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	Call2(fdHangingVinePlace, ACTOR_WEST_CHAIN, 1);
	fdHangingVinePlace(ACTOR_EAST_CHAIN, 1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	fdActorGet(ACTOR_FLOATING_PLATFORM_A)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_B)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_C)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_D)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_E)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	fdActorGet(ACTOR_FLOATING_PLATFORM_F)->priFlg |= ACTOR_PRIORITY_PLATFORM;
	if(!fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT)) {
		scJupiterLighthousePlaceRaisedActors();
	} else {
		fdActorGet(ACTOR_UNSEEN_LID)->scaleX = 0;
	}
	if(MainRoutineStateWork.entrance !=
			JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A &&
		MainRoutineStateWork.entrance !=
			JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B) {
		fdActorSetPos(ACTOR_UNSEEN_LID, 0, 0);
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_BEACON_ORB), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BEACON_ORB), 0);
	sySetTickEvent(scJupiterLighthousePulseBeacon, EVENT_PASS_TICK);
	if(MainRoutineStateWork.entrance == ENTRANCE_BEACON_LIT) {
		scJupiterLighthouseBeaconIsLit();

		MainRoutineStateWork.retreatScene = SCENE_JUPITER_LIGHTHOUSE_ENTRANCE;
		MainRoutineStateWork.retreatEntrance =
			JUPITER_LIGHTHOUSE_ENTRANCE_FROM_WORLD_MAP;
	} else {
		if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_FIRST_BATTLE) {
			gmPartyAddMember(JENNA);
			scJupiterLighthouseJennaJoinsTheFight();
		} else {
			if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_SECOND_BATTLE) {
				gmPartyAddMember(SHEBA);
				scJupiterLighthouseShebaJoinsTheFight();
			} else {
				if(MainRoutineStateWork.entrance == ENTRANCE_LAST_BATTLE_WON) {
					scJupiterLighthouseWeBeatThem();
				} else {
					if(MainRoutineStateWork.entrance ==
						ENTRANCE_LAST_BATTLE_LOST) {
						scJupiterLighthouseKarstAndAgatioRetreat();
					} else {
						if(fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT)) {
							fdActorSetPos(ACTOR_AGATIO, 0, 0);
							fdActorSetPos(ACTOR_KARST, 0, 0);
						}
					}
				}
			}
		}
	}
	return 0;
}

/* Sets the scene's arrival flags before the map is built. Returns 0. */
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleTick;

/* Sets the palette cycle's tick (PaletteCycleTick). */
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

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/* Sets the priority the lit actors' sprites are drawn at. */
void scLightSprsSetPriority(Sint32 value)
{
	LightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const Sint32 *FieldEffectDurationScripts[];
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

void fdCrackedTileUpdate(void);

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);

#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flg, Sint32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

/* Who the elevator carries down, by scJupiterLighthouseLowerElevator's mode:
   Alex, Agatio and Karst; Isaac; Felix; no one. */
Sint32 JupiterLighthouseFirstElevatorRiders[] = {
	ACTOR_ALEX, ACTOR_AGATIO, ACTOR_KARST, ELEVATOR_RIDERS_END};
Sint32 JupiterLighthouseSecondElevatorRiders[] = {
	ACTOR_ISAAC, ELEVATOR_RIDERS_END};
Sint32 JupiterLighthouseThirdElevatorRiders[] = {
	ACTOR_FELIX, ELEVATOR_RIDERS_END};
Sint32 JupiterLighthouseFourthElevatorRiders[] = {ELEVATOR_RIDERS_END};
/* Who rides the elevator back up (scJupiterLighthouseRaiseElevator): no one. */
Sint32 JupiterLighthouseRisingElevatorRiders[] = {ELEVATOR_RIDERS_END};

/* clang-format off */
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

/* Unused: turns its actor's facing back and forth, 9/128 of a turn at a */
/* time, and ends. Nothing starts it. */
LATE_DATA Sint32 JupiterLighthouseRockScript[] = {
	SC_WAIT(5),
	SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
	SC_WAIT(5),
	SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
	SC_WAIT(5),
	SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
	SC_WAIT(2),
	SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
	SC_WAIT(2),
	SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
	SC_WAIT(5),
	SC_ADD(SW_FACING, -TRIG_TURN * 9 / 128),
	SC_WAIT(5),
	SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
	SC_WAIT(5),
	SC_ADD(SW_FACING, TRIG_TURN * 9 / 128),
	SC_END,
};

/* A party member walks into the leader it was sent to and is released there. */
LATE_DATA Sint32 JupiterLighthouseJoinLeaderScript[] = {
	SC_SYNC,
	SC_RELEASE,
};

/* Walks west round the well to the western floating platforms. */
LATE_DATA Sint32 JupiterLighthouseWalkToPlatformsScript[] = {
	SC_SPEED(1),
	SC_ACCELERATION(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(480, 72, 360),
	SC_SYNC,
	SC_MOVE(456, 72, 360),
	SC_SYNC,
	SC_MOVE(440, 72, 256),
	SC_SYNC,
	SC_MOVE(408, 72, 256),
	SC_SYNC,
	SC_MOVE(409, 32, 312),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Hops west over the three western floating platforms. */
LATE_DATA Sint32 JupiterLighthouseHopWestOverPlatformsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SPEED(3.5),
	SC_ACCELERATION(1),
	SC_MOVE(409, 32, 312),
	SC_SYNC,
	SC_WAIT(10),
	SC_MOVE(376, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(344, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_WAIT(10),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(312, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(312, 32, 278),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Hops east over the three eastern floating platforms. */
LATE_DATA Sint32 JupiterLighthouseHopEastOverPlatformsScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SPEED(3.5),
	SC_ACCELERATION(1),
	SC_MOVE(616, 32, 312),
	SC_SYNC,
	SC_WAIT(10),
	SC_MOVE(648, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(681, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_WAIT(10),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(712, 32, 312),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_WAIT(20),
	SC_MOVE(712, 32, 264),
	SC_SET(SW_MOTION, ACTOR_FALLS),
	SC_SET(SW_VELOCITY_Y, toFIXED(5)),
	SC_CALL(scJupiterLighthouseJumpWithoutShadow),
	SC_SYNC,
	SC_CALL(scJupiterLighthouseShowShadow),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The column of light widens by a sixteenth of its picture's width a tick */
/* for 32 ticks. Past its end are the scripts that narrow the column and */
/* lift it away, which nothing here runs. */
LATE_DATA Sint32 JupiterLighthouseBeamOpenScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(32, SC_TOP),
	SC_END,
	SC_ADD(SW_SCALE_X, -toFIXED(7) / 512),
	SC_WAIT(1),
	SC_LOOP(140, SC_TOP),
	SC_END,
	SC_ADD(SW_SCALE_X, -toFIXED(7) / 512),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_END,
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
	SC_ADD(SW_Y, toFIXED(2)),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

/* The lit beacon's orb breathes: narrower for four ticks, wider for four, */
/* again and again. */
LATE_DATA Sint32 JupiterLighthouseBeaconRisesScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 128),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 128),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* A spark shed by the rising orb (scJupiterLighthouseRaiseBeaconLight): it */
/* drops a pixel a tick for 16 ticks and is released. */
LATE_DATA Sint32 JupiterLighthouseBeaconSparkScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(0.8)),
	SC_SET(SW_SCALE_Y, toFIXED(0.8)),
	SC_ANIMATION(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, toFIXED(-1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseAerieBeacon),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ENTRANCE, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_AERIE_BEACON),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_WORLD_MAP, WorldMap, WORLD_MAP_JUPITER_BEACON),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_EAST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_C, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_WEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_AERIE),
	SCENE_EXITS_END,
};

LATE_DATA ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 472, 40, 256, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_STAND, 552, 40, 256, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 880, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_BASIN, FLAG_JUPITER_BEACON_LIT, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_QUARTER_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_QUARTER_DISC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_AGATIO, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 512, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KARST, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), MOVEMENT_TYPE_FACE_LEADER, 480, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, IF_FLAG_SET(FLAG_JUPITER_BEACON_LIT), JupiterLighthouseBeaconRisesScript, 512, 8, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* A particle of the column of light */
/* (scJupiterLighthouseSpawnBeaconParticle): */
/* it climbs a pixel a tick for 16 ticks and is released. Past its end are */
/* two more, which nothing here runs. */
LATE_DATA Sint32 JupiterLighthouseBeaconParticleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(0.8)),
	SC_SET(SW_SCALE_Y, toFIXED(0.8)),
	SC_ANIMATION(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, toFIXED(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
#if BEACON_PARTICLE_SPARE_WORD
	0,
#endif
	SC_SET(SW_SCALE_Y, toFIXED(1.4)),
	SC_SET(SW_SCALE_X, toFIXED(1.4)),
	SC_ANIMATION(4),
	SC_WAIT(30),
	SC_RELEASE,
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(8),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};
/* clang-format on */

void scJupiterLighthouseCastTheJupiterStar(void);

LATE_DATA CUE SceneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		JUPITER_LIGHTHOUSE_AERIE_BEACON_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B},
	{CUE_STEP, METATILE_BEFORE_KARST_AND_AGATIO,
		FLAG_KARST_AND_AGATIO_MET_FELIX,
		SCRIPT_REF(scJupiterLighthouseKarstAndAgatioWait)},
	{CUE_STEP, METATILE_BEFORE_THE_WELL, FLAG_JUPITER_BEACON_LIT,
		SCRIPT_REF(scJupiterLighthouseCastTheJupiterStar)},
	{CUE_TALK, ACTOR_KARST, CONDITION_ALWAYS, MSGNO(MsgOnceYouVe)},
	{CUE_TALK, ACTOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgOnceJupiterIs)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
		MSGNO(MsgOnceJupiterIsLit)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AGATIO, CONDITION_ALWAYS,
		MSGNO(MsgDoEvenThink)},
	{CUE_TABLE_END},
};

/* The aerie's six floating platforms. */
/* clang-format off */
LATE_DATA Sint32 JupiterLighthouseFloatingPlatforms[] = {
	ACTOR_FLOATING_PLATFORM_A, ACTOR_FLOATING_PLATFORM_B, ACTOR_FLOATING_PLATFORM_C,
	ACTOR_FLOATING_PLATFORM_D, ACTOR_FLOATING_PLATFORM_E, ACTOR_FLOATING_PLATFORM_F,
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
