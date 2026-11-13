	.global Field_WestLunpaCave
Field_WestLunpaCave:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_CELL_PICTURES
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_SHAPES
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_GRID
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_ANIMATION
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_BLEND
	map_offset Field_WestLunpaCave, .LField_WestLunpaCave_MARKERS
.LField_WestLunpaCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/west_lunpa_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_WestLunpaCave_SHAPES:
	.incbin "games/tbs/src/map/west_lunpa_cave/shapes.table.lz"
	.balign 4, 0
.LField_WestLunpaCave_GRID:
	.incbin "games/tbs/src/map/west_lunpa_cave/map.grid.lz"
	.balign 4, 0
.LField_WestLunpaCave_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_WestLunpaCave_BLEND:
	.incbin "games/tbs/src/map/fuchin_falls_cave_zephyr/blend.script.lz"
	.balign 4, 0
.LField_WestLunpaCave_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteWestLunpaCave
Tileset_PaletteWestLunpaCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/west_lunpa_cave.gbapal.lz"
	.balign 4, 0
