	.section .rodata
	.global Tileset_Set71Palette
Tileset_Set71Palette:
	.incbin "games/tla/src/graphics/tileset/set_71/palette.gbapal.lz"
	.byte 0x55, 0x71, 0xd4	@ PACKER: left in the buffer by Field_Map148+0x161
