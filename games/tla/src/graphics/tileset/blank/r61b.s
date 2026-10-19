	.section .rodata
	.global Resource_Data61B
Resource_Data61B:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_a.4bpp.mtf"
	.byte 0x01, 0xff	@ PACKER: left in the buffer by Field_Map292+0x14e
