/* fdEffectClearOwnedSlot, the same in both games. The Broken Seal's
   field/script_commands_field_particles.c and The Lost Age's
   field/field_psynergy_step_vector.c include it where the function stands. */

/* Frees a field effect sprite and its sprite, clearing it to zero
   (inactive). */
void fdEffectClearOwnedSlot(FIELD_PARTICLE *effect)
{
	if(effect->spr)
		fdSprDestroyStub(effect->spr);
	DMA_FILL32(0, effect, sizeof *effect);
}
