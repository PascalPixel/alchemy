/* @file */
/* Tolbi's ship deck and Vale's storm: things that throb. */

/* An actor script call, each frame: every 60 to 149 frames the actor */
/* squashes and stretches back to its own shape over six frames. */
static s32 scSquashAndStretch(ACTOR *state) {
	switch(state->tmr) {
	case 6:
		state->scaleX -= FIXED_ONE / 4;
		state->scaleY += FIXED_ONE / 8;
		break;
	case 4:
		state->scaleX += FIXED_ONE / 8;
		state->scaleY -= FIXED_ONE / 16;
		break;
	case 2:
		state->scaleX += FIXED_ONE / 16;
		state->scaleY -= FIXED_ONE / 32;
		break;
	case 0:
		state->scaleX = FIXED_ONE;
		state->scaleY = FIXED_ONE;
		state->tmr = SQUASH_AND_STRETCH_PAUSE();
		break;
	}
	state->tmr--;
	return 1;
}
