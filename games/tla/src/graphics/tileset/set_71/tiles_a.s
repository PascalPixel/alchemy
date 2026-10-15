	.section .rodata
	.global Tileset_Set71TilesA
Tileset_Set71TilesA:
	.incbin "games/tla/src/graphics/tileset/set_71/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x92, 0x40	@ PACKER: left in the buffer by Field_Map148+0x26dd
