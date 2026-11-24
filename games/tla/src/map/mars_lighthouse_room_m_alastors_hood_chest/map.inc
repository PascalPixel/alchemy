	.global Field_MarsLighthouseRoomMAlastorsHoodChest
Field_MarsLighthouseRoomMAlastorsHoodChest:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_CELL_TILES
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_SHAPES
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_GRID
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_SHAPE_GRID
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_ANIMATION
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest
	map_block Field_MarsLighthouseRoomMAlastorsHoodChest, .LField_MarsLighthouseRoomMAlastorsHoodChest_SCENE
.LField_MarsLighthouseRoomMAlastorsHoodChest_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_alastors_hood_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMAlastorsHoodChest_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_alastors_hood_chest/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMAlastorsHoodChest_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_alastors_hood_chest/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMAlastorsHoodChest_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_alastors_hood_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMAlastorsHoodChest_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMAlastorsHoodChest_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_alastors_hood_chest/scene.scene.lz"
	.balign 4, 0
