	.global Field_GabombaCatacombs4
Field_GabombaCatacombs4:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_CELL_PICTURES
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_SHAPES
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_GRID
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_SHAPE_GRID
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_ANIMATION
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_BLEND
	map_block Field_GabombaCatacombs4, .LField_GabombaCatacombs4_SCENE
.LField_GabombaCatacombs4_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_catacombs_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_4/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_4/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs4_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_4/scene.scene.lz"
	.balign 4, 0
