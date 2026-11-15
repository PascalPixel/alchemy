	.global Field_MarsLighthouseVenusLedge
Field_MarsLighthouseVenusLedge:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_CELL_PICTURES
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SHAPES
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_GRID
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SHAPE_GRID
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_ANIMATION
	map_block Field_MarsLighthouseVenusLedge
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SCENE
.LField_MarsLighthouseVenusLedge_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/scene.scene.lz"
	.global Tileset_PaletteMarsLighthouseRoomM5A
Tileset_PaletteMarsLighthouseRoomM5A:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_a.gbapal.lz"
	.byte 0x95, 0xe2, 0xb5	@ PACKER: left in the buffer by Field_MarsLighthouseVenusLedge+0x129
	.global Tileset_PaletteMarsLighthouseRoomM5B
Tileset_PaletteMarsLighthouseRoomM5B:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_room_m_5_b.gbapal.lz"
	.byte 0x81	@ PACKER: left in the buffer by Field_MarsLighthouseVenusLedge+0x177
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
