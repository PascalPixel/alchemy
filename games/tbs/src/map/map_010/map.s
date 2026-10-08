	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map010
Field_Map010:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map010, .LField_Map010_METATILES
	map_block Field_Map010, .LField_Map010_SHAPES
	map_block Field_Map010, .LField_Map010_GRID
	map_block Field_Map010, .LField_Map010_ANIMATION
	map_block Field_Map010, .LField_Map010_BLEND
	map_block Field_Map010, .LField_Map010_MARKERS
.LField_Map010_METATILES:
	.incbin "map/map_010/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map010_SHAPES:
	.incbin "map/map_010/shapes.table.lz"
	.balign 4, 0
.LField_Map010_GRID:
	.incbin "map/map_010/grid.parts.lz"
	.balign 4, 0
.LField_Map010_ANIMATION:
	.incbin "map/map_010/animation.script.lz"
	.balign 4, 0
.LField_Map010_BLEND:
	.incbin "map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map010_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
