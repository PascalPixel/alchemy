	.section .rodata
	.global Tileset_Set116TilesD
Tileset_Set116TilesD:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_d.4bpp.mtf"
	.byte 0x81, 0x03	@ PACKER: left in the buffer by Tileset_Set116TilesA+0x1ac2
