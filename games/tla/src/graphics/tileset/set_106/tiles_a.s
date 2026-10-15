	.section .rodata
	.global Tileset_Set106TilesA
Tileset_Set106TilesA:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map252+0x14fb
