/* The Mars Lighthouse's dragon hall, high in the frozen lighthouse, where two
   Flame Dragons sleep in a block of ice below a great dragon's head carved in
   the north wall, with the corridor and the small room that lead to it.

   Two torches stand on stands before the frozen dragons, the east one burning.
   Blaze from the burning torch lights the other, which sends two sparkles round
   the hall, each launching a fireball into the ice, which cracks; Burst on the
   crack frees the dragons, which wake and fight. Beaten, they are Agatio and
   Karst, too weak and cold to go on: they ask Felix to light the beacon and
   leave him the Mars Star. Taken up into the Mythril Bag and set in the dragon
   head's face, the star turns the lighthouse back to fire and its ice is gone,
   here and in every room below. */

#include "graphics/palette_buffers.h"
#include "scene/flame_throwers.h"
#include "item_ids.h"
#include "field/field_mode.h"
#include "field/script_inline.h"
#include "scene/mars_lighthouse_star_room.h"
#include "scene/scene.h"
#include "edition.h"
#include "field/field_mode_script.h"
#include "field/field_sprite.h"
#include "shared/mars_lighthouse_entrance_room.h"
#include "field/call.h"
#include "window/window.h"
#include "macros.h"
#include "shared/script_check_spheres_overlap.h"
#include "field/actor_script.h"
#include "graphics/glyph.h"
#include "text/msg_ids.h"

/* Felix took it from where Agatio left it */
#define FLAG_MARS_STAR_TAKEN			0xa3a
#define FLAG_ADEPTS_WOKE				0xa3b /* Agatio and Karst woke after the battle */
/* The east fireball cracked the ice. */
#define FLAG_DRAGON_ICE_CRACKED			0xa4a
/* Set on coming back from the Flame Dragons' battle. */
#define FLAG_FLAME_DRAGONS_BEATEN		0xa4b
/* In every edition but the Japanese: the Mars Star was set in the statue once,
   and the lighthouse thawed; set again, it only rises and falls back. */
#define FLAG_MARS_STAR_SET				0xa9e
/* Both fireballs have reached the ice: Burst can break it. */
#define FLAG_BURST_CAN_BREAK_DRAGON_ICE FLAG_SCENE_TEMP_33
/* In every edition but the Japanese: the ice is burst, and fireballs still on
   their way no longer crack it. */
#define FLAG_DRAGON_ICE_BURST			FLAG_SCENE_TEMP_38

/* The room's own ways between its parts: the dragon hall's south end and the
   west doorway of a long corridor lead to each other, and the corridor's east
   doorway to the south end of a small room, whose north doorway leads out. Each
   exit leads to the entrance of the same name. */
enum {
	ENTRANCE_HALL_SOUTH_END = 1,
	ENTRANCE_CORRIDOR_WEST_DOORWAY,
	ENTRANCE_CORRIDOR_EAST_DOORWAY,
	ENTRANCE_SMALL_ROOM_SOUTH_END
};

/* The exits, each to the entrance of the same name. */
enum {
	EXIT_TO_CORRIDOR_WEST_DOORWAY = 1,
	EXIT_TO_HALL_SOUTH_END,
	EXIT_TO_SMALL_ROOM_SOUTH_END,
	EXIT_TO_CORRIDOR_EAST_DOORWAY
};

/* Raised by Burst once the ice is gone: the dragons wake. */
#define DRAGON_WAKE_CUE		 20
/* The metatiles of the ice the dragons sleep in, which Burst breaks. */
#define METATILE_DRAGON_ICE	 20
/* The metatile before the great dragon head on the north wall, which asks for
   something to be placed in it, and on which the Mars Star is used. */
#define METATILE_STATUE_FACE 21

enum {
	/* The two torches on stands before the frozen dragons: the east one burns,
	   the west one is unlit. Blaze's cast starts at the east one and hits the
	   west one, which sends out the sparkles. */
	ACTOR_EAST_TORCH = 8,
	ACTOR_WEST_TORCH = 9,
	/* The crack the fireballs leave in the ice the dragons sleep in. */
	ACTOR_ICE_CRACK = 10,
	ACTOR_AGATIO_DRAGON = 11, /* the Flame Dragon that was Agatio */
	ACTOR_KARST_DRAGON = 12,  /* the Flame Dragon that was Karst */
	ACTOR_AGATIO = 13,
	ACTOR_KARST = 14,
	/* The lights that rise from the dragons as they turn back into Adepts. */
	ACTOR_AGATIO_LIGHT = 15,
	ACTOR_KARST_LIGHT = 16,
	/* The sparkles Blaze sends from the west torch round each side of the hall;
	   at the end of its way each launches its fireball, which rolls into the
	   ice and cracks it. */
	ACTOR_EAST_SPARKLE = 17,
	ACTOR_WEST_SPARKLE = 18,
	ACTOR_EAST_FIREBALL = 19,
	ACTOR_WEST_FIREBALL = 20
};

/* A torch's script call: the torches stand without a shadow. */
Sint32 scMarsLighthouseStarRoomSetUpTorch(ACTOR *actor)
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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Felix casts Blaze at the burning torch on the east stand before the frozen
   dragons, and its jet of flame comes out long and fast. Cast westward from
   beside it, the jet runs through the torch and on across the floor to the
   unlit torch on the west stand, which flashes white and gives off sparks.

   The cast sets `blazeLongSpray`, which gives Blaze its long jet in place of
   the short one; every cast clears it again as it starts. */
void scMarsLighthouseStarRoomStartBlazeOnEastTorch(void)
{
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	event->blazeLongSpray = 1;
}

/* Blaze's flame reaches the unlit torch on the west stand before the frozen
   dragons, which sends out the two sparkles that launch the fireballs. */
void scMarsLighthouseStarRoomBlazeWestTorch(void)
{
	scMarsLighthouseStarRoomShootFireballs();
}

/* Nothing happens when Blaze ends at the west torch; the sparkles and fireballs
   go on alone. */
void scMarsLighthouseStarRoomEndBlazeOnWestTorch(void)
{
}

void scActorSetupScriptedPart(ACTOR *actor, Sint32 pri);

/* Creates one of the dragon hall's effects at a point in the room. */
static __inline__ ACTOR *scMarsLighthouseStarRoomCreateActor(
	Sint32 kind, Sint32 x, Sint32 y, Sint32 z)
{
	return syImportActorCreate(kind, x, y, z);
}

extern Sint16 SlideOnIceSteerAngles[];

extern Uint32 DebrisScript[];

/* Six chips of ice burst from an actor and bounce away: the wind stone's
   sprite, tinted to ice. Each flies off on a random heading at up to 8 pixels a
   tick, and rises at 2 to 6, higher the further round its heading is. */
