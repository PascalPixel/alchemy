	.section .rodata
	.global Tileset_Set115TilesB
Tileset_Set115TilesB:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_b.4bpp.mtf"
	.byte 0x0f	@ PACKER: left in the buffer by Tileset_Set115TilesA+0x172b
