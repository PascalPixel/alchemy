	.section .rodata
	.global BattleFx_ParticleSpritesA
BattleFx_ParticleSpritesA:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_A.parts.lz"
	.byte 0x0c, 0x40, 0x08	@ PACKER: left in the buffer by BattleFx_MagentaTailSheet+0x299
