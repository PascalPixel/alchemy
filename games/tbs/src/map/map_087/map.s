	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map087, .LField_Map087_METATILES
	map_block Field_Map087, .LField_Map087_SHAPES
	map_block Field_Map087, .LField_Map087_GRID
	map_block Field_Map087, .LField_Map087_ANIMATION
	map_block Field_Map087, .LField_Map087_BLEND
	map_block Field_Map087, .LField_Map087_MARKERS
.LField_Map087_METATILES:
	.incbin "map/map_087/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "map/map_087/shapes.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "map/map_087/grid.parts.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "map/map_080/animation.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map087_MARKERS:
	.incbin "map/map_087/markers.table"
	.balign 4, 0
