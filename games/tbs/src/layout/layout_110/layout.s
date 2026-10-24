	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout110
Field_Layout110:
	.ifeq EDITION_INTERNATIONAL
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout110, .LField_Layout110_METATILES
	layout_offset Field_Layout110, .LField_Layout110_SHAPES
	layout_offset Field_Layout110, .LField_Layout110_GRID
	layout_offset Field_Layout110, .LField_Layout110_ANIMATION
	layout_offset Field_Layout110
	layout_offset Field_Layout110, .LField_Layout110_MARKERS
.LField_Layout110_METATILES:
	.incbin "games/tbs/src/layout/layout_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout110_SHAPES:
	.incbin "games/tbs/src/layout/layout_110/shapes.table.lz"
	.balign 4, 0
.LField_Layout110_GRID:
	.incbin "games/tbs/src/layout/layout_110/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout110_ANIMATION:
	.incbin "games/tbs/src/layout/layout_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout110_MARKERS:
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
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout110, .LField_Layout110_METATILES
	layout_offset Field_Layout110, .LField_Layout110_SHAPES
	layout_offset Field_Layout110, .LField_Layout110_GRID
	layout_offset Field_Layout110, .LField_Layout110_ANIMATION
	layout_offset Field_Layout110
	layout_offset Field_Layout110, .LField_Layout110_MARKERS
.LField_Layout110_METATILES:
	.incbin "games/tbs/src/layout/layout_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout110_SHAPES:
	.incbin "games/tbs/src/layout/layout_110/shapes.table.lz"
	.balign 4, 0
.LField_Layout110_GRID:
	.incbin "games/tbs/src/layout/layout_110/map_ja.grid.lz"
	.balign 4, 0
.LField_Layout110_ANIMATION:
	.incbin "games/tbs/src/layout/layout_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout110_MARKERS:
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
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout110, .LField_Layout110_METATILES
	layout_offset Field_Layout110, .LField_Layout110_SHAPES
	layout_offset Field_Layout110, .LField_Layout110_GRID
	layout_offset Field_Layout110, .LField_Layout110_ANIMATION
	layout_offset Field_Layout110
	layout_offset Field_Layout110, .LField_Layout110_MARKERS
.LField_Layout110_METATILES:
	.incbin "games/tbs/src/layout/layout_110/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout110_SHAPES:
	.incbin "games/tbs/src/layout/layout_110/shapes.table.lz"
	.balign 4, 0
.LField_Layout110_GRID:
	.incbin "games/tbs/src/layout/layout_110/map.grid.lz"
	.balign 4, 0
.LField_Layout110_ANIMATION:
	.incbin "games/tbs/src/layout/layout_110/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout110_MARKERS:
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
	.byte 0x10, 0x2f, 0x79	@ PACKER: left in the buffer by Field_Layout110+0x181
