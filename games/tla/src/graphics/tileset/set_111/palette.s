	.section .rodata
	.global Tileset_Set111Palette
Tileset_Set111Palette:
	.incbin "games/tla/src/graphics/tileset/set_111/palette.gbapal.lz"
	.byte 0x05, 0x18	@ PACKER: left in the buffer by Field_Map265+0x1b2
