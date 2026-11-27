/* fdEffectPulseActorScale and fdEffectAdvanceAngWithRamp, the same in both
   games. Both games' field/script_commands_jupiter.c include it where the
   functions stand. */

/* Per-tick update: makes an actor pulse, switching its scale between the two
   values of FieldEffectPulseScales every 4 ticks. */
void fdEffectPulseActorScale(ACTOR *actor)
{
	const Uint32 *tbl = FieldEffectPulseScales;
	Uint32		  idx = ((Uint32)FrameCount >> 2) & 1;
	Uint32		  scale = tbl[idx];
	actor->scaleX = scale;
	actor->scaleY = scale;
}

/* Per-tick update: spins an actor ever faster, from 1/16 turn a tick up to
   just over 1/8 (DEGtoANG(45) + 64), 80 more each tick for 52 ticks. */
void fdEffectAdvanceAngWithRamp(ACTOR *actor)
{
	Uint32 step;

	step = actor->spinRampFrm * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP) {
		actor->spinRampFrm = actor->spinRampFrm + 1;
	}
}
