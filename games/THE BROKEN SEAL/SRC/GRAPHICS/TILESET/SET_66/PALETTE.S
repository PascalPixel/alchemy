	.section .rodata
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "GRAPHICS/TILESET/SET_66/PALETTE.gbapal.lz"
	.byte 0x43, 0x11, 0xf8	@ PACKER: left in the buffer by Field_Map169+0xe5
