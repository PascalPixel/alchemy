/* Adds one white puff of chimney smoke at (x, y, z), which scSmokeDrift moves
   for SMOKE_FRAMES. */
void scSmokeSpawn(Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, x, y, z);

	if(actor != NULL) {
		actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
		((FIELD_SPR *)actor->spr)->pri = 1;
		fdActorSetSprFlags(actor, 0);
		actor->scaleX = toFIXED(1) / 2;
		actor->scaleY = toFIXED(1) / 2;
		*(Sint32 *)&actor->watchedActor = SMOKE_FRAMES;
		actor->update = (OBJECT_UPDATE)scSmokeDrift;
		fdActorSelectAnimation(actor, 5);
		fdActorSetBlendMode(actor, 1);
	}
}
