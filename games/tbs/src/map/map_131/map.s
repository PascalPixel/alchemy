	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map131
Field_Map131:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map131, .LField_Map131_METATILES
	map_block Field_Map131, .LField_Map131_SHAPES
	map_block Field_Map131, .LField_Map131_GRID
	map_block Field_Map131, .LField_Map131_ANIMATION
	map_block Field_Map131, .LField_Map131_BLEND
	map_block Field_Map131, .LField_Map131_MARKERS
.LField_Map131_METATILES:
	.incbin "games/tbs/src/map/map_131/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map131_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map131_GRID:
	.incbin "games/tbs/src/map/map_131/grid.parts.lz"
	.balign 4, 0
.LField_Map131_ANIMATION:
	.incbin "games/tbs/src/map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map131_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map131_MARKERS:
	.incbin "games/tbs/src/map/map_131/markers.table"
	.balign 4, 0
