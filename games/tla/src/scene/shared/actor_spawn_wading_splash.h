/* A splash at the actor's feet as it wades, on its sprite's priority. */
void scActorSpawnWadingSplash(ACTOR *actor)
{
	ACTOR	  *child = scFieldActorCreate(SPRITE_WADING_SPLASH, actor->x.fixed,
			actor->y.fixed + toFIXED(4), actor->z.fixed + toFIXED(4));
	FIELD_SPR *source = actor->spr;

	if(child != 0) {
		FIELD_SPR *sprite = child->spr;

		child->groundHeight = actor->groundHeight;
		fdActorStartScript(child, WadingSplash_Script);
		child->motionFlg = 0;
		if(sprite != 0) {
			fdSprStartAnimation(sprite, 1);
			sprite->flg = 0;
			sprite->pri = source->pri;
		}
	}
}
