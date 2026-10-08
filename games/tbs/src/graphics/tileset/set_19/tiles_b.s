	.section .rodata
	.global Tileset_Set19TilesB
Tileset_Set19TilesB:
	.incbin "graphics/tileset/set_19/tiles_b.4bpp.mtf"
	.byte 0x27, 0x88	@ PACKER: left in the buffer by Tileset_Set19TilesA+0x20fa
