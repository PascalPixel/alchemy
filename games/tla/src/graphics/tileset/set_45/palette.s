	.section .rodata
	.global Tileset_Set45Palette
Tileset_Set45Palette:
	.incbin "games/tla/src/graphics/tileset/set_45/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map093+0x19b
