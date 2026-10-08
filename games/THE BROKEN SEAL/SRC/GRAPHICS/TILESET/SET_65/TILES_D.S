	.section .rodata
	.global Tileset_Set65TilesD
Tileset_Set65TilesD:
	.incbin "GRAPHICS/TILESET/SET_65/TILES_D.4bpp.mtf"
	.byte 0x69, 0x4e, 0x50	@ PACKER: left in the buffer by Tileset_Set65TilesC+0x160d
