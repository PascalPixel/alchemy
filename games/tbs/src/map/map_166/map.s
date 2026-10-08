	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map166
Field_Map166:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map166, .LField_Map166_METATILES
	map_block Field_Map166, .LField_Map166_SHAPES
	map_block Field_Map166, .LField_Map166_GRID
	map_block Field_Map166, .LField_Map166_ANIMATION
	map_block Field_Map166, .LField_Map166_BLEND
	map_block Field_Map166, .LField_Map166_MARKERS
.LField_Map166_METATILES:
	.incbin "map/map_166/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map166_SHAPES:
	.incbin "map/map_166/shapes.table.lz"
	.balign 4, 0
.LField_Map166_GRID:
	.incbin "map/map_166/grid.parts.lz"
	.balign 4, 0
.LField_Map166_ANIMATION:
	.incbin "map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map166_BLEND:
	.incbin "map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map166_MARKERS:
	.incbin "map/map_166/markers.table"
	.balign 4, 0
