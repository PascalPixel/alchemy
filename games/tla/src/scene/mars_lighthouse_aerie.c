/* Mars Lighthouse's aerie, at the top of the last lighthouse: the party faces
   the Doom Dragon (scMarsLighthouseAerieFaceDoomDragon) and learns who the
   dragon was (scMarsLighthouseAerieFindWhoTheDragonWas), the three it was speak
   to the party as they lie dying (scMarsLighthouseAerieTalkToJennasMother and
   the talks after it), and the beacon is lit with the Mars Star
   (scMarsLighthouseAerieLightBeacon) before the Golden Sun rises
   (scMarsLighthouseAerieWatchGoldenSunRise).

   The party climbs to the beacon in the storm with all eight and Kraden; a
   voice calls them betrayers and the Doom Dragon rises. Beaten, it is three
   people, Isaac's father and Felix and Jenna's parents, too weak to live long;
   Kraden holds the children back while the Mercury Adepts try to heal them.
   Then the voice asks for the Mars Star, the beacon is lit, and the party
   leaves. */

#include "graphics/palette_buffers.h"
#include "item_ids.h"
#include "field/field_mode.h"
#include "scene/scene.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "macros.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "field/field_script.h"
#include "edition.h"
#include "field/actor_script.h"
#include "scene/sparkle.h"
#include "window/window.h"
#include "text/msg_ids.h"

/* The voice has asked for the Mars Star; stepping back from the beacon asks
   again. */
#define FLAG_ASKED_FOR_MARS_STAR  FLAG_SCENE_TEMP_02
/* The beacon's light has broken through: its orbs rise every tick. */
#define FLAG_BEACON_BROKE_THROUGH FLAG_SCENE_TEMP_00

/* The aerie's actors, by the rows of its three actor placement tables; the
   party's rows go to their party slots. Ids 8 to 16 and Kraden's are the same
   in all three. */
enum {
	/* The beacon's fire and its glow: SPRITE_BLUE_ORB on the beacon's table,
	   placed over the beacon once FLAG_MARS_BEACON_BURNING is set. */
	ACTOR_BEACON_FIRE = 8,
	ACTOR_BEACON_GLOW,
	/* Seven platforms (SPRITE_FLOATING_PLATFORM) along the aerie's east edge,
	   floating before the battle. */
	ACTOR_FIRST_PLATFORM,
	ACTOR_LAST_PLATFORM = ACTOR_FIRST_PLATFORM + 6,
	/* Before the battle (MarsLighthouseAeriePlacements) and after it: the Doom
	   Dragon and the Wise One, the voice that calls the party betrayers. */
	ACTOR_DOOM_DRAGON,
	ACTOR_WISE_ONE,
	ACTOR_KRADEN
};

/* The metatile behaviours the aerie's map marks its spots with, which its cue
   table answers. */
typedef enum MARS_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR {
	/* The landing the party reaches the aerie on, where Teleport takes them
	   back down. */
	METATILE_TELEPORT_LANDING = 1,
	/* The spot before the unlit beacon, where the voice speaks before the
	   battle. */
	METATILE_BEFORE_BEACON = 10,
	/* Where Kraden stops Felix, west and east of the three as they lie
	   dying. */
	METATILE_KRADEN_STOPS_WEST = 20,
	METATILE_KRADEN_STOPS_EAST = 21,
	/* At the beacon, where the voice asks for the Mars Star, and the ring of
	   metatiles round it that let it ask again. */
	METATILE_AT_BEACON = 22,
	METATILE_ROUND_BEACON = 23
} MARS_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR;

/* The entrances the aerie's own scripts arrive by. */
enum {
	ENTRANCE_DRAGON_BEATEN = 2, /* after the Doom Dragon's battle */
	/* Straight to the beacon's lighting; no exit leads here. */
	ENTRANCE_LIGHT_BEACON = 4
};

/* The exit that takes the party, faded to black, from the fallen dragon to the
   beacon (MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE). */
#define EXIT_TO_THE_BEACON 2

/* After the battle (MarsLighthouseAerieAfterBattlePlacements): the party but
   the leader stand in on their smaller sprites (SPRITE_JENNA_SMALL and the
   rest), the dragon's three heads fall, and under each lies one of the three it
   was. */
enum {
	ACTOR_JENNA_STAND_IN = 20,
	ACTOR_SHEBA_STAND_IN,
	ACTOR_PIERS_STAND_IN,
	ACTOR_ISAAC_STAND_IN,
	ACTOR_GARET_STAND_IN,
	ACTOR_MIA_STAND_IN,
	ACTOR_IVAN_STAND_IN,
	ACTOR_DRAGON_HEAD_A,
	ACTOR_DRAGON_HEAD_B,
	ACTOR_DRAGON_HEAD_C,
	ACTOR_FREED_JENNAS_MOTHER,
	ACTOR_FREED_JENNAS_FATHER,
	ACTOR_FREED_KYLE
};

/* At the beacon (MarsLighthouseAerieBeaconPlacements), entered from
   MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE on:
   the three lie dying, and an unseen actor beside each answers for them when
   talked to. */
enum {
	ACTOR_JENNAS_MOTHER_TALK_SPOT = 17,
	/* Unseen: "Will you cast in the Mars Star?" */
	ACTOR_VOICE,
	ACTOR_JENNAS_MOTHER = 20, /* "I...can't see you, but I sense..." */
	ACTOR_JENNAS_FATHER,	  /* "I am so happy...to have one last chance..." */
	ACTOR_KYLE,				  /* "Tell Dora...to live her life..." */
	/* The beacon's beam, in five segments (SPRITE_PIPE_NORTH_SOUTH) from the
	   bottom up, as at Venus Lighthouse's aerie. */
	ACTOR_BEAM_BOTTOM,
	ACTOR_BEAM_LOWER,
	ACTOR_BEAM_MIDDLE,
	ACTOR_BEAM_UPPER,
	ACTOR_BEAM_TOP,
	/* Hamma, who speaks to the party from afar as the Golden Sun rises; until
	   then her actor stands unseen beside Ivan as his talk spot. */
	ACTOR_HAMMA,
	ACTOR_JENNAS_FATHER_TALK_SPOT,
	ACTOR_KYLE_TALK_SPOT
};
struct CUE;

void scMarsLighthouseAerieLookAround(ACTOR *actor);
void scMarsLighthouseAerieStartTalkWhileHalted(Sint32 id);
void scMarsLighthouseAerieEndTalkWhileHalted(Sint32 id);
void scTeleportStartShimmer(void);
void scTeleportStopShimmer(void);
void fdPaletteCycleFade(void);
void fdTeleportPlaySparks(void);

extern Uint8	   MsgUhhhhnnnICan[];
extern Uint8	   MsgKoffKoffYou[];
extern Uint8	   MsgUnnnnIAm[];
extern Uint8	   MsgIUnderstandYour[];
extern ACTOR_PLACE MarsLighthouseAeriePlacements[];
extern ACTOR_PLACE MarsLighthouseAerieAfterBattlePlacements[];
extern ACTOR_PLACE MarsLighthouseAerieBeaconPlacements[];
Uint32 MarsLighthouseAerieBeaconFlameTick			   BSS_DATA;
Uint32 MarsLighthouseAerieFlickerTick				   BSS_DATA;
Uint32 MarsLighthouseAerieBeaconFireTicks			   BSS_DATA;
Uint32 MarsLighthouseAerieFlickerRate				   BSS_DATA;
SPARKLE_STATE MarsLighthouseAerieRisingRingsB BSS_DATA ALIGNED(16);
SPARKLE_STATE MarsLighthouseAerieRisingRingsA BSS_DATA ALIGNED(16);
Uint32 MarsLighthouseAerieBeaconFireTick			   BSS_DATA;
/* The teleport circle's shimmer, a palette cycle that fdPaletteCycleFade
   steps. */
Uint16 PaletteCycleBright		  BSS_DATA;
Uint16 *PaletteCycleSlots		  BSS_DATA;
Uint16 PaletteCycleTimer		  BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA ALIGNED(4);
Uint16 *PaletteSlots			  BSS_DATA;
Uint16 *PaletteColors			  BSS_DATA;
Uint16 *PaletteCycleColors		  BSS_DATA;
Uint16 PaletteCycleLength		  BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA ALIGNED(4);

void scMarsLighthouseAerieHaltSpeaker(Sint32 spot, Sint32 speaker);
void scMarsLighthouseAerieStartRisingRings(ACTOR *actor, SPARKLE_STATE *state);

extern Uint8 MsgMarsLighthouseAerieDoNotLetYourSadness[];

Sint32 syListCountEntries(Uint16 *list);

/* Each tick while waiting in the storm: now and then the actor looks another
   way. */
void scMarsLighthouseAerieLookAround(ACTOR *actor)
{
	Uint8 *tmr = &actor->sceneCnt;

	if(*tmr != 0) {
		(*tmr)--;
	} else {
		*tmr = (syRandomBelow(80)) + 60;
		actor->facing = syRandom();
	}
}

/* The actor flickers: hidden for two ticks, shown for two. */
void scMarsLighthouseAerieFlicker(ACTOR *actor)
{
	if((TickCount >> 1) & 1)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
}

/* The actor swells now and then and shrinks back, as if breathing. */
void scMarsLighthouseAerieThrob(ACTOR *actor)
{
	Uint8 *tmr;
	Sint32 scale = actor->scaleX;

	if(scale > toFIXED(1)) {
		actor->scaleX = scale - toFIXED(1) / 32;
		actor->scaleY -= toFIXED(1) / 64;
	}
	tmr = &actor->sceneCnt;
	if(*tmr != 0) {
		(*tmr)--;
	} else {
		*tmr = *tmr + syRandomBelow(80) + 40;
		actor->scaleX = toFIXED(9) / 8;
		actor->scaleY = toFIXED(17) / 16;
	}
}

/* A spark of the beacon's fire circles it as it rises. */
void scMarsLighthouseAerieOrbitBeaconFire(ACTOR *actor)
{
	ACTOR  *anchor = fdActorGetById(ACTOR_BEACON_FIRE);
	Uint16 *pangle = (Uint16 *)&actor->tmr;
	Sint32	ang = *pangle;
	FIXED	cosine;
	FIXED	sine;
	FIXED	along;
	FIXED	across;
	Uint8  *counter;

	cosine = syCos(ang);
	counter = &actor->sceneCnt;
	along = anchor->x.fixed + cosine * (actor->speedLimit + *counter + 6);
	actor->x.fixed = along;
	sine = syMathSin(ang);
	across = anchor->z.fixed + sine * (*counter + 4);
	actor->z.fixed = across;
	actor->tgtX = actor->x.fixed;
	actor->tgtZ = across;
	*pangle -= TRIG_TURN / 32;
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
	if(MainRoutineStateWork.entrance == ENTRANCE_DRAGON_BEATEN)
		return MarsLighthouseAerieAfterBattlePlacements;
	/* From the fallen dragon, for the beacon's lighting, or back from Mercury
	   Lighthouse: the three entrances that arrive at the beacon. */
	if(MainRoutineStateWork.entrance >= 3 && MainRoutineStateWork.entrance <= 5)
		return MarsLighthouseAerieBeaconPlacements;
	return MarsLighthouseAeriePlacements;
}

/* Teleport takes the party back down to the inscriptions room, in a shimmer of
   sparks. */
