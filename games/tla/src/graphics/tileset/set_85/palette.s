	.section .rodata
	.global Tileset_Set85Palette
Tileset_Set85Palette:
	.incbin "games/tla/src/graphics/tileset/set_85/palette.gbapal.lz"
	.byte 0x18, 0x61, 0xb5	@ PACKER: left in the buffer by Field_Map193+0x1a9
