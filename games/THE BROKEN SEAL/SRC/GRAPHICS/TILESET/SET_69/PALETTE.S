	.section .rodata
	.global Tileset_Set69Palette
Tileset_Set69Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map173+0xb
