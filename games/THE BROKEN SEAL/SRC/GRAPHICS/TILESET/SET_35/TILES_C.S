	.section .rodata
	.global Tileset_Set35TilesC
Tileset_Set35TilesC:
	.incbin "GRAPHICS/TILESET/SET_35/TILES_C.4bpp.mtf"
	.byte 0x6b	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x286f
