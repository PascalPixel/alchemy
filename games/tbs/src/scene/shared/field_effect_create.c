/* @file */
/* Twenty-three scenes, most of those that spawn effects */
/* (field_effect_spawn.c): two ways to place a still actor of a given sprite, */
/* each taking whole pixels, with an FX form for a 16.16 position. */

/* Sets the actor's sprite priority (0 in front of the backgrounds, 3 */
/* behind). */
UNUSED static void scSetActorSprPriority(ACTOR *actor, s32 priority) {
	actor->spr->image.pri = priority;
}

/* A still, translucent black actor in front of the backgrounds (sprite */
/* priority 0), at a 16.16 position. */
UNUSED static void *scActorPrepareFX(FIXED x, FIXED y, FIXED z, s32 type) {
	ACTOR *actor = syFieldActorCreate(type, x, y, z);

	if(actor != NULL) {
		actor->spr->image.pri = 0;
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(actor, ACTOR_BLEND_TRANSLUCENT);
		return actor;
	}
	return NULL;
}

/* scActorPrepareFX at a point in whole pixels: */
/* `fdActorPrepare(464, 0, 290, PIT_MASK)`. */
#define fdActorPrepare(x, y, z, type)                                          \
	scActorPrepareFX(toFIXED(x), toFIXED(y), toFIXED(z), type)

/* A still, invisible actor at sprite priority 1, sorted under the other */
/* actors, at a 16.16 position. */
UNUSED static void *scActorCreateConfiguredFX(FIXED x, FIXED y, FIXED z,
											  s32 type) {
	ACTOR *actor = syFieldActorCreate(type, x, y, z);

	if(actor != NULL) {
		actor->spr->image.pri = 1;
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg = (actor->priFlg & ~ACTOR_PRIORITY_AUTOMATIC) |
						ACTOR_PRIORITY_UNDERFOOT;
		return actor;
	}
	return NULL;
}

/* scActorCreateConfiguredFX at a point in whole pixels: */
/* `fdActorCreateConfigured(128, 0, 420, SPRITE_CRATE)`. */
#define fdActorCreateConfigured(x, y, z, type)                                 \
	scActorCreateConfiguredFX(toFIXED(x), toFIXED(y), toFIXED(z), type)
