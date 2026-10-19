	.section .rodata
	.global Battle_TretTreeBranchBackdrop
Battle_TretTreeBranchBackdrop:
	.incbin "games/tla/src/graphics/backdrop/tret_tree_branch.gbapal"
	.incbin "games/tla/src/graphics/backdrop/tret_tree_branch.bitmap.d7"
	.byte 0xfa, 0x8e	@ PACKER: left in the buffer by Battle_ValeStormBackdrop+0x42b2
