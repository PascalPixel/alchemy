	.section .rodata
	.global Tileset_Set111TilesA
Tileset_Set111TilesA:
	.incbin "games/tla/src/graphics/tileset/set_111/tiles_a.4bpp.mtf"
	.byte 0x23, 0x44, 0x28	@ PACKER: left in the buffer by Tileset_Set110TilesA+0x223d
