	.section .rodata
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "graphics/tileset/set_53/tiles_a.4bpp.mtf"
	.byte 0x09, 0x02	@ PACKER: left in the buffer by Field_Map098+0x1a82
