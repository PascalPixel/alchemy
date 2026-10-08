	.section .rodata
	.global Tileset_Set04TilesC
Tileset_Set04TilesC:
	.incbin "graphics/tileset/set_04/tiles_c.4bpp.mtf"
	.byte 0x86, 0xb4	@ PACKER: left in the buffer by Tileset_Set04TilesB+0x15fa
