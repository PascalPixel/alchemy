	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map169
Field_Map169:
	map_header 0, 0, 128, 53, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x3f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map169, .LField_Map169_METATILES
	map_block Field_Map169, .LField_Map169_SHAPES
	map_block Field_Map169, .LField_Map169_GRID
	map_block Field_Map169, .LField_Map169_ANIMATION
	map_block Field_Map169, .LField_Map169_BLEND
	map_block Field_Map169, .LField_Map169_MARKERS
.LField_Map169_METATILES:
	.incbin "map/map_169/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map169_SHAPES:
	.incbin "map/map_169/shapes.table.lz"
	.balign 4, 0
.LField_Map169_GRID:
	.incbin "map/map_169/grid.parts.lz"
	.balign 4, 0
.LField_Map169_ANIMATION:
	.incbin "map/map_169/animation.script.lz"
	.balign 4, 0
.LField_Map169_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map169_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
