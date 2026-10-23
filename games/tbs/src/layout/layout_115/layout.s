	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout115
Field_Layout115:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout115, .LField_Layout115_METATILES
	layout_offset Field_Layout115, .LField_Layout115_SHAPES
	layout_offset Field_Layout115, .LField_Layout115_GRID
	layout_offset Field_Layout115, .LField_Layout115_ANIMATION
	layout_offset Field_Layout115, .LField_Layout115_BLEND
	layout_offset Field_Layout115, .LField_Layout115_MARKERS
.LField_Layout115_METATILES:
	.incbin "games/tbs/src/layout/layout_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout115_SHAPES:
	.incbin "games/tbs/src/layout/layout_115/shapes.table.lz"
	.balign 4, 0
.LField_Layout115_GRID:
	.incbin "games/tbs/src/layout/layout_115/map_es.grid.lz"
	.balign 4, 0
.LField_Layout115_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout115_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout115_MARKERS:
	.incbin "games/tbs/src/layout/layout_115/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout115, .LField_Layout115_METATILES
	layout_offset Field_Layout115, .LField_Layout115_SHAPES
	layout_offset Field_Layout115, .LField_Layout115_GRID
	layout_offset Field_Layout115, .LField_Layout115_ANIMATION
	layout_offset Field_Layout115, .LField_Layout115_BLEND
	layout_offset Field_Layout115, .LField_Layout115_MARKERS
.LField_Layout115_METATILES:
	.incbin "games/tbs/src/layout/layout_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout115_SHAPES:
	.incbin "games/tbs/src/layout/layout_115/shapes.table.lz"
	.balign 4, 0
.LField_Layout115_GRID:
	.incbin "games/tbs/src/layout/layout_115/map_es.grid.lz"
	.balign 4, 0
.LField_Layout115_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout115_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout115_MARKERS:
	.incbin "games/tbs/src/layout/layout_115/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout115, .LField_Layout115_METATILES
	layout_offset Field_Layout115, .LField_Layout115_SHAPES
	layout_offset Field_Layout115, .LField_Layout115_GRID
	layout_offset Field_Layout115, .LField_Layout115_ANIMATION
	layout_offset Field_Layout115, .LField_Layout115_BLEND
	layout_offset Field_Layout115, .LField_Layout115_MARKERS
.LField_Layout115_METATILES:
	.incbin "games/tbs/src/layout/layout_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout115_SHAPES:
	.incbin "games/tbs/src/layout/layout_115/shapes.table.lz"
	.balign 4, 0
.LField_Layout115_GRID:
	.incbin "games/tbs/src/layout/layout_115/map_es.grid.lz"
	.balign 4, 0
.LField_Layout115_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout115_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout115_MARKERS:
	.incbin "games/tbs/src/layout/layout_115/treasure_spots_es.table"
	.else
	layout_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout115, .LField_Layout115_METATILES
	layout_offset Field_Layout115, .LField_Layout115_SHAPES
	layout_offset Field_Layout115, .LField_Layout115_GRID
	layout_offset Field_Layout115, .LField_Layout115_ANIMATION
	layout_offset Field_Layout115, .LField_Layout115_BLEND
	layout_offset Field_Layout115, .LField_Layout115_MARKERS
.LField_Layout115_METATILES:
	.incbin "games/tbs/src/layout/layout_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout115_SHAPES:
	.incbin "games/tbs/src/layout/layout_115/shapes.table.lz"
	.balign 4, 0
.LField_Layout115_GRID:
	.incbin "games/tbs/src/layout/layout_115/map.grid.lz"
	.balign 4, 0
.LField_Layout115_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout115_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout115_MARKERS:
	.incbin "games/tbs/src/layout/layout_115/treasure_spots.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette21
Tileset_Palette21:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_21.gbapal.lz"
	.byte 0xfe, 0xa6	@ PACKER: left in the buffer by Field_Layout115+0x1ba
