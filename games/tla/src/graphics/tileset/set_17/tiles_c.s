	.section .rodata
	.global Tileset_Set17TilesC
Tileset_Set17TilesC:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Field_Map034+0x217f
