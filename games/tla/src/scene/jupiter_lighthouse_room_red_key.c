/* The hall of Jupiter Lighthouse where the lighthouse is woken, between the
   room of locked doors below and the cracked floor's room and Room M above.
   Below the platform in its center a voice asks the holder of the star to show
   the power of Anemos (scJupiterLighthouseHearTheVoice), and Hover cast at the
   foot of the ladder up to it activates the lighthouse
   (scJupiterLighthouseActivateTheLighthouse): its grey blocks rise to hover in
   every room and a beam of light stands in the hall. Later, on the way to the
   fight by the aerie, Alex stops the party here and heals it
   (scJupiterLighthouseAlexHealsTheParty). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "system/field_effect.h"
#include "field/script_inline.h"
#include "macros.h"
#include "scene/scene.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "scene/cycle_pictures.h"

/* Set while the leader stands in the center of the hall, so the voice speaks
   once each time he steps in. */
#define FLAG_VOICE_HEARD					  0xa0e
/* The grey block Moved onto the empty socket has risen to hover over it. */
#define FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED 0xa09
/* Set as Alex stops the party here; he stays in the hall until Isaac and Ivan
   have fallen. */
#define FLAG_ALEX_WAITING					  0xa22

/* The hall's actors, numbering the rows of ActorPlacements. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_RED_KEY_ACTOR_ID {
	/* The three grey blocks over the spikes at the west end, which hover once
	   the lighthouse is activated; the middle one is Moved onto the empty
	   socket between the others to rise with them. */
	ACTOR_WEST_HOVERING_BLOCK = ACTOR_FIRST_PLACED + 2,
	ACTOR_SOCKET_BLOCK,
	ACTOR_EAST_HOVERING_BLOCK,
	/* The four grey blocks the activation raises before the party's eyes. */
	ACTOR_FIRST_RISING_BLOCK,
	ACTOR_LAST_RISING_BLOCK = ACTOR_FIRST_RISING_BLOCK + 3,
	/* The three yellow blocks pushed or Moved about the floor. */
	ACTOR_FIRST_PUSHED_BLOCK,
	ACTOR_LAST_PUSHED_BLOCK = ACTOR_FIRST_PUSHED_BLOCK + 2,
	ACTOR_ALEX = ACTOR_LAST_PUSHED_BLOCK + 3,
	/* The two halves of the beam of light the activation raises. */
	ACTOR_LOWER_BEAM,
	ACTOR_UPPER_BEAM
} JUPITER_LIGHTHOUSE_ROOM_RED_KEY_ACTOR_ID;

/* The empty socket between the west grey blocks. */
#define EMPTY_SOCKET_METATILE_X 13
#define EMPTY_SOCKET_METATILE_Z 34
/* The metatile, just north of where the voice speaks, where Hover wakes the
   lighthouse. */
#define ACTIVATION_METATILE_X	42
#define ACTIVATION_METATILE_Z	41
/* The times the ring of sparks pulses, each pause shorter than the last. */
#define RING_PULSES				20
/* The sparks of the activation's ring, from the leader to the beam. */
#define ACTIVATION_SPARKS		96
#include "text/msg_ids.h"
#include "scene/lighthouse_palettes.h"

struct ACTOR;
struct METATILE_TARGET;
struct METATILE_TARGET_ENTRY;

extern Uint32 OrbRisingScript[];

void scJupiterLighthouseStartSparkRing(const Sint32 *from, const Sint32 *to);

void scJupiterLighthouseUpdateSparkRing(void);

/* Fills an effect sprite: its OAM attributes, its tiles and its size from its
   shape, pivoted at (pivotX, pivotY) (fdEffectSprInit, through the overlay's
   own import row). */
void scJupiterLighthouseInitSparkSpr(EFFECT_SPR *spr, Sint32 pivotX,
	Sint32 pivotY, Uint32 attributes, Sint32 tile);

