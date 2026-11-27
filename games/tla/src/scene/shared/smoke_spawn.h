/* Adds one white puff of chimney smoke at (x, y, z), which scSmokeDrift moves
   for SMOKE_TICKS. */
void scSmokeSpawn(Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, x, y, z);

	if(actor != NULL) {
		actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
		actor->motionFlags = 0;
		actor->timer = 0;
		actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
		scFieldActorSetSpriteColor(actor, SPRITE_COLOR_WHITE);
		((FIELD_SPR *)actor->sprite)->priority = 1;
		fdActorSetSpriteFlags(actor, 0);
		actor->scaleX = toFIXED(1) / 2;
		actor->scaleY = toFIXED(1) / 2;
		*(Sint32 *)&actor->watchedActor = SMOKE_TICKS;
		actor->update = (OBJECT_UPDATE)scSmokeDrift;
		fdActorSelectAnimation(actor, 5);
		fdActorSetBlendMode(actor, 1);
	}
}
