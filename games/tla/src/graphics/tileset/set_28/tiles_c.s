	.section .rodata
	.global Tileset_Set28TilesC
Tileset_Set28TilesC:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_c.4bpp.mtf"
	.byte 0x20, 0xdb	@ PACKER: left in the buffer by Tileset_Set28TilesB+0x1aca
