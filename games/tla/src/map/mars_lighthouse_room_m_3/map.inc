	.global Field_MarsLighthouseRoomM3
Field_MarsLighthouseRoomM3:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_CELL_PICTURES
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_SHAPES
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_GRID
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_SHAPE_GRID
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_ANIMATION
	map_block Field_MarsLighthouseRoomM3
	map_block Field_MarsLighthouseRoomM3, .LField_MarsLighthouseRoomM3_SCENE
.LField_MarsLighthouseRoomM3_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM3_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_3/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM3_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_3/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM3_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM3_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM3_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_3/scene.scene.lz"
	.balign 4, 0
