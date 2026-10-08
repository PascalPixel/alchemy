	.section .rodata
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_B.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_ParticleSpritesA at +0x1bb */
	.byte 0x94
