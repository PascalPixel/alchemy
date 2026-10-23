	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout164
Field_Layout164:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout164, .LField_Layout164_METATILES
	layout_offset Field_Layout164, .LField_Layout164_SHAPES
	layout_offset Field_Layout164, .LField_Layout164_GRID
	layout_offset Field_Layout164, .LField_Layout164_ANIMATION
	layout_offset Field_Layout164, .LField_Layout164_BLEND
	layout_offset Field_Layout164, .LField_Layout164_MARKERS
.LField_Layout164_METATILES:
	.incbin "games/tbs/src/layout/layout_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout164_SHAPES:
	.incbin "games/tbs/src/layout/layout_164/shapes.table.lz"
	.balign 4, 0
.LField_Layout164_GRID:
	.incbin "games/tbs/src/layout/layout_164/map_es.grid.lz"
	.balign 4, 0
.LField_Layout164_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout164_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout164_MARKERS:
	.incbin "games/tbs/src/layout/layout_164/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout164, .LField_Layout164_METATILES
	layout_offset Field_Layout164, .LField_Layout164_SHAPES
	layout_offset Field_Layout164, .LField_Layout164_GRID
	layout_offset Field_Layout164, .LField_Layout164_ANIMATION
	layout_offset Field_Layout164, .LField_Layout164_BLEND
	layout_offset Field_Layout164, .LField_Layout164_MARKERS
.LField_Layout164_METATILES:
	.incbin "games/tbs/src/layout/layout_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout164_SHAPES:
	.incbin "games/tbs/src/layout/layout_164/shapes.table.lz"
	.balign 4, 0
.LField_Layout164_GRID:
	.incbin "games/tbs/src/layout/layout_164/map_es.grid.lz"
	.balign 4, 0
.LField_Layout164_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout164_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout164_MARKERS:
	.incbin "games/tbs/src/layout/layout_164/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout164, .LField_Layout164_METATILES
	layout_offset Field_Layout164, .LField_Layout164_SHAPES
	layout_offset Field_Layout164, .LField_Layout164_GRID
	layout_offset Field_Layout164, .LField_Layout164_ANIMATION
	layout_offset Field_Layout164, .LField_Layout164_BLEND
	layout_offset Field_Layout164, .LField_Layout164_MARKERS
.LField_Layout164_METATILES:
	.incbin "games/tbs/src/layout/layout_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout164_SHAPES:
	.incbin "games/tbs/src/layout/layout_164/shapes.table.lz"
	.balign 4, 0
.LField_Layout164_GRID:
	.incbin "games/tbs/src/layout/layout_164/map_es.grid.lz"
	.balign 4, 0
.LField_Layout164_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout164_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout164_MARKERS:
	.incbin "games/tbs/src/layout/layout_164/treasure_spots_es.table"
	.else
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout164, .LField_Layout164_METATILES
	layout_offset Field_Layout164, .LField_Layout164_SHAPES
	layout_offset Field_Layout164, .LField_Layout164_GRID
	layout_offset Field_Layout164, .LField_Layout164_ANIMATION
	layout_offset Field_Layout164, .LField_Layout164_BLEND
	layout_offset Field_Layout164, .LField_Layout164_MARKERS
.LField_Layout164_METATILES:
	.incbin "games/tbs/src/layout/layout_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout164_SHAPES:
	.incbin "games/tbs/src/layout/layout_164/shapes.table.lz"
	.balign 4, 0
.LField_Layout164_GRID:
	.incbin "games/tbs/src/layout/layout_164/map.grid.lz"
	.balign 4, 0
.LField_Layout164_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout164_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout164_MARKERS:
	.incbin "games/tbs/src/layout/layout_164/treasure_spots.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette51
Tileset_Palette51:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_51.gbapal.lz"
	.byte 0xf0, 0x40, 0xef	@ PACKER: left in the buffer by Field_Layout164+0x159
