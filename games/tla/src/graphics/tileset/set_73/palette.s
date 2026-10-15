	.section .rodata
	.global Tileset_Set73Palette
Tileset_Set73Palette:
	.incbin "games/tla/src/graphics/tileset/set_73/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map160+0x18f
