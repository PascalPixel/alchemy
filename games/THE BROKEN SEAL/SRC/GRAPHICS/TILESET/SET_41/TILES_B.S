	.section .rodata
	.global Tileset_Set41TilesB
Tileset_Set41TilesB:
	.incbin "GRAPHICS/TILESET/SET_41/TILES_B.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set41TilesA+0x14b7
