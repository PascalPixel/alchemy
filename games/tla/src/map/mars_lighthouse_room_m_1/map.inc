	.global Field_MarsLighthouseRoomM1
Field_MarsLighthouseRoomM1:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_CELL_PICTURES
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_SHAPES
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_GRID
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_SHAPE_GRID
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_ANIMATION
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_BLEND
	map_block Field_MarsLighthouseRoomM1, .LField_MarsLighthouseRoomM1_SCENE
.LField_MarsLighthouseRoomM1_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_1/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_1/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_2/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM1_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseRoomM1Palette
Tileset_MarsLighthouseRoomM1Palette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_1/palette.gbapal.lz"
	.byte 0x05, 0x18	@ PACKER: left in the buffer by Field_MarsLighthouseRoomM1+0x1b2
	.global Tileset_MarsLighthouseRoomM1TilesA
Tileset_MarsLighthouseRoomM1TilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_1/tiles_a.4bpp.mtf"
	.byte 0x23, 0x44, 0x28	@ PACKER: left in the buffer by Tileset_MarsLighthouseAerieTilesA+0x223d
	.global Tileset_MarsLighthouseRoomM1TilesB
Tileset_MarsLighthouseRoomM1TilesB:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_1/tiles_b.4bpp.mtf"
	.byte 0x8e	@ PACKER: left in the buffer by Tileset_MarsLighthouseRoomM1TilesA+0xb23
