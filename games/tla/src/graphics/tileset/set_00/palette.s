	.section .rodata
	.global Tileset_Set00Palette
Tileset_Set00Palette:
	.incbin "games/tla/src/graphics/tileset/set_00/palette.gbapal.lz"
	.byte 0x85, 0x50	@ PACKER: left in the buffer by Field_Map001+0x1ce
