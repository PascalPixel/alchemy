/* The base of Mercury Lighthouse in Imil, near the end of the game: as the last
   beacon, Mars Lighthouse's, is lit, two of Imil's children hear the voices of
   the heroes far away, glints of light rising round them. */

#include "field/script_inline.h"
#include "game/game.h"
#include "field/field_sprite.h"
#include "field/actor_script.h"
#include "field/field_mode.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "macros.h"

void scMercuryLighthouseHearVoices(void);

extern Sint32					   MercuryLighthouseGlintScript[];
Uint32 MercuryLighthouseGlintTicks BSS_DATA; /* ticks the glint has run */

enum {
	/* Unused: an entrance no exit leads to, which the map does not have. By it
	   the screen fades to white to SE_ASCEND, as the Mars Lighthouse aerie does
	   before it sends the party here, and the party comes in again by
	   EXIT_BACK_TO_THE_VOICES. */
	ENTRANCE_UNUSED_WHITE_FADE = 12,
	/* Back into this scene by the entrance where the voices begin. */
	EXIT_BACK_TO_THE_VOICES = 9
};

/* A glint's script call: it flickers, hidden and white by turns every two
   ticks. */
Sint32 scMercuryLighthouseFlickerGlint(ACTOR *actor)
{
	if((MercuryLighthouseGlintTicks >> 1) & 1)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	return 0;
}

/* An Imil villager's update while the voices talk: every 40 to 120 ticks it
   turns between two headings, as if looking around for them. */
