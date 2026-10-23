	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map118
Field_Map118:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_MARKERS
.LField_Map118_METATILES:
	.incbin "games/tbs/src/layout/layout_118/metatiles_ja.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "games/tbs/src/layout/layout_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "games/tbs/src/layout/layout_118/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "games/tbs/src/layout/layout_077/animation.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	.incbin "games/tbs/src/layout/layout_118/markers_ja.table"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_MARKERS
.LField_Map118_METATILES:
	.incbin "games/tbs/src/layout/layout_118/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "games/tbs/src/layout/layout_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "games/tbs/src/layout/layout_118/grid.parts.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "games/tbs/src/layout/layout_077/animation.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	.incbin "games/tbs/src/layout/layout_118/markers.table"
	.endif
	.balign 4, 0
	.global Tileset_Palette24
Tileset_Palette24:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_24.gbapal.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5c	@ PACKER: left in the buffer by Field_Map118+0x1a7
	.else
	.byte 0xc8	@ PACKER: left in the buffer by Field_Map118+0x1a7
	.endif
