@ Field_Map153: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
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
	.incbin "COMMON/MAP/MAP_153/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map153_SHAPES:
	.incbin "COMMON/MAP/MAP_153/SHAPES.table.lz"
	.balign 4, 0
.LField_Map153_GRID:
	.incbin "MAP/MAP_153/GRID.parts.lz"
	.balign 4, 0
.LField_Map153_ANIMATION:
	.incbin "COMMON/MAP/MAP_153/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map153_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map153_MARKERS:
	.incbin "MAP/MAP_153/MARKERS.table"