void scLeaveByExit(void)
{
	scTeleportStartShimmer();
	fdTeleportPlaySparks();
	MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
	syFieldScriptSetExitRequest(MARS_LIGHTHOUSE_AERIE_TO_MARS_LIGHTHOUSE_HALL);
	syFieldScriptWait(40);
	scTeleportStopShimmer();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

extern Uint16 TeleportCirclePaletteSlots[];
extern Uint16 TeleportCircleColors[];
extern Uint16 TeleportCircleShimmerColors[];
extern Uint16 TeleportCircleShimmerSlots[];

void scPaletteSetColsFade(Uint16 *slots, Uint16 *colours, Uint16 *a, Uint16 *b);
void scMarsLighthouseAerieSpawnBeaconFlames(void);

/* The beacon's colours and its fire, burning once the beacon is lit. */
void scMarsLighthouseAerieSetUpBeacon(void)
{
	ACTOR *actor;
	Sint32 i;

	scPaletteSetColsFade(TeleportCirclePaletteSlots, TeleportCircleColors,
		TeleportCircleShimmerColors, TeleportCircleShimmerSlots);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	for(i = ACTOR_FIRST_PLATFORM; i <= ACTOR_LAST_PLATFORM; i++) {
		actor = fdActorGet(i);
		actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
		fdActorSetSprColMode(actor, SPRITE_COLOR_BRG);
	}
	if(!fdScriptGetFlag(FLAG_MARS_BEACON_BURNING))
		return;
	fdActorSetPos(ACTOR_BEACON_FIRE, 512, 264);
	fdActorSetPos(ACTOR_BEACON_GLOW, 512, 271);
	syWait(1);
	scFieldActorSetSprPriority(ACTOR_BEACON_FIRE, 1);
	actor = fdActorGet(ACTOR_BEACON_FIRE);
	actor->motionFlg = ACTOR_FLOATS;
	actor->y.fixed = toFIXED(112);
	fdActorSetSprColMode(actor, SPRITE_COLOR_BRG);
	fdActorSetSprFlags(actor, 0);
	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	scFieldActorSetSprPriority(ACTOR_BEACON_GLOW, 2);
	actor = fdActorGet(ACTOR_BEACON_GLOW);
	actor->motionFlg = ACTOR_FLOATS;
	actor->y.fixed = toFIXED(112);
	actor->scaleX = toFIXED(13) / 10;
	actor->scaleY = toFIXED(13) / 10;
	fdActorSetSprColMode(actor, SPRITE_COLOR_BRG);
	fdActorSetSprFlags(actor, 0);
	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	MarsLighthouseAerieBeaconFlameTick = 0;
	syFieldSetTickEvent(
		scMarsLighthouseAerieSpawnBeaconFlames, EVENT_PASS_TICK);
}

/* Sets the scene up for the entrance the party arrived by: the Golden Sun
   rising, the beacon's lighting, the three at the beacon, or the fallen
   dragon. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scMarsLighthouseAerieSetUpBeacon();
	fdScriptSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
	if(MainRoutineStateWork.entrance ==
		MARS_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EAST_B) {
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		scMarsLighthouseAerieWatchGoldenSunRise();
	} else if(MainRoutineStateWork.entrance == ENTRANCE_LIGHT_BEACON) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdActorSetAnimation(ACTOR_JENNAS_MOTHER, 8);
		fdActorSetAnimation(ACTOR_JENNAS_FATHER, 8);
		fdActorSetAnimation(ACTOR_KYLE, 7);
		fdActorSetPos(ACTOR_KRADEN, 498, 320);
		fdActorSetPos(ACTOR_GARET, 538, 320);
		fdActorSetPos(MainRoutineStateWork.leaderActorId, 506, 280);
		syWait(1);
		fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		syScriptCameraGetActor()->motionFlg = 0;
		fdCameraMoveTo(512, 96, 312, 1);
		fdMapGridCenterOnFollowedPoint();
		syWait(1);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		syFieldScriptWait(40);
		scMarsLighthouseAerieLightBeacon();
		fdScriptEnd();
	} else if(MainRoutineStateWork.entrance ==
		MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE) {
		fdActorSetAnimation(ACTOR_JENNAS_MOTHER, 8);
		fdActorSetAnimation(ACTOR_JENNAS_FATHER, 8);
		fdActorSetAnimation(ACTOR_KYLE, 7);
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT) == 0)
			scMarsLighthouseAerieUrgeOnToBeacon();
	} else if(MainRoutineStateWork.entrance == ENTRANCE_DRAGON_BEATEN) {
		scMarsLighthouseAerieFindWhoTheDragonWas();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.entrance ==
			MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_HALL &&
		fdScriptGetFlag(FLAG_MARS_BEACON_LIT) == 0)
		fdWeatherStartRain(1);
	fdMapCopyPalette(MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
		(Sint32)BG_PLTT);
	return 0;
}

extern Sint32 MarsLighthouseAerieBeaconFlameScript[];

void scMarsLighthouseAerieSwingBeaconFlame(ACTOR *actor);

/* A flame swings round the beacon's fire BEACON_FLAME_SWING_RADIUS pixels out,
   and up to BEACON_FLAME_SWING_SPREAD farther, by the height it rose from. */
#define BEACON_FLAME_SWING_RADIUS 28
#define BEACON_FLAME_SWING_SPREAD 24

/* Every sixteenth tick a flame rises from the beacon's fire. */
void scMarsLighthouseAerieSpawnBeaconFlames(void)
{
	ACTOR	  *anchor = fdActorGet(ACTOR_BEACON_FIRE);
	FIXED	   offset = toFIXED(syRandomBelow(48));
	ACTOR	  *actor;
	FIELD_SPR *spr;
	Uint32	   phase;
	Sint32	   ang;

	if(anchor != NULL &&
		(phase = MarsLighthouseAerieBeaconFlameTick & 15) == 0) {
		actor =
			scFieldActorCreate(SPRITE_PARTICLE, anchor->x.fixed + toFIXED(8),
				anchor->y.fixed + offset + toFIXED(8), anchor->z.fixed);
		offset = INTtoFIXED(offset / toFIXED(6));
		if(actor != NULL) {

			spr = actor->spr;
			fdActorStartScript(actor, MarsLighthouseAerieBeaconFlameScript);
			scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
			actor->motionFlg = phase;
			ang = syRandom() & 0xffff000;
			actor->tmr = ang;
			actor->userValue = phase;
			actor->update = scMarsLighthouseAerieSwingBeaconFlame;
			actor->watchedActor = (union FIELD_OBJECT *)anchor;
			actor->speedLimit = FIXEDtoINT(
				syMathSin((offset & 0xfffff) >> 4) * BEACON_FLAME_SWING_SPREAD);
			spr->flg = 0;
			spr->pri = ((FIELD_SPR *)anchor->spr)->pri;
		}
	}
	MarsLighthouseAerieBeaconFlameTick++;
}

/* A flame swings round the beacon's fire, behind it and then in front. */
void scMarsLighthouseAerieSwingBeaconFlame(ACTOR *actor)
{
	Uint16 *pangle = (Uint16 *)&actor->tmr;
	Uint16	ang = *pangle;
	ACTOR  *parent = (ACTOR *)actor->watchedActor;

	actor->x.fixed = parent->x.fixed +
		syCos(ang) * (actor->speedLimit + BEACON_FLAME_SWING_RADIUS);
	actor->z.fixed = (syMathSin(ang) << 4) + toFIXED(264);
	actor->tgtX = actor->x.fixed;
	actor->tgtZ = actor->z.fixed;
	if(actor->z.fixed < toFIXED(264))
		((FIELD_SPR *)actor->spr)->pri = 2;
	else
		((FIELD_SPR *)actor->spr)->pri = 1;
	*pangle -= TRIG_TURN / 128;
}

/* The actor stands looking about. */
void scMarsLighthouseAerieStartLookingAround(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	scFieldActorStop(id);
	actor->sceneCnt = (syRandomBelow(40)) + 20;
	actor->update = (OBJECT_UPDATE)scMarsLighthouseAerieLookAround;
}

/* The actor stops looking about and faces the beacon. */
void scMarsLighthouseAerieStopLookingAround(Sint32 id)
{
	fdActorGet(id)->update = NULL;
	fdActorFaceDirection(id, FACING_NORTH, 0);
}

extern Sint32 MarsLighthouseAerieJennaApproachScript[];
extern Sint32 MarsLighthouseAerieShebaApproachScript[];
extern Sint32 MarsLighthouseAeriePiersApproachScript[];
extern Sint32 MarsLighthouseAerieIvanApproachScript[];
extern Sint32 MarsLighthouseAerieKradenApproachScript[];
extern Sint32 MarsLighthouseAerieVoiceFadeScript[];
extern Sint32 MarsLighthouseAerieDoomDragonRiseScript[];
extern Sint32 MarsLighthouseAerieLeaderReadyScript[];
extern Sint32 MarsLighthouseAerieIsaacReadyScript[];
extern Sint32 MarsLighthouseAerieJennaReadyScript[];
extern Sint32 MarsLighthouseAerieShebaReadyScript[];
extern Sint32 MarsLighthouseAeriePiersReadyScript[];
extern Sint32 MarsLighthouseAerieIvanReadyScript[];
extern Sint32 MarsLighthouseAerieMiaReadyScript[];
extern Sint32 MarsLighthouseAerieGaretReadyScript[];

/* The party walks up to the unlit beacon in the storm (metatiles (32, 20) to
   (34, 20)), and the voice calls them betrayers before the Doom Dragon's
   battle. */
void scMarsLighthouseAerieFaceDoomDragon(void)
{
	MAIN_ROUTINE_STATE *state;
	Uint16			   *textCol;
	ACTOR			   *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	textCol = (Uint16 *)&CENTERED_TEXT_COLOR;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorSetSprCol(fdActorGet(ACTOR_WISE_ONE), SPRITE_COLOR_HIDDEN);
	fdActorSetPos(ACTOR_WISE_ONE, 512, 276);
	syWait(1);
	scFieldActorSetSprPriority(ACTOR_SHEBA, 1);
	scFieldActorSetSprPriority(ACTOR_JENNA, 1);
	scFieldActorSetSprPriority(ACTOR_KRADEN, 1);
	scFieldActorSetSprPriority(ACTOR_IVAN, 1);
	scFieldActorSetSprPriority(ACTOR_MIA, 1);
	fdScriptPlaySnd(MUS_SILENCE);
	syFieldScriptSetMessageId(
		(Sint32)MsgMarsLighthouseAerieBetrayersYouHaveArrived);
	/* "Betrayers, you have arrived!" */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	syScriptCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(512, CAMERA_KEEP, 304, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 504, 320);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorPlaceOnActor(ACTOR_ISAAC, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorSetSpeed(ACTOR_ISAAC, 1.6);
	fdActorWalkToAndWait(ACTOR_ISAAC, 520, 320);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorPlaceOnActor(ACTOR_GARET, ACTOR_ISAAC);
	syWait(1);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkToAndWait(ACTOR_GARET, 536, 320);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	/* "Who said that!?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorPlaceOnActor(ACTOR_MIA, ACTOR_GARET);
	syWait(1);
	fdActorSetSpeed(ACTOR_MIA, 1.6);
	fdActorWalkToAndWait(ACTOR_MIA, 544, 328);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_NORTHWEST);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "The wind's too strong! I can't see anything!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	scFieldActorStop(MainRoutineStateWork.leaderActorId);
	actor->sceneCnt = (syRandomBelow(20)) + 20;
	actor->update = (OBJECT_UPDATE)scMarsLighthouseAerieLookAround;
	scMarsLighthouseAerieStartLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStartLookingAround(ACTOR_MIA);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_IVAN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorSetSpeed(ACTOR_IVAN, 1.6);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetScript(ACTOR_JENNA, MarsLighthouseAerieJennaApproachScript);
	fdActorSetScript(ACTOR_SHEBA, MarsLighthouseAerieShebaApproachScript);
	fdActorSetScript(ACTOR_PIERS, MarsLighthouseAeriePiersApproachScript);
	fdActorSetScript(ACTOR_KRADEN, MarsLighthouseAerieKradenApproachScript);
	fdActorRunScriptAndWait(ACTOR_IVAN, MarsLighthouseAerieIvanApproachScript);
	syFieldScriptWait(10);
	fdActorReleaseById(ACTOR_LAST_PLATFORM);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM + 5);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM + 4);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM + 3);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM + 2);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM + 1);
	fdActorReleaseById(ACTOR_FIRST_PLATFORM);
	scMarsLighthouseAerieStartLookingAround(ACTOR_JENNA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_SHEBA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_PIERS);
	scMarsLighthouseAerieStartLookingAround(ACTOR_IVAN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_KRADEN);
	syFieldScriptWait(80);
	fdActorGet(ACTOR_SHEBA)->update = NULL;
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	/* "I heard it too, but I don't see anyone else up here!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorGet(ACTOR_JENNA)->update = NULL;
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
	scFieldActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	fdActorGet(ACTOR_PIERS)->update = NULL;
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	scFieldActorJump(ACTOR_PIERS, 4, 10);
	/* "Felix! Quick! Cast the Mars Star in before anything has a chance
	   to..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->update = NULL;
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_WEST);
	scFieldActorSetAnimationAndWait(
		MainRoutineStateWork.leaderActorId, ANIM_NOD);
	scMarsLighthouseAerieStopLookingAround(ACTOR_ISAAC);
	scMarsLighthouseAerieStopLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStopLookingAround(ACTOR_KRADEN);
	scMarsLighthouseAerieStopLookingAround(ACTOR_IVAN);
	scMarsLighthouseAerieStopLookingAround(ACTOR_MIA);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(40);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 504, 292);
	fdScriptPlaySnd(SE_WAVE);
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.2);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 504, 308);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 3);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 504, 320);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	*textCol = RGB_WHITE;
	syWait(1);
	/* "So, you are still intent on lighting the beacon of Mars Lighthouse?" */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	fdActorWalkToAndWait(ACTOR_ISAAC, 520, 308);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	/* "Who said that!?" */
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 20);
	/* "Have you forgotten me so soon, Isaac? And you, Garet?" */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
	/* "It seems to know you, Isaac... Do you have any idea what it is?" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
	scFieldActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdScriptPlaySnd(MUS_CONFRONTATION);
	/* "Then search your heart, boy!" */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTH);
	scMarsLighthouseAerieStartLookingAround(MainRoutineStateWork.leaderActorId);
	scMarsLighthouseAerieStartLookingAround(ACTOR_JENNA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_SHEBA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_PIERS);
	scMarsLighthouseAerieStartLookingAround(ACTOR_ISAAC);
	scMarsLighthouseAerieStartLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStartLookingAround(ACTOR_MIA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_IVAN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_KRADEN);
	syFieldScriptWait(40);
	scMarsLighthouseAerieStopLookingAround(ACTOR_KRADEN);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scFieldActorJump(ACTOR_KRADEN, 4, 10);
	scFieldActorJump(ACTOR_KRADEN, 6, 10);
	/* "That voice..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scMarsLighthouseAerieStopLookingAround(MainRoutineStateWork.leaderActorId);
	scMarsLighthouseAerieStopLookingAround(ACTOR_JENNA);
	scMarsLighthouseAerieStopLookingAround(ACTOR_SHEBA);
	scMarsLighthouseAerieStopLookingAround(ACTOR_PIERS);
	scMarsLighthouseAerieStopLookingAround(ACTOR_ISAAC);
	scMarsLighthouseAerieStopLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStopLookingAround(ACTOR_MIA);
	scMarsLighthouseAerieStopLookingAround(ACTOR_IVAN);
	scFieldActorSetSprCol(fdActorGet(ACTOR_WISE_ONE), SPRITE_COLOR_NORMAL);
	actor = fdActorGet(ACTOR_WISE_ONE);
	actor->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		fdActorGet(ACTOR_WISE_ONE), toFIXED(512), toFIXED(120), toFIXED(208));
	syFieldScriptWait(20);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(512, CAMERA_KEEP, 216, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	/* "It's...the Wise One!" */
	syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
	fdCameraMoveTo(512, CAMERA_KEEP, 304, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_ISAAC, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Isaac, since when are you on a first-name basis with the Wise One?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
	/* "It must have been... Isaac, what happened in Sol Sanctum after we..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	/* "When Saturos and Menardi stole the Elemental Stars, they also..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "But...wait, Mt. Aleph did erupt! There was a huge explosion! We
	   saw..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	/* "But it would have erupted with us still inside... There was no
	   way..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	/* "The Wise One held off the eruption until we could escape... He
	   even..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	/* "I can't believe it... Nobody has enough power to do that..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
	/* "I know it sounds weird, but I was there, and it happened, so
	   you'll..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	/* "If it can do all that, this Wise One seems more like a god than
	   an..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(512, CAMERA_KEEP, 296, 1);
	fdActorSetSpeed(ACTOR_WISE_ONE, 0.8);
	scFieldActorSetMoveTarget(actor, 512, 96, 276);
	scFieldActorWaitForMoveById(ACTOR_WISE_ONE);
	syFieldScriptWait(10);
	/* "I did not just save you. I also tasked you with recovering the
	   four..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorRunRepeatedMotion(ACTOR_ISAAC, 1);
	fdActorWalkToAndWait(ACTOR_ISAAC, 520, 320);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 9);
	syFieldScriptWait(20);
	/* "Why have you disobeyed my command? Why have you come to light the..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	/* "Because Prox will be destroyed if we don't! We can't let that
	   happen..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(10);
	/* "Prox? They have brought this disaster upon themselves." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Are you saying we should just abandon them to die? What did they
	   do..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 9);
	syFieldScriptWait(20);
	/* "The people of Prox have committed an unforgivable sin. They must
	   pay..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	/* "For lighting the lighthouses? Is that their sin? Does that
	   warrant..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	/* "If we don't light the beacon, Gaia Falls will eventually erode
	   away..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	/* "We have fought for so long to save all the people of our world,
	   and..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	scFieldActorJump(ACTOR_SHEBA, 4, 10);
	/* "Gaia Falls is growing. It's consuming more and more with each..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "How can you just allow the world to crumble into nothingness?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 11);
	syFieldScriptWait(20);
	/* "The seal needs to be broken! The world will be destroyed if it's
	   not!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 12);
	fdActorWaitForAnimationChange(ACTOR_WISE_ONE);
	syFieldScriptWait(20);
	/* "You have learned far too much." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Wise One! You can't continue to protect the lighthouses! You know..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 10);
	syFieldScriptWait(20);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(10);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 10);
	syFieldScriptWait(5);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(20);
	/* "It is your duty to protect all of Weyard! If Weyard is destroyed..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "Why won't you answer us, Wise One?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 9);
	syFieldScriptWait(20);
	/* "If Alchemy is unleashed, mankind may well destroy all of Weyard
	   itself." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "But we can combine our strengths, ensure that Alchemy not be used..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 10);
	syFieldScriptWait(20);
	/* "It is inevitable. In time, one man will seek to rule over all. It
	   is..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 9);
	syFieldScriptWait(20);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 3);
	syFieldScriptWait(10);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 6);
	syFieldScriptWait(10);
	scMarsLighthouseAerieStartRisingRings(
		actor, &MarsLighthouseAerieRisingRingsA);
	syFieldScriptWait(80);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 3);
	syFieldScriptWait(10);
	/* "And it shall come sooner than any of you think." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Why do you say that?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 3);
	syFieldScriptWait(20);
	/* "The Water Adept who climbs toward the peak of Mt. Aleph even as
	   we..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 20);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 6);
	syFieldScriptWait(20);
	fdScriptPlaySnd(SE_SWIRL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
	fdPaletteFadeInterpolate(40);
	syFieldScriptWait(40);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeInterpolate(40);
	syFieldScriptWait(40);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 3);
	syFieldScriptWait(20);
	/* "Alex is his name. Surely, you have not forgotten him?" */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	/* "Alex!? What would he be doing on Mt. Aleph?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 4);
	syFieldScriptWait(20);
	/* "He understands far more than you do. He knows that when the four..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "But what would he gain from being there?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	syFieldScriptWait(20);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 13);
	syFieldScriptWait(20);
	/* "When the final beam of light reaches the peak of Mt. Aleph, the..." */
	syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 20);
	/* "The Golden Sun!? What is that? And what would Alex want with it?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 8);
	syFieldScriptWait(180);
	/* "When the four beams merge into one, they form a golden light..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	/* "Is... Is that Alchemy? I mean, pure Alchemy made real, at the
	   heart..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(5);
	fdActorSetAnimation(ACTOR_WISE_ONE, 9);
	fdActorShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "And it's that light that gives shape to the Stone of Sages?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scMarsLighthouseAerieEndRisingRings(actor);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(20);
	/* "This has been Alex's one true desire from the very start." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	scFieldActorJump(ACTOR_PIERS, 4, 10);
	/* "Alex planned all of this? Then he must have been after this power..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 10);
	syFieldScriptWait(20);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 11);
	syFieldScriptWait(5);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 1);
	syFieldScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHEAST);
	/* "We've been duped! He used us all! Oh, you'd better believe he's
	   not..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "Alex... How could he do this? He's... He's one of my own people!
	   I..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
	/* "None of that matters right now. We still have to light the
	   beacon...." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
	/* "If we don't do it now, Prox will be destroyed! Felix... We have
	   to..." */
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0)) {
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
		scFieldActorJump(ACTOR_SHEBA, 4, 10);
		/* "You bet we do! The Wise One will have to deal with Alex on his
		   own." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
		FieldStateWork->msg++;
	} else {
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
		scFieldActorJump(ACTOR_SHEBA, 4, 10);
		FieldStateWork->msg++;
		/* "Felix! We don't have any choice! The Wise One will have to deal
		   with..." */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
	}
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 11);
	syFieldScriptWait(20);
	/* "I cannot interfere in the actions of mankind." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
	/* "If you can't interfere, then how about getting out of our way so
	   we..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimation(ACTOR_WISE_ONE, 8);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorWaitForAnimationChange(ACTOR_WISE_ONE);
	fdActorSetAnimation(ACTOR_WISE_ONE, 1);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "Ooo... Good one, Jenna!" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
	/* "I don't like doing exactly what Alex wants, but it's looking like..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
	/* "Don't worry, Felix! Just throw the Mars Star into the beacon's well!
	   Now!" */
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0)) {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		FieldStateWork->msg++;
	} else {
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		/* "The Wise One himself said he's not allowed to interfere! You're
		   all..." */
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	}
	state = &MainRoutineStateWork;
	fdActorFaceDirectionAndWait(state->leaderActorId, FACING_EAST);
	scFieldActorSetAnimationAndWait(state->leaderActorId, 3);
	fdActorFaceDirectionAndWait(state->leaderActorId, FACING_NORTH);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorSetSpeed(state->leaderActorId, 0.4);
	fdActorWalkToAndWait(state->leaderActorId, 504, 308);
	fdScriptPlaySnd(SE_ASCEND);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 14);
	syFieldScriptWait(10);
	fdActorSetSprColById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
	fdActorRunRepeatedMotion(state->leaderActorId, 2);
	fdActorSetSpeed(state->leaderActorId, 1.6);
	scFieldActorJump(state->leaderActorId, 4, 0);
	fdActorGet(state->leaderActorId)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(state->leaderActorId, 504, 320);
	syFieldScriptWait(1);
	fdActorGet(state->leaderActorId)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Wise One! Didn't you just say that you aren't allowed to
	   interfere..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 14);
	syFieldScriptWait(10);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 10);
	syFieldScriptWait(20);
	/* "That is correct. I cannot stop you." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	scFieldActorSetAnimation(ACTOR_WISE_ONE, 9);
	syFieldScriptWait(20);
	/* "But...what if some miracle were to occur, one that prevented you
	   from..." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(state->leaderActorId, FACING_WEST, 40);
	/* "Miracle? What are you talking about? What kind of miracle?" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdScriptPlaySnd(SE_CRUMBLE);
	fdActorSetSprColById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
	scFieldActorSetSprCol(fdActorGet(ACTOR_WISE_ONE), SPRITE_COLOR_WHITE);
	syFieldScriptWait(20);
	scFieldActorSetSprCol(fdActorGet(ACTOR_WISE_ONE), SPRITE_COLOR_NORMAL);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(state->leaderActorId, FACING_NORTH, 20);
	/* "If you can defeat a miracle, only then can you ignite the beacon's
	   flame." */
	fdScriptShowMessage(ACTOR_WISE_ONE, 0);
	/* "The Wise One is up to something! Be wary, everyone! We don't know
	   what..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	scFieldActorSetAnimation(state->leaderActorId, 3);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	scFieldActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	actor->update = (OBJECT_UPDATE)scMarsLighthouseAerieFlicker;
	fdActorSetScript(ACTOR_WISE_ONE, MarsLighthouseAerieVoiceFadeScript);
	fdPaletteFadeApplyTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
	fdPaletteFadeInterpolate(20);
	syWait(20);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeInterpolate(40);
	syWait(40);
	fdActorSetPos(ACTOR_PIERS, 0, 0);
	fdActorSetPos(state->leaderActorId, 0, 0);
	fdActorSetPos(ACTOR_ISAAC, 0, 0);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_SHEBA, 0, 0);
	fdActorSetPos(ACTOR_JENNA, 0, 0);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	syWait(1);
	actor = fdActorGet(ACTOR_DOOM_DRAGON);
	actor->scaleX = toFIXED(1) / 16;
	actor->scaleY = toFIXED(1) / 16;
	fdActorSetPos(ACTOR_DOOM_DRAGON, 512, 280);
	syWait(1);
	actor->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		actor, toFIXED(512), toFIXED(48), toFIXED(280));
	syWait(1);
	actor->motionFlg = 0;
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeInterpolate(20);
	fdScriptPlaySnd(MUS_ALEX);
	fdActorRunScriptAndWait(
		ACTOR_DOOM_DRAGON, MarsLighthouseAerieDoomDragonRiseScript);
	REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(40);
	fdActorSetSpeedAndAcceleration(ACTOR_DOOM_DRAGON, 3.2, 0.2);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	scFieldActorSetMoveTarget(actor, 544, 88, 280);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(10);
	scFieldActorWaitForMoveById(ACTOR_DOOM_DRAGON);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	scFieldActorSetMoveTarget(actor, 464, 88, 280);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(10);
	scFieldActorWaitForMoveById(ACTOR_DOOM_DRAGON);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	scFieldActorSetMoveTarget(actor, 576, 88, 280);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(10);
	scFieldActorWaitForMoveById(ACTOR_DOOM_DRAGON);
	fdActorSetSpeed(ACTOR_DOOM_DRAGON, 6.4);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	scFieldActorSetMoveTarget(actor, 508, 208, 288);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeInterpolate(10);
	scFieldActorWaitForMoveById(ACTOR_DOOM_DRAGON);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	REG_DISPCNT |= DISPCNT_BG1_ON | DISPCNT_BG2_ON;
	fdActorSetPos(ACTOR_PIERS, 488, 320);
	fdActorSetPos(state->leaderActorId, 504, 320);
	fdActorSetPos(ACTOR_ISAAC, 520, 320);
	fdActorSetPos(ACTOR_GARET, 536, 320);
	fdActorSetPos(ACTOR_SHEBA, 480, 328);
	fdActorSetPos(ACTOR_JENNA, 496, 328);
	fdActorSetPos(ACTOR_KRADEN, 512, 328);
	fdActorSetPos(ACTOR_IVAN, 528, 328);
	fdActorSetPos(ACTOR_MIA, 544, 328);
	syWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeInterpolate(10);
	syWait(20);
	fdScriptPlaySnd(SE_ITEM_BREAK);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	syWait(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeInterpolate(40);
	syWait(40);
	actor->gravity = toFIXED(1) / 4;
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_BANE_ESCAPE);
	fdMapGridSetShake(2, 4, 1);
	syFieldScriptWait(10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	syFieldScriptWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_DOOM_DRAGON, 1);
	fdScriptPlaySnd(SE_BOSS_ROAR);
	syFieldScriptWait(80);
	/* "A three-headed dragon? That's your miracle?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "So you would have us fight for our future? Fine, then fight we
	   shall!" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "What's he thinking? We already beat a two-headed dragon. How much..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	/* "I don't care how many heads it has. Nothing's going to stop us now!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
	/* "Let's do it! For Prox! For the future of Weyard!" */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorSetSpeed(state->leaderActorId, 1.6);
	fdActorSetSpeed(ACTOR_ISAAC, 1.6);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_IVAN, 1.6);
	fdActorSetSpeed(ACTOR_MIA, 1.6);
	fdActorSetScript(ACTOR_PIERS, MarsLighthouseAeriePiersReadyScript);
	fdActorSetScript(
		state->leaderActorId, MarsLighthouseAerieLeaderReadyScript);
	fdActorSetScript(ACTOR_ISAAC, MarsLighthouseAerieIsaacReadyScript);
	fdActorSetScript(ACTOR_SHEBA, MarsLighthouseAerieShebaReadyScript);
	fdActorSetScript(ACTOR_JENNA, MarsLighthouseAerieJennaReadyScript);
	fdActorSetScript(ACTOR_IVAN, MarsLighthouseAerieIvanReadyScript);
	fdActorSetScript(ACTOR_MIA, MarsLighthouseAerieMiaReadyScript);
	fdActorRunScriptAndWait(ACTOR_GARET, MarsLighthouseAerieGaretReadyScript);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "Wait a second... Wasn't that two-headed dragon actually... So
	   that..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTHWEST);
	/* "Felix! NO! You mustn't fight that dragon! It's°" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTHWEST);
	scFieldActorJump(ACTOR_GARET, 6, 10);
	/* "Too late, Kraden! We can't get away from it now!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	scFieldActorJump(ACTOR_KRADEN, 2, 10);
	scFieldActorJump(ACTOR_KRADEN, 4, 10);
	/* "Felix, don't! Stop!!!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	state->battleType = BATTLE_TYPE_NO_ESCAPE;
	syFieldScriptSetAfterBattleDestination(
		SCENE_MARS_LIGHTHOUSE_AERIE, ENTRANCE_DRAGON_BEATEN);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 1);
	return;
}

/* The Doom Dragon's parts flicker white, ever faster. */
void scMarsLighthouseAerieFlickerDragon(ACTOR *actor)
{
	if((MarsLighthouseAerieFlickerTick >> MarsLighthouseAerieFlickerRate) & 1)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	MarsLighthouseAerieFlickerTick++;
}

