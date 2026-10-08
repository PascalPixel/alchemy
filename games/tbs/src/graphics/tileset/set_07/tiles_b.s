	.section .rodata
	.global Tileset_Set07TilesB
Tileset_Set07TilesB:
	.incbin "common/graphics/tileset/set_07/tiles_b.4bpp.mtf"
	.byte 0xce, 0x43, 0x48	@ PACKER: left in the buffer by Field_Map008+0x1c85