/* The state of the code every Jupiter Lighthouse room's overlay keeps its own
   copy of: a whirlwind's ride (shared/whirlwind_*.h), the palette cycle
   (shared/palette_cycle_step.h), the lit actors' sprites
   (shared/light_sprites_*.h) and the cracked tiles (shared/cracked_tile_*.h).
   This hall has neither whirlwind nor cracked tile, but its copies of that code
   keep their state here all the same. */
Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16						 PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16						 PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile		 BSS_DATA;
Sint16 LightSpriteCount		 BSS_DATA;
ACTOR						*LightSpriteActors[16] BSS_DATA;
LIGHT_OAM					 LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge	 BSS_DATA;
Sint16 LightSpritePriority	 BSS_DATA;
METATILE_TARGET CrackedTiles BSS_DATA;

void scLeaderCenterInMetatile(void);

void scCrackedTileCrack(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry);

void scJupiterLighthouseHearTheVoice(void);

void scJupiterLighthouseLeaveTheCenter(void);

void scJupiterLighthouseAlexHealsTheParty(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Uint32 FieldEffectLongScript[];

struct FIELD_EFFECT;

/* Queues an effect sprite for this tick at its depth, turned and scaled about
   its pivot (fdEffectSprDraw, through the overlay's own import row). */
void scJupiterLighthouseDrawSparkSpr(EFFECT_SPR *spr);

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

extern Sint16 JupiterLighthousePushedBlocks[];

/* As Move lifts one of the hall's blocks: lifts it off its metatiles. */
void scJupiterLighthouseLiftPushedBlock(void)
{
	fdActorLiftTargetBlock(JupiterLighthousePushedBlocks);
}

void scActorRaiseFromMetatile(Sint32 id);

/* As a block is set down: the middle grey block, once the lighthouse is
   activated and it sits on the empty socket, rises 32 pixels and hovers for
   good; any other block is set on its metatiles. */
void scJupiterLighthousePlacePushedBlock(Sint32 unused, Sint32 id)
{
	ACTOR *actor;
	Sint32 frm;

	actor = fdActorGet(id);
	if(id == ACTOR_SOCKET_BLOCK) {
		if(!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) ||
			FIELD_METATILE(actor->x.fixed) != EMPTY_SOCKET_METATILE_X ||
			FIELD_METATILE(actor->z.fixed) != EMPTY_SOCKET_METATILE_Z)
			return;
		actor->motionFlg = 0;
		for(frm = 0; frm < 16; frm++) {
			actor->y.fixed += toFIXED(2);
			syWait(1);
		}
		scActorRaiseFromMetatile(id);
		fdScriptSetFlag(FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED);
	} else {
		fdActorPlaceMovedBlock();
	}
}

#include "shared/actor_cycle_pictures.h"

/* An actor's update while the lighthouse wakes: every other tick its sprite
   steps through six colour modes. */
void scJupiterLighthouseFlashCols(ACTOR *actor)
{
	Uint32 frmCnt = TickCount;

	if(frmCnt & 1)
		fdActorSetSprColMode(actor, (frmCnt >> 1) % 6);
}

/* A rising particle's update: it circles the beam of light. */
void scOrbUpdate(ACTOR *self)
{
	ACTOR *centre = fdActorGet(ACTOR_LOWER_BEAM);
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
	self->tmr += -TRIG_TURN / 32;
}

/* Starts a particle circling the beam of light at a random height, every third
   tick; every tick while FLAG_SCENE_TEMP_01 is set, which nothing in the hall
   sets. */
