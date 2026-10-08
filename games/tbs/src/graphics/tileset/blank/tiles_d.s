	.section .rodata
	.global Tileset_BlankTilesD
Tileset_BlankTilesD:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
