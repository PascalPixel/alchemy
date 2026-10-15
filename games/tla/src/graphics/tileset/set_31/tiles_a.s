	.section .rodata
	.global Tileset_Set31TilesA
Tileset_Set31TilesA:
	.incbin "games/tla/src/graphics/tileset/set_31/tiles_a.4bpp.mtf"
	.byte 0x08, 0x80	@ PACKER: left in the buffer by Tileset_Set30TilesB+0x21de
