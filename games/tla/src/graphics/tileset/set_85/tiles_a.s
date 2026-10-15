	.section .rodata
	.global Tileset_Set85TilesA
Tileset_Set85TilesA:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_a.4bpp.mtf"
	.byte 0xd5, 0x83	@ PACKER: left in the buffer by Tileset_Set84TilesA+0x1b0a
