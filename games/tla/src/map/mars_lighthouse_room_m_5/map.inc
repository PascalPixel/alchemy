	.global Field_MarsLighthouseRoomM5
Field_MarsLighthouseRoomM5:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_CELL_PICTURES
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_SHAPES
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_GRID
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_SHAPE_GRID
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_ANIMATION
	map_block Field_MarsLighthouseRoomM5
	map_block Field_MarsLighthouseRoomM5, .LField_MarsLighthouseRoomM5_SCENE
.LField_MarsLighthouseRoomM5_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_5/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM5_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_5/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM5_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_5/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM5_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM5_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomM5_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_5/scene.scene.lz"
	.global Tileset_PaletteMarsLighthouseRoomM5A
Tileset_PaletteMarsLighthouseRoomM5A:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_a.gbapal.lz"
	.byte 0x95, 0xe2, 0xb5	@ PACKER: left in the buffer by Field_MarsLighthouseRoomM5+0x129
	.global Tileset_PaletteMarsLighthouseRoomM5B
Tileset_PaletteMarsLighthouseRoomM5B:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_b.gbapal.lz"
	.byte 0x81	@ PACKER: left in the buffer by Field_MarsLighthouseRoomM5+0x177
	.global Tileset_PaletteMarsLighthouseRoomM5C
Tileset_PaletteMarsLighthouseRoomM5C:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_c.gbapal.lz"
	.byte 0x00, 0x81	@ PACKER: left in the buffer by Tileset_PaletteMarsLighthouseRoomM5B+0x176
	.global Tileset_PaletteMarsLighthouseRoomM5D
Tileset_PaletteMarsLighthouseRoomM5D:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_d.gbapal.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_PaletteMarsLighthouseRoomM5E
Tileset_PaletteMarsLighthouseRoomM5E:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_e.gbapal.lz"
	.byte 0x17	@ PACKER: left in the buffer by Tileset_PaletteMarsLighthouseRoomM5D+0x18f
