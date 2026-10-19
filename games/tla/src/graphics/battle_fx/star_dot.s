	.section .rodata
	.global BattleFx_StarDotSheet
BattleFx_StarDotSheet:
	.incbin "games/tla/src/graphics/battle_fx/star_dot_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/star_dot.parts.lz"
	.byte 0x01, 0xba	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0x89e
