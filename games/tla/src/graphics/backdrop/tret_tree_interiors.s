	.section .rodata
	.global Battle_TretTreeInteriorsBackdrop
Battle_TretTreeInteriorsBackdrop:
	.incbin "games/tla/src/graphics/backdrop/tret_tree_interiors.gbapal"
	.incbin "games/tla/src/graphics/backdrop/tret_tree_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KolimaForestBackdrop+0x49fa
