	.section .rodata
	.global Tileset_Set61Palette
Tileset_Set61Palette:
	.incbin "games/tla/src/graphics/tileset/set_61/palette.gbapal.lz"
	.byte 0x11, 0x98, 0x70	@ PACKER: left in the buffer by Field_Map126+0x171
