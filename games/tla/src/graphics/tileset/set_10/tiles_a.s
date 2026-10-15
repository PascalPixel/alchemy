	.section .rodata
	.global Tileset_Set10TilesA
Tileset_Set10TilesA:
	.incbin "games/tla/src/graphics/tileset/set_10/tiles_a.4bpp.mtf"
	.byte 0x14	@ PACKER: left in the buffer by Field_Map019+0x1ae3
