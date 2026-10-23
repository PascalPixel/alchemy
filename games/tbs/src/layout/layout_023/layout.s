	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout023
Field_Layout023:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout023, .LField_Layout023_METATILES
	layout_offset Field_Layout023, .LField_Layout023_SHAPES
	layout_offset Field_Layout023, .LField_Layout023_GRID
	layout_offset Field_Layout023, .LField_Layout023_ANIMATION
	layout_offset Field_Layout023, .LField_Layout023_BLEND
	layout_offset Field_Layout023, .LField_Layout023_MARKERS
.LField_Layout023_METATILES:
	.incbin "games/tbs/src/layout/layout_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Layout023_SHAPES:
	.incbin "games/tbs/src/layout/layout_023/shapes.table.lz"
	.balign 4, 0
.LField_Layout023_GRID:
	.incbin "games/tbs/src/layout/layout_023/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout023_ANIMATION:
	.incbin "games/tbs/src/layout/layout_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout023_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout023_MARKERS:
	.incbin "games/tbs/src/layout/layout_023/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout023, .LField_Layout023_METATILES
	layout_offset Field_Layout023, .LField_Layout023_SHAPES
	layout_offset Field_Layout023, .LField_Layout023_GRID
	layout_offset Field_Layout023, .LField_Layout023_ANIMATION
	layout_offset Field_Layout023, .LField_Layout023_BLEND
	layout_offset Field_Layout023, .LField_Layout023_MARKERS
.LField_Layout023_METATILES:
	.incbin "games/tbs/src/layout/layout_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Layout023_SHAPES:
	.incbin "games/tbs/src/layout/layout_023/shapes.table.lz"
	.balign 4, 0
.LField_Layout023_GRID:
	.incbin "games/tbs/src/layout/layout_023/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout023_ANIMATION:
	.incbin "games/tbs/src/layout/layout_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout023_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout023_MARKERS:
	.incbin "games/tbs/src/layout/layout_023/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout023, .LField_Layout023_METATILES
	layout_offset Field_Layout023, .LField_Layout023_SHAPES
	layout_offset Field_Layout023, .LField_Layout023_GRID
	layout_offset Field_Layout023, .LField_Layout023_ANIMATION
	layout_offset Field_Layout023, .LField_Layout023_BLEND
	layout_offset Field_Layout023, .LField_Layout023_MARKERS
.LField_Layout023_METATILES:
	.incbin "games/tbs/src/layout/layout_023/metatiles_es.delta2.lz"
	.balign 4, 0
.LField_Layout023_SHAPES:
	.incbin "games/tbs/src/layout/layout_023/shapes.table.lz"
	.balign 4, 0
.LField_Layout023_GRID:
	.incbin "games/tbs/src/layout/layout_023/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout023_ANIMATION:
	.incbin "games/tbs/src/layout/layout_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout023_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout023_MARKERS:
	.incbin "games/tbs/src/layout/layout_023/treasure_spots_es.table"
	.else
	layout_header 0, 0, 96, 115, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 115, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout023, .LField_Layout023_METATILES
	layout_offset Field_Layout023, .LField_Layout023_SHAPES
	layout_offset Field_Layout023, .LField_Layout023_GRID
	layout_offset Field_Layout023, .LField_Layout023_ANIMATION
	layout_offset Field_Layout023, .LField_Layout023_BLEND
	layout_offset Field_Layout023, .LField_Layout023_MARKERS
.LField_Layout023_METATILES:
	.incbin "games/tbs/src/layout/layout_023/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout023_SHAPES:
	.incbin "games/tbs/src/layout/layout_023/shapes.table.lz"
	.balign 4, 0
.LField_Layout023_GRID:
	.incbin "games/tbs/src/layout/layout_023/grid.parts.lz"
	.balign 4, 0
.LField_Layout023_ANIMATION:
	.incbin "games/tbs/src/layout/layout_023/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout023_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout023_MARKERS:
	.incbin "games/tbs/src/layout/layout_023/treasure_spots.table"
	.endif
	.endif
	.endif
	.balign 4, 0
