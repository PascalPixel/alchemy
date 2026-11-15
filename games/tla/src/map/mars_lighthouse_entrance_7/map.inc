	.global Field_MarsLighthouseEntrance7
Field_MarsLighthouseEntrance7:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_CELL_PICTURES
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_SHAPES
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_GRID
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_SHAPE_GRID
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_ANIMATION
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_BLEND
	map_block Field_MarsLighthouseEntrance7, .LField_MarsLighthouseEntrance7_SCENE
.LField_MarsLighthouseEntrance7_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_7/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_7/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_7/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_2/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance7_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_7/scene.scene.lz"
	.balign 4, 0
