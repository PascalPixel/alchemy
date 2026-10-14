	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map085
Field_Map085:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map085, .LField_Map085_METATILES
	map_block Field_Map085, .LField_Map085_SHAPES
	map_block Field_Map085, .LField_Map085_GRID
	map_block Field_Map085, .LField_Map085_ANIMATION
	map_block Field_Map085, .LField_Map085_BLEND
	map_block Field_Map085, .LField_Map085_MARKERS
.LField_Map085_METATILES:
	.incbin "games/tbs/src/map/map_085/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map085_SHAPES:
	.incbin "games/tbs/src/map/map_085/shapes.table.lz"
	.balign 4, 0
.LField_Map085_GRID:
	.incbin "games/tbs/src/map/map_085/grid.parts.lz"
	.balign 4, 0
.LField_Map085_ANIMATION:
	.incbin "games/tbs/src/map/map_080/animation.script.lz"
	.balign 4, 0
.LField_Map085_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map085_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
