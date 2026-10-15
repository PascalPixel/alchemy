	.section .rodata
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_a.4bpp.mtf"
	.byte 0x14, 0xf0	@ PACKER: left in the buffer by Field_Map002+0x2866
