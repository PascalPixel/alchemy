	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map089
Field_Map089:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_METATILES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_MARKERS
.LField_Map089_METATILES:
	.incbin "map/map_089/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "map/map_089/shapes.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "map/map_089/grid.parts.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map089_MARKERS:
	.incbin "map/map_089/markers.table"
	.balign 4, 0
