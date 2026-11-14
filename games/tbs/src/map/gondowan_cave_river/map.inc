	.global Field_GondowanCaveRiver
Field_GondowanCaveRiver:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_CELL_PICTURES
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_SHAPES
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_GRID
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_ANIMATION
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_BLEND
	map_offset Field_GondowanCaveRiver, .LField_GondowanCaveRiver_MARKERS
.LField_GondowanCaveRiver_CELL_PICTURES:
	.incbin "games/tbs/src/map/gondowan_cave_river/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GondowanCaveRiver_SHAPES:
	.incbin "games/tbs/src/map/gondowan_cave_river/shapes.table.lz"
	.balign 4, 0
.LField_GondowanCaveRiver_GRID:
	.incbin "games/tbs/src/map/gondowan_cave_river/map.grid.lz"
	.balign 4, 0
.LField_GondowanCaveRiver_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GondowanCaveRiver_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GondowanCaveRiver_MARKERS:
	treasure_spot 27, 6, 100
	treasure_spot 24, 13, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteGondowanCave
Tileset_PaletteGondowanCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/gondowan_cave.gbapal.lz"
	.balign 4, 0
