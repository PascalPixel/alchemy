	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map002
Field_Map002:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_METATILES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002
	map_block Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "map/map_002/grid_es.parts.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "map/map_002/animation.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	.incbin "map/map_002/markers_es.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_METATILES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002
	map_block Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "map/map_002/grid_es.parts.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "map/map_002/animation.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	.incbin "map/map_002/markers_es.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_METATILES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002
	map_block Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "map/map_002/grid_it.parts.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "map/map_002/animation.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	.incbin "map/map_002/markers_es.table"
	.else
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_METATILES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002
	map_block Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "map/map_002/grid.parts.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "map/map_002/animation.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	.incbin "map/map_002/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
