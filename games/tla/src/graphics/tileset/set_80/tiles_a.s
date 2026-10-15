	.section .rodata
	.global Tileset_Set80TilesA
Tileset_Set80TilesA:
	.incbin "games/tla/src/graphics/tileset/set_80/tiles_a.4bpp.mtf"
	.byte 0x24, 0x03, 0x8f	@ PACKER: left in the buffer by Field_Map179+0x10dd
