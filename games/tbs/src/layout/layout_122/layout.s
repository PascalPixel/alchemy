	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout122
Field_Layout122:
	layout_header 0, 0, 48, 56, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 48, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout122, .LField_Layout122_METATILES
	layout_offset Field_Layout122, .LField_Layout122_SHAPES
	layout_offset Field_Layout122, .LField_Layout122_GRID
	layout_offset Field_Layout122
	layout_offset Field_Layout122
	layout_offset Field_Layout122, .LField_Layout122_MARKERS
.LField_Layout122_METATILES:
	.incbin "games/tbs/src/layout/layout_122/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout122_SHAPES:
	.incbin "games/tbs/src/layout/layout_122/shapes.table.lz"
	.balign 4, 0
.LField_Layout122_GRID:
	.incbin "games/tbs/src/layout/layout_122/map.grid.lz"
	.balign 4, 0
.LField_Layout122_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_27.gbapal.lz"
	.byte 0x86, 0xf1	@ PACKER: left in the buffer by Field_Layout122+0x18e
