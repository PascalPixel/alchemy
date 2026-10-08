	.section .rodata
	.global Tileset_Set66TilesD
Tileset_Set66TilesD:
	.incbin "GRAPHICS/TILESET/SET_66/TILES_D.4bpp.mtf"
	.byte 0x04, 0x5c	@ PACKER: left in the buffer by Tileset_Set66TilesC+0x5b2
