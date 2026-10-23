	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout152
Field_Layout152:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout152, .LField_Layout152_METATILES
	layout_offset Field_Layout152, .LField_Layout152_SHAPES
	layout_offset Field_Layout152, .LField_Layout152_GRID
	layout_offset Field_Layout152, .LField_Layout152_ANIMATION
	layout_offset Field_Layout152, .LField_Layout152_BLEND
	layout_offset Field_Layout152, .LField_Layout152_MARKERS
.LField_Layout152_METATILES:
	.incbin "games/tbs/src/layout/layout_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout152_SHAPES:
	.incbin "games/tbs/src/layout/layout_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Layout152_GRID:
	.incbin "games/tbs/src/layout/layout_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout152_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/animation.script.lz"
	.balign 4, 0
.LField_Layout152_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout152_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout152, .LField_Layout152_METATILES
	layout_offset Field_Layout152, .LField_Layout152_SHAPES
	layout_offset Field_Layout152, .LField_Layout152_GRID
	layout_offset Field_Layout152, .LField_Layout152_ANIMATION
	layout_offset Field_Layout152, .LField_Layout152_BLEND
	layout_offset Field_Layout152, .LField_Layout152_MARKERS
.LField_Layout152_METATILES:
	.incbin "games/tbs/src/layout/layout_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout152_SHAPES:
	.incbin "games/tbs/src/layout/layout_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Layout152_GRID:
	.incbin "games/tbs/src/layout/layout_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout152_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/animation.script.lz"
	.balign 4, 0
.LField_Layout152_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout152_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout152, .LField_Layout152_METATILES
	layout_offset Field_Layout152, .LField_Layout152_SHAPES
	layout_offset Field_Layout152, .LField_Layout152_GRID
	layout_offset Field_Layout152, .LField_Layout152_ANIMATION
	layout_offset Field_Layout152, .LField_Layout152_BLEND
	layout_offset Field_Layout152, .LField_Layout152_MARKERS
.LField_Layout152_METATILES:
	.incbin "games/tbs/src/layout/layout_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout152_SHAPES:
	.incbin "games/tbs/src/layout/layout_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Layout152_GRID:
	.incbin "games/tbs/src/layout/layout_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout152_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/animation.script.lz"
	.balign 4, 0
.LField_Layout152_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout152_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout152, .LField_Layout152_METATILES
	layout_offset Field_Layout152, .LField_Layout152_SHAPES
	layout_offset Field_Layout152, .LField_Layout152_GRID
	layout_offset Field_Layout152, .LField_Layout152_ANIMATION
	layout_offset Field_Layout152, .LField_Layout152_BLEND
	layout_offset Field_Layout152, .LField_Layout152_MARKERS
.LField_Layout152_METATILES:
	.incbin "games/tbs/src/layout/layout_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout152_SHAPES:
	.incbin "games/tbs/src/layout/layout_152/shapes.table.lz"
	.balign 4, 0
.LField_Layout152_GRID:
	.incbin "games/tbs/src/layout/layout_152/grid.parts.lz"
	.balign 4, 0
.LField_Layout152_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/animation.script.lz"
	.balign 4, 0
.LField_Layout152_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout152_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette41
Tileset_Palette41:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_41.gbapal.lz"
	.byte 0xfc, 0x22, 0x8f	@ PACKER: left in the buffer by Field_Layout152+0x171
