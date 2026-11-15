	.global Field_MarsLighthouseRoomF3
Field_MarsLighthouseRoomF3:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_CELL_PICTURES
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_SHAPES
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_GRID
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_SHAPE_GRID
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_ANIMATION
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_BLEND
	map_block Field_MarsLighthouseRoomF3, .LField_MarsLighthouseRoomF3_SCENE
.LField_MarsLighthouseRoomF3_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF3_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF3_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseRoomF3_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseRoomF3_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF3_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_2/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF3_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_room_f_3/scene.scene.lz"
	.endif
	.balign 4, 0
