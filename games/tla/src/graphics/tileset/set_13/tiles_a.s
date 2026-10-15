	.section .rodata
	.global Tileset_Set13TilesA
Tileset_Set13TilesA:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_a.4bpp.mtf"
	.byte 0x86	@ PACKER: left in the buffer by Tileset_Set12TilesA+0x239f
