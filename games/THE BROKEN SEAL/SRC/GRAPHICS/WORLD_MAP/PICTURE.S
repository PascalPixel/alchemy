	.section .rodata
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.gbapal"
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.8bpp.lz"
	.byte 0x02, 0x31, 0x01	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x3b01
