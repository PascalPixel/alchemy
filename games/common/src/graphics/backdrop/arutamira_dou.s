	.section .rodata
	.global Battle_ArutamiraDouBackdrop
Battle_ArutamiraDouBackdrop:
	.incbin "games/common/src/graphics/backdrop/arutamira_dou.gbapal"
	.incbin "games/common/src/graphics/backdrop/arutamira_dou.bitmap.d7"
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_KorosseoBackdrop+0x40d6
