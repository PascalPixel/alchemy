	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout118
Field_Layout118:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout118, .LField_Layout118_METATILES
	layout_offset Field_Layout118, .LField_Layout118_SHAPES
	layout_offset Field_Layout118, .LField_Layout118_GRID
	layout_offset Field_Layout118, .LField_Layout118_ANIMATION
	layout_offset Field_Layout118, .LField_Layout118_BLEND
	layout_offset Field_Layout118, .LField_Layout118_MARKERS
.LField_Layout118_METATILES:
	.incbin "games/tbs/src/layout/layout_118/metatiles_ja.delta1.lz"
	.balign 4, 0
.LField_Layout118_SHAPES:
	.incbin "games/tbs/src/layout/layout_118/shapes.table.lz"
	.balign 4, 0
.LField_Layout118_GRID:
	.incbin "games/tbs/src/layout/layout_118/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout118_ANIMATION:
	.incbin "games/tbs/src/layout/layout_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout118_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout118_MARKERS:
	.incbin "games/tbs/src/layout/layout_118/treasure_spots_ja.table"
	.else
	layout_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout118, .LField_Layout118_METATILES
	layout_offset Field_Layout118, .LField_Layout118_SHAPES
	layout_offset Field_Layout118, .LField_Layout118_GRID
	layout_offset Field_Layout118, .LField_Layout118_ANIMATION
	layout_offset Field_Layout118, .LField_Layout118_BLEND
	layout_offset Field_Layout118, .LField_Layout118_MARKERS
.LField_Layout118_METATILES:
	.incbin "games/tbs/src/layout/layout_118/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout118_SHAPES:
	.incbin "games/tbs/src/layout/layout_118/shapes.table.lz"
	.balign 4, 0
.LField_Layout118_GRID:
	.incbin "games/tbs/src/layout/layout_118/map.grid.lz"
	.balign 4, 0
.LField_Layout118_ANIMATION:
	.incbin "games/tbs/src/layout/layout_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout118_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout118_MARKERS:
	.incbin "games/tbs/src/layout/layout_118/treasure_spots.table"
	.endif
	.balign 4, 0
	.global Tileset_Palette24
Tileset_Palette24:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_24.gbapal.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5c	@ PACKER: left in the buffer by Field_Layout118+0x1a7
	.else
	.byte 0xc8	@ PACKER: left in the buffer by Field_Layout118+0x1a7
	.endif
