	.section .rodata
	.global Tileset_Set118TilesA
Tileset_Set118TilesA:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_a.4bpp.mtf"
	.byte 0x0e, 0x59, 0x80	@ PACKER: left in the buffer by Tileset_Set117TilesD+0x167d
