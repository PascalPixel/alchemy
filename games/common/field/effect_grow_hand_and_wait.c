/* fdEffectGrowHandAndWait, the same in both games. The Broken Seal's
   field/script_commands_move.c and The Lost Age's
   field/script_commands_mind_read.c include it where the function stands. */

/* Grows a hand to full size at once: when the actor is smaller, sets its scale
   to its first step of 1/16 past toFIXED(1) - 1 (exactly toFIXED(1) from a
   multiple of 1/16), then waits, at most 600 ticks, for it to reach where it is
   going. */
void fdEffectGrowHandAndWait(ACTOR *actor)
{
	FIXED next;
	FIXED scale;

	if(actor != NULL) {
		scale = actor->scaleX;
		if(scale < toFIXED(1)) {
			do {
				next = scale + toFIXED(1) / 16;
				scale = next;
			} while(next < toFIXED(1));
			actor->scaleX = next;
			actor->scaleY = next;
		}
		fdActorWaitForMoveStub(actor);
	}
}
