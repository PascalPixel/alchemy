	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map163
Field_Map163:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map163, .LField_Map163_CELL_PICTURES
	map_offset Field_Map163, .LField_Map163_SHAPES
	map_offset Field_Map163, .LField_Map163_GRID
	map_offset Field_Map163, .LField_Map163_ANIMATION
	map_offset Field_Map163, .LField_Map163_BLEND
	map_offset Field_Map163, .LField_Map163_MARKERS
.LField_Map163_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_163/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "games/tbs/src/map/map_163/shapes.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "games/tbs/src/map/map_163/map_ja.grid.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "games/tbs/src/map/map_163/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map163_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map163_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map163, .LField_Map163_CELL_PICTURES
	map_offset Field_Map163, .LField_Map163_SHAPES
	map_offset Field_Map163, .LField_Map163_GRID
	map_offset Field_Map163, .LField_Map163_ANIMATION
	map_offset Field_Map163, .LField_Map163_BLEND
	map_offset Field_Map163, .LField_Map163_MARKERS
.LField_Map163_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_163/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "games/tbs/src/map/map_163/shapes.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "games/tbs/src/map/map_163/map.grid.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "games/tbs/src/map/map_163/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map163_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map163_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.endif
	.balign 4, 0
	.global Tileset_Palette50
Tileset_Palette50:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_50.gbapal.lz"
