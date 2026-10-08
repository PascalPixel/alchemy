	.section .rodata
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "GRAPHICS/TILESET/SET_14/TILES_B.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x26c2
