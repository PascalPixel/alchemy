	.section .rodata
	.global Tileset_Set59Palette
Tileset_Set59Palette:
	.incbin "GRAPHICS/TILESET/SET_59/PALETTE.gbapal.lz"
	.byte 0x8e, 0x89	@ PACKER: left in the buffer by Field_Map124+0x166
