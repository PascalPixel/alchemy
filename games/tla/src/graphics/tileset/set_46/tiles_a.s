	.section .rodata
	.global Tileset_Set46TilesA
Tileset_Set46TilesA:
	.incbin "games/tla/src/graphics/tileset/set_46/tiles_a.4bpp.mtf"
	.byte 0x94, 0xc6	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x276a
