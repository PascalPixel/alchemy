	.section .rodata
	.global Tileset_Set108TilesA
Tileset_Set108TilesA:
	.incbin "games/tla/src/graphics/tileset/set_108/tiles_a.4bpp.mtf"
	.byte 0x40, 0xff	@ PACKER: left in the buffer by Field_Map258+0xac2
