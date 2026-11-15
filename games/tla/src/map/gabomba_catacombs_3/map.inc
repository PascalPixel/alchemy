	.global Field_GabombaCatacombs3
Field_GabombaCatacombs3:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_CELL_PICTURES
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_SHAPES
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_GRID
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_SHAPE_GRID
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_ANIMATION
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_BLEND
	map_block Field_GabombaCatacombs3, .LField_GabombaCatacombs3_SCENE
.LField_GabombaCatacombs3_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_catacombs_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_3/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_3/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs3_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_3/scene.scene.lz"
	.balign 4, 0
