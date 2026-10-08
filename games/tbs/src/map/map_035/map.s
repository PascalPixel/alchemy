	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map035
Field_Map035:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "map/map_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "map/map_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "map/map_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "map/map_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "map/map_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "map/map_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "map/map_035/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "map/map_035/grid.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
