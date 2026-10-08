	.section .rodata
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "GRAPHICS/TILESET/SET_05/PALETTE.gbapal.lz"
	.byte 0x64, 0x36, 0x80	@ PACKER: left in the buffer by Field_Map006+0x18d
