	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map021
Field_Map021:
	map_header 0, 0, 64, 57, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map021, .LField_Map021_METATILES
	map_block Field_Map021, .LField_Map021_SHAPES
	map_block Field_Map021, .LField_Map021_GRID
	map_block Field_Map021, .LField_Map021_ANIMATION
	map_block Field_Map021, .LField_Map021_BLEND
	map_block Field_Map021, .LField_Map021_MARKERS
.LField_Map021_METATILES:
	.incbin "map/map_021/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map021_SHAPES:
	.incbin "map/map_021/shapes.table.lz"
	.balign 4, 0
.LField_Map021_GRID:
	.incbin "map/map_021/grid.parts.lz"
	.balign 4, 0
.LField_Map021_ANIMATION:
	.incbin "map/map_021/animation.script.lz"
	.balign 4, 0
.LField_Map021_BLEND:
	.incbin "map/map_021/blend.script.lz"
	.balign 4, 0
.LField_Map021_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
