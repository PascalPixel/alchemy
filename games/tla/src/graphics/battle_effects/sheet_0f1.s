	.section .rodata
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f1_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f1.parts.lz"
	.byte 0xc2, 0x3a, 0x02	@ PACKER: left in the buffer by Resource_Data0F0+0x1791
