	.section .rodata
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "games/common/src/graphics/tileset/set_07/tiles_d.4bpp.mtf"
	.byte 0x4e	@ PACKER: left in the buffer by Tileset_Set01TilesC+0x13f
