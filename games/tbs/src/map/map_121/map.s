	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map121
Field_Map121:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map121, .LField_Map121_METATILES
	map_block Field_Map121, .LField_Map121_SHAPES
	map_block Field_Map121, .LField_Map121_GRID
	map_block Field_Map121, .LField_Map121_ANIMATION
	map_block Field_Map121, .LField_Map121_BLEND
	map_block Field_Map121, .LField_Map121_MARKERS
.LField_Map121_METATILES:
	.incbin "map/map_121/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map121_SHAPES:
	.incbin "map/map_121/shapes.table.lz"
	.balign 4, 0
.LField_Map121_GRID:
	.incbin "map/map_121/grid.parts.lz"
	.balign 4, 0
.LField_Map121_ANIMATION:
	.incbin "map/map_094/animation.script.lz"
	.balign 4, 0
.LField_Map121_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map121_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
