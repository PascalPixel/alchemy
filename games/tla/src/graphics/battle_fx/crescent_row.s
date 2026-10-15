	.section .rodata
	.global BattleFx_CrescentRowSheet
BattleFx_CrescentRowSheet:
	.incbin "games/tla/src/graphics/battle_fx/crescent_row.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/crescent_row.bitmap.lz"
	.byte 0x26	@ PACKER: left in the buffer by BattleFx_PetalSheet+0x9bb
