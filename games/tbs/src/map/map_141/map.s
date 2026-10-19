	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map141
Field_Map141:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_MARKERS
.LField_Map141_METATILES:
	.incbin "games/tbs/src/map/map_141/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "games/tbs/src/map/map_141/shapes.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "games/tbs/src/map/map_141/grid.parts.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "games/tbs/src/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "games/tbs/src/map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map141_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
