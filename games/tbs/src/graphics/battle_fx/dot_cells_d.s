	.section .rodata
	.global BattleFx_ParticleSpritesD
BattleFx_ParticleSpritesD:
	.incbin "graphics/battle_fx/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleFx_ParticleSpritesC+0x417
