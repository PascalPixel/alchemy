	.section .rodata
	.global Tileset_Set45TilesC
Tileset_Set45TilesC:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_c.4bpp.mtf"
	.byte 0x4a	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x10fb
