	.section .rodata
	.global Battle_BeachBackdrop
Battle_BeachBackdrop:
	.incbin "games/tla/src/graphics/backdrop/beach.gbapal"
	.incbin "games/tla/src/graphics/backdrop/beach.bitmap.d7"
	.byte 0x10, 0xb5	@ PACKER: left in the buffer by Battle_TretTreeBranchBackdrop+0x397e
