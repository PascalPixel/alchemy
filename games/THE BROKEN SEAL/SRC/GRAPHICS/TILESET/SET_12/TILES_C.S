	.section .rodata
	.global Tileset_Set12TilesC
Tileset_Set12TilesC:
	.incbin "GRAPHICS/TILESET/SET_12/TILES_C.4bpp.mtf"
	.byte 0x8a, 0x18, 0x32	@ PACKER: left in the buffer by Tileset_Set12TilesB+0xf3d
