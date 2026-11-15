	.global Field_GabombaCatacombs5
Field_GabombaCatacombs5:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_CELL_PICTURES
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_SHAPES
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_GRID
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_SHAPE_GRID
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_ANIMATION
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_BLEND
	map_block Field_GabombaCatacombs5, .LField_GabombaCatacombs5_SCENE
.LField_GabombaCatacombs5_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_catacombs_5/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_5/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_5/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_ANIMATION:
	.incbin "games/tla/src/map/gabomba_catacombs_5/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs5_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_5/scene.scene.lz"
