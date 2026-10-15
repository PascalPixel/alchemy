	.section .rodata
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_a.4bpp.mtf"
	.byte 0x0c, 0x09	@ PACKER: left in the buffer by Field_Map104+0x2482
