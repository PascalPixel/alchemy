/* Creates a hidden, still actor of `kind` at (x, y, z), untargeted and drawn
   underfoot, its sprite at priority 1. */
ACTOR *scCreateActor(Sint32 x, Sint32 y, Sint32 z, Sint32 kind)
{
	ACTOR *actor;

	actor = scFieldActorCreate(kind, x, y, z);
	if(actor != 0) {
		fdSetActorSpritePriority(actor, 1);
		actor->motionFlags = 0;
		actor->collisionFlags = ACTOR_NOT_TARGETED;
		fdActorSetSpriteFlags(actor, 0);
		scFieldActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT |
			ACTOR_PRIORITY_IGNORES_RISES | actor->priorityFlags;
		return actor;
	}
	return 0;
}
