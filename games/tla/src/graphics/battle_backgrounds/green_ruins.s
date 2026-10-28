	.section .rodata
	.global Battle_GreenRuinsBackdrop
Battle_GreenRuinsBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/green_ruins.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/green_ruins.bitmap.d7"
	.byte 0x11, 0x10	@ PACKER: left in the buffer by Battle_BlueRuinsBackdrop+0x3dfa
