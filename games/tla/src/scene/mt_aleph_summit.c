/* The summit of Mt. Aleph, at the end of the game: the Golden Sun forms over
   the mountain, Alex claims its power, and the scene's colours and dust follow
   what happens. */
#include "scene/script_layer.h"
#include "graphics/palette_buffers.h"
#include "field/script_inline.h"
#include "game/game.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "../field/actor_set_sprite_priority.c"

/* The summit, by the rows of ActorPlacements. */
typedef enum MT_ALEPH_SUMMIT_ACTOR_ID {
	ACTOR_ALEX = ACTOR_FIRST_PLACED,
	ACTOR_WISE_ONE /* "I am called the Wise One..." */
} MT_ALEPH_SUMMIT_ACTOR_ID;

/* The summit's ways in, each a stretch of the ending. Nothing in the game
   enters it any other way; one that does goes on to the credits' opening at
   once, unless B is held, which keeps it on the summit until B is let go or A
   is pressed. */
enum {
	/* Alex sees the Golden Sun begin to form and climbs toward it, between the
	   credits' first two stretches. */
	ENTRANCE_GOLDEN_SUN_FORMS = 2,
	/* Alex takes the Golden Sun's power and the Wise One shows him its limits,
	   after the lighthouses' beacons. */
	ENTRANCE_ALEX_TAKES_THE_POWER = 3,
	/* Unused: goes straight on to the credits' world map, by the Jupiter
	   Lighthouse (its entrance 72). */
	ENTRANCE_SKIP_TO_CREDITS = 10
};

/* Alex's lines that open a run of the summit's talk, as messages after
   MsgGoldenSunEternalLifeAndPower. */
typedef enum MT_ALEPH_SUMMIT_ALEX_LINE {
	ALEX_WHO_ARE_YOU = 4, /* "Wh-Who are you?" (MsgGoldenSunWhoAreYou) */
	/* "If you are trying to anger me, have a little taste of exactly how much
	   power I have attained!" */
	ALEX_TRYING_TO_ANGER_ME = 12
} MT_ALEPH_SUMMIT_ALEX_LINE;

/* The stretches of the staff credits the summit goes on to (staff_credit.c's
   entrances): the opening, which comes back to the Golden Sun forming, and the
   one that goes on to the lighthouses' beacons. */
enum { STAFF_CREDIT_OPENING = 0, STAFF_CREDIT_TO_BEACONS = 1 };

/* The epilogue's entrance from the summit, once the Wise One has gone. The
   epilogue reads no entrance and lists only its default
   (shared/scene_entrances_mapless.c): the number is the summit's alone. */
enum { EPILOGUE_FROM_MT_ALEPH_SUMMIT = 1 };

#include "../field/field_effect_move.c"

/* The palette fade's three channels of one 16-colour palette. */
#define PALETTE_CHANNELS (3 * PALETTE_COLORS)

extern const Sint32 *FieldEffect_DurationScripts[];

extern Uint32 FieldEffect_ShortScript[];

