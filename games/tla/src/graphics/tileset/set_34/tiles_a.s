	.section .rodata
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_a.4bpp.mtf"
	.byte 0x8c, 0xeb	@ PACKER: left in the buffer by Field_Map053+0x11f6
