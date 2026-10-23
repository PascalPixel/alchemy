	.section .rodata
	.global Battle_TretTreeInteriorsBackdrop
Battle_TretTreeInteriorsBackdrop:
	.incbin "games/tbs/src/graphics/battle_bg/tret_tree_interiors.gbapal"
	.incbin "games/tbs/src/graphics/battle_bg/tret_tree_interiors.bitmap.d7"
	.byte 0x58, 0xe7	@ PACKER: left in the buffer by Battle_KolimaForestBackdrop+0x49fa