void scMarsLighthouseStarRoomScatterIceChips(Sint32 id)
{
	ACTOR *src = fdActorGet(id);
	Sint32 i;

	for(i = 0; i < 6; i++) {
		FIXED  x = src->x.fixed + (syRandom() << 4) - (syRandom() << 4);
		ACTOR *actor = scMarsLighthouseStarRoomCreateActor(SPRITE_WIND_STONE, x,
			(syRandom() << 3) + src->y.fixed + toFIXED(4),
			src->z.fixed + (syRandom() << 4) - (syRandom() << 4));

		if(actor != NULL) {
			Sint32 heading;
			Sint32 speed;
			Sint32 scale;

			heading = syRandom();
			speed = syRandom();
			scale = ((Uint32)syRandom() >> 1) + toFIXED(1) / 4;
			fdActorStartScript(actor, DebrisScript);
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

/* The west fireball reaches the ice: a small crack shows, unless the east one
   came first. */
void scMarsLighthouseStarRoomWestFireballCracksIce(void)
{
	ACTOR *actor = fdActorGet(ACTOR_ICE_CRACK);

#if !DRAGON_ICE_CRACK_CHECKS_BURST
	if(fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !DRAGON_ICE_STAYS_BROKEN
	if(fdScriptGetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
		fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
	if(fdScriptGetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
		fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
		fdScriptPlaySnd(SE_ICE_CRACK);
		actor->x.fixed = toFIXED(actor->homeX);
		actor->z.fixed = toFIXED(actor->homeZ);
		if(fdScriptGetFlag(FLAG_DRAGON_ICE_CRACKED) == 0) {
			actor->scaleX = toFIXED(3) / 5;
			actor->scaleY = toFIXED(3) / 5;
		}
	}
}

/* The east fireball reaches the ice: the whole crack shows, and Burst can break
   it. */
void scMarsLighthouseStarRoomEastFireballCracksIce(void)
{
	ACTOR *actor = fdActorGet(ACTOR_ICE_CRACK);

#if !DRAGON_ICE_CRACK_CHECKS_BURST
	if(fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#elif !DRAGON_ICE_STAYS_BROKEN
	if(fdScriptGetFlag(FLAG_DRAGON_ICE_BURST) == 0 ||
		fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#else
	if(fdScriptGetFlag(FLAG_DRAGON_ICE_BURST) == 0 &&
		fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN) == 0) {
#endif
		fdScriptPlaySnd(SE_ICE_CRACK);
		fdScriptSetFlag(FLAG_BURST_CAN_BREAK_DRAGON_ICE);
		actor->x.fixed = toFIXED(actor->homeX);
		actor->z.fixed = toFIXED(actor->homeZ);
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
		fdScriptSetFlag(FLAG_DRAGON_ICE_CRACKED);
	}
}

void scMarsLighthouseStarRoomClearDragonIce(void);

/* Felix casts Burst on the cracked ice the Flame Dragons sleep in, metatiles
   (41, 14) to (43, 14): a flash, the ice gone, chips flying, and the dragons
   wake. */
void scMarsLighthouseStarRoomBurstIce(Sint32 phase, Sint32 frm)
{
	if(phase == 1) {
		fdScriptPlaySnd(SE_FLASH);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeInterpolate(60);
		scMarsLighthouseStarRoomClearDragonIce();
		fdActorSetPos(ACTOR_ICE_CRACK, 0, 0);
#if DRAGON_ICE_CRACK_CHECKS_BURST
		fdScriptSetFlag(FLAG_DRAGON_ICE_BURST);
#endif
		FieldStateWork->raisedCue = DRAGON_WAKE_CUE;
	}
	/* From tick 97 to 271, chips burst from the ice every 30 ticks. */
	if((Uint32)(frm - 97) <= 174 && frm % 30 == 0)
		scMarsLighthouseStarRoomScatterIceChips(ACTOR_ICE_CRACK);
	if(frm == 276) {
		fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
		fdPaletteFadeInterpolate(20);
	}
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

#include "shared/actor_freeze.h"

void scMarsLighthouseStarRoomWakeAfterBattle(void);
void scMarsLighthouseStarRoomCreateMarsStar(Sint32 rise);
void scMarsLighthouseStarRoomLayDownAdept(Sint32 actor);
void scFlameThrowersCreate(void);
void scCrackedTileDropIn(void);
ACTOR *MarsLighthouseStarRoomMarsStar BSS_DATA;

Sint32 scInit(void)
{
	Sint32 scene = MainRoutineStateWork.next;
	Sint32 entrance = MainRoutineStateWork.entrance;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	MarsLighthouseStarRoomMarsStar = NULL;
	fdScriptSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
	scFlameThrowersCreate();
	scFlameThrowersStart(0, 0, 0, STEP_CUE_FLAME_HIT);
	if(scene == (Sint32)&SceneId_MarsLighthouseStarRoom) {
		fdEffectStartBlankOverlay(0);
		if(fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
			/* The ice is gone from the hall. */
			fdMapGridCopyMetatileAttributes(65, 65, 9, 21, 38, 24);
			fdMapGridCopyMetatilesLayered(65, 65, 9, 21, 38, 24);
			fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 38, 87);
			fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 102, 23);
		}
		/* Through the crack in the floor above: the party lands below it. */
		if(entrance ==
			MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_B) {
			ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
			FIELD_STATE *state = Ram_HeapSlots->fieldState;
			Sint32		 column = scFieldGameFlagGetByte(FLAG_BYTE_FALL_COLUMN);
			Sint32		 row = scFieldGameFlagGetByte(FLAG_BYTE_FALL_ROW);
			ACTOR		*other;
			Sint32		 x;

			x = FIELD_METATILE_CENTER(column);
			leader->x.fixed = x;
			leader->z.fixed = FIELD_METATILE_ORIGIN(row) - toFIXED(296);
			other = state->camera;
			other->x.fixed = x;
			other->z.fixed = leader->z.fixed;
			fdMapGridCenterOnFollowedPoint();
			if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
				scCrackedTileDropIn();
		}
		if(entrance ==
				MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_EAST ||
			entrance ==
				MARS_LIGHTHOUSE_STAR_ROOM_FROM_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_WEST) {
			if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION))
				scCrackedTileDropIn();
		}
		if(entrance == ENTRANCE_HALL_SOUTH_END ||
			entrance == MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE) {
			if(!fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
				scFieldActorSetSprPriority(ACTOR_AGATIO_DRAGON, 3);
				scFieldActorSetSprPriority(ACTOR_KARST_DRAGON, 3);
				fdActorGetById(ACTOR_KARST_DRAGON)->scaleX = -toFIXED(1);
			}
			if(fdScriptGetFlag(FLAG_FLAME_DRAGONS_BEATEN)) {
				scMarsLighthouseStarRoomClearDragonIce();
				fdActorSetPos(ACTOR_ICE_CRACK, 0, 0);
			} else if(fdScriptGetFlag(FLAG_DRAGON_ICE_CRACKED)) {
				fdScriptSetFlag(FLAG_BURST_CAN_BREAK_DRAGON_ICE);
			} else {
				fdActorSetPos(ACTOR_ICE_CRACK, 0, 0);
			}
			if(fdScriptGetFlag(FLAG_ADEPTS_WOKE)) {
				if(!fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED)) {
					fdActorSetPos(ACTOR_AGATIO_DRAGON, 0, 0);
					fdActorSetPos(ACTOR_KARST_DRAGON, 0, 0);
				}
				scMarsLighthouseStarRoomClearDragonIce();
				syWait(10);
				if(!fdScriptGetFlag(FLAG_MARS_STAR_TAKEN)) {
					scMarsLighthouseStarRoomCreateMarsStar(0);
					if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
						scMarsLighthouseStarRoomLayDownAdept(ACTOR_AGATIO);
						scMarsLighthouseStarRoomLayDownAdept(ACTOR_KARST);
					}
				}
				if(fdActorGetById(ACTOR_AGATIO)->x.fixed > toFIXED(400)) {
					fdMapGridCopyMetatileAttributes(39, 22, 2, 1, 43, 13);
					fdMapGridCopyMetatileAttributes(31, 21, 2, 2, 40, 12);
				}
			} else if(entrance ==
				MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE) {
				scMarsLighthouseStarRoomWakeAfterBattle();
			}
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
		fdMapCopyPalette(
			MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS,
			(Sint32)BG_PLTT);
	if(MainRoutineStateWork.entrance ==
		MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE)
		fdScriptSetFlag(FLAG_FLAME_DRAGONS_BEATEN);
	return 0;
}

/* Free of the ice, the Flame Dragons rise and roar, and the battle begins. */
void scMarsLighthouseStarRoomWakeFlameDragons(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scMarsLighthouseStarRoomClearDragonIce();
	fdScriptPlaySnd(MUS_SILENCE);
	syFieldScriptWait(40);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, 8);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 4, 10);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, 16);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 20);
	fdScriptPlaySnd(MUS_BATTLE_WITH_FLAME_DRAGON);
	fdActorSetAnimation(ACTOR_AGATIO_DRAGON, 2);
	fdActorSetAnimation(ACTOR_KARST_DRAGON, 2);
	syFieldScriptWait(24);
	fdActorSetPos(ACTOR_AGATIO_DRAGON, 696, 232);
	fdActorSetPos(ACTOR_KARST_DRAGON, 664, 232);
	fdActorSetAnimation(ACTOR_AGATIO_DRAGON, 3);
	fdActorSetAnimation(ACTOR_KARST_DRAGON, 3);
	fdScriptPlaySnd(SE_TREMOR);
	fdMapGridSetShake(2, 4, 1);
	syFieldScriptWait(20);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(1, 2, 1);
	syFieldScriptWait(20);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	syFieldScriptWait(20);
	fdScriptPlaySnd(SE_BOSS_ROAR);
	fdActorSetAnimation(ACTOR_AGATIO_DRAGON, 4);
	fdActorSetAnimation(ACTOR_KARST_DRAGON, 4);
	syFieldScriptWait(60);
	MainRoutineStateWork.battleType = BATTLE_TYPE_NO_ESCAPE;
	syFieldScriptSetAfterBattleDestination(
		(Sint32)&SceneId_MarsLighthouseStarRoom,
		MARS_LIGHTHOUSE_STAR_ROOM_AFTER_FLAME_DRAGONS_BATTLE);
	MainRoutineStateWork.battleBg = SCENE_GONDOWAN_CLIFFS_HIDEOUT;
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 0);
}

