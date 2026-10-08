	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map080
Field_Map080:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map080, .LField_Map080_METATILES
	map_block Field_Map080, .LField_Map080_SHAPES
	map_block Field_Map080, .LField_Map080_GRID
	map_block Field_Map080, .LField_Map080_ANIMATION
	map_block Field_Map080, .LField_Map080_BLEND
	map_block Field_Map080, .LField_Map080_MARKERS
.LField_Map080_METATILES:
	.incbin "map/map_080/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map080_SHAPES:
	.incbin "map/map_080/shapes.table.lz"
	.balign 4, 0
.LField_Map080_GRID:
	.incbin "map/map_080/grid.parts.lz"
	.balign 4, 0
.LField_Map080_ANIMATION:
	.incbin "map/map_080/animation.script.lz"
	.balign 4, 0
.LField_Map080_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map080_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
