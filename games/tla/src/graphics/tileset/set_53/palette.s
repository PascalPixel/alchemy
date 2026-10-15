	.section .rodata
	.global Tileset_Set53Palette
Tileset_Set53Palette:
	.incbin "games/tla/src/graphics/tileset/set_53/palette.gbapal.lz"
	.byte 0xd0, 0xe0	@ PACKER: left in the buffer by Field_Map104+0x166
