	.section .rodata
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "GRAPHICS/TILESET/SET_14/TILES_C.4bpp.mtf"
	.byte 0x0a, 0xcd	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x2756
