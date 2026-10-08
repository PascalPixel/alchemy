	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map003
Field_Map003:
	map_header 0, 0, 60, 114, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map003, .LField_Map003_METATILES
	map_block Field_Map003, .LField_Map003_SHAPES
	map_block Field_Map003, .LField_Map003_GRID
	map_block Field_Map003, .LField_Map003_ANIMATION
	map_block Field_Map003
	map_block Field_Map003, .LField_Map003_MARKERS
.LField_Map003_METATILES:
	.incbin "map/map_003/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map003_SHAPES:
	.incbin "map/map_003/shapes.table.lz"
	.balign 4, 0
.LField_Map003_GRID:
	.incbin "map/map_003/grid.parts.lz"
	.balign 4, 0
.LField_Map003_ANIMATION:
	.incbin "map/map_003/animation.script.lz"
	.balign 4, 0
.LField_Map003_MARKERS:
	.incbin "map/map_003/markers.table"
	.balign 4, 0
