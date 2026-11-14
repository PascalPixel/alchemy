	.global Field_GondowanCave
Field_GondowanCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GondowanCave, .LField_GondowanCave_CELL_PICTURES
	map_offset Field_GondowanCave, .LField_GondowanCave_SHAPES
	map_offset Field_GondowanCave, .LField_GondowanCave_GRID
	map_offset Field_GondowanCave, .LField_GondowanCave_ANIMATION
	map_offset Field_GondowanCave, .LField_GondowanCave_BLEND
	map_offset Field_GondowanCave, .LField_GondowanCave_MARKERS
.LField_GondowanCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/gondowan_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GondowanCave_SHAPES:
	.incbin "games/tbs/src/map/gondowan_cave/shapes.table.lz"
	.balign 4, 0
.LField_GondowanCave_GRID:
	.incbin "games/tbs/src/map/gondowan_cave/map.grid.lz"
	.balign 4, 0
.LField_GondowanCave_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GondowanCave_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GondowanCave_MARKERS:
	treasure_spots_end
	.balign 4, 0
