	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map110
Field_Map110:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map110, .LField_Map110_METATILES
	map_offset Field_Map110, .LField_Map110_SHAPES
	map_offset Field_Map110, .LField_Map110_GRID
	map_offset Field_Map110, .LField_Map110_ANIMATION
	map_offset Field_Map110
	map_offset Field_Map110, .LField_Map110_MARKERS
.LField_Map110_METATILES:
	.incbin "games/tbs/src/map/map_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map110_SHAPES:
	.incbin "games/tbs/src/map/map_110/shapes.table.lz"
	.balign 4, 0
.LField_Map110_GRID:
	.incbin "games/tbs/src/map/map_110/map_ja.grid.lz"
	.balign 4, 0
.LField_Map110_ANIMATION:
	.incbin "games/tbs/src/map/map_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map110_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map110, .LField_Map110_METATILES
	map_offset Field_Map110, .LField_Map110_SHAPES
	map_offset Field_Map110, .LField_Map110_GRID
	map_offset Field_Map110, .LField_Map110_ANIMATION
	map_offset Field_Map110
	map_offset Field_Map110, .LField_Map110_MARKERS
.LField_Map110_METATILES:
	.incbin "games/tbs/src/map/map_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map110_SHAPES:
	.incbin "games/tbs/src/map/map_110/shapes.table.lz"
	.balign 4, 0
.LField_Map110_GRID:
	.incbin "games/tbs/src/map/map_110/map_ja.grid.lz"
	.balign 4, 0
.LField_Map110_ANIMATION:
	.incbin "games/tbs/src/map/map_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map110_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map110, .LField_Map110_METATILES
	map_offset Field_Map110, .LField_Map110_SHAPES
	map_offset Field_Map110, .LField_Map110_GRID
	map_offset Field_Map110, .LField_Map110_ANIMATION
	map_offset Field_Map110
	map_offset Field_Map110, .LField_Map110_MARKERS
.LField_Map110_METATILES:
	.incbin "games/tbs/src/map/map_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map110_SHAPES:
	.incbin "games/tbs/src/map/map_110/shapes.table.lz"
	.balign 4, 0
.LField_Map110_GRID:
	.incbin "games/tbs/src/map/map_110/map.grid.lz"
	.balign 4, 0
.LField_Map110_ANIMATION:
	.incbin "games/tbs/src/map/map_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map110_MARKERS:
	treasure_spot 58, 36, 107
	treasure_spot 13, 38, 100
	treasure_spot 17, 38, 101
	treasure_spot 21, 38, 102
	treasure_spot 25, 38, 103
	treasure_spot 40, 47, 104
	treasure_spot 49, 47, 106
	treasure_spot 45, 53, 105
	treasure_spots_end
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette15
Tileset_Palette15:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_15.gbapal.lz"
	.byte 0x10, 0x2f, 0x79	@ PACKER: left in the buffer by Field_Map110+0x181
