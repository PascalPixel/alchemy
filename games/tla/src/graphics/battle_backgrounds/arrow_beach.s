	.section .rodata
	.global Battle_ArrowBeachBackdrop
Battle_ArrowBeachBackdrop:
	.incbin "games/tla/src/graphics/battle_backgrounds/arrow_beach.gbapal"
	.incbin "games/tla/src/graphics/battle_backgrounds/arrow_beach.bitmap.d7"
	.byte 0x55, 0x24	@ PACKER: left in the buffer by Battle_LavaTempleBackdrop+0x375a
