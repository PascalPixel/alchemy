	.section .rodata
	.global Tileset_Set93TilesA
Tileset_Set93TilesA:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_a.4bpp.mtf"
	.byte 0x20, 0xce	@ PACKER: left in the buffer by Field_Map219+0x281e
