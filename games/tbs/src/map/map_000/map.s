	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0, 0, 60, 188, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map000, .LField_Map000_METATILES
	map_block Field_Map000, .LField_Map000_SHAPES
	map_block Field_Map000, .LField_Map000_GRID
	map_block Field_Map000, .LField_Map000_ANIMATION
	map_block Field_Map000
	map_block Field_Map000, .LField_Map000_MARKERS
.LField_Map000_METATILES:
	.incbin "map/map_000/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map000_SHAPES:
	.incbin "map/map_000/shapes.table.lz"
	.balign 4, 0
.LField_Map000_GRID:
	.incbin "map/map_000/grid.parts.lz"
	.balign 4, 0
.LField_Map000_ANIMATION:
	.incbin "map/map_000/animation.script.lz"
	.balign 4, 0
.LField_Map000_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
