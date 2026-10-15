	.section .rodata
	.global Tileset_Set72TilesA
Tileset_Set72TilesA:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_a.4bpp.mtf"
	.byte 0xbd	@ PACKER: left in the buffer by Field_Map149+0x217b
