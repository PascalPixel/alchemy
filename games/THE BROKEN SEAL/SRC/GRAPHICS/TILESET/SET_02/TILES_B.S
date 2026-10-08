	.section .rodata
	.global Tileset_Set02TilesB
Tileset_Set02TilesB:
	.incbin "GRAPHICS/TILESET/SET_02/TILES_B.4bpp.mtf"
	.byte 0xdc, 0x73	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x2646
