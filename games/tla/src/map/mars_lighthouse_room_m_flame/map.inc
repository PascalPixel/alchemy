	.global Field_MarsLighthouseRoomMFlame
Field_MarsLighthouseRoomMFlame:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_CELL_TILES
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_SHAPES
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_GRID
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_SHAPE_GRID
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_ANIMATION
	map_block Field_MarsLighthouseRoomMFlame
	map_block Field_MarsLighthouseRoomMFlame, .LField_MarsLighthouseRoomMFlame_SCENE
.LField_MarsLighthouseRoomMFlame_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMFlame_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMFlame_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMFlame_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMFlame_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMFlame_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_flame/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseRoomMFlameTilesD
Tileset_MarsLighthouseRoomMFlameTilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_flame/tiles_d.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_MarsLighthouseRoomMFlame+0xac2
