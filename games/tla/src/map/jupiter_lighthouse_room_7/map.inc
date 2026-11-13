	.global Field_JupiterLighthouseRoom7
Field_JupiterLighthouseRoom7:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_CELL_PICTURES
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_SHAPES
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_GRID
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_SHAPE_GRID
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_ANIMATION
	map_block Field_JupiterLighthouseRoom7
	map_block Field_JupiterLighthouseRoom7, .LField_JupiterLighthouseRoom7_SCENE
.LField_JupiterLighthouseRoom7_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_7/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom7_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_7/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom7_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_7/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom7_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom7_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom7_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_7/scene.scene.lz"
	.balign 4, 0
