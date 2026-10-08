	.section .rodata
	.global Tileset_Set63TilesA
Tileset_Set63TilesA:
	.incbin "graphics/tileset/set_63/tiles_a.4bpp.mtf"
	.byte 0x02, 0x04, 0x30	@ PACKER: left in the buffer by Field_Map143+0x29c9