extern Uint8  MsgMarsLighthouseStarRoomWhyWhyAmILying[];
extern Sint32 MarsLighthouseStarRoomLightRiseScript[];
extern Sint32 MarsLighthouseStarRoomLightSinkScript[];
extern Sint32 MarsLighthouseStarRoomAgatioDragonShrinkScript[];
extern Sint32 MarsLighthouseStarRoomKarstDragonShrinkScript[];

/* After the battle the dragons shrink into two lights that sink to the floor,
   and Agatio and Karst lie there: lost in the frozen lighthouse, its eye took
   their will, and they ask Felix to light the beacon with the Mars Star. */
void scMarsLighthouseStarRoomWakeAfterBattle(void)
{
	Sint32 answer;
	ACTOR *dragon, *agatioLight, *karstLight;

	dragon = fdActorGet(ACTOR_AGATIO_DRAGON);
	agatioLight = fdActorGet(ACTOR_AGATIO_LIGHT);
	karstLight = fdActorGet(ACTOR_KARST_LIGHT);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scMarsLighthouseStarRoomClearDragonIce();
	fdActorSetSprFlags(fdActorGet(ACTOR_AGATIO), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_KARST), 0);
	scFieldActorSetSprCol(fdActorGet(ACTOR_AGATIO_LIGHT), SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_KARST_LIGHT), SPRITE_COLOR_WHITE);
	syWait(1);
	scFieldActorSetPosAndResetMotion(
		dragon, dragon->x.fixed, 0, dragon->z.fixed - toFIXED(8));
	agatioLight->motionFlg = 0;
	agatioLight->scaleX = toFIXED(1) / 4;
	agatioLight->scaleY = toFIXED(1) / 4;
	karstLight->motionFlg = 0;
	karstLight->scaleX = toFIXED(1) / 4;
	karstLight->scaleY = toFIXED(1) / 4;
	fdCameraMoveTo(680, CAMERA_KEEP, 228, 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 680, 264);
	syWait(1);
	fdMapGridCenterOnFollowedPoint();
	syWait(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syFieldScriptWait(20);
	fdScriptPlaySnd(SE_LIGHT_APPEAR);
	scFieldActorSetSprCol(fdActorGet(ACTOR_AGATIO_DRAGON), SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_KARST_DRAGON), SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_AGATIO), SPRITE_COLOR_WHITE);
	scFieldActorSetSprCol(fdActorGet(ACTOR_KARST), SPRITE_COLOR_WHITE);
	syFieldScriptWait(20);
	fdScriptPlaySnd(SE_WARP);
	fdActorPlaceOnActor(ACTOR_AGATIO_LIGHT, ACTOR_AGATIO_DRAGON);
	fdActorPlaceOnActor(ACTOR_KARST_LIGHT, ACTOR_KARST_DRAGON);
	syWait(1);
	fdActorSetScript(ACTOR_AGATIO_DRAGON,
		(Sint32)&MarsLighthouseStarRoomAgatioDragonShrinkScript);
	fdActorSetScript(ACTOR_KARST_DRAGON,
		(Sint32)&MarsLighthouseStarRoomKarstDragonShrinkScript);
	fdActorSetScript(
		ACTOR_AGATIO_LIGHT, (Sint32)&MarsLighthouseStarRoomLightRiseScript);
	fdActorRunScriptAndWait(
		ACTOR_KARST_LIGHT, (Sint32)&MarsLighthouseStarRoomLightRiseScript);
	scMarsLighthouseStarRoomLayDownAdept(ACTOR_AGATIO);
	scMarsLighthouseStarRoomLayDownAdept(ACTOR_KARST);
	fdActorSetScript(
		ACTOR_AGATIO_LIGHT, (Sint32)&MarsLighthouseStarRoomLightSinkScript);
	fdActorRunScriptAndWait(
		ACTOR_KARST_LIGHT, (Sint32)&MarsLighthouseStarRoomLightSinkScript);
	syFieldScriptWait(10);
	scFieldActorSetSprCol(fdActorGet(ACTOR_AGATIO), SPRITE_COLOR_NORMAL);
	scFieldActorSetSprCol(fdActorGet(ACTOR_KARST), SPRITE_COLOR_NORMAL);
	syFieldScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	fdActorSetAnimation(ACTOR_AGATIO, 5);
	syFieldScriptWait(20);
	syFieldScriptSetMessageId((Sint32)&MsgMarsLighthouseStarRoomWhyWhyAmILying);
	/* "Why... Why am I lying here?" */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 80);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 1);
	syFieldScriptWait(20);
	fdScriptPlaySnd(MUS_KARST_AND_AGATIO);
	/* "Wait, I think I remember... We were going to light the beacon..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_KARST, 1);
	fdActorSetAnimation(ACTOR_KARST, 5);
	syFieldScriptWait(20);
	/* "But the lighthouse was frozen. We wandered lost inside, trying to..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 20);
	/* "We must hurry... If we delay, Prox is destroyed." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorRunRepeatedMotion(ACTOR_AGATIO, 2);
	fdActorSetAnimation(ACTOR_AGATIO, 6);
	/* "It's no use... I'm absolutely drained... I can't even stand." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetAnimation(ACTOR_KARST, 6);
	/* "And the cold... Why am I so cold? We of the Fire Clan are meant
	   to..." */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 20);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I feel the cold too, Karst... I... I'm afraid we're going to die
	   up..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	scFieldActorShowEmote(ACTOR_KARST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "I can feel it now... The long, dark sleep creeping into my soul..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	/* "Wait... I think I remember..." */
	syFieldScriptShowMessageThenWait(ACTOR_AGATIO, 0, 20);
	/* "The eye... It told us that we lack the will to go any farther." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetAnimation(ACTOR_KARST, 7);
	/* "The eye... I remember now... I felt its gaze pierce me to the
	   core..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	/* "That's right... And I remember fighting against Felix..." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	/* "I do, too... Was it a dream? An illusion?" */
	syFieldScriptShowMessageThenWait(ACTOR_KARST, 0, 40);
	fdActorShowEmote(ACTOR_AGATIO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Felix? Is that you?" */
	fdScriptOpenMessage(ACTOR_AGATIO, 0);
	answer = syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		fdActorSetAnimation(ACTOR_AGATIO, 7);
		fdScriptShowMessage(ACTOR_AGATIO, 0);
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg++;
		fdActorWaitForAnimationChange(MainRoutineStateWork.leaderActorId);
		fdActorSetAnimation(ACTOR_AGATIO, 6);
		fdScriptShowMessage(ACTOR_AGATIO, 0);
	}
	syFieldScriptWait(40);
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	scFieldActorStartRepeatedMotion(ACTOR_KARST, 2);
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	scMarsLighthouseStarRoomCreateMarsStar(1);
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorSetAnimationAndWait(ACTOR_AGATIO, 7);
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetAnimation(ACTOR_AGATIO, 10);
	fdActorSetAnimation(ACTOR_KARST, 9);
	fdScriptSetFlag(FLAG_ADEPTS_WOKE);
	gmPlayMusic();
	fdScriptEnd();
}

