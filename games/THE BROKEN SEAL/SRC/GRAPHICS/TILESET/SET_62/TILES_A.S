	.section .rodata
	.global Tileset_Set62TilesA
Tileset_Set62TilesA:
	.incbin "GRAPHICS/TILESET/SET_62/TILES_A.4bpp.mtf"
	.byte 0x81, 0xdf	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x2732
