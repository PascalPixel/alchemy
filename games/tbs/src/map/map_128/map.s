	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map128
Field_Map128:
	map_header 0, 0, 166, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_METATILES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128, .LField_Map128_BLEND
	map_block Field_Map128, .LField_Map128_MARKERS
.LField_Map128_METATILES:
	.incbin "map/map_128/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "map/map_128/shapes.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.incbin "map/map_128/grid.parts.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "map/map_128/animation.script.lz"
	.balign 4, 0
.LField_Map128_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map128_MARKERS:
	.incbin "map/map_128/markers.table"
	.balign 4, 0
