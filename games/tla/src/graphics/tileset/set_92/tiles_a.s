	.section .rodata
	.global Tileset_Set92TilesA
Tileset_Set92TilesA:
	.incbin "games/tla/src/graphics/tileset/set_92/tiles_a.4bpp.mtf"
	.byte 0x40, 0x00	@ PACKER: left in the buffer by Field_Map218+0x18ba
