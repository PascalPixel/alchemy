	.section .rodata
	.global Tileset_Set30TilesC
Tileset_Set30TilesC:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_c.4bpp.mtf"
	.byte 0x9d	@ PACKER: left in the buffer by Tileset_Set30TilesB+0x11af
