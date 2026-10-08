	.section .rodata
	.global Tileset_Set19TilesA
Tileset_Set19TilesA:
	.incbin "graphics/tileset/set_19/tiles_a.4bpp.mtf"
	.byte 0x12, 0x00	@ PACKER: left in the buffer by Tileset_Set17TilesA+0x2c16
