	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map051
Field_Map051:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid_de.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "map/map_051/markers_de.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "map/map_051/markers_de.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "map/map_051/markers_de.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid_es.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "map/map_051/markers_de.table"
	.else
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "map/map_051/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "map/map_051/shapes.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "map/map_051/grid.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "map/map_051/markers.table"
	.endif
	.endif
	.endif
	.endif
