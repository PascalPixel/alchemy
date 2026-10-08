	.section .rodata
	.global Tileset_Set26TilesA
Tileset_Set26TilesA:
	.incbin "graphics/tileset/set_26/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2b3b
