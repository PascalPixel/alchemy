	.section .rodata
	.global Battle_TretBranchBackdrop
Battle_TretBranchBackdrop:
	.incbin "games/common/src/graphics/backdrop/tret_branch.gbapal"
	.incbin "games/common/src/graphics/backdrop/tret_branch.bitmap.d7"
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_ValeStormBackdrop+0x42b2
