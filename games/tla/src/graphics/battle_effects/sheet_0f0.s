	.section .rodata
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f0_00.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/sheet_0f0.parts.lz"
	.byte 0x40, 0x83, 0x0f	@ PACKER: left in the buffer by Resource_Data0DC+0x22b9
