/* fdSprDissolveRun and fdSprDissolveRunMany, the same in both games. Both
   games' field/field_actors.c include it where the functions stand. */

/* Dissolves a sprite pixel by pixel, four steps a tick, over 32 ticks. */
void fdSprDissolveRun(Uint8 *spr)
{
	Uint32 step;

	step = 0;
	/* Finish all four dissolve steps before waiting for the next video
	   frame. */
	do {
		fdSprDissolveStep(spr, step);
		fdSprDissolveStep(spr, step + 1);
		fdSprDissolveStep(spr, step + 2);
		fdSprDissolveStep(spr, step + 3);
		step += 4;
		sySynch(1);
	} while(step <= DISSOLVE_STEPS - 1);
}

/* Dissolves count sprites together, pixel by pixel, over 32 ticks. */
void fdSprDissolveRunMany(Uint8 **sprs, Sint32 cnt)
{
	Uint8 **firstSpr;
	Uint8 **sprCursor;
	Sint32	sprsRemaining;
	Uint32	step;
	Uint32	secondDissolveStep;
	Uint32	thirdDissolveStep;
	Uint32	fourthDissolveStep;

	firstSpr = sprs;
	for(step = 0; step <= DISSOLVE_STEPS - 1U; step += 4) {
		if(cnt > 0) {
			secondDissolveStep = step + 1;
			thirdDissolveStep = step + 2;
			sprCursor = firstSpr;
			fourthDissolveStep = step + 3;
			sprsRemaining = cnt;
			do {
				fdSprDissolveStep(*sprCursor, step);
				fdSprDissolveStep(*sprCursor, secondDissolveStep);
				fdSprDissolveStep(*sprCursor, thirdDissolveStep);
				sprsRemaining -= 1;
				fdSprDissolveStep(*sprCursor++, fourthDissolveStep);
			} while(sprsRemaining != 0);
		}
		sySynch(1U);
	}
}
