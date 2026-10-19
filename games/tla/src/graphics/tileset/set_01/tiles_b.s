	.section .rodata
	.global Tileset_Set01TilesB
Tileset_Set01TilesB:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_b.4bpp.mtf"
	.byte 0x5b, 0xac, 0x10	@ PACKER: left in the buffer by Field_Map002+0x1c85