/* The Mars Star's update: drawn at the leader's sprite priority. */
void scMarsLighthouseStarRoomDrawStarWithLeader(ACTOR *actor)
{
	ACTOR *owner = fdActorGet(MainRoutineStateWork.leaderActorId);

	((FIELD_SPR *)actor->spr)->pri = ((FIELD_SPR *)owner->spr)->pri;
}

/* The Mars Star as an actor on the floor by Agatio, rising first when asked. */
void scMarsLighthouseStarRoomCreateMarsStar(Sint32 rise)
{
	ACTOR		 **slot = &MarsLighthouseStarRoomMarsStar;
	ICON_TRANSFER *glyph;
	ACTOR		  *actor;
	FIELD_SPR	  *spr;
	Sint32		   size;
	Uint8		   clear = 0;

	actor =
		scFieldActorCreate(SPRITE_UNSEEN_MARKER, toFIXED(702), 0, toFIXED(224));
	*slot = actor;
	if(actor == NULL)
		return;
	spr = actor->spr;
	spr->fullCol = 0;
	spr->pal = 0;
	spr->pri = 1;
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
	actor->update = scMarsLighthouseStarRoomDrawStarWithLeader;
	size = sizeof(ICON_TRANSFER);
	spr->flg = clear;
	spr->cnt = clear;
	glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, size);
	gmItemLoadIcon(ITEM_MARS_STAR);
	mnVRAMLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	if(rise) {
		syWait(1);
		(*slot)->velY = toFIXED(5) / 2;
		syWait(4);
		fdScriptPlaySnd(SE_CLICK);
		syWait(8);
		fdScriptPlaySnd(SE_CLICK);
	}
}

/* Felix talks to Karst, lying on the floor of the dragon hall where her dragon
   fell: "Your hand...so warm... People have such warm hands..." */
void scMarsLighthouseStarRoomHoldWarmHand(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_KARST, 5);
	syFieldScriptSetMessageId(
		(Sint32)MsgMarsLighthouseStarRoomYourHandSoWarmPeople);
	/* "Your hand...so warm... People have such warm hands... I had
	   forgotten..." */
	fdScriptShowMessage(ACTOR_KARST, 0);
	fdActorSetAnimation(ACTOR_KARST, 9);
	fdScriptEnd();
}

/* Felix talks to Agatio, lying on the floor of the dragon hall: "It's too late
   for us now... Hurry, light the beacon." The first time, the Mars Star flies
   up from the floor to Felix and goes into the Mythril Bag, and the view turns
   to the great dragon head on the north wall, which glows and speaks: "You who
   hold the star, open your heart..." */
void scMarsLighthouseStarRoomTakeMarsStar(void)
{
	ACTOR  *leader;
	ACTOR  *actor;
	Sint32	i;
	Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_AGATIO, 5);
	fdScriptSetMessageId((Sint32)MsgMarsLighthouseStarRoomItsTooLateForUs);
	/* "It's too late for us now... Hurry, light the beacon." */
	fdScriptShowMessage(ACTOR_AGATIO, 0);
	fdActorSetAnimation(ACTOR_AGATIO, 10);
	if(!fdScriptGetFlag(FLAG_MARS_STAR_TAKEN)) {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
		actor = MarsLighthouseStarRoomMarsStar;
		actor->motionFlg = 0;
		actor->speedLimit = toFIXED(4);
		actor->acceleration = toFIXED(2);
		scFieldActorSetMoveTargetFX(actor, leader->x.fixed,
			leader->y.fixed + toFIXED(32), leader->z.fixed);
		scFieldActorWaitForMove(MarsLighthouseStarRoomMarsStar);
		fdScriptPlaySnd(MUS_TREASURE_FOUND);
		gmPartyInventoryRemove(ITEM_MYTHRIL_BAG);
		gmInventoryAddItem(FELIX, ITEM_MYTHRIL_BAG_MARS_STAR);
		/* "Felix got the Mars Star!" */
		mnMessageShowCentered(
			(Sint32)MsgMarsLighthouseStarRoomFelixGotMarsStar, 1);
		for(i = 0; i < INVENTORY_SLOTS; i++) {
			if((syUnitGet(FELIX)->inventory[i] & INVENTORY_ITEM_ID_MASK) ==
				ITEM_MYTHRIL_BAG_MARS_STAR) {
				gmInventoryEquip(FELIX, i);
				break;
			}
		}
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		fdScriptSetFlag(FLAG_MARS_STAR_TAKEN);
		scFieldActorSetPosAndResetMotion(
			MarsLighthouseStarRoomMarsStar, 0, 0, 0);
		syWait(1);
		fdActorRelease(MarsLighthouseStarRoomMarsStar);
		syWait(1);
		fdCameraSetSpeed(4.8);
		fdCameraMoveTo(680, CAMERA_KEEP, 146, 1);
		fdCameraWaitForArrival();
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
		fdPaletteFadeInterpolate(20);
		syWait(20);
		*color = RGB_WHITE;
		syWait(1);
		mnFieldMessageShowCentered(
			(Sint32)MsgMarsLighthouseStarRoomYouWhoHoldStarOpen, 0, 0);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeInterpolate(20);
		syWait(20);
	}
	fdScriptEnd();
}

