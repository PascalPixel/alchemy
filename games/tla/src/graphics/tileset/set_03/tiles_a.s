	.section .rodata
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_a.4bpp.mtf"
	.byte 0x68, 0x3d, 0x1a	@ PACKER: left in the buffer by Field_Map002+0x2de1
