@ BattleFx_StagedParticlePath: the motion path of the staged particle effect,
@ one step per frame.
	.section .rodata
	.global BattleFx_StagedParticlePath
BattleFx_StagedParticlePath:
	.incbin "GRAPHICS/BATTLE_FX/STAGED_PARTICLE_PATH.table"
