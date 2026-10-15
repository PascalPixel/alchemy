	.section .rodata
	.global Tileset_Set39TilesB
Tileset_Set39TilesB:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_b.4bpp.mtf"
	.byte 0x48	@ PACKER: left in the buffer by Tileset_Set39TilesA+0x269f