/* The block of ice the dragons slept in is gone from the floor. */
void scMarsLighthouseStarRoomClearDragonIce(void)
{
	fdMapGridCopyMetatilesTo(46, 68, 40, 73, 5, 6);
	fdMapGridCopyMetatileAttributes(104, 9, 5, 6, 40, 9);
}

/* Agatio or Karst lies on the floor where the dragon fell. */
void scMarsLighthouseStarRoomLayDownAdept(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	fdActorSetSprFlags(fdActorGet(id), 0);
	scFieldActorSetSprPriority(id, 3);
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->collisionFlg = 0;
	if(id == ACTOR_AGATIO) {
		fdActorSetPos(ACTOR_AGATIO, 700, 224);
		fdActorSetAnimation(ACTOR_AGATIO, 10);
		fdMapGridCopyMetatileAttributes(39, 22, 2, 1, 43, 13);
	} else {
		fdActorSetPos(ACTOR_KARST, 660, 216);
		fdActorSetAnimation(ACTOR_KARST, 9);
		fdMapGridCopyMetatileAttributes(31, 21, 2, 2, 40, 12);
	}
}

/* Felix examines the great dragon head on the north wall of the dragon hall,
   metatile (42, 8): "There's a space for you to place something." Then the
   field menu opens, for the party to choose what to place. */
void scMarsLighthouseStarRoomExamineStatue(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	/* "There's a space for you to place something." */
	mnMessageShowCentered((Sint32)MsgMarsLighthouseStarRoomTheresSpace, 1);
	state->fieldMenu = 1;
	fdScriptEnd();
}

void scMarsLighthouseStarRoomSpawnRisingSparks(void);

#if !MARS_STAR_REUSE_SHORT_SCENE

/* Felix uses the Mars Star on the great dragon head on the north wall of the
   dragon hall and sets it in the head's face: the room shakes, sparks rise, the
   ice melts into lava and the lighthouse turns back to fire; the star falls
   back out. */
void scMarsLighthouseStarRoomUseMarsStar(void)
{
	ACTOR		 **slot;
	ACTOR		  *actor;
	FIELD_SPR	  *spr;
	ICON_TRANSFER *glyph;
	PALETTE_FADE  *fade;

	fade = Ram_HeapSlots->palFade;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	/* "Felix used the Mars Star!" */
	mnMessageShowCentered(
		(Sint32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syWait(1);
	fdScriptPlaySnd(SE_GEM_SET);
	slot = &MarsLighthouseStarRoomMarsStar;
	actor = scFieldActorCreate(
		SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8), toFIXED(147));
	*slot = actor;
	if(actor != 0) {
		spr = actor->spr;
		spr->flg = 0;
		spr->fullCol = 0;
		spr->pal = 0;
		spr->pri = 2;
		actor->motionFlg = 0;
		actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		spr->cnt = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		gmItemLoadIcon(ITEM_MARS_STAR);
		mnVRAMLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	syFieldScriptWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 680, 186);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 40);
	fdScriptPlaySnd(MUS_SILENCE);
	fdScriptPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	syFieldScriptWait(20);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdMapGridSetShake(1, 1, 1);
	syFieldScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
	fdPaletteFadeInterpolate(80);
	syFieldScriptWait(80);
	sySetTickEvent(scMarsLighthouseStarRoomSpawnRisingSparks, EVENT_PASS_TICK);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(680, CAMERA_KEEP, 480, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(80);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(80);
	syWait(60);
	syFieldCloseEvent(scMarsLighthouseStarRoomSpawnRisingSparks);
	syWait(20);
	fdMapGridSetShake(0, 0, 0);
	syFieldScriptWait(20);
	fdMapGridCopyMetatileAttributes(65, 65, 9, 21, 38, 24);
	fdMapGridCopyMetatilesLayered(65, 65, 9, 21, 38, 24);
	fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 38, 87);
	fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 102, 23);
	fdMapCopyPalette(
		MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS, fade);
	fade->pal[0] = RGB(0, 0, 0); /* the backdrop */
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	syWait(160);
	fdCameraMoveTo(680, CAMERA_KEEP, 186, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	actor = *slot;
	if(actor != 0) {
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		actor->velY = toFIXED(2);
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1);
		scFieldActorSetMoveTarget(actor, 672, 0, 160);
		scFieldActorWaitForMove(*slot);
		fdScriptPlaySnd(SE_CLICK);
		actor = *slot;
		actor->acceleration = toFIXED(1) / 2;
		actor->velY = toFIXED(4);
		actor->speedLimit = toFIXED(1);
		scFieldActorSetMoveTarget(actor, 664, 0, 158);
		scFieldActorWaitForMove(*slot);
	}
	syFieldScriptWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 670, 170);
	syFieldScriptWait(1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	actor = *slot;
	if(actor != 0)
		fdActorRelease(actor);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 680, 186);
	syFieldScriptWait(1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	gmPlayMusic();
	fdScriptSetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
	MainRoutineStateWork.battleBg = SCENE_NARIBWE;
	fdScriptEnd();
}

#else

/* Felix uses the Mars Star on the great dragon head on the north wall of the
   dragon hall and sets it in the head's face: the room shakes, sparks rise, the
   ice melts into lava and the lighthouse turns back to fire; the star falls
   back out. */
