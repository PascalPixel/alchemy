	.section .rodata
	.global Tileset_Set22TilesA
Tileset_Set22TilesA:
	.incbin "graphics/tileset/set_22/tiles_a.4bpp.mtf"
	.byte 0x04, 0x1c, 0xf2	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x2b3d
