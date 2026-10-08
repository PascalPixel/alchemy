	.section .rodata
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "graphics/tileset/set_30/tiles_a.4bpp.mtf"
	.byte 0x21, 0x2e	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x1902