void scMarsLighthouseStarRoomUseMarsStar(void)
{
	Sint32		   flg;
	ACTOR		 **slot, **placedStarSlot;
	ACTOR		  *actor, *placedStar;
	FIELD_SPR	  *spr, *placedStarSpr;
	ICON_TRANSFER *glyph, *placedStarGlyph;
	Uint16		  *palette;

	flg = fdScriptGetFlag(FLAG_MARS_STAR_SET);
	if(flg == 0) {
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		/* "Felix used the Mars Star!" */
		mnMessageShowCentered(
			(Sint32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		syWait(1);
		fdScriptPlaySnd(SE_GEM_SET);
		slot = &MarsLighthouseStarRoomMarsStar;
		actor = (ACTOR *)scFieldActorCreate(
			SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8), toFIXED(147));
		*slot = actor;
		if(actor != 0) {
			spr = actor->spr;
			spr->flg = 0;
			spr->fullCol = 0;
			spr->pal = 0;
			spr->pri = 2;
			actor->motionFlg = 0;
			actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
			spr->cnt = 0;
			actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			glyph =
				syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
			gmItemLoadIcon(ITEM_MARS_STAR);
			mnVRAMLoad(spr->slot, ICON_TILE_BYTES, glyph->tiles);
			syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		}
		syFieldScriptWait(20);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 680, 186);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_NORTH, 80);
		fdActorShowEmote(MainRoutineStateWork.leaderActorId,
			EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		actor = *slot;
		if(actor != 0) {
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			actor->velY = toFIXED(2);
			actor->speedLimit = toFIXED(2);
			actor->acceleration = toFIXED(1);
			scFieldActorSetMoveTarget(actor, 672, 0, 160);
			scFieldActorWaitForMove((Sint32)*slot);
			fdScriptPlaySnd(SE_CLICK);
			actor = *slot;
			actor->acceleration = toFIXED(1) / 2;
			actor->velY = toFIXED(4);
			actor->speedLimit = toFIXED(1);
			scFieldActorSetMoveTarget(actor, 664, 0, 158);
			scFieldActorWaitForMove((Sint32)*slot);
		}
		syFieldScriptWait(20);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
			~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 670, 170);
		syFieldScriptWait(1);
		fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
			ACTOR_FACES_MOTION;
		actor = *slot;
		if(actor != 0)
			fdActorRelease(actor);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		gmPlayMusic();
		fdScriptEnd();
		return;
	}
	palette = (Uint16 *)Ram_HeapSlots->palFade;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	/* "Felix used the Mars Star!" */
	mnMessageShowCentered(
		(Sint32)MsgMarsLighthouseStarRoomFelixUsedMarsStar, 1);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	syWait(1);
	fdScriptPlaySnd(SE_GEM_SET);
	placedStarSlot = &MarsLighthouseStarRoomMarsStar;
	placedStar = scFieldActorCreate(
		SPRITE_UNSEEN_MARKER, toFIXED(680), toFIXED(8), toFIXED(147));
	*placedStarSlot = placedStar;
	if(placedStar != 0) {
		placedStarSpr = placedStar->spr;
		placedStarSpr->flg = 0;
		placedStarSpr->cnt = 0;
		placedStarSpr->fullCol = 0;
		placedStarSpr->pal = 0;
		placedStarSpr->pri = 2;
		placedStar->motionFlg = 0;
		placedStar->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		placedStar->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		placedStarGlyph =
			syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		gmItemLoadIcon(ITEM_MARS_STAR);
		mnVRAMLoad(
			placedStarSpr->slot, ICON_TILE_BYTES, placedStarGlyph->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	syFieldScriptWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 680, 186);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 40);
	fdScriptPlaySnd(MUS_SILENCE);
	fdScriptPlaySnd(SE_RUMBLE);
	fdMapGridSetShake(2, 2, 1);
	syFieldScriptWait(20);
	fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdMapGridSetShake(1, 1, 1);
	syFieldScriptWait(20);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_RED, PALETTES_ALL);
	fdPaletteFadeInterpolate(80);
	syFieldScriptWait(80);
	sySetTickEvent(scMarsLighthouseStarRoomSpawnRisingSparks, EVENT_PASS_TICK);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(680, CAMERA_KEEP, 480, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(80);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdPaletteFadeApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
	fdPaletteFadeInterpolate(80);
	syWait(60);
	syFieldCloseEvent(scMarsLighthouseStarRoomSpawnRisingSparks);
	syWait(20);
	fdMapGridSetShake(0, 0, 0);
	syFieldScriptWait(20);
	fdMapGridCopyMetatileAttributes(65, 65, 9, 21, 38, 24);
	fdMapGridCopyMetatilesLayered(65, 65, 9, 21, 38, 24);
	fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 38, 87);
	fdMapGridCopyMetatilesLayered(76, 64, 9, 22, 102, 23);
	fdMapCopyPalette(
		MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS, palette);
	*palette = 0;
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeInterpolate(120);
	syWait(160);
	fdCameraMoveTo(680, CAMERA_KEEP, 186, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	placedStar = *placedStarSlot;
	if(placedStar != 0) {
		placedStar->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		placedStar->velY = toFIXED(2);
		placedStar->speedLimit = toFIXED(2);
		placedStar->acceleration = toFIXED(1);
		scFieldActorSetMoveTarget(placedStar, 672, 0, 160);
		scFieldActorWaitForMove((Sint32)*placedStarSlot);
		fdScriptPlaySnd(SE_CLICK);
		placedStar = *placedStarSlot;
		placedStar->acceleration = toFIXED(1) / 2;
		placedStar->velY = toFIXED(4);
		placedStar->speedLimit = toFIXED(1);
		scFieldActorSetMoveTarget(placedStar, 664, 0, 158);
		scFieldActorWaitForMove((Sint32)*placedStarSlot);
	}
	syFieldScriptWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg &=
		~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 670, 170);
	syFieldScriptWait(1);
	fdActorGet(MainRoutineStateWork.leaderActorId)->steeringFlg |=
		ACTOR_FACES_MOTION;
	placedStar = *placedStarSlot;
	if(placedStar != 0)
		fdActorRelease(placedStar);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	gmPlayMusic();
	fdScriptSetFlag(FLAG_MARS_LIGHTHOUSE_THAWED);
	fdScriptSetFlag(FLAG_MARS_STAR_SET);
	MainRoutineStateWork.battleBg = SCENE_NARIBWE;
	fdScriptEnd();
}

#endif

#define RISING_SPARK_TICKS			   60 /* a spark's life */
/* The ticks a rising spark has left, kept where an actor keeps its watched
   actor. */
#define RISING_SPARK_TICKS_LEFT(actor) (*(Sint32 *)&(actor)->watchedActor)

/* A spark rises and grows for a second, then is gone. */
void scMarsLighthouseStarRoomUpdateRisingSpark(ACTOR *actor)
{
	Sint32 growth;

	actor->y.fixed += toFIXED(1) / 2;
	growth = toFIXED(1) / 50;
	actor->scaleX += growth;
	actor->scaleY += growth;
	RISING_SPARK_TICKS_LEFT(actor)--;
	if(RISING_SPARK_TICKS_LEFT(actor) == 1)
		scFieldActorSetPosAndResetMotion(actor, 0, 0, 0);
	else if(RISING_SPARK_TICKS_LEFT(actor) == 0)
		fdActorRelease(actor);
}

/* The two strips of the hall's floor the sparks rise from, each 28 pixels wide
   and 120 deep: their west edges and their north edge. */
#define WEST_SPARKS_X 620
#define EAST_SPARKS_X 712
#define SPARKS_Z	  460

