	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map024
Field_Map024:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/layout/layout_024/grid_de.parts.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/layout/layout_024/grid_de.parts.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/layout/layout_024/grid_de.parts.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/layout/layout_024/grid_de.parts.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map024, .LField_Map024_METATILES
	map_block Field_Map024, .LField_Map024_SHAPES
	map_block Field_Map024, .LField_Map024_GRID
	map_block Field_Map024, .LField_Map024_ANIMATION
	map_block Field_Map024, .LField_Map024_BLEND
	map_block Field_Map024, .LField_Map024_MARKERS
.LField_Map024_METATILES:
	.incbin "games/tbs/src/layout/layout_024/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map024_SHAPES:
	.incbin "games/tbs/src/layout/layout_024/shapes.table.lz"
	.balign 4, 0
.LField_Map024_GRID:
	.incbin "games/tbs/src/layout/layout_024/grid.parts.lz"
	.balign 4, 0
.LField_Map024_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Map024_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Map024_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
