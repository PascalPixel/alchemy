	.section .rodata
	.global Tileset_Set85TilesC
Tileset_Set85TilesC:
	.incbin "games/tla/src/graphics/tileset/set_85/tiles_c.4bpp.mtf"
	.byte 0x29	@ PACKER: left in the buffer by Tileset_Set85TilesA+0xe77
