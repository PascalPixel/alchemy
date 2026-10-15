	.section .rodata
	.global BattleFx_VineCardSheet
BattleFx_VineCardSheet:
	.incbin "games/tla/src/graphics/battle_fx/vine_card.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/vine_card.bitmap.lz"
	.byte 0x1d, 0x6a, 0x00	@ PACKER: left in the buffer by Resource_Data0DD+0x1039
