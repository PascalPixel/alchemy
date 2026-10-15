	.section .rodata
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "games/tla/src/graphics/tileset/set_35/palette.gbapal.lz"
	.byte 0x87, 0x37, 0x18	@ PACKER: left in the buffer by Field_Map060+0x12d
