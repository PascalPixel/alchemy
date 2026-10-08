	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map124
Field_Map124:
	map_header 0, 0, 70, 34, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map124, .LField_Map124_METATILES
	map_block Field_Map124, .LField_Map124_SHAPES
	map_block Field_Map124, .LField_Map124_GRID
	map_block Field_Map124, .LField_Map124_ANIMATION
	map_block Field_Map124, .LField_Map124_BLEND
	map_block Field_Map124, .LField_Map124_MARKERS
.LField_Map124_METATILES:
	.incbin "map/map_124/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map124_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map124_GRID:
	.incbin "map/map_124/grid.parts.lz"
	.balign 4, 0
.LField_Map124_ANIMATION:
	.incbin "map/map_124/animation.script.lz"
	.balign 4, 0
.LField_Map124_BLEND:
	.incbin "map/map_124/blend.script.lz"
	.balign 4, 0
.LField_Map124_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
