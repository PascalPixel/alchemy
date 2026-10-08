	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map146
Field_Map146:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map146, .LField_Map146_METATILES
	map_block Field_Map146, .LField_Map146_SHAPES
	map_block Field_Map146, .LField_Map146_GRID
	map_block Field_Map146, .LField_Map146_ANIMATION
	map_block Field_Map146, .LField_Map146_BLEND
	map_block Field_Map146, .LField_Map146_MARKERS
.LField_Map146_METATILES:
	.incbin "map/map_146/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map146_SHAPES:
	.incbin "map/map_146/shapes.table.lz"
	.balign 4, 0
.LField_Map146_GRID:
	.incbin "map/map_146/grid.parts.lz"
	.balign 4, 0
.LField_Map146_ANIMATION:
	.incbin "map/map_145/animation.script.lz"
	.balign 4, 0
.LField_Map146_BLEND:
	.incbin "map/map_009/blend.script.lz"
	.balign 4, 0
.LField_Map146_MARKERS:
	.incbin "map/map_146/markers.table"
