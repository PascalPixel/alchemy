	.global Field_GarohWayUp
Field_GarohWayUp:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohWayUp, .LField_GarohWayUp_CELL_TILES
	map_block Field_GarohWayUp, .LField_GarohWayUp_SHAPES
	map_block Field_GarohWayUp, .LField_GarohWayUp_GRID
	map_block Field_GarohWayUp, .LField_GarohWayUp_SHAPE_GRID
	map_block Field_GarohWayUp, .LField_GarohWayUp_ANIMATION
	map_block Field_GarohWayUp, .LField_GarohWayUp_BLEND
	map_block Field_GarohWayUp, .LField_GarohWayUp_SCENE
.LField_GarohWayUp_CELL_TILES:
	.incbin "games/tla/src/map/garoh_way_up/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GarohWayUp_SHAPES:
	.incbin "games/tla/src/map/garoh_way_up/shapes.table.lz"
	.balign 4, 0
.LField_GarohWayUp_GRID:
	.incbin "games/tla/src/map/garoh_way_up/map.grid.lz"
	.balign 4, 0
.LField_GarohWayUp_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_way_up/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohWayUp_ANIMATION:
	.incbin "games/tla/src/map/garoh/animation.script.lz"
	.balign 4, 0
.LField_GarohWayUp_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GarohWayUp_SCENE:
	.incbin "games/tla/src/map/garoh_way_up/scene.scene.lz"
	.balign 4, 0
