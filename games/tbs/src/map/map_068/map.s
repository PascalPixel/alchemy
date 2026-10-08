	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map068
Field_Map068:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map068, .LField_Map068_METATILES
	map_block Field_Map068, .LField_Map068_SHAPES
	map_block Field_Map068, .LField_Map068_GRID
	map_block Field_Map068, .LField_Map068_ANIMATION
	map_block Field_Map068
	map_block Field_Map068, .LField_Map068_MARKERS
.LField_Map068_METATILES:
	.incbin "map/map_068/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map068_SHAPES:
	.incbin "map/map_068/shapes.table.lz"
	.balign 4, 0
.LField_Map068_GRID:
	.incbin "map/map_068/grid.parts.lz"
	.balign 4, 0
.LField_Map068_ANIMATION:
	.incbin "map/map_068/animation.script.lz"
	.balign 4, 0
.LField_Map068_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
