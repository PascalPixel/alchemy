	.section .rodata
	.global Tileset_Set45TilesD
Tileset_Set45TilesD:
	.incbin "GRAPHICS/TILESET/SET_45/TILES_D.4bpp.mtf"
	.byte 0xfb, 0x14, 0x4d	@ PACKER: left in the buffer by Tileset_Set45TilesC+0xb99
