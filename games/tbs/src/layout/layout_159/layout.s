	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map159
Field_Map159:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/markers_de.table"
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/markers_de.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/markers_de.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid_de.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/markers_de.table"
	.else
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map159, .LField_Map159_METATILES
	map_block Field_Map159, .LField_Map159_SHAPES
	map_block Field_Map159, .LField_Map159_GRID
	map_block Field_Map159, .LField_Map159_ANIMATION
	map_block Field_Map159
	map_block Field_Map159, .LField_Map159_MARKERS
.LField_Map159_METATILES:
	.incbin "games/tbs/src/layout/layout_159/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/layout/layout_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/layout/layout_159/grid.parts.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	.incbin "games/tbs/src/layout/layout_159/markers.table"
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
