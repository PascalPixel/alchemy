	.section .rodata
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "games/tla/src/graphics/tileset/set_36/palette.gbapal.lz"
	.byte 0x1b	@ PACKER: left in the buffer by Field_Map072+0x14f
