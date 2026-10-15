	.section .rodata
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "games/tla/src/graphics/tileset/set_25/tiles_a.4bpp.mtf"
	.byte 0xce	@ PACKER: left in the buffer by Field_Map041+0x212f
