	.section .rodata
	.global Battle_RedMesaBackdrop
Battle_RedMesaBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/red_mesa.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/red_mesa.bitmap.d7"
	.byte 0xed, 0xc5	@ PACKER: left in the buffer by Battle_BlueCaveBackdrop+0x3a66