void scOrbSpawnRising(void)
{
	ACTOR	  *center;
	ACTOR	  *effect;
	FIELD_SPR *spr;
	Sint32	   ang;

	if(fdScriptGetFlag(FLAG_SCENE_TEMP_01) == 0 && TickCount % 3 != 0)
		return;
	center = fdActorGet(ACTOR_LOWER_BEAM);
	if(fdScriptGetFlag(FLAG_SCENE_TEMP_01) != 0) {
		Uint32 random;

		random = syRandom();
		effect = scFieldActorCreate(SPRITE_PARTICLE, center->x.fixed,
			(Sint32)toFIXED(FIXEDtoINT((Uint32)(random << 8))) +
				center->y.fixed - toFIXED(28),
			center->z.fixed);
	} else {
		Uint32 random;

		random = syRandom();
		effect = scFieldActorCreate(SPRITE_PARTICLE, center->x.fixed,
			(Sint32)toFIXED(FIXEDtoINT((Uint32)(random << 6))) +
				center->y.fixed - toFIXED(28),
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
		effect->update = (OBJECT_UPDATE)scOrbUpdate;
		effect->speedLimit =
			syMathSin((Uint32)(syRandom() * 65535) >> 20) * 24 >> 16;
		spr->flg = 0;
		spr->pri = 1;
	}
}

extern Sint32 JupiterLighthouseSparksPerTick;
extern Sint32 JupiterLighthouseSparkSoundOn;

void scJupiterLighthouseAddSparks(Sint32 count);

/* A tick event while the lighthouse wakes: adds this tick's sparks to the
   ring, with the rising sound until the tower starts to shake. */
void scJupiterLighthouseFeedSparkRing(void)
{
	if(JupiterLighthouseSparkSoundOn != 0)
		fdScriptPlaySnd(SE_ASCEND);
	scJupiterLighthouseAddSparks(JupiterLighthouseSparksPerTick);
}

extern Uint32 JupiterLighthouseBringInScript[];
extern Sint32 JupiterLighthouseNarrowBeamScript[];

void scJupiterLighthouseEndSparkRing(void);

/* Shows one half of the beam of light, drawn underfoot at priority 2 in colour
   7, flat and upside down at first, and starts the script that brings it in. */
static __inline__ void scJupiterLighthouseBringInBeam(Sint32 id)
{
	ACTOR *actor;

	fdActorSetSprPriority(id, 2);
	fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
	actor = fdActorGetById(id);
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->scaleY = -toFIXED(1);
	actor->scaleX = toFIXED(1) / 10;
	fdActorSetMovementTypeById(id, JupiterLighthouseBringInScript);
}

/* Hover cast at the ladder's foot wakes the lighthouse: the leader faces north
   and flashes in shifting colours, a ring of sparks pulses faster and faster
   toward the hall's far end as the tower shakes, the beam of light rises there
   with particles circling it, the hall's floor changes and its lights cycle,
   and the four grey blocks over the spikes beside the ladder rise to hover. */
void scJupiterLighthouseActivateTheLighthouse(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *a = fdActorGetById(ACTOR_FIRST_RISING_BLOCK);
	ACTOR *b = fdActorGetById(ACTOR_FIRST_RISING_BLOCK + 1);
	ACTOR *c = fdActorGetById(ACTOR_FIRST_RISING_BLOCK + 2);
	ACTOR *d = fdActorGetById(ACTOR_LAST_RISING_BLOCK);
	FIXED  from[3];
	FIXED  to[3];
	Sint32 i;

	if(FIELD_METATILE(leader->x.fixed) != ACTIVATION_METATILE_X ||
		FIELD_METATILE(leader->z.fixed) != ACTIVATION_METATILE_Z)
		return;
	fdScriptSetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED);
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	syFieldScriptWait(30);
	leader->update = scJupiterLighthouseFlashCols;
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_SHIMMER);
	fdActorSetAnimationSpeed(leader, ANIM_STEP_NORMAL * 4);
	syFieldScriptWait(50);
	fdScriptPlaySnd(SE_GATHERING_POWER);
	leader->tmr = 0;
	leader->update = scActorCyclePictures;
	syFieldScriptWait(30);
	from[0] = leader->x.fixed;
	from[1] = leader->y.fixed + toFIXED(20);
	from[2] = leader->z.fixed;
	to[0] = toFIXED(680);
	to[1] = 0;
	to[2] = toFIXED(574);
	scJupiterLighthouseStartSparkRing(from, to);
	JupiterLighthouseSparksPerTick = 0;
	JupiterLighthouseSparkSoundOn = 1;
	sySetTickEvent(scJupiterLighthouseFeedSparkRing, EVENT_PASS_TICK);
	for(i = 0; i < RING_PULSES; i++) {
		JupiterLighthouseSparksPerTick = 0;
		syWait((RING_PULSES - i) / 2);
		JupiterLighthouseSparksPerTick = 1;
		syWait(1);
	}
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(1, 1, 1);
	JupiterLighthouseSparksPerTick = 1;
	syFieldScriptWait(90);
	fdMapGridSetShake(3, 3, 1);
	JupiterLighthouseSparksPerTick = 3;
	JupiterLighthouseSparkSoundOn = 0;
	syFieldScriptWait(40);
	scJupiterLighthouseBringInBeam(ACTOR_LOWER_BEAM);
	scJupiterLighthouseBringInBeam(ACTOR_UPPER_BEAM);
	sySetTickEvent(scOrbSpawnRising, EVENT_PASS_TICK);
	fdScriptPlaySnd(SE_CLATTER);
	fdMapGridSetShake(6, 6, 1);
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), 0);
	fdPaletteFadeInterpolate(60);
	syWait(60);
	fdScriptPlaySnd(SE_CLATTER);
	syWait(30);
	fdScriptPlaySnd(SE_CLATTER);
	syWait(30);
	fdScriptPlaySnd(SE_CLATTER);
	syWait(30);
	fdMapGridCopyMetatilesLayered(58, 99, 3, 3, 41, 99);
	fdMapGridCopyMetatilesLayered(123, 32, 1, 5, 106, 32);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_KEEP_COPIES);
	fdScriptPlaySnd(SE_CLATTER);
	syWait(30);
	leader->tmr = 3;
	syWait(1);
	leader->update = 0;
	fdActorSetAnimationSpeed(leader, ANIM_STEP_NORMAL);
	fdActorSetSprFlags(
		fdActorGetById(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
	fdActorSetScript(ACTOR_LOWER_BEAM, JupiterLighthouseNarrowBeamScript);
	fdActorSetScript(ACTOR_UPPER_BEAM, JupiterLighthouseNarrowBeamScript);
	JupiterLighthouseSparksPerTick = 2;
	syFieldScriptWait(30);
	JupiterLighthouseSparksPerTick = 1;
	syFieldScriptWait(30);
	JupiterLighthouseSparksPerTick = 0;
	scFieldActorWaitForScriptEndById(ACTOR_LOWER_BEAM);
	scFieldActorWaitForScriptEndById(ACTOR_UPPER_BEAM);
	fdActorGetById(ACTOR_LOWER_BEAM)->scaleX = 0;
	fdActorGetById(ACTOR_UPPER_BEAM)->scaleX = 0;
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeInterpolate(30);
	fdMapGridSetShake(2, 2, 1);
	syFieldScriptWait(30);
	fdMapGridSetShake(0, 0, 1);
	syFieldCloseEvent(scJupiterLighthouseFeedSparkRing);
	scJupiterLighthouseEndSparkRing();
	syFieldScriptWait(30);
	a->motionFlg = 0;
	b->motionFlg = 0;
	c->motionFlg = 0;
	d->motionFlg = 0;
	for(i = 0; i < 16; i++) {
		a->y.fixed += toFIXED(2);
		b->y.fixed += toFIXED(2);
		c->y.fixed += toFIXED(2);
		d->y.fixed += toFIXED(2);
		syWait(1);
	}
	scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK);
	scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK + 1);
	scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK + 2);
	scActorRaiseFromMetatile(ACTOR_LAST_RISING_BLOCK);
	fdScriptPlaySnd(MUS_PUZZLE_SOLVED);
	mnSndWaitForJingle();
}

