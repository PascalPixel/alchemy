	.section .rodata
	.global Tileset_Set65TilesD
Tileset_Set65TilesD:
	.incbin "graphics/tileset/set_65/tiles_d.4bpp.mtf"
	.byte 0x69, 0x4e, 0x50	@ PACKER: left in the buffer by Tileset_Set65TilesC+0x160d
