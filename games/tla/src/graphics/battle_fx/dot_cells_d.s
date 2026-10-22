	.section .rodata
	.global BattleEffect_ParticleSpritesD
BattleEffect_ParticleSpritesD:
	.incbin "games/tla/src/graphics/battle_fx/dot_cells_d.parts.lz"
	.byte 0x0c	@ PACKER: left in the buffer by BattleEffect_ParticleSpritesC+0x417
