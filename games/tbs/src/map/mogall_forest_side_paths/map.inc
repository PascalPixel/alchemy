	.global Field_MogallForestSidePaths
Field_MogallForestSidePaths:
	map_header 0, 0, 108, 90, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MogallForestSidePaths, .LField_MogallForestSidePaths_CELL_PICTURES
	map_offset Field_MogallForestSidePaths, .LField_MogallForestSidePaths_SHAPES
	map_offset Field_MogallForestSidePaths, .LField_MogallForestSidePaths_GRID
	map_offset Field_MogallForestSidePaths
	map_offset Field_MogallForestSidePaths
	map_offset Field_MogallForestSidePaths, .LField_MogallForestSidePaths_MARKERS
.LField_MogallForestSidePaths_CELL_PICTURES:
	.incbin "games/tbs/src/map/mogall_forest_side_paths/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_MogallForestSidePaths_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_glade/shapes.table.lz"
	.balign 4, 0
.LField_MogallForestSidePaths_GRID:
	.incbin "games/tbs/src/map/mogall_forest_side_paths/map.grid.lz"
	.balign 4, 0
.LField_MogallForestSidePaths_MARKERS:
	treasure_spot 8, 34, 100
	treasure_spots_end
	.balign 4, 0
