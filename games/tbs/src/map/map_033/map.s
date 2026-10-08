	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map033
Field_Map033:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map033, .LField_Map033_METATILES
	map_block Field_Map033, .LField_Map033_SHAPES
	map_block Field_Map033, .LField_Map033_GRID
	map_block Field_Map033, .LField_Map033_ANIMATION
	map_block Field_Map033, .LField_Map033_BLEND
	map_block Field_Map033, .LField_Map033_MARKERS
.LField_Map033_METATILES:
	.incbin "map/map_033/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map033_SHAPES:
	.incbin "map/map_033/shapes.table.lz"
	.balign 4, 0
.LField_Map033_GRID:
	.incbin "map/map_033/grid.parts.lz"
	.balign 4, 0
.LField_Map033_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map033_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map033_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