extern Sint32 PaletteCyclePausesInScripts;

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);

/* Stepping into the hall's centre: the screen warms and a voice asks for the
   power of Anemos, once until the party leaves the centre. */
void scJupiterLighthouseHearTheVoice(void)
{
	if(!fdScriptGetFlag(FLAG_VOICE_HEARD)) {
		fdScriptSetFlag(FLAG_VOICE_HEARD);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, 0);
		fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, 1);
		fdPaletteFadeInterpolate(30);
		fdScriptWaitTransitionTicks();
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_STAND);
		syFieldScriptWait(30);
		/* "Holder of the star... Show the power of Anemos!" */
		mnFieldMessageShowCentered((Sint32)MsgFieldShowAnemosPower, 0, -2);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, 0);
		fdPaletteFadeInterpolate(30);
		fdScriptEnd();
	}
}

/* Stepping out of the centre: the voice speaks again next time. */
void scJupiterLighthouseLeaveTheCenter(void)
{
	syGameFlagClear(FLAG_VOICE_HEARD);
}

/* Alex appears out of a shower of rising sparks. */
void scJupiterLighthouseAlexAppears(void)
{
	EFFECT_OPTIONS params;
	ACTOR		  *actor;
	Uint32		   frm;
	FIXED		   x;
	Sint32		   y;
	Uint32		   random;
	Uint32		   randomY;

	actor = fdActorGet(ACTOR_ALEX);
	fdScriptPlaySnd(SE_WARP);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
	params.pri = 1;
	params.pal = SPRITE_COLOR_BRG;
	params.type = SPRITE_PARTICLE;
	params.startScaleX = toFIXED(2) / 5;
	params.startScaleY = toFIXED(3);
	for(frm = 0; frm < 32; frm++) {
		syFieldScriptWait(1);
		if(!(frm & 1)) {
			/* A whole pixel from 12 west of Alex to 11 east, and up to 31 over
			   the 32 above his feet. */
			random = syRandom();
			x = actor->x.fixed + toFIXED(FIXEDtoINT(random * 24)) - toFIXED(12);
			randomY = syRandom();
			y = actor->y.fixed;
			scFieldEffectSpawn(x,
				y + toFIXED(FIXEDtoINT(randomY << 5)) + toFIXED(32),
				actor->z.fixed, 0, -toFIXED(4), 0,
				EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
					EFFECT_USE_PALETTE,
				&params);
		}
		if(frm == 20)
			fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
	}
	fdActorSetSprColById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

/* The healer raises arcs of light over the target, which spins and flashes for
   two seconds to the revival jingle and then stands facing north. */
void scJupiterLighthouseHealTarget(Sint32 actorId, Sint32 tgt)
{
	Sint32 frm;

	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(
		fdActorGet(actorId), CASTER_LOOK_CYCLING_COLORS);
	fdScriptPlaySnd(SE_ASCEND);
	syWait(50);
	fdScriptPlaySnd(MUS_RECOVERING_AFTER_COMBAT);
	for(frm = 0; frm < 120; frm++) {
		{
			ACTOR *actor = fdActorGet(tgt);
			Uint32 facing = actor->facing;

			actor->facing = facing + FACING_STEP;
		}
		scFieldActorSetSprCol(fdActorGet(tgt),
			(frm & 4) ? SPRITE_COLOR_WHITE : SPRITE_COLOR_NORMAL);
		syWait(1);
	}
	fdActorGet(tgt)->facing = FACING_NORTH;
	scFieldActorSetSprCol(fdActorGet(tgt), SPRITE_COLOR_NORMAL);
	fdBattleEffectSetObjectUpdateMode(fdActorGet(actorId), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	fdActorSetAnimation(tgt, ANIM_STAND);
}

/* On the way to the fight by the aerie, Alex's voice stops Felix, who looks
   about; Alex appears before him, asks whether he will light the beacon, tells
   him to save his friends and heals the party. From then on there are no
   encounters and Retreat is blocked. */
void scJupiterLighthouseAlexHealsTheParty(void)
{
	Sint32 ang;

	if(!fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED))
		return;
	if(fdScriptGetFlag(FLAG_ISAAC_AND_IVAN_FELL))
		return;
	if(fdScriptGetFlag(FLAG_ALEX_WAITING))
		return;
	fdScriptSetFlag(FLAG_ALEX_WAITING);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId(MsgFieldWaitingForCompanion);
	/* "Ah, Felix... I've been waiting for you." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	fdActorWalkToAndWait(ACTOR_FELIX, 68, 280);
	fdActorWalkToAndWait(ACTOR_FELIX, 68, 272);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	syFieldScriptWait(10);
	fdActorSetSprFlags(((void *)fdActorGet(ACTOR_ALEX)), 0);
	scFieldActorSetSprCol(
		((void *)fdActorGet(ACTOR_ALEX)), SPRITE_COLOR_HIDDEN);
	ang = FACING_SOUTH;
	fdActorSetPosAndFacing(ACTOR_ALEX, toFIXED(64), toFIXED(225), ang);
	scJupiterLighthouseAlexAppears();
	syFieldScriptWait(40);
	fdCameraMoveTo(68, CAMERA_KEEP, 240, 1);
	/* "Have you come here to light the beacon?" Yes: "And you're sure that's
	   all you're going to..." No: "Hmph. They consider you their enemy,
	   and..." */
	fdScriptOpenMessage(ACTOR_ALEX, 0);
	if(syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
		/* "And you're sure that's all you're going to do?" */
		syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
		FieldStateWork->msg++;
	} else {
		fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
		syFieldScriptWait(10);
		FieldStateWork->msg++;
		/* "Hmph. They consider you their enemy, and yet you pity them." */
		syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	}
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(10);
	/* "I saw you watching when Isaac's friends fell into Karst's trap." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "I know you, Felix. I know that if you leave Isaac behind, you'll..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "You're not like me. You can't simply discard someone who is no..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	/* "Well, you'd better hurry if you still hope to save them." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(30);
	fdCameraMoveTo(68, CAMERA_KEEP, 256, 1);
	fdActorWalkByAndWait(ACTOR_ALEX, 0, 16);
	syFieldScriptWait(30);
	scJupiterLighthouseHealTarget(ACTOR_ALEX, ACTOR_FELIX);
	gmPartyRestoreHpPp(1);
	/* "Everyone recovered fully." */
	mnMessageShowCentered((Sint16)FieldStateWork->msg++, 5);
	syFieldScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldScriptWait(30);
	/* "There you go! Consider it a gift. You can still make it. Go on..." */
	syFieldScriptShowMessageThenWait(ACTOR_ALEX, 0, 5);
	fdScriptSetFlag(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY);
	fdScriptSetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
	fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Sets up the hall: Alex while he waits for the party, the beam of light hidden
   until the lighthouse is activated, then shown with its particles. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 ang;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(fdScriptGetFlag(FLAG_ISAAC_AND_IVAN_FELL))
		fdScriptClearFlag(FLAG_ALEX_WAITING);
	if(fdScriptGetFlag(FLAG_ALEX_WAITING)) {
		ang = FACING_SOUTH;
		fdActorSetPosAndFacing(ACTOR_ALEX, toFIXED(64), toFIXED(225), ang);
	}
	fdActorPlaceBlocks(JupiterLighthousePushedBlocks);
	fdActorSetMovementType(
		fdActorGet(FIELD_TREASURE_ACTOR), MOVEMENT_TYPE_TAKE_LEADER_LAYER);
	actor = fdActorGet(ACTOR_LOWER_BEAM);
	actor->scaleX = 0;
	actor->motionFlg = 0;
	SET_ACTOR_POSITION(actor, toFIXED(680), toFIXED(32), toFIXED(580));
	fdActorSetSprFlags(fdActorGet(ACTOR_LOWER_BEAM), 0);
	actor = fdActorGet(ACTOR_UPPER_BEAM);
	actor->scaleX = 0;
	actor->motionFlg = 0;
	SET_ACTOR_POSITION(actor, toFIXED(680), toFIXED(64), toFIXED(580));
	fdActorSetSprFlags(fdActorGet(ACTOR_UPPER_BEAM), 0);
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
		/* The floor as the activation left it. */
		fdMapGridCopyMetatilesLayered(58, 99, 3, 3, 41, 99);
		fdMapGridCopyMetatilesLayered(123, 32, 1, 5, 106, 32);
		sySetTickEvent(scOrbSpawnRising, EVENT_PASS_TICK);
	}
	return 0;
}

