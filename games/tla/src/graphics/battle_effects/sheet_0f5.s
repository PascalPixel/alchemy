	.section .rodata
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f5_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f5.parts.lz"
	.byte 0xe4, 0x8a, 0x13	@ PACKER: left in the buffer by Resource_Data0F3+0x11d5
