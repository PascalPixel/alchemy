	.section .rodata
	.global Tileset_Set61TilesB
Tileset_Set61TilesB:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_b.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x28cf
