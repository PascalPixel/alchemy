	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MogallForestQuartz
Field_MogallForestQuartz:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MogallForestQuartz, .LField_MogallForestQuartz_CELL_PICTURES
	map_offset Field_MogallForestQuartz, .LField_MogallForestQuartz_SHAPES
	map_offset Field_MogallForestQuartz, .LField_MogallForestQuartz_GRID
	map_offset Field_MogallForestQuartz, .LField_MogallForestQuartz_ANIMATION
	map_offset Field_MogallForestQuartz
	map_offset Field_MogallForestQuartz, .LField_MogallForestQuartz_MARKERS
.LField_MogallForestQuartz_CELL_PICTURES:
	.incbin "games/tbs/src/map/mogall_forest_quartz/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_MogallForestQuartz_SHAPES:
	.incbin "games/tbs/src/map/mogall_forest_quartz/shapes.table.lz"
	.balign 4, 0
.LField_MogallForestQuartz_GRID:
	.incbin "games/tbs/src/map/mogall_forest_quartz/map.grid.lz"
	.balign 4, 0
.LField_MogallForestQuartz_ANIMATION:
	.incbin "games/tbs/src/map/mogall_forest_sign/tileset_anims.script.lz"
	.balign 4, 0
.LField_MogallForestQuartz_MARKERS:
	treasure_spot 8, 20, 100
	treasure_spots_end
	.balign 4, 0
