	.section .rodata
	.global Battle_AltmillerCaveBackdrop
Battle_AltmillerCaveBackdrop:
	.incbin "games/common/src/graphics/backdrop/altmiller_cave.gbapal"
	.incbin "games/common/src/graphics/backdrop/altmiller_cave.bitmap.d7"
	.byte 0x41, 0x40	@ PACKER: left in the buffer by Battle_ColossoBackdrop+0x40d6
