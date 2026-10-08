	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map170
Field_Map170:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map170, .LField_Map170_METATILES
	map_block Field_Map170, .LField_Map170_SHAPES
	map_block Field_Map170, .LField_Map170_GRID
	map_block Field_Map170, .LField_Map170_ANIMATION
	map_block Field_Map170, .LField_Map170_BLEND
	map_block Field_Map170, .LField_Map170_MARKERS
.LField_Map170_METATILES:
	.incbin "map/map_170/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map170_SHAPES:
	.incbin "common/map/map_170/shapes.table.lz"
	.balign 4, 0
.LField_Map170_GRID:
	.incbin "map/map_170/grid.parts.lz"
	.balign 4, 0
.LField_Map170_ANIMATION:
	.incbin "common/map/map_170/animation.script.lz"
	.balign 4, 0
.LField_Map170_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map170_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
