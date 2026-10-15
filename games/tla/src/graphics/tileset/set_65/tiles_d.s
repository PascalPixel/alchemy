	.section .rodata
	.global Tileset_Set65TilesD
Tileset_Set65TilesD:
	.incbin "games/tla/src/graphics/tileset/set_65/tiles_d.4bpp.mtf"
	.byte 0x4b	@ PACKER: left in the buffer by Tileset_Set65TilesB+0x162b
