#ifndef ALCHEMY_FIELD_SCRIPT_H
#define ALCHEMY_FIELD_SCRIPT_H

/* Creating actors and starting their scripts and animations from a scene. */

#include "fixed_math.h"
#include "types.h"
#include "field/field_sprite.h"
#include "field/field_mode_script.h"

#include "field/actor.h"

/* ACTOR with its timer unsigned, as the two scene functions that keep a flag
   number in it store it: Tundaria Tower's and the Mars Lighthouse entrance's
   placing of their actors from a table. Through ACTOR.tmr, an Sint16, the store
   compiles differently. */
typedef struct FIELD_ACTOR {
	Uint8  unused[85]; /* no code names it */
	Uint8  motionFlg;
	Uint8  unusedAfterMotionFlg[3]; /* no code names it */
	Uint8  collisionFlg;
	Uint8  unusedAfterCollisionFlg[10]; /* no code names it */
	Uint16 tmr;
} FIELD_ACTOR;

void   syImportScriptSetExitRequest(Sint32 exit);
ACTOR *syImportActorCreate(Sint32 type, Sint32 x, Sint32 y, Sint32 z);
/* Starts an animation on an actor's sprite (fdActorSelectAnimation, through the
   overlay's own import row). */
void syImportActorSelectAnimation(ACTOR *actor, Sint32 anm);
/* Gives an actor a script and starts it from the top (fdActorStartScript,
   through the overlay's own import row). */
void syImportActorStartScript(ACTOR *actor, const void *script);

#endif
