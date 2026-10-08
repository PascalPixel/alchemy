	.section .rodata
	.global Tileset_Set45TilesC
Tileset_Set45TilesC:
	.incbin "graphics/tileset/set_45/tiles_c.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set45TilesB+0x1573
