	.global Field_LacquerRoomsA
Field_LacquerRoomsA:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_CELL_PICTURES
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_SHAPES
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_GRID
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_ANIMATION
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_BLEND
	map_offset Field_LacquerRoomsA, .LField_LacquerRoomsA_MARKERS
.LField_LacquerRoomsA_CELL_PICTURES:
	.incbin "games/tbs/src/map/lacquer_rooms_a/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LacquerRoomsA_SHAPES:
	.incbin "games/tbs/src/map/lacquer_rooms_a/shapes.table.lz"
	.balign 4, 0
.LField_LacquerRoomsA_GRID:
	.incbin "games/tbs/src/map/lacquer_rooms_a/map.grid.lz"
	.balign 4, 0
.LField_LacquerRoomsA_ANIMATION:
	.incbin "games/tbs/src/map/lacquer_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_LacquerRoomsA_BLEND:
	.incbin "games/tbs/src/map/lacquer_rooms_a/blend.script.lz"
	.balign 4, 0
.LField_LacquerRoomsA_MARKERS:
	treasure_spot 6, 14, 100
	treasure_spot 12, 27, 101
	treasure_spot 27, 27, 102
	treasure_spots_end
	.global Tileset_LacquerRoomsAPalette
Tileset_LacquerRoomsAPalette:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_a/palette.gbapal.lz"
	.byte 0xa8, 0x80, 0x60	@ PACKER: left in the buffer by Field_LacquerRoomsA+0x1d9
	.global Tileset_LacquerRoomsATilesA
Tileset_LacquerRoomsATilesA:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_a/tiles_a.4bpp.mtf"
	.byte 0x13, 0x00	@ PACKER: left in the buffer by Field_FuchinTempleGrounds+0x24ee
	.global Tileset_LacquerRoomsATilesB
Tileset_LacquerRoomsATilesB:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_a/tiles_b.4bpp.mtf"
	.global Tileset_LacquerRoomsATilesC
Tileset_LacquerRoomsATilesC:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_a/tiles_c.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_LacquerRoomsATilesB+0x1573
	.global Tileset_LacquerRoomsATilesD
Tileset_LacquerRoomsATilesD:
	.incbin "games/tbs/src/graphics/tileset/lacquer_rooms_a/tiles_d.4bpp.mtf"
	.byte 0xfb, 0x14, 0x4d	@ PACKER: left in the buffer by Tileset_LacquerRoomsATilesC+0xb99
	.balign 4, 0
