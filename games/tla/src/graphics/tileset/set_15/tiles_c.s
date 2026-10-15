	.section .rodata
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map032+0x217f
