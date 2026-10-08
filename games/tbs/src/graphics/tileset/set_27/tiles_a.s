	.section .rodata
	.global Tileset_Set27TilesA
Tileset_Set27TilesA:
	.incbin "graphics/tileset/set_27/tiles_a.4bpp.mtf"
	.byte 0x60, 0xcf, 0x83	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2d91
