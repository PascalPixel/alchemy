	.section .rodata
	.global Resource_Data61C
Resource_Data61C:
	.incbin "games/common/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Resource_Data61B+0x13f
