	.section .rodata
	.global Tileset_Set73Palette
Tileset_Set73Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map177+0xb
