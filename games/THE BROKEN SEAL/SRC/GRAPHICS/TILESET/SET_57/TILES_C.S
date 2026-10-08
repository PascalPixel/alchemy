	.section .rodata
	.global Tileset_Set57TilesC
Tileset_Set57TilesC:
	.incbin "GRAPHICS/TILESET/SET_57/TILES_C.4bpp.mtf"
	.byte 0xd6, 0x83, 0x39	@ PACKER: left in the buffer by Tileset_Set57TilesB+0x1271