/* While the lighthouse thaws, sparks rise from the floor every other tick. */
void scMarsLighthouseStarRoomSpawnRisingSparks(void)
{
	Uint32	   frm = TickCount;
	ACTOR	  *actor;
	FIELD_SPR *spr;
	Uint32	   x, r;
	Sint32	   z;
	Sint32	   left;

	if(frm & 1)
		return;
	/* Each side makes its own spark, in turn: x is a random 28 pixels from the
	   strip's west edge, z a random 120 from SPARKS_Z. */
	if((frm >> 1) & 1) {
		r = syRandom();
		x = r << 3;
		x -= r;
		z = syRandom() * 15;
		x <<= 2;
		x >>= 16;
		left = toFIXED(WEST_SPARKS_X);
		z <<= 3;
		x <<= 16;
		x += left;
		z = FIXEDtoINT((Uint32)z);
		z = toFIXED(z);
		z += toFIXED(SPARKS_Z);
		actor = scMarsLighthouseStarRoomCreateActor(
			SPRITE_DUST_CLOUD, x, toFIXED(32), z);
	} else {
		r = syRandom();
		x = r << 3;
		x -= r;
		z = syRandom() * 15;
		x <<= 2;
		x >>= 16;
		left = toFIXED(EAST_SPARKS_X);
		z <<= 3;
		x <<= 16;
		x += left;
		z = FIXEDtoINT((Uint32)z);
		z = toFIXED(z);
		z += toFIXED(SPARKS_Z);
		actor = scMarsLighthouseStarRoomCreateActor(
			SPRITE_DUST_CLOUD, x, toFIXED(32), z);
	}
	if(actor != NULL) {
		actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		spr = actor->spr;
		spr->pri = 2;
		fdActorSetSprFlags(actor, 0);
		actor->scaleX = toFIXED(1) / 2;
		actor->scaleY = toFIXED(1) / 2;
		RISING_SPARK_TICKS_LEFT(actor) = RISING_SPARK_TICKS;
		actor->update = scMarsLighthouseStarRoomUpdateRisingSpark;
		fdActorSelectAnimation(actor, 5);
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	}
}

extern Sint32 MarsLighthouseStarRoomWestSparkleRoute[];
extern Sint32 MarsLighthouseStarRoomEastSparkleRoute[];

/* The west torch sends its two sparkles round the hall, west and east; each
   launches its fireball at the ice, the west one rolling east and the east one
   west. */
void scMarsLighthouseStarRoomShootFireballs(void)
{
	scActorAttach(ACTOR_WEST_TORCH, ACTOR_WEST_SPARKLE, ACTOR_WEST_FIREBALL,
		FACING_EAST, MarsLighthouseStarRoomWestSparkleRoute);
	scActorAttach(ACTOR_WEST_TORCH, ACTOR_EAST_SPARKLE, ACTOR_EAST_FIREBALL,
		FACING_WEST, MarsLighthouseStarRoomEastSparkleRoute);
}

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/flame_update_forward.h"

extern Sint32 ScriptedPartScript[];

#include "shared/actor_reset_scripted_for.h"

/* A flame thrower's flame sent forward from `pos` at `speed`; `flg` keeps
   its sprite when off screen. */
