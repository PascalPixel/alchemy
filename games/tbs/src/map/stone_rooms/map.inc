	.global Field_StoneRooms
Field_StoneRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_StoneRooms, .LField_StoneRooms_CELL_PICTURES
	map_offset Field_StoneRooms, .LField_StoneRooms_SHAPES
	map_offset Field_StoneRooms, .LField_StoneRooms_GRID
	map_offset Field_StoneRooms, .LField_StoneRooms_ANIMATION
	map_offset Field_StoneRooms, .LField_StoneRooms_BLEND
	map_offset Field_StoneRooms, .LField_StoneRooms_MARKERS
.LField_StoneRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/stone_rooms/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_StoneRooms_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_StoneRooms_GRID:
	.incbin "games/tbs/src/map/stone_rooms/map.grid.lz"
	.balign 4, 0
.LField_StoneRooms_ANIMATION:
	.incbin "games/tbs/src/map/stone_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_StoneRooms_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_statue/blend.script.lz"
	.balign 4, 0
.LField_StoneRooms_MARKERS:
	treasure_spot 32, 7, 100
	treasure_spot 33, 7, 101
	treasure_spot 35, 7, 102
	treasure_spot 36, 7, 103
	treasure_spot 33, 8, 104
	treasure_spot 35, 8, 105
	treasure_spots_end
	.balign 4, 0
	.global Tileset_StoneRoomsPalette
Tileset_StoneRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/stone_rooms/palette.gbapal.lz"
	.byte 0x3f	@ PACKER: left in the buffer by Field_StoneRooms+0xf3
	.global Tileset_StoneRoomsTilesA
Tileset_StoneRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/stone_rooms/tiles_a.4bpp.mtf"
	.global Tileset_StoneRoomsTilesB
Tileset_StoneRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/stone_rooms/tiles_b.4bpp.mtf"
	.global Tileset_StoneRoomsTilesC
Tileset_StoneRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/stone_rooms/tiles_c.4bpp.mtf"
	.byte 0x1d, 0x65	@ PACKER: left in the buffer by Tileset_StoneRoomsTilesB+0x186
	.global Tileset_StoneRoomsTilesD
Tileset_StoneRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/stone_rooms/tiles_d.4bpp.mtf"
	.byte 0x82, 0xb0	@ PACKER: left in the buffer by Tileset_StoneRoomsTilesA+0xda2
	.balign 4, 0
