	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map140
Field_Map140:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map140, .LField_Map140_METATILES
	map_offset Field_Map140, .LField_Map140_SHAPES
	map_offset Field_Map140, .LField_Map140_GRID
	map_offset Field_Map140, .LField_Map140_ANIMATION
	map_offset Field_Map140, .LField_Map140_BLEND
	map_offset Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "games/tbs/src/map/map_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "games/tbs/src/map/map_140/map_ja.grid.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "games/tbs/src/map/map_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "games/tbs/src/map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map140, .LField_Map140_METATILES
	map_offset Field_Map140, .LField_Map140_SHAPES
	map_offset Field_Map140, .LField_Map140_GRID
	map_offset Field_Map140, .LField_Map140_ANIMATION
	map_offset Field_Map140, .LField_Map140_BLEND
	map_offset Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "games/tbs/src/map/map_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "games/tbs/src/map/map_140/map.grid.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "games/tbs/src/map/map_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "games/tbs/src/map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	treasure_spots_end
	.endif
	.balign 4, 0
