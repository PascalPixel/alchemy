	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map148
Field_Map148:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_offset Field_Map148, .LField_Map148_CELL_PICTURES
	map_offset Field_Map148, .LField_Map148_SHAPES
	map_offset Field_Map148, .LField_Map148_GRID
	map_offset Field_Map148
	map_offset Field_Map148
	map_offset Field_Map148, .LField_Map148_MARKERS
.LField_Map148_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_148/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/map/map_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/map/map_148/map_ja.grid.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_offset Field_Map148, .LField_Map148_CELL_PICTURES
	map_offset Field_Map148, .LField_Map148_SHAPES
	map_offset Field_Map148, .LField_Map148_GRID
	map_offset Field_Map148
	map_offset Field_Map148
	map_offset Field_Map148, .LField_Map148_MARKERS
.LField_Map148_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_148/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/map/map_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/map/map_148/map_ja.grid.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_offset Field_Map148, .LField_Map148_CELL_PICTURES
	map_offset Field_Map148, .LField_Map148_SHAPES
	map_offset Field_Map148, .LField_Map148_GRID
	map_offset Field_Map148
	map_offset Field_Map148
	map_offset Field_Map148, .LField_Map148_MARKERS
.LField_Map148_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_148/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "games/tbs/src/map/map_148/shapes.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "games/tbs/src/map/map_148/map.grid.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.endif
	.endif
