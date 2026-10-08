	.section .rodata
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "graphics/tileset/set_03/tiles_a.4bpp.mtf"
	.byte 0x1e	@ PACKER: left in the buffer by Field_Map003+0x283f