void scMercuryLighthouseLookAround(ACTOR *actor)
{
	Uint8 *tmr = &actor->sceneCnt;
	Uint8 *state;

	if(*tmr != 0) {
		(*tmr)--;
		return;
	}
	*tmr = (syRandomBelow(80)) + 40;
	state = &actor->sceneByte;
	switch(*state) {
	case 0:
		actor->facing = FACING_WEST;
		*state = 1;
		break;
	case 1:
		actor->facing = FACING_NORTH_NORTHWEST;
		*state = 0;
		break;
	case 2:
		actor->facing = FACING_EAST;
		*state = 3;
		break;
	case 3:
		actor->facing = FACING_NORTH_NORTHEAST;
		*state = 2;
		break;
	}
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

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* The actors at the lighthouse, by the rows of ActorPlacements; Mia's, Garet's
   and Sheba's rows go to their party slots. */
enum {
	/* Two of Imil's children, who hear the party: "Hey! I know that voice! It's
	   Mia, I just know it!" and "Of course it's us! We can hear you, Mia!" */
	ACTOR_BOY = ACTOR_FIRST_PLACED,
	ACTOR_GIRL,
	/* placed at the map's corner, out of sight; nothing uses her */
	ACTOR_WOMAN,
	/* "I don't know. They've been standing here telling everyone to
	   leave..." */
	ACTOR_OLD_MAN,
	ACTOR_OLD_WOMAN, /* "Who are those two kids talking to?" */
	/* Two unseen actors the voices' Mind Read wave runs between. */
	ACTOR_VOICES_SOURCE,
	ACTOR_VOICES_TARGET
};

/* The event scMercuryLighthouseHearVoices starts: every 16 ticks it starts a
   flickering glint (SPRITE_GLINT) at a random spot in front of the lighthouse,
   and it counts the ticks in MercuryLighthouseGlintTicks. */
void scMercuryLighthouseSpawnGlints(void)
{
	ACTOR *actor;

	fdActorSetSprPriority(ACTOR_BOY, 3);
	fdActorSetSprPriority(ACTOR_GIRL, 3);
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	if((TickCount & 15) == 0) {
		actor = syImportActorCreate(SPRITE_GLINT, 0, 0, 0);
		if(actor != NULL) {
			FIELD_SPR *sprite = actor->spr;
			FIXED	   x = syRandom() * 40 + toFIXED(812);
			FIXED	   z = toFIXED(696);

			actor->motionFlg = 0;
			actor->x.fixed = x;
			actor->y.fixed = toFIXED(160);
			actor->z.fixed = z;
			fdActorSetSprFlags(actor, 0);
			actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			sprite->pri = 2;
			fdActorStartScript(actor, MercuryLighthouseGlintScript);
			fdActorSelectAnimation(actor, 1);
		}
	}
	MercuryLighthouseGlintTicks++;
}

/* As the Mars beacon is lit, two children of Imil at the base of Mercury
   Lighthouse hear Mia and her friends far away, each of their lines carried on
   a Mind Read wave, until Sheba knows who sent the children there: "The... The
   Wise One!" The party then returns to the aerie, its beacon burning. */
void scMercuryLighthouseHearVoices(void)
{
	ACTOR *actor;
	Uint8  delay;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	scFieldActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
		SPRITE_COLOR_HIDDEN);
	sySynch(1);
	MercuryLighthouseGlintTicks = 0;
	sySetEvent(scMercuryLighthouseSpawnGlints, EVENT_PASS_TICK);
	fdActorSetSprPriority(ACTOR_BOY, 2);
	fdActorSetSprPriority(ACTOR_GIRL, 2);
	FieldStateWork->transitionFrm = 40;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	fdScriptSetMessageId(MsgImilVoicesDisbeliefReply);
	/* "You... You can't be!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	fdActorShowEmote(ACTOR_BOY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_BOY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_BOY, FACING_SOUTH_SOUTHEAST, 0);
	fdActorJump(ACTOR_BOY, 4, 10);
	/* "Hey! I know that voice! It's Mia, I just know it! Mia!!!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
	fdActorShowEmote(ACTOR_OLD_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_OLD_MAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_OLD_WOMAN, FACING_NORTH_NORTHWEST, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "It's you!" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	fdActorSetAnimation(ACTOR_GIRL, ANIM_NOD);
	/* "Of course it's us! We can hear you, Mia! You're all right! I'm so
	   happy!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
	fdActorFaceDirectionAndWait(ACTOR_OLD_WOMAN, FACING_EAST);
	/* "Who are those two kids talking to?" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_OLD_WOMAN, 0);
	fdActorFaceDirectionAndWait(ACTOR_OLD_MAN, FACING_WEST);
	fdActorSetAnimation(ACTOR_OLD_MAN, ANIM_SHAKE_HEAD);
	/* "I don't know. They've been standing here telling everyone to
	   leave..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_OLD_MAN, 0);
	actor = fdActorGetById(ACTOR_OLD_MAN);
	delay = (syRandomBelow(80)) + 40;
	actor->sceneCnt = delay;
	actor->sceneByte = 0;
	actor->update = (OBJECT_UPDATE)scMercuryLighthouseLookAround;
	actor = fdActorGetById(ACTOR_OLD_WOMAN);
	delay = (syRandomBelow(80)) + 40;
	actor->sceneCnt = delay;
	actor->sceneByte = 2;
	actor->update = (OBJECT_UPDATE)scMercuryLighthouseLookAround;
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "How is it that we can hear you?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	fdActorSetAnimation(ACTOR_BOY, ANIM_SHAKE_HEAD);
	/* "I don't know, but he told us we'd be able to talk to you if we
	   came..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "He? Who's "he"?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	/* "I don't know... We were sleeping, and he came to us in a dream...
	   He..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "The lighthouse? Why?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	/* "He said that we have to deliver a message... He was too busy to
	   do..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "What message?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	/* "We have to warn everyone to stay clear of the lighthouses!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "What's going to happen?" */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	fdActorSetAnimationAndWait(ACTOR_BOY, ANIM_NOD);
	/* "I don't know, but he told us to warn people away from Mt. Aleph as
	   well!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "But who is "he"!? You still haven't told us!!!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdPsynergyStopMindReadWave();
	fdActorJump(ACTOR_GIRL, 2, 0);
	scFieldActorShowEmote(ACTOR_GIRL, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "You're a meanie! I don't tell meanies anything!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "Don't pay any attention to him... It's me, Mia... Can you tell me..." */
	fdScriptShowMessage(ACTOR_MIA, 0);
	fdPsynergyStopMindReadWave();
	fdActorSetAnimationAndWait(ACTOR_BOY, ANIM_NOD);
	/* "I told you, I don't know, but he looked like a big rock... with a..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
	fdPsynergyStartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
	/* "The... The Wise One!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdPsynergyStopMindReadWave();
	fdScriptWait(20);
	/* The backdrop, black. */
	BG_PLTT_COLOR(0, 0) = RGB(0, 0, 0);
	sySynch(1);
	FieldStateWork->transitionFrm = 16;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetFlag(FLAG_MARS_BEACON_BURNING);
	syFieldScriptSetExitRequest(
		MERCURY_LIGHTHOUSE_TO_MARS_LIGHTHOUSE_AERIE_EAST_B);
}

Sint32 scInit(void)
{
	if(MainRoutineStateWork.entrance == ENTRANCE_UNUSED_WHITE_FADE) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptPlaySnd(SE_ASCEND);
		fdPaletteFadeSetTarget(RGB_WHITE, 0);
		fdPaletteFadeStartInterpolation(20);
		sySynch(20);
		syFieldScriptSetExitRequest(EXIT_BACK_TO_THE_VOICES);
	} else {
		scMercuryLighthouseHearVoices();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

Sint32 MercuryLighthouseGlintScript[] = {
	SC_ADD(SW_Y, toFIXED(-2)),
	SC_CALL(scMercuryLighthouseFlickerGlint),
	SC_WAIT(1),
	SC_ADD(SW_Y, toFIXED(-2)),
	SC_CALL(scMercuryLighthouseFlickerGlint),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, toFIXED(-1)),
	SC_CALL(scMercuryLighthouseFlickerGlint),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_RELEASE,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(MercuryLighthouse),
	SCENE_EXIT(2, MercuryLighthouse,
		MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_TO_MARS_LIGHTHOUSE_AERIE_EAST_B,
		MarsLighthouseAerie,
		MARS_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EAST_B),
	SCENE_EXIT(EXIT_BACK_TO_THE_VOICES, MercuryLighthouse,
		MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 696, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 906, 0, 744, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 762, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 780, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE SceneCues[] = {
	{CUE_TABLE_END},
};
