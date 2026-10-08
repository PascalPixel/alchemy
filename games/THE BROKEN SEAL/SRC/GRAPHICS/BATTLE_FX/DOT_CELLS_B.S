	.section .rodata
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_B.parts.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleFx_ParticleSpritesA+0x1bb
