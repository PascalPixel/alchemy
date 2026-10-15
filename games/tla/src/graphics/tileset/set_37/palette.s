	.section .rodata
	.global Tileset_Set37Palette
Tileset_Set37Palette:
	.incbin "games/tla/src/graphics/tileset/set_37/palette.gbapal.lz"
	.byte 0x68	@ PACKER: left in the buffer by Field_Map073+0x14f
