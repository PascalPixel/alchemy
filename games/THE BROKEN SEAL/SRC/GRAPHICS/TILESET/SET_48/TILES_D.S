	.section .rodata
	.global Tileset_Set48TilesD
Tileset_Set48TilesD:
	.incbin "GRAPHICS/TILESET/SET_48/TILES_D.4bpp.mtf"
	.byte 0xd0	@ PACKER: left in the buffer by Tileset_Set48TilesC+0x130b
