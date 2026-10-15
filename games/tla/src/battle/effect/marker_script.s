	.section .rodata
@ The Broken Seal's BattleFx_MarkerParticleScript, in this game's command
@ numbers: call Func_080cf158 (The Broken Seal's EffectRuntime_IsActive),
@ then free the object.
	.global Data_080f01b8
Data_080f01b8:
	.4byte 0x2e, Func_080cf158	@ call
	.4byte 0x26			@ free
