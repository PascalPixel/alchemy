	.section .rodata
	.global Tileset_Set106TilesB
Tileset_Set106TilesB:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_b.4bpp.mtf"
	.byte 0xb5	@ PACKER: left in the buffer by Field_Map252+0x1bc7
