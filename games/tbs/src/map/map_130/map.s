	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map130
Field_Map130:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map130, .LField_Map130_METATILES
	map_block Field_Map130, .LField_Map130_SHAPES
	map_block Field_Map130, .LField_Map130_GRID
	map_block Field_Map130, .LField_Map130_ANIMATION
	map_block Field_Map130, .LField_Map130_BLEND
	map_block Field_Map130, .LField_Map130_MARKERS
.LField_Map130_METATILES:
	.incbin "map/map_130/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map130_SHAPES:
	.incbin "common/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map130_GRID:
	.incbin "map/map_130/grid.parts.lz"
	.balign 4, 0
.LField_Map130_ANIMATION:
	.incbin "map/map_130/animation.script.lz"
	.balign 4, 0
.LField_Map130_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map130_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
