	.section .rodata
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_RockWallSheet at +0x89e */
	.byte 0x01, 0xba
