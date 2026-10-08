	.section .rodata
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_D.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x417
