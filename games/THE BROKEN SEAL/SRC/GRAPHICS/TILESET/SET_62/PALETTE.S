	.section .rodata
	.global Tileset_Set62Palette
Tileset_Set62Palette:
	.incbin "GRAPHICS/TILESET/SET_62/PALETTE.gbapal.lz"
	.byte 0x22, 0xb6	@ PACKER: left in the buffer by Field_Map129+0x13a
