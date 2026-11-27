/* A script call that shows an actor 16 pixels over the ground: unseen for half
   a second, then whole while the actor it watches whooshes into its second
   animation and flashes white every other two frames for half a second more;
   then that one takes its first animation again with a pop, and the actor drops
   to the ground. 1 while it runs. */
Bool scActorPopUpSlow(ACTOR *actor)
{
	Sint32 frame = actor->tmr;

	if(frame == 0) {
		actor->y.fixed =
			fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) +
			toFIXED(16);
		actor->scaleX = frame;
		actor->scaleY = frame;
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1);
		fdActorSetSprPriorityDirect(actor, 1);
	} else if(frame <= 30) {
		if(frame == 30) {
			fdScriptPlaySnd(SE_WHOOSH);
			fdActorSelectAnimation((ACTOR *)actor->watchedActor, 2);
		}
	} else if(frame <= 60) {
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
		{
			Sint32 half = (actor->tmr / 2) & 1;

			fdActorSetSprColMode(
				(ACTOR *)actor->watchedActor, half * SPRITE_COLOR_WHITE);
		}
	} else {
		fdActorSelectAnimation((ACTOR *)actor->watchedActor, 1);
		fdScriptPlaySnd(SE_POP_UP);
		actor->y.fixed =
			fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		actor->watchedActor = 0;
		return FALSE;
	}
	actor->tmr++;
	return TRUE;
}
