	.section .rodata
	.global Ending_ReunionPicture
Ending_ReunionPicture:
	.incbin "games/tla/src/graphics/ending/reunion.gbapal"
	.incbin "games/tla/src/graphics/ending/reunion.bitmap.d7"
	.byte 0x37, 0x16	@ PACKER: left in the buffer by Ending_VillagersPictureB+0x7842
