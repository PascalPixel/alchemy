	.section .rodata
	.global Tileset_Set33TilesA
Tileset_Set33TilesA:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_a.4bpp.mtf"
	.byte 0x40, 0x4e, 0x01	@ PACKER: left in the buffer by Field_Map052+0x20ed
