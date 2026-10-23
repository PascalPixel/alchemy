	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map148
Field_Map148:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148
	map_block Field_Map148
	map_block Field_Map148, .LField_Map148_MARKERS
.LField_Map148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/layout/layout_148/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	.incbin "games/tbs/src/layout/layout_148/markers_ja.table"
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148
	map_block Field_Map148
	map_block Field_Map148, .LField_Map148_MARKERS
.LField_Map148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/layout/layout_148/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	.incbin "games/tbs/src/layout/layout_148/markers_ja.table"
	.else
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148
	map_block Field_Map148
	map_block Field_Map148, .LField_Map148_MARKERS
.LField_Map148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/layout/layout_148/grid.parts.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	.incbin "games/tbs/src/layout/layout_148/markers.table"
	.endif
	.endif
