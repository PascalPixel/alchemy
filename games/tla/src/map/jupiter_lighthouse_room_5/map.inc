	.global Field_JupiterLighthouseRoom5
Field_JupiterLighthouseRoom5:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_CELL_PICTURES
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_SHAPES
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_GRID
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_SHAPE_GRID
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_ANIMATION
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_BLEND
	map_block Field_JupiterLighthouseRoom5, .LField_JupiterLighthouseRoom5_SCENE
.LField_JupiterLighthouseRoom5_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_ANIMATION:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_BLEND:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/blend.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom5_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_5/scene.scene.lz"
	.balign 4, 0
