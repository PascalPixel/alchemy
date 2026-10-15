	.section .rodata
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "games/tla/src/graphics/tileset/set_74/tiles_a.4bpp.mtf"
	.byte 0x05, 0x80, 0x00	@ PACKER: left in the buffer by Field_Map161+0x1701
