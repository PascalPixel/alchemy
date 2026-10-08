	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map105
Field_Map105:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_METATILES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_MARKERS
.LField_Map105_METATILES:
	.incbin "map/map_105/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "map/map_105/shapes.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "map/map_105/grid.parts.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "map/map_105/animation.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "map/map_105/blend.script.lz"
	.balign 4, 0
.LField_Map105_MARKERS:
	.incbin "map/map_105/markers.table"
