	.section .rodata
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "GRAPHICS/TILESET/SET_21/PALETTE.gbapal.lz"
	.byte 0x98, 0x49	@ PACKER: left in the buffer by Field_Map028+0x19a
