	.section .rodata
	.global Tileset_Set80TilesD
Tileset_Set80TilesD:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_d.4bpp.mtf"
	.byte 0x24, 0x40	@ PACKER: left in the buffer by Field_Map179+0x1f46
