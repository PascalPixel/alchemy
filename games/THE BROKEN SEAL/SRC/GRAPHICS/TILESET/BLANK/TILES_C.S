	.section .rodata
	.global Tileset_BlankTilesC
Tileset_BlankTilesC:
	.incbin "GRAPHICS/TILESET/SET_68/TILES_B.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
