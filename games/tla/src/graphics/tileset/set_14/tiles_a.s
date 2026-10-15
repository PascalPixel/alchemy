	.section .rodata
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x81, 0x45	@ PACKER: left in the buffer by Tileset_Set12TilesA+0x259a
