	.section .rodata
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_a.4bpp.mtf"
	.byte 0x61	@ PACKER: left in the buffer by Field_Map049+0x1a2b
