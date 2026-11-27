#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

/* @file */
/* Creating actors and starting their scripts and animations from a scene. */

#include "fixed_math.h"
#include "types.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"

#include "field/actor.h"

/* ACTOR with its timer unsigned, as the two scene functions that keep a
 */
/* flag number in it store it: Tundaria Tower's and the Mars Lighthouse */
/* entrance's placing of their actors from a table. Through ACTOR.tmr, an */
/* s16, the store compiles differently. */
typedef struct FIELD_ACTOR {
	u8 unused[85]; /* no code names it */
	u8 motionFlg;
	u8 unusedAfterMotionFlg[3]; /* no code names it */
	u8 collisionFlg;
	u8 unusedAfterCollisionFlg[10]; /* no code names it */
	u16 tmr;
} FIELD_ACTOR;

void syImportEventSetExitRequest(s32 exit);
ACTOR *syImportActorCreate(s32 type, s32 x, s32 y, s32 z);
/* Starts an animation on an actor's sprite (fdActorSelectAnimation, through */
/* the overlay's own import row). */
void syImportActorSelectAnimation(ACTOR *actor, s32 animation);
/* Gives an actor a script and starts it from the top (fdActorStartScript, */
/* through the overlay's own import row). */
void syImportActorStartScript(ACTOR *actor, const void *script);

#endif
