	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout140
Field_Layout140:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout140, .LField_Layout140_METATILES
	layout_offset Field_Layout140, .LField_Layout140_SHAPES
	layout_offset Field_Layout140, .LField_Layout140_GRID
	layout_offset Field_Layout140, .LField_Layout140_ANIMATION
	layout_offset Field_Layout140, .LField_Layout140_BLEND
	layout_offset Field_Layout140, .LField_Layout140_MARKERS
.LField_Layout140_METATILES:
	.incbin "games/tbs/src/layout/layout_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout140_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout140_GRID:
	.incbin "games/tbs/src/layout/layout_140/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout140_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout140_BLEND:
	.incbin "games/tbs/src/layout/layout_139/blend.script.lz"
	.balign 4, 0
.LField_Layout140_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.else
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout140, .LField_Layout140_METATILES
	layout_offset Field_Layout140, .LField_Layout140_SHAPES
	layout_offset Field_Layout140, .LField_Layout140_GRID
	layout_offset Field_Layout140, .LField_Layout140_ANIMATION
	layout_offset Field_Layout140, .LField_Layout140_BLEND
	layout_offset Field_Layout140, .LField_Layout140_MARKERS
.LField_Layout140_METATILES:
	.incbin "games/tbs/src/layout/layout_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout140_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout140_GRID:
	.incbin "games/tbs/src/layout/layout_140/map.grid.lz"
	.balign 4, 0
.LField_Layout140_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout140_BLEND:
	.incbin "games/tbs/src/layout/layout_139/blend.script.lz"
	.balign 4, 0
.LField_Layout140_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.endif
	.balign 4, 0
