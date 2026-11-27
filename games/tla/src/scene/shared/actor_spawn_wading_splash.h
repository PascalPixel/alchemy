/* A splash at the actor's feet as it wades, on its sprite's priority. */
void scActorSpawnWadingSplash(ACTOR *actor)
{
	ACTOR	  *child = scFieldActorCreate(SPRITE_WADING_SPLASH, actor->x.fixed,
			actor->y.fixed + toFIXED(4), actor->z.fixed + toFIXED(4));
	FIELD_SPR *src = actor->spr;

	if(child != 0) {
		FIELD_SPR *spr = child->spr;

		child->groundHeight = actor->groundHeight;
		fdActorStartScript(child, WadingSplashScript);
		child->motionFlg = 0;
		if(spr != 0) {
			fdSprStartAnimation(spr, 1);
			spr->flg = 0;
			spr->pri = src->pri;
		}
	}
}
