	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_VenusEntrance
Field_VenusEntrance:
	layout_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_VenusEntrance, .LField_VenusEntrance_METATILES
	layout_offset Field_VenusEntrance, .LField_VenusEntrance_SHAPES
	layout_offset Field_VenusEntrance, .LField_VenusEntrance_GRID
	layout_offset Field_VenusEntrance
	layout_offset Field_VenusEntrance
	layout_offset Field_VenusEntrance, .LField_VenusEntrance_MARKERS
.LField_VenusEntrance_METATILES:
	.incbin "games/tbs/src/layout/venus_entrance/metatiles.delta1.lz"
	.balign 4, 0
.LField_VenusEntrance_SHAPES:
	.incbin "games/tbs/src/layout/venus_entrance/shapes.table.lz"
	.balign 4, 0
.LField_VenusEntrance_GRID:
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/layout/venus_entrance/map_fr.grid.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/layout/venus_entrance/map_es.grid.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/layout/venus_entrance/map_es.grid.lz"
	.else
	.incbin "games/tbs/src/layout/venus_entrance/map.grid.lz"
	.endif
	.endif
	.endif
	.balign 4, 0
.LField_VenusEntrance_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_49.gbapal.lz"
	.byte 0x1e, 0x33	@ PACKER: left in the buffer by Field_VenusEntrance+0x106
