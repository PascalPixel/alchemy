	.section .rodata
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "GRAPHICS/TILESET/SET_23/TILES_B.4bpp.mtf"
	.byte 0xa6, 0x09, 0x9b	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x28c5
