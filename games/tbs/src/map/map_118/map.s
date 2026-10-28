	.include "games/tbs/include/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map118
Field_Map118:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map118, .LField_Map118_CELL_PICTURES
	map_offset Field_Map118, .LField_Map118_SHAPES
	map_offset Field_Map118, .LField_Map118_GRID
	map_offset Field_Map118, .LField_Map118_ANIMATION
	map_offset Field_Map118, .LField_Map118_BLEND
	map_offset Field_Map118, .LField_Map118_MARKERS
.LField_Map118_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_118/cell_pictures_ja.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "games/tbs/src/map/map_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "games/tbs/src/map/map_118/map_ja.grid.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "games/tbs/src/map/map_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	treasure_spot 0, 0, 100
	treasure_spot 0, 1, 100
	treasure_spots_end
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map118, .LField_Map118_CELL_PICTURES
	map_offset Field_Map118, .LField_Map118_SHAPES
	map_offset Field_Map118, .LField_Map118_GRID
	map_offset Field_Map118, .LField_Map118_ANIMATION
	map_offset Field_Map118, .LField_Map118_BLEND
	map_offset Field_Map118, .LField_Map118_MARKERS
.LField_Map118_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_118/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "games/tbs/src/map/map_118/shapes.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "games/tbs/src/map/map_118/map.grid.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "games/tbs/src/map/map_077/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	treasure_spot 0, 0, 100
	treasure_spot 0, 1, 100
	treasure_spots_end
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
