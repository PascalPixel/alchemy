/* fdParticleSetAnimation and fdParticleSetCallback, the same in both games.
   The Broken Seal's field/script_commands_field_particles.c and The Lost
   Age's field/field_psynergy_step_vector.c include it where the functions
   stand. */

/* Sets the animation of a field effect sprite. */
void fdParticleSetAnimation(FIELD_PARTICLE *effect, Sint32 mode)
{
	fdSprStartAnimationStub(effect->spr, mode);
}

/* Gives a field effect sprite its per-tick callback, starting at state 0. */
void fdParticleSetCallback(FIELD_PARTICLE *effect, EFFECT_CALLBACK func)
{
	effect->func = func;
	effect->funcDelay = 0;
	effect->age = 0;
	effect->state = 0;
}
