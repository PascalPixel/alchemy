	.section .rodata
	.global Tileset_Set43TilesA
Tileset_Set43TilesA:
	.incbin "games/tla/src/graphics/tileset/set_43/tiles_a.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_Map090+0x26af