void scFlameThrowerSendForward(Sint32 *pos, Sint32 speed, Sint32 flg)
{
	ACTOR *actor =
		scFieldActorCreate(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);

	if(actor != NULL) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = speed;
		scActorResetScriptedFor(actor, 2);
		actor->update = scFlameUpdateForward;
		if(flg != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

Bool fdActorIsTouching(ACTOR *a, ACTOR *b);
Bool fdActorIsBeside(ACTOR *a, ACTOR *b);

#include "shared/flame_update_sideways.h"

#include "shared/actor_setup_scripted_part.h"

/* Defines a flame thrower's flame sent backward from `position` at `speed`
   under `updateRoutine`; `flag` keeps its sprite when off screen. */
#define DEFINE_RIDE_SPAWN_BACKWARD(name, updateRoutine) \
	void name(Sint32 *position, Sint32 speed, Sint32 flag) \
	{ \
		ACTOR *actor = scFieldActorCreate( \
			SPRITE_BLAZE_FLAME, position[0], position[1], position[2]); \
\
		if(actor != NULL) { \
			actor->bounce = 0; \
			actor->gravity = 0; \
			actor->sceneWord = -speed; \
			scActorSetupScriptedPart(actor, 3); \
			actor->update = updateRoutine; \
			if(flag != 0) \
				actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN; \
			fdActorSelectAnimation(actor, 2); \
		} \
	}

DEFINE_RIDE_SPAWN_BACKWARD(scFlameThrowerSendBackward, scFlameUpdateForward)

/* A flame thrower's flame that lands as it goes forward from `pos`. */
void scFlameThrowerSendEast(Sint32 *pos, Sint32 speed, Sint32 flg)
{
	ACTOR *actor =
		scFieldActorCreate(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);

	if(actor != NULL) {
		actor->bounce = 0;
		actor->gravity = 0;
		actor->sceneWord = speed;
		scActorSetupScriptedPart(actor, 3);
		actor->update = scFlameUpdateSideways;
		if(flg != 0)
			actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		fdActorSelectAnimation(actor, 2);
	}
}

DEFINE_RIDE_SPAWN_BACKWARD(scFlameThrowerSendWest, scFlameUpdateSideways)

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted_for.h"

void scFlameThrowerUpdateSpark();

/* A spark puffed from a flame thrower, drifting off in a random direction. */
void scFlameThrowerSendSpark(Sint32 *pos, Sint32 base, Sint32 mode)
{
	ACTOR *actor = scMarsLighthouseStarRoomCreateActor(
		SPRITE_DUST_CLOUD, pos[0], pos[1], pos[2] - toFIXED(2));
	Sint32 spin;

	if(actor != 0) {
		actor->bounce =
			syCos((syRandomBelow(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
		actor->gravity = (Uint16)syRandom();
		actor->sceneWord = (Uint16)(syRandom() << 1) + base;
		spin = (Uint16)syRandom() - DEGtoANG(180);
		actor->tmr = spin;
		scActorSetupScriptedFor(actor, 2);
		actor->update = scFlameThrowerUpdateSpark;
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	}
}

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

#include "shared/flame_throwers_release_flame.h"

void scFlameThrowersSetMode(Sint16 value)
{
	FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

Sint32 scFlameThrowersGetCounter(void)
{
	return FLAME_THROWERS_STATE->cnt;
}

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
void scMarsLighthouseStarRoomKnockBackIfHit(void)
{
	if(FLAME_THROWERS_STATE->active)
		fdLeaderKnockBack();
}

/* A rolling fireball hits Felix in the dragon hall once the Flame Dragons are
   beaten: he is hurt, sits down blinking, and is carried along with the
   fireball until it stops.

   The fireball is the hit state's `car`; Felix moves by its step each tick,
   stopping at the centre of a metatile where the floor ends. */
Sint32 scFireballCarryLeader(void)
{
	FIELD_STATE		*event = Ram_HeapSlots->fieldState;
	FLAME_HIT_STATE *ride;
	ACTOR			*leader;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32 unused[3];

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

#include "shared/fireball_carry_leader_if_hit.h"

/* Whether a flame or fireball has hit the leader (FLAME_HIT_STATE.active). */
Sint32 scMarsLighthouseStarRoomIsLeaderHit(void)
{
	return FLAME_THROWERS_STATE->active;
}

extern Uint32 CrumbsScript[];

#include "shared/actor_scatter_crumbs.c"

extern Uint32 IceChipsScript[];

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_script.c"

#include "shared/actor_roll_and_crumble.c"

#include "shared/actor_attach.c"

#include "shared/actor_launch_ahead.h"

Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern Sint16 SlideOnIceAnglesByKey[];

#include "shared/leader_slide_on_ice.h"

volatile Sint16 SlideOnIceStopped BSS_DATA;
METATILE_TARGET CrackedTiles	  BSS_DATA;

#include "shared/slide_on_ice_from_edge.c"

/* Stops the leader's slide on the ice where it is (nonzero), or lets it run. */
void scLeaderSetSlideStopped(Sint32 value)
{
	SlideOnIceStopped = value;
}

#include "shared/leader_slide_on_ice_ahead.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_RIDE_SPAWN_BACKWARD
#undef IWRAM
#undef STATE
#undef Position
extern const Sint32 *FieldEffectDurationScripts[];
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepth
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY
#include "shared/cracked_tile_drop_in.h"

void syGameFlagSetByte(Sint32 flg, Sint32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern Sint32 MarsLighthouseStarRoomTorchScript[];

#include "shared/debris_script.c"

/* clang-format off */
/* The west sparkle's way from the west torch round the west side of the hall */
/* and up to the ice, where it leaves its fireball to roll in. */
Sint32 MarsLighthouseStarRoomWestSparkleRoute[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(616, 0, 264),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(616, 0, 248),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(568, 0, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(568, 32, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(568, 32, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(576, 32, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(576, 22, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(592, 22, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseStarRoomWestFireballCracksIce),
	SC_CALL(scActorPark),
	SC_END,
};

/* The east sparkle's longer way round the east side of the hall to the ice. */
Sint32 MarsLighthouseStarRoomEastSparkleRoute[] = {
	SC_CALL(scActorPopUp),
	SC_MOVE(680, 0, 264),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(680, 0, 296),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(728, 0, 296),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(728, 0, 264),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(808, 0, 264),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(808, 0, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(808, 24, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(792, 24, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(792, 32, 240),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(792, 32, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(784, 32, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(784, 22, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_MOVE(768, 22, 216),
	SC_SYNC,
	SC_WAIT(2),
	SC_SET(SW_WATCHED_ACTOR, (Sint32)scMarsLighthouseStarRoomEastFireballCracksIce),
	SC_CALL(scActorPark),
	SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/field_effect_duration_scripts.c"

Sint32 MarsLighthouseStarRoomTorchScript[] = {
	SC_CALL(scMarsLighthouseStarRoomSetUpTorch),
	SC_END,
};

/* A light rises from a shrinking dragon, growing, for 40 ticks. */
Sint32 MarsLighthouseStarRoomLightRiseScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
	SC_ADD(SW_Y, -toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(40, SC_TOP),
	SC_END,
};

/* The light sinks to the floor, shrinking, and is put away. */
Sint32 MarsLighthouseStarRoomLightSinkScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_ADD(SW_Y, toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(24, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

/* The dragons shrink to nothing over 40 ticks and are put away; Karst's, */
/* which faces the other way, mirrored. */
Sint32 MarsLighthouseStarRoomAgatioDragonShrinkScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(40, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 MarsLighthouseStarRoomKarstDragonShrinkScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_LOOP(40, SC_TOP),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};
/* clang-format on */

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(MarsLighthouseStarRoom),
	SCENE_EXIT(EXIT_TO_CORRIDOR_WEST_DOORWAY, MarsLighthouseStarRoom,
		ENTRANCE_CORRIDOR_WEST_DOORWAY),
	SCENE_EXIT(EXIT_TO_HALL_SOUTH_END, MarsLighthouseStarRoom,
		ENTRANCE_HALL_SOUTH_END),
	SCENE_EXIT(EXIT_TO_SMALL_ROOM_SOUTH_END, MarsLighthouseStarRoom,
		ENTRANCE_SMALL_ROOM_SOUTH_END),
	SCENE_EXIT(EXIT_TO_CORRIDOR_EAST_DOORWAY, MarsLighthouseStarRoom,
		ENTRANCE_CORRIDOR_EAST_DOORWAY),
	SCENE_EXIT(
		MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_SOUTHEAST,
		MarsLighthouseEntranceSolBladeChest,
		MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
	SCENE_EXIT(
		MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_SOUTHEAST,
		MarsLighthouseEntranceValkyrieMailChest,
		MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_SOUTHEAST),
	SCENE_EXIT(
		MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_NORTHWEST_A,
		MarsLighthouseEntranceSolBladeChest,
		MARS_LIGHTHOUSE_ENTRANCE_SOL_BLADE_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST_A),
	SCENE_EXIT(
		MARS_LIGHTHOUSE_STAR_ROOM_TO_MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_NORTHWEST,
		MarsLighthouseEntranceValkyrieMailChest,
		MARS_LIGHTHOUSE_ENTRANCE_VALKYRIE_MAIL_CHEST_FROM_MARS_LIGHTHOUSE_STAR_ROOM_NORTHWEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS,
		(Sint32)MarsLighthouseStarRoomTorchScript, 712, 0, 264, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS,
		(Sint32)MarsLighthouseStarRoomTorchScript, 648, 0, 264, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICE_CRACK, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 680, 0, 240, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED,
		MOVEMENT_TYPE_CLEAR_COLLISION, 664, 0, 216, FACING_SOUTH,
		TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLAME_DRAGON, FLAG_MARS_LIGHTHOUSE_THAWED,
		MOVEMENT_TYPE_CLEAR_COLLISION, 696, 0, 216, FACING_SOUTH,
		TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_AGATIO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KARST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
		0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		1),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP,
		0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, EXIT_TO_CORRIDOR_WEST_DOORWAY},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 2, CONDITION_ALWAYS,
		EXIT_TO_HALL_SOUTH_END},
	{CUE_EXIT | EXIT_STYLE_HIDDEN_LAYER, 3, CONDITION_ALWAYS,
		EXIT_TO_SMALL_ROOM_SOUTH_END},
	{CUE_EXIT, 4, CONDITION_ALWAYS, EXIT_TO_CORRIDOR_EAST_DOORWAY},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_STEP, STEP_CUE_FIREBALL_HIT, IF_FLAG_SET(FLAG_FLAME_DRAGONS_BEATEN),
		SCRIPT_REF(scFireballCarryLeader)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_EAST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomStartBlazeOnEastTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS),
		ACTOR_WEST_TORCH, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomBlazeWestTorch)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END),
		ACTOR_WEST_TORCH, CONDITION_ALWAYS,
		(Uint32)(scMarsLighthouseStarRoomEndBlazeOnWestTorch)},
	{CUE_RAISED, DRAGON_WAKE_CUE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomWakeFlameDragons)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
		METATILE_DRAGON_ICE, IF_FLAG_SET(FLAG_BURST_CAN_BREAK_DRAGON_ICE),
		SCRIPT_REF(scMarsLighthouseStarRoomBurstIce)},
	{CUE_TALK, ACTOR_KARST, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomHoldWarmHand)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KARST, CONDITION_ALWAYS,
		MSGNO(MsgMarsLighthouseStarRoomImSoColdColderThan)},
	{CUE_TALK, ACTOR_AGATIO, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomTakeMarsStar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AGATIO, CONDITION_ALWAYS,
		MSGNO(MsgMarsLighthouseStarRoomImSoTiredFog)},
	{CUE_EXAMINE, METATILE_STATUE_FACE, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomExamineStatue)},
	{USE_ITEM(ITEM_MYTHRIL_BAG_MARS_STAR),
		METATILE_STATUE_FACE | CUE_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
		SCRIPT_REF(scMarsLighthouseStarRoomUseMarsStar)},
	{CUE_TABLE_END},
};

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};
