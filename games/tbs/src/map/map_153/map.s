	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map153
Field_Map153:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map153, .LField_Map153_METATILES
	map_block Field_Map153, .LField_Map153_SHAPES
	map_block Field_Map153, .LField_Map153_GRID
	map_block Field_Map153, .LField_Map153_ANIMATION
	map_block Field_Map153, .LField_Map153_BLEND
	map_block Field_Map153, .LField_Map153_MARKERS
.LField_Map153_METATILES:
	.incbin "common/map/map_153/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map153_SHAPES:
	.incbin "common/map/map_153/shapes.table.lz"
	.balign 4, 0
.LField_Map153_GRID:
	.incbin "map/map_153/grid.parts.lz"
	.balign 4, 0
.LField_Map153_ANIMATION:
	.incbin "common/map/map_153/animation.script.lz"
	.balign 4, 0
.LField_Map153_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map153_MARKERS:
	.incbin "map/map_153/markers.table"
	.balign 4, 0
