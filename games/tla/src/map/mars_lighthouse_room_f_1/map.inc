	.global Field_MarsLighthouseRoomF1
Field_MarsLighthouseRoomF1:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_CELL_PICTURES
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_SHAPES
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_GRID
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_SHAPE_GRID
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_ANIMATION
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_BLEND
	map_block Field_MarsLighthouseRoomF1, .LField_MarsLighthouseRoomF1_SCENE
.LField_MarsLighthouseRoomF1_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_BLEND:
	.incbin "games/tla/src/map/venus_lighthouse/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomF1_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_f_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseRoomF1Palette
Tileset_MarsLighthouseRoomF1Palette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_f_1/palette.gbapal.lz"
	.byte 0x14, 0x8f	@ PACKER: left in the buffer by Field_MarsLighthouseRoomF1+0x19a
	.global Tileset_MarsLighthouseRoomF1TilesA
Tileset_MarsLighthouseRoomF1TilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_f_1/tiles_a.4bpp.mtf"
	.byte 0x10, 0x00	@ PACKER: left in the buffer by Field_Prox4+0x22aa
	.global Tileset_MarsLighthouseRoomF1TilesB
Tileset_MarsLighthouseRoomF1TilesB:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_f_1/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x68, 0x02	@ PACKER: left in the buffer by Tileset_MarsLighthouseRoomF1TilesA+0x21c5
	.global Tileset_MarsLighthouseRoomF1TilesC
Tileset_MarsLighthouseRoomF1TilesC:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_f_1/tiles_c.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_MarsLighthouseRoomF1TilesB+0x16fb
	.global Tileset_MarsLighthouseRoomF1TilesD
Tileset_MarsLighthouseRoomF1TilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_f_1/tiles_d.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_MarsLighthouseRoomF1TilesC+0xb23
