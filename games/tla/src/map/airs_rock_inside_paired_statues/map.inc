	.global Field_AirsRockInsidePairedStatues
Field_AirsRockInsidePairedStatues:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_CELL_PICTURES
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_SHAPES
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_GRID
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_SHAPE_GRID
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_ANIMATION
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_BLEND
	map_block Field_AirsRockInsidePairedStatues, .LField_AirsRockInsidePairedStatues_SCENE
.LField_AirsRockInsidePairedStatues_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_paired_statues/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_paired_statues/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_paired_statues/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_paired_statues/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStatues_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_paired_statues/scene.scene.lz"
	.balign 4, 0
