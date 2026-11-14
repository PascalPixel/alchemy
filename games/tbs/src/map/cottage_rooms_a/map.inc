	.global Field_CottageRoomsA
Field_CottageRoomsA:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_CELL_PICTURES
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_SHAPES
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_GRID
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_ANIMATION
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_BLEND
	map_offset Field_CottageRoomsA, .LField_CottageRoomsA_MARKERS
.LField_CottageRoomsA_CELL_PICTURES:
	.incbin "games/tbs/src/map/cottage_rooms_a/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_CottageRoomsA_SHAPES:
	.incbin "games/tbs/src/map/cottage_rooms_a/shapes.table.lz"
	.balign 4, 0
.LField_CottageRoomsA_GRID:
	.incbin "games/tbs/src/map/cottage_rooms_a/map.grid.lz"
	.balign 4, 0
.LField_CottageRoomsA_ANIMATION:
	.incbin "games/tbs/src/map/cottage_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_CottageRoomsA_BLEND:
	.incbin "games/tbs/src/map/cottage_rooms_a/blend.script.lz"
	.balign 4, 0
.LField_CottageRoomsA_MARKERS:
	treasure_spot 5, 3, 200
	treasure_spot 6, 3, 200
	treasure_spot 4, 4, 200
	treasure_spot 5, 4, 200
	treasure_spot 6, 4, 200
	treasure_spot 22, 5, 204
	treasure_spot 23, 5, 204
	treasure_spot 24, 5, 204
	treasure_spot 43, 5, 202
	treasure_spot 44, 5, 202
	treasure_spot 45, 5, 202
	treasure_spot 26, 11, 100
	treasure_spot 46, 25, 205
	treasure_spot 47, 25, 205
	treasure_spot 29, 29, 101
	treasure_spot 26, 43, 203
	treasure_spot 27, 43, 203
	treasure_spot 28, 43, 203
	treasure_spot 49, 45, 201
	treasure_spot 50, 45, 201
	treasure_spot 39, 50, 104
	treasure_spot 27, 52, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_CottageRoomsAPalette
Tileset_CottageRoomsAPalette:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_a/palette.gbapal.lz"
	.global Tileset_CottageRoomsATilesA
Tileset_CottageRoomsATilesA:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_a/tiles_a.4bpp.mtf"
	.global Tileset_CottageRoomsATilesB
Tileset_CottageRoomsATilesB:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_a/tiles_b.4bpp.mtf"
	.byte 0x02, 0xf6, 0x07	@ PACKER: left in the buffer by Field_CottageRoomsA+0x253d
	.global Tileset_CottageRoomsATilesC
Tileset_CottageRoomsATilesC:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_a/tiles_c.4bpp.mtf"
	.byte 0x86, 0xb4	@ PACKER: left in the buffer by Tileset_CottageRoomsATilesB+0x15fa
	.global Tileset_CottageRoomsATilesD
Tileset_CottageRoomsATilesD:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_a/tiles_d.4bpp.mtf"
	.byte 0x3c	@ PACKER: left in the buffer by Tileset_CottageRoomsATilesC+0x12c7
	.balign 4, 0
