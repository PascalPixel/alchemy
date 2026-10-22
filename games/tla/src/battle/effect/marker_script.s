	.section .rodata
@ The Broken Seal's gBattleEffect_MarkerParticleScript, in this game's command
@ numbers: call Field_IsRevealActive, then free the object.
	.global Data_080f01b8
Data_080f01b8:
	.4byte 0x2e, Field_IsRevealActive	@ call
	.4byte 0x26			@ free
