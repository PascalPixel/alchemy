	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map030
Field_Map030:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map030, .LField_Map030_METATILES
	map_block Field_Map030, .LField_Map030_SHAPES
	map_block Field_Map030, .LField_Map030_GRID
	map_block Field_Map030, .LField_Map030_ANIMATION
	map_block Field_Map030
	map_block Field_Map030, .LField_Map030_MARKERS
.LField_Map030_METATILES:
	.incbin "games/tbs/src/map/map_030/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map030_SHAPES:
	.incbin "games/tbs/src/map/map_030/shapes.table.lz"
	.balign 4, 0
.LField_Map030_GRID:
	.incbin "games/tbs/src/map/map_030/grid.parts.lz"
	.balign 4, 0
.LField_Map030_ANIMATION:
	.incbin "games/common/src/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map030_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
