	.global Field_SuhallaDesertSouth
Field_SuhallaDesertSouth:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_CELL_PICTURES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_SHAPES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_GRID
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_MARKERS
.LField_SuhallaDesertSouth_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_desert_south/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_SHAPES:
	.incbin "games/tbs/src/map/suhalla_desert_south/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_GRID:
	.incbin "games/tbs/src/map/suhalla_desert_south/map_ja.grid.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, 135, 35, 0x1f, 0x1f
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_CELL_PICTURES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_SHAPES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_GRID
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_MARKERS
.LField_SuhallaDesertSouth_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_desert_south/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_SHAPES:
	.incbin "games/tbs/src/map/suhalla_desert_south/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_GRID:
	.incbin "games/tbs/src/map/suhalla_desert_south/map_ja.grid.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.else
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_CELL_PICTURES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_SHAPES
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_GRID
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth
	map_offset Field_SuhallaDesertSouth, .LField_SuhallaDesertSouth_MARKERS
.LField_SuhallaDesertSouth_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_desert_south/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_SHAPES:
	.incbin "games/tbs/src/map/suhalla_desert_south/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_GRID:
	.incbin "games/tbs/src/map/suhalla_desert_south/map.grid.lz"
	.balign 4, 0
.LField_SuhallaDesertSouth_MARKERS:
	treasure_spot 28, 8, 104
	treasure_spot 12, 14, 101
	treasure_spot 12, 71, 100
	treasure_spots_end
	.endif
	.endif
	.balign 4, 0
