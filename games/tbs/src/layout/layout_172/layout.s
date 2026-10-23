	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout172
Field_Layout172:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout172, .LField_Layout172_METATILES
	layout_offset Field_Layout172, .LField_Layout172_SHAPES
	layout_offset Field_Layout172, .LField_Layout172_GRID
	layout_offset Field_Layout172
	layout_offset Field_Layout172
	layout_offset Field_Layout172, .LField_Layout172_MARKERS
.LField_Layout172_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout172_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout172_GRID:
	.incbin "games/tbs/src/layout/layout_001/map.grid.lz"
	.balign 4, 0
.LField_Layout172_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
@ Tilesets 68 to 75 belong to the test rooms (Field_Layout172 to Field_Layout179) and
@ are one blank set: fourteen black palettes, two tiles in colour 1 and three
@ empty banks. Their sheets are drawn with that black palette, the only one
@ the game pairs them with, as 1-bit images of its first two colours; the
@ bank with two tiles is drawn two tiles wide, as the layout finder reads it.
	.global Tileset_Set68Palette
Tileset_Set68Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout172+0xb
	.global Tileset_Set68TilesA
Tileset_Set68TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout172+0x14e
	.global Tileset_Set68TilesB
Tileset_Set68TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
	.global Tileset_Set68TilesC
Tileset_Set68TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
	.global Tileset_Set68TilesD
Tileset_Set68TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