/* The three it was flicker white out of step with the dragon. */
void scMarsLighthouseAerieFlickerParent(ACTOR *actor)
{
	if((MarsLighthouseAerieFlickerTick >> MarsLighthouseAerieFlickerRate) & 1)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
}

extern Sint32 MarsLighthouseAerieShebaToThosePeopleScript[];
extern Sint32 MarsLighthouseAerieJennaToThosePeopleScript[];
extern Sint32 MarsLighthouseAerieJennaTurnScript[];
extern Sint32 MarsLighthouseAerieLeaderTurnScript[];
extern Sint32 MarsLighthouseAerieIsaacTurnScript[];
extern Sint32 MarsLighthouseAerieGaretTurnScript[];
extern Sint32 MarsLighthouseAerieMiaTurnScript[];

/* The Doom Dragon falls, and the party finds who it was: Felix and Jenna's
   parents and Isaac's father, Kyle, freed. */
void scMarsLighthouseAerieFindWhoTheDragonWas(void)
{
	Sint32 facing;
	ACTOR *actor, *mother, *father, *kyle;
	Sint32 leaderId;

	actor = fdActorGet(ACTOR_DOOM_DRAGON);
	mother = fdActorGet(ACTOR_FREED_JENNAS_MOTHER);
	father = fdActorGet(ACTOR_FREED_JENNAS_FATHER);
	kyle = fdActorGet(ACTOR_FREED_KYLE);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_DOOM_DRAGON, 2);
	actor->motionFlg = 0;
	actor->x.fixed -= toFIXED(4);
	actor->z.fixed += toFIXED(16);
	fdActorSetAnimation(ACTOR_DRAGON_HEAD_B, 2);
	fdActorSetAnimation(ACTOR_DRAGON_HEAD_C, 3);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	fdActorSetAnimation(ACTOR_FREED_JENNAS_MOTHER, 7);
	fdActorSetAnimation(ACTOR_FREED_JENNAS_FATHER, 7);
	fdActorSetAnimation(ACTOR_FREED_KYLE, 6);
	leaderId = MainRoutineStateWork.leaderActorId;
	facing = FACING_NORTH;
	fdActorSetPosAndFacing(leaderId, toFIXED(504), toFIXED(320), facing);
	syWait(1);
	fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeInterpolate(1);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_MOTHER)), SPRITE_COLOR_HIDDEN);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_FATHER)), SPRITE_COLOR_HIDDEN);
	scFieldActorSetSprCol((fdActorGet(ACTOR_FREED_KYLE)), SPRITE_COLOR_HIDDEN);
	syWait(1);
	scFieldActorSetSprPriority(ACTOR_SHEBA_STAND_IN, 1);
	scFieldActorSetSprPriority(ACTOR_JENNA_STAND_IN, 1);
	scFieldActorSetSprPriority(ACTOR_KRADEN, 1);
	scFieldActorSetSprPriority(ACTOR_IVAN_STAND_IN, 1);
	scFieldActorSetSprPriority(ACTOR_MIA_STAND_IN, 1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	FieldStateWork->transitionFrm = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
	fdPaletteFadeInterpolate(16);
	syWait(16);
	syFieldScriptWait(80);
	MarsLighthouseAerieFlickerTick = 0;
	MarsLighthouseAerieFlickerRate = 4;
	fdActorGet(ACTOR_DOOM_DRAGON)->update =
		(OBJECT_UPDATE)scMarsLighthouseAerieFlickerDragon;
	fdActorGet(ACTOR_DRAGON_HEAD_A)->update =
		(OBJECT_UPDATE)scMarsLighthouseAerieFlickerDragon;
	fdActorGet(ACTOR_DRAGON_HEAD_B)->update =
		(OBJECT_UPDATE)scMarsLighthouseAerieFlickerDragon;
	fdActorGet(ACTOR_DRAGON_HEAD_C)->update =
		(OBJECT_UPDATE)scMarsLighthouseAerieFlickerDragon;
	mother->update = (OBJECT_UPDATE)scMarsLighthouseAerieFlickerParent;
	father->update = (OBJECT_UPDATE)scMarsLighthouseAerieFlickerParent;
	kyle->update = (OBJECT_UPDATE)scMarsLighthouseAerieFlickerParent;
	fdScriptPlaySnd(SE_DRAGON_FLICKER);
	syWait(10);
	fdScriptPlaySnd(SE_DRAGON_FLICKER);
	syWait(10);
	fdScriptPlaySnd(SE_DRAGON_FLICKER);
	syWait(10);
	fdScriptPlaySnd(SE_DRAGON_FLICKER);
	syWait(10);
	fdScriptPlaySnd(SE_DRAGON_FLICKER);
	syWait(40);
	MarsLighthouseAerieFlickerRate = 3;
	syWait(30);
	MarsLighthouseAerieFlickerRate = 2;
	syWait(20);
	fdActorSetPos(ACTOR_DOOM_DRAGON, 0, 0);
	fdActorSetPos(ACTOR_DRAGON_HEAD_A, 0, 0);
	fdActorSetPos(ACTOR_DRAGON_HEAD_B, 0, 0);
	fdActorSetPos(ACTOR_DRAGON_HEAD_C, 0, 0);
	fdActorGet(ACTOR_DOOM_DRAGON)->update = NULL;
	fdActorGet(ACTOR_DRAGON_HEAD_A)->update = NULL;
	fdActorGet(ACTOR_DRAGON_HEAD_B)->update = NULL;
	fdActorGet(ACTOR_DRAGON_HEAD_C)->update = NULL;
	mother->update = NULL;
	father->update = NULL;
	kyle->update = NULL;
	syWait(1);
	fdScriptPlaySnd(SE_CURSE);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_MOTHER)), SPRITE_COLOR_BLUE_TINT);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_FATHER)), SPRITE_COLOR_BLUE_TINT);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_KYLE)), SPRITE_COLOR_BLUE_TINT);
	syFieldScriptWait(20);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_MOTHER)), SPRITE_COLOR_NORMAL);
	scFieldActorSetSprCol(
		(fdActorGet(ACTOR_FREED_JENNAS_FATHER)), SPRITE_COLOR_NORMAL);
	scFieldActorSetSprCol((fdActorGet(ACTOR_FREED_KYLE)), SPRITE_COLOR_NORMAL);
	syFieldScriptWait(20);
	mother->sceneCnt = (syRandomBelow(80)) + 40;
	father->sceneCnt = (syRandomBelow(80)) + 40;
	kyle->sceneCnt = (syRandomBelow(80)) + 40;
	mother->update = (OBJECT_UPDATE)scMarsLighthouseAerieThrob;
	father->update = (OBJECT_UPDATE)scMarsLighthouseAerieThrob;
	kyle->update = (OBJECT_UPDATE)scMarsLighthouseAerieThrob;
	syFieldScriptWait(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
	fdPaletteFadeInterpolate(40);
	syWait(80);
	syFieldScriptSetMessageId((Sint32)MsgMarsLighthouseAerieWhoAreThey);
	fdActorFaceDirectionAndWait(ACTOR_IVAN_STAND_IN, FACING_WEST);
	/* "...Who are they?" */
	fdScriptShowMessage(ACTOR_IVAN_STAND_IN, 0);
	/* "That's what I was trying to warn you about!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS_STAND_IN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC_STAND_IN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET_STAND_IN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA_STAND_IN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN_STAND_IN, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA_STAND_IN, FACING_SOUTH, 0);
	fdCameraSetSpeed(1.6);
	syScriptCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(512, 80, 340, 1);
	fdCameraWaitForArrival();
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "It wasn't just the dragon you killed..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdScriptPlaySnd(MUS_THE_WISE_ONES_TRAP);
	fdActorSetAttachedEffect(
		ACTOR_GARET_STAND_IN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "I remember you warning us, but it was too late for us to stop..." */
	fdScriptShowMessage(ACTOR_GARET_STAND_IN, 0);
	/* "Kraden, what do you know? Why were you trying to stop us from..." */
	fdScriptShowMessage(ACTOR_MIA_STAND_IN, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "It was the Wise One's final trap..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAttachedEffect(
		ACTOR_IVAN_STAND_IN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Trap? What do you mean?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN_STAND_IN, 0);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "The Wise One knew he couldn't stop you, so he played a cruel..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorShowEmote(
		ACTOR_ISAAC_STAND_IN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorSetSpeed(ACTOR_ISAAC_STAND_IN, 1.6);
	fdActorWalkToAndWait(ACTOR_ISAAC_STAND_IN, 512, 328);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC_STAND_IN, FACING_SOUTHEAST);
	/* "Kraden, I don't understand... What are you trying to tell us?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC_STAND_IN, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA_STAND_IN, FACING_WEST);
	/* "Sheba, let's see who those people are while they try to sort this
	   out." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN,
		0);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	scFieldActorSetSprPriority(ACTOR_SHEBA_STAND_IN, 2);
	fdActorGet(ACTOR_JENNA_STAND_IN)->sceneCnt = 0;
	fdActorGet(ACTOR_SHEBA_STAND_IN)->sceneCnt = 0;
	fdActorSetSpeed(ACTOR_SHEBA_STAND_IN, 0.8);
	fdActorSetSpeed(ACTOR_JENNA_STAND_IN, 0.8);
	fdActorSetScript(
		ACTOR_SHEBA_STAND_IN, MarsLighthouseAerieShebaToThosePeopleScript);
	fdActorSetScript(
		ACTOR_JENNA_STAND_IN, MarsLighthouseAerieJennaToThosePeopleScript);
	fdActorSetSpeed(ACTOR_KRADEN, 2.4);
	fdActorWalkToAndWait(ACTOR_KRADEN, 540, 340);
	scFieldActorJump(ACTOR_KRADEN, 4, 10);
	/* "No, Jenna! Don't look! You mustn't look! It will only bring you
	   pain..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	actor = fdActorGet(ACTOR_SHEBA_STAND_IN);
	do {
		syWait(1);
	} while(actor->sceneCnt == 0);
	fdActorFaceDirection(ACTOR_JENNA_STAND_IN, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
	/* "What are you talking about, Kraden? They can't hurt us anymore..." */
	fdScriptShowMessage(ACTOR_SHEBA_STAND_IN, 0);
	scFieldActorSetSprPriority(ACTOR_JENNA_STAND_IN, 2);
	scFieldActorSetSprPriority(ACTOR_PIERS_STAND_IN, 2);
	scFieldActorShowEmote(
		ACTOR_JENNA_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetSpeed(ACTOR_JENNA_STAND_IN, 1.6);
	fdActorWalkToAndWait(ACTOR_JENNA_STAND_IN, 490, 296);
	fdActorFaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_JENNA_STAND_IN, FACING_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_JENNA_STAND_IN, FACING_NORTHEAST, 20);
	fdActorSetSpeed(ACTOR_JENNA_STAND_IN, 0.2);
	fdActorGet(ACTOR_JENNA_STAND_IN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JENNA_STAND_IN, 488, 300);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_JENNA_STAND_IN)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_JENNA_STAND_IN, 2);
	fdActorGet(ACTOR_JENNA_STAND_IN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JENNA_STAND_IN, 486, 308);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_JENNA_STAND_IN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(512, 96, 320, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_SHEBA_STAND_IN, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_EAST);
	/* "What's the matter, Jenna?" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA_STAND_IN, 0);
	fdActorFaceDirection(ACTOR_PIERS_STAND_IN, FACING_NORTH, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTHWEST, 0);
	/* "It...can't be... How? How..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
	actor = fdActorGet(ACTOR_FREED_KYLE);
	actor->update = NULL;
	syWait(1);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	fdActorRunRepeatedMotion(ACTOR_FREED_KYLE, 2);
	/* "Nnn... Uhnnn..." */
	fdScriptShowMessage(ACTOR_FREED_KYLE, 0);
	fdActorShowEmote(
		ACTOR_ISAAC_STAND_IN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_ISAAC_STAND_IN, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_MIA_STAND_IN, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_MIA_STAND_IN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "What is it, Isaac?" */
	fdScriptShowMessage(ACTOR_MIA_STAND_IN, 0);
	fdActorFaceDirection(ACTOR_ISAAC_STAND_IN, FACING_EAST, 40);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC_STAND_IN, FACING_NORTH);
	scFieldActorStartRepeatedMotion(ACTOR_FREED_KYLE, 3);
	/* "Uhhh...unnnn..." */
	fdScriptShowMessage(ACTOR_FREED_KYLE, 0);
	fdActorFaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_GARET_STAND_IN, FACING_SOUTHWEST);
	/* "Isaac! I... I know that guy! That's your dad!" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET_STAND_IN, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA_STAND_IN, FACING_SOUTHEAST);
	fdActorSetAttachedEffect(
		ACTOR_SHEBA_STAND_IN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Then...does that mean... the other two are..." */
	fdScriptShowMessage(ACTOR_SHEBA_STAND_IN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET_STAND_IN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN_STAND_IN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA_STAND_IN, FACING_NORTH, 0);
	actor = fdActorGet(ACTOR_FREED_JENNAS_MOTHER);
	actor->update = NULL;
	syWait(1);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	actor = fdActorGet(ACTOR_FREED_JENNAS_FATHER);
	actor->update = NULL;
	syWait(1);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	syFieldScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_FREED_JENNAS_FATHER, 1);
	/* "Hhnnnng..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_FREED_JENNAS_FATHER, 0);
	fdActorRunRepeatedMotion(ACTOR_FREED_JENNAS_MOTHER, 2);
	/* "Hhhnnn..." */
	fdScriptShowMessage(ACTOR_FREED_JENNAS_MOTHER, 0);
	scFieldActorJump(ACTOR_JENNA_STAND_IN, 2, 10);
	scFieldActorJump(ACTOR_JENNA_STAND_IN, 4, 10);
	/* "Mom... Dad..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA_STAND_IN, 0);
	fdActorSetSpeed(ACTOR_JENNA_STAND_IN, 2.4);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2.4);
	fdActorSetSpeed(ACTOR_ISAAC_STAND_IN, 2.4);
	fdActorSetSpeed(ACTOR_GARET_STAND_IN, 2.4);
	fdActorSetScript(ACTOR_JENNA_STAND_IN, MarsLighthouseAerieJennaTurnScript);
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
		MarsLighthouseAerieLeaderTurnScript);
	fdActorSetScript(ACTOR_ISAAC_STAND_IN, MarsLighthouseAerieIsaacTurnScript);
	fdActorRunScriptAndWait(
		ACTOR_GARET_STAND_IN, MarsLighthouseAerieGaretTurnScript);
	syFieldScriptWait(20);
	fdActorSetSpeed(ACTOR_PIERS_STAND_IN, 1.6);
	fdActorWalkToAndWait(ACTOR_PIERS_STAND_IN, 528, 320);
	fdActorFaceDirection(ACTOR_PIERS_STAND_IN, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MIA_STAND_IN, FACING_NORTHWEST);
	/* "I'll heal Jenna's parents! Quickly, Mia! Tend to Isaac's father!" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS_STAND_IN, 0);
	fdActorSetSpeed(ACTOR_MIA_STAND_IN, 1.6);
	fdActorSetScript(ACTOR_MIA_STAND_IN, MarsLighthouseAerieMiaTurnScript);
	fdActorWalkToAndWait(ACTOR_PIERS_STAND_IN, 486, 308);
	fdActorFaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_NORTHWEST);
	fdActorSetSpeed(ACTOR_SHEBA_STAND_IN, 2.4);
	fdActorGet(ACTOR_SHEBA_STAND_IN)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_SHEBA_STAND_IN, 474, 316);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_SHEBA_STAND_IN)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_SHEBA_STAND_IN, FACING_NORTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_PIERS_STAND_IN, 462, 280);
	fdActorFaceDirectionAndWait(ACTOR_PIERS_STAND_IN, FACING_EAST);
	scFieldActorSetSprPriority(ACTOR_PIERS_STAND_IN, 1);
	scMarsLighthouseAerieStartRisingRings(
		fdActorGet(ACTOR_PIERS_STAND_IN), &MarsLighthouseAerieRisingRingsA);
	scMarsLighthouseAerieStartRisingRings(
		fdActorGet(ACTOR_MIA_STAND_IN), &MarsLighthouseAerieRisingRingsB);
	syFieldScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "If only... If only I'd realized it sooner..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	syWait(120);
	scMarsLighthouseAerieEndRisingRings(fdActorGet(ACTOR_PIERS_STAND_IN));
	scMarsLighthouseAerieEndRisingRings(fdActorGet(ACTOR_MIA_STAND_IN));
	syFieldScriptWait(80);
	syFieldScriptSetExitRequest(EXIT_TO_THE_BEACON);
	return;
}

/* The actor stands where the party will pass, no longer in the way. */
void scMarsLighthouseAerieStandAside(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	actor->collisionFlg = 0;
	actor->motionFlg = 0;
	scFieldActorSetSprPriority(id, 1);
}

