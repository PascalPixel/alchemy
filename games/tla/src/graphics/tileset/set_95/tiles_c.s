	.section .rodata
	.global Tileset_Set95TilesC
Tileset_Set95TilesC:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0xff, 0xf3	@ PACKER: left in the buffer by Tileset_Set95TilesA+0x1751
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
