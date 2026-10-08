	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map081
Field_Map081:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map081, .LField_Map081_METATILES
	map_block Field_Map081, .LField_Map081_SHAPES
	map_block Field_Map081, .LField_Map081_GRID
	map_block Field_Map081, .LField_Map081_ANIMATION
	map_block Field_Map081, .LField_Map081_BLEND
	map_block Field_Map081, .LField_Map081_MARKERS
.LField_Map081_METATILES:
	.incbin "map/map_081/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map081_SHAPES:
	.incbin "map/map_081/shapes.table.lz"
	.balign 4, 0
.LField_Map081_GRID:
	.incbin "map/map_081/grid.parts.lz"
	.balign 4, 0
.LField_Map081_ANIMATION:
	.incbin "map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map081_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map081_MARKERS:
	.incbin "map/map_081/markers.table"
	.balign 4, 0
