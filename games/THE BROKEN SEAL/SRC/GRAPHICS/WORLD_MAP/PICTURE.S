	.section .rodata
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.gbapal"
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.8bpp.lz"
	/* PACKER: left in the packer's buffer by Title_IntroGraphicsC at +0x3b01 */
	.byte 0x02, 0x31, 0x01