/* Raises the grey blocks once the lighthouse is activated, the Moved one once
   it is in its gap. */
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED))
		fdMapGridSetMetatileBehavior(toFIXED(208), toFIXED(672),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_HOVER);
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
		scActorRaiseFromMetatile(ACTOR_WEST_HOVERING_BLOCK);
		scActorRaiseFromMetatile(ACTOR_EAST_HOVERING_BLOCK);
		scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK);
		scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK + 1);
		scActorRaiseFromMetatile(ACTOR_FIRST_RISING_BLOCK + 2);
		scActorRaiseFromMetatile(ACTOR_LAST_RISING_BLOCK);
	}
	if(fdScriptGetFlag(FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED)) {
		fdActorSetPos(ACTOR_SOCKET_BLOCK, 216, 552);
		scActorRaiseFromMetatile(ACTOR_SOCKET_BLOCK);
	}
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleTick;

/* Restarts the palette cycle at the given tick. */
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

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
#include "shared/whirlwind_update.h"

#include "shared/whirlwind_spawn.h"

/* Unused: nothing in the hall sets scActorEmitSmoke as an update. */
#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/* One spark of the ring: its sideways and upward swing, in pixels, and its age
   along the way from one end to the other; it is drawn only while its age runs
   from 0 to its duration. */
