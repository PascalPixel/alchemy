	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map164
Field_Map164:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/grid_es.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "games/tbs/src/map/map_164/markers_es.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/grid_es.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "games/tbs/src/map/map_164/markers_es.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/grid_es.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "games/tbs/src/map/map_164/markers_es.table"
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map164, .LField_Map164_METATILES
	map_block Field_Map164, .LField_Map164_SHAPES
	map_block Field_Map164, .LField_Map164_GRID
	map_block Field_Map164, .LField_Map164_ANIMATION
	map_block Field_Map164, .LField_Map164_BLEND
	map_block Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/grid.parts.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/animation.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	.incbin "games/tbs/src/map/map_164/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette51
Tileset_Palette51:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_51.gbapal.lz"
	.byte 0xf0, 0x40, 0xef	@ PACKER: left in the buffer by Field_Map164+0x159
