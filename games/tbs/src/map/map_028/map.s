	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map028
Field_Map028:
	map_header 0, 0, 42, 54, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map028, .LField_Map028_METATILES
	map_block Field_Map028, .LField_Map028_SHAPES
	map_block Field_Map028, .LField_Map028_GRID
	map_block Field_Map028, .LField_Map028_ANIMATION
	map_block Field_Map028, .LField_Map028_BLEND
	map_block Field_Map028, .LField_Map028_MARKERS
.LField_Map028_METATILES:
	.incbin "map/map_028/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map028_SHAPES:
	.incbin "map/map_028/shapes.table.lz"
	.balign 4, 0
.LField_Map028_GRID:
	.incbin "map/map_028/grid.parts.lz"
	.balign 4, 0
.LField_Map028_ANIMATION:
	.incbin "map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map028_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map028_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
