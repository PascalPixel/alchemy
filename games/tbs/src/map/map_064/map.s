	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map064
Field_Map064:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map064, .LField_Map064_METATILES
	map_block Field_Map064, .LField_Map064_SHAPES
	map_block Field_Map064, .LField_Map064_GRID
	map_block Field_Map064, .LField_Map064_ANIMATION
	map_block Field_Map064, .LField_Map064_BLEND
	map_block Field_Map064, .LField_Map064_MARKERS
.LField_Map064_METATILES:
	.incbin "map/map_064/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map064_SHAPES:
	.incbin "map/map_064/shapes.table.lz"
	.balign 4, 0
.LField_Map064_GRID:
	.incbin "map/map_064/grid.parts.lz"
	.balign 4, 0
.LField_Map064_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map064_BLEND:
	.incbin "map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map064_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
