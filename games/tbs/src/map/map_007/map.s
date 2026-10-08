	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map007
Field_Map007:
	map_header 0, 0, 48, 33, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map007, .LField_Map007_METATILES
	map_block Field_Map007, .LField_Map007_SHAPES
	map_block Field_Map007, .LField_Map007_GRID
	map_block Field_Map007, .LField_Map007_ANIMATION
	map_block Field_Map007, .LField_Map007_BLEND
	map_block Field_Map007, .LField_Map007_MARKERS
.LField_Map007_METATILES:
	.incbin "map/map_007/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map007_SHAPES:
	.incbin "map/map_007/shapes.table.lz"
	.balign 4, 0
.LField_Map007_GRID:
	.incbin "map/map_007/grid.parts.lz"
	.balign 4, 0
.LField_Map007_ANIMATION:
	.incbin "map/map_007/animation.script.lz"
	.balign 4, 0
.LField_Map007_BLEND:
	.incbin "map/map_005/blend.script.lz"
	.balign 4, 0
.LField_Map007_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
