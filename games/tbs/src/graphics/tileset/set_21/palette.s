	.section .rodata
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "graphics/tileset/set_21/palette.gbapal.lz"
	.byte 0x98, 0x49	@ PACKER: left in the buffer by Field_Map028+0x19a
