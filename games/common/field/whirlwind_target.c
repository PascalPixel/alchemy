/* fdWhirlwindUpdateTarget and fdPsynergyWhirlwindOnTarget, the same in both
   games. The Broken Seal's field/script_commands_whirlwind.c and The Lost
   Age's field/field_psynergy_radial_effects.c include it where the functions
   stand. */

/* Per-tick update of Whirlwind's target: it shrinks a 512th a tick and, below
   half its size, is moved to the map's origin, stopped and its update
   removed. */
void fdWhirlwindUpdateTarget(ACTOR *actor)
{
	Sint32 scale;

	scale = actor->scaleX - WHIRLWIND_TARGET_SCALE_STEP;
	actor->scaleY = scale;
	actor->scaleX = scale;
	if(scale < WHIRLWIND_TARGET_MIN_SCALE) {
		fdActorSetPosAndResetMotionStub(actor, 0, 0, 0);
		actor->update = 0;
	}
}

/* Runs Whirlwind cast on a target, using the target already stored in the field
   effect. */
void fdPsynergyWhirlwindOnTarget(Sint32 unused)
{
	fdPsynergyWhirlwind();
}
