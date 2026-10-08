	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map037
Field_Map037:
	map_header 0, 0, 58, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map037, .LField_Map037_METATILES
	map_block Field_Map037, .LField_Map037_SHAPES
	map_block Field_Map037, .LField_Map037_GRID
	map_block Field_Map037, .LField_Map037_ANIMATION
	map_block Field_Map037, .LField_Map037_BLEND
	map_block Field_Map037, .LField_Map037_MARKERS
.LField_Map037_METATILES:
	.incbin "map/map_037/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map037_SHAPES:
	.incbin "map/map_037/shapes.table.lz"
	.balign 4, 0
.LField_Map037_GRID:
	.incbin "map/map_037/grid.parts.lz"
	.balign 4, 0
.LField_Map037_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map037_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map037_MARKERS:
	.incbin "map/map_037/markers.table"
	.balign 4, 0
