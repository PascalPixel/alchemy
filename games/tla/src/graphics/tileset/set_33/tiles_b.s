	.section .rodata
	.global Tileset_Set33TilesB
Tileset_Set33TilesB:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_b.4bpp.mtf"
	.byte 0x43, 0x23	@ PACKER: left in the buffer by Tileset_Set33TilesA+0x1a1a
