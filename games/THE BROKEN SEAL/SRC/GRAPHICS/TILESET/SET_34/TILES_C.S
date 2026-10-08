	.section .rodata
	.global Tileset_Set34TilesC
Tileset_Set34TilesC:
	.incbin "GRAPHICS/TILESET/SET_34/TILES_C.4bpp.mtf"
	.byte 0xd6, 0xb6, 0x08	@ PACKER: left in the buffer by Tileset_Set34TilesB+0x17c1
