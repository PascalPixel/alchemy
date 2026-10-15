	.section .rodata
	.global BattleFx_GreatSwordSheet
BattleFx_GreatSwordSheet:
	.incbin "games/tla/src/graphics/battle_fx/great_sword.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/great_sword.bitmap.lz"
	.byte 0x70, 0xa8, 0x48	@ PACKER: left in the buffer by Resource_Data0DA+0x6bd
