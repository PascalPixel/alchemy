	.global Field_Daila
Field_Daila:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Daila, .LField_Daila_CELL_TILES
	map_block Field_Daila, .LField_Daila_SHAPES
	map_block Field_Daila, .LField_Daila_GRID
	map_block Field_Daila, .LField_Daila_SHAPE_GRID
	map_block Field_Daila, .LField_Daila_ANIMATION
	map_block Field_Daila
	map_block Field_Daila, .LField_Daila_SCENE
.LField_Daila_CELL_TILES:
	.incbin "games/tla/src/map/daila/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_Daila_SHAPES:
	.incbin "games/tla/src/map/daila/shapes.table.lz"
	.balign 4, 0
.LField_Daila_GRID:
	.incbin "games/tla/src/map/daila/map.grid.lz"
	.balign 4, 0
.LField_Daila_SHAPE_GRID:
	.incbin "games/tla/src/map/daila/shape_grid.plane.lz"
	.balign 4, 0
.LField_Daila_ANIMATION:
	.incbin "games/tla/src/map/daila_flooded/animation.script.lz"
	.balign 4, 0
.LField_Daila_SCENE:
	.incbin "games/tla/src/map/daila/scene.scene.lz"
