	.include "games/tbs/include/game/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map135
Field_Map135:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles_ja.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map_ja.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map_de.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map_de.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map_de.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map_de.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map135, .LField_Map135_METATILES
	map_offset Field_Map135, .LField_Map135_SHAPES
	map_offset Field_Map135, .LField_Map135_GRID
	map_offset Field_Map135, .LField_Map135_ANIMATION
	map_offset Field_Map135, .LField_Map135_BLEND
	map_offset Field_Map135, .LField_Map135_MARKERS
.LField_Map135_METATILES:
	.incbin "games/tbs/src/map/map_135/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map135_SHAPES:
	.incbin "games/tbs/src/map/map_135/shapes.table.lz"
	.balign 4, 0
.LField_Map135_GRID:
	.incbin "games/tbs/src/map/map_135/map.grid.lz"
	.balign 4, 0
.LField_Map135_ANIMATION:
	.incbin "games/tbs/src/map/map_133/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map135_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map135_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