typedef struct SPARK {
	Uint8  unused[12]; /* never read or written */
	Sint32 dx;
	Sint32 dy;
	Sint32 duration;
	Sint32 age;
} SPARK;

/* The spark ring in its heap slot: the orbs' pictures, the sparks and their
   sprites, the ring's two ends and its VRAM entry. */
typedef struct SPARK_RING_STATE {
	Uint8	   tiles[ORB_PICTURES * TILE_SIZE_8BPP];
	SPARK	   sparks[ACTIVATION_SPARKS];
	EFFECT_SPR sprs[ACTIVATION_SPARKS];
	Sint32	   from[3];
	Sint32	   to[3];
	Sint16	   next;
	Uint16	   tile;
	Sint16	   vramEntry;
} SPARK_RING_STATE;

/* A tick event: moves each live spark along the way between the ring's ends,
   swinging out and back on a half sine, and draws it. */
void scJupiterLighthouseUpdateSparkRing(void)
{
	SPARK_RING_STATE *state = RamHeapSlots->menuState;
	SPARK			 *spark = state->sparks;
	EFFECT_SPR		 *spr = state->sprs;
	Sint32			  pos[3];
	Sint32			  duration;
	Sint32			  i;

	for(i = 0; i < ACTIVATION_SPARKS; spr++, i++, spark++) {
		duration = spark->duration;
		if(spark->age < 0 || spark->age > duration)
			continue;
		{
			FIXED sine =
				syMathSin((Sint32)((spark->age * (TRIG_TURN / 2)) / duration));
			pos[0] = state->from[0] +
				(state->to[0] - state->from[0]) * spark->age / duration +
				sine * spark->dx;
			sine =
				syMathSin((Sint32)((spark->age * (TRIG_TURN / 2)) / duration));
			pos[1] = state->from[1] +
				(state->to[1] - state->from[1]) * spark->age / duration +
				sine * spark->dy;
			pos[2] = state->from[2] +
				(state->to[2] - state->from[2]) * spark->age / duration;
			fdCameraWorldToScreen(pos);
			spr->x = pos[0];
			spr->y = pos[2];
			scJupiterLighthouseDrawSparkSpr(spr);
			spark->age++;
		}
	}
}

