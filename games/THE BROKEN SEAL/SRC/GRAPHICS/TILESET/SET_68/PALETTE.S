@ Tile sets 68 to 75 belong to the test rooms (Field_Map172 to Field_Map179) and
@ are one blank set: fourteen black palettes, two tiles in colour 1 and three
@ empty banks. Their sheets are drawn with that black palette, the only one
@ the game pairs them with, as 1-bit images of its first two colours; the
@ bank with two tiles is drawn two tiles wide, as the layout finder reads it.
	.section .rodata
	.global Tileset_Set68Palette
Tileset_Set68Palette:
	.incbin "GRAPHICS/TILESET/SET_68/PALETTE.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map172+0xb
