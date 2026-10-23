	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map122
Field_Map122:
	map_header 0, 0, 48, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 48, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map122, .LField_Map122_METATILES
	map_block Field_Map122, .LField_Map122_SHAPES
	map_block Field_Map122, .LField_Map122_GRID
	map_block Field_Map122
	map_block Field_Map122
	map_block Field_Map122, .LField_Map122_MARKERS
.LField_Map122_METATILES:
	.incbin "games/tbs/src/layout/layout_122/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map122_SHAPES:
	.incbin "games/tbs/src/layout/layout_122/shapes.table.lz"
	.balign 4, 0
.LField_Map122_GRID:
	.incbin "games/tbs/src/layout/layout_122/grid.parts.lz"
	.balign 4, 0
.LField_Map122_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette27
Tileset_Palette27:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_27.gbapal.lz"
	.byte 0x86, 0xf1	@ PACKER: left in the buffer by Field_Map122+0x18e
