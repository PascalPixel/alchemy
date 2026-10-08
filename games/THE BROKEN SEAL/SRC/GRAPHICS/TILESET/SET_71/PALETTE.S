	.section .rodata
	.global Tileset_Set71Palette
Tileset_Set71Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map175+0xb
