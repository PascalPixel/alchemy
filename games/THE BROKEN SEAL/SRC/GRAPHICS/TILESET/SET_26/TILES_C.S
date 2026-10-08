	.section .rodata
	.global Tileset_Set26TilesC
Tileset_Set26TilesC:
	.incbin "GRAPHICS/TILESET/SET_26/TILES_C.4bpp.mtf"
	.byte 0xc9, 0xe1	@ PACKER: left in the buffer by Tileset_Set26TilesB+0xfea
