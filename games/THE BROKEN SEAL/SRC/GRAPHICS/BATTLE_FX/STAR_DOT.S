	.section .rodata
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/STAR_DOT.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0x89e
