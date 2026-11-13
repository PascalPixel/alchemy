	.global Field_LogRoomsC
Field_LogRoomsC:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LogRoomsC, .LField_LogRoomsC_CELL_PICTURES
	map_offset Field_LogRoomsC, .LField_LogRoomsC_SHAPES
	map_offset Field_LogRoomsC, .LField_LogRoomsC_GRID
	map_offset Field_LogRoomsC, .LField_LogRoomsC_ANIMATION
	map_offset Field_LogRoomsC, .LField_LogRoomsC_BLEND
	map_offset Field_LogRoomsC, .LField_LogRoomsC_MARKERS
.LField_LogRoomsC_CELL_PICTURES:
	.incbin "games/tbs/src/map/log_rooms_c/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_LogRoomsC_SHAPES:
	.incbin "games/tbs/src/map/log_rooms_c/shapes.table.lz"
	.balign 4, 0
.LField_LogRoomsC_GRID:
	.incbin "games/tbs/src/map/log_rooms_c/map.grid.lz"
	.balign 4, 0
.LField_LogRoomsC_ANIMATION:
	.incbin "games/tbs/src/map/vault_log_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_LogRoomsC_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_LogRoomsC_MARKERS:
	treasure_spot 5, 7, 200
	treasure_spot 6, 7, 200
	treasure_spot 4, 23, 100
	treasure_spot 25, 26, 201
	treasure_spot 26, 26, 201
	treasure_spot 27, 26, 201
	treasure_spot 24, 37, 202
	treasure_spot 25, 37, 202
	treasure_spot 26, 37, 202
	treasure_spot 22, 39, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_LogRoomsCPalette
Tileset_LogRoomsCPalette:
	.incbin "games/tbs/src/graphics/tileset/log_rooms_c/palette.gbapal.lz"
	.byte 0x8e, 0x9e, 0x21	@ PACKER: left in the buffer by Field_LogRoomsC+0x1a1
	.global Tileset_LogRoomsCTilesA
Tileset_LogRoomsCTilesA:
	.incbin "games/tbs/src/graphics/tileset/log_rooms_c/tiles_a.4bpp.mtf"
	.byte 0x21, 0x2e	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x1902
	.balign 4, 0
