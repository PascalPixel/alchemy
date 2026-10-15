	.section .rodata
	.global Battle_SnowfieldCBackdrop
Battle_SnowfieldCBackdrop:
	.incbin "games/tla/src/graphics/backdrop/snowfield_c.gbapal"
	.incbin "games/tla/src/graphics/backdrop/snowfield_c.bitmap.d7"
	.byte 0x15, 0x54	@ PACKER: left in the buffer by Battle_IceHallBackdrop+0x439a
