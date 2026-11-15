	.global Field_MarsLighthouseRoomM6
Field_MarsLighthouseRoomM6:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_CELL_PICTURES
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_SHAPES
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_GRID
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_SHAPE_GRID
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_ANIMATION
	map_block Field_MarsLighthouseRoomM6
	map_block Field_MarsLighthouseRoomM6, .LField_MarsLighthouseRoomM6_SCENE
.LField_MarsLighthouseRoomM6_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM6_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM6_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM6_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM6_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM6_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_6/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseRoomM6TilesD
Tileset_MarsLighthouseRoomM6TilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_6/tiles_d.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_MarsLighthouseRoomM6+0xac2
