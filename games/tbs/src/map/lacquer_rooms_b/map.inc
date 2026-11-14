	.global Field_LacquerRoomsB
Field_LacquerRoomsB:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_CELL_PICTURES
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_SHAPES
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_GRID
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_ANIMATION
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_BLEND
	map_offset Field_LacquerRoomsB, .LField_LacquerRoomsB_MARKERS
.LField_LacquerRoomsB_CELL_PICTURES:
	.incbin "games/tbs/src/map/lacquer_rooms_b/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LacquerRoomsB_SHAPES:
	.incbin "games/tbs/src/map/lacquer_rooms_b/shapes.table.lz"
	.balign 4, 0
.LField_LacquerRoomsB_GRID:
	.incbin "games/tbs/src/map/lacquer_rooms_b/map.grid.lz"
	.balign 4, 0
.LField_LacquerRoomsB_ANIMATION:
	.incbin "games/tbs/src/map/lacquer_rooms_b/tileset_anims.script.lz"
	.balign 4, 0
.LField_LacquerRoomsB_BLEND:
	.incbin "games/tbs/src/map/lacquer_rooms_b/blend.script.lz"
	.balign 4, 0
.LField_LacquerRoomsB_MARKERS:
	treasure_spot 5, 3, 100
	treasure_spot 7, 3, 200
	treasure_spot 8, 3, 200
	treasure_spot 26, 3, 201
	treasure_spot 27, 3, 201
	treasure_spot 42, 3, 202
	treasure_spot 43, 3, 202
	treasure_spot 26, 4, 201
	treasure_spot 27, 4, 201
	treasure_spot 45, 4, 206
	treasure_spot 23, 6, 205
	treasure_spot 24, 6, 205
	treasure_spot 24, 11, 101
	treasure_spot 18, 22, 203
	treasure_spot 18, 23, 203
	treasure_spot 18, 24, 204
	treasure_spot 18, 25, 204
	treasure_spot 27, 27, 207
	treasure_spots_end
	.balign 4, 0
	.global Tileset_LacquerRoomsBPalette
Tileset_LacquerRoomsBPalette:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_b/palette.gbapal.lz"
	.byte 0x98, 0xc9	@ PACKER: left in the buffer by Field_LacquerRoomsB+0x192
	.global Tileset_LacquerRoomsBTilesA
Tileset_LacquerRoomsBTilesA:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_b/tiles_a.4bpp.mtf"
	.byte 0x01, 0x6d	@ PACKER: left in the buffer by Field_LacquerRoomsB+0x150e
	.global Tileset_LacquerRoomsBTilesB
Tileset_LacquerRoomsBTilesB:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_b/tiles_b.4bpp.mtf"
	.byte 0xec	@ PACKER: left in the buffer by Field_LacquerRoomsB+0x1de3
	.global Tileset_LacquerRoomsBTilesC
Tileset_LacquerRoomsBTilesC:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_b/tiles_c.4bpp.mtf"
	.byte 0x2e, 0x1f	@ PACKER: left in the buffer by Tileset_LacquerRoomsBTilesB+0x1092
	.global Tileset_LacquerRoomsBTilesD
Tileset_LacquerRoomsBTilesD:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_b/tiles_d.4bpp.mtf"
	.balign 4, 0
