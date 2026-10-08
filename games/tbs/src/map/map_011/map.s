	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map011
Field_Map011:
	map_header 0, 0, 64, 32, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map011, .LField_Map011_METATILES
	map_block Field_Map011, .LField_Map011_SHAPES
	map_block Field_Map011, .LField_Map011_GRID
	map_block Field_Map011, .LField_Map011_ANIMATION
	map_block Field_Map011, .LField_Map011_BLEND
	map_block Field_Map011, .LField_Map011_MARKERS
.LField_Map011_METATILES:
	.incbin "map/map_011/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map011_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map011_GRID:
	.incbin "map/map_011/grid.parts.lz"
	.balign 4, 0
.LField_Map011_ANIMATION:
	.incbin "map/map_011/animation.script.lz"
	.balign 4, 0
.LField_Map011_BLEND:
	.incbin "map/map_011/blend.script.lz"
	.balign 4, 0
.LField_Map011_MARKERS:
	.incbin "map/map_011/markers.table"
	.balign 4, 0
