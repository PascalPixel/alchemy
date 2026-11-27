/* Creates a hidden, still actor of `kind` at (x, y, z), untargeted and drawn */
/* underfoot, its sprite at priority 1. */
ACTOR *scCreateActor(s32 x, s32 y, s32 z, s32 kind) {
	ACTOR *actor;

	actor = scFieldActorCreate(kind, x, y, z);
	if(actor != 0) {
		scSetActorSprPriority(actor, 1);
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		scFieldActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg = ACTOR_PRIORITY_UNDERFOOT |
						ACTOR_PRIORITY_IGNORES_RISES | actor->priFlg;
		return actor;
	}
	return 0;
}
