	.section .rodata
	.global Tileset_Set67TilesD
Tileset_Set67TilesD:
	.incbin "graphics/tileset/set_67/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x02	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xe82