extern Uint32 FieldEffect_MediumScript[];
extern Uint32 FieldEffect_LongScript[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

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

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* Moves each of count colours one step of red, green and blue toward its target
   colour. */
void scMtAlephSummitStepColsToward(Uint16 *colors, Uint16 *target, Sint32 count)
{
	Sint32 i = 0;

	if(count == 0)
		return;
	do {
		Sint32 to_r = RGB_RED(*target);
		Sint32 to_g = RGB_GREEN(*target);
		Sint32 to_b = RGB_BLUE(*target);
		Sint32 r = RGB_RED(*colors);
		Sint32 g = RGB_GREEN(*colors);
		Sint32 b = RGB_BLUE(*colors);

		if(r < to_r)
			r++;
		else if(r > to_r)
			r--;
		if(g < to_g)
			g++;
		else if(g > to_g)
			g--;
		if(b < to_b)
			b++;
		else if(b > to_b)
			b--;
		*colors = RGB(r, g, b);
		i++;
		target++;
		colors++;
	} while(i != count);
}

/* Adds delta to each of count colours' red, green and blue, kept within 0 to
   31. */
void scMtAlephSummitShiftCols(Uint16 *colors, Sint32 count, Sint32 delta)
{
	Sint32 i;
	Sint32 r, g, b;

	i = 0;
	if(count == 0)
		return;
	for(; i != count; i++) {
		r = RGB_RED(*colors) + delta;
		g = RGB_GREEN(*colors) + delta;
		b = RGB_BLUE(*colors) + delta;
		if(r > RGB_LEVEL_MAX)
			r = RGB_LEVEL_MAX;
		if(r < 0)
			r = 0;
		if(g > RGB_LEVEL_MAX)
			g = RGB_LEVEL_MAX;
		if(g < 0)
			g = 0;
		if(b > RGB_LEVEL_MAX)
			b = RGB_LEVEL_MAX;
		if(b < 0)
			b = 0;
		*colors++ = RGB(r, g, b);
	}
}

/* An event: background palette 4's colours 1 to 15 pulse up to two levels
   brighter and darker than palette 6's, once every 64 frames. */
void scMtAlephSummitPulseCols(void)
{
	Uint16 *src = (Uint16 *)&BG_PLTT_COLOR(6, 1);
	Uint16 *dst = (Uint16 *)&BG_PLTT_COLOR(4, 1);
	/* The pulse turns a 64th of a turn a frame. */
	Sint32 delta = FIXEDtoINT(syMathSin(FrameCount * (TRIG_TURN / 32) / 2) * 2);
	Sint32 i;

	for(i = 0; i != PALETTE_COLORS - 1; i++, src++, dst++) {
		Sint32 r = RGB_RED(*src) + delta;
		Sint32 g = RGB_GREEN(*src) + delta;
		Sint32 b = RGB_BLUE(*src) + delta;

		if(r > RGB_LEVEL_MAX)
			r = RGB_LEVEL_MAX;
		if(r < 0)
			r = 0;
		if(g > RGB_LEVEL_MAX)
			g = RGB_LEVEL_MAX;
		if(g < 0)
			g = 0;
		if(b > RGB_LEVEL_MAX)
			b = RGB_LEVEL_MAX;
		if(b < 0)
			b = 0;
		*dst = RGB(r, g, b);
	}
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"
/* clang-format on */

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0,
		344, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120,
		0, 24, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_TABLE_END},
};

#include "../field/field_effect_advance_motion.c"

/* The summit's scripts: the Golden Sun's formation and what follows it. */

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);

/* Scatters count dust puffs in a ring of radius around Alex. */
void scMtAlephSummitScatterDustRing(Sint32 radius, Uint32 count)
{
	ACTOR		  *actor;
	EFFECT_OPTIONS options;
	VECTOR3		   dir;
	Uint32		   i;
	Sint32		   angle;

	actor = fdActorGet(ACTOR_ALEX);
	options.update = fdEffectAdvanceMotion;
	options.startScaleX = toFIXED(4) / 5;
	options.startScaleY = toFIXED(4) / 5;
	options.pri = ((FIELD_SPR *)actor->spr)->pri;
	i = 0;
	do {
		angle = i * (TRIG_TURN / (Sint32)count);
		dir.x = syCos(angle) * radius / 256;
		dir.y = 0;
		dir.z = syMathSin(angle) * radius / 256;
		dir.x += dir.x >> 1;
		dir.x += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
		dir.z += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
		scFieldEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
			dir.x, dir.y, dir.z,
			EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
				EFFECT_SCRIPT_MEDIUM,
			&options);
		i++;
	} while(i <= count);
}

void scMtAlephSummitStepColsToward(
	Uint16 *colors, Uint16 *target, Sint32 count);
