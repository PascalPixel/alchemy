	.global Field_CottageRoomsB
Field_CottageRoomsB:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_CELL_PICTURES
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_SHAPES
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_GRID
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_ANIMATION
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_BLEND
	map_offset Field_CottageRoomsB, .LField_CottageRoomsB_MARKERS
.LField_CottageRoomsB_CELL_PICTURES:
	.incbin "games/tbs/src/map/cottage_rooms_b/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_CottageRoomsB_SHAPES:
	.incbin "games/tbs/src/map/cottage_rooms_b/shapes.table.lz"
	.balign 4, 0
.LField_CottageRoomsB_GRID:
	.incbin "games/tbs/src/map/cottage_rooms_b/map.grid.lz"
	.balign 4, 0
.LField_CottageRoomsB_ANIMATION:
	.incbin "games/tbs/src/map/cottage_rooms_b/tileset_anims.script.lz"
	.balign 4, 0
.LField_CottageRoomsB_BLEND:
	.incbin "games/tbs/src/map/cottage_rooms_a/blend.script.lz"
	.balign 4, 0
.LField_CottageRoomsB_MARKERS:
	treasure_spot 20, 3, 206
	treasure_spot 21, 3, 206
	treasure_spot 24, 3, 207
	treasure_spot 25, 3, 207
	treasure_spot 26, 3, 207
	treasure_spot 27, 3, 207
	treasure_spot 28, 3, 208
	treasure_spot 29, 3, 208
	treasure_spot 30, 3, 208
	treasure_spot 40, 3, 108
	treasure_spot 20, 4, 206
	treasure_spot 21, 4, 206
	treasure_spot 47, 5, 107
	treasure_spot 42, 6, 109
	treasure_spot 26, 8, 101
	treasure_spot 31, 8, 209
	treasure_spot 32, 8, 209
	treasure_spot 4, 12, 100
	treasure_spot 49, 19, 207
	treasure_spot 50, 19, 207
	treasure_spot 52, 19, 208
	treasure_spot 53, 19, 208
	treasure_spot 55, 20, 209
	treasure_spot 56, 20, 209
	treasure_spot 55, 21, 209
	treasure_spot 56, 21, 209
	treasure_spot 28, 22, 210
	treasure_spot 29, 22, 210
	treasure_spot 30, 22, 210
	treasure_spot 3, 23, 212
	treasure_spot 4, 23, 212
	treasure_spot 54, 24, 103
	treasure_spot 9, 40, 102
	treasure_spot 9, 41, 102
	treasure_spot 31, 41, 211
	treasure_spot 32, 41, 211
	treasure_spots_end
	.balign 4, 0
	.global Tileset_CottageRoomsBPalette
Tileset_CottageRoomsBPalette:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_b/palette.gbapal.lz"
	.byte 0x64, 0x36, 0x80	@ PACKER: left in the buffer by Field_CottageRoomsB+0x18d
	.global Tileset_CottageRoomsBTilesA
Tileset_CottageRoomsBTilesA:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_b/tiles_a.4bpp.mtf"
	.byte 0x86, 0x47	@ PACKER: left in the buffer by Field_CottageRoomsB+0x1dae
	.global Tileset_CottageRoomsBTilesB
Tileset_CottageRoomsBTilesB:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_b/tiles_b.4bpp.mtf"
	.global Tileset_CottageRoomsBTilesC
Tileset_CottageRoomsBTilesC:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_b/tiles_c.4bpp.mtf"
	.byte 0x38, 0xf0, 0x3d	@ PACKER: left in the buffer by Tileset_CottageRoomsBTilesB+0x1609
	.global Tileset_CottageRoomsBTilesD
Tileset_CottageRoomsBTilesD:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_b/tiles_d.4bpp.mtf"
	.byte 0x7e	@ PACKER: left in the buffer by Tileset_CottageRoomsBTilesC+0x150b
	.balign 4, 0
