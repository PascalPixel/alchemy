	.section .rodata
	.global Tileset_Set49TilesA
Tileset_Set49TilesA:
	.incbin "games/tla/src/graphics/tileset/set_49/tiles_a.4bpp.mtf"
	.byte 0x96, 0x80	@ PACKER: left in the buffer by Field_Map099+0x2756
