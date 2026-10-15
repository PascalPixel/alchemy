	.section .rodata
	.global Tileset_Set83TilesB
Tileset_Set83TilesB:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_b.4bpp.mtf"
	.byte 0x75, 0xea	@ PACKER: left in the buffer by Tileset_Set83TilesA+0x18ce
