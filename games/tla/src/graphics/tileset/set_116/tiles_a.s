	.section .rodata
	.global Tileset_Set116TilesA
Tileset_Set116TilesA:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_a.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Field_Map252+0x2fbf
