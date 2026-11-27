// Tolbi's ship deck and Vale's storm: things that throb.

// An actor script call, each tick: every 60 to 149 ticks the actor squashes and stretches back to
// its own shape over six ticks.
static Sint32 scSquashAndStretch(ACTOR *state)
{
	switch (state->timer)
	{
		case 6:
			state->scaleX -= toFIXED(1) / 4;
			state->scaleY += toFIXED(1) / 8;
			break;
		case 4:
			state->scaleX += toFIXED(1) / 8;
			state->scaleY -= toFIXED(1) / 16;
			break;
		case 2:
			state->scaleX += toFIXED(1) / 16;
			state->scaleY -= toFIXED(1) / 32;
			break;
		case 0:
			state->scaleX = toFIXED(1);
			state->scaleY = toFIXED(1);
			state->timer  = SQUASH_AND_STRETCH_PAUSE();
			break;
	}
	state->timer--;
	return 1;
}
