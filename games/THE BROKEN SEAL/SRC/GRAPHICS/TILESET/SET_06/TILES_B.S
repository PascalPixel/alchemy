	.section .rodata
	.global Tileset_Set06TilesB
Tileset_Set06TilesB:
	.incbin "GRAPHICS/TILESET/SET_06/TILES_B.4bpp.mtf"
	.byte 0xe4, 0x05, 0x41	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x1181