/* At the beacon, before it is lit: Piers and Mia can heal the three no longer,
   Kraden tells Jenna they spent their last strength as the dragon, the party
   turns on the Wise One, and Kraden urges Felix to light the beacon. */
void scMarsLighthouseAerieUrgeOnToBeacon(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 506, 280);
	syWait(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	scFieldActorSetSprPriority(ACTOR_MIA, 1);
	scMarsLighthouseAerieStartRisingRings(
		fdActorGet(ACTOR_PIERS), &MarsLighthouseAerieRisingRingsA);
	scMarsLighthouseAerieStartRisingRings(
		fdActorGet(ACTOR_MIA), &MarsLighthouseAerieRisingRingsB);
	syFieldScriptWait(120);
	scMarsLighthouseAerieEndRisingRings(fdActorGet(ACTOR_PIERS));
	scMarsLighthouseAerieEndRisingRings(fdActorGet(ACTOR_MIA));
	syFieldScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
	syFieldScriptSetMessageId(MsgMarsLighthouseAerieWhatsMatterPiersWhyDid);
	/* "What's the matter, Piers? Why did you stop? Jenna's parents need
	   you..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
	/* "Don't give up, Mia. You can't! You have to save them..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "It's no use, Ivan... I'm tapped..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_MIA, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "I am, too... And even if I weren't, it's just too late..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHEAST);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	scFieldActorJump(ACTOR_JENNA, 2, 10);
	scFieldActorJump(ACTOR_JENNA, 4, 10);
	/* "What are you saying!?" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_WEST);
	/* "They're not... They can't be..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "That's not what I'm saying, Jenna... I..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "I finally found them... I was going to be with them again... For
	   the..." */
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 40);
	/* "Jenna..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "Please... It can't be..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetAnimation(ACTOR_JENNA, 5);
	fdActorMoveToAndWait(ACTOR_JENNA, 460, 288);
	fdActorSetAnimation(ACTOR_JENNA, 22);
	/* "Mom... Wake up! It's me... It's Jenna! ...Don't leave me..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdCameraSetSpeed(0.2);
	fdCameraMoveTo(512, CAMERA_KEEP, 320, 1);
	fdCameraWaitForArrival();
	/* "Jenna, you must prepare yourself for what comes next..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "Being transformed into a dragon, fighting in that form... This..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	/* "What are you saying, Kraden?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "In fighting you, your parents were forced to use every last ounce
	   of..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
	/* "They don't have the strength to..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	/* "Even if they had won the battle, they would not have survived.
	   You..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 40);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 542, 328);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
	fdCameraSetSpeed(0.8);
	syScriptCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(540, 76, 340, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_SOUTH);
	fdActorWalkToAndWait(ACTOR_GARET, 542, 340);
	scFieldActorJump(ACTOR_GARET, 2, 10);
	scFieldActorJump(ACTOR_GARET, 4, 10);
	/* "You monster!!!" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
	scFieldActorJump(ACTOR_GARET, 4, 10);
	/* "Why did you do this? Why did you make us fight Jenna's parents?" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(478, 96, 312, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHWEST);
	scFieldActorJump(ACTOR_SHEBA, 4, 10);
	/* "You're no god! You're no protector! You're evil!" */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
	fdCameraMoveTo(480, CAMERA_KEEP, 276, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 20);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
	/* "You don't understand the pain you have caused, Wise One." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
	/* "You have no idea the damage done to a child who learns she has..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
	/* "That's enough..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdCameraMoveTo(512, CAMERA_KEEP, 312, 1);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	/* "I knew what I was doing the moment I raised my sword." */
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 20);
	scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "We defied the Wise One in order to save the world. Our parents
	   would..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHWEST);
	/* "Don't you think so, Felix?" */
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		/* "You're right. It hurts, but it's true. We didn't do this for..." */
		fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
		FieldStateWork->msg += 1;
	} else {
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		FieldStateWork->msg += 1;
		/* "I agree with my brother... But it does no one any good if we
		   don't..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
	}
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 548, 320);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	/* "We still have a chance to save Prox." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "Perhaps we can't save your parents, but we can save countless
	   others." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "Kyle and the others saved them, too... They sacrificed their lives
	   so..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	/* "I never imagined that my actions would help to save the world..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	/* "Even though lighting the beacon may create wars and strife, I
	   regret..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkToAndWait(ACTOR_KRADEN, 498, 320);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "There's little time left, Felix... Use the Mars Star and light
	   the..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdScriptSetFlag(FLAG_MARS_BEACON_LIT);
	fdScriptPlaySnd(MUS_MARS_LIGHTHOUSE);
	fdActorSetSpeed(ACTOR_ISAAC, 0.8);
	fdActorWalkToAndWait(ACTOR_ISAAC, 540, 286);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
	scMarsLighthouseAerieStandAside(ACTOR_PIERS);
	scMarsLighthouseAerieStandAside(ACTOR_JENNA);
	scMarsLighthouseAerieStandAside(ACTOR_SHEBA);
	scMarsLighthouseAerieStandAside(ACTOR_KRADEN);
	scMarsLighthouseAerieStandAside(ACTOR_ISAAC);
	scMarsLighthouseAerieStandAside(ACTOR_MIA);
	scMarsLighthouseAerieStandAside(ACTOR_IVAN);
	scMarsLighthouseAerieStandAside(ACTOR_GARET);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdScriptSetFlag(FLAG_ASKED_FOR_MARS_STAR);
	fdMapGridCopyMetatileAttributes(39, 40, 18, 16, 23, 10);
	gmPartyRestoreHpPp(1);
	fdPartyRemoveMemberRestored(7);
	fdPartyRemoveMemberRestored(5);
	fdPartyRemoveMemberRestored(6);
	fdPartyRemoveMemberRestored(0);
	fdPartyRemoveMemberRestored(3);
	fdPartyRemoveMemberRestored(2);
	fdPartyRemoveMemberRestored(1);
	fdActorGet(ACTOR_JENNAS_MOTHER)->collisionFlg |= ACTOR_NOT_TARGETED;
	fdActorGet(ACTOR_JENNAS_FATHER)->collisionFlg |= ACTOR_NOT_TARGETED;
	scFieldActorSetSprCol(fdActorGet(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
	fdActorSetPos(ACTOR_HAMMA, 568, 274);
	fdActorGet(ACTOR_JENNAS_MOTHER_TALK_SPOT)->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(fdActorGet(ACTOR_JENNAS_MOTHER_TALK_SPOT),
		toFIXED(492), toFIXED(96), toFIXED(284));
	fdActorGet(ACTOR_JENNAS_FATHER_TALK_SPOT)->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(fdActorGet(ACTOR_JENNAS_FATHER_TALK_SPOT),
		toFIXED(510), toFIXED(96), toFIXED(295));
	fdActorGet(ACTOR_KYLE_TALK_SPOT)->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(fdActorGet(ACTOR_KYLE_TALK_SPOT),
		toFIXED(540), toFIXED(96), toFIXED(300));
	syWait(1);
	fdScriptSetFlag(FLAG_CANNOT_SAVE_HERE);
	fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
	fdScriptEnd();
}

/* An orb circles the beacon's beam as it rises. */
void scOrbUpdateRising(ACTOR *self)
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
	self->tmr += -(TRIG_TURN / 32);
}

