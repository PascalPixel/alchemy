	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map167
Field_Map167:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map167, .LField_Map167_METATILES
	map_block Field_Map167, .LField_Map167_SHAPES
	map_block Field_Map167, .LField_Map167_GRID
	map_block Field_Map167, .LField_Map167_ANIMATION
	map_block Field_Map167, .LField_Map167_BLEND
	map_block Field_Map167, .LField_Map167_MARKERS
.LField_Map167_METATILES:
	.incbin "map/map_167/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map167_SHAPES:
	.incbin "map/map_167/shapes.table.lz"
	.balign 4, 0
.LField_Map167_GRID:
	.incbin "map/map_167/grid.parts.lz"
	.balign 4, 0
.LField_Map167_ANIMATION:
	.incbin "map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map167_BLEND:
	.incbin "map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map167_MARKERS:
	.incbin "map/map_167/markers.table"
	.balign 4, 0
