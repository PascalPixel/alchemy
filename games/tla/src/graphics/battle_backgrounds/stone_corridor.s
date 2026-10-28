	.section .rodata
	.global Battle_StoneCorridorBackdrop
Battle_StoneCorridorBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/stone_corridor.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/stone_corridor.bitmap.d7"
	.ifdef TLA_EDITION_JA
	.byte 0x55, 0x44	@ PACKER: left in the buffer by Battle_PalmCliffBackdrop+0x41fe
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
