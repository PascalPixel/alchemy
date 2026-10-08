	.section .rodata
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "graphics/tileset/set_14/tiles_b.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x26c2
