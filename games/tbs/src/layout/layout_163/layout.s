	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout163
Field_Layout163:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout163, .LField_Layout163_METATILES
	layout_offset Field_Layout163, .LField_Layout163_SHAPES
	layout_offset Field_Layout163, .LField_Layout163_GRID
	layout_offset Field_Layout163, .LField_Layout163_ANIMATION
	layout_offset Field_Layout163, .LField_Layout163_BLEND
	layout_offset Field_Layout163, .LField_Layout163_MARKERS
.LField_Layout163_METATILES:
	.incbin "games/tbs/src/layout/layout_163/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout163_SHAPES:
	.incbin "games/tbs/src/layout/layout_163/shapes.table.lz"
	.balign 4, 0
.LField_Layout163_GRID:
	.incbin "games/tbs/src/layout/layout_163/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout163_ANIMATION:
	.incbin "games/tbs/src/layout/layout_163/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout163_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout163_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.else
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout163, .LField_Layout163_METATILES
	layout_offset Field_Layout163, .LField_Layout163_SHAPES
	layout_offset Field_Layout163, .LField_Layout163_GRID
	layout_offset Field_Layout163, .LField_Layout163_ANIMATION
	layout_offset Field_Layout163, .LField_Layout163_BLEND
	layout_offset Field_Layout163, .LField_Layout163_MARKERS
.LField_Layout163_METATILES:
	.incbin "games/tbs/src/layout/layout_163/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout163_SHAPES:
	.incbin "games/tbs/src/layout/layout_163/shapes.table.lz"
	.balign 4, 0
.LField_Layout163_GRID:
	.incbin "games/tbs/src/layout/layout_163/map.grid.lz"
	.balign 4, 0
.LField_Layout163_ANIMATION:
	.incbin "games/tbs/src/layout/layout_163/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout163_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout163_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.endif
	.balign 4, 0
	.global Tileset_Palette50
Tileset_Palette50:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_50.gbapal.lz"
