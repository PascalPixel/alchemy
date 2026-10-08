	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0, 0, 64, 78, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 152, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map107, .LField_Map107_METATILES
	map_block Field_Map107, .LField_Map107_SHAPES
	map_block Field_Map107, .LField_Map107_GRID
	map_block Field_Map107, .LField_Map107_ANIMATION
	map_block Field_Map107
	map_block Field_Map107, .LField_Map107_MARKERS
.LField_Map107_METATILES:
	.incbin "map/map_107/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map107_SHAPES:
	.incbin "map/map_107/shapes.table.lz"
	.balign 4, 0
.LField_Map107_GRID:
	.incbin "map/map_107/grid.parts.lz"
	.balign 4, 0
.LField_Map107_ANIMATION:
	.incbin "map/map_107/animation.script.lz"
	.balign 4, 0
.LField_Map107_MARKERS:
	.incbin "map/map_107/markers.table"
