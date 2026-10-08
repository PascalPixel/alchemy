	.section .rodata
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "GRAPHICS/TILESET/SET_16/TILES_D.4bpp.mtf"
	.byte 0x82, 0xb0	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xda2
