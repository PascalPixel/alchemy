	.section .rodata
	.global BattleFx_StarCrescentSheet
BattleFx_StarCrescentSheet:
	.incbin "games/tla/src/graphics/battle_fx/star_crescent.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/star_crescent.bitmap.lz"
	.byte 0x03	@ PACKER: left in the buffer by BattleFx_GreyCrescentSheet+0xadb
