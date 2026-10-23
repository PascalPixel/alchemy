	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout135
Field_Layout135:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles_ja.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid_ja.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots_ja.table"
	.else
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots_ja.table"
	.else
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots_ja.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots_ja.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots_ja.table"
	.else
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout135, .LField_Layout135_METATILES
	layout_offset Field_Layout135, .LField_Layout135_SHAPES
	layout_offset Field_Layout135, .LField_Layout135_GRID
	layout_offset Field_Layout135, .LField_Layout135_ANIMATION
	layout_offset Field_Layout135, .LField_Layout135_BLEND
	layout_offset Field_Layout135, .LField_Layout135_MARKERS
.LField_Layout135_METATILES:
	.incbin "games/tbs/src/layout/layout_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout135_SHAPES:
	.incbin "games/tbs/src/layout/layout_135/shapes.table.lz"
	.balign 4, 0
.LField_Layout135_GRID:
	.incbin "games/tbs/src/layout/layout_135/grid.parts.lz"
	.balign 4, 0
.LField_Layout135_ANIMATION:
	.incbin "games/tbs/src/layout/layout_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout135_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout135_MARKERS:
	.incbin "games/tbs/src/layout/layout_135/treasure_spots.table"
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
