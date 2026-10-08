	.section .rodata
	.global Tileset_Set22TilesB
Tileset_Set22TilesB:
	.incbin "GRAPHICS/TILESET/SET_22/TILES_B.4bpp.mtf"
	.byte 0xf9	@ PACKER: left in the buffer by Tileset_Set22TilesA+0x1dcf