/* Every third tick, or every tick once the light has broken through, an orb
   starts to rise round the beacon's beam. */
void scOrbSpawnRising(void)
{
	ACTOR	  *center;
	ACTOR	  *effect;
	FIELD_SPR *spr;
	Sint32	   ang;

	if(fdScriptGetFlag(FLAG_BEACON_BROKE_THROUGH) == 0 && TickCount % 3 != 0)
		return;
	center = fdActorGet(ACTOR_BEAM_BOTTOM);
	if(fdScriptGetFlag(FLAG_BEACON_BROKE_THROUGH) != 0) {
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
		fdActorStartScript(effect, OrbRisingScript);
		scFieldActorSetSprCol(effect, SPRITE_COLOR_GRB);
		effect->motionFlg = 0;
		ang = syRandom() & 0xffff000;
		effect->tmr = ang;
		effect->userValue = 0;
		effect->update = (OBJECT_UPDATE)scOrbUpdateRising;
		effect->speedLimit =
			syMathSin((Uint32)(syRandom() * 65535) >> 20) * 24 >> 16;
		spr->flg = 0;
		spr->pri = 2;
	}
}

#include "../field/actor_stop_where_it_stands.c"

void fdTeleportPlayWhirl(void);

extern Sint32 MarsLighthouseAerieBeaconFireScript[];

/* The beacon's fire: a flash and a quake, then it falls from above into the
   beacon with glittering sparks. */
