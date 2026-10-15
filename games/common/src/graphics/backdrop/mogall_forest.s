	.section .rodata
	.global Battle_MogoruMoriBackdrop
Battle_MogoruMoriBackdrop:
	.incbin "games/common/src/graphics/backdrop/mogall_forest.gbapal"
	.incbin "games/common/src/graphics/backdrop/mogall_forest.bitmap.d7"
	.byte 0x50, 0x00	@ PACKER: left in the buffer by Battle_BabiIriguchiBackdrop+0x4326
