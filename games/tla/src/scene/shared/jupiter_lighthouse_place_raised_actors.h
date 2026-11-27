#include "jupiter_lighthouse_aerie.h"

/* Puts the aerie's two angels and the well's lid where the archers' arrows */
/* left them (FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT and its right */
/* twin): each shot angel and its archer glow, the angel rises and its */
/* side of the lid lifts; with both, the angels and the lid float over the */
/* well. */
void scJupiterLighthousePlaceRaisedActors(void) {
	ACTOR *leftAngel = fdActorGet(ACTOR_LEFT_ANGEL);
	ACTOR *rightAngel = fdActorGet(ACTOR_RIGHT_ANGEL);
	ACTOR *lid = fdActorGet(ACTOR_LID);
	FIELD_SPR *sprite = lid->spr;

	fdActorSetPos(ACTOR_UNSEEN_LID, 512, 248);
	fdActorSetPos(ACTOR_LID, 512, 248);
	fdActorSetPos(ACTOR_LEFT_ANGEL, 472, 256);
	fdActorSetPos(ACTOR_RIGHT_ANGEL, 552, 256);
	fdActorGet(ACTOR_UNSEEN_LID)->scaleX = 0;
	lid->motionFlg = 0;
	lid->y.fixed = lid->groundHeight;
	leftAngel->motionFlg = 0;
	leftAngel->y.fixed = leftAngel->groundHeight;
	rightAngel->motionFlg = 0;
	rightAngel->y.fixed = rightAngel->groundHeight;
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT)) {
		leftAngel->y.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 10);
		{
			s32 step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100);

			lid->y.fixed += step;
		}
		lid->x.fixed -= ANGEL_RISE_FRAMES * (toFIXED(1) / 25);
		sprite->rotation += TRIG_TURN / 32;
		fdActorSetSprColMode(fdActorGet(ACTOR_LEFT_ANGEL), SPRITE_COLOR_GBR);
		fdActorSetSprColMode(fdActorGet(ACTOR_LEFT_ARCHER), SPRITE_COLOR_GBR);
	}
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT)) {
		rightAngel->y.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 10);
		{
			s32 step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100);

			lid->y.fixed += step;
		}
		lid->x.fixed += ANGEL_RISE_FRAMES * (toFIXED(1) / 25);
		sprite->rotation -= TRIG_TURN / 32;
		fdActorSetSprColMode(fdActorGet(ACTOR_RIGHT_ANGEL), SPRITE_COLOR_GBR);
		fdActorSetSprColMode(fdActorGet(ACTOR_RIGHT_ARCHER), SPRITE_COLOR_GBR);
	}
	if(fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT) &&
	   fdEventGetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT)) {
		leftAngel->y.fixed += LID_LIFT_FRAMES * (toFIXED(1) / 10);
		rightAngel->y.fixed += LID_LIFT_FRAMES * (toFIXED(1) / 10);
		{
			FIXED step = ANGEL_RISE_FRAMES * (toFIXED(3) / 100) +
						 LID_LIFT_FRAMES * (toFIXED(1) / 10);

			lid->y.fixed += step;
		}
		leftAngel->motionFlg = ACTOR_FLOATS;
		rightAngel->motionFlg = ACTOR_FLOATS;
		lid->motionFlg = ACTOR_FLOATS;
	}
}
