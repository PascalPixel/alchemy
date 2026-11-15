	.global Field_MarsLighthouseEntrance3
Field_MarsLighthouseEntrance3:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_CELL_PICTURES
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_SHAPES
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_GRID
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_SHAPE_GRID
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_ANIMATION
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_BLEND
	map_block Field_MarsLighthouseEntrance3, .LField_MarsLighthouseEntrance3_SCENE
.LField_MarsLighthouseEntrance3_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_3/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_3/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_2/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance3_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_3/scene.scene.lz"
	.balign 4, 0