void scMtAlephSummitShiftCols(Uint16 *colors, Sint32 count, Sint32 delta);
void scMtAlephSummitPulseCols(void);

/* Alex's climb as the Golden Sun forms: he creeps up the mountain's side a
   pixel every 30 frames for CREEP_STEPS steps, the camera drifting after him
   from the third; the sky turns gold in SKY_GOLD_STEPS steps of four frames;
   then he climbs two pixels every eight frames for CLIMB_STEPS steps, the
   camera panning up to the summit at CLIMB_CAMERA_STEP and the screen fading
   out at CLIMB_FADE_STEP. */
#define CREEP_STEPS				 10
#define SKY_GOLD_STEPS			 10
/* Each step of the sky's turn to gold raises its green by one level for every
   SKY_RED_LEVELS_PER_GREEN its red rises, more than red ever rises in one. */
#define SKY_RED_LEVELS_PER_GREEN 16
#define CLIMB_STEPS				 20
#define CLIMB_CAMERA_STEP		 16
#define CLIMB_FADE_STEP			 19

/* Alex takes the power. While the summit shakes, background 2 scrolls eight
   pixels every five frames for BG2_SCROLL_STEPS steps. Then, in POWER_STEPS
   steps of four frames, Alex turns golden at POWER_ALEX_GOLDEN and the column
   of light (background 1) blends in a level every two steps, up to 8 by
   POWER_BLEND_PAUSE and on from POWER_BLEND_RESUME to full strength by
   POWER_BLEND_FULL; the summit (palette 4) turns to palette 6's gold from
   POWER_SUMMIT_GOLDEN; the background brightens a level a step from
   POWER_WHITEN; and at POWER_TREMOR the earth trembles and the colours saved at
   the start are put back under the brightening. */
#define BG2_SCROLL_STEPS	30
#define POWER_STEPS			84
#define POWER_ALEX_GOLDEN	16
#define POWER_SUMMIT_GOLDEN 20
#define POWER_BLEND_PAUSE	28
#define POWER_BLEND_RESUME	34
#define POWER_BLEND_FULL	50
#define POWER_WHITEN		50
#define POWER_TREMOR		51

/* The Wise One's blinks at each of its four sways as Alex gathers his power. */
#define WISE_ONE_BLINKS 10

/* The passes of the other entrances' hold on the summit, which waits no frame
   between them. */
#define SUMMIT_HOLD_PASSES 320

/* Plays the stretch of the ending the entrance names, the Golden Sun forming or
   Alex taking its power until Mt. Aleph sinks, then goes on to the credits or
   the epilogue. */
