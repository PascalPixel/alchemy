	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map123, .LField_Map123_METATILES
	map_block Field_Map123, .LField_Map123_SHAPES
	map_block Field_Map123, .LField_Map123_GRID
	map_block Field_Map123, .LField_Map123_ANIMATION
	map_block Field_Map123, .LField_Map123_BLEND
	map_block Field_Map123, .LField_Map123_MARKERS
.LField_Map123_METATILES:
	.incbin "map/map_123/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "map/map_123/shapes.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "map/map_123/grid.parts.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "map/map_123/animation.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map123_MARKERS:
	.incbin "map/map_123/markers.table"
