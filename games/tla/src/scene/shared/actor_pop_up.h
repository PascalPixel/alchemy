// scActorPopUpSlow without the wait before it rises.
Bool scActorPopUp(ACTOR *actor)
{
	Sint32 frame = actor->timer;

	if (frame == 0)
	{
		fdScriptPlaySound(SE_WHOOSH);
		fdActorSelectAnimation((ACTOR *)actor->watchedActor, 2);
		actor->y.fixed    = fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed) + toFIXED(16);
		actor->scaleX     = frame;
		actor->scaleY     = frame;
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 2;
		fdActorSetSpritePriorityDirect(actor, 1);
	}
	else if (frame <= 32)
	{
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
		{
			Sint32 half = (actor->timer / 2) & 1;

			fdActorSetSpriteColorMode((ACTOR *)actor->watchedActor, half * SPRITE_COLOR_WHITE);
		}
	}
	else
	{
		fdActorSelectAnimation((ACTOR *)actor->watchedActor, 1);
		fdScriptPlaySound(SE_POP_UP);
		actor->y.fixed      = fdMapGetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
		actor->watchedActor = 0;
		return FALSE;
	}
	actor->timer++;
	return TRUE;
}
