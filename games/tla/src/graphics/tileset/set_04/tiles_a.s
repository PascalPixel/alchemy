	.section .rodata
	.global Tileset_Set04TilesA
Tileset_Set04TilesA:
	.incbin "games/tla/src/graphics/tileset/set_04/tiles_a.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x2afb
