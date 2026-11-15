	.section .rodata
@ The Broken Seal's table of this name, in this game's command
@ numbers: call Field_IsRevealActive, then free the object.
	.global gBattleEffect_MarkerParticleScript
gBattleEffect_MarkerParticleScript:
	.4byte 0x2e, Field_IsRevealActive	@ call
	.4byte 0x26			@ free
