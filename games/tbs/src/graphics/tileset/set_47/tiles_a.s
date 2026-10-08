	.section .rodata
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "graphics/tileset/set_47/tiles_a.4bpp.mtf"
	.byte 0x01, 0x6d	@ PACKER: left in the buffer by Field_Map073+0x150e
