	.section .rodata
	.global Tileset_Set59TilesC
Tileset_Set59TilesC:
	.incbin "GRAPHICS/TILESET/SET_59/TILES_C.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x4bb
