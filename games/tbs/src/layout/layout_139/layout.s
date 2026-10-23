	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map139
Field_Map139:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map139, .LField_Map139_METATILES
	map_block Field_Map139, .LField_Map139_SHAPES
	map_block Field_Map139, .LField_Map139_GRID
	map_block Field_Map139, .LField_Map139_ANIMATION
	map_block Field_Map139, .LField_Map139_BLEND
	map_block Field_Map139, .LField_Map139_MARKERS
.LField_Map139_METATILES:
	.incbin "games/tbs/src/layout/layout_139/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map139_SHAPES:
	.incbin "games/tbs/src/layout/layout_139/shapes.table.lz"
	.balign 4, 0
.LField_Map139_GRID:
	.incbin "games/tbs/src/layout/layout_139/grid.parts.lz"
	.balign 4, 0
.LField_Map139_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/animation.script.lz"
	.balign 4, 0
.LField_Map139_BLEND:
	.incbin "games/tbs/src/layout/layout_139/blend.script.lz"
	.balign 4, 0
.LField_Map139_MARKERS:
	.incbin "games/tbs/src/layout/layout_139/markers.table"
	.balign 4, 0
	.global Tileset_Palette36
Tileset_Palette36:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_36.gbapal.lz"
