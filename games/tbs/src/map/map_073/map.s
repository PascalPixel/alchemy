	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map073
Field_Map073:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map073, .LField_Map073_METATILES
	map_block Field_Map073, .LField_Map073_SHAPES
	map_block Field_Map073, .LField_Map073_GRID
	map_block Field_Map073, .LField_Map073_ANIMATION
	map_block Field_Map073, .LField_Map073_BLEND
	map_block Field_Map073, .LField_Map073_MARKERS
.LField_Map073_METATILES:
	.incbin "map/map_073/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map073_SHAPES:
	.incbin "map/map_073/shapes.table.lz"
	.balign 4, 0
.LField_Map073_GRID:
	.incbin "map/map_073/grid.parts.lz"
	.balign 4, 0
.LField_Map073_ANIMATION:
	.incbin "map/map_073/animation.script.lz"
	.balign 4, 0
.LField_Map073_BLEND:
	.incbin "map/map_073/blend.script.lz"
	.balign 4, 0
.LField_Map073_MARKERS:
	.incbin "map/map_073/markers.table"
	.balign 4, 0
