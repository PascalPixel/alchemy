	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map159
Field_Map159:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map159, .LField_Map159_CELL_PICTURES
	map_offset Field_Map159, .LField_Map159_SHAPES
	map_offset Field_Map159, .LField_Map159_GRID
	map_offset Field_Map159, .LField_Map159_ANIMATION
	map_offset Field_Map159
	map_offset Field_Map159, .LField_Map159_MARKERS
.LField_Map159_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_159/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/map/map_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/map/map_159/map_de.grid.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/map/map_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	treasure_spot 18, 7, 100
	treasure_spot 15, 13, 101
	treasure_spot 26, 25, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map159, .LField_Map159_CELL_PICTURES
	map_offset Field_Map159, .LField_Map159_SHAPES
	map_offset Field_Map159, .LField_Map159_GRID
	map_offset Field_Map159, .LField_Map159_ANIMATION
	map_offset Field_Map159
	map_offset Field_Map159, .LField_Map159_MARKERS
.LField_Map159_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_159/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/map/map_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/map/map_159/map_de.grid.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/map/map_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	treasure_spot 18, 7, 100
	treasure_spot 15, 13, 101
	treasure_spot 26, 25, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map159, .LField_Map159_CELL_PICTURES
	map_offset Field_Map159, .LField_Map159_SHAPES
	map_offset Field_Map159, .LField_Map159_GRID
	map_offset Field_Map159, .LField_Map159_ANIMATION
	map_offset Field_Map159
	map_offset Field_Map159, .LField_Map159_MARKERS
.LField_Map159_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_159/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/map/map_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/map/map_159/map_de.grid.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/map/map_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	treasure_spot 18, 7, 100
	treasure_spot 15, 13, 101
	treasure_spot 26, 25, 102
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map159, .LField_Map159_CELL_PICTURES
	map_offset Field_Map159, .LField_Map159_SHAPES
	map_offset Field_Map159, .LField_Map159_GRID
	map_offset Field_Map159, .LField_Map159_ANIMATION
	map_offset Field_Map159
	map_offset Field_Map159, .LField_Map159_MARKERS
.LField_Map159_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_159/cell_pictures_de.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/map/map_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/map/map_159/map_de.grid.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/map/map_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	treasure_spot 18, 7, 100
	treasure_spot 15, 13, 101
	treasure_spot 26, 25, 102
	treasure_spots_end
	.else
	map_header 0, 0, 64, 56, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map159, .LField_Map159_CELL_PICTURES
	map_offset Field_Map159, .LField_Map159_SHAPES
	map_offset Field_Map159, .LField_Map159_GRID
	map_offset Field_Map159, .LField_Map159_ANIMATION
	map_offset Field_Map159
	map_offset Field_Map159, .LField_Map159_MARKERS
.LField_Map159_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_159/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map159_SHAPES:
	.incbin "games/tbs/src/map/map_159/shapes.table.lz"
	.balign 4, 0
.LField_Map159_GRID:
	.incbin "games/tbs/src/map/map_159/map.grid.lz"
	.balign 4, 0
.LField_Map159_ANIMATION:
	.incbin "games/tbs/src/map/map_150/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map159_MARKERS:
	treasure_spot 18, 7, 100
	treasure_spot 15, 13, 101
	treasure_spot 26, 25, 102
	treasure_spots_end
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
