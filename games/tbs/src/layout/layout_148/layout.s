	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout148
Field_Layout148:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	layout_offset Field_Layout148, .LField_Layout148_METATILES
	layout_offset Field_Layout148, .LField_Layout148_SHAPES
	layout_offset Field_Layout148, .LField_Layout148_GRID
	layout_offset Field_Layout148
	layout_offset Field_Layout148
	layout_offset Field_Layout148, .LField_Layout148_MARKERS
.LField_Layout148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Layout148_GRID:
	.incbin "games/tbs/src/layout/layout_148/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	layout_offset Field_Layout148, .LField_Layout148_METATILES
	layout_offset Field_Layout148, .LField_Layout148_SHAPES
	layout_offset Field_Layout148, .LField_Layout148_GRID
	layout_offset Field_Layout148
	layout_offset Field_Layout148
	layout_offset Field_Layout148, .LField_Layout148_MARKERS
.LField_Layout148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Layout148_GRID:
	.incbin "games/tbs/src/layout/layout_148/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	layout_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 192, 0, 20, 20, -121, 35, 0x1f, 0x1f
	layout_offset Field_Layout148, .LField_Layout148_METATILES
	layout_offset Field_Layout148, .LField_Layout148_SHAPES
	layout_offset Field_Layout148, .LField_Layout148_GRID
	layout_offset Field_Layout148
	layout_offset Field_Layout148
	layout_offset Field_Layout148, .LField_Layout148_MARKERS
.LField_Layout148_METATILES:
	.incbin "games/tbs/src/layout/layout_148/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout148_SHAPES:
	.incbin "games/tbs/src/layout/layout_148/shapes.table.lz"
	.balign 4, 0
.LField_Layout148_GRID:
	.incbin "games/tbs/src/layout/layout_148/map.grid.lz"
	.balign 4, 0
.LField_Layout148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.endif
	.endif
