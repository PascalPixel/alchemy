	.section .rodata
	.global BattleFx_ParticleSpritesB
BattleFx_ParticleSpritesB:
	.incbin "graphics/battle_fx/dot_cells_b.parts.lz"
	.byte 0x94	@ PACKER: left in the buffer by BattleFx_ParticleSpritesA+0x1bb
