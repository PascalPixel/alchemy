	.section .rodata
	.global Tileset_Set26TilesA
Tileset_Set26TilesA:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_a.4bpp.mtf"
	.byte 0xdf	@ PACKER: left in the buffer by Tileset_Set25TilesA+0x1f5b
