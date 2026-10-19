	.section .rodata
	.global Resource_Data61D
Resource_Data61D:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data61C+0x13f
