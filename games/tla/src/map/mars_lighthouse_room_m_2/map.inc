	.global Field_MarsLighthouseRoomM2
Field_MarsLighthouseRoomM2:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_CELL_PICTURES
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_SHAPES
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_GRID
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_SHAPE_GRID
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_ANIMATION
	map_block Field_MarsLighthouseRoomM2
	map_block Field_MarsLighthouseRoomM2, .LField_MarsLighthouseRoomM2_SCENE
.LField_MarsLighthouseRoomM2_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM2_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_2/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM2_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_2/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM2_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM2_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM2_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_2/scene.scene.lz"
	.global Tileset_MarsLighthouseRoomM2TilesD
Tileset_MarsLighthouseRoomM2TilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_2/tiles_d.4bpp.mtf"
	.byte 0x02, 0x19, 0xdd	@ PACKER: left in the buffer by Field_MarsLighthouseRoomM2+0x7cd
