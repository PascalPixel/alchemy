	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map056
Field_Map056:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map056, .LField_Map056_METATILES
	map_block Field_Map056, .LField_Map056_SHAPES
	map_block Field_Map056, .LField_Map056_GRID
	map_block Field_Map056, .LField_Map056_ANIMATION
	map_block Field_Map056, .LField_Map056_BLEND
	map_block Field_Map056, .LField_Map056_MARKERS
.LField_Map056_METATILES:
	.incbin "games/tbs/src/map/map_056/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map056_SHAPES:
	.incbin "games/tbs/src/map/map_056/shapes.table.lz"
	.balign 4, 0
.LField_Map056_GRID:
	.incbin "games/tbs/src/map/map_056/grid.parts.lz"
	.balign 4, 0
.LField_Map056_ANIMATION:
	.incbin "games/tbs/src/map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map056_BLEND:
	.incbin "games/common/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map056_MARKERS:
	.incbin "games/tbs/src/map/map_056/markers.table"