/* Sends count new sparks along the ring, each swinging up to 64 pixels out to a
   random side (never straight down) and taking 16 to 31 ticks to arrive. */
void scJupiterLighthouseAddSparks(Sint32 cnt)
{
	SPARK_RING_STATE *state = RamHeapSlots->menuState;
	SPARK			 *spark;
	Sint32			  ang;
	Sint32			  i;
	FIXED			  radius;
	Sint32			  duration;

	if(cnt > 0) {
		i = cnt;
		do {
			spark = &state->sparks[state->next];
			ang = ((syRandom() * 3) >> 2) + DEGtoANG(135);
			radius = toFIXED(64);
			duration = syRandomBelow(16) + 16;
			spark->dx = FIXEDtoINT(syMulFX(syCos(ang), radius));
			spark->dy = FIXEDtoINT(-syMulFX(syMathSin(ang), radius));
			spark->duration = duration;
			spark->age = 0;
			state->next++;
			state->next %= ACTIVATION_SPARKS;
		} while(--i != 0);
	}
}

/* Starts the spark ring between two points: loads the coloured orbs' pictures,
   readies the sparks' sprites at the leader's priority and just in front of
   him, and starts the update event. */
void scJupiterLighthouseStartSparkRing(const Sint32 *from, const Sint32 *to)
{
	SPARK_RING_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(SPARK_RING_STATE));
	ACTOR	   *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	EFFECT_SPR *spr;
	SPARK	   *spark;
	Sint32		entry;
	Sint32		tile;
	Sint32		i;

	state->from[0] = from[0];
	state->from[1] = from[1];
	state->from[2] = from[2];
	state->to[0] = to[0];
	state->to[1] = to[1];
	state->to[2] = to[2];
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_COLOURED_ORBS), state);
	entry = syVRAMCacheFindFreeEntry();
	tile = mnVRAMLoad(entry, sizeof(state->tiles), state);
	state->tile = tile;
	state->vramEntry = entry;
	for(i = 0, spark = state->sparks, spr = state->sprs; i < ACTIVATION_SPARKS;
		i++, spr++, spark++) {
		scJupiterLighthouseInitSparkSpr(
			spr, 4, 4, 0, tile + (i & (ORB_PICTURES - 1)) * 2);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = fdActorGetSprPriority(leader);
		spr->depth = fdActorGetSprDepth(leader) - 1;
		spark->age = -1;
	}
	{
		Sint32 firstSlot = 0;

		state->next = firstSlot;
	}
	sySetTickEvent(scJupiterLighthouseUpdateSparkRing, EVENT_PASS_TICK);
}