void scMarsLighthouseAerieRaiseBeaconFire(void)
{
	ACTOR					  *anchor = fdActorGetById(ACTOR_BEACON_FIRE);
	Sint32					   spawn = 0;
	ACTOR					  *actor;
	MARS_LIGHTHOUSE_AERIE_SPR *spr;
	Sint32					   ang;
	Uint8					   phase;

	switch(MarsLighthouseAerieBeaconFireTick) {
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
		anchor->x.fixed = toFIXED(512);
		anchor->y.fixed = -toFIXED(360);
		anchor->z.fixed = toFIXED(204);
		anchor->scaleX = toFIXED(1);
		anchor->scaleY = toFIXED(1);
		fdActorStopWhereItStands(anchor);
		fdActorSetScript(
			ACTOR_BEACON_FIRE, MarsLighthouseAerieBeaconFireScript);
		break;
	case 25:
		MarsLighthouseAerieBeaconFireTick--;
		if(anchor->y.fixed > 0) {
			fdPaletteFadeApplyTarget(
				PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
			fdPaletteFadeInterpolate(16);
			MarsLighthouseAerieBeaconFireTick++;
		} else {
			if((TickCount & 7) == 0)
				fdScriptPlaySnd(SE_GLITTER);
			spawn = 1;
			anchor->y.fixed += toFIXED(9) / 4;
		}
		break;
	case 26:
		MarsLighthouseAerieBeaconFireTick--;
		if(anchor->y.fixed > toFIXED(20)) {
			MarsLighthouseAerieBeaconFireTick = 999;
			syFieldCloseEvent(scMarsLighthouseAerieRaiseBeaconFire);
		} else {
			if((TickCount & 7) == 0)
				fdScriptPlaySnd(SE_GLITTER);
			anchor->y.fixed += toFIXED(9) / 4;
			spawn = 1;
		}
		break;
	}
	if(spawn) {
		actor = scFieldActorCreate(SPRITE_PARTICLE, anchor->x.fixed,
			anchor->y.fixed - ((syRandomBelow(80)) << 16) - toFIXED(8),
			anchor->z.fixed);
		if(actor != NULL) {
			spr = actor->spr;
			fdActorStartScript(actor, MarsLighthouseAerieBeaconSparkScript);
			scFieldActorSetSprCol(actor, SPRITE_COLOR_GRB);
			actor->motionFlg = 0;
			ang = syRandom() & 0xffff000;
			actor->tmr = ang;
			actor->userValue = 0;
			phase = syRandom() >> 13;
			actor->sceneCnt = phase;
			actor->update = scMarsLighthouseAerieOrbitBeaconFire;
			actor->speedLimit = syMathSin(syRandom() * 0xffff >> 20) * 24;
			actor->speedLimit = FIXEDtoINT(anchor->speedLimit);
			spr->flg = 0;
			((FIELD_SPR *)spr)->pri = 1;
		}
	}
	if(++MarsLighthouseAerieBeaconFireTick > MarsLighthouseAerieBeaconFireTicks)
		syFieldCloseEvent(scMarsLighthouseAerieRaiseBeaconFire);
}

/* The actor steps back from the beam, to the left. */
void scMarsLighthouseAerieStepBackLeft(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	scFieldActorSetSprPriority(id, 1);
	scFieldActorSetPosAndResetMotion(actor, actor->x.fixed - toFIXED(16),
		actor->y.fixed, actor->z.fixed + toFIXED(32));
}

/* The actor steps back from the beam, to the right. */
void scMarsLighthouseAerieStepBackRight(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	scFieldActorSetSprPriority(id, 1);
	scFieldActorSetPosAndResetMotion(actor, actor->x.fixed + toFIXED(16),
		actor->y.fixed, actor->z.fixed + toFIXED(32));
}

/* The actor keeps to the ground as the beacon's beam rises. */
void scMarsLighthouseAerieHoldStill(Sint32 id)
{
	fdActorGetById(id)->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

void		  scMarsLighthouseAerieStartLookingAround(Sint32 id);
void		  scMarsLighthouseAerieStopLookingAround(Sint32 id);
extern Sint32 MarsLighthouseAerieBeamNarrowScript[];

/* The Mars Star is cast into the beacon: the beam rises, the fire falls into it
   and the beacon is lit. */
void scMarsLighthouseAerieLightBeacon(void)
{
	MARS_LIGHTHOUSE_AERIE_SPR *spr;
	ACTOR					  *star;
	ACTOR					  *anchor;
	ICON_TRANSFER			  *icon;
	ACTOR					  *bottomBeam, *lowerBeam, *middleBeam;
	ACTOR					  *upperBeam, *topBeam;
	ACTOR					  *actor;

	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syWait(1);
	fdActorSetPos(ACTOR_HAMMA, 0, 0);
	fdActorSetPos(ACTOR_JENNAS_FATHER_TALK_SPOT, 0, 0);
	fdActorSetPos(ACTOR_KYLE_TALK_SPOT, 0, 0);
	fdCameraSetSpeed(1.6);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(512, 96, 312, 1);
	fdCameraWaitForArrival();
	syWait(1);
	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	fdMapGridCopyMetatileAttributes(18, 40, 18, 16, 23, 10);
	scMarsLighthouseAerieHoldStill(ACTOR_PIERS);
	scMarsLighthouseAerieHoldStill(ACTOR_JENNA);
	scMarsLighthouseAerieHoldStill(ACTOR_SHEBA);
	scMarsLighthouseAerieHoldStill(ACTOR_KRADEN);
	scMarsLighthouseAerieHoldStill(ACTOR_ISAAC);
	scMarsLighthouseAerieHoldStill(ACTOR_MIA);
	scMarsLighthouseAerieHoldStill(ACTOR_IVAN);
	scMarsLighthouseAerieHoldStill(ACTOR_GARET);
	syFieldScriptAddPartyMember(0, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 512, 280);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdActorRunRepeatedMotion(MainRoutineStateWork.leaderActorId, 2);
	syFieldScriptWait(20);
	gmPartyInventoryRemove(ITEM_MYTHRIL_BAG_MARS_STAR);
	anchor = fdActorGet(ACTOR_FELIX);
	star = scFieldActorCreate(SPRITE_UNSEEN_MARKER, anchor->x.fixed,
		anchor->y.fixed + toFIXED(8), anchor->z.fixed);
	if(star != NULL) {
		spr = (MARS_LIGHTHOUSE_AERIE_SPR *)star->spr;
		spr->fullCol = 0;
		spr->layerCnt = 0;
		spr->pal = 0;
		spr->pri = 2;
		star->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		star->motionFlg = 0;
		star->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		star->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprFlags(star, 0);
		star->speedLimit = toFIXED(2) / 5;
		star->acceleration = toFIXED(1) / 20;
		star->gravity = toFIXED(1) / 2;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		gmItemLoadIcon(ITEM_MARS_STAR);
		mnVRAMLoad(spr->vramBlock, ICON_TILE_BYTES, icon->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		star->motionFlg = 0;
		scFieldActorSetMoveTarget(star, 512, 128, 280);
		scFieldActorWaitForMove(star);
		star->motionFlg = ACTOR_FLOATS;
		syFieldScriptWait(60);
		star->motionFlg = 0;
		scFieldActorSetMoveTarget(star, 512, 128, 264);
		scFieldActorWaitForMove(star);
		syFieldScriptWait(20);
		scFieldActorSetMoveTarget(star, 512, 80, 252);
		scFieldActorWaitForMove(star);
		syFieldScriptWait(20);
		fdActorRelease(star);
	}
	syFieldScriptWait(40);
	scFieldActorSetSprPriority(ACTOR_BEAM_BOTTOM, 2);
	scFieldActorSetSprPriority(ACTOR_BEAM_LOWER, 2);
	scFieldActorSetSprPriority(ACTOR_BEAM_MIDDLE, 2);
	scFieldActorSetSprPriority(ACTOR_BEAM_UPPER, 2);
	scFieldActorSetSprPriority(ACTOR_BEAM_TOP, 2);
	fdScriptPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(1, 1, 1);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_WHITE);
	bottomBeam = fdActorGet(ACTOR_BEAM_BOTTOM);
	bottomBeam->scaleX = toFIXED(1) / 10;
	bottomBeam->scaleY = -toFIXED(1);
	bottomBeam->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		bottomBeam, toFIXED(512), toFIXED(64), toFIXED(204));
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_LOWER), SPRITE_COLOR_WHITE);
	lowerBeam = fdActorGet(ACTOR_BEAM_LOWER);
	lowerBeam->scaleX = toFIXED(1) / 10;
	lowerBeam->scaleY = -toFIXED(1);
	lowerBeam->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		lowerBeam, toFIXED(512), toFIXED(96), toFIXED(204));
	sySetTickEvent(scOrbSpawnRising, EVENT_PASS_TICK);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 80);
	fdActorSetScript(ACTOR_BEAM_BOTTOM, MarsLighthouseAerieBeamWidenScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, MarsLighthouseAerieBeamWidenScript);
	fdScriptPlaySnd(SE_CRASH);
	fdMapGridSetShake(6, 6, 1);
	fdPaletteFadeApplyTarget(
		PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeInterpolate(16);
	syWait(20);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(24);
	syWait(60);
	fdScriptPlaySnd(SE_RUMBLE);
	fdScriptSetFlag(FLAG_BEACON_BROKE_THROUGH);
	fdActorGet(ACTOR_BEAM_BOTTOM)->y.fixed = -toFIXED(56);
	fdActorGet(ACTOR_BEAM_LOWER)->y.fixed = -toFIXED(24);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_MIDDLE), SPRITE_COLOR_WHITE);
	middleBeam = fdActorGet(ACTOR_BEAM_MIDDLE);
	middleBeam->scaleY = -toFIXED(1);
	middleBeam->scaleX = (fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX);
	middleBeam->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		middleBeam, toFIXED(512), toFIXED(8), toFIXED(204));
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_UPPER), SPRITE_COLOR_WHITE);
	upperBeam = fdActorGet(ACTOR_BEAM_UPPER);
	upperBeam->scaleY = -toFIXED(1);
	upperBeam->scaleX = (fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX);
	upperBeam->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		upperBeam, toFIXED(512), toFIXED(40), toFIXED(204));
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_TOP), SPRITE_COLOR_WHITE);
	topBeam = fdActorGet(ACTOR_BEAM_TOP);
	topBeam->scaleY = -toFIXED(1);
	topBeam->scaleX = (fdActorGet(ACTOR_BEAM_BOTTOM)->scaleX);
	topBeam->motionFlg = 0;
	scFieldActorSetPosAndResetMotion(
		topBeam, toFIXED(512), toFIXED(72), toFIXED(204));
	fdMapGridCopyMetatilesTo(106, 36, 87, 7, 17, 23);
	fdMapGridCopyMetatilesTo(42, 98, 23, 69, 17, 23);
	fdMapGridCopyMetatileAttributes(0, 40, 18, 20, 23, 10);
	scMarsLighthouseAerieStepBackLeft(ACTOR_PIERS);
	scMarsLighthouseAerieStepBackLeft(ACTOR_JENNA);
	scMarsLighthouseAerieStepBackLeft(ACTOR_SHEBA);
	scMarsLighthouseAerieStepBackLeft(ACTOR_JENNAS_MOTHER);
	scMarsLighthouseAerieStepBackLeft(ACTOR_JENNAS_FATHER);
	scMarsLighthouseAerieStepBackLeft(ACTOR_KRADEN);
	scMarsLighthouseAerieStepBackRight(ACTOR_IVAN);
	scMarsLighthouseAerieStepBackRight(ACTOR_GARET);
	scMarsLighthouseAerieStepBackRight(ACTOR_KYLE);
	scMarsLighthouseAerieStepBackRight(ACTOR_ISAAC);
	scMarsLighthouseAerieStepBackRight(ACTOR_MIA);
	fdActorSetPosAndFacing(MainRoutineStateWork.leaderActorId, toFIXED(484),
		toFIXED(312), FACING_NORTHEAST);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	syWait(1);
	fdMapGridSetShake(4, 4, 1);
	fdPaletteFadeApplyTarget(
		PALETTE_TINT_BLEND | RGB(31, 31, 24), PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	fdActorSetScript(ACTOR_BEAM_BOTTOM, MarsLighthouseAerieBeamNarrowScript);
	fdActorSetScript(ACTOR_BEAM_LOWER, MarsLighthouseAerieBeamNarrowScript);
	fdActorSetScript(ACTOR_BEAM_MIDDLE, MarsLighthouseAerieBeamNarrowScript);
	fdActorSetScript(ACTOR_BEAM_UPPER, MarsLighthouseAerieBeamNarrowScript);
	fdActorSetScript(ACTOR_BEAM_TOP, MarsLighthouseAerieBeamNarrowScript);
	syWait(120);
	fdMapGridSetShake(3, 3, 1);
	fdPaletteFadeApplyTarget(
		PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	syWait(120);
	fdMapGridSetShake(2, 2, 1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	syWait(120);
	fdMapGridSetShake(1, 1, 1);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdActorGet(ACTOR_BEAM_BOTTOM)->scaleY = toFIXED(1) / 50;
	fdActorSetScript(ACTOR_BEAM_LOWER, MarsLighthouseAerieBeamShootUpScript);
	fdActorSetScript(ACTOR_BEAM_MIDDLE, MarsLighthouseAerieBeamShootUpScript);
	fdActorSetScript(ACTOR_BEAM_UPPER, MarsLighthouseAerieBeamShootUpScript);
	fdActorRunScriptAndWait(
		ACTOR_BEAM_TOP, MarsLighthouseAerieBeamShootUpScript);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_HIDDEN);
	syFieldScriptWait(20);
	fdActorRunScriptAndWait(
		ACTOR_BEAM_BOTTOM, MarsLighthouseAerieBeamRiseScript);
	syFieldCloseEvent(scOrbSpawnRising);
	actor = fdActorGet(ACTOR_BEACON_FIRE);
	actor->motionFlg = 0;
	fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEACON_FIRE), SPRITE_COLOR_WHITE);
	scFieldActorSetSprPriority(ACTOR_BEACON_FIRE, 2);
	MarsLighthouseAerieBeaconFireTick = 0;
	MarsLighthouseAerieBeaconFireTicks = 240;
	sySetTickEvent(scMarsLighthouseAerieRaiseBeaconFire, EVENT_PASS_TICK);
	do {
		syWait(1);
	} while(MarsLighthouseAerieBeaconFireTick <= 100);
	fdScriptPlaySnd(MUS_MERCURY_LIGHTHOUSE);
	syFieldScriptWait(120);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	syFieldScriptSetMessageId((Sint32)MsgMarsLighthouseAerieBeaconIsLit);
	/* "The beacon is lit!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	scFieldActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "And to think, I joined this quest hoping to prevent exactly this..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	/* "And I began this quest as a prisoner, taken against my will." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "And if that tidal wave hadn't sent me far off course, I wouldn't..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	/* "How many lives have been taken and changed forever just to light..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	/* "Mom... Dad... Weyard is safe now." */
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0, 10);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldMapGridWaitForShakeToSettle();
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "You're right! I hear voices, too!" */
	mnMessageShowCentered(((Sint32)MsgMarsLighthouseAerieBeaconIsLit + 6), 1);
	fdPsynergyStopMindReadWave();
	syFieldScriptWait(20);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "I told you... I told you I could hear them!" */
	mnMessageShowCentered(((Sint32)MsgMarsLighthouseAerieBeaconIsLit + 7), 1);
	fdPsynergyStopMindReadWave();
	FieldStateWork->msg += 2;
	scMarsLighthouseAerieStartLookingAround(MainRoutineStateWork.leaderActorId);
	scMarsLighthouseAerieStartLookingAround(ACTOR_PIERS);
	scMarsLighthouseAerieStartLookingAround(ACTOR_JENNA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_SHEBA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_KRADEN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStartLookingAround(ACTOR_IVAN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 40);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	/* "Who said that? Where are you?!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 0, 0, 10);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "We're in Imil! We're at the base of Mercury Lighthouse!" */
	mnMessageShowCentered(((Sint32)MsgMarsLighthouseAerieBeaconIsLit + 9), 1);
	fdPsynergyStopMindReadWave();
	scMarsLighthouseAerieStopLookingAround(ACTOR_MIA);
	fdActorFaceDirectionAndWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_MIA,
		FACING_SOUTH);
	fdActorGet(ACTOR_MIA)->update = NULL;
	FieldStateWork->msg++;
	scFieldActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "You... You can't be!" */
	syFieldScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdScriptPlaySnd(SE_ASCEND);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(20);
	syWait(20);
	syFieldScriptSetExitRequest(
		MARS_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EAST_A);
	return;
}

void scMarsLighthouseAerieSpawnBeaconFlames(void);

