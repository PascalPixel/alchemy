	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map093
Field_Map093:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map093, .LField_Map093_METATILES
	map_block Field_Map093, .LField_Map093_SHAPES
	map_block Field_Map093, .LField_Map093_GRID
	map_block Field_Map093, .LField_Map093_ANIMATION
	map_block Field_Map093, .LField_Map093_BLEND
	map_block Field_Map093, .LField_Map093_MARKERS
.LField_Map093_METATILES:
	.incbin "map/map_093/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map093_SHAPES:
	.incbin "map/map_093/shapes.table.lz"
	.balign 4, 0
.LField_Map093_GRID:
	.incbin "map/map_093/grid.parts.lz"
	.balign 4, 0
.LField_Map093_ANIMATION:
	.incbin "map/map_093/animation.script.lz"
	.balign 4, 0
.LField_Map093_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map093_MARKERS:
	.incbin "map/map_093/markers.table"
	.balign 4, 0
