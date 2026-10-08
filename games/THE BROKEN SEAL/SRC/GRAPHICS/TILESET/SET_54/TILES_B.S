	.section .rodata
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "GRAPHICS/TILESET/SET_54/TILES_B.4bpp.mtf"
	.byte 0x83	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x23e3
