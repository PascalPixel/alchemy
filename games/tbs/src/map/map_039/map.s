	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map039
Field_Map039:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
