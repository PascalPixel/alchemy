	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map163
Field_Map163:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map163, .LField_Map163_METATILES
	map_block Field_Map163, .LField_Map163_SHAPES
	map_block Field_Map163, .LField_Map163_GRID
	map_block Field_Map163, .LField_Map163_ANIMATION
	map_block Field_Map163, .LField_Map163_BLEND
	map_block Field_Map163, .LField_Map163_MARKERS
.LField_Map163_METATILES:
	.incbin "games/tbs/src/layout/layout_163/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "games/tbs/src/layout/layout_163/shapes.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "games/tbs/src/layout/layout_163/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "games/tbs/src/layout/layout_163/animation.script.lz"
	.balign 4, 0
.LField_Map163_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Map163_MARKERS:
	.incbin "games/tbs/src/layout/layout_163/markers_ja.table"
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map163, .LField_Map163_METATILES
	map_block Field_Map163, .LField_Map163_SHAPES
	map_block Field_Map163, .LField_Map163_GRID
	map_block Field_Map163, .LField_Map163_ANIMATION
	map_block Field_Map163, .LField_Map163_BLEND
	map_block Field_Map163, .LField_Map163_MARKERS
.LField_Map163_METATILES:
	.incbin "games/tbs/src/layout/layout_163/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "games/tbs/src/layout/layout_163/shapes.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "games/tbs/src/layout/layout_163/grid.parts.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "games/tbs/src/layout/layout_163/animation.script.lz"
	.balign 4, 0
.LField_Map163_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Map163_MARKERS:
	.incbin "games/tbs/src/layout/layout_163/markers.table"
	.endif
	.balign 4, 0
	.global Tileset_Palette50
Tileset_Palette50:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_50.gbapal.lz"
