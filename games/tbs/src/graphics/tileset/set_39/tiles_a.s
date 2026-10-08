	.section .rodata
	.global Tileset_Set39TilesA
Tileset_Set39TilesA:
	.incbin "graphics/tileset/set_39/tiles_a.4bpp.mtf"
	.byte 0x71	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x22b7
