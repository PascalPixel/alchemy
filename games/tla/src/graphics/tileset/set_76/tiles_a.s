	.section .rodata
	.global Tileset_Set76TilesA
Tileset_Set76TilesA:
	.incbin "games/tla/src/graphics/tileset/set_76/tiles_a.4bpp.mtf"
	.byte 0x04, 0xc3, 0x4b	@ PACKER: left in the buffer by Field_Map163+0x4d1
