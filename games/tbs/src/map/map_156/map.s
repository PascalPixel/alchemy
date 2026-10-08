	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map156
Field_Map156:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map156, .LField_Map156_METATILES
	map_block Field_Map156, .LField_Map156_SHAPES
	map_block Field_Map156, .LField_Map156_GRID
	map_block Field_Map156, .LField_Map156_ANIMATION
	map_block Field_Map156, .LField_Map156_BLEND
	map_block Field_Map156, .LField_Map156_MARKERS
.LField_Map156_METATILES:
	.incbin "map/map_156/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map156_SHAPES:
	.incbin "map/map_156/shapes.table.lz"
	.balign 4, 0
.LField_Map156_GRID:
	.incbin "map/map_156/grid.parts.lz"
	.balign 4, 0
.LField_Map156_ANIMATION:
	.incbin "map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map156_BLEND:
	.incbin "common/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map156_MARKERS:
	.incbin "map/map_156/markers.table"
	.balign 4, 0
