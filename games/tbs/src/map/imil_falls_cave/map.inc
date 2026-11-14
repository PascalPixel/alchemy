	.global Field_ImilFallsCave
Field_ImilFallsCave:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 70, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_CELL_PICTURES
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_SHAPES
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_GRID
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_ANIMATION
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_BLEND
	map_offset Field_ImilFallsCave, .LField_ImilFallsCave_MARKERS
.LField_ImilFallsCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/imil_falls_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ImilFallsCave_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_ImilFallsCave_GRID:
	.incbin "games/tbs/src/map/imil_falls_cave/map.grid.lz"
	.balign 4, 0
.LField_ImilFallsCave_ANIMATION:
	.incbin "games/tbs/src/map/imil_falls_cave/tileset_anims.script.lz"
	.balign 4, 0
.LField_ImilFallsCave_BLEND:
	.incbin "games/tbs/src/map/imil_falls_cave/blend.script.lz"
	.balign 4, 0
.LField_ImilFallsCave_MARKERS:
	treasure_spots_end
	.balign 4, 0
