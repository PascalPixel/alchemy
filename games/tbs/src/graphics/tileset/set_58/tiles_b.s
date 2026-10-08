	.section .rodata
	.global Tileset_Set58TilesB
Tileset_Set58TilesB:
	.incbin "graphics/tileset/set_58/tiles_b.4bpp.mtf"
	.byte 0x08, 0xe8	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x2672
