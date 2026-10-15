	.section .rodata
	.global Tileset_Set78Palette
Tileset_Set78Palette:
	.incbin "games/tla/src/graphics/tileset/set_78/palette.gbapal.lz"
	.byte 0x5c, 0x13	@ PACKER: left in the buffer by Field_Map168+0x11e
