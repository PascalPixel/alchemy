	.section .rodata
	.global Tileset_Set68TilesD
Tileset_Set68TilesD:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_Set68TilesC+0x195f
