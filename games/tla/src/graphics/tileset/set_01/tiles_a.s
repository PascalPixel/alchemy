	.section .rodata
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "games/common/src/graphics/tileset/set_07/tiles_a.4bpp.mtf"
	.byte 0x49, 0x8e, 0xe3	@ PACKER: left in the buffer by Field_Map002+0x11e9
