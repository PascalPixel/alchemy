	.include "games/tbs/include/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusEntrance
Field_VenusEntrance:
	map_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusEntrance, .LField_VenusEntrance_CELL_PICTURES
	map_offset Field_VenusEntrance, .LField_VenusEntrance_SHAPES
	map_offset Field_VenusEntrance, .LField_VenusEntrance_GRID
	map_offset Field_VenusEntrance
	map_offset Field_VenusEntrance
	map_offset Field_VenusEntrance, .LField_VenusEntrance_MARKERS
.LField_VenusEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VenusEntrance_SHAPES:
	.incbin "games/tbs/src/map/venus_entrance/shapes.table.lz"
	.balign 4, 0
.LField_VenusEntrance_GRID:
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/map/venus_entrance/map_fr.grid.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/map/venus_entrance/map_es.grid.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/map/venus_entrance/map_es.grid.lz"
	.else
	.incbin "games/tbs/src/map/venus_entrance/map.grid.lz"
	.endif
	.endif
	.endif
	.balign 4, 0
.LField_VenusEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_49.gbapal.lz"
	.byte 0x1e, 0x33	@ PACKER: left in the buffer by Field_VenusEntrance+0x106
