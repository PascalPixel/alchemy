@ The whirlwind's 52 tiles of 16 colours: 26 columns of 8x16, the top halves
@ in the first row and the bottom halves in the second, as they sit at
@ background tiles 0x140 and 0x160. Overlays 663 and 696 unpack them, copy
@ each row to its place and draw them with Field_WhirlwindMap less 0xc0.
@ Overlay 696 shows every tile in background palette 1, and the picture
@ carries that row of its first map's tileset palette (Tileset_Set100Palette).
	.section .rodata
	.global Field_WhirlwindTiles
Field_WhirlwindTiles:
	.incbin "games/tla/src/graphics/field/whirlwind_tiles.4bpp.lz"
	.byte 0x07, 0x12, 0x36	@ PACKER: left in the buffer by WorldMap_TilesD6+0x2f5
