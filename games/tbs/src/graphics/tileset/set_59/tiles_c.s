	.section .rodata
	.global Tileset_Set59TilesC
Tileset_Set59TilesC:
	.incbin "graphics/tileset/set_59/tiles_c.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x4bb
