	.global Field_GarohBlocks
Field_GarohBlocks:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohBlocks, .LField_GarohBlocks_CELL_PICTURES
	map_block Field_GarohBlocks, .LField_GarohBlocks_SHAPES
	map_block Field_GarohBlocks, .LField_GarohBlocks_GRID
	map_block Field_GarohBlocks, .LField_GarohBlocks_SHAPE_GRID
	map_block Field_GarohBlocks, .LField_GarohBlocks_ANIMATION
	map_block Field_GarohBlocks, .LField_GarohBlocks_BLEND
	map_block Field_GarohBlocks, .LField_GarohBlocks_SCENE
.LField_GarohBlocks_CELL_PICTURES:
	.incbin "games/tla/src/map/garoh_blocks/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GarohBlocks_SHAPES:
	.incbin "games/tla/src/map/garoh_blocks/shapes.table.lz"
	.balign 4, 0
.LField_GarohBlocks_GRID:
	.incbin "games/tla/src/map/garoh_blocks/map.grid.lz"
	.balign 4, 0
.LField_GarohBlocks_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_blocks/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohBlocks_ANIMATION:
	.incbin "games/tla/src/map/garoh_1/animation.script.lz"
	.balign 4, 0
.LField_GarohBlocks_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GarohBlocks_SCENE:
	.incbin "games/tla/src/map/garoh_blocks/scene.scene.lz"
	.balign 4, 0
