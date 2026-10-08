	.section .rodata
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "GRAPHICS/TILESET/SET_13/PALETTE.gbapal.lz"
	.byte 0x27, 0x50	@ PACKER: left in the buffer by Field_Map015+0x10a
