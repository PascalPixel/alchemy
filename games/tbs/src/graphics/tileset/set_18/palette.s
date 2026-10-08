	.section .rodata
	.global Tileset_Set18Palette
Tileset_Set18Palette:
	.incbin "graphics/tileset/set_18/palette.gbapal.lz"
	.byte 0x71	@ PACKER: left in the buffer by Field_Map021+0xcb
