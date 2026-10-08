	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map046
Field_Map046:
	map_header 0, 0, 128, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map046, .LField_Map046_METATILES
	map_block Field_Map046, .LField_Map046_SHAPES
	map_block Field_Map046, .LField_Map046_GRID
	map_block Field_Map046, .LField_Map046_ANIMATION
	map_block Field_Map046, .LField_Map046_BLEND
	map_block Field_Map046, .LField_Map046_MARKERS
.LField_Map046_METATILES:
	.incbin "map/map_046/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map046_SHAPES:
	.incbin "map/map_046/shapes.table.lz"
	.balign 4, 0
.LField_Map046_GRID:
	.incbin "map/map_046/grid.parts.lz"
	.balign 4, 0
.LField_Map046_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map046_BLEND:
	.incbin "map/map_046/blend.script.lz"
	.balign 4, 0
.LField_Map046_MARKERS:
	.incbin "map/map_046/markers.table"
