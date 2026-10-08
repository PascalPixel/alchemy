	.section .rodata
	.global Tileset_Set67TilesB
Tileset_Set67TilesB:
	.incbin "GRAPHICS/TILESET/SET_67/TILES_B.4bpp.mtf"
	.byte 0x58, 0xfe, 0x8a	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xc6d
