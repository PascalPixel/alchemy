	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout159
Field_Layout159:
	.ifdef TBS_EDITION_DE
	layout_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout159, .LField_Layout159_METATILES
	layout_offset Field_Layout159, .LField_Layout159_SHAPES
	layout_offset Field_Layout159, .LField_Layout159_GRID
	layout_offset Field_Layout159, .LField_Layout159_ANIMATION
	layout_offset Field_Layout159
	layout_offset Field_Layout159, .LField_Layout159_MARKERS
.LField_Layout159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Layout159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Layout159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout159, .LField_Layout159_METATILES
	layout_offset Field_Layout159, .LField_Layout159_SHAPES
	layout_offset Field_Layout159, .LField_Layout159_GRID
	layout_offset Field_Layout159, .LField_Layout159_ANIMATION
	layout_offset Field_Layout159
	layout_offset Field_Layout159, .LField_Layout159_MARKERS
.LField_Layout159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Layout159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Layout159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout159, .LField_Layout159_METATILES
	layout_offset Field_Layout159, .LField_Layout159_SHAPES
	layout_offset Field_Layout159, .LField_Layout159_GRID
	layout_offset Field_Layout159, .LField_Layout159_ANIMATION
	layout_offset Field_Layout159
	layout_offset Field_Layout159, .LField_Layout159_MARKERS
.LField_Layout159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Layout159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Layout159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/treasure_spots_de.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout159, .LField_Layout159_METATILES
	layout_offset Field_Layout159, .LField_Layout159_SHAPES
	layout_offset Field_Layout159, .LField_Layout159_GRID
	layout_offset Field_Layout159, .LField_Layout159_ANIMATION
	layout_offset Field_Layout159
	layout_offset Field_Layout159, .LField_Layout159_MARKERS
.LField_Layout159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Layout159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Layout159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Layout159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/treasure_spots_de.table"
	.else
	layout_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout159, .LField_Layout159_METATILES
	layout_offset Field_Layout159, .LField_Layout159_SHAPES
	layout_offset Field_Layout159, .LField_Layout159_GRID
	layout_offset Field_Layout159, .LField_Layout159_ANIMATION
	layout_offset Field_Layout159
	layout_offset Field_Layout159, .LField_Layout159_MARKERS
.LField_Layout159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Layout159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid.parts.lz"
	.balign 4, 0
.LField_Layout159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/treasure_spots.table"
	.endif
	.endif
	.endif
	.endif
	.global Tileset_Palette47
Tileset_Palette47:
	.ifdef TBS_EDITION_DE
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_47_de.gbapal.lz"
	.else
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_47.gbapal.lz"
	.endif
	.endif
	.endif
	.endif
