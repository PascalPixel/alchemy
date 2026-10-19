	.section .rodata
	.global Resource_Data640
Resource_Data640:
	.incbin "games/tla/src/graphics/tileset/blank/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data63F+0x13f