Sint32 scInit(void)
{
	PALETTE_FADE	*fade;
	void			*buffer;
	Sint32			 mode;
	Sint32			 i, j;
	FIXED			 scale;
	Sint32			 r, g, b, value;
	Sint32			 c;
	Uint16			*color, *out;
	ACTOR			*actor;
	volatile Uint32 *held;
	HEAP_SLOTS		*heap;
	MAP_STATE		*map;

	buffer = syHeapAllocEWRAM(BG_PLTT_SIZE);
	syWindowLoadGraphics();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	fdActorSetPos(ACTOR_FELIX, 0, 0);
	mode = MainRoutineStateWork.entrance;
	if(mode == ENTRANCE_SKIP_TO_CREDITS) {
		fdScriptSetDestination(
			SCENE_STAFF_CREDIT, STAFF_CREDIT_FROM_MT_ALEPH_SUMMIT);
	} else if(mode == ENTRANCE_GOLDEN_SUN_FORMS) {
		fdCameraMoveTo(256, CAMERA_KEEP, 128, 0);
		fdCameraSetSpeed(1);
		fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
		fdActorSetAnimation(ACTOR_ALEX, 12);
		fdActorSetAnimationSpeedById(ACTOR_ALEX, ANIM_STEP_NORMAL / 2);
		FieldStateWork->transition = 0;
		REG_DISPCNT &= ~DISPCNT_BG1_ON;
		REG_BG1CNT &= ~BGCNT_PRIORITY(3);
		REG_BG1CNT |= BGCNT_PRIORITY(3);
		REG_BG3CNT &= ~BGCNT_PRIORITY(3);
		REG_BG3CNT |= BGCNT_PRIORITY(2);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdCameraMoveTo(256, CAMERA_KEEP, 318, 1);
		fdCameraWaitForArrival();
		fdCameraMoveTo(316, CAMERA_KEEP, 318, 1);
		fdCameraWaitForArrival();
		for(i = 0; i != CREEP_STEPS; i++) {
			if(i == 3) {
				fdCameraSetSpeed(0.1);
				fdCameraMoveTo(316, CAMERA_KEEP, 300, 1);
			}
			fdActorGet(ACTOR_ALEX)->z.fixed -= toFIXED(1);
			syWait(30);
		}
		fdActorSetAnimationSpeedById(ACTOR_ALEX, 0);
		/* DEAD: nothing reads the buffer in this stretch; the ROM keeps the
		   copy. */
		syWordsCopy(buffer, (void *)&BG_PLTT_COLOR(0, 4), 14);
		/* The sky, background palette 0's colours 7 to 14, turns gold: red
		   rises and blue falls to no less than 8. */
		for(j = 0; j != SKY_GOLD_STEPS; j++) {
			out = (Uint16 *)&BG_PLTT_COLOR(0, 7);
			color = out;
			for(i = 0; i != 8; i++) {
				c = *color;
				r = RGB_RED(c);
				g = RGB_GREEN((Uint16)c);
				b = RGB_BLUE((Uint16)c);
				r += j;
				/* DEAD: red rises fewer than SKY_RED_LEVELS_PER_GREEN levels in
				   every step, so green never changes; the ROM keeps its shift
				   and add. */
				g += j / SKY_RED_LEVELS_PER_GREEN;
				b -= j;
				if(r > RGB_LEVEL_MAX)
					r = RGB_LEVEL_MAX;
				if(g > RGB_LEVEL_MAX)
					g = RGB_LEVEL_MAX;
				if(b < 8)
					b = 8;
				if(b > RGB_LEVEL_MAX)
					b = RGB_LEVEL_MAX;
				*out = RGB(r, g, b);
				color++;
				out++;
			}
			syWait(4);
		}
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		syWait(30);
		syFieldScriptSetMessageId((Sint32)MsgGoldenSunFormation);
		/* "The Golden Sun... The very quintessence of Alchemy's power!
		   It's..." */
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ALEX, 0);
		syWait(30);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		/* "Wait for me! Please, wait! Wait until I reach the summit of Mt.
		   Aleph!" */
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ALEX, 0);
		fdActorSetAnimationSpeedById(ACTOR_ALEX, ANIM_STEP_NORMAL * 2);
		for(i = 0; i != CLIMB_STEPS; i++) {
			if(i == CLIMB_CAMERA_STEP) {
				fdCameraMoveTo(316, CAMERA_KEEP, 128, 1);
				fdCameraSetSpeed(2);
			}
			if(i == CLIMB_FADE_STEP)
				fdScriptFinishScreenTransition();
			fdActorGet(ACTOR_ALEX)->z.fixed -= toFIXED(2);
			syWait(8);
		}
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_TO_BEACONS);
	} else if(mode == ENTRANCE_ALEX_TAKES_THE_POWER) {
		heap = Ram_HeapSlots;
		fade = heap->palFade;
		syWordsCopy(buffer, (void *)&BG_PLTT_COLOR(0, 0),
			PALETTE_FADE_BG_COLORS * sizeof(Uint16));
		fdCameraMoveTo(256, CAMERA_KEEP, 144, 0);
		fdCameraSetSpeed(1);
		fdActorSetAnimation(ACTOR_ALEX, 14);
		actor = fdActorGet(ACTOR_ALEX);
		actor->facing = 0;
		fdActorGet(ACTOR_ALEX)->x.fixed = toFIXED(248);
		fdActorGet(ACTOR_ALEX)->z.fixed = toFIXED(152);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
			BLDCNT_TGT1_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1,
			IO_WRITE_HALFWORD);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
			BLEND_ALPHA(0, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
		REG_DISPCNT &= ~DISPCNT_BG1_ON;
		REG_BG1CNT &= ~BGCNT_PRIORITY(3);
		REG_BG1CNT |= BGCNT_PRIORITY(3);
		REG_BG3CNT &= ~BGCNT_PRIORITY(3);
		REG_BG3CNT |= BGCNT_PRIORITY(2);
		syWait(1);
		((FIELD_STATE *)heap->fieldState)->transition = 0;
		REG_DISPCNT &= ~DISPCNT_BG1_ON;
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		fdMapGridSetShake(4, 4, 1);
		for(i = 0; i != BG2_SCROLL_STEPS; i++) {
			BgScrollWork[2].y += 8;
			syFieldScriptWait(5);
		}
		fdActorSetAnimation(ACTOR_ALEX, 15);
		syWait(60);
		fdScriptPlaySnd(SE_POWER_UP);
		REG_BG1CNT &= ~BGCNT_PRIORITY(3);
		REG_BG1CNT |= BGCNT_PRIORITY(1);
		REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
			BLEND_ALPHA(2, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
		for(j = 0; j != POWER_STEPS; j++) {
			if(j == POWER_ALEX_GOLDEN) {
				fdScriptPlaySnd(SE_SPIN);
				fdActorSetAnimation(ACTOR_ALEX, 16);
			}
			if(j == POWER_TREMOR) {
				fdScriptPlaySnd(SE_TREMOR);
				syWordsCopyEntry(buffer, (void *)&BG_PLTT_COLOR(0, 0),
					PALETTE_FADE_BG_COLORS * sizeof(Uint16));
				syWordsCopyEntry(&fade->tgt[4 * PALETTE_CHANNELS],
					&fade->tgt[6 * PALETTE_CHANNELS],
					PALETTE_CHANNELS * sizeof(Uint16));
			}
			if(j >= POWER_ALEX_GOLDEN && j <= POWER_BLEND_PAUSE) {
				value = (j - POWER_ALEX_GOLDEN) / 2 + 2;
				IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
					BLEND_ALPHA(value, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
			}
			if(j >= POWER_BLEND_RESUME && j <= POWER_BLEND_FULL) {
				value = (j - POWER_BLEND_RESUME) / 2 + 8;
				IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
					BLEND_ALPHA(value, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
			}
			if(j >= POWER_SUMMIT_GOLDEN && j <= POWER_TREMOR)
				scMtAlephSummitStepColsToward((Uint16 *)&BG_PLTT_COLOR(4, 0),
					(Uint16 *)&BG_PLTT_COLOR(6, 0), 16);
			if(j >= POWER_WHITEN)
				scMtAlephSummitShiftCols(
					(Uint16 *)&BG_PLTT_COLOR(0, 0), PALETTE_FADE_BG_COLORS, 1);
			syWait(4);
		}
		scFieldActorSetSprPriority(ACTOR_ALEX, 3);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
		fdCameraMoveTo(256, CAMERA_KEEP, 318, 0);
		IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG1CNT,
			(REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3),
			IO_WRITE_HALFWORD);
		fdScriptPlaySnd(SE_POWER_UP);
		syWait(1);
		sySetFrameEvent(scMtAlephSummitPulseCols, EVENT_PASS_FRAME);
		/* The background walks back to the saved colours, a level a step, as
		   many steps as a channel has levels. */
		for(j = 0; j != RGB_LEVEL_MAX + 1; j++) {
			scMtAlephSummitStepColsToward(
				(Uint16 *)&BG_PLTT_COLOR(0, 0), buffer, PALETTE_FADE_BG_COLORS);
			syWait(4);
		}
		scFieldActorSetSprPriority(ACTOR_ALEX, 1);
		fdActorSetAnimation(ACTOR_ALEX, 1);
		fdCameraMoveTo(256, CAMERA_KEEP, 128, 1);
		fdCameraSetSpeed(1);
		fdCameraWaitForArrival();
		fdActorSetSpeed(ACTOR_ALEX, 0.3);
		fdActorWalkToAndWait(ACTOR_ALEX, 248, 128);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		syFieldScriptSetMessageId((Sint32)MsgGoldenSunEternalLifeAndPower);
		/* "At last! I have it! Eternal life...and limitless power!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
		syFieldScriptWait(30);
		fdActorSetAnimation(ACTOR_ALEX, 14);
		/* "At last, the power of nature is mine to control as I will!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_ALEX, 15);
		/* "Rise, storms! Rise up and unleash your might upon Vale and
		   the..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 150);
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorSetAnimation(ACTOR_ALEX, 1);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
		syFieldScriptWait(10);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
		syFieldScriptWait(10);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
		syFieldScriptWait(10);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
		syFieldScriptWait(10);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
		syFieldScriptWait(50);
		fdActorSetAnimation(ACTOR_ALEX, 15);
		syFieldScriptWait(50);
		/* "That's odd... I should have limitless power... So why can't I
		   call..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		syFieldScriptWait(50);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		fdActorSetSprFlags(fdActorGet(ACTOR_WISE_ONE), 0);
		fdActorWalkToAndWait(ACTOR_WISE_ONE, 216, 96);
		fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		syFieldScriptSetMessageId(
			(Sint32)MsgGoldenSunEternalLifeAndPower + ALEX_WHO_ARE_YOU);
		/* "Wh-Who are you?" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdScriptPlaySnd(MUS_ELEMENTAL_STARS);
		/* "I am called the Wise One..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorSetAnimation(ACTOR_ALEX, 1);
		fdActorFaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdActorGet(ACTOR_ALEX)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_ALEX, 256, 136);
		fdActorSetSpeed(ACTOR_ALEX, 0.6);
		syFieldScriptWait(20);
		fdActorSetAnimation(ACTOR_ALEX, 1);
		/* "The Wise One? Vale's protector?" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 4);
		/* "You wish to have limitless power?" */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		/* "Wish to? I just got it!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 5);
		/* "No. Your power is nearly limitless, but it has boundaries." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorSetAttachedEffect(
			ACTOR_ALEX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		syFieldScriptWait(20);
		/* "Nearly limitless? You speak in riddles. Can't you see? The power
		   is..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 6);
		/* "Yes. You also have nearly endless life. And your Psynergy is..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
		syFieldScriptSetMessageId(
			(Sint32)MsgGoldenSunEternalLifeAndPower + ALEX_TRYING_TO_ANGER_ME);
		/* "If you are trying to anger me, have a little taste of exactly
		   how..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_ALEX, 13);
		fdBattleEffectStartArcEffect();
		fdBattleEffectSetObjectUpdateMode(
			fdActorGet(ACTOR_ALEX), CASTER_LOOK_CYCLING_COLORS);
		fdScriptPlaySnd(SE_GATHERING_POWER);
		fdScriptStartColTransition(8, RGB(4, 4, 16));
		syFieldScriptWait(20);
		fdActorSetAnimation(ACTOR_WISE_ONE, 3);
		fdActorSetSpeed(ACTOR_WISE_ONE, 0.1);
		for(j = 0; j != 4; j++) {
			if(j & 1) {
				fdActorWalkToAndWait(ACTOR_WISE_ONE, 216, 96);
			} else {
				fdActorGet(ACTOR_WISE_ONE)->steeringFlg &= ~ACTOR_FACES_MOTION;
				fdActorWalkTo(ACTOR_WISE_ONE, 208, 88);
			}
			for(i = 0; i != WISE_ONE_BLINKS; i++) {
				fdActorSetSprColById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
				syFieldScriptWait(3);
				fdActorSetSprColById(ACTOR_WISE_ONE, 0);
				syFieldScriptWait(3);
			}
		}
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
		/* "Look at me! My body is brimming with power!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		syFieldScriptWait(10);
		fdBattleEffectSetObjectUpdateMode(
			fdActorGet(ACTOR_ALEX), CASTER_LOOK_NORMAL);
		fdBattleEffectWaitForArcGroups();
		fdBattleEffectEndArcEffect();
		fdActorSetAnimation(ACTOR_WISE_ONE, 6);
		fdBattleEffectStartArcEffect();
		fdBattleEffectSetObjectUpdateMode(
			fdActorGet(ACTOR_WISE_ONE), CASTER_LOOK_CYCLING_COLORS);
		syWait(30);
		fdScriptPlaySnd(SE_POWER_UP);
		fdMapGridSetShake(16, 16, 1);
		for(i = 0; i != 4; i++) {
			fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
			fdPaletteFadeInterpolate(1);
			syFieldScriptWait(2);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeInterpolate(1);
			syFieldScriptWait(2);
		}
		fdMapGridSetShake(0, 0, 0);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		fdStartPaletteTransition(1);
		fdScriptStartColTransition(8, RGB(16, 8, 4));
		syFieldScriptWait(20);
		fdActorSetSpeed(ACTOR_ALEX, 8);
		scMtAlephSummitScatterDustRing(128, 8);
		fdActorGet(ACTOR_ALEX)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkTo(ACTOR_ALEX, 280, 160);
		fdActorSetAnimation(ACTOR_ALEX, 17);
		fdScriptPlaySnd(SE_HEAVY_IMPACT);
		syFieldScriptWait(2);
		scMtAlephSummitScatterDustRing(128, 8);
		syFieldScriptWait(2);
		scMtAlephSummitScatterDustRing(128, 8);
		syFieldScriptWait(56);
		fdScriptPlaySnd(SE_GATHERING_POWER);
		i = 0;
		fdActorGet(ACTOR_ALEX)->motionFlg = i;
		for(; i != 64; i++) {
			if(i == 16)
				fdActorSetAnimation(ACTOR_ALEX, 18);
			fdActorGet(ACTOR_ALEX)->y.fixed += toFIXED(3) / 20;
			syFieldScriptWait(1);
		}
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		/* "How!? What's going on? I should be all-powerful! How can you
		   defeat..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		for(i = 0; i != 32; i++) {
			fdActorGet(ACTOR_ALEX)->y.fixed += toFIXED(1);
			syFieldScriptWait(1);
		}
		fdActorGet(ACTOR_ALEX)->motionFlg = ACTOR_FOLLOWS_TERRAIN;
		for(i = 0; i != 5; i++) {
			fdActorGet(ACTOR_ALEX)->y.fixed -= toFIXED(832) / 100;
			syFieldScriptWait(1);
		}
		scMtAlephSummitScatterDustRing(256, 16);
		fdActorSetAnimation(ACTOR_ALEX, 17);
		fdScriptPlaySnd(SE_CRASH);
		fdMapGridSetShake(16, 16, 1);
		syFieldScriptWait(10);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdBattleEffectSetObjectUpdateMode(
			fdActorGet(ACTOR_WISE_ONE), CASTER_LOOK_NORMAL);
		fdBattleEffectWaitForArcGroups();
		fdBattleEffectEndArcEffect();
		fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
		fdActorSetAnimation(ACTOR_WISE_ONE, 4);
		fdStartPaletteTransition(32);
		syFieldScriptWait(10);
		/* "You are not all-powerful, Alex. Your power has its limits, as
		   does..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		/* "That cannot be! Who is responsible for this treachery? Who
		   has..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 3);
		/* "I, the Wise One, imbued the Mars Star with some of the power of
		   the..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorSetAnimation(ACTOR_ALEX, 18);
		syFieldScriptWait(10);
		/* "Why?" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		syFieldScriptWait(120);
		fdScriptPlaySnd(SE_TREMOR);
		for(i = 0; i != 2; i++) {
			fdMapGridSetShakeFX(
				toFIXED(i) + toFIXED(1), toFIXED(i) + toFIXED(1), toFIXED(1));
			syFieldScriptWait(8);
		}
		fdActorSetAnimation(ACTOR_WISE_ONE, 8);
		syFieldScriptWait(60);
		fdActorSetAnimation(ACTOR_WISE_ONE, 7);
		/* "The heavens and earth are changing, Alex! You must flee now!" */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		/* "Wha-What!?" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 8);
		/* "Mt. Aleph will soon be drawn into the heart of the earth! You
		   must..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		for(i = 0, j = toFIXED(2); i != 4; i++, j += toFIXED(1)) {
			fdMapGridSetShakeFX(j, j, toFIXED(1));
			syFieldScriptWait(8);
		}
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		/* "Flee?! I can't flee! I can't even move!" */
		syFieldScriptShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 3);
		/* "Ah, yes. You now see the limits of your power. If you are
		   swallowed..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
		syFieldScriptWait(5);
		fdActorSetAnimation(ACTOR_WISE_ONE, 4);
		/* "If you survive, perhaps we shall meet again someday..." */
		syFieldScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
		syFieldScriptWait(10);
		map = Ram_HeapSlots->mapGrid;
		fdMapGridSetShake(16, 16, 1);
		for(j = 0, scale = -toFIXED(15) / 8; j != 128;
			j++, scale += toFIXED(1) / 16) {
			if(j == 20)
				fdActorSetAnimation(ACTOR_WISE_ONE, 7);
			if(j >= 30 && j <= 46) {
				fdActorGet(ACTOR_WISE_ONE)->scaleX = toFIXED(1) - scale;
				fdActorGet(ACTOR_WISE_ONE)->scaleY = scale + toFIXED(1);
				fdActorGet(ACTOR_WISE_ONE)->z.fixed -= toFIXED(2);
			}
			if(j == 47)
				fdActorSetPos(ACTOR_WISE_ONE, 0, 0);
			if(j == 64) {
				fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
				fdPaletteFadeInterpolate(64);
			}
			map->layers[1].ofsY -= toFIXED(1);
			fdActorGet(ACTOR_ALEX)->z.fixed += toFIXED(1);
			syWait(1);
		}
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		syFieldScriptWait(32);
		fdScriptPlaySnd(SE_BANE_ESCAPE);
		syFieldScriptWait(120);
		syFieldCloseEvent(scMtAlephSummitPulseCols);
		fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
		fdPaletteFadeInterpolate(1);
		syWait(1);
		fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
		fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
		fdPaletteFadeInterpolate(120);
		syWait(300);
		fdScriptSetDestination(SCENE_EPILOGUE, EPILOGUE_FROM_MT_ALEPH_SUMMIT);
	} else {
		FieldStateWork->transition = 0;
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionFrames();
		for(j = 0; j != SUMMIT_HOLD_PASSES; j++) {
			held = &PerWork.data;
			if(*held & KEY_B)
				j = 0;
			if(*held & KEY_A)
				break;
			if(j == 0) {
				fdActorSetSprFlags(fdActorGet(ACTOR_ALEX), 0);
				fdActorSetAnimation(ACTOR_ALEX, 12);
			}
		}
		fdScriptSetDestination(SCENE_STAFF_CREDIT, STAFF_CREDIT_OPENING);
	}
	syHeapRelease(buffer);
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}
