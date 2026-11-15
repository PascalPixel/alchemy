	.global Field_MarsLighthouseEntrance6
Field_MarsLighthouseEntrance6:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_CELL_PICTURES
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_SHAPES
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_GRID
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_SHAPE_GRID
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_ANIMATION
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_BLEND
	map_block Field_MarsLighthouseEntrance6, .LField_MarsLighthouseEntrance6_SCENE
.LField_MarsLighthouseEntrance6_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance6_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance6_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntrance6_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntrance6_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance6_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_2/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntrance6_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_6/scene.scene.lz"
	.endif
	.balign 4, 0
