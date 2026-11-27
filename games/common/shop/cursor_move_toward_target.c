/* mnCursorMoveTowardTarget, the same in both games. Both games'
   shop/shop_cursor.c include it where the function stands. */

/* Eases a cursor toward its target, a quarter of the way on each axis a tick
   and at least a pixel; the settings and name-entry screens move their markers
   with it. */
void mnCursorMoveTowardTarget(MENU_CURSOR *cursor)
{
	WINDOW_SPR *spr;
	Sint32		delta;
	Sint32		step;

	spr = cursor->anchor;
	if(spr != NULL) {
		delta = spr->x - cursor->tgtX;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				spr->x = spr->x - step;
			else
				spr->x = spr->x - 1;
			spr->spr.oam.x = spr->x;
		} else if(delta < 0) {
			if(step != 0)
				spr->x = spr->x + step;
			else
				spr->x = spr->x + 1;
			spr->spr.oam.x = spr->x;
		}

		delta = spr->y - cursor->tgtY;
		step = delta / 4;
		if(step < 0)
			step = -step;
		if(delta > 0) {
			if(step != 0)
				spr->y = spr->y - step;
			else
				spr->y = spr->y - 1;
			spr->spr.oam.y = spr->y;
		} else if(delta < 0) {
			if(step != 0)
				spr->y = spr->y + step;
			else
				spr->y = spr->y + 1;
			spr->spr.oam.y = spr->y;
		}
	}
}
