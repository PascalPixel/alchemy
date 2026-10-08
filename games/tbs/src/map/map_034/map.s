	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map034
Field_Map034:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "map/map_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "map/map_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "map/map_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "map/map_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "map/map_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "map/map_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "map/map_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "map/map_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "map/map_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "map/map_034/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "map/map_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "map/map_034/grid.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "map/map_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
