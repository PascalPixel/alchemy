	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map178
Field_Map178:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map178, .LField_Map178_METATILES
	map_block Field_Map178, .LField_Map178_SHAPES
	map_block Field_Map178, .LField_Map178_GRID
	map_block Field_Map178
	map_block Field_Map178
	map_block Field_Map178, .LField_Map178_MARKERS
.LField_Map178_METATILES:
	.incbin "common/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map178_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map178_GRID:
	.incbin "map/map_001/grid.parts.lz"
	.balign 4, 0
.LField_Map178_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
