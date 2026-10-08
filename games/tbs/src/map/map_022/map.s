	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map022
Field_Map022:
	map_header 0, 0, 96, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map022, .LField_Map022_METATILES
	map_block Field_Map022, .LField_Map022_SHAPES
	map_block Field_Map022, .LField_Map022_GRID
	map_block Field_Map022, .LField_Map022_ANIMATION
	map_block Field_Map022, .LField_Map022_BLEND
	map_block Field_Map022, .LField_Map022_MARKERS
.LField_Map022_METATILES:
	.incbin "map/map_022/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map022_SHAPES:
	.incbin "map/map_022/shapes.table.lz"
	.balign 4, 0
.LField_Map022_GRID:
	.incbin "map/map_022/grid.parts.lz"
	.balign 4, 0
.LField_Map022_ANIMATION:
	.incbin "map/map_022/animation.script.lz"
	.balign 4, 0
.LField_Map022_BLEND:
	.incbin "map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map022_MARKERS:
	.incbin "map/map_022/markers.table"
	.balign 4, 0
