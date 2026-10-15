	.section .rodata
	.global Tileset_Set28TilesA
Tileset_Set28TilesA:
	.incbin "games/tla/src/graphics/tileset/set_28/tiles_a.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Field_Map041+0x25af