/* Stops the spark ring and frees its pictures and state. */
void scJupiterLighthouseEndSparkRing(void)
{
	SPARK_RING_STATE *state = RamHeapSlots->menuState;

	syFieldCloseEvent(scJupiterLighthouseUpdateSparkRing);
	syVRAMCacheFreeEntry(state->vramEntry);
	syHeapSlotRelease(HEAP_SLOT_MENU);
}

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Unused: sets the priority the lit actors' sprites are drawn at. */
UNUSED void scLightSprsSetPriority(Sint32 value)
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

void scJupiterLighthouseLiftPushedBlock(void);
void scJupiterLighthouseActivateTheLighthouse(void);

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* The beam of light widens over 32 ticks. */
Uint32 JupiterLighthouseBringInScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_LOOP(32, SC_TOP),
	SC_END,
};

/* The beam of light narrows away over 140 ticks. */
Sint32 JupiterLighthouseNarrowBeamScript[] = {                                                               
	  SC_ADD(SW_SCALE_X, -960),                                                 
	  SC_WAIT(1),                                                              
	  SC_LOOP(140, SC_TOP),                                                     
	  SC_END,                                                                  
  };

/* The three blocks Move pushes, ended by -1. */
Sint16 JupiterLighthousePushedBlocks[] = {
	ACTOR_FIRST_PUSHED_BLOCK, ACTOR_FIRST_PUSHED_BLOCK + 1, ACTOR_LAST_PUSHED_BLOCK, -1,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {                                                        
	  SCENE_EXITS(JupiterLighthouseRoomRedKey),         SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY),    SCENE_EXIT(2, JupiterLighthouseRoomRedKey, 3), 
	  SCENE_EXIT(3, JupiterLighthouseRoomRedKey, 2),  SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES_WEST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST),    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES_EAST, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST),  
	  SCENE_EXIT(7, JupiterLighthouseRoomRedKey, 8),   SCENE_EXIT(8, JupiterLighthouseRoomRedKey, 7), SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST), 
	  SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST), SCENE_EXITS_END,                                
  };

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 608, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 608, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 552, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 600, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 664, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 328, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 328, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{SCENE_TABLE_END},
};

#include "shared/orb_rising_script.c"

/* How many sparks the ring takes each tick, and whether the rising sound */
/* plays with them. */
Sint32 JupiterLighthouseSparksPerTick = 0;
Sint32 JupiterLighthouseSparkSoundOn = 1;

CUE SceneCues[] = {
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, SCRIPT_REF(scJupiterLighthouseHearTheVoice)},
	{CUE_STEP, 22, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, SCRIPT_REF(scJupiterLighthouseLeaveTheCenter)},
	{CUE_STEP, 100, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseAlexHealsTheParty)},
	{CUE_TALK, ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgFieldFollowTrueFeelings)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgFieldPartWithAgatio)},
	{CUE_PUSH, -1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLiftPushedBlock)},
	{CUE_PUSHED, -1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePlacePushedBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PUSHED_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLiftPushedBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PUSHED_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePlacePushedBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PUSHED_BLOCK + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLiftPushedBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PUSHED_BLOCK + 1, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePlacePushedBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LAST_PUSHED_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLiftPushedBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_PUSHED_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePlacePushedBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseLiftPushedBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePlacePushedBlock)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_ACTS), 0, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, SCRIPT_REF(scJupiterLighthouseActivateTheLighthouse)},
	{CUE_TABLE_END},
};

Sint32 PaletteCycleTick = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
/* clang-format on */

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};
