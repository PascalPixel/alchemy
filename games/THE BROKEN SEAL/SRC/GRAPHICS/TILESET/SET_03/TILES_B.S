	.section .rodata
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "GRAPHICS/TILESET/SET_03/TILES_B.4bpp.mtf"
	.byte 0xcb, 0x42, 0xda	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x1e15
