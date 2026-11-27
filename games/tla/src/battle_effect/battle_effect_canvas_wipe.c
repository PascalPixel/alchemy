/* The canvas wipe: the ragged sweep that blacks out or clears the canvas as the
   summons and the larger effects open and close. Part of battle_effect.c. */

/* How far a row of btEffectWipeCanvas's wipe lags behind its front, at most:
   each row lags a random 0 to WIPE_LAG_MAX pixels. */
#define WIPE_LAG_MAX	63
/* Where the across wipe's front stops, three frames past */
/* CANVAS_SIZE + WIPE_LAG_MAX, where the wipe is finished. */
#define WIPE_ACROSS_END 256

/* A direction btEffectWipeCanvas leaves the canvas alone for; no caller passes
   it. */
#define CANVAS_WIPE_NONE 3

/* Wipes the whole canvas to one colour with a ragged edge, a frame at a time:
   23 frames across, 15 down. Each frame sets state->transferPending, so the
   caller's transfer event must already be running.

   direction: CANVAS_WIPE_ACROSS sweeps left to right, CANVAS_WIPE_NONE does
			  nothing, anything else sweeps top to bottom.
   fill: what the wipe leaves (CANVAS_FILL). */
void btEffectWipeCanvas(Sint32 direction, Sint32 fill)
{
	BATTLE_EFFECT_STATE *state;
	Uint8				*canvas;
	Uint8				 delay[CANVAS_SIZE];
	Sint32				 x;
	Sint32				 y;
	Sint32				 wipePosition;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	for(x = 0; x != CANVAS_SIZE; x++)
		delay[x] = syRandom() & WIPE_LAG_MAX;

	if(direction == CANVAS_WIPE_ACROSS) {
		Sint32 front;
		Sint32 speed;

		front = 0;
		speed = 1;
		x = 0;
		do {
			front += speed;
			speed++;
			for(; x != front; x++) {
				for(y = 0; y != CANVAS_SIZE; y++) {
					wipePosition = x - delay[y];
					if(wipePosition >= 0)
						if(wipePosition < CANVAS_SIZE)
							canvas[CANVAS_PIXEL_OFFSET(wipePosition, y)] =
								1 - fill;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		} while(front <= WIPE_ACROSS_END);
	} else if(direction != CANVAS_WIPE_NONE) {
		Sint32 front;
		Sint32 speed;

		front = 0;
		speed = 1;
		y = 0;
		do {
			front += speed / 2;
			speed += 4;
			for(; y != front; y++) {
				for(x = 0; x != CANVAS_SIZE; x++) {
					wipePosition = y - delay[x];
					if(wipePosition >= 0)
						if(wipePosition < CANVAS_SIZE)
							canvas[CANVAS_PIXEL_OFFSET(x, wipePosition)] =
								1 - fill;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		} while(front <= CANVAS_SIZE + WIPE_LAG_MAX);
	}
}

#undef WIPE_LAG_MAX
#undef WIPE_ACROSS_END
#undef CANVAS_WIPE_NONE
