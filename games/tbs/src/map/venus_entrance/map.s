	.include "games/common/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VinasuIriguchi
Field_VinasuIriguchi:
	map_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_METATILES
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_SHAPES
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_GRID
	map_block Field_VinasuIriguchi
	map_block Field_VinasuIriguchi
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_MARKERS
.LField_VinasuIriguchi_METATILES:
	.incbin "games/tbs/src/map/venus_entrance/metatiles.delta1.lz"
	.balign 4, 0
.LField_VinasuIriguchi_SHAPES:
	.incbin "games/tbs/src/map/venus_entrance/shapes.table.lz"
	.balign 4, 0
.LField_VinasuIriguchi_GRID:
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/map/venus_entrance/grid_fr.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/map/venus_entrance/grid_es.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/map/venus_entrance/grid_es.parts.lz"
	.else
	.incbin "games/tbs/src/map/venus_entrance/grid.parts.lz"
	.endif
	.endif
	.endif
	.balign 4, 0
.LField_VinasuIriguchi_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "games/common/src/graphics/tileset/palettes/pal_49.gbapal.lz"
	.byte 0x1e, 0x33	@ PACKER: left in the buffer by Field_VinasuIriguchi+0x106
