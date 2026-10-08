	.include "games/common/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map140
Field_Map140:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "map/map_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "common/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "map/map_140/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "common/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "map/map_140/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "common/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "map/map_140/grid.parts.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "common/map/map_029/animation.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.balign 4, 0
