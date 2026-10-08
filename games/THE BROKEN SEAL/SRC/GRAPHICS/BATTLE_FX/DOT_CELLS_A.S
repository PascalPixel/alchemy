	.section .rodata
	.global BattleFx_ParticleSpritesA
BattleFx_ParticleSpritesA:
	.incbin "GRAPHICS/BATTLE_FX/DOT_CELLS_A.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_MagentaTailSheet at +0x299 */
	.byte 0x0c, 0x40, 0x08
