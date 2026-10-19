	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map132
Field_Map132:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map132, .LField_Map132_METATILES
	map_block Field_Map132, .LField_Map132_SHAPES
	map_block Field_Map132, .LField_Map132_GRID
	map_block Field_Map132, .LField_Map132_ANIMATION
	map_block Field_Map132, .LField_Map132_BLEND
	map_block Field_Map132, .LField_Map132_MARKERS
.LField_Map132_METATILES:
	.incbin "games/tbs/src/map/map_132/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map132_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map132_GRID:
	.incbin "games/tbs/src/map/map_132/grid.parts.lz"
	.balign 4, 0
.LField_Map132_ANIMATION:
	.incbin "games/tbs/src/map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map132_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map132_MARKERS:
	.incbin "games/tbs/src/map/map_132/markers.table"
	.balign 4, 0
