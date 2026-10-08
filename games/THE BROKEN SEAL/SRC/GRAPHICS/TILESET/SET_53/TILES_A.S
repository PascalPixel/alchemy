	.section .rodata
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "GRAPHICS/TILESET/SET_53/TILES_A.4bpp.mtf"
	.byte 0x09, 0x02	@ PACKER: left in the buffer by Field_Map098+0x1a82
