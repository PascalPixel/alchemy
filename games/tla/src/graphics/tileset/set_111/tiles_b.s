	.section .rodata
	.global Tileset_Set111TilesB
Tileset_Set111TilesB:
	.incbin "games/tla/src/graphics/tileset/set_111/tiles_b.4bpp.mtf"
	.byte 0x8e	@ PACKER: left in the buffer by Tileset_Set111TilesA+0xb23
