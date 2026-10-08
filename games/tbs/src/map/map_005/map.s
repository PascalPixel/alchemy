	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map005
Field_Map005:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map005, .LField_Map005_METATILES
	map_block Field_Map005, .LField_Map005_SHAPES
	map_block Field_Map005, .LField_Map005_GRID
	map_block Field_Map005, .LField_Map005_ANIMATION
	map_block Field_Map005, .LField_Map005_BLEND
	map_block Field_Map005, .LField_Map005_MARKERS
.LField_Map005_METATILES:
	.incbin "map/map_005/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map005_SHAPES:
	.incbin "map/map_005/shapes.table.lz"
	.balign 4, 0
.LField_Map005_GRID:
	.incbin "map/map_005/grid.parts.lz"
	.balign 4, 0
.LField_Map005_ANIMATION:
	.incbin "map/map_005/animation.script.lz"
	.balign 4, 0
.LField_Map005_BLEND:
	.incbin "map/map_005/blend.script.lz"
	.balign 4, 0
.LField_Map005_MARKERS:
	.incbin "map/map_005/markers.table"
	.balign 4, 0
