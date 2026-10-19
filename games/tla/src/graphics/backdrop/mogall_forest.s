	.section .rodata
	.global Battle_MogallForestBackdrop
Battle_MogallForestBackdrop:
	.incbin "games/tla/src/graphics/backdrop/mogall_forest.gbapal"
	.incbin "games/tla/src/graphics/backdrop/mogall_forest.bitmap.d7"
	.byte 0x50, 0x00	@ PACKER: left in the buffer by Battle_BabiLighthouseBackdrop+0x4326
