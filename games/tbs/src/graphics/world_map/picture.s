	.section .rodata
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "graphics/world_map/picture.gbapal"
	.incbin "graphics/world_map/picture.8bpp.lz"
	.byte 0x02, 0x31, 0x01	@ PACKER: left in the buffer by Title_IntroGraphicsC+0x3b01