/* All look up as the beam widens and the Golden Sun begins to rise. */
void scMarsLighthouseAerieWatchGoldenSunRise(void)
{
	Sint32 frm;
	ACTOR *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorGet(MainRoutineStateWork.leaderActorId)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_PIERS)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_JENNA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_SHEBA)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_KRADEN)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorGet(ACTOR_ISAAC)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_GARET)->facing = FACING_SOUTH;
	fdActorGet(ACTOR_IVAN)->facing = FACING_SOUTH;
	syWait(1);
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, 8);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, 8);
	fdActorSetAnimation(ACTOR_KYLE, 7);
	fdActorSetPos(ACTOR_KRADEN, 498, 320);
	fdActorSetPos(ACTOR_GARET, 538, 320);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 506, 280);
	syWait(1);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(512, 96, 288, 0);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptWait(20);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 40);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHEAST);
	syFieldScriptSetMessageId((Sint32)MsgMarsLighthouseAerieWhatCouldHeBeDoing);
	/* "What could he be doing?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	scFieldActorJump(ACTOR_KRADEN, 4, 10);
	/* "Of course! I understand..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(512, CAMERA_KEEP, 320, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
	/* "The Wise One said that when all four lighthouses have been lit,
	   the..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "When that happens, Mt. Aleph and the lighthouses will probably..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	/* "So, the Wise One is warning people in dreams, telling them to
	   seek..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "Why would he do that!? You saw what he did to us... He can't be
	   up..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	scFieldActorSetSprCol(fdActorGet(ACTOR_HAMMA), SPRITE_COLOR_HIDDEN);
	fdActorSetPos(ACTOR_HAMMA, 512, 360);
	syWait(1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "Have you not learned? One's actions do not always reveal one's
	   true..." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	scFieldActorJump(ACTOR_IVAN, 4, 10);
	/* "Hamma... Sister..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Master Hamma! Did you receive a message, too?" */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "Yes. I was called to Jupiter Lighthouse in a dream." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 20);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "You have completed your quest, but I see it comes with great
	   loss......" */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	actor = fdActorGet(ACTOR_JENNA);
	actor->facing = FACING_SOUTHWEST;
	fdActorSetAnimation(ACTOR_JENNA, 22);
	syFieldScriptWait(10);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 2);
	/* "Master Hamma... My parents..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "I know, Jenna, and I am sorry. But do not give up hope for them
	   just..." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	actor->facing = FACING_SOUTH;
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "What do you mean, Master Hamma?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "Appearances can be an illusion... The Wise One has a caring heart." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	/* "He forced us to fight Isaac's and Jenna's parents! What's so..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "If the Wise One were truly evil, he would not be warning me of..." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Well, I... I guess not... But then, what's he up to?" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "We do not have time to discuss it right now." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "She's right. If the Wise One said it's not safe to be near the..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdPsynergyStartMindReadWave(ACTOR_BEAM_BOTTOM, ACTOR_BEAM_LOWER);
	/* "I am heading for safety, too. All of you must get away from the..." */
	fdScriptShowMessage(ACTOR_HAMMA, 0);
	fdPsynergyStopMindReadWave();
	syFieldScriptWait(40);
	fdCameraMoveTo(512, CAMERA_KEEP, 296, 1);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	/* "Hurry, everyone! Let's go!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	/* "What about our parents?" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
	/* "We can't leave them behind..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
	/* "I understand how you feel, but we won't make it if we have to
	   take..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "Make the decision, Felix! Should we take them with us?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
	fdPaletteFadeApplyTarget(PALETTE_TINT_MUTED, PALETTES_BG);
	fdPaletteFadeInterpolate(40);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	fdScriptPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(1, 1, 1);
	syFieldScriptWait(20);
	scMarsLighthouseAerieStartLookingAround(MainRoutineStateWork.leaderActorId);
	scMarsLighthouseAerieStartLookingAround(ACTOR_JENNA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_SHEBA);
	scMarsLighthouseAerieStartLookingAround(ACTOR_PIERS);
	scMarsLighthouseAerieStartLookingAround(ACTOR_KRADEN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_ISAAC);
	scMarsLighthouseAerieStartLookingAround(ACTOR_GARET);
	scMarsLighthouseAerieStartLookingAround(ACTOR_IVAN);
	scMarsLighthouseAerieStartLookingAround(ACTOR_MIA);
	syFieldScriptWait(20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(512, CAMERA_KEEP, 288, 1);
	fdCameraWaitForArrival();
	syFieldCloseEvent(scMarsLighthouseAerieSpawnBeaconFlames);
	fdScriptPlaySnd(SE_SPIN);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEACON_FIRE), SPRITE_COLOR_DARK);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEACON_GLOW), SPRITE_COLOR_DARK);
	scFieldActorSetSprPriority(ACTOR_BEAM_BOTTOM, 2);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEAM_BOTTOM), SPRITE_COLOR_WHITE);
	actor = fdActorGet(ACTOR_BEAM_BOTTOM);
	actor->scaleY = -toFIXED(1);
	actor->scaleX = toFIXED(1) / 10;
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	scFieldActorSetPosAndResetMotion(
		actor, toFIXED(512), toFIXED(64), toFIXED(204));
	for(frm = 0; (Uint32)frm < 40; frm++) {
		if(frm == 20) {
			fdMapGridSetShake(2, 2, 1);
			scFieldActorShowEmote(MainRoutineStateWork.leaderActorId,
				EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scFieldActorShowEmote(
				ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			scMarsLighthouseAerieStopLookingAround(
				MainRoutineStateWork.leaderActorId);
			scMarsLighthouseAerieStopLookingAround(ACTOR_JENNA);
			scMarsLighthouseAerieStopLookingAround(ACTOR_SHEBA);
			scMarsLighthouseAerieStopLookingAround(ACTOR_PIERS);
			scMarsLighthouseAerieStopLookingAround(ACTOR_KRADEN);
			scMarsLighthouseAerieStopLookingAround(ACTOR_ISAAC);
			scMarsLighthouseAerieStopLookingAround(ACTOR_GARET);
			scMarsLighthouseAerieStopLookingAround(ACTOR_IVAN);
			scMarsLighthouseAerieStopLookingAround(ACTOR_MIA);
		}
		actor->scaleX += toFIXED(1) / 64;
		syWait(1);
	}
	fdScriptPlaySnd(MUS_ALCHEMY_UNLEASHED);
	fdActorSetScript(
		ACTOR_BEAM_BOTTOM, MarsLighthouseAerieWatchGoldenSunRiseScript);
	syFieldScriptWait(80);
	scFieldActorJump(ACTOR_GARET, 4, 10);
	scFieldActorJump(ACTOR_GARET, 6, 10);
	fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_WEST);
	/* "It's too dangerous... The Golden Sun is forming!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	/* "A ray of light is emanating from the beacon... From the Mars Star." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTHWEST);
	/* "What will happen?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	/* "The power of Alchemy will be unleashed upon the world!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorFaceDirectionAndWait(ACTOR_MIA, FACING_WEST);
	/* "We must take Isaac's father and Jenna's parents and flee!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, 3);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdCameraMoveTo(512, CAMERA_KEEP, 312, 1);
	fdActorSetPos(ACTOR_BEACON_FIRE, 0, 0);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BEACON_GLOW), SPRITE_COLOR_WHITE);
	scFieldActorSetSprPriority(ACTOR_BEACON_GLOW, 1);
	fdActorGet(ACTOR_BEACON_GLOW)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdScriptPlaySnd(SE_SPIN);
	fdMapGridSetShake(3, 3, 1);
	fdPaletteFadeApplyTarget(
		PALETTE_TINT_CUSTOM | RGB(16, 16, 12), PALETTES_ALL);
	fdPaletteFadeInterpolate(40);
	fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 80);
	scFieldActorJump(ACTOR_GARET, 4, 0);
	fdActorSetSpeed(ACTOR_GARET, 3.2);
	fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 542, 328);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
	/* "Stop it! Let go of my wrist!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 40);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHEAST, 20);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	scFieldActorJump(ACTOR_ISAAC, 4, 10);
	/* "Garet, get a grip! No one's touching you!" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ISAAC, 0);
	fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
	/* "I'm serious! I..." */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "The beacon!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdActorSetAttachedEffect(
		ACTOR_PIERS, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "Jenna... We have to move your parents!" */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	scFieldActorJump(ACTOR_JENNA, 4, 10);
	/* "Mom! Dad!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	/* "What's happening?" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
	scFieldActorJump(ACTOR_KRADEN, 2, 10);
	scFieldActorJump(ACTOR_KRADEN, 4, 10);
	/* "There's no time for questions! Just carry them! Carry them and go!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdScriptPlaySnd(SE_STOP_EXIT_SE);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syWait(1);
	fdScriptPlaySnd(SE_SPIN);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(20);
	syWait(240);
	REG_DISPCNT = 0;
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeInterpolate(8);
	syWait(8);
	syFieldScriptSetExitRequest(MARS_LIGHTHOUSE_AERIE_TO_PROX_HOUSES);
	return;
}

/* Before the beacon is lit, Felix walks away south from the three as they lie
   dying and reaches the head of the broad stairway down from the beacon's
   platform on its west half (metatiles (29, 23) and (30, 23), map.txt's copy of
   them at (45, 53) and (46, 53)). Kraden turns to him: "Do not let your sadness
   overwhelm you, Felix! Go! Once the beacon has been ignited, you can mourn as
   much as you need." and Felix walks back up onto the platform, to (480, 348),
   south-west of them. */
