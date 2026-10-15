	.section .rodata
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "games/tla/src/graphics/tileset/set_16/palette.gbapal.lz"
	.byte 0x98	@ PACKER: left in the buffer by Field_Map033+0x14f
