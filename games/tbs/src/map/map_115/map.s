	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map115
Field_Map115:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "map/map_115/grid_es.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "map/map_115/markers_es.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "map/map_115/grid_es.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "map/map_115/markers_es.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "map/map_115/grid_es.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "map/map_115/markers_es.table"
	.else
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map115, .LField_Map115_METATILES
	map_block Field_Map115, .LField_Map115_SHAPES
	map_block Field_Map115, .LField_Map115_GRID
	map_block Field_Map115, .LField_Map115_ANIMATION
	map_block Field_Map115, .LField_Map115_BLEND
	map_block Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "map/map_115/grid.parts.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	.incbin "map/map_115/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette21
Tileset_Palette21:
	.incbin "graphics/tileset/palettes/pal_21.gbapal.lz"
	.byte 0xfe, 0xa6	@ PACKER: left in the buffer by Field_Map115+0x1ba
