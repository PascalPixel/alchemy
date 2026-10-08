	.section .rodata
	.global Tileset_Set03TilesC
Tileset_Set03TilesC:
	.incbin "GRAPHICS/TILESET/SET_03/TILES_C.4bpp.mtf"
	.byte 0xeb, 0x91	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x26de
