	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map142
Field_Map142:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map142, .LField_Map142_METATILES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_MARKERS
.LField_Map142_METATILES:
	.incbin "map/map_142/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "common/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "map/map_142/grid.parts.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "common/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map142_MARKERS:
	.incbin "map/map_142/markers.table"
	.balign 4, 0