void scMarsLighthouseAerieHoldBackFromParents(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
	syFieldScriptSetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
	/* "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 480, 348);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdScriptEnd();
}

/* The same at the head of the stairway's east half (metatiles (33, 23) and (34,
   23), map.txt's copy of them at (49, 53) and (50, 53)): Kraden turns to him,
   "Do not let your sadness overwhelm you, Felix! Go! Once the beacon has been
   ignited, you can mourn as much as you need. I will protect you.", and Felix
   walks back up onto the platform, to (544, 348), south-east of them. */
void scMarsLighthouseAerieHoldBackFromParentsAgain(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	syFieldScriptSetMessageId(MsgMarsLighthouseAerieDoNotLetYourSadness);
	/* "Do not let your sadness overwhelm you, Felix! Go! Once the beacon..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 544, 348);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
	fdScriptEnd();
}

/* Felix steps up to the beacon, and the voice asks: "Will you cast in the Mars
   Star?" Yes lights the beacon (scMarsLighthouseAerieLightBeacon); no leaves it
   to be asked again once he steps back. */
void scMarsLighthouseAerieOfferToCastMarsStar(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_VOICE, 512, 232);
	syWait(1);
	syFieldScriptSetMessageId(MsgMarsLighthouseAerieWillYouCastInMars);
	/* "Will you cast in the Mars Star?" */
	fdScriptOpenMessage(ACTOR_VOICE, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetPos(ACTOR_VOICE, 0, 0);
		syWait(1);
		scMarsLighthouseAerieLightBeacon();
	}
	fdActorSetPos(ACTOR_VOICE, 0, 0);
	syWait(1);
	fdScriptSetFlag(FLAG_ASKED_FOR_MARS_STAR);
	fdScriptEnd();
}

/* Nothing visible happens as Felix steps back from the beacon without casting
   in the Mars Star; the game lets the voice ask again when he steps up. */
void scMarsLighthouseAerieClearCastOffer(void)
{
	syGameFlagClear(FLAG_ASKED_FOR_MARS_STAR);
}

#if HALT_ON_MARS_AERIE_SPEAKERS
/* In every edition but the Japanese: the one Halt holds speaks haltingly, with
   a pause before each character. */
void scMarsLighthouseAerieStartTalkWhileHalted(Sint32 id)
{
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;

	if(MainRoutineStateWork.haltedActor == id)
		state->haltingSpeech = 1;
}
#endif

#if HALT_ON_MARS_AERIE_SPEAKERS

/* In every edition but the Japanese: the talk over, Halt's hold on its speaker
   ends, and with it the halting speech. */
void scMarsLighthouseAerieEndTalkWhileHalted(Sint32 id)
{
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	Sint32	   held = MainRoutineStateWork.haltedActor;

	if(held == id) {
		state->haltingSpeech = 0;
		MainRoutineStateWork.haltedActor = -1;
		fdActorGet(held)->update = NULL;
		scFieldActorSetSprCol(fdActorGet(held), SPRITE_COLOR_NORMAL);
	}
}

#endif

/* Felix talks to Ivan, who stands by the three: "Felix, I understand your
   loss..." In every edition but the Japanese, if Halt holds him, he speaks
   haltingly. */
void scMarsLighthouseAerieTalkToIvan(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieStartTalkWhileHalted(ACTOR_IVAN);
#endif
	fdActorFaceActor(ACTOR_IVAN, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptSetMessageId(MsgIUnderstandYour);
	/* "Felix, I understand your loss... But remember, you and Jenna
	   still..." */
	fdScriptShowMessage(ACTOR_IVAN, 0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
	scMarsLighthouseAerieEndTalkWhileHalted(ACTOR_IVAN);
#else
	fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_WEST);
#endif
	fdScriptEnd();
}

/* Felix talks to Jenna's mother as she lies dying: "Uhhhhnnn... Felix...
   I...can't see you, but I sense that you're..." */
void scMarsLighthouseAerieTalkToJennasMother(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieStartTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
	syFieldScriptSetMessageId(MsgUhhhhnnnICan);
	/* "Uhhhhnnn... Felix... I...can't see you, but I sense that you're..." */
	fdScriptShowMessage(ACTOR_JENNAS_MOTHER, 0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieEndTalkWhileHalted(ACTOR_JENNAS_MOTHER);
#endif
	fdScriptEnd();
}

/* Felix talks to Kyle, Isaac's father, as he lies dying: "*Koff! Koff!*
   Felix... You made it back...alive..." */
void scMarsLighthouseAerieTalkToKyle(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieStartTalkWhileHalted(ACTOR_KYLE);
#endif
	syFieldScriptSetMessageId(MsgKoffKoffYou);
	/* "*Koff! Koff!* Felix... You made it back...alive... Please, I must..." */
	fdScriptShowMessage(ACTOR_KYLE, 0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieEndTalkWhileHalted(ACTOR_KYLE);
#endif
	fdScriptEnd();
}

/* Felix talks to Jenna's father as he lies dying: "Unnnn... Felix? ...Jenna? I
   am so happy...to have one last chance..." */
void scMarsLighthouseAerieTalkToJennasFather(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieStartTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
	syFieldScriptSetMessageId(MsgUnnnnIAm);
	/* "Unnnn... Felix? ...Jenna? I am so happy...to have one last chance
	   to..." */
	fdScriptShowMessage(ACTOR_JENNAS_FATHER, 0);
#if HALT_ON_MARS_AERIE_SPEAKERS
	scMarsLighthouseAerieEndTalkWhileHalted(ACTOR_JENNAS_FATHER);
#endif
	fdScriptEnd();
}

#if HALT_ON_MARS_AERIE_SPEAKERS

/* In every edition but the Japanese: Halt on one of the three holds the speaker
   beside them. */
void scMarsLighthouseAerieHaltSpeaker(Sint32 spot, Sint32 speaker)
{
	ACTOR			   *found = fdActorGet(spot);
	ACTOR			   *actor;
	ACTOR			   *tgt;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	party->pausedColMode = 0;
	actor = found;
	party->haltedActor = speaker;
	tgt = fdActorGet(speaker);
	tgt->update = actor->update;
	actor->update = NULL;
}
#endif

#if HALT_ON_MARS_AERIE_SPEAKERS

/* In every edition but the Japanese: Ivan is held where he stands while he
   talks. */
void scMarsLighthouseAerieHaltIvan(void)
{
	scMarsLighthouseAerieHaltSpeaker(ACTOR_HAMMA, ACTOR_IVAN);
}

/* In every edition but the Japanese: Jenna's mother is held where she lies
   while she talks. */
void scMarsLighthouseAerieHaltJennasMother(void)
{
	scMarsLighthouseAerieHaltSpeaker(
		ACTOR_JENNAS_MOTHER_TALK_SPOT, ACTOR_JENNAS_MOTHER);
}

/* In every edition but the Japanese: Kyle is held where he lies while he
   talks. */
void scMarsLighthouseAerieHaltKyle(void)
{
	scMarsLighthouseAerieHaltSpeaker(ACTOR_KYLE_TALK_SPOT, ACTOR_KYLE);
}

/* In every edition but the Japanese: Jenna's father is held where he lies while
   he talks. */
void scMarsLighthouseAerieHaltJennasFather(void)
{
	scMarsLighthouseAerieHaltSpeaker(
		ACTOR_JENNAS_FATHER_TALK_SPOT, ACTOR_JENNAS_FATHER);
}
#endif

#include "shared/teleport_shimmer.c"

/* Each tick of an actor's rings: a ring climbs RING_CLIMB pixels up the actor,
   swelling and shrinking as it goes, and the next starts with a chime
   RING_INTERVAL ticks later, two at a time, while the actor's colours cycle.
   Once stopped, the rings climb out and the actor's colours come back. The
   Psynergy test's dbPsynergyTestUpdateRings is the same function. */
void scMarsLighthouseAerieUpdateRisingRings(ACTOR *actor)
{
	VECTOR3		   pos;
	Sint32		   x, z;
	FIXED		   sine;
	Sint32		   i, j;
	SPARKLE_STATE *state = (SPARKLE_STATE *)actor->watchedActor;
	Uint8		  *cnt = &actor->sceneByte;
	EFFECT_SPR	  *spr;

	/* Every other tick, the next of the normal colours and the five swaps. */
	if(*cnt & 1)
		fdActorSetSprColMode(actor,
			(Uint8)((Uint32)((COUNTER_BITS *)cnt)->rest %
				(SPRITE_COLOR_BRG + 1)));
	if(actor->sceneCnt == 0) {
		if((*cnt & (RING_INTERVAL - 1)) == 0) {
			fdScriptPlaySnd(SE_RISING_RINGS);
			state->heights[state->nextPair] = 0;
			state->nextPair ^= 1;
		}
	} else if(actor->sceneCnt == 1) {
		if(state->heights[0] >= RING_CLIMB && state->heights[1] >= RING_CLIMB) {
			fdActorSetSprColMode(actor, SPRITE_COLOR_NORMAL);
			syVRAMCacheFreeEntry(state->vramEntry);
			actor->update = NULL;
			return;
		}
	}
	(*cnt)++;
	for(i = 0; i < 2; i++) {
		/* Its size: half a turn of sine over the climb. */
		sine = syMathSin(state->heights[i] * (DEGtoANG(180) / RING_CLIMB));
		if(state->heights[i] >= 0) {
			if(state->heights[i] < RING_CLIMB) {

				SET_POSITION(&pos, actor->x.fixed,
					actor->y.fixed + toFIXED(state->heights[i]),
					actor->z.fixed);
				fdCameraWorldToScreen(&pos);
				x = pos.x;
				z = pos.z;
				spr = &state->sprs[i * 2];
				for(j = 0; j <= 1; j++) {
					spr->x = x;
					spr->y = z;
					spr->scaleX = sine;
					spr->scaleY = sine;
					if(j == 0) {
						spr->depth = fdActorGetSprDepth(actor) - 1;
					} else {
						spr->depth = fdActorGetSprDepth(actor) + 1;
						spr->y -= toFIXED(1);
						spr->scaleY = -spr->scaleY;
					}
					fdEffectSprDraw(spr);
					spr++;
				}
				state->heights[i]++;
			}
		}
	}
}

/* Starts an actor's rings, kept in `state`: loads their tiles and hooks
   scMarsLighthouseAerieUpdateRisingRings to the actor. */
void scMarsLighthouseAerieStartRisingRings(ACTOR *actor, SPARKLE_STATE *state)
{
	EFFECT_SPR *spr;
	Sint32		tile;
	Sint32		row;
	Sint32		col;
	Sint32		ofs;

	state->vramEntry = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(state->vramEntry, 8 * TILE_SIZE_4BPP,
		MarsLighthouseAerieRisingRingTiles);
	state->nextPair = 0;
	state->heights[0] = RING_NOT_STARTED;
	state->heights[1] = RING_NOT_STARTED;
	state->actor = actor;
	actor->sceneCnt = 0;
	actor->sceneByte = 0;
	actor->update = scMarsLighthouseAerieUpdateRisingRings;
	tile = VramCacheEntries[state->vramEntry].ofs / TILE_SIZE_4BPP;
	actor->watchedActor = (union FIELD_OBJECT *)state;
	for(row = 0; row <= 1; row++) {
		col = 1;
		ofs = (row * 2) * sizeof(EFFECT_SPR);
		spr = (EFFECT_SPR *)(ofs + (Sint32)state);
		for(; col >= 0; col--) {
			fdEffectSprInit(spr, 16, 16, OBJ_SIZE_32X16, tile);
			spr->oam.fullCol = 0;
			spr->oam.pal = 0;
			spr->oam.pri = fdActorGetSprPriority(actor);
			spr++;
		}
	}
}

/* Lets an actor's rings climb out. */
void scMarsLighthouseAerieEndRisingRings(ACTOR *actor)
{
	actor->sceneCnt = 1;
}

/* clang-format off */
/* The rings' tiles: half a ring of light, 32 by 16 pixels of 16 colours, */
/* which scMarsLighthouseAerieUpdateRisingRings draws twice, the second */
/* flipped, for the whole ring; only the first 8 tiles are loaded. */
Sint32 MarsLighthouseAerieRisingRingTiles[] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x70000000, 0xf7700000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x77770000, 0xffff7770, 0xfffffff7, 0x7777ffff,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00007777, 0x0777ffff, 0x7fffffff, 0xffff7777,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000007, 0x0000077f,
	0xfff70000, 0xffff7000, 0x7ffff700, 0x07ffff70,
	0x007fff70, 0x007ffff7, 0x0007fff7, 0x0007fff7,
	0x000077ff, 0x00000077, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0xff770000, 0x77000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00007fff, 0x0007ffff, 0x007ffff7, 0x07ffff70,
	0x07fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000,
	/* Unused: the same half ring outlined in colour 4. */
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x40000000, 0xf4400000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x44440000, 0xffff4440, 0xfffffff4, 0x4444ffff,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00004444, 0x0444ffff, 0x4fffffff, 0xffff4444,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000004, 0x0000044f,
	0xfff40000, 0xffff4000, 0x4ffff400, 0x04ffff40,
	0x004fff40, 0x004ffff4, 0x0004fff4, 0x0004fff4,
	0x000044ff, 0x00000044, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0xff440000, 0x44000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00004fff, 0x0004ffff, 0x004ffff4, 0x04ffff40,
	0x04fff400, 0x4ffff400, 0x4fff4000, 0x4fff4000,
	/* Unused: three 8 by 8 sparkles of 256 colours. */
	0xe9000000, 0x000000e9, 0xeb000000, 0x000000eb,
	0xedeb0000, 0x0000ebed, 0xeeedebe9, 0xe9ebedee,
	0xeeedebe9, 0xe9ebedee, 0xedeb0000, 0x0000ebed,
	0xeb000000, 0x000000eb, 0xe9000000, 0x000000e9,
	0x0000e900, 0x00e90000, 0x00ebe9e9, 0xe9e9eb00,
	0xedebeb00, 0x00ebebed, 0xefed0000, 0x0000edef,
	0xefed0000, 0x0000edef, 0xedebeb00, 0x00ebebed,
	0x00ebe9e9, 0xe9e9eb00, 0x0000e900, 0x00e90000,
	0xe9000000, 0x000000e9, 0xebe90000, 0x0000e9eb,
	0xedebe900, 0x00e9ebed, 0xeeedebe9, 0xe9ebedee,
	0xeeedebe9, 0xe9ebedee, 0xedebe900, 0x00e9ebed,
	0xebe90000, 0x0000e9eb, 0xe9000000, 0x000000e9,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 137
#include "shared/teleport_circle_palette.c"

Sint32 MarsLighthouseAerieBeaconFireScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -(toFIXED(3) / 128)),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 128),
	SC_WAIT(1),
	SC_LOOP(4, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 MarsLighthouseAerieJennaApproachScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(496, 0, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAerieShebaApproachScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(480, 0, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAeriePiersApproachScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(488, 0, 320),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAerieIvanApproachScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(528, 0, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAerieKradenApproachScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(512, 0, 328),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAerieVoiceFadeScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(64, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 MarsLighthouseAerieDoomDragonRiseScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 128),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 128),
	SC_ADD(SW_Y, toFIXED(1) / 8),
	SC_WAIT(1),
	SC_LOOP(128, SC_TOP),
	SC_END,
};

Sint32 MarsLighthouseAerieLeaderReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(500, 0, 306),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieIsaacReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(524, 0, 306),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieJennaReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(488, 0, 320),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieShebaReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(472, 0, 304),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAeriePiersReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(464, 0, 280),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieIvanReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(538, 0, 320),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieMiaReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(552, 0, 304),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieGaretReadyScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(560, 0, 280),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieShebaToThosePeopleScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(472, 0, 320),
	SC_SYNC,
	SC_MOVE(472, 0, 304),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_SCENE_COUNTER, 1),
	SC_END,
};

Sint32 MarsLighthouseAerieJennaToThosePeopleScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(476, 0, 328),
	SC_SYNC,
	SC_MOVE(472, 0, 320),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_SCENE_COUNTER, 1),
	SC_END,
};

Sint32 MarsLighthouseAerieJennaTurnScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(486, 0, 286),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieLeaderTurnScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(504, 0, 300),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 MarsLighthouseAerieIsaacTurnScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(528, 0, 292),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The same script as MarsLighthouseAerieGaretReadyScript, under another */
/* name. */
Sint32 MarsLighthouseAerieGaretTurnScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(560, 0, 280),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieMiaTurnScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(548, 0, 292),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 MarsLighthouseAerieBeamWidenScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(32, SC_TOP),
	SC_END,
	SC_ADD(SW_SCALE_X, -896),
	SC_WAIT(1),
	SC_LOOP(140, SC_TOP),
	SC_END,
};

Sint32 MarsLighthouseAerieBeamNarrowScript[] = {
	SC_ADD(SW_SCALE_X, -896),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_END,
};

Uint32 MarsLighthouseAerieBeamShootUpScript[] = {
	SC_ADD(SW_Y, toFIXED(4)),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Uint32 MarsLighthouseAerieBeamRiseScript[] = {
	SC_ADD(SW_Y, toFIXED(2)),
	SC_WAIT(1),
	SC_LOOP(160, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 MarsLighthouseAerieBeaconSparkScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(0.8)),
	SC_SET(SW_SCALE_Y, toFIXED(0.8)),
	SC_ANIMATION(1),
	SC_LABEL(1),
	SC_ADD(SW_Y, -toFIXED(1)),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 MarsLighthouseAerieWatchGoldenSunRiseScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(8, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(8, SC_TO(1)),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(MarsLighthouseAerie),
	SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE),
	SCENE_EXIT(EXIT_TO_THE_BEACON, MarsLighthouseAerie, MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_AERIE),
	SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EAST_A, MercuryLighthouse, MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
	SCENE_EXIT(MARS_LIGHTHOUSE_AERIE_TO_PROX_HOUSES, ProxHouses, PROX_HOUSES_FROM_MARS_LIGHTHOUSE_AERIE),
	SCENE_EXITS_END,
};

ACTOR_PLACE MarsLighthouseAeriePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 584, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 616, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseAerieAfterBattlePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 512, 0, 236, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 354, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 496, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 544, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 478, 0, 266, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 506, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_DOOM_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 276, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 282, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 506, 0, 292, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 540, 0, 284, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseAerieBeaconPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 584, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 616, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 440, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 458, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 476, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 276, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 529, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 538, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 274, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 566, 0, 286, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 480, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 502, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 548, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_HAMMA, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), METATILE_TELEPORT_LANDING, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit)},
	{CUE_STEP, METATILE_BEFORE_BEACON, FLAG_MARS_BEACON_LIT, SCRIPT_REF(scMarsLighthouseAerieFaceDoomDragon)},
	{CUE_TALK, ACTOR_ISAAC, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieOurQuestIsAlmostOver)},
	{CUE_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIWishICouldConsole)},
	{CUE_TALK, ACTOR_MIA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIfAlexSetThisAll)},
	{CUE_TALK, ACTOR_JENNA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIThoughtIdGet)},
	{CUE_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieIDontKnowMyParents)},
	{CUE_TALK, ACTOR_PIERS, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieImSorryThingsHad)},
	{CUE_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, MSGNO(MsgMarsLighthouseAerieBothKyleAndYourParents)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_HAMMA, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToIvan)},
	{TALK_CUE_FACING(FACING_SOUTH), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToJennasMother)},
	{TALK_CUE_FACING(FACING_SOUTHWEST), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToJennasMother)},
	{TALK_CUE_FACING(FACING_NORTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToKyle)},
	{TALK_CUE_FACING(FACING_EAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToKyle)},
	{TALK_CUE_FACING(FACING_SOUTHEAST), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToKyle)},
	{CUE_TALK, ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieTalkToJennasFather)},
#if HALT_ON_MARS_AERIE_SPEAKERS
	{PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_HAMMA, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHaltIvan)},
	{PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_MOTHER_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHaltJennasMother)},
	{PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_KYLE_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHaltKyle)},
	{PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_JENNAS_FATHER_TALK_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHaltJennasFather)},
#endif
	{CUE_STEP, METATILE_KRADEN_STOPS_WEST, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHoldBackFromParents)},
	{CUE_STEP, METATILE_KRADEN_STOPS_EAST, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseAerieHoldBackFromParentsAgain)},
	{CUE_STEP, METATILE_AT_BEACON, FLAG_ASKED_FOR_MARS_STAR, SCRIPT_REF(scMarsLighthouseAerieOfferToCastMarsStar)},
	{CUE_STEP, METATILE_ROUND_BEACON, IF_FLAG_SET(FLAG_ASKED_FOR_MARS_STAR), SCRIPT_REF(scMarsLighthouseAerieClearCastOffer)},
	{CUE_TABLE_END},
};

Sint32 MarsLighthouseAerieBeaconFlameScript[] = {
	SC_SET(SW_SCALE_Y, toFIXED(1.4)),
	SC_SET(SW_SCALE_X, toFIXED(1.4)),
	SC_ANIMATION(4),
	SC_WAIT(30),
	SC_RELEASE,
};

#include "shared/orb_rising_script.c"
/* clang-format on */
