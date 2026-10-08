	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map119
Field_Map119:
	map_header 0, 0, 76, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 76, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map119, .LField_Map119_METATILES
	map_block Field_Map119, .LField_Map119_SHAPES
	map_block Field_Map119, .LField_Map119_GRID
	map_block Field_Map119, .LField_Map119_ANIMATION
	map_block Field_Map119, .LField_Map119_BLEND
	map_block Field_Map119, .LField_Map119_MARKERS
.LField_Map119_METATILES:
	.incbin "map/map_119/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map119_SHAPES:
	.incbin "map/map_119/shapes.table.lz"
	.balign 4, 0
.LField_Map119_GRID:
	.incbin "map/map_119/grid.parts.lz"
	.balign 4, 0
.LField_Map119_ANIMATION:
	.incbin "map/map_119/animation.script.lz"
	.balign 4, 0
.LField_Map119_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map119_MARKERS:
	.incbin "map/map_119/markers.table"
	.balign 4, 0
