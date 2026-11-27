/* Flashes an actor's sprite on its timer: white three frames in four, its */
/* own colours on the fourth, redrawn each frame. */
void fdActorCyclePictures(ACTOR *actor) {
	ANIMATED_SPR *sprite = (ANIMATED_SPR *)actor->spr;
	SPR_ANIMATION *animation = sprite->anm;

	switch(actor->tmr & 3) {
	case 0:
		animation->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = 2;
		break;
	case 1:
		sprite->dirty = 1;
		sprite->flg = 0;
		break;
	case 2:
		animation->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = 0;
		break;
	case 3:
		animation->colMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		sprite->flg = 0;
		break;
	}
	actor->tmr++;
}
