	.global Field_IceRooms
Field_IceRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_IceRooms, .LField_IceRooms_CELL_PICTURES
	map_offset Field_IceRooms, .LField_IceRooms_SHAPES
	map_offset Field_IceRooms, .LField_IceRooms_GRID
	map_offset Field_IceRooms, .LField_IceRooms_ANIMATION
	map_offset Field_IceRooms, .LField_IceRooms_BLEND
	map_offset Field_IceRooms, .LField_IceRooms_MARKERS
.LField_IceRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/ice_rooms/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_IceRooms_SHAPES:
	.incbin "games/tbs/src/map/ice_rooms/shapes.table.lz"
	.balign 4, 0
.LField_IceRooms_GRID:
	.incbin "games/tbs/src/map/ice_rooms/map.grid.lz"
	.balign 4, 0
.LField_IceRooms_ANIMATION:
	.incbin "games/tbs/src/map/ice_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_IceRooms_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_IceRooms_MARKERS:
	treasure_spot 7, 5, 200
	treasure_spot 8, 5, 200
	treasure_spot 7, 6, 200
	treasure_spot 8, 6, 200
	treasure_spot 4, 7, 100
	treasure_spot 21, 34, 101
	treasure_spot 15, 46, 102
	treasure_spot 16, 46, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_IceRoomsPalette
Tileset_IceRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/ice_rooms/palette.gbapal.lz"
	.byte 0x1f	@ PACKER: left in the buffer by Field_IceRooms+0x1c7
	.global Tileset_IceRoomsTilesA
Tileset_IceRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/ice_rooms/tiles_a.4bpp.mtf"
	.byte 0x47, 0x90, 0x0e	@ PACKER: left in the buffer by Tileset_BilibinCaveIceTilesA+0x2901
	.global Tileset_IceRoomsTilesB
Tileset_IceRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/ice_rooms/tiles_b.4bpp.mtf"
	.global Tileset_IceRoomsTilesC
Tileset_IceRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/ice_rooms/tiles_c.4bpp.mtf"
	.byte 0xc3, 0x73, 0x89	@ PACKER: left in the buffer by Tileset_IceRoomsTilesB+0x1c59
	.global Tileset_IceRoomsTilesD
Tileset_IceRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/ice_rooms/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x83	@ PACKER: left in the buffer by Tileset_IceRoomsTilesC+0x156e
	.balign 4, 0
