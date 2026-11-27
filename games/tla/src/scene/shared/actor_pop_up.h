/* fdActorPopUpSlow without the wait before it rises. */
s32 fdActorPopUp(ACTOR *actor) {
	s32 frame = actor->tmr;

	if(frame == 0) {
		fdEventPlaySnd(SE_WHOOSH);
		fdActorSelectAnimation((ACTOR *)actor->watchedActor, 2);
		actor->y.fixed =
			fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) +
			toFIXED(16);
		actor->scaleX = frame;
		actor->scaleY = frame;
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 2;
		fdActorSetSprPriorityDirect(actor, 1);
	}
	else if(frame <= 32) {
		actor->scaleX = FIXED_ONE;
		actor->scaleY = FIXED_ONE;
		{
			s32 half = (actor->tmr / 2) & 1;

			fdActorSetSprColMode((ACTOR *)actor->watchedActor,
								 half * SPRITE_COLOR_WHITE);
		}
	}
	else {
		fdActorSelectAnimation((ACTOR *)actor->watchedActor, 1);
		fdEventPlaySnd(SE_POP_UP);
		actor->y.fixed =
			fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		actor->watchedActor = 0;
		return 0;
	}
	actor->tmr++;
	return 1;
}
