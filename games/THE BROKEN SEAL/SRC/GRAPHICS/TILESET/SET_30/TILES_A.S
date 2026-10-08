	.section .rodata
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "GRAPHICS/TILESET/SET_30/TILES_A.4bpp.mtf"
	.byte 0x21, 0x2e	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x1902
