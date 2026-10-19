	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map152
Field_Map152:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_MARKERS
.LField_Map152_METATILES:
	.incbin "games/tbs/src/map/map_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "games/tbs/src/map/map_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "games/tbs/src/map/map_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "games/tbs/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map152_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_MARKERS
.LField_Map152_METATILES:
	.incbin "games/tbs/src/map/map_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "games/tbs/src/map/map_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "games/tbs/src/map/map_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "games/tbs/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map152_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_MARKERS
.LField_Map152_METATILES:
	.incbin "games/tbs/src/map/map_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "games/tbs/src/map/map_152/shapes_es.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "games/tbs/src/map/map_152/grid_es.parts.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "games/tbs/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map152_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_MARKERS
.LField_Map152_METATILES:
	.incbin "games/tbs/src/map/map_152/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "games/tbs/src/map/map_152/shapes.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "games/tbs/src/map/map_152/grid.parts.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "games/tbs/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map152_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette41
Tileset_Palette41:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_41.gbapal.lz"
	.byte 0xfc, 0x22, 0x8f	@ PACKER: left in the buffer by Field_Map152+0x171
