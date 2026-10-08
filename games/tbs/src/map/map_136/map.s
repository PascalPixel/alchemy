	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map136
Field_Map136:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map136, .LField_Map136_METATILES
	map_block Field_Map136, .LField_Map136_SHAPES
	map_block Field_Map136, .LField_Map136_GRID
	map_block Field_Map136, .LField_Map136_ANIMATION
	map_block Field_Map136, .LField_Map136_BLEND
	map_block Field_Map136, .LField_Map136_MARKERS
.LField_Map136_METATILES:
	.incbin "map/map_136/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map136_SHAPES:
	.incbin "map/map_136/shapes.table.lz"
	.balign 4, 0
.LField_Map136_GRID:
	.incbin "map/map_136/grid.parts.lz"
	.balign 4, 0
.LField_Map136_ANIMATION:
	.incbin "map/map_022/animation.script.lz"
	.balign 4, 0
.LField_Map136_BLEND:
	.incbin "map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map136_MARKERS:
	.incbin "map/map_136/markers.table"
	.balign 4, 0
