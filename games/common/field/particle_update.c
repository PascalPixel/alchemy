/* fdParticleUpdate, the same in both games. The Broken Seal's
   field/script_commands_field_particles.c and The Lost Age's
   field/field_psynergy_step_vector.c include it where the function stands. */

/* Updates one field effect sprite: counts its age, runs its callback (after any
   delay), moves it and draws it. */
void fdParticleUpdate(FIELD_PARTICLE *effect)
{
	if(effect->active != 0) {
		effect->age++;
		if(effect->funcDelay != 0)
			effect->funcDelay--;
		else if(effect->func != 0)
			effect->func(effect);
		/* A callback may release its own slot; do not move or draw it
		   afterward. */
		if(effect->active != 0) {
			if(effect->updateMotion != 0)
				fdParticleUpdateMotion(effect);
			if(effect->render != 0)
				fdEffectDrawScaledActor(effect);
		}
	}
}
