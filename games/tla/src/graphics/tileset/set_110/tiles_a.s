	.section .rodata
	.global Tileset_Set110TilesA
Tileset_Set110TilesA:
	.incbin "games/tla/src/graphics/tileset/set_110/tiles_a.4bpp.mtf"
	.byte 0x04, 0x07, 0x1a	@ PACKER: left in the buffer by Field_Map257+0x23a5
