	.global Field_JupiterLighthouseRoom8
Field_JupiterLighthouseRoom8:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_CELL_PICTURES
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_SHAPES
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_GRID
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_SHAPE_GRID
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_ANIMATION
	map_block Field_JupiterLighthouseRoom8
	map_block Field_JupiterLighthouseRoom8, .LField_JupiterLighthouseRoom8_SCENE
.LField_JupiterLighthouseRoom8_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_8/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom8_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_8/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom8_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_8/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom8_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom8_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom8_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_8/scene.scene.lz"
	.balign 4, 0
