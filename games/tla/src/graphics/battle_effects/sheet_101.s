	.section .rodata
	.global Resource_Data101
Resource_Data101:
	.incbin "games/tla/src/graphics/battle_effects/sheet_101.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_101.bitmap.lz"
	.byte 0xa5, 0x0a, 0x23	@ PACKER: left in the buffer by Resource_Data0F8+0x9a5
