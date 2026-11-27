/* Flashes an actor's sprite on its timer: white three ticks in four, its own
   colours on the fourth, redrawn each tick. */
void scActorCyclePictures(ACTOR *actor)
{
	ANIMATED_SPR  *spr = (ANIMATED_SPR *)actor->spr;
	SPR_ANIMATION *anm = spr->anm;

	switch(actor->tmr & 3) {
	case 0:
		anm->colMode = SPRITE_COLOR_WHITE;
		spr->dirty = 1;
		spr->flg = 2;
		break;
	case 1:
		spr->dirty = 1;
		spr->flg = 0;
		break;
	case 2:
		anm->colMode = SPRITE_COLOR_WHITE;
		spr->dirty = 1;
		spr->flg = 0;
		break;
	case 3:
		anm->colMode = SPRITE_COLOR_NORMAL;
		spr->dirty = 1;
		spr->flg = 0;
		break;
	}
	actor->tmr++;
}
