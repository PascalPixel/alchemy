	.section .rodata
	.global Tileset_Set73Palette
Tileset_Set73Palette:
	.incbin "graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map177+0xb
