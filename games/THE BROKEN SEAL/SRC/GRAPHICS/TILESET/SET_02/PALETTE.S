	.section .rodata
	.global Tileset_Set02Palette
Tileset_Set02Palette:
	.incbin "GRAPHICS/TILESET/SET_02/PALETTE.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_Map003+0x17f
