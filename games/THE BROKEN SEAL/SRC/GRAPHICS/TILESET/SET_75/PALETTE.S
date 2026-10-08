	.section .rodata
	.global Tileset_Set75Palette
Tileset_Set75Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map179+0xb
