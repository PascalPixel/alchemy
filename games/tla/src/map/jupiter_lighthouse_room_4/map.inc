	.global Field_JupiterLighthouseRoom4
Field_JupiterLighthouseRoom4:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_CELL_PICTURES
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_SHAPES
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_GRID
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_SHAPE_GRID
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_ANIMATION
	map_block Field_JupiterLighthouseRoom4
	map_block Field_JupiterLighthouseRoom4, .LField_JupiterLighthouseRoom4_SCENE
.LField_JupiterLighthouseRoom4_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom4_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom4_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom4_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom4_ANIMATION:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom4_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_4/scene.scene.lz"
	.balign 4, 0
